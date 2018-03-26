#include "cmonitor.h"

CMonitor::CMonitor(QWidget *parent) : QWidget(parent)
{
    ClearData();
    begin_y[0] = 0;
    begin_y[1] = 100;
    end_y[0] = begin_y[1]-1;
    end_y[1] = hieght_image;
    size_y[0] = end_y[0] - begin_y[0];
    size_y[1] = end_y[1] - begin_y[1];
    center_y[0] = (end_y[0] + begin_y[0]) / 2;
    center_y[1] = (end_y[1] + begin_y[1]) / 2;
    begin_x = 0;
    end_x = width_image;
    size_x = end_x - begin_x;
    center_x = (end_x + begin_x) / 2;
}
void CMonitor::SetComPort(CComPort *cp)
{
    com_port = cp;
    timer_id = QObject::startTimer(param.GetMonitorCompressX());
}

void CMonitor::Paint()
{
    if(!visiable)
        return;
    update();
}
void CMonitor::paintEvent(QPaintEvent *p)
{
    if(!visiable)
        return;

     QColor color_datchik[6] = {QColor(255, 255, 0, 255),
                                QColor(180, 180, 180, 230),
                                QColor(0, 255, 255, 210),
                                QColor(255, 0, 250, 190),
                                QColor(255, 0, 0, 190),
                                QColor(255, 0, 0, 190)};
     const unsigned number_datchik = param.GetMonitorValue();
     const int compressy = param.GetMonitorCompressY();
     const int pb_null = (number_datchik <= 3)? param.GetPbNull(number_datchik) : 0;

     QPainter painter(this);
     painter.drawImage(0, 0, image_back);

     for(int i = 0; i < size_x; i ++)
     {
        if(number_datchik <= 3)
        {
            int y = center_y[1] - (data_pb[i][number_datchik] - pb_null)/compressy;
            if(y > end_y[1])
                y = end_y[1];
            if(y < begin_y[1])
                y = begin_y[1];
            data[i].setY(y);
        }
        else if(number_datchik <= 5)
           data[i].setY(center_y[1] - data_dm[i][number_datchik-4]);
     }
     painter.setPen(QPen(color_datchik[number_datchik]));
     painter.drawPolyline(data, size_x);
     painter.setPen(QPen(Qt::white));
   //  —Œ—“ﬂÕ»≈
     {
         {
             painter.drawText(QPointF(begin_x + 50, center_y[0] + 5), QString::fromLocal8Bit("—¬ﬂ«‹"));
             painter.drawText(QPointF(center_x - 80, begin_y[0] + 25), QString::fromLocal8Bit("œ¡1"));
             painter.drawText(QPointF(center_x - 30, begin_y[0] + 25), QString::fromLocal8Bit("œ¡2"));
             painter.drawText(QPointF(center_x - 80, end_y[0] - 25), QString::fromLocal8Bit("œ¡3"));
             painter.drawText(QPointF(center_x - 30, end_y[0] - 25), QString::fromLocal8Bit("œ¡4"));
             painter.drawText(QPointF(center_x - 130, end_y[0] - 25), QString::fromLocal8Bit("ƒÃ1"));
             painter.drawText(QPointF(center_x + 20, end_y[0] - 25), QString::fromLocal8Bit("ƒÃ2"));
         }
         painter.setPen(QPen(Qt::darkGreen));
         painter.setBrush(QBrush((state_bloc.enabled)? Qt::green : Qt::black));
         painter.drawEllipse(QRect(begin_x + 25, center_y[0]-7, 15, 15));
         painter.setBrush(QBrush((state_bloc.connect_pb & 0x01)? Qt::green : Qt::black));
         painter.drawEllipse(QRect(center_x - 100, begin_y[0] + 15, 10, 10));
         painter.setBrush(QBrush((state_bloc.connect_pb & 0x02)? Qt::green : Qt::black));
         painter.drawEllipse(QRect(center_x - 50, begin_y[0] + 15, 10, 10));
         painter.setBrush(QBrush((state_bloc.connect_pb & 0x04)? Qt::green : Qt::black));
         painter.drawEllipse(QRect(center_x - 100, end_y[0] - 35, 10, 10));
         painter.setBrush(QBrush((state_bloc.connect_pb & 0x08)? Qt::green : Qt::black));
         painter.drawEllipse(QRect(center_x - 50, end_y[0] - 35, 10, 10));

         painter.setPen(QPen(Qt::darkRed));
         painter.setBrush(QBrush((state_bloc.state_pedal & 0x01)? Qt::red : Qt::black));
         painter.drawEllipse(QRect(center_x - 150, end_y[0] - 35, 10, 10));
         painter.setBrush(QBrush((state_bloc.state_pedal & 0x02)? Qt::red : Qt::black));
         painter.drawEllipse(QRect(center_x + 0, end_y[0] - 35, 10, 10));

         QFont font("Courier New", 15, 20);
         font.setBold(true);
         painter.setFont(font);
         painter.setPen(QPen(Qt::white));
         if(state_bloc.direction == 2)
            painter.drawText(QPointF(center_x + 80, center_y[0]-10), QString::fromLocal8Bit("<"));
         else if(state_bloc.direction == 1)
            painter.drawText(QPointF(center_x + 210, center_y[0]-10), QString::fromLocal8Bit(">"));
         painter.setPen(QPen(Qt::green));
         if(state_bloc.speed)
            painter.drawText(QPointF(center_x + 110, center_y[0]-10), QString::number((int)((double)state_bloc.speed * 3.6)) + QString::fromLocal8Bit(" ÍÏ/˜   "));
         if(state_bloc.kol_of_van)
         {
             QString s1 = QString::fromLocal8Bit(" ‚‡„ÓÌÓ‚");
             int end_ch1 = state_bloc.kol_of_van % 10;
             if(end_ch1 == 1)
                 s1 = QString::fromLocal8Bit(" ‚‡„ÓÌ");
             else if((state_bloc.kol_of_van < 5 || state_bloc.kol_of_van > 21) && (end_ch1 > 1 && end_ch1 < 5))
                 s1 = QString::fromLocal8Bit(" ‚‡„ÓÌ‡");

             painter.drawText(QPointF(center_x + 90, center_y[0]+25), QString::number(state_bloc.kol_of_van) + s1);
         }
     }

    painter.end();
}

