#ifndef USERINFOWND_H
#define USERINFOWND_H

#include <QWidget>
#include "user_table.h"

namespace Ui {
class userInfoWnd;
}

class userInfoWnd : public QWidget
{
    Q_OBJECT

public:
    explicit userInfoWnd(QWidget *parent = nullptr);
    ~userInfoWnd();
    void setInfo(TriModel::user_table usrnfo);
Q_SIGNALS:
    void showUserInfo();
protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;
private:
    Ui::userInfoWnd *ui;
    TriModel::user_table _userInfo;
};

#endif // USERINFOWND_H
