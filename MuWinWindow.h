#ifndef MUWINWINDOW_H
#define MUWINWINDOW_H

#include <QWidget>
#include"widget.h"
#include"curriculam.h"
#ifdef Q_OS_WIN32

#include <Windows.h>
#include "userinfownd.h"
#include "userscorelabel.h"
#include "taskwnd.h"
#include "userlistwnd.h"
namespace Ui {
class MuWinWindow;
}

class QHBoxLayout;
class QVBoxLayout;

class MuWinWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MuWinWindow(QWidget *parent = 0);
    ~MuWinWindow();

    QString iconFileName() const;
    void setWindowIcon(const QString &fileName);
    void setWindowTitle(const QString &title);
    void updateScore(int nScore);
    bool isMaximized() const
    { return isMaximized_; }
    userInfoWnd* getuserInfoWnd(){return _infoWnd;}
    void setUserInfo(TriModel::user_table* userInfo);
protected:
    virtual bool nativeEvent(const QByteArray &eventType,
                             void *message,
                             long *result);

    void paintEvent(QPaintEvent *e);

private:
    LRESULT calculateBorder(const QPoint &pt);

private:
    Ui::MuWinWindow *ui;

    QString iconFileName_;
    QString title_;
    UserListWnd* _userListWnd;
    QVBoxLayout *mainLayout_;
    QWidget *clientWidget_;
    userInfoWnd* _infoWnd;
    userScoreLabel* _userPtr;
    TaskWnd * _ptrTaskWnd;
    Widget* rooms;
    Curriculam *curriculamWidget;
    bool isMaximized_;
};

#endif

#endif // MUWINWINDOW_H
