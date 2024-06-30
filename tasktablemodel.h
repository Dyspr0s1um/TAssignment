#ifndef TASKTABLEMODEL_H
#define TASKTABLEMODEL_H


#include <QVector>
#include "abstracttablemodel.h"
#include "task_table.h"
class TaskTableModel : public AbstractTableModel
{
    Q_OBJECT
public:
    explicit TaskTableModel(QObject *parent = nullptr);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::CheckState getCheckState(int row) const;
    void setCheckState(int row, Qt::CheckState state);
    void removeCheckedRows();
Q_SIGNALS:
    void taskFinished(TriModel::task_table*);
};

#endif // TASKTABLEMODEL_H
