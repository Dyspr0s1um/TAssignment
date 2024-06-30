#ifndef USERTABLEMODEL_H
#define USERTABLEMODEL_H
#include "abstracttablemodel.h"
class UserTableModel : public AbstractTableModel
{
public:
    explicit UserTableModel(QObject *parent = nullptr);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

};

#endif // USERTABLEMODEL_H
