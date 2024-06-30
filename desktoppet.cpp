#include "desktoppet.h"
#include <QHBoxLayout>
#include <QScreen>
#include <QApplication>
#include <QPixmap>
#include <QMouseEvent>


DesktopPet::DesktopPet(QWidget *parent) : QWidget(parent) {
    setupUI();
}

void DesktopPet::setupUI() {
    setFixedSize(200, 200); // 设置桌宠大小为200x200

    // 创建布局
    QHBoxLayout *layout = new QHBoxLayout(this);

    // 设置图片
    QPixmap pixmap(":/pets/peking.png"); // 替换为你的图片路径
    QLabel *imageLabel = new QLabel(this);
    imageLabel->setPixmap(pixmap.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation)); // 缩放图片并保持纵横比
    layout->addWidget(imageLabel);


    // 应用布局
    setLayout(layout);

    // 设置窗口属性
    setWindowFlags(Qt::Widget | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground); // 设置背景透明

    // 将桌宠放置在桌面右上角
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect desktopRect = screen->availableGeometry(); // 桌面可用区域
    move(desktopRect.width() - width(), 0); // 移动到右上角
}

void DesktopPet::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        dragging = true;
        dragStartPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void DesktopPet::mouseMoveEvent(QMouseEvent *event) {
    if (dragging && (event->buttons() & Qt::LeftButton)) {
        move(event->globalPos() - dragStartPosition);
        event->accept();
    }
}

void DesktopPet::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        dragging = false;
        event->accept();
    }
}

void DesktopPet::mouseDoubleClickEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        emit petClicked();
    }
}

