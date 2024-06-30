#include "SqlObject.h"
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QException>
#include <QDebug>
#include <QTimer>
#include <QDataStream>
#include <QBuffer>
#include <QCryptographicHash>
#include <QSharedPointer>
#include <QMetaObject>
#include <QMetaProperty>
#include <QDateTime>
#include <QDebug>
#include <QJsonDocument>
#include <QMutex>
#include <QCoreApplication>
#include <QThread>
#include <QUuid>

#define MSSQL_FIX_VALUE(VALUE) { \
switch ( static_cast<qint32>(VALUE.type()) ) { \
    case QVariant::DateTime: \
        VALUE = VALUE.toDateTime().toString(QStringLiteral("yyyyMMdd HH:mm:ss")); \
        break; \
    case QVariant::Date: \
        VALUE = VALUE.toDateTime().toString(QStringLiteral("yyyyMMdd")); \
        break; \
    case QVariant::Time: \
        VALUE = VALUE.toDateTime().toString(QStringLiteral("HH:mm:ss")); \
        break; \
} \
}

namespace kcdz {
class SqlObject::Private
{
public:
    QString table;
    QStringList primaryKeys;
    QVariant driver;

    QString driverStr;
    QString databaseName;
    QString databaseNameTemplate;
    QString host;
    qint32 port;
    QString userName;
    QString password;
    QString createQuery;
    QSqlError lastError;

