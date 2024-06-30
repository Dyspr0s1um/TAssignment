#ifndef GLOBALSETTING_H
#define GLOBALSETTING_H

#include <QObject>
#include "Singleholder.h"
#include <QSettings>
#include <QSharedPointer>
class GlobalSetting : public QObject,public PH::SingleHolder<GlobalSetting>
{
    Q_OBJECT
public:

    ///设备编号
   // Q_INVOKABLE QString getDeviceId(const QString& defValue="F34A4123456");
private:
    friend class PH::SingleHolder<GlobalSetting>;
    explicit GlobalSetting(QObject *parent = nullptr);
    QSharedPointer<QSettings> _ptrSetting;
    template<typename T>
    T getValue(const T& value,const QString& key,const QString& group="");
    void setValue(const QVariant& value,const QString& key,const QString& group="");
    QString getValue(const QString& key,const QString& group="");
    std::mutex _mutex;
};

#endif // GLOBALSETTING_H
