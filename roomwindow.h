#ifndef ROOMWINDOW_H
#define ROOMWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QTimer>
#include <QCloseEvent>
#include <QInputDialog>
#include <QGuiApplication>
#include <QScreen>
#include <QDesktopServices>
#include <QUrl>

class RoomWindow : public QWidget {
    Q_OBJECT

public:
    explicit RoomWindow(const QString &roomName, const QString &treeType, QWidget *parent = nullptr);
    void setTreeImage(const QString &treetype);

signals:
    void roomClosed();
    void backToMain();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onBackButtonClicked();
    void onStartFocusClicked();
    void updateTimer();
    void onDurationChanged(int index);
    void openAiChatUrl();
    void onExitFocusClicked();

private:
    QLabel *treeLabel;
    QComboBox *friendsComboBox;
    QPushButton *startFocusButton;
    QPushButton *inviteFriendsButton;
    QPushButton *backButton;
    QPushButton *aiChatLinkButton;
    QWidget *focusWindow;
    QTimer *focusTimer;
    QLabel *timerLabel;

    int remainingTime;
    QString currentTreeType;
    QComboBox *focusTimeComboBox;
    QPushButton *exitButton;
    void displayQuestionAnswer(const QString &question, const QString &answer); // 显示问题和答案

};


#endif // ROOMWINDOW_H
