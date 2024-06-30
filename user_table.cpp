#include "user_table.h"
using namespace TriModel;
user_table::user_table(QObject *parent) : kcdz::SqlObject(parent)
{
    this->setObjectName("id;");
    init();
}

user_table::user_table(const user_table &user_table):kcdz::SqlObject(user_table.parent())
{
    this->setObjectName(user_table.objectName());

    this->m_id=user_table.id();
    this->m_user_name=user_table.user_name();
    this->m_user_desc=user_table.user_desc();
    this->m_user_pwd=user_table.user_pwd();
    this->m_user_img=user_table.user_img();
    this->m_user_score=user_table.user_score();
    this->m_user_level=user_table.user_level();
    this->m_user_last_score=user_table.user_last_score();
    this->m_user_last_level=user_table.user_last_level();
    init();
}
void user_table::operator=(user_table user_table)
{
    this->setObjectName(user_table.objectName());
    
    this->m_id=user_table.id();
    this->m_user_name=user_table.user_name();
    this->m_user_desc=user_table.user_desc();
    this->m_user_pwd=user_table.user_pwd();
    this->m_user_img=user_table.user_img();
    this->m_user_score=user_table.user_score();
    this->m_user_level=user_table.user_level();
    this->m_user_last_score=user_table.user_last_score();
    this->m_user_last_level=user_table.user_last_level();
}

bool user_table::operator==(user_table &other)
{
    if(this->m_id!=other.id()) return false;
    if(this->m_user_name!=other.user_name()) return false;
    if(this->m_user_desc!=other.user_desc()) return false;
    if(this->m_user_pwd!=other.user_pwd()) return false;
    if(this->m_user_img!=other.user_img()) return false;
    if(this->m_user_score!=other.user_score()) return false;
    if(this->m_user_level!=other.user_level()) return false;
    if(this->m_user_last_score!=other.user_last_score()) return false;
    if(this->m_user_last_level!=other.user_last_level()) return false;
    return  true;
}
bool user_table::operator !=(user_table &other)
{
    return !(*this==other);
}
void user_table::init()
{
    setTable("user_table");
    setPrimaryKeys(QString("id;").split(";"));
    setDriver(kcdz::SqlObject::SQLite);
    setDatabaseName("task.db");
}

qint32 user_table::id() const 
{
    return m_id;
}

void user_table::setid(const qint32 &id)
{
    if(m_id==id) return;
    m_id=id;
    emit idChanged();
}

QString user_table::user_name() const 
{
    return m_user_name;
}

void user_table::setuser_name(const QString &user_name)
{
    if(m_user_name==user_name) return;
    m_user_name=user_name;
    emit user_nameChanged();
}

QString user_table::user_desc() const 
{
    return m_user_desc;
}

void user_table::setuser_desc(const QString &user_desc)
{
    if(m_user_desc==user_desc) return;
    m_user_desc=user_desc;
    emit user_descChanged();
}

QString user_table::user_pwd() const 
{
    return m_user_pwd;
}

void user_table::setuser_pwd(const QString &user_pwd)
{
    if(m_user_pwd==user_pwd) return;
    m_user_pwd=user_pwd;
    emit user_pwdChanged();
}

QByteArray user_table::user_img() const 
{
    return m_user_img;
}

void user_table::setuser_img(const QByteArray &user_img)
{
    if(m_user_img==user_img) return;
    m_user_img=user_img;
    emit user_imgChanged();
}

qint32 user_table::user_score() const 
{
    return m_user_score;
}

void user_table::setuser_score(const qint32 &user_score)
{
    if(m_user_score==user_score) return;
    m_user_score=user_score;
    emit user_scoreChanged();
}

qint32 user_table::user_level() const 
{
    return m_user_level;
}

void user_table::setuser_level(const qint32 &user_level)
{
    if(m_user_level==user_level) return;
    m_user_level=user_level;
    emit user_levelChanged();
}

qint32 user_table::user_last_score() const 
{
    return m_user_last_score;
}

void user_table::setuser_last_score(const qint32 &user_last_score)
{
    if(m_user_last_score==user_last_score) return;
    m_user_last_score=user_last_score;
    emit user_last_scoreChanged();
}

qint32 user_table::user_last_level() const 
{
    return m_user_last_level;
}

void user_table::setuser_last_level(const qint32 &user_last_level)
{
    if(m_user_last_level==user_last_level) return;
    m_user_last_level=user_last_level;
    emit user_last_levelChanged();
}

