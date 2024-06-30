#include "usertablemodel.h"
#include "user_table.h"
#include "userregwnd.h"
UserTableModel::UserTableModel(QObject *parent)
    :AbstractTableModel(parent) {}


QVariant UserTableModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= items.size() || index.column() >= headers.size() || role != Qt::DisplayRole)
        return QVariant();
    TriModel::user_table* item=items[index.row()]["TriModel::user_table"].value<TriModel::user_table*>();

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        if(0==index.column()) return item->user_name();
        if(1==index.column()) return UserRegWnd::byteArrayToImage(item->user_img());
        if(2==index.column()) return item->user_score();
        if(3==index.column()) return item->user_level();
        if(4==index.column()) return item->user_desc();
    }

    return QVariant();
}
