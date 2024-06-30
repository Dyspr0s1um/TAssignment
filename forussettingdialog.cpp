#include "forussettingdialog.h"
#include<QLabel>

FocusSettingDialog::FocusSettingDialog(QWidget *parent)
    : QDialog(parent),
    layout(new QVBoxLayout),
    durationComboBox(new QComboBox(this)),

    okButton(new QPushButton("确定", this))
{
    setWindowTitle("专注设置");

    durationComboBox->addItem("10分钟", QVariant(10));
    durationComboBox->addItem("20分钟", QVariant(20));
    durationComboBox->addItem("30分钟", QVariant(30));
    durationComboBox->addItem("45分钟", QVariant(45));
    durationComboBox->addItem("60分钟", QVariant(60));


    layout->addWidget(new QLabel("选择专注时长:", this));
    layout->addWidget(durationComboBox);

    layout->addWidget(okButton);

    setLayout(layout);

    connect(okButton, &QPushButton::clicked, this, &FocusSettingDialog::onOkClicked);
}

void FocusSettingDialog::onOkClicked()
{
    int durationMinutes = durationComboBox->currentData().toInt();

    emit startFocus(durationMinutes);
    accept();
}
