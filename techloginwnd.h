#ifndef TECHLOGINWND_H
#define TECHLOGINWND_H
#include "MuShadowWindow.h"
#include "Singleholder.h"
#include "user_table.h"
#include "MuWinWindow.h"
#include "userregwnd.h"
class TechLoginWnd :  public MuShadowWindow<QWidget>,public PH::SingleHolder<TechLoginWnd>
{
    Q_OBJECT
public:
    TriModel::user_table* _userInfo=nullptr;
    void updateScore();
public Q_SLOTS:
    void showUserInfo();
private:
    friend class PH::SingleHolder<TechLoginWnd>;
    explicit TechLoginWnd(QWidget *parent = nullptr);
    MuWinWindow* _maninWdn;
    UserRegWnd* _userReg;
};

#endif // TECHLOGINWND_H
