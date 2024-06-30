#ifndef USERLISTWND_H
#define USERLISTWND_H

#include <QWidget>

namespace Ui {
class UserListWnd;
}

class UserListWnd : public QWidget
{
    Q_OBJECT

public:
    explicit UserListWnd(QWidget *parent = nullptr);
    ~UserListWnd();
private Q_SLOTS:
    void updateUserLevel();
private:
    Ui::UserListWnd *ui;
    void executeTask();
};

#endif // USERLISTWND_H
