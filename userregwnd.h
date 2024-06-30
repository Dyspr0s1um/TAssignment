#ifndef USERREGWND_H
#define USERREGWND_H

#include <QWidget>
#include <QImage>
#include "user_table.h"

namespace Ui {
class UserRegWnd;
}

class UserRegWnd : public QWidget
{
    Q_OBJECT

public:
    explicit UserRegWnd(QWidget *parent = nullptr);
    ~UserRegWnd();
    static QByteArray imageToByteArray(const QImage &image) ;
    static QImage byteArrayToImage(const QByteArray &byteArray) ;
    void updateUserInfo(TriModel::user_table* usr);
Q_SIGNALS:
    void closeSig();
protected:
    void closeEvent(QCloseEvent *event)override;
private:
    Ui::UserRegWnd *ui;
    QByteArray _imgBt;
};

#endif // USERREGWND_H
