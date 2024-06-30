#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>
#include "createroomdialog.h"
#include "roomwindow.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();


public slots:
    void showRoomWindow(); // 添加槽函数声明

private slots:
    void onCreateRoomClicked();
    void onRoomButtonClicked();
    void onRoomWindowClosed();
    void onBackToMain(); // 槽函数，处理返回主窗口信号

private:
    QPushButton *createRoomButton;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *scrollAreaLayout;
    RoomWindow *currentRoomWindow; // 当前房间窗口
};

#endif // WIDGET_H
