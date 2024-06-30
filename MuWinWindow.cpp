#include "MuWinWindow.h"

#ifdef Q_OS_WIN32

#include <QLayout>
#include <QPainter>
#include <QIcon>
#include <QDebug>
#include <QPushButton>
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QLabel>
#include <QStackedWidget>
#include <QFrame>

#include <QtWinExtras/QtWin>
#include <dwmapi.h>

#include "MuWinTitlebar.h"



#pragma comment (lib, "user32.lib")

#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lParam)    ((int)(short)LOWORD(lParam))
#endif
#ifndef GET_Y_LPARAM
#define GET_Y_LPARAM(lParam)    ((int)(short)HIWORD(lParam))
#endif

MuWinWindow::MuWinWindow(QWidget *parent) :
    QWidget(parent),
    clientWidget_(new QWidget(this))
{
    setWindowFlags(Qt::FramelessWindowHint);
    setMinimumSize(800, 600);
    //resize(1400,950);

    HWND hwnd = reinterpret_cast<HWND>(this->winId());
    DWORD style = GetWindowLong(hwnd, GWL_STYLE);
    SetWindowLongPtr(hwnd, GWL_STYLE, style | WS_MAXIMIZEBOX | WS_THICKFRAME | WS_CAPTION);

    bool enabled = QtWin::isCompositionEnabled();
    if (enabled) {
        HWND hwnd = (HWND)this->winId();
        DWORD style = ::GetWindowLong(hwnd, GWL_STYLE);
        ::SetWindowLong(hwnd, GWL_STYLE, style | WS_THICKFRAME | WS_CAPTION | WS_BORDER);
        QtWin::extendFrameIntoClientArea(this, 1, 1, 1, 1);
    }

    // 其实是设置的标题栏的颜色
    setStyleSheet("background-color: #52bacc");
    clientWidget_->setStyleSheet("background-color: #FFFFFF");
    setWindowIcon(":/images/logo.jpg");

    MuWinTitlebar *titleBar = new MuWinTitlebar(this);
    titleBar->setFixedHeight(50);

    mainLayout_ = new QVBoxLayout(this);
    mainLayout_->addWidget(titleBar);
    mainLayout_->addWidget(clientWidget_);
    mainLayout_->setContentsMargins(0, 0, 0, 0);

    installEventFilter(titleBar);

    setWindowTitle(QStringLiteral("主窗口"));

    connect(titleBar, &MuWinTitlebar::ShowMinimized, this, &MuWinWindow::showMinimized);
    connect(titleBar, &MuWinTitlebar::ShowMaximized, this, &MuWinWindow::showMaximized);
    connect(titleBar, &MuWinTitlebar::ShowRestoreSize, this, &MuWinWindow::showNormal);
    connect(titleBar, &MuWinTitlebar::Close, this, &MuWinWindow::close);


    QHBoxLayout *centerLayout = new QHBoxLayout(clientWidget_);
    centerLayout->setContentsMargins(0,9,0,0);
    centerLayout->setSpacing(0);
    QVBoxLayout *navMenu = new QVBoxLayout();
    navMenu->setMargin(0);
    navMenu->setSpacing(4);
    // 创建导航按钮
    QPushButton *button1 = new QPushButton(QStringLiteral("四象工作法"));
    QPushButton *button2 = new QPushButton(QStringLiteral("课表录入"));
    QPushButton *button3 = new QPushButton(QStringLiteral("建立房间"));
    QPushButton *button4 = new QPushButton(QStringLiteral("好友"));
    QString buttonStyle = R"(
   QPushButton {
                background-color: #4CAF50; /* 绿色背景 */
                color: white;             /* 白色文字 */
                border: 2px solid #4CAF50; /* 绿色边框 */
                border-radius: 5px;       /* 圆角 */
                padding: 10px;            /* 内边距 */
                font-family: Arial;        /* 字体 */
                font-size: 18px;           /* 字体大小 */
                font-weight: bold;         /* 字体粗细 */
            }
            QPushButton:hover {
                background-color: #45a049; /* 浮动状态下的背景颜色 */
            }
            QPushButton:pressed {
                background-color: #3e8e41; /* 按下状态下的背景颜色 */
            }
        )";

    button1->setStyleSheet(buttonStyle);
    button1->setFixedSize(240,60);
    button2->setStyleSheet(button1->styleSheet());
    button2->setFixedSize(button1->size());
    button3->setStyleSheet(button1->styleSheet());
    button3->setFixedSize(button1->size());
    button4->setStyleSheet(button1->styleSheet());
    button4->setFixedSize(button1->size());
    _infoWnd =new userInfoWnd();
    _infoWnd->setFixedSize(240,140);
    navMenu->addWidget(_infoWnd);

    // 将按钮添加到导航菜单中
    navMenu->addWidget(button1);
    navMenu->addSpacing(20); // 添加弹性空间使按钮靠上排列

    navMenu->addWidget(button2);
    navMenu->addSpacing(20); // 添加弹性空间使按钮靠上排列

    navMenu->addWidget(button3);
    navMenu->addSpacing(20);// 添加弹性空间使按钮靠上排列

    navMenu->addWidget(button4);
    navMenu->addSpacing(20); // 添加弹性空间使按钮靠上排列

    navMenu->setSpacing(20);

    // 创建堆叠窗体
    QStackedWidget *stackedWidget = new QStackedWidget();
    stackedWidget->setFixedSize(1400,950);
    stackedWidget->layout()->setMargin(0);

    // 创建各个页面并添加到堆叠窗体中
    _ptrTaskWnd = new TaskWnd();
    _ptrTaskWnd->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QWidget *page2 = new QWidget();
    QLabel *label2 = new QLabel("This is Page 2", page2);
    QVBoxLayout *page2Layout = new QVBoxLayout(page2);
    page2Layout->addWidget(label2);

    QWidget *page3 = new QWidget();
    QLabel *label3 = new QLabel("This is Page 3", page3);
    QVBoxLayout *page3Layout = new QVBoxLayout(page3);
    page3Layout->addWidget(label3);

    _userListWnd=new UserListWnd();
    rooms = new Widget();

    curriculamWidget = new Curriculam(670, 600, this);
    curriculamWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);


    stackedWidget->addWidget(_ptrTaskWnd);
    stackedWidget->addWidget(curriculamWidget);
    stackedWidget->addWidget(rooms);
    stackedWidget->addWidget(_userListWnd);

    // 连接按钮和页面切换
    connect(button1, &QPushButton::clicked, [stackedWidget]() { stackedWidget->setCurrentIndex(0); });
    connect(button2, &QPushButton::clicked, [stackedWidget]() { stackedWidget->setCurrentIndex(1); });
    connect(button3, &QPushButton::clicked, [stackedWidget]() { stackedWidget->setCurrentIndex(2); });
    connect(button4, &QPushButton::clicked, [stackedWidget]() { stackedWidget->setCurrentIndex(3); });

    QFrame* leftFrame=new QFrame(clientWidget_);
    leftFrame->setFrameShape(QFrame::Box);
    leftFrame->setFrameShadow(QFrame::Raised);
    leftFrame->setStyleSheet(R"(
            QFrame {
                border: 0px; /* 绿色边框 */
                border-radius: 0px;       /* 圆角 */
                background-color: #52baff;
            }
        )");
    leftFrame->setLayout(navMenu);
    centerLayout->addWidget(leftFrame);


    // 创建一个 QFrame 并设置样式
    QFrame *frameRight = new QFrame();
    frameRight->setFrameShape(QFrame::Box);
    frameRight->setFrameShadow(QFrame::Raised);
    frameRight->setObjectName("frameRight");
    frameRight->setStyleSheet(R"(
            QFrame#frameRight {
                border: 2px solid #CCCCCC; /* 绿色边框 */
                border-radius: 10px;       /* 圆角 */
                padding: 20px;             /* 内边距 */
            }
        )");
    QHBoxLayout* subLayout=new QHBoxLayout(frameRight);
    subLayout->addWidget(stackedWidget);

    // 将堆叠窗体添加到 QFrame 中
    QVBoxLayout *frameLayout = new QVBoxLayout();
    frameLayout->setMargin(9);
    _userPtr=new userScoreLabel();
    frameLayout->addWidget(_userPtr);
    frameLayout->addSpacing(9);
    frameLayout->addWidget(frameRight);

    centerLayout->addWidget(leftFrame);
    centerLayout->addLayout(frameLayout);
}

