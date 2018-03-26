#ifndef CIMAGE_H
#define CIMAGE_H

#include <QWidget>
#include <QPainter>
#include <QtGui>
#include "CStackFourth.h"
#include "cparam.h"
#define max_size 1000


class CImage : public QWidget
{
    Q_OBJECT
public:
    enum eState{esVesOnly, esPictureOnly, esVesAndPicture};

    explicit CImage(eState s, QWidget *parent = 0);

    void paintEvent(QPaintEvent *p);
    void resizeEvent(QResizeEvent *r);

    void ClearImage();

    eState GetState()const {return state;}
    int GetWidth()const {return width() - 40;}
    int GetHeight()const {return height();}
    QPointF* GetPtrData(const unsigned &n) {return data[n];}
    bool* GetBoolMyVan() {return my_van;}
    void SetVes(const unsigned &n, const QString &str) {ves[n] = str;}
    void SetImageBack(QImage &ib, QRect &r1, QRect &r2);// {image_back = ib; Paint();}
    void Paint();
private:
    eState state;
    QPointF data[4][max_size];
    bool my_van[max_size];
  //  QPointF data_real[max_size];
    QString ves[100];    
 //   bool timer_state;
    QImage image, image_back;
    QPainter painter;
    int begin_mouse_x;
    int begin_mouse_compress_x, begin_mouse_compress_y;

  //  void timerEvent(QTimerEvent *event);
protected:
    virtual void mousePressEvent(QMouseEvent *pe);
    virtual void mouseDoubleClickEvent(QMouseEvent *pe);
    virtual void mouseMoveEvent(QMouseEvent *pe);
    virtual void mouseReleaseEvent(QMouseEvent *pe);
    virtual void wheelEvent(QWheelEvent *pe);
    virtual void enterEvent(QEvent *);
    virtual void leaveEvent(QEvent *);
signals:
    void Drag(int s);
    void WheelX(int s);
    void WheelY(int s);
    void IAmReSize();
    void DoubleClick();

public slots:

};

#endif // CIMAGE_H