void CMonitor::SetVisible(bool v)
{
    visiable = v;
}

void CMonitor::resizeEvent(QResizeEvent *r)
{
    QWidget::resizeEvent(r);
}

void CMonitor::ClearData()
{
    for(int i = 0; i < width_image; i ++)
    {
        data_pb[i] = 0;
        data_dm[i] = 0;
    }
    ClearImage();
}
void CMonitor::ClearImage()
{
    for(int i = 0; i < width_image; i ++)
    {
        data[i].setX(i);
        data[i].setY(-1);
    }
    Paint();
}
void CMonitor::NewParam()
{
    QObject::killTimer(timer_id);
   // if(param.GetMonitorValue())
        timer_id = QObject::startTimer(5+param.GetMonitorCompressX());
 //   else
   // {
      //  timer_id = QObject::startTimer(100);
    //    ClearImage();
   // }
}
void CMonitor::NewPBNull()
{
    SStateBloc sb;
    com_port->GetData(sb);
    state_bloc = sb;
    param.SetPbNull(state_bloc.pb_null);    
}
void CMonitor::timerEvent(QTimerEvent *event)
{
    for(int i = 1; i < width_image; i ++)
    {
        data_pb[i-1] = data_pb[i];
        data_dm[i-1] = data_dm[i];
    }
    SStateBloc sb;
    com_port->GetData(sb);
    state_bloc = sb;
    data_pb[width_image-1] = state_bloc.data_pb;
    data_dm[width_image-1] = state_bloc.data_dm;
//double koof = 0.3;
//SElementFourth<int> pb_null = param.GetPbNull();
//tmp_ves = (int)((double)(abs(state_bloc.pb_null[2] - pb_null[2])) * koof);
    Paint();
}
void CMonitor::mouseDoubleClickEvent(QMouseEvent *pe)
{
    if(pe->y() >= begin_y[1])
        emit ClickGraph();
}
void CMonitor::SetImageBack(QImage &ib, const QRect &r)
{
    image_back = QImage(width(), height(), QImage::Format_RGB32);
    QPainter painter(&image_back);
    painter.drawImage(rect(), ib, r);
    painter.end();
    Paint();
}
void CMonitor::wheelEvent(QWheelEvent *pe)
{
    int numDegrees = pe->delta() / 8;
    int numSteps = numDegrees / 5;
    if(pe->buttons() == Qt::LeftButton)
        emit WheelX(-numSteps / 3);
    else
        emit WheelY(-numSteps / 3);
}
