#ifndef task_tableREPOSITORY_H
#define task_tableREPOSITORY_H

#include "task_table.h"
#include "ModelManager.h"
#include "tasktablemodel.h"

namespace TriRepository
{
class  task_tableModelMgr :public QObject, public AbstractModelMgr<TaskTableModel,TriModel::task_table>
{
    Q_OBJECT
public:
    explicit task_tableModelMgr(const   QString& filtr,QObject* parent=nullptr);
    Q_INVOKABLE void insert();
private:
};
} //namespace TriRepository
#endif // task_tableREPOSITORY_H
