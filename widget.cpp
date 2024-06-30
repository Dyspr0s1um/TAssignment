#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent),
    createRoomButton(new QPushButton("创建房间", this)),
    scrollArea(new QScrollArea(this)),
    scrollAreaWidgetContents(new QWidget),
    scrollAreaLayout(new QVBoxLayout),
    currentRoomWindow(nullptr)
{
    createRoomButton->setStyleSheet("QPushButton { background-color: #52bacc;font-size:20px} ");
    setWindowTitle("房间管理");
    createRoomButton->setGeometry(QRect(QPoint(1200, 10), QSize(100, 40)));
    connect(createRoomButton, &QPushButton::clicked, this, &Widget::onCreateRoomClicked);
    scrollArea->setGeometry(QRect(QPoint(10, 60), QSize(1350, 900)));
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(scrollAreaWidgetContents);
    scrollAreaWidgetContents->setLayout(scrollAreaLayout);

    scrollAreaLayout->setAlignment(Qt::AlignTop);
}

Widget::~Widget() {}

void Widget::onCreateRoomClicked()
{
    CreateRoomDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString roomName = dialog.getRoomName();
        QString treeType = dialog.getSelectedTreeType();

        QPushButton *newRoomButton = new QPushButton(roomName + " (" + treeType + ")", scrollAreaWidgetContents);
        newRoomButton->setFixedSize(1300, 50);
        newRoomButton->setProperty("roomName", roomName);
        newRoomButton->setProperty("treeType", treeType);
        newRoomButton->setStyleSheet("QPushButton { background-color: #52bacc;font-size:20px} ");
        connect(newRoomButton, &QPushButton::clicked, this, &Widget::onRoomButtonClicked);

        scrollAreaLayout->insertWidget(0, newRoomButton);
    }
}

void Widget::onRoomButtonClicked()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QString roomName = button->property("roomName").toString();
        QString treeType = button->property("treeType").toString();

        currentRoomWindow = new RoomWindow(roomName, treeType);
        connect(currentRoomWindow, &RoomWindow::roomClosed, this, &Widget::onRoomWindowClosed);
        connect(currentRoomWindow, &RoomWindow::backToMain, this, &Widget::onBackToMain);

        currentRoomWindow->show();
        this->hide();
    }
}

void Widget::onRoomWindowClosed()
{
    this->show();
    if (currentRoomWindow) {
        currentRoomWindow->deleteLater();
        currentRoomWindow = nullptr;
    }
}

void Widget::onBackToMain()
{
    this->show();
    if (currentRoomWindow) {
        currentRoomWindow->deleteLater();
        currentRoomWindow = nullptr;
    }
}

void Widget::showRoomWindow() {
    this->show();
}
