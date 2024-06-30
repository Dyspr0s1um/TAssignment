#include "tasktablemodel.h"
#include <QDateTime>
#include <QDebug>
TaskTableModel::TaskTableModel(QObject *parent)
    : AbstractTableModel(parent) {

}

QVariant TaskTableModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();


    TriModel::task_table* item=items[index.row()]["TriModel::task_table"].value<TriModel::task_table*>();
    if (index.column() == 0) {
        if (role == Qt::CheckStateRole) {
            return item->checkState()? Qt::Checked : Qt::Unchecked;
        }
    }else{
        if (role == Qt::DisplayRole || role == Qt::EditRole) {
            if(1==index.column()) return item->task_name();
            if(2==index.column()) return item->task_tm();
            if(3==index.column()) return item->task_score();
            if(4==index.column()) return item->task_desc();
            if(5==index.column()) return item->is_done();
        }
    }
    return QVariant();
}

bool TaskTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    TriModel::task_table* item=items[index.row()]["TriModel::task_table"].value<TriModel::task_table*>();
    if (index.column() == 0 && role == Qt::CheckStateRole) {
        item->setCheckState(value == Qt::Checked);
        emit dataChanged(index, index, {Qt::CheckStateRole});
        return true;
    } else if (role == Qt::EditRole) {
        // 设置其他列的数据
        QVariant last;
        if(1==index.column())  {
            QString valueStr=value.toString();
            if(valueStr.isEmpty()) return false;
            last=item->task_name();
            item->settask_name(valueStr);
        }
        if(2==index.column())  {
            QString valueStr=value.toString();
            if(valueStr.isEmpty()) return false;
            last=item->task_tm();
            item->settask_tm(valueStr);
        }
        if(3==index.column()) {
            int valueStr=value.toInt();
            if(0==valueStr) return false;
            last=item->task_score();
            item->settask_score(valueStr);
        }
        if(4==index.column()) {
            QString valueStr=value.toString();
            if(valueStr.isEmpty()) return false;
            last=item->task_desc();
            item->settask_desc(valueStr);
        }
        bool taskDown=false;
        if(5==index.column()) {
            QDateTime dateTime = QDateTime::fromString(item->task_tm(), "yyyy-MM-dd HH:mm:ss");
            QDateTime curr=QDateTime::currentDateTime();
            qint64 secondsDiff = curr.secsTo(dateTime);
            if(secondsDiff<=5&&item->task_level()!=TriModel::TaskLevel::unjinji_unzhongyao) return false;
            bool valueStr=value.toBool();
            if(item->is_done()) return false;
            last=item->is_done();
            item->setis_done(valueStr);
            taskDown=item->is_done();
        }
        if(!item->update(QString("id=%1").arg(item->id()))){
            if(1==index.column())  {
                QString valueStr=last.toString();
                item->settask_name(valueStr);
            }
            if(2==index.column())  {
                QString valueStr=last.toString();
                item->settask_tm(valueStr);
            }
            if(3==index.column()) {
                int valueStr=last.toInt();
                item->settask_score(valueStr);
            }
            if(4==index.column()) {
                QString valueStr=last.toString();
                item->settask_desc(valueStr);
            }
            if(5==index.column()) {
                bool valueStr=last.toBool();
                item->setis_done(valueStr);
            }
            return false;
        }
        if(taskDown) Q_EMIT taskFinished(item);
        emit dataChanged(index, index);
        return true;
    }
    return false;
}
Qt::CheckState TaskTableModel::getCheckState(int row) const {
    if (row < 0 || row >= items.size())
        return Qt::Unchecked;
    TriModel::task_table* item=items[row]["TriModel::task_table"].value<TriModel::task_table*>();
    return item->checkState()?Qt::Checked:Qt::Unchecked;
}

void TaskTableModel::setCheckState(int row, Qt::CheckState state) {
    if (row >= 0 && row < items.size()) {
        TriModel::task_table* item=items[row]["TriModel::task_table"].value<TriModel::task_table*>();
        item->setCheckState(state==Qt::Checked);
        QModelIndex index = createIndex(row, 0);
        emit dataChanged(index, index, {Qt::CheckStateRole});
    }
}
void TaskTableModel::removeCheckedRows()
{
    for(int i=items.size()-1;i>=0;i--){
        TriModel::task_table* item=items[i]["TriModel::task_table"].value<TriModel::task_table*>();
        if(item->checkState()){
            beginRemoveRows(QModelIndex(), i, i);
            items.removeAt(i);
            endRemoveRows();
            item->deleteQuery(QString("id=%1").arg(item->id()));
            item->deleteLater();
        }
    }
}
