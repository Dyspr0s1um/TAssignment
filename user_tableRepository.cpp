#include "user_tableRepository.h"
#include "user_table.h"
using namespace TriRepository;
user_tableModelMgr::user_tableModelMgr(QObject *parent)
    :QObject(parent),
    AbstractModelMgr<UserTableModel,TriModel::user_table>()
{
    init("ALL","",true);
}
void user_tableModelMgr::insert()
{
}
