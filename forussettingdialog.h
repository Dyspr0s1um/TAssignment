#ifndef FOCUSSETTINGDIALOG_H
#define FOCUSSETTINGDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QVBoxLayout>
#include <QPushButton>

class FocusSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FocusSettingDialog(QWidget *parent = nullptr);

signals:
    void startFocus(int durationMinutes);

private slots:
    void onOkClicked();

private:
    QVBoxLayout *layout;
    QComboBox *durationComboBox;

    QPushButton *okButton;
};

#endif // FOCUSSETTINGDIALOG_H
