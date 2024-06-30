#ifndef DESKTOPPET_H
#define DESKTOPPET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>

class DesktopPet : public QWidget {
    Q_OBJECT
public:
    explicit DesktopPet(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

signals:
    void petClicked();

private:
    void setupUI();
    bool dragging;
    QPoint dragStartPosition;
};

#endif // DESKTOPPET_H
