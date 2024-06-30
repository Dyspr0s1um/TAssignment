#ifndef user_table_H
#define user_table_H
#include "SqlObject.h"

namespace TriModel{
/**
 * @brief 
 */
class  user_table : public kcdz::SqlObject
{
    Q_OBJECT

    Q_PROPERTY(qint32 id READ id WRITE setid NOTIFY idChanged)
    Q_PROPERTY(QString user_name READ user_name WRITE setuser_name NOTIFY user_nameChanged)
    Q_PROPERTY(QString user_desc READ user_desc WRITE setuser_desc NOTIFY user_descChanged)
    Q_PROPERTY(QString user_pwd READ user_pwd WRITE setuser_pwd NOTIFY user_pwdChanged)
    Q_PROPERTY(QByteArray user_img READ user_img WRITE setuser_img NOTIFY user_imgChanged)
    Q_PROPERTY(qint32 user_score READ user_score WRITE setuser_score NOTIFY user_scoreChanged)
    Q_PROPERTY(qint32 user_level READ user_level WRITE setuser_level NOTIFY user_levelChanged)
    Q_PROPERTY(qint32 user_last_score READ user_last_score WRITE setuser_last_score NOTIFY user_last_scoreChanged)
    Q_PROPERTY(qint32 user_last_level READ user_last_level WRITE setuser_last_level NOTIFY user_last_levelChanged)
public:
    Q_INVOKABLE explicit user_table(QObject *parent = nullptr);
    Q_INVOKABLE user_table(const user_table &user_table);
    Q_INVOKABLE void operator=(user_table user_table);
    Q_INVOKABLE bool operator==(user_table &other);
    Q_INVOKABLE bool operator !=(user_table &other);
    
    Q_INVOKABLE qint32 id() const;
    Q_INVOKABLE void setid(const qint32& id);
    Q_INVOKABLE QString user_name() const;
    Q_INVOKABLE void setuser_name(const QString& user_name);
    Q_INVOKABLE QString user_desc() const;
    Q_INVOKABLE void setuser_desc(const QString& user_desc);
    Q_INVOKABLE QString user_pwd() const;
    Q_INVOKABLE void setuser_pwd(const QString& user_pwd);
    Q_INVOKABLE QByteArray user_img() const;
    Q_INVOKABLE void setuser_img(const QByteArray& user_img);
    Q_INVOKABLE qint32 user_score() const;
    Q_INVOKABLE void setuser_score(const qint32& user_score);
    Q_INVOKABLE qint32 user_level() const;
    Q_INVOKABLE void setuser_level(const qint32& user_level);
    Q_INVOKABLE qint32 user_last_score() const;
    Q_INVOKABLE void setuser_last_score(const qint32& user_last_score);
    Q_INVOKABLE qint32 user_last_level() const;
    Q_INVOKABLE void setuser_last_level(const qint32& user_last_level);
signals:

    void idChanged();
    void user_nameChanged();
    void user_descChanged();
    void user_pwdChanged();
    void user_imgChanged();
    void user_scoreChanged();
    void user_levelChanged();
    void user_last_scoreChanged();
    void user_last_levelChanged();
public slots:
private:
    void init();
    QStringList _primaKeys;
    
    qint32 m_id;
    QString m_user_name;
    QString m_user_desc;
    QString m_user_pwd;
    QByteArray m_user_img;
    qint32 m_user_score=0;
    qint32 m_user_level=0;
    qint32 m_user_last_score=0;
    qint32 m_user_last_level=0;
};
} //namespace TriModel
Q_DECLARE_METATYPE(TriModel::user_table);
#endif // user_table_H
