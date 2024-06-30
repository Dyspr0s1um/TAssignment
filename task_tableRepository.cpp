#include "task_tableRepository.h"

using namespace TriRepository;
task_tableModelMgr::task_tableModelMgr(const   QString& filtr,QObject* parent):
    QObject(parent),
    AbstractModelMgr<TaskTableModel,TriModel::task_table>()
{
    AbstractModelMgr::init(filtr,"",true);
}
void task_tableModelMgr::insert()
{
}
