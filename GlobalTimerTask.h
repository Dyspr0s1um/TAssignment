#ifndef GLOBALTIMERTASK_H
#define GLOBALTIMERTASK_H

#include <QThread>
#include "Singleholder.h"
#include <QSharedPointer>
class GlobalTimerTask : public QThread,public PH::SingleHolder<GlobalTimerTask>
{
    Q_OBJECT
public:
    void run()override;
    void stop();
Q_SIGNALS:
    void updateSystemTime();
    void checkUnJinJiZhongYao();
    void updateUserLevel();
private:
    explicit GlobalTimerTask(QObject *parent = nullptr);
    friend class PH::SingleHolder<GlobalTimerTask>;
    class Private;
    QSharedPointer<Private> _p;
};

#endif // GLOBALTIMERTASK_H
