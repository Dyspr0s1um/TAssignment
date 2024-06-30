#include "createroomdialog.h"

CreateRoomDialog::CreateRoomDialog(QWidget *parent)
    : QDialog{parent},
    layout(new QVBoxLayout),
    roomNameEdit(new QLineEdit(this)),
    okButton(new QPushButton("确定", this)),
    treeTypeComboBox(new QComboBox(this)),
    roomNameLabel(new QLabel("请输入房间名称:", this)), // 初始化房间名称标签
    treeTypeLabel(new QLabel("请选择你想要的树种:", this)) // 初始化树种标签
{
    setWindowTitle("创建新房间");

    // 添加下拉框选项
    treeTypeComboBox->addItem("蓝花楹");
    treeTypeComboBox->addItem("黄金风铃木");
    treeTypeComboBox->addItem("橘子树");
    treeTypeComboBox->addItem("荷花");
    treeTypeComboBox->addItem("水仙花");
    treeTypeComboBox->addItem("波斯菊");
    treeTypeComboBox->addItem("梅花");
    treeTypeComboBox->addItem("郁金香");
    treeTypeComboBox->addItem("百合花");
    treeTypeComboBox->addItem("女巫蘑菇");
    treeTypeComboBox->addItem("情人树");
    treeTypeComboBox->addItem("圣代树");
    treeTypeComboBox->addItem("玫瑰");
    treeTypeComboBox->addItem("向日葵");
    treeTypeComboBox->addItem("糖果树");
    treeTypeComboBox->addItem("竹");
    treeTypeComboBox->addItem("紫藤");
    treeTypeComboBox->addItem("西瓜");

    // 将标签和输入框添加到布局
    layout->addWidget(roomNameLabel);
    layout->addWidget(roomNameEdit);
    layout->addWidget(treeTypeLabel);
    layout->addWidget(treeTypeComboBox);
    layout->addWidget(okButton);

    setLayout(layout);

    connect(okButton, &QPushButton::clicked, this, &CreateRoomDialog::onOkClicked);
}

QString CreateRoomDialog::getRoomName() const
{
    return roomNameEdit->text();
}

void CreateRoomDialog::onOkClicked()
{
    accept();
}

QString CreateRoomDialog::getSelectedTreeType() const
{
    return treeTypeComboBox->currentText();
}
