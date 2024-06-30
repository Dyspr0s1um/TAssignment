#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include <QObject>
#include <QVariantMap>
#include <QCoreApplication>
#include <QElapsedTimer>
#include <QDebug>

template<class M,class T>
class AbstractModelMgr
{
public:
    AbstractModelMgr(){
        _ptrModel=QSharedPointer<M>(new M);
        _ptrModel->moveToThread(QCoreApplication::instance()->thread());
    }

    M*  model(){return _ptrModel.get();}

    void appendModel(QObject *ptrObj){
        QVariantMap vm;
        vm[ptrObj->metaObject()->className()]=QVariant::fromValue(ptrObj);
          _ptrModel->append(vm);
    }

    void appendModels(const QList<T*>& ptrObjs){
        QVector<QVariantMap> data;
        foreach (auto it, ptrObjs) {
            QVariantMap vm;
            vm[it->metaObject()->className()]=QVariant::fromValue(it);
            data.append(vm);
        }
        _ptrModel->appends(data);
    }

    qint32 findItemIndex(qint32 itemId,M* listModel){
        QVariantList data=listModel->data();
        for (int i=0;i<data.count();i++) {
            T* ptr=data[i].toMap().first().value<T*>();
            if(itemId==ptr->id()) return i;
        }
        return -1;
    }

    qint32 findItemIndex(T* ptr,M* listModel){
        QVariantList data=listModel->data();
        for (int i=0;i<data.count();i++) {
            T* item=data[i].toMap().first().value<T*>();
            if(ptr==item) return i;
        }
        return -1;
    }

    T* findItemAndIndex(qint32& index,qint32 itemId,M* listModel){
        QVariantList data=listModel->data();
        for (int i=0;i<data.count();i++) {
            T* ptr=data[i].toMap().first().value<T*>();
            if(itemId==ptr->id()){
                index=i;
                return ptr;
            }
        }
        return nullptr;
    }

    T* findItem(qint32 itemId){
        auto& data=model()->allItems();
        foreach (auto& it, data) {
            T* ptr=it.first().value<T*>();
            if(itemId==ptr->id()) return ptr;
        }
        return nullptr;
    }

    QVariantList init(const QString& filter,const QString& connName="",bool append=false){
        QSharedPointer<T> ptr=QSharedPointer<T>(new T);
        if(!connName.isEmpty())ptr->setConnectName(connName);
        QVariantList list;
        if(filter.isEmpty()){
            ptr->getNextPage(list);
        }else if("ALL"==filter.toUpper()){
            list=ptr->selectAll();
        }else{
            list=ptr->select(filter);
        }
        if(!append) return list;
        if(list.isEmpty()) return list;

        QVariant var=list[0].toMap()[ptr->metaObject()->className()];
        QObject* ptrTemp=var.value<QObject*>();
        if(nullptr!=ptrTemp){
            foreach (auto& it, list) {
                QCoreApplication::processEvents();
                var=it.toMap()[ptr->metaObject()->className()];
                appendModel(var.value<QObject*>());
            }
        }else{
            foreach (auto& it, list) {
                QCoreApplication::processEvents();
                QVariantMap mp=it.toMap();
                T* item=new T();
                for(auto nt=mp.constBegin();nt!=mp.constEnd();++nt){
                    item->setProperty(nt.key().toStdString().c_str(),nt.value());
                }
                appendModel(item);
            }
        }
        return list;
    }

    void insertToModelByPos(QObject* ptrObj,qint32 pos){
        QVariantMap vm;
        vm[ptrObj->metaObject()->className()]=QVariant::fromValue(ptrObj);
        _ptrModel->insert(pos,vm);
    }

private:
    QSharedPointer<M> _ptrModel;
};



#endif // MODELMANAGER_H
