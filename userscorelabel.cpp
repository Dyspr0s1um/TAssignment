#include "userscorelabel.h"
#include "ui_userscorelabel.h"

userScoreLabel::userScoreLabel(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::userScoreLabel)
{
    ui->setupUi(this);
    ui->label->setFixedSize(90,40);
    ui->label_2->setFixedSize(ui->label->size());
}

userScoreLabel::~userScoreLabel()
{
    delete ui;
}

void userScoreLabel::updateScore(int score)
{
    ui->label_2->setText(QString::number(score));
}
