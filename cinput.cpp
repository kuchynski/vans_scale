#include "cinput.h"

CInput::CInput(QWidget *parent): QWidget(parent),
    check_dat(this),  check_port(this),
    com_port(0), monitor(this),
    visiable(true),
    now_exit(false), dir_path(QDir::current().absolutePath())
{
    monitor.setGeometry(QRect(50, 200, 500, 300));
    QObject::connect(&monitor, SIGNAL(ClickGraph()), this, SLOT(on_center_monitor_clicked()));
    QObject::connect(&monitor, SIGNAL(WheelX(int)), this, SLOT(on_dial_x_monitor_valueChanged(int)));
    QObject::connect(&monitor, SIGNAL(WheelY(int)), this, SLOT(on_dial_y_monitor_valueChanged(int)));

    QPalette palette8;
    QBrush brush8(QColor(0, 30, 255, 255));
    palette8.setBrush(QPalette::Active, QPalette::Button, brush8);
    palette8.setBrush(QPalette::Inactive, QPalette::Button, brush8);
    palette8.setBrush(QPalette::Disabled, QPalette::Button, brush8);
    QPalette palette7;
    QBrush brush7(QColor(245, 245, 255, 255));
    brush7.setStyle(Qt::SolidPattern);
    palette7.setBrush(QPalette::Active, QPalette::WindowText, brush7);
    palette7.setBrush(QPalette::Inactive, QPalette::WindowText, brush7);
    palette7.setBrush(QPalette::Disabled, QPalette::WindowText, brush7);


    check_dat.setGeometry(QRect(50, 510, 500, 50));
    CChecks::SButtonsInit chek_init;
    chek_init.direction = CChecks::SButtonsInit::edHorizontal;
    chek_init.font = this->font();
    chek_init.font.setPixelSize(15);
    chek_init.color_backgraund = QColor(0, 30, 185, 255);
    chek_init.color_backgraund_checked = QColor(100, 100, 255, 255);
    chek_init.color_text = Qt::white;
    check_dat.SetInitStruct(chek_init);
    check_dat.AddButton(CChecks::SButton("RB1"));
    check_dat.AddButton(CChecks::SButton("RB2"));
    check_dat.AddButton(CChecks::SButton("RB3"));
    check_dat.AddButton(CChecks::SButton("RB4"));
    check_dat.AddButton(CChecks::SButton("SM1"));
    check_dat.AddButton(CChecks::SButton("SM2"));
    check_dat.SetChecked(param.GetMonitorValue());
    QObject::connect(&check_dat, SIGNAL(PressButton(unsigned,bool)), this, SLOT(on_checked_datChanged(unsigned,bool)));

    label_com_state = new QLabel(this);
    label_com_state->setGeometry(QRect(50, 50, 300, 51));
    QFont font4;
   // font4.setFamily(QString::fromUtf8("Courier"));
    font4.setPointSize(30);
    label_com_state->setFont(font4);
   // label_com_state->setFrameShape(QFrame::Box);
   // label_com_state->setFrameShadow(QFrame::Raised);
  //  label_com_state->setLineWidth(3);
 //   label_com_state->setAlignment(Qt::AlignCenter);
    label_com_state->setPalette(palette7);

    on_checked_portChanged(param.GetComNumber(), true);
    check_port.setGeometry(QRect(50, 120, 900, 50));
    check_port.SetInitStruct(chek_init);
    check_port.AddButton(CChecks::SButton("NONE"));
    check_port.AddButton(CChecks::SButton("COM1"));
    check_port.AddButton(CChecks::SButton("COM2"));
    check_port.AddButton(CChecks::SButton("COM3"));
    check_port.AddButton(CChecks::SButton("COM4"));
    check_port.AddButton(CChecks::SButton("COM5"));
    check_port.AddButton(CChecks::SButton("COM6"));
    check_port.AddButton(CChecks::SButton("COM7"));
    check_port.AddButton(CChecks::SButton("COM8"));
    check_port.AddButton(CChecks::SButton("COM9"));
    check_port.AddButton(CChecks::SButton("----"));
    check_port.SetChecked(param.GetComNumber());
    QObject::connect(&check_port, SIGNAL(PressButton(unsigned,bool)), this, SLOT(on_checked_portChanged(unsigned,bool)));

    monitor.SetComPort(&com_port);
    QObject::connect(&com_port, SIGNAL(NewState(unsigned)), this, SLOT(OnNewComPortState(unsigned)));
    QObject::connect(&com_port, SIGNAL(NewSostav()), this, SLOT(OnNewSostav()));
    com_port.SetNumberPort(param.GetComNumber());
    com_port.start(QThread::NormalPriority);
    monitor.SetVisible(visiable);
    on_checked_datChanged(param.GetMonitorValue(), true);

    process_reload = new QProcess(this);
    QObject::connect(process_reload, SIGNAL(finished(int)), this, SLOT(OnEndReload(int)));
}
CInput::~CInput()
{
    now_exit = true;
    com_port.SetNumberPort(100);
    com_port.wait();
}
void CInput::OnNewComPortState(unsigned s)
{
    if(now_exit)
        return;
    switch(s)
    {
        case CComPort::cpsDisConnect:
            label_com_state->setText("---- ------¸-");
            QObject::killTimer(timer_new_second);
            break;
        case CComPort::cpsConnect:
            label_com_state->setText("connect");
            timer_new_second = QObject::startTimer(1000);
            break;
        case CComPort::cpsConnect1:
            label_com_state->setText("connect");
            break;
        case CComPort::cpsConnect2:
            label_com_state->setText("connect2");
            break;
        case CComPort::cpsReload:
            label_com_state->setText("reload");
            process_reload->start(dir_path + "/usb_reload.bat");
            break;
        case CComPort::cpsError:
            label_com_state->setText("error");
            break;
        case CComPort::cpsNoState:
            label_com_state->setText("");
            break;
        default:
            label_com_state->setText(QString(""));
    }
    //monitor.ClearImage();
}
void CInput::OnNewSostav()
{
    emit NewSostav();
}
void CInput::OnEndReload(int res)
{
    on_checked_portChanged(param.GetComNumber()+100, true);
}
void CInput::on_checked_datChanged(unsigned n, bool tmp)
{
    param.SetMonitorValue(n);
    monitor.NewParam();
}
void CInput::on_checked_portChanged(unsigned n, bool tmp)
{
    int n2 = (n > 100)? n - 100 : n;
    com_port.SetNumberPort(n);
    param.SetComNumber(n2);
}
void CInput::SetVisiable(bool v)
{
    visiable = v;
    monitor.SetVisible(visiable);
}
void CInput::on_center_monitor_clicked()
{
    monitor.NewPBNull();
   // ui->lcdNumber_value_monitor->display(param.GetPbNull(param.GetMonitorValue()-1));
}
void CInput::on_dial_x_monitor_valueChanged(int value)
{
    int tmp = param.GetMonitorCompressX();
    int tmp2 = tmp + value;
    if(tmp2 <= 0)
        tmp2 = 1;
    if(tmp2 > 100)
        tmp2 = 100;
    if(tmp == tmp2)
        return;
    param.SetMonitorCompressX(tmp2);
    monitor.NewParam();
}
void CInput::on_dial_y_monitor_valueChanged(int value)
{
    int tmp = param.GetMonitorCompressY();
    int tmp2 = tmp + value;
    if(tmp2 <= 0)
        tmp2 = 1;
    if(tmp2 > 300)
        tmp2 = 300;
    if(tmp == tmp2)
        return;
    param.SetMonitorCompressY(tmp2);
    monitor.NewParam();
}
//---------------------------------------------
void CInput::paintEvent(QPaintEvent *p)
{
    if(!QWidget::isVisible())
        return;
    painter.begin(this);
    painter.drawImage(p->rect(), image_my, p->rect());
    painter.end();
}
void CInput::resizeEvent(QResizeEvent *r)
{
    image_my = QImage(width(), height(), QImage::Format_RGB32);
    painter.begin(&image_my);
    QLinearGradient gradient(0, 0, 0, height());
    QRect rect1(0, 0, width(), height());
    painter.setBrush(Qt::gray);
    painter.drawRect(rect1);
    QColor cg = QColor(0, 30, 185, 255);
    int stap_gr[8] = {160, 220, 220, 220, 220, 220, 255};
    for(int i = 0; i < 8; i ++)
    {
        cg.setAlpha(stap_gr[i]);
        gradient.setColorAt((double)i / 6.0, cg);
    }
    painter.setPen(Qt::NoPen);
    painter.setBrush(gradient);
    painter.drawRect(rect1);
    painter.end();
    check_dat.SetImageBack(image_my, check_dat.geometry());
    check_port.SetImageBack(image_my, check_port.geometry());
    monitor.SetImageBack(image_my, monitor.geometry());
}


