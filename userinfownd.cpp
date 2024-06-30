#include "userinfownd.h"
#include "ui_userinfownd.h"
#include "userregwnd.h"
#include <QMouseEvent>
#include <QDebug>
userInfoWnd::userInfoWnd(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::userInfoWnd)
{
    ui->setupUi(this);
    ui->label->setFixedSize(48,48);
}

userInfoWnd::~userInfoWnd()
{
    delete ui;
}

void userInfoWnd::setInfo(TriModel::user_table usrnfo)
{
    ui->label->setFixedSize(120,120);
    _userInfo=usrnfo;
    QImage img=UserRegWnd::byteArrayToImage(_userInfo.user_img());
    QSize labelSize = ui->label->size();
    // 调整图片大小以适应 QLabel
    QPixmap pixmap = QPixmap::fromImage(img);
    QPixmap scaledPixmap = pixmap.scaled(labelSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    // 设置 QLabel 的内容为调整后的 QPixmap
    ui->label->setPixmap(scaledPixmap);
    // 设置 QLabel 自动适应内容大小
    ui->label->setScaledContents(true);

    ui->label_2->setText(_userInfo.user_desc());
}

void userInfoWnd::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        qDebug() << "Widget double-clicked at position:" << event->pos();
        // 在这里添加你的处理逻辑

        Q_EMIT showUserInfo();
    }

    // 调用基类的方法
    QWidget::mouseDoubleClickEvent(event);
}
