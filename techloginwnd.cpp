#include "techloginwnd.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QGraphicsDropShadowEffect>

#include "MuCustomWindow.h"




TechLoginWnd::TechLoginWnd(QWidget *parent)
    : MuShadowWindow<QWidget>(false, 20, parent)
{
    // 设置窗口标题和大小
    setFixedSize(500, 360);
    setWindowTitle("");


    // 创建主布局
    QVBoxLayout *mainLayout = static_cast<QVBoxLayout *>(clientLayout());
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // 创建标题标签
    QLabel *titleLabel = new QLabel(QStringLiteral("系统登录"));
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("color: #00FF00; font-size: 24px; font-weight: bold;");

    // 创建用户名标签和输入框
    QLineEdit *usernameEdit = new QLineEdit();
    usernameEdit->setFixedHeight(40);
    usernameEdit->setPlaceholderText("Username");
    usernameEdit->setStyleSheet("background-color: #333333; color: #FFFFFF; padding: 10px; border-radius: 5px;");

    // 创建密码标签和输入框
    QLineEdit *passwordEdit = new QLineEdit();
    passwordEdit->setPlaceholderText("Password");
    passwordEdit->setFixedHeight(40);
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setStyleSheet("background-color: #333333; color: #FFFFFF; padding: 10px; border-radius: 5px;");

    // 创建登录按钮
    QPushButton *loginButton = new QPushButton(QStringLiteral("登录"));
    loginButton->setStyleSheet(R"(
            QPushButton {
                background-color: #52baff;
                color: #000000;
                padding: 10px;
                border-radius: 5px;
                font-size: 16px;
            }
            QPushButton:hover {
                background-color: #52bacc;
            }
        )");
    QPushButton *regButton = new QPushButton(QStringLiteral("注册"));
    regButton->setStyleSheet(loginButton->styleSheet());

    // 添加阴影效果
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect();
    shadowEffect->setBlurRadius(20);
    loginButton->setFixedHeight(48);
    shadowEffect->setColor(QColor(0, 255, 0, 160));
    shadowEffect->setOffset(0, 0);
    loginButton->setGraphicsEffect(shadowEffect);

    regButton->setFixedHeight(48);
    regButton->setGraphicsEffect(shadowEffect);

    // 将各个部件添加到主布局
    mainLayout->addWidget(titleLabel);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(usernameEdit);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(passwordEdit);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(loginButton);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(regButton);


    // // 设置整体背景颜色
    clientWidget()->setStyleSheet("background-color: #000000;");

    _maninWdn  =new MuWinWindow();
    // 连接登录按钮的点击事件
    connect(loginButton, &QPushButton::clicked, this, [this,usernameEdit, passwordEdit]() {
        QString username = usernameEdit->text();
        QString password = passwordEdit->text();
        TriModel::user_table usr;
        QVariantList list= usr.select(QString("user_name = %1 and user_pwd = %2;").arg(username).arg(password));
        if (list.size()>0) {
            MuCustomMessageBox::showInformation(nullptr, "Login Successful", "Welcome, " + username + "!");
            _maninWdn->showMaximized();
            this->hide();
            _userInfo= list[0].toMap()["TriModel::user_table"].value<TriModel::user_table*>();
            _maninWdn->setUserInfo(_userInfo);
        } else {
            MuCustomMessageBox::showError(nullptr, "Login Failed", "Invalid username or password.");
        }
    });

    _userReg=new UserRegWnd();
    connect(regButton, &QPushButton::clicked, this, [this ]() {
        this->hide();
        _userReg->show();
    });
    connect(_userReg, &UserRegWnd::closeSig, this, [this ]() {
        if(_maninWdn->isHidden()) this->show();
    });

    connect(_maninWdn->getuserInfoWnd(),&userInfoWnd::showUserInfo,this,&TechLoginWnd::showUserInfo);
}
void TechLoginWnd::updateScore()
{
    _maninWdn->updateScore(_userInfo->user_score());
}

void TechLoginWnd::showUserInfo()
{
    this->hide();
    _userReg->updateUserInfo(_userInfo);
    _userReg->show();
}
