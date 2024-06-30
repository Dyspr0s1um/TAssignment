#include "roomwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QScreen>


RoomWindow::RoomWindow(const QString &roomName, const QString &treeType, QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle(roomName + " - " + treeType);

    // Tree label
    treeLabel = new QLabel(this);
    setTreeImage(treeType);
    treeLabel->setAlignment(Qt::AlignCenter);


    // Start focus button
    startFocusButton = new QPushButton("开始专注", this);
    startFocusButton->setStyleSheet("QPushButton { background-color: #52bacc;font-size:20px} ");

    connect(startFocusButton, &QPushButton::clicked, this, &RoomWindow::onStartFocusClicked);


    // Back button
    backButton = new QPushButton("返回", this);
    backButton->setStyleSheet("QPushButton { background-color: #52bacc;font-size:20px} ");

    connect(backButton, &QPushButton::clicked, this, &RoomWindow::onBackButtonClicked);

    // Focus time duration combo box
    focusTimeComboBox = new QComboBox(this);
    focusTimeComboBox->addItem("30 分钟", QVariant(30));
    focusTimeComboBox->addItem("60 分钟", QVariant(60));
    focusTimeComboBox->addItem("90 分钟", QVariant(90));
    focusTimeComboBox->addItem("120 分钟", QVariant(120));
    focusTimeComboBox->addItem("150 分钟", QVariant(150));
    focusTimeComboBox->addItem("180 分钟", QVariant(180));
    focusTimeComboBox->addItem("自定义");
    focusTimeComboBox->setStyleSheet(
        "QComboBox { background-color: #52bacc; color: white; }"
        "QComboBox QAbstractItemView { background-color: #a2d4e6; }"
        );
    connect(focusTimeComboBox, QOverload<int>::of(&QComboBox::activated), this, &RoomWindow::onDurationChanged);

    // Layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *bottomLayout = new QHBoxLayout;


    bottomLayout->addWidget(focusTimeComboBox);
    bottomLayout->addWidget(startFocusButton);

    mainLayout->addWidget(treeLabel);
    mainLayout->addLayout(bottomLayout);
    mainLayout->addWidget(backButton);

    setLayout(mainLayout);
}


void RoomWindow::closeEvent(QCloseEvent *event)
{
    emit roomClosed();
    QWidget::closeEvent(event);
}

void RoomWindow::onBackButtonClicked()
{
    emit backToMain();
    close();
}

