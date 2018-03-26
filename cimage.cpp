#include "cimage.h"

CImage::CImage(eState s, QWidget *parent) : QWidget(parent), state(s)
{
    ClearImage();
 //   QObject::startTimer(500);
    QWidget::setAttribute(Qt::WA_PaintOnScreen);
}
void CImage::ClearImage()
{
    for(int i = 0; i < 100; i ++)
        ves[i] = "";
    for(int n = 0; n < 4; n ++)
        for(int i = 0; i < max_size; i ++)
            data[n][i] = QPointF(0,0);
    Paint();
}
void CImage::Paint()
{
    image = image_back;
    painter.begin(&image);
    int space = 20;
    const int size_y[2] = {240, (state == esPictureOnly)? height() - 2*space : height() - 240 - 3*space};
    const int begin_y[2] = {space, (state == esPictureOnly)? space : size_y[0] + 2*space};
    const int end_y[2] = {size_y[0] + space, height() - space};
    const int begin_x = space;
    const int end_x = width() - space;
    const int size_x = end_x - begin_x;

    //_______________________ВЕС__________________________

    painter.setRenderHint(QPainter::Antialiasing, true);

    SElementFourth<unsigned> mask = param.GetArchonoffPb();
    //if(ves[0] != "")
    if(state == esVesAndPicture || state == esVesOnly)
    {
        QString linux_str1 = QString::fromLocal8Bit("информация о вагоне");
        QString linux_str2 = QString::fromLocal8Bit("информация о составе");
        QFont font("Courier New", 11, 20);
        font.setBold(true);
        painter.setFont(font);
        int pos_y = space + 10;
        const int step_y = 14;
        painter.setBrush(QBrush(Qt::NoBrush));
        painter.setPen(QPen(Qt::white));
        painter.drawRoundedRect(QRect(begin_x, begin_y[0], size_x, size_y[0]), 30, 30);
        painter.drawText(QPointF(begin_x + 15, pos_y+=step_y), linux_str2);
        painter.drawText(QPointF(begin_x + 15, pos_y+=step_y), ves[12]);
        painter.drawText(QPointF(begin_x + 15, pos_y+=step_y), ves[13]);
        painter.drawText(QPointF(begin_x + 15, pos_y+=step_y), linux_str1);// + ves[0]);
        for(int i = 1; i < 8; i++)
            painter.drawText(QPointF(begin_x + 15, pos_y+=step_y), ves[i]);
        for(int i = 8; i < 12; i++)
            if(mask[i-8])
                painter.drawText(QPointF(begin_x + 15, pos_y+=step_y), ves[i]);
    }

    if(state == esVesAndPicture || state == esPictureOnly)//_______________________ГРАФИК__________________________
    {
        const int y_null = begin_y[1] + size_y[1] / 3;
        painter.setPen(QPen(Qt::white));
     //   if(ves[0] != "")
        {
            painter.drawLine(begin_x+10, y_null, end_x - 10, y_null);
            painter.setBrush(QBrush(Qt::NoBrush));
            painter.setPen(QPen(Qt::white));
            painter.drawRoundedRect(QRect(begin_x, begin_y[1], size_x, size_y[1]), 30, 30);

            QColor cm = QColor(100, 200, 255, 70);
            painter.setPen(QPen(cm));
            for(int j = 0; j < size_x; j ++)
                if(my_van[j])
      //          painter.drawLine(j+begin_x, begin_y[1] + 30, j+begin_x, end_y[1] - 30);
                    painter.drawLine(j+begin_x, y_null+20, j+begin_x, y_null-20);

            QColor color_pb[4] = {QColor(255, 255, 0, 255),
                                  QColor(180, 180, 180, 230),
                                  QColor(0, 255, 255, 210),
                                  QColor(255, 0, 250, 190)};
            QPointF data_real[max_size];
            for(int i = 0; i < 4; i ++)
                if(mask[i])
                {
                    for(int j = 0; j < size_x; j ++)
                    {

                        qreal tmp = data[i][j].ry() + y_null;
                        if(tmp <= begin_y[1]) tmp = begin_y[1] + 2;
                        if(tmp >= end_y[1]) tmp = end_y[1] - 1;
                        data_real[j].setY(tmp);
                        data_real[j].setX(data[i][j].rx()+ begin_x);
                    }
                    painter.setPen(QPen(color_pb[i], 2));
                    painter.drawPolyline(data_real, size_x);
                }

        }
    }
    painter.setPen(QPen(Qt::white));
   // painter.drawText(QPointF(begin_x + 15, begin_y[2] + 15), QString::number(width()));
    painter.end();
    update();
}
void CImage::paintEvent(QPaintEvent *p)
{
  //  QWidget::paintEvent(p);
    if(!QWidget::isVisible())
        return;
    painter.begin(this);
    painter.drawImage(p->rect(), image, p->rect());
    painter.end();
}
void CImage::resizeEvent(QResizeEvent *r)
{
  //  QWidget::resizeEvent(r);
    IAmReSize();
    Paint();
}
//void CImage::timerEvent(QTimerEvent *event)
//{
    //timer_state = !timer_state;
  //  Paint();
