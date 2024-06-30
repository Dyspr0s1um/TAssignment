#include "userregwnd.h"
#include "ui_userregwnd.h"
#include <QFileDialog>
#include <QBuffer>
#include "MuCustomWindow.h"
#include "user_table.h"
#include "user_tableRepository.h"

QImage UserRegWnd::byteArrayToImage(const QByteArray &byteArray) {
    QImage image;
    image.loadFromData(byteArray);
    return image;
}


QByteArray UserRegWnd::imageToByteArray(const QImage &image) {
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG"); // 可以根据需要保存为其他格式，如 "JPEG" 等
    return byteArray;
}

UserRegWnd::UserRegWnd(QWidget *parent)
    : QWidget{parent}
    , ui(new Ui::UserRegWnd)
{
    ui->setupUi(this);
    setFixedSize(800,560);
    ui->label_img->setFixedSize(128,128);
    ui->lineEdit_desc->setFixedHeight(48);
    ui->lineEdit_name->setFixedHeight(48);
    ui->lineEdit_pwd1->setFixedHeight(48);
    ui->lineEdit_pwd2->setFixedHeight(48);


    connect(ui->btn_select, &QPushButton::clicked, [this]() {
        QString filePath = QFileDialog::getOpenFileName(this, "Select Image", "", "Images (*.png *.jpg)");

        if (!filePath.isEmpty()) {
            QImage image(filePath);
            if (image.isNull()) {
                MuCustomMessageBox::showError(nullptr, "Warning", "Failed to load the image.");
                return;
            }

            // 调整图片大小以适应标签
            QPixmap pixmap = QPixmap::fromImage(image);
            QSize labelSize = ui->label_img->size();
            QPixmap scaledPixmap = pixmap.scaled(labelSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

            ui->label_img->setPixmap(scaledPixmap);
            ui->label_img->setScaledContents(true);

            _imgBt = imageToByteArray(image);
        }
    });
    connect(ui->btn_add, &QPushButton::clicked, [this]() {
        if(ui->lineEdit_desc->text().isEmpty()){
            MuCustomMessageBox::showError(nullptr, "Warning", "desc is empty");
            return;
        }
        if(ui->lineEdit_name->text().isEmpty()){
            MuCustomMessageBox::showError(nullptr, "Warning", "name is empty");
            return;
        }
        if(ui->lineEdit_pwd1->text().isEmpty()){
            MuCustomMessageBox::showError(nullptr, "Warning", "pwd is empty");
            return;
        }
        if(ui->lineEdit_pwd1->text()!=ui->lineEdit_pwd2->text()){
            MuCustomMessageBox::showError(nullptr, "Warning", "two pwd is not equal");
            return;
        }


        TriModel::user_table *usr=new TriModel::user_table();
        usr->setuser_desc(ui->lineEdit_desc->text());
        usr->setuser_img(this->_imgBt);
        usr->setuser_name(ui->lineEdit_name->text());
        usr->setuser_pwd(ui->lineEdit_pwd1->text());
        qint32 id=usr->insert();
        if(0!=id){
            usr->setid(id);
            TriRepository::user_tableModelMgr::getInstance()->appendModel(usr);
        }else{
            if("19"==usr->lastError().nativeErrorCode()){
                MuCustomMessageBox::showError(nullptr, "Warning", QStringLiteral("账号重复注册"));
            }
            usr->deleteLater();
        }
    });
}
void UserRegWnd::updateUserInfo(TriModel::user_table *usr)
{

    QPixmap pixmap = QPixmap::fromImage(byteArrayToImage(usr->user_img()));
    QSize labelSize = ui->label_img->size();
    QPixmap scaledPixmap = pixmap.scaled(labelSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    ui->label_img->setPixmap(scaledPixmap);
    ui->label_img->setScaledContents(true);

    ui->lineEdit_name->setText(usr->user_name());
    ui->lineEdit_desc->setText(usr->user_desc());
}

UserRegWnd::~UserRegWnd()
{
    delete ui;
}

void UserRegWnd::closeEvent(QCloseEvent *)
{
    Q_EMIT closeSig();
}