    qint32 pageLimitSize=1000;
    qint32 pageIndex=1;
    QString connName;
    QStringList properties;
    static QHash<QString/*connect name*/, Core*> cores;
    QString key;
    bool isMainThread()
    {
        QThread *currentThread = QThread::currentThread();
        QThread *mainThread = QCoreApplication::instance()->thread();
        return currentThread == mainThread;
    }
};


class SqlObject::Core {
public:
    ~Core() {
    }
    static void remove(const QString& name){
        QMutexLocker locker(&mutex);
        if(objects.contains(name)){
            QSqlDatabase* db=objects[name]->_db;
            if (db->isOpen())
                db->close();

            delete db;
            delete objects[name];
            QSqlDatabase::removeDatabase(name);
        }
        objects.remove(name);
    }
    static QSqlDatabase& getDB(const QString& connName,SqlObject::Private* p){
        QMutexLocker locker(&mutex);
        if(!objects.contains(connName)) {
            init(connName,p);
        }
        return objects[connName]->get();
    }
    QString _connectionName;
    static QMutex mutex;
    static QHash<QString, Core*> objects;
private:
    QSqlDatabase &get() {
        return *_db;
    }
    static Core *getInstance(const QString &driver, const QString &connectionName) {
        Core *res = objects.value(connectionName);
        if(!res)
        {
            res = new Core(driver, connectionName);
            objects[connectionName] = res;
        }
        return res;
    }
    static void init(const QString &connName,SqlObject::Private* p)
    {
        Core* core=Core::getInstance(p->driverStr, connName);

        if (!core->_db->isOpen())
        {
            core->_db->setHostName(p->host);
            core->_db->setPort(p->port);
            core->_db->setUserName(p->userName);
            core->_db->setPassword(p->password);
            core->_db->setDatabaseName(p->databaseName);

            if (!core->_db->open()) {
                throw std::exception("db not open");
            }
        }
        // 启用 WAL 模式
        QSqlQuery walQuery(*core->_db);
        walQuery.exec("PRAGMA journal_mode=MEMORY;");

        SqlObject::Private::cores[connName] = core;
    }
    Core(const QString &driver, const QString &connectionName):
        _connectionName(connectionName)
    {
        *_db = QSqlDatabase::addDatabase(driver, connectionName);
        objects.insert(_connectionName, this);
    }
    QSqlDatabase *_db = new QSqlDatabase;
};


QHash<QString, SqlObject::Core*> SqlObject::Private::cores;
QHash<QString, SqlObject::Core*> SqlObject::Core::objects;
QMutex SqlObject::Core::mutex;

SqlObject::SqlObject(QObject *parent):
    QObject(parent)
{
    p = new Private;
}



void SqlObject::setDriver(const QVariant &driver)
{
    if (p->driver == driver)
        return;

    p->driver = driver;
    if (p->driver.type() == QVariant::String)
        p->driverStr = p->driver.toString();
    else
    {
        switch (p->driver.toInt())
        {
        case NoneDriver:
            p->driverStr = QStringLiteral("");
            break;

        case SQLite:
            p->driverStr = QStringLiteral("QSQLITE");
            break;

        case MySQL:
            p->driverStr = QStringLiteral("QMYSQL");
            break;

        case PostgreSQL:
            p->driverStr = QStringLiteral("QPSQL");
            break;

        case MSSQL:
            p->databaseNameTemplate = QStringLiteral( "%1;"
                                                     "SERVER=%HOSTADDRESS%;"
                                                     "DATABASE=%DATABASENAME%;"
                                                     "UID=%USERNAME%;"
                                                     "PWD=%PASSWORD%;");
#ifdef Q_OS_LINUX
            p->databaseNameTemplate = p->databaseNameTemplate.arg("DRIVER={ODBC Driver 17 for SQL Server}");
#else
            p->databaseNameTemplate = p->databaseNameTemplate.arg("DRIVER={SQL Server}");
#endif
            p->driverStr = QStringLiteral("QODBC");
            break;

        case ODBC:
            p->driverStr = QStringLiteral("QODBC");
            break;
        }
    }
    refreshKey();

    Q_EMIT driverChanged();
}

QVariant SqlObject::driver() const
{
    return p->driver;
}

void SqlObject::setTable(const QString &table)
{
    if (p->table == table)
        return;

    p->table = table;
    Q_EMIT tableChanged();
}

QString SqlObject::table() const
{
    return p->table;
}

void SqlObject::setPrimaryKeys(const QStringList &primaryKeys)
{
    if (p->primaryKeys == primaryKeys)
        return;

    p->primaryKeys = primaryKeys;
    Q_EMIT primaryKeysChanged();
}

QStringList SqlObject::primaryKeys() const
{
    return p->primaryKeys;
}

qint32 SqlObject::port() const
{
    return p->port;
}

void SqlObject::setPort(qint32 port)
{
    if (p->port == port)
        return;

    p->port = port;
    refreshKey();
    void portChanged();
}

QString SqlObject::host() const
{
    return p->host;
}

void SqlObject::setHost(const QString &host)
{
    if (p->host == host)
        return;

    p->host = host;
    refreshKey();
    void hostChanged();
}

QString SqlObject::databaseName() const
{
    return p->databaseName;
}

void SqlObject::setDatabaseName(const QString &databaseName)
{
    if (p->databaseName == databaseName)
        return;

    p->databaseName = databaseName;
    p->properties=properties();
    //refreshKey();
    void databaseNameChanged();
}

QString SqlObject::userName() const
{
    return p->userName;
}

void SqlObject::setUserName(const QString &userName)
{
    if (p->userName == userName)
        return;

    p->userName = userName;
    refreshKey();
    void userNameChanged();
}

QString SqlObject::password() const
{
    return p->password;
}

void SqlObject::setPassword(const QString &password)
{
    if (p->password == password)
        return;

    p->password = password;
    refreshKey();
    void passwordChanged();
}

QString SqlObject::createQuery() const
{
    return p->createQuery;
}

void SqlObject::setCreateQuery(const QString &createQuery)
{
    if (p->createQuery == createQuery)
        return;

    p->createQuery = createQuery;
    Q_EMIT createQueryChanged();
}

QSqlError SqlObject::lastError()
{
    return p->lastError;
}

QSqlDatabase SqlObject::getDataBase()
{
    QString connName=p->isMainThread()?"mainThread":p->connName;
    assert(!connName.isEmpty());
    return Core::getDB(connName,this->p);
}

qint32 SqlObject::getPageIndex()
{
    return p->pageIndex;
}
void SqlObject::getPage(int pageIndex, QVariantList& list){
    list=select("","",QString("order by id limit %1 offset %2").arg(p->pageLimitSize).arg(pageIndex-1),{});
    if(list.size()>0){
        p->pageIndex=pageIndex;
        qint32 total=getTotalPage();
        if(p->pageIndex>total)p->pageIndex=total;
    }
}

void SqlObject::getNextPage(QVariantList& list){
    p->pageIndex++;
    qint32 total=getTotalPage();
    if(p->pageIndex>total)p->pageIndex=total;
    list=select("","",QString("order by id limit %1 offset %2").arg(p->pageLimitSize).arg(p->pageIndex-1),{});
}
void SqlObject::getPrevPage(QVariantList& list){
    p->pageIndex-=1;
    if(getTotalPage() <= 0){
        if(p->pageIndex<0) p->pageIndex=0;
    }
    if(getTotalPage() >= 1){
        if(p->pageIndex<1) p->pageIndex=1;
    }
    list=select("","",QString("order by id limit %1 offset %2").arg(p->pageLimitSize).arg(p->pageIndex-1),{});
}
void SqlObject::transaction(){
    QSqlDatabase db=getDataBase();
    QSqlQuery query(db);
    query.exec("BEGIN TRANSACTION");
}
void SqlObject::commit(){
    QSqlDatabase db=getDataBase();
    QSqlQuery query(db);
    query.exec("COMMIT");
}
qint32 SqlObject::insert(const QString &extra)
{
    QStringList properties = p->properties;
    if(primaryKeys().contains(QStringLiteral("id"))){
        properties.removeAll(QStringLiteral("id"));
    }
    if(primaryKeys().contains(QStringLiteral("ID"))){
        properties.removeAll(QStringLiteral("ID"));
    }

    QString queryArgs = properties.join(QStringLiteral(", "));
    QString queryArgsBinds = QStringLiteral(":") + properties.join(QStringLiteral(", :"));

    QString query = QStringLiteral("INSERT  INTO ") + p->table + QStringLiteral(" (") + queryArgs + QStringLiteral(")") +
                    QStringLiteral(" VALUES (") + queryArgsBinds + QStringLiteral(")");

    QSqlDatabase db=getDataBase();
    QSqlQuery q(db);
    q.prepare(query + QStringLiteral(" ") + extra);
    for (const QString &key: properties)
    {
        QString bindKey=":"+key;
        if(!query.contains(bindKey)) continue;
        QVariant value = property(key.toUtf8());
        if (p->driver.toInt() == MSSQL)
            MSSQL_FIX_VALUE(value)
        q.bindValue(bindKey, value);
    }
    ushort insertCounter=0;
    while (!q.exec()&&insertCounter<100) {
        insertCounter++;
    }
    if(insertCounter>=100){
        db.rollback();
        qDebug()<<"==================start";
        qDebug() << "SQL:" << q.lastQuery();
        qDebug() << "Bound values:" << q.boundValues();
        qDebug()<<"==================end";
        qDebug()<<q.lastError();
        setLastError(q.lastError());
        return 0;
    }
    return q.lastInsertId().toInt();
}

bool SqlObject::update(const QString &_where, const QVariantMap &binds)
{
    QString query = "UPDATE " + p->table + " SET ";

    QStringList properties = p->properties;
    QStringList pkeys = primaryKeys();

    QString set;
    for (const QString &pr: properties)
    {
        if(pkeys.contains(pr)||pkeys.contains(pr.toLower())) continue;
        if (set.length())
            set += QStringLiteral(", ");
        set += (pr + "=:" + pr);
    }
    query += set + " WHERE " + _where+";";
    bool flag=false;
#ifdef QT_SQL_LIB
    QSqlDatabase db=getDataBase();
    QSqlQuery q(db);
    q.prepare(query);
    QVariantMap fixedBinds = prepareBinds(binds);
    QMapIterator<QString, QVariant> i(fixedBinds);

    while (i.hasNext())
    {
        i.next();
        if(!query.contains(i.key())) continue;
        q.bindValue(i.key(), i.value());
    }
    flag = queryExec(q,db);
    if(!flag) q.finish();
#endif
    return flag;
}

QVariantList SqlObject::selectAll()
{
    QString query = QStringLiteral("SELECT * FROM ")+ p->table;
    return SqlObject::query(query, {});
}

QVariantList SqlObject::select()
{
    QString where;
    QStringList properties = p->properties;
    QStringList pkeys = primaryKeys();
    for (const QString &pk: pkeys)
    {
        if (where.length())
            where += QStringLiteral(" AND ");
        where += (":" + pk + " = " + pk);
    }

    return select(QStringLiteral(""), where, QStringLiteral(""), {});
}

QVariantList SqlObject::select(const QString join, const QString &where, const QString &extra, const QVariantMap &binds)
{
    QString query = QStringLiteral("SELECT * FROM ") + p->table + (join.count()? QStringLiteral(" ") + join : QStringLiteral(""))
                    + (where.count()? QStringLiteral(" WHERE ") + where : QStringLiteral(""))
                    + (extra.count()? QStringLiteral(" ") + extra : QStringLiteral(""));

    return SqlObject::query(query, binds);
}

void SqlObject::fetch()
{
    QStringList properties = p->properties;
    QStringList pkeys = primaryKeys();
    QVariantList list = select();
    if (list.isEmpty())
        return;

    QVariantMap map = list.first().toMap();
    QMapIterator<QString, QVariant> i(map);
    while (i.hasNext())
    {
        i.next();
        const QString &key = i.key();
        if(!properties.contains(key) || pkeys.contains(key))
            continue;

        setProperty(key.toUtf8(), i.value());
    }
}

bool SqlObject::deleteQuery(const QString &where, const QVariantMap &binds)
{
    QString query = QStringLiteral("DELETE FROM ") + p->table + (where.count()? QStringLiteral(" WHERE ") + where : QStringLiteral(""));
    bool flag=false;
#ifdef QT_SQL_LIB
    QSqlDatabase db=getDataBase();

    QSqlQuery q(db);
    q.prepare(query);
    QVariantMap fixedBinds = prepareBinds(binds);
    QMapIterator<QString, QVariant> i(fixedBinds);

    while (i.hasNext())
    {
        i.next();
        if(!query.contains(i.key())) continue;
        q.bindValue(i.key(), i.value());
    }
    flag = queryExec(q,db);
    if(!flag) q.finish();
#endif
    return flag;
}

QVariantList SqlObject::query(const QString &query, const QVariantMap &binds)
{
    QVariantList res;
    QSqlDatabase db=getDataBase();
    QSqlQuery q(db);
    q.prepare(query);
    QVariantMap fixedBinds = prepareBinds(binds);
    QMapIterator<QString, QVariant> i(fixedBinds);

    while (i.hasNext())
    {
        i.next();
        if(!query.contains(i.key())) continue;
        q.bindValue(i.key(), i.value());
    }
    if (!queryExec(q,db)){
        q.finish();
        return QVariantList();
    }

    return generateResult(q);
}

qlonglong SqlObject::getTotalPage()
{
    QVariantList list=query("SELECT count(*) FROM " + p->table+";");
    if(list.size()==0) return 0;
    return list[0].toMap()["count(*)"].toLongLong()/p->pageLimitSize+1;
}

QVariantMap SqlObject::prepareBinds(const QVariantMap &binds) const
{
    QVariantMap res;

    QStringList properties = p->properties;
    for (const QString &key: properties)
    {
        QVariant value = property(key.toUtf8());
        if (p->driver.toInt() == MSSQL)
            MSSQL_FIX_VALUE(value)

        res[":" + key] = value;
    }

    QMapIterator<QString, QVariant> i(binds);
    while (i.hasNext())
    {
        i.next();
        QVariant value = i.value();
        if (p->driver.toInt() == MSSQL)
            MSSQL_FIX_VALUE(value)

        res[":" + i.key()] = value;
    }

    return res;
}

QVariantList SqlObject::generateResult(QSqlQuery &q)
{
    QVariantList res;
    while (q.next())
    {
        QSqlRecord r = q.record();
        const QMetaObject* metaObj=this->metaObject();
        QObject* ptr=metaObj->newInstance();
        if(ptr){
            foreach (const auto& kerStr, p->properties) {
                ptr->setProperty(kerStr.toStdString().c_str(),r.value(kerStr));
            }
            QVariantMap map;
            map[this->metaObject()->className()]=QVariant::fromValue(ptr);
            res << map;
        }else{
            QVariantMap map;
            for (qint32 i=0; i<r.count(); i++)
                map[r.fieldName(i)] = r.value(i);
            res << map;
        }
    }
    q.finish();
    return res;
}

void SqlObject::repairDatabase()
{
    QSqlQuery query(getDataBase());
    if (!query.exec("VACUUM")) {
        qDebug() << "VACUUM failed:" << query.lastError().text();
    } else {
        qDebug() << "VACUUM succeeded.";
    }

    // Perform integrity check
    if (!query.exec("PRAGMA integrity_check;")) {
        qDebug() << "Integrity check failed:" << query.lastError().text();
    } else {
        while (query.next()) {
            qDebug() << query.value(0).toString();
        }
    }
}

void SqlObject::create()
{
    QStringList queries = p->createQuery.split(QStringLiteral(";"), Qt::SkipEmptyParts);
    for (const QString &q: queries)
        SqlObject::query(q);
}

void SqlObject::drop()
{
    SqlObject::query("DROP TABLE " + p->table);
}

QVariantMap SqlObject::getData()
{
    QStringList properties = p->properties;
    if(primaryKeys().contains(QStringLiteral("id")) && property("id").type() == QVariant::Int && property("id").toInt() == 0)
        properties.removeAll(QStringLiteral("id"));

    QVariantMap res;
    for (const QString &key: properties)
    {
        QVariant value = property(key.toUtf8());
        if (p->driver.toInt() == MSSQL)
            MSSQL_FIX_VALUE(value)

        res[key] = value;
    }

    return res;
}

void SqlObject::setLastError( QSqlError lastError)
{
    p->lastError = lastError;
    Q_EMIT lastErrorChanged();
}


int SqlObject::queryExec(QSqlQuery &q,QSqlDatabase& db)
{
    if(!db.transaction()){
        setLastError(db.lastError());
        qWarning()<<"sql error ************"<<db.lastError().text();
        return false;
    }
    bool ret=false;
    ret = q.exec();
    if (!ret){
        setLastError(q.lastError());
        qDebug()<<"queryExec="<<ret<<q.lastError().text()<<q.lastQuery();
        db.rollback();
    }
    if(!db.commit()){
        qDebug()<<"commit failed."<<ret<<q.lastError().text()<<q.lastQuery();
        return false;
    }
    return ret;
}


QStringList SqlObject::properties() const
{
    QStringList res;
    const QMetaObject *meta = metaObject();
    for (int i=0; i<meta->propertyCount(); i++)
    {
        QMetaProperty property = meta->property(i);
        if(property.type()>QVariant::SizePolicy) continue;
        const QByteArray &propertyName = property.name();
        if (propertyName == "table" ||
            propertyName == "driver" ||
            propertyName == "databaseName" ||
            propertyName == "host" ||
            propertyName == "port" ||
            propertyName == "userName" ||
            propertyName == "password" ||
            propertyName == "createQuery" ||
            propertyName == "primaryKeys" ||
            propertyName == "lastError" ||
            propertyName == "objectName")
            continue;

        res<< QString::fromUtf8(propertyName);
    }

    return res;
}



void SqlObject::refreshKey()
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << p->host << p->port << p->driver
           << p->userName << p->password << p->databaseName
           << p->databaseNameTemplate;

    p->key = QString::fromUtf8(QCryptographicHash::hash(data, QCryptographicHash::Md5).toHex() );
}

SqlObject::~SqlObject()
{
}

void SqlObject::removeDB(const QString& name)
{
    Core::remove(name);
}

void SqlObject::setConnectName(const QString &connName)
{
    p->connName=connName;
}
}
