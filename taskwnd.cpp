#include "taskwnd.h"
#include "ui_taskwnd.h"
#include "MuCustomWindow.h"
#include <QDateTime>
#include <QCheckBox>
#include "GlobalTimerTask.h"
#include "techloginwnd.h"
#include "user_tableRepository.h"
TaskWnd::TaskWnd(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TaskWnd)
{
    ui->setupUi(this);

    _ptrModelMgr1=QSharedPointer<TriRepository::task_tableModelMgr>(new TriRepository::task_tableModelMgr(QString("task_level=%1").arg(TriModel::TaskLevel::unjinji_unzhongyao)));
    _ptrModelMgr2=QSharedPointer<TriRepository::task_tableModelMgr>(new TriRepository::task_tableModelMgr(QString("task_level=%1").arg(TriModel::TaskLevel::jinji_unzhongyao)));
    _ptrModelMgr3=QSharedPointer<TriRepository::task_tableModelMgr>(new TriRepository::task_tableModelMgr(QString("task_level=%1").arg(TriModel::TaskLevel::unjinji_zhongyao)));
    _ptrModelMgr4=QSharedPointer<TriRepository::task_tableModelMgr>(new TriRepository::task_tableModelMgr(QString("task_level=%1").arg(TriModel::TaskLevel::jinji_zhongyao)));

    connect(GlobalTimerTask::getInstance(),&GlobalTimerTask::checkUnJinJiZhongYao,this,&TaskWnd::checkUnJinJiZhongYao);
    TaskTableModel* model=_ptrModelMgr1->model();
    connect(model,&TaskTableModel::taskFinished,this,&TaskWnd::taskFinished);
    model=_ptrModelMgr2->model();
    connect(model,&TaskTableModel::taskFinished,this,&TaskWnd::taskFinished);
    model=_ptrModelMgr3->model();
    connect(model,&TaskTableModel::taskFinished,this,&TaskWnd::taskFinished);
    model=_ptrModelMgr4->model();
    connect(model,&TaskTableModel::taskFinished,this,&TaskWnd::taskFinished);

    QStringList list;
    list<<QString("是否选中")<<QStringLiteral("任务名称")<<QStringLiteral("截至时间")<<QStringLiteral("分数")<<QStringLiteral("备注")<<QStringLiteral("是否完成");
    _ptrModelMgr4->model()->setHeaders(list);
    _ptrModelMgr3->model()->setHeaders(list);
    _ptrModelMgr2->model()->setHeaders(list);
    _ptrModelMgr1->model()->setHeaders(list);

    ui->tableView->setModel(_ptrModelMgr4->model());
    ui->tableView_4->setModel(_ptrModelMgr2->model());
    ui->tableView_2->setModel(_ptrModelMgr3->model());
    ui->tableView_3->setModel(_ptrModelMgr1->model());

    ui->tableView->verticalHeader()->setHidden(true);
    ui->tableView_2->verticalHeader()->setHidden(true);
    ui->tableView_3->verticalHeader()->setHidden(true);
    ui->tableView_4->verticalHeader()->setHidden(true);

    ui->tableView->setEditTriggers(QAbstractItemView::DoubleClicked);
    ui->tableView_2->setEditTriggers(QAbstractItemView::DoubleClicked);
    ui->tableView_3->setEditTriggers(QAbstractItemView::DoubleClicked);
    ui->tableView_4->setEditTriggers(QAbstractItemView::DoubleClicked);


    ui->tableView->setSelectionMode(QTableView::SingleSelection);
    ui->tableView->setSelectionBehavior(QTableView::SelectRows);


    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_3->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_4->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    connect(ui->pushButton_3,&QPushButton::released,this,[this](){
        TriModel::task_table* task=new TriModel::task_table(this);
        QDateTime currentDateTime = QDateTime::currentDateTime();
        currentDateTime=currentDateTime.addDays(3);
        QString formattedDateTime = currentDateTime.toString("yyyy-MM-dd hh:mm:ss");
        task->settask_tm(formattedDateTime);
        task->settask_name(QString("双击编辑任务名称"));
        task->settask_level(TriModel::TaskLevel::jinji_zhongyao);
        task->settask_score(5);
        qint32 id=task->insert();
        if(0==id){
            task->deleteLater();
            MuCustomMessageBox::showError(nullptr,"Warning","insert failed.");
            return;
        }
        task->setid(id);
        _ptrModelMgr4->appendModel(task);
    });

    connect(ui->pushButton_8,&QPushButton::released,this,[this](){
        TriModel::task_table* task=new TriModel::task_table(this);
        QDateTime currentDateTime = QDateTime::currentDateTime();
        currentDateTime=currentDateTime.addDays(3);
        QString formattedDateTime = currentDateTime.toString("yyyy-MM-dd hh:mm:ss");
        task->settask_tm(formattedDateTime);
        task->settask_name(QString("双击编辑任务名称"));
        task->settask_level(TriModel::TaskLevel::jinji_unzhongyao);
        task->settask_score(5);
        qint32 id=task->insert();
        if(0==id){
            task->deleteLater();
            MuCustomMessageBox::showError(nullptr,"Warning","insert failed.");
            return;
        }
        task->setid(id);
        _ptrModelMgr2->appendModel(task);
    });

    connect(ui->pushButton_4,&QPushButton::released,this,[this](){
        TriModel::task_table* task=new TriModel::task_table(this);
        QDateTime currentDateTime = QDateTime::currentDateTime();
        currentDateTime=currentDateTime.addDays(3);
        QString formattedDateTime = currentDateTime.toString("yyyy-MM-dd hh:mm:ss");
        task->settask_tm(formattedDateTime);
        task->settask_name(QString("双击编辑任务名称"));
        task->settask_level(TriModel::TaskLevel::unjinji_zhongyao);
        task->settask_score(10);
        qint32 id=task->insert();
        if(0==id){
            task->deleteLater();
            MuCustomMessageBox::showError(nullptr,"Warning","insert failed.");
            return;
        }
        task->setid(id);
        _ptrModelMgr3->appendModel(task);
    });

    connect(ui->pushButton_6,&QPushButton::released,this,[this](){
        TriModel::task_table* task=new TriModel::task_table(this);
        QDateTime currentDateTime = QDateTime::currentDateTime();
        currentDateTime=currentDateTime.addDays(3);
        QString formattedDateTime = currentDateTime.toString("yyyy-MM-dd hh:mm:ss");
        task->settask_tm(formattedDateTime);
        task->settask_name(QString("双击编辑任务名称"));
        task->settask_level(TriModel::TaskLevel::unjinji_unzhongyao);
        task->settask_score(5);
        qint32 id=task->insert();
        if(0==id){
            task->deleteLater();
            MuCustomMessageBox::showError(nullptr,"Warning","insert failed.");
            return;
        }
        task->setid(id);
        _ptrModelMgr1->appendModel(task);
    });

    connect(ui->pushButton_2,&QPushButton::released,this,[this](){
        _ptrModelMgr4->model()->removeCheckedRows();
    });

    connect(ui->pushButton_9,&QPushButton::released,this,[this](){
        _ptrModelMgr2->model()->removeCheckedRows();
    });

    connect(ui->pushButton_5,&QPushButton::released,this,[this](){
        _ptrModelMgr3->model()->removeCheckedRows();
    });

    connect(ui->pushButton_7,&QPushButton::released,this,[this](){
        _ptrModelMgr1->model()->removeCheckedRows();
    });
}

