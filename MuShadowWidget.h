﻿

#ifndef MUSHADOWWIDGET_H
#define MUSHADOWWIDGET_H

#include <QWidget>

class MuSkin9GridImage
{
protected:
    QImage m_img;
    QRect m_arrayImageGrid[9];
    //
    bool clear();
public:
    static bool splitRect(const QRect& rcSrc, QPoint ptTopLeft, QRect* parrayRect, int nArrayCount);
    bool setImage(const QImage& image, QPoint ptTopLeft);
    void drawBorder(QPainter* p, QRect rc) const;
};


class  MuShadowWidget : public QWidget
{
    Q_OBJECT
public:
    MuShadowWidget(int shadowSize, bool canResize, QWidget *parent = nullptr);

    virtual void paintEvent(QPaintEvent *e);

private:
    MuSkin9GridImage* m_shadow;
    int m_shadowSize;
};

#endif // MUSHADOWWIDGET_H