//}
//___________________MOUSE_____________________________________MOUSE
void CImage::mousePressEvent(QMouseEvent *pe)
{
  //  QWidget::mousePressEvent(pe);
    if(pe->button() == Qt::LeftButton)
        begin_mouse_x = pe->x();
    if(pe->button() == Qt::RightButton)
    {
        begin_mouse_compress_x = pe->x();
        begin_mouse_compress_y = pe->y();
    }
}
void CImage::mouseMoveEvent(QMouseEvent *pe)
{
   // QWidget::mouseMoveEvent(pe);
    Qt::MouseButtons r = pe->buttons();
    if(pe->buttons() == Qt::LeftButton)
        if(abs(pe->x() - begin_mouse_x) >= 10)
        {
            emit Drag(pe->x() - begin_mouse_x);
            begin_mouse_x = pe->x();
        }
    if(pe->buttons() == Qt::RightButton)
    {
        if(abs(pe->y() - begin_mouse_compress_y) >= 5)
        {
            emit WheelY(-(pe->y() - begin_mouse_compress_y)/5);
            begin_mouse_compress_y = pe->y();
        }
        if(abs(pe->x() - begin_mouse_compress_x) >= 5)
        {
            emit WheelX(-(pe->x() - begin_mouse_compress_x)/5);
            begin_mouse_compress_x = pe->x();
        }
    }
}
void CImage::mouseReleaseEvent(QMouseEvent *pe)
{
    QWidget::mouseReleaseEvent(pe);
    //if(pe->button() == Qt::LeftButton)
        begin_mouse_x = 0;
//    if(pe->button() == Qt::RightButton)
    {
        begin_mouse_compress_x = 0;
        begin_mouse_compress_y = 0;
    }
}
void CImage::mouseDoubleClickEvent(QMouseEvent *pe)
{
    QWidget::mouseDoubleClickEvent(pe);
    emit DoubleClick();
}
void CImage::wheelEvent(QWheelEvent *pe)
{
    QWidget::wheelEvent(pe);
    int numDegrees = pe->delta() / 8;
    int numSteps = numDegrees / 5;
    if(pe->buttons() == Qt::LeftButton)
        emit WheelX(-numDegrees/3);
    else
        emit WheelY(-numDegrees);
}
void CImage::enterEvent(QEvent *pe)
{
    QWidget::enterEvent(pe);
//    setMouseTracking(true);
}
void CImage::leaveEvent(QEvent *pe)
{
    QWidget::leaveEvent(pe);
//    QWidget::leaveEvent(pe);
  //  setMouseTracking(false);
}
void CImage::SetImageBack(QImage &ib, QRect &r1, QRect &r2)
{
    image_back = QImage(width(), height(), QImage::Format_RGB32);
//    image_back = QImage(r1.width(), r1.height(), QImage::Format_RGB32);
    QPainter painter(&image_back);
    painter.drawImage(r1, ib, r2);
    painter.end();
    Paint();
}


