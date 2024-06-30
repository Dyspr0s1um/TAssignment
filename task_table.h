#ifndef task_table_H
#define task_table_H

#include "SqlObject.h"

namespace TriModel{
/**
 * @brief 
 */

enum TaskLevel{
    unjinji_unzhongyao=0,
    jinji_unzhongyao=1,
    unjinji_zhongyao=2,
    jinji_zhongyao=3
};

class  task_table : public kcdz::SqlObject
{
    Q_OBJECT

    Q_PROPERTY(qint32 id READ id WRITE setid NOTIFY idChanged)
    Q_PROPERTY(QString task_name READ task_name WRITE settask_name NOTIFY task_nameChanged)
    Q_PROPERTY(QString task_desc READ task_desc WRITE settask_desc NOTIFY task_descChanged)
    Q_PROPERTY(QString task_tm READ task_tm WRITE settask_tm NOTIFY task_tmChanged)
    Q_PROPERTY(qint32 task_score READ task_score WRITE settask_score NOTIFY task_scoreChanged)
    Q_PROPERTY(qint32 task_level READ task_level WRITE settask_level NOTIFY task_levelChanged)
    Q_PROPERTY(bool is_done READ is_done WRITE setis_done NOTIFY is_doneChanged)
public:
    Q_INVOKABLE explicit task_table(QObject *parent = nullptr);
    Q_INVOKABLE task_table(const task_table &task_table);
    Q_INVOKABLE void operator=(task_table task_table);
    Q_INVOKABLE bool operator==(task_table &other);
    Q_INVOKABLE bool operator !=(task_table &other);
    
    Q_INVOKABLE qint32 id() const;
    Q_INVOKABLE void setid(const qint32& id);
    Q_INVOKABLE QString task_name() const;
    Q_INVOKABLE void settask_name(const QString& task_name);
    Q_INVOKABLE QString task_desc() const;
    Q_INVOKABLE void settask_desc(const QString& task_desc);
    Q_INVOKABLE QString task_tm() const;
    Q_INVOKABLE void settask_tm(const QString& task_tm);
    Q_INVOKABLE qint32 task_score() const;
    Q_INVOKABLE void settask_score(const qint32& task_score);
    Q_INVOKABLE qint32 task_level() const;
    Q_INVOKABLE void settask_level(const qint32& task_level);
    Q_INVOKABLE bool is_done() const;
    Q_INVOKABLE void setis_done(const bool& is_done);
    bool checkState(){return _checkState; }
    void setCheckState(bool  state);
signals:
    void checkStateChanged();
    void idChanged();
    void task_nameChanged();
    void task_descChanged();
    void task_tmChanged();
    void task_scoreChanged();
    void task_levelChanged();
    void is_doneChanged();
public slots:
private:
    void init();
    QStringList _primaKeys;
    
    qint32 m_id;
    QString m_task_name;
    QString m_task_desc;
    QString m_task_tm;
    qint32 m_task_score=0;
    qint32 m_task_level=0;
    bool m_is_done=false;
    bool _checkState=false;
};
} //namespace TriModel
Q_DECLARE_METATYPE(TriModel::task_table);
#endif // task_table_H
