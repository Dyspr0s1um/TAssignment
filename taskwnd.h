#ifndef TASKWND_H
#define TASKWND_H

#include <QWidget>
#include "task_tableRepository.h"

namespace Ui {
class TaskWnd;
}

class TaskWnd : public QWidget
{
    Q_OBJECT

public:
    explicit TaskWnd(QWidget *parent = nullptr);
    ~TaskWnd();
private Q_SLOTS:
    void checkUnJinJiZhongYao();
     void taskFinished(TriModel::task_table*);
private:
    Ui::TaskWnd *ui;
    QSharedPointer<TriRepository::task_tableModelMgr> _ptrModelMgr1;
    QSharedPointer<TriRepository::task_tableModelMgr> _ptrModelMgr2;
    QSharedPointer<TriRepository::task_tableModelMgr> _ptrModelMgr3;
    QSharedPointer<TriRepository::task_tableModelMgr> _ptrModelMgr4;
};

#endif // TASKWND_H