void RoomWindow::onStartFocusClicked()
{
    // 根据选择的时间设置倒计时时长
    if (focusTimeComboBox->currentIndex() >= 0) {
        int index = focusTimeComboBox->currentIndex();
        int durationMinutes;
        if (focusTimeComboBox->itemData(index).isValid()) {
            durationMinutes = focusTimeComboBox->itemData(index).toInt();
        } else {
            // 自定义时长逻辑
            bool ok;
            durationMinutes = QInputDialog::getInt(this, "自定义专注时间", "请输入自定义专注时间（分钟）：", 25, 1, 9999, 1, &ok);
            if (!ok)
                return;
        }

        // 获取屏幕的大小
        QScreen *screen = QGuiApplication::primaryScreen();
        QRect screenGeometry = screen->geometry();

        // 创建全屏窗口显示专注倒计时
        focusWindow = new QWidget();
        focusWindow->setGeometry(screenGeometry);
        focusWindow->showFullScreen(); // 设置为全屏显示

        // 设置背景图片
        QString backgroundImage = ":/background/background.png"; // 请确保已将图片添加到资源文件中
        QPalette palette;
        palette.setBrush(focusWindow->backgroundRole(), QBrush(QPixmap(backgroundImage).scaled(screenGeometry.size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
        focusWindow->setPalette(palette);
        focusWindow->setAutoFillBackground(true);
        // 创建主布局
        QVBoxLayout *mainLayout = new QVBoxLayout(focusWindow);

        // 创建顶部布局用于放置退出按钮
        QHBoxLayout *topLayout = new QHBoxLayout();
        exitButton = new QPushButton("退出专注", focusWindow);
        exitButton->setStyleSheet("font-size: 18px;");
        connect(exitButton, &QPushButton::clicked, this, &RoomWindow::onExitFocusClicked);
        topLayout->addWidget(exitButton, 0, Qt::AlignRight | Qt::AlignTop);

        // 创建中心布局用于放置倒计时标签
        timerLabel = new QLabel(focusWindow);
        timerLabel->setAlignment(Qt::AlignCenter); // 居中对齐
        timerLabel->setStyleSheet("font-size: 48px;"); // 设置字体大小

        // 创建底部布局用于放置AI聊天按钮
        QHBoxLayout *bottomLayout = new QHBoxLayout();
        aiChatLinkButton = new QPushButton("进入AI聊天", focusWindow);
        aiChatLinkButton->setStyleSheet("font-size: 18px;"); // 设置按钮字体大小
        connect(aiChatLinkButton, &QPushButton::clicked, this, &RoomWindow::openAiChatUrl);
        bottomLayout->addWidget(aiChatLinkButton, 0, Qt::AlignCenter);

        // 将所有布局添加到主布局
        mainLayout->addLayout(topLayout);
        mainLayout->addStretch(); // 添加伸缩空间，使倒计时标签位于中央
        mainLayout->addWidget(timerLabel);
        mainLayout->addStretch(); // 添加伸缩空间，使AI聊天按钮位于底部
        mainLayout->addLayout(bottomLayout);

        remainingTime = durationMinutes * 60; // 转换为秒数

        // 创建倒计时定时器
        focusTimer = new QTimer(this);
        connect(focusTimer, &QTimer::timeout, this, &RoomWindow::updateTimer);
        focusTimer->start(1000); // 每秒更新一次
    }
}


void RoomWindow::updateTimer()
{
    QString timeString = QString("%1:%2")
                             .arg(remainingTime / 60, 2, 10, QChar('0'))
                             .arg(remainingTime % 60, 2, 10, QChar('0'));
    timerLabel->setText("倒计时：" + timeString);

    remainingTime--;
    if (remainingTime < 0) {
        focusTimer->stop();
        focusWindow->close();
    }
}

void RoomWindow::setTreeImage(const QString &treeType)
{
    QString imagePath;
    // 根据树种类型设置图片路径
    if (treeType == "蓝花楹") {
        imagePath = ":/photos/photo1.png";
    } else if (treeType == "黄金风铃木") {
        imagePath = ":/photos/photo2.png";
    } else if (treeType == "橘子树") {
        imagePath = ":/photos/photo3.png";
    } else if (treeType == "荷花") {
        imagePath = ":/photos/photo4.png";
    } else if (treeType == "水仙花") {
        imagePath = ":/photos/photo5.png";
    } else if (treeType == "波斯菊") {
        imagePath = ":/photos/photo6.png";
    } else if (treeType == "梅花") {
        imagePath = ":/photos/photo7.png";
    } else if (treeType == "郁金香") {
        imagePath = ":/photos/photo8.png";
    } else if (treeType == "百合花") {
        imagePath = ":/photos/photo9.png";
    } else if (treeType == "女巫蘑菇") {
        imagePath = ":/photos/photo9.png";
    } else if (treeType == "情人树") {
        imagePath = ":/photos/photo10.png";
    } else if (treeType == "圣代树") {
        imagePath = ":/photos/photo11.png";
    } else if (treeType == "玫瑰") {
        imagePath = ":/photos/photo12.png";
    } else if (treeType == "向日葵") {
        imagePath = ":/photos/photo13.png";
    } else if (treeType == "糖果树") {
        imagePath = ":/photos/photo14.png";
    } else if (treeType == "竹") {
        imagePath = ":/photos/photo15.png";
    } else if (treeType == "紫藤") {
        imagePath = ":/photos/photo16.png";
    } else if (treeType == "西瓜") {
        imagePath = ":/photos/photo17.png";
    }

    if (!imagePath.isEmpty()) {
        treeLabel->setPixmap(QPixmap(imagePath).scaled(200, 200, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}



void RoomWindow::openAiChatUrl()
{
    // 打开AI聊天的链接
    QDesktopServices::openUrl(QUrl("https://chat18.aichatos8.com/#/"));
}

void RoomWindow::onDurationChanged(int index)
{
    if (index >= 0) {
        QVariant data = focusTimeComboBox->itemData(index);
        if (data.isValid() && data.toString() == "自定义") {
            // 自定义时长逻辑
            bool ok;
            int durationMinutes = QInputDialog::getInt(this, "自定义专注时间", "请输入自定义专注时间（分钟）：", 25, 1, 9999, 1, &ok);
            if (!ok)
                return;

            // 更新界面或执行其他逻辑
        }
    }
}


void RoomWindow::onExitFocusClicked()
{
    focusTimer->stop();
    focusWindow->close();
    this->show();
}