TaskWnd::~TaskWnd()
{
    delete ui;
}


void TaskWnd::checkUnJinJiZhongYao()
{
    auto items=_ptrModelMgr1->model()->allItems();
    QList<int> list;
    for(int i=0;i<items.size();i++){
        TriModel::task_table* item=items[i]["TriModel::task_table"].value<TriModel::task_table*>();
        QDateTime dateTime = QDateTime::fromString(item->task_tm(), "yyyy-MM-dd HH:mm:ss");
        dateTime=dateTime.addDays(-3);
        dateTime=dateTime.addSecs(3*24*3600*0.8);
        QDateTime curr=QDateTime::currentDateTime();
        if(curr<dateTime) continue;
        if(item->is_done()) continue;
        list.append(i);
    }
    std::sort(list.begin(), list.end(), std::greater<int>());

    QVector<QVariantMap> maps;
    foreach (auto it, list) {
        QVariantMap var=_ptrModelMgr1->model()->removeItem(it);
        TriModel::task_table* item=var["TriModel::task_table"].value<TriModel::task_table*>();
        item->settask_level(TriModel::TaskLevel::jinji_zhongyao);
        item->settask_score(5);
        item->update(QString("id=%1").arg(item->id()));

        QVariantMap vm;
        vm[item->metaObject()->className()]=QVariant::fromValue(item);
        _ptrModelMgr4->model()->append(vm);
    }
}

void TaskWnd::taskFinished(TriModel::task_table * item)
{
    int currScore=TechLoginWnd::getInstance()->_userInfo->user_score();
    if(item->task_level()==TriModel::TaskLevel::jinji_unzhongyao||item->task_level()==TriModel::TaskLevel::jinji_zhongyao){
        currScore+=item->task_score();
    }else{
        if(_ptrModelMgr1->model()->allItems().size()>0||_ptrModelMgr3->model()->allItems().size()>0){
            currScore-=5;
            if(currScore<0) currScore=0;
        }else{
            currScore+=item->task_score();
        }
    }

    TechLoginWnd::getInstance()->_userInfo->setuser_score(currScore);
    TechLoginWnd::getInstance()->_userInfo->update(QString("id=%1").arg(TechLoginWnd::getInstance()->_userInfo->id()));
    TechLoginWnd::getInstance()->updateScore();
    TriModel::user_table* user=TriRepository::user_tableModelMgr::getInstance()->findItem(TechLoginWnd::getInstance()->_userInfo->id());
    user->setuser_score(TechLoginWnd::getInstance()->_userInfo->user_score());
}
