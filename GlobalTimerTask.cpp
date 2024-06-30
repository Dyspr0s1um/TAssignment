#include "GlobalTimerTask.h"
#include <atomic>
#include <QElapsedTimer>
#include <QDebug>

class GlobalTimerTask::Private{
public:
    std::atomic_bool _stop;
    QElapsedTimer _sysTimeUpdate1;
    QElapsedTimer _sysTimeUpdate2;
    QElapsedTimer _updateUserLevelTm;
    Private(){
        _sysTimeUpdate1.start();
        _sysTimeUpdate2.start();
        _updateUserLevelTm.start();
    }

};

GlobalTimerTask::GlobalTimerTask(QObject *parent)
    : QThread{parent}
{
    _p=QSharedPointer<Private>(new Private());
    _p->_stop=false;
}
void GlobalTimerTask::run()
{
    while (!_p->_stop) {
        QThread::msleep(100);

        if(_p->_sysTimeUpdate1.elapsed()>1000){
            _p->_sysTimeUpdate1.restart();
            emit checkUnJinJiZhongYao();
        }

        if(_p->_updateUserLevelTm.elapsed()>5000){
            _p->_updateUserLevelTm.restart();
            emit updateUserLevel();
        }
    }
}

void GlobalTimerTask::stop()
{
    _p->_stop=true;
}
