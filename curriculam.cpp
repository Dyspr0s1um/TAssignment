#include "Curriculam.h"
#include <QStringList>
#include<QVBoxLayout>

Curriculam::Curriculam(int w,int h,QWidget *parent)
    : QWidget(parent)
{
    // 创建布局
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // 创建按钮
    importButton = new QPushButton(QString::fromWCharArray(L"课表导入"), this);
    layout->addWidget(importButton);

    // 创建表格
    tableWidget = new QTableWidget(13, 7, this);
    layout->addWidget(tableWidget);

    // 设置表头
    QStringList headers;
    headers << "Mon" << "Tue" << "Wed" << "Thu" << "Fri" << "Sat" << "Sun";
    tableWidget->setHorizontalHeaderLabels(headers);

    // 设置行头
    for (int i = 0; i < 13; ++i) {
        tableWidget->setVerticalHeaderItem(i, new QTableWidgetItem(QString("%1").arg(i+1)));
    }

    // 允许编辑单元格
    tableWidget->setEditTriggers(QAbstractItemView::DoubleClicked);


    // 设置每行的高度
    for (int row = 0; row < tableWidget->rowCount(); ++row) {
        tableWidget->setRowHeight(row, 60); // 例如将每行高度设置为50像素
    }

    // 设置每列的宽度
    for (int column = 0; column < tableWidget->columnCount(); ++column) {
        tableWidget->setColumnWidth(column, 180); // 例如将每列宽度设置为100像素
    }

    // 设置大小策略
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}
