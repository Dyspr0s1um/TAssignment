#include "userlistwnd.h"
#include "ui_userlistwnd.h"
#include "user_tableRepository.h"
#include "GlobalTimerTask.h"
#include <QDateTime>
UserListWnd::UserListWnd(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UserListWnd)
{
    ui->setupUi(this);

    QStringList list;
    list<<QStringLiteral("名称")<<QStringLiteral("图形")<<QStringLiteral("分数")<<QStringLiteral("等级")<<QStringLiteral("备注");
    TriRepository::user_tableModelMgr::getInstance()->model()->setHeaders(list);

    ui->tableView->setModel(TriRepository::user_tableModelMgr::getInstance()->model());

    ui->tableView->verticalHeader()->setHidden(true);

    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ///按等级排序
    ui->tableView->sortByColumn(3, Qt::AscendingOrder);

    connect(GlobalTimerTask::getInstance(),&GlobalTimerTask::updateUserLevel,this,&UserListWnd::updateUserLevel);

}

UserListWnd::~UserListWnd()
{
    delete ui;
}

void UserListWnd::updateUserLevel()
{
    QDateTime now = QDateTime::currentDateTime();
    if (now.date().dayOfWeek() == Qt::Monday && now.time().hour() == 0 && now.time().minute() == 0) {
        executeTask();
    }
}

void UserListWnd::executeTask()
{
    auto data=TriRepository::user_tableModelMgr::getInstance()->model()->allItems();
    std::sort(data.begin(), data.end(), [](QVariantMap &a, QVariantMap &b) {
        TriModel::user_table* itemA=a["TriModel::user_table"].value<TriModel::user_table*>();
        TriModel::user_table* itemB=b["TriModel::user_table"].value<TriModel::user_table*>();
        return itemA->user_score() > itemB->user_score();
    });

    if(data.size()>1){
         TriModel::user_table* item=data[0]["TriModel::user_table"].value<TriModel::user_table*>();
        item->setuser_level(item->user_level()+3);
    }
    if(data.size()>2){
        TriModel::user_table* item=data[1]["TriModel::user_table"].value<TriModel::user_table*>();
        item->setuser_level(item->user_level()+2);
    }
    if(data.size()>3){
        TriModel::user_table* item=data[2]["TriModel::user_table"].value<TriModel::user_table*>();
        item->setuser_level(item->user_level()+1);
    }

    foreach (auto& it, data) {
        TriModel::user_table* item=it["TriModel::user_table"].value<TriModel::user_table*>();
        item->setuser_last_score(item->user_score());
        item->setuser_score(0);
    }

    TriRepository::user_tableModelMgr::getInstance()->model()->resetItems(data);
}
