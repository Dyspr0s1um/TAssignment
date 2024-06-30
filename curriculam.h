#ifndef CURRICULAM_H
#define CURRICULAM_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>

class Curriculam : public QWidget
{
    Q_OBJECT
public:
    explicit Curriculam(int width, int height, QWidget *parent = nullptr);

private:
    QTableWidget *tableWidget;
    QPushButton *importButton;

signals:

public slots:
};

#endif // CURRICULAM_H
