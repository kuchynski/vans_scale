#ifndef CIMAGEDEFECT_H
#define CIMAGEDEFECT_H

#include <QWidget>
#include <QPainter>
#include <QtGui>
#include "CStackFourth.h"
#include "cparam.h"
#define max_size 1000


class CImageDefect : public QWidget
{
    Q_OBJECT
public:
    explicit CImageDefect(QWidget *parent = 0);

    void paintEvent(QPaintEvent *p);
    void resizeEvent(QResizeEvent *r);

    void ClearImage();

    unsigned GetWidth()const {return width()/2;}
    unsigned GetHeight()const {return height()/3;}
    QPointF* GetPtrData1(const unsigned &n) {return data1[n];}
    QPointF* GetPtrData2(const unsigned &n) {return data2[n];}
    void SetPorog(const unsigned &n, const unsigned &p) {if(n < 4) porog[n] = p;}
    void SetDefectSize(const unsigned &n, const unsigned &p) {if(n < 4) defect_size[n] = p;}
    void SetImageBack(QImage &ib, QRect &r1, QRect &r2);
    void Paint();
private:
    QImage image, image_back;
    QPointF data1[4][max_size];
    QPointF data2[4][max_size];
    unsigned porog[4];
    unsigned defect_size[4];
    QPainter painter;
protected:
signals:
    void IAmReSize();

public slots:

};

#endif
