#ifndef USERSCORELABEL_H
#define USERSCORELABEL_H

#include <QWidget>

namespace Ui {
class userScoreLabel;
}

class userScoreLabel : public QWidget
{
    Q_OBJECT

public:
    explicit userScoreLabel(QWidget *parent = nullptr);
    ~userScoreLabel();
    void updateScore(int);
private:
    Ui::userScoreLabel *ui;
};

#endif // USERSCORELABEL_H
