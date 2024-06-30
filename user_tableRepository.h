#ifndef user_tableREPOSITORY_H
#define user_tableREPOSITORY_H

#include "Singleholder.h"
#include "ModelManager.h"
#include "usertablemodel.h"
#include "user_table.h"
namespace TriRepository
{
class  user_tableModelMgr :public QObject,
                           public AbstractModelMgr<UserTableModel,TriModel::user_table>,
                           public PH::SingleHolder<user_tableModelMgr>
{
    Q_OBJECT
public:
    Q_INVOKABLE void insert();
private:
    friend class PH::SingleHolder<user_tableModelMgr>;
    explicit user_tableModelMgr(QObject *parent = nullptr);
};
} //namespace TriRepository
#endif // user_tableREPOSITORY_H
