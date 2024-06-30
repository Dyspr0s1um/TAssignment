#ifndef CREATEROOMDIALOG_H
#define CREATEROOMDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include<QComboBox>
#include<QLabel>

class CreateRoomDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CreateRoomDialog(QWidget *parent = nullptr);

    QString getRoomName() const;
    QString getSelectedTreeType() const;


private slots:
    void onOkClicked();

private:
    QLineEdit *roomNameEdit;
    QPushButton *okButton;
    QVBoxLayout *layout;
    QComboBox *treeTypeComboBox;
    QLabel *roomNameLabel;
    QLabel *treeTypeLabel;
signals:
};

#endif // CREATEROOMDIALOG_H
