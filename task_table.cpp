#include "task_table.h"
using namespace TriModel;
task_table::task_table(QObject *parent) : kcdz::SqlObject(parent)
{
    this->setObjectName("id;");
    init();
}

task_table::task_table(const task_table &task_table):kcdz::SqlObject(task_table.parent())
{
    this->setObjectName(task_table.objectName());

    this->m_id=task_table.id();
    this->m_task_name=task_table.task_name();
    this->m_task_desc=task_table.task_desc();
    this->m_task_tm=task_table.task_tm();
    this->m_task_score=task_table.task_score();
    this->m_task_level=task_table.task_level();
    this->m_is_done=task_table.is_done();
    init();
}
void task_table::operator=(task_table task_table)
{
    this->setObjectName(task_table.objectName());
    
    this->m_id=task_table.id();
    this->m_task_name=task_table.task_name();
    this->m_task_desc=task_table.task_desc();
    this->m_task_tm=task_table.task_tm();
    this->m_task_score=task_table.task_score();
    this->m_task_level=task_table.task_level();
    this->m_is_done=task_table.is_done();
}

bool task_table::operator==(task_table &other)
{
    
    if(this->m_id!=other.id()) return false;
    if(this->m_task_name!=other.task_name()) return false;
    if(this->m_task_desc!=other.task_desc()) return false;
    if(this->m_task_tm!=other.task_tm()) return false;
    if(this->m_task_score!=other.task_score()) return false;
    if(this->m_task_level!=other.task_level()) return false;
    if(this->m_is_done!=other.is_done()) return false;
    return  true;
}
bool task_table::operator !=(task_table &other)
{
    return !(*this==other);
}
void task_table::init()
{
    setTable("task_table");
    setPrimaryKeys(QString("id;").split(";"));
    setDriver(kcdz::SqlObject::SQLite);
    setDatabaseName("task.db");
}

qint32 task_table::id() const 
{
    return m_id;
}

void task_table::setid(const qint32 &id)
{
    if(m_id==id) return;
    m_id=id;
    emit idChanged();
}

QString task_table::task_name() const 
{
    return m_task_name;
}

void task_table::settask_name(const QString &task_name)
{
    if(m_task_name==task_name) return;
    m_task_name=task_name;
    emit task_nameChanged();
}

QString task_table::task_desc() const 
{
    return m_task_desc;
}

void task_table::settask_desc(const QString &task_desc)
{
    if(m_task_desc==task_desc) return;
    m_task_desc=task_desc;
    emit task_descChanged();
}

QString task_table::task_tm() const 
{
    return m_task_tm;
}

void task_table::settask_tm(const QString &task_tm)
{
    if(m_task_tm==task_tm) return;
    m_task_tm=task_tm;
    emit task_tmChanged();
}

qint32 task_table::task_score() const 
{
    return m_task_score;
}

void task_table::settask_score(const qint32 &task_score)
{
    if(m_task_score==task_score) return;
    m_task_score=task_score;
    emit task_scoreChanged();
}

qint32 task_table::task_level() const 
{
    return m_task_level;
}

void task_table::settask_level(const qint32 &task_level)
{
    if(m_task_level==task_level) return;
    m_task_level=task_level;
    emit task_levelChanged();
}

bool task_table::is_done() const 
{
    return m_is_done;
}

void task_table::setis_done(const bool &is_done)
{
    if(m_is_done==is_done) return;
    m_is_done=is_done;
    emit is_doneChanged();
}

void task_table::setCheckState(bool state)
{
    if(_checkState==state) return;
    _checkState=state;
    Q_EMIT checkStateChanged();
}