MuWinWindow::~MuWinWindow()
{
}

///
/// \brief MuWinWindow::paintEvent 通过paintEvent绘制图标和标题，如果用MuTitleBar来显示的图标和标题的话,
///                                当鼠标点击图标和标题时无法移动窗体
/// \param e
///
void MuWinWindow::paintEvent(QPaintEvent *e)
{
    QPainter p(this);

    // 绘制窗口图标
    QRect imgTarget(10, 10, 30, 30);
    QImage img(iconFileName_);
    p.drawImage(imgTarget, img.scaled(30, 30, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    // 绘制标题
    QFont font;
    font.setPixelSize(14);
    QFontMetrics fm(font);
    QRect titleTarget(45, 10, fm.width(title_), 30);

    p.setFont(font);
    p.setPen(Qt::white);
    p.drawText(titleTarget, title_, QTextOption(Qt::AlignCenter));

    QWidget::paintEvent(e);
}

QString MuWinWindow::iconFileName() const
{
    return iconFileName_;
}

void MuWinWindow::setWindowIcon(const QString &fileName)
{
    iconFileName_ = fileName;
    this->QWidget::setWindowIcon(QIcon(fileName));
}

void MuWinWindow::setWindowTitle(const QString &title)
{
    title_ = title;
    this->QWidget::setWindowTitle(title);
}

void MuWinWindow::setUserInfo(TriModel::user_table *userInfo)
{
    _infoWnd->setInfo(*userInfo);
    _userPtr->updateScore(userInfo->user_score());
}

bool MuWinWindow::nativeEvent(const QByteArray &eventType,
                              void *message,
                              long *result)
{

#ifdef Q_OS_WIN
    if (eventType != "windows_generic_MSG")
        return false;

    MSG* msg = static_cast<MSG*>(message);

    QWidget* widget = QWidget::find(reinterpret_cast<WId>(msg->hwnd));
    if (!widget)
        return false;

    switch (msg->message) {
    case WM_NCCALCSIZE: {
        *result = 0;
        return true;
    }

    case WM_NCHITTEST: {
        int x = GET_X_LPARAM(msg->lParam);
        int y = GET_Y_LPARAM(msg->lParam);

        QPoint pt = mapFromGlobal(QPoint(x, y));
        *result = calculateBorder(pt);
        if (*result == HTCLIENT) {
            QWidget* tempWidget = this->childAt(pt.x(), pt.y());
            if (tempWidget == NULL) {
                *result = HTCAPTION;
            }
        }
        return true;
    }

    case WM_GETMINMAXINFO: {
        if (::IsZoomed(msg->hwnd)) {
            isMaximized_ = true;
            RECT frame = { 0, 0, 0, 0 };
            AdjustWindowRectEx(&frame, WS_OVERLAPPEDWINDOW, FALSE, 0);
            frame.left = abs(frame.left);
            frame.top = abs(frame.bottom);
            widget->setContentsMargins(frame.left, frame.top, frame.right, frame.bottom);
        }
        else {
            widget->setContentsMargins(0, 0, 0, 0);
            isMaximized_ = false;
        }

        *result = ::DefWindowProc(msg->hwnd, msg->message, msg->wParam, msg->lParam);
        return true;
    }
    break;

    default:
        break;
    }

#endif

    return QWidget::nativeEvent(eventType, message, result);
}

LRESULT MuWinWindow::calculateBorder(const QPoint &pt)
{
    if (::IsZoomed((HWND)this->winId())) {
        return HTCLIENT;
    }
    int borderSize = 4;
    int cx = this->size().width();
    int cy = this->size().height();

    QRect rectTopLeft(0, 0, borderSize, borderSize);
    if (rectTopLeft.contains(pt)) {
        return HTTOPLEFT;
    }

    QRect rectLeft(0, borderSize, borderSize, cy - borderSize * 2);
    if (rectLeft.contains(pt)) {
        return HTLEFT;
    }

    QRect rectTopRight(cx - borderSize, 0, borderSize, borderSize);
    if (rectTopRight.contains(pt)) {
        return HTTOPRIGHT;
    }

    QRect rectRight(cx - borderSize, borderSize, borderSize, cy - borderSize * 2);
    if (rectRight.contains(pt)) {
        return HTRIGHT;
    }

    QRect rectTop(borderSize, 0, cx - borderSize * 2, borderSize);
    if (rectTop.contains(pt)) {
        return HTTOP;
    }

    QRect rectBottomLeft(0, cy - borderSize, borderSize, borderSize);
    if (rectBottomLeft.contains(pt)) {
        return HTBOTTOMLEFT;
    }

    QRect rectBottomRight(cx - borderSize, cy - borderSize, borderSize, borderSize);
    if (rectBottomRight.contains(pt)) {
        return HTBOTTOMRIGHT;
    }

    QRect rectBottom(borderSize, cy - borderSize, cx - borderSize * 2, borderSize);
    if (rectBottom.contains(pt)) {
        return HTBOTTOM;
    }

    return HTCLIENT;
}

#endif

void MuWinWindow::updateScore(int nScore)
{
    _userPtr->updateScore(nScore);
}
