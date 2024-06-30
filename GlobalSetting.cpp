#include "GlobalSetting.h"
#include "qtextcodec.h"
#include "QHash"

GlobalSetting::GlobalSetting(QObject *parent)
    : QObject{parent}
{
    _ptrSetting=QSharedPointer<QSettings>(new QSettings("./systemConfig.ini",QSettings::IniFormat));
    _ptrSetting->setIniCodec(QTextCodec::codecForName("UTF-8"));
}



// void GlobalSetting::setPlatformName(const QVariant& value)
// {
//     setValue(QVariant::fromValue(value),"platformName","Setting");
// }


// QString GlobalSetting::getBacklight(const QString &defValue)
// {
//     return getValue<QString>(defValue, "backlightData","Setting");
// }


template<typename T>
T GlobalSetting::getValue(const T& value,const QString& key,const QString& group){
    std::lock_guard<std::mutex> lock(_mutex);
    T result=value;
    _ptrSetting->beginGroup(group);
    if(_ptrSetting->contains(key)){
        result =_ptrSetting->value(key,value).template value<T>();
    }else{
        _ptrSetting->setValue(key,value);
        _ptrSetting->sync();
    }
    _ptrSetting->endGroup();
    return result;
}

void GlobalSetting::setValue(const QVariant &value, const QString &key, const QString &group)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _ptrSetting->beginGroup(group);
    _ptrSetting->setValue(key,value);
    _ptrSetting->sync();
    _ptrSetting->endGroup();
}
