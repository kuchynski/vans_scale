#include "coptions.h"

COptions::COptions(QWidget *parent) : QWidget(parent), label_about(this)//, buttons_auto_null(this),
{
    QPalette palette7;
    QBrush brush7(QColor(200, 200, 200, 255));
    brush7.setStyle(Qt::SolidPattern);
    palette7.setBrush(QPalette::Active, QPalette::WindowText, brush7);
    palette7.setBrush(QPalette::Inactive, QPalette::WindowText, brush7);
    palette7.setBrush(QPalette::Disabled, QPalette::WindowText, brush7);
    QFont font2;
    font2.setPointSize(12);
    font2.setBold(true);
    font2.setWeight(75);

    CLabelChange::SLabelInit struct_label;
    struct_label.color_backgraund = QColor(0, 30, 185, 255);
    struct_label.color_line = QColor(0, 30, 185, 255);
    struct_label.color_text = QColor(255, 255, 255, 255);
    struct_label.font = font2;
    label_about.SetInitStruct(struct_label);
    label_about.AddString("the weight load indicator");
    label_about.AddString("WLI-2-30");
    label_about.AddString("v 2.1");
    label_about.AddString("kuchynskiandrei@gmail.com");
    label_about.setGeometry(QRect(0, height() - 40, width(), 20));
    label_about.Start();


    CChecks::SButtonsInit chek_init;
    chek_init.style_background = CChecks::SButtonsInit::ebPicture;
    QImage imagea1("OK1.png");
    QImage imagea2("OK2.png");
/*    buttons_auto_null.setGeometry(QRect(300, 100, 60, 60));
    buttons_auto_null.SetInitStruct(chek_init);
    buttons_auto_null.AddButton(QString(""), imagea1, imagea2);
    buttons_auto_null.SetChecked(0, (param.GetAutoSetPBNull()));
    QObject::connect(&buttons_auto_null, SIGNAL(PressButton(unsigned,bool)), this, SLOT(OnCheckedAutosetPbNull(unsigned,bool)));
    QLabel *label_an = new QLabel(this);
    label_an->setGeometry(QRect(260, 50, 200, 21));
    label_an->setPalette(palette7);
    label_an->setFont(font2);
    label_an->setText("autoset");*/

    int b_x[4] = {50, 170, 290, 410};
    int b_y = 50;
    for(int i = 0; i < 4; i++)
    {
        buttons_enabled_pb[i] = new CChecks(this);
        buttons_enabled_pb[i]->setGeometry(QRect(b_x[i], b_y+25, 60, 60));
        buttons_enabled_pb[i]->SetInitStruct(chek_init);
        buttons_enabled_pb[i]->AddButton(QString(""), imagea1, imagea2);
        buttons_enabled_pb[i]->SetChecked(0, param.GetonoffPb(i));
    }
    QObject::connect(buttons_enabled_pb[0], SIGNAL(PressButton(unsigned,bool)), this, SLOT(OnCheckedEnabledPb0(unsigned,bool)));
    QObject::connect(buttons_enabled_pb[1], SIGNAL(PressButton(unsigned,bool)), this, SLOT(OnCheckedEnabledPb1(unsigned,bool)));
    QObject::connect(buttons_enabled_pb[2], SIGNAL(PressButton(unsigned,bool)), this, SLOT(OnCheckedEnabledPb2(unsigned,bool)));
    QObject::connect(buttons_enabled_pb[3], SIGNAL(PressButton(unsigned,bool)), this, SLOT(OnCheckedEnabledPb3(unsigned,bool)));

    QLabel *label_e[5];
    for(int i = 0; i < 4; i ++)
    {
        label_e[i] = new QLabel(this);
        label_e[i]->setGeometry(QRect(b_x[i]+15, b_y, 40, 21));
        label_e[i]->setPalette(palette7);
        label_e[i]->setFont(font2);
        label_e[i]->setText("RB" + QString::number(i+1));
    }

    for(int i = 0; i < 4; i++)
    {
        pb_options[i] = new CPBOptions(true, 350, "", this);
//        pb_options[i] = new CPBOptions(true, 300, "ratio RB" + QString::number(i+1), this);
        pb_options[i]->setGeometry(QRect(b_x[i]-20, b_y+100, 100, 320));
        pb_options[i]->SetKoof(param.GetKoofA(i), param.GetKoofB(i), param.GetKoofC(i), param.GetPorogDefect(i));
        pb_options[i]->SetNS((param.GetNSPb(i))? true : false);
        pb_options[i]->SetVisible((param.GetonoffPb(i)));
    }
    QObject::connect(pb_options[0], SIGNAL(ChangeKoof(unsigned, int)), this, SLOT(OnChangedKoofPB0(unsigned, int)));
    QObject::connect(pb_options[1], SIGNAL(ChangeKoof(unsigned, int)), this, SLOT(OnChangedKoofPB1(unsigned, int)));
    QObject::connect(pb_options[2], SIGNAL(ChangeKoof(unsigned, int)), this, SLOT(OnChangedKoofPB2(unsigned, int)));
    QObject::connect(pb_options[3], SIGNAL(ChangeKoof(unsigned, int)), this, SLOT(OnChangedKoofPB3(unsigned, int)));
    QObject::connect(pb_options[0], SIGNAL(CheckedNSPB(bool)), this, SLOT(OnCheckedNSPB0(bool)));
    QObject::connect(pb_options[1], SIGNAL(CheckedNSPB(bool)), this, SLOT(OnCheckedNSPB1(bool)));
    QObject::connect(pb_options[2], SIGNAL(CheckedNSPB(bool)), this, SLOT(OnCheckedNSPB2(bool)));
    QObject::connect(pb_options[3], SIGNAL(CheckedNSPB(bool)), this, SLOT(OnCheckedNSPB3(bool)));


    CSpinBox::SSpinBoxInit spin_init;
    spin_init.height_buttons = 20;
    spin_init.color_backgraund = QColor(30, 30, 155, 255);
    spin_init.color_backgraund_buttons = QColor(2, 20, 105, 255);
    QFont fo;
    fo.setBold(true);
    fo.setPointSize(16);
    fo.setWeight(30);
    spin_init.font = fo;
    spin_init.height_buttons = 14;
    int sb_y[3] = {b_y+130, b_y+200, b_y+270};
    for(int i = 0; i < 3; i++)
    {
        spin_box_rast[i] = new CSpinBox(this);
        spin_box_rast[i]->setGeometry(QRect(690, sb_y[i], 50, 60));
        spin_box_rast[i]->SetInitStruct(spin_init);
        spin_box_rast[i]->SetMinMax(-150, 150);
        spin_box_rast[i]->SetValue(param.GetRastA(i));
    }
    spin_box_rast[2]->SetMinMax(300, 400);
    spin_box_rast[2]->SetValue(param.GetRastA(2));
    QObject::connect(spin_box_rast[0], SIGNAL(NewValue(int)), this, SLOT(OnRastA0ValueChanged(int)));
    QObject::connect(spin_box_rast[1], SIGNAL(NewValue(int)), this, SLOT(OnRastA1ValueChanged(int)));
    QObject::connect(spin_box_rast[2], SIGNAL(NewValue(int)), this, SLOT(OnRastA2ValueChanged(int)));
    spin_box_k8 = new CSpinBox(this);
    spin_box_k8->setGeometry(QRect(790, sb_y[0], 50, 60));
    spin_box_k8->SetInitStruct(spin_init);
    spin_box_k8->SetMinMax(0, 99);
    spin_box_k8->SetValue(param.GetKoof8());//static_cast<int>((param.GetKoof8()-1.0)*100.0));
    QObject::connect(spin_box_k8, SIGNAL(NewValue(int)), this, SLOT(OnKoof8ValueChanged(int)));
    spin_box_vl80 = new CSpinBox(this);
    spin_box_vl80->setGeometry(QRect(790, sb_y[2], 50, 60));
    spin_box_vl80->SetInitStruct(spin_init);
    spin_box_vl80->SetMinMax(80, 110);
    spin_box_vl80->SetValue(param.GetkoofVl80());//static_cast<int>((param.GetKoof8()-1.0)*100.0));
    QObject::connect(spin_box_vl80, SIGNAL(NewValue(int)), this, SLOT(OnKoofVl80ValueChanged(int)));


    QLabel *label_sb[4];
    int label_sb_y[4] = {b_y, sb_y[0]+20, sb_y[1]+20, sb_y[2]+20};
    for(int i = 0; i < 4; i ++)
    {
        label_sb[i] = new QLabel(this);
        label_sb[i]->setGeometry(QRect(530, label_sb_y[i], 160, 21));
        label_sb[i]->setPalette(palette7);
        label_sb[i]->setFont(font2);
    }
    label_sb[0]->setText("distance ");
    label_sb[1]->setText("SM1 to RB1");
    label_sb[2]->setText("SM2 to RB2");
    label_sb[3]->setText("SM1 to SM2");

    QLabel *label_at = new QLabel(this);
    label_at->setGeometry(QRect(790, b_y-20, 160, 40));
    label_at->setPalette(palette7);
    label_at->setFont(font2);
    label_at->setText("auto\ncalibration");
    QLabel *label_k8 = new QLabel(this);
    label_k8->setGeometry(QRect(790, sb_y[0]-30, 100, 20));
    label_k8->setPalette(palette7);
    label_k8->setFont(font2);
    label_k8->setText("weight ratio 8");
    QLabel *label_vl80 = new QLabel(this);
    label_vl80->setGeometry(QRect(790, sb_y[2]-30, 100, 20));
    label_vl80->setPalette(palette7);
    label_vl80->setFont(font2);
    label_vl80->setText("weight ratio VL-80");

    auto_tarirovra = new CChecks(this);
    auto_tarirovra->setGeometry(QRect(790, b_y+25, 60, 60));
    auto_tarirovra->SetInitStruct(chek_init);
    auto_tarirovra->AddButton(QString(""), imagea1, imagea2);
    auto_tarirovra->SetChecked(0, param.GetAutoTarirovka());
    QObject::connect(auto_tarirovra, SIGNAL(PressButton(unsigned,bool)), this, SLOT(OnCheckedAutoTarirovka(unsigned,bool)));
}
void COptions::NewKoof()
{
    for(int i = 0; i < 4; i++)
        pb_options[i]->SetKoof(param.GetKoofA(i), param.GetKoofB(i), param.GetKoofC(i), param.GetPorogDefect(i));
}
void COptions::SetKoof(unsigned pb, unsigned n, int value)
{
    switch(n)
    {
        case 0: param.SetKoofA(pb,  value); break;
        case 1: param.SetKoofB(pb,  value); break;
        case 2: param.SetKoofC(pb,  value); break;
        case 3: param.SetPorogDefect(pb,  value); break;
    }
}
void COptions::OnChangedKoofPB0(unsigned n, int value)
{
    SetKoof(0, n, value);
}
void COptions::OnChangedKoofPB1(unsigned n, int value)
{
    SetKoof(1, n, value);
}
void COptions::OnChangedKoofPB2(unsigned n, int value)
{
    SetKoof(2, n, value);
}
void COptions::OnChangedKoofPB3(unsigned n, int value)
{
    SetKoof(3, n, value);
}
void COptions::OnRastA0ValueChanged(int value)
{
    param.SetRastA(0, value);
}
void COptions::OnRastA1ValueChanged(int value)
{
    param.SetRastA(1, value);
}
void COptions::OnRastA2ValueChanged(int value)
{
    param.SetRastA(2, value);
}
void COptions::OnKoof8ValueChanged(int value)
{
    param.SetKoof8(value);
}
void COptions::OnKoofVl80ValueChanged(int value)
{
    param.SetKoofVl80(value);
}
void COptions::OnCheckedEnabledPb0(unsigned n, bool s)
{
    param.SetonoffPb(0, (s)? 1 : 0);
    pb_options[0]->SetVisible(s);
}
void COptions::OnCheckedEnabledPb1(unsigned n, bool s)
{
    param.SetonoffPb(1, (s)? 1 : 0);
    pb_options[1]->SetVisible(s);
}
void COptions::OnCheckedEnabledPb2(unsigned n, bool s)
{
    param.SetonoffPb(2, (s)? 1 : 0);
    pb_options[2]->SetVisible(s);
}
void COptions::OnCheckedEnabledPb3(unsigned n, bool s)
{
    param.SetonoffPb(3, (s)? 1 : 0);
    pb_options[3]->SetVisible(s);
}
void COptions::OnCheckedAutoTarirovka(unsigned n, bool s)
{
    param.SetAutoTarirovka(s);
}
void COptions::OnCheckedNSPB0(bool s)
{
    param.SetNSPb(0, (s)? 1 : 0);
}
void COptions::OnCheckedNSPB1(bool s)
{
    param.SetNSPb(1, (s)? 1 : 0);
}
void COptions::OnCheckedNSPB2(bool s)
{
    param.SetNSPb(2, (s)? 1 : 0);
}
void COptions::OnCheckedNSPB3(bool s)
{
    param.SetNSPb(3, (s)? 1 : 0);
}

void COptions::OnCheckedAutosetPbNull(unsigned n, bool s)
{
    param.SetAutoSetPBNull((s)? 1 : 0);
}
//---------------------------------------------
void COptions::paintEvent(QPaintEvent *p)
{
    if(!QWidget::isVisible())
        return;
    painter.begin(this);
    painter.drawImage(p->rect(), image_my, p->rect());
    painter.end();
}
void COptions::resizeEvent(QResizeEvent *r)
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
//    painter.setPen(QPen(cg));
    painter.setBrush(gradient);
    painter.drawRect(rect1);
    painter.setPen(QPen(Qt::white));
    painter.setBrush(Qt::NoBrush);
    painter.drawRoundedRect(QRect(510, 150, 250, 300), 5, 5);
    painter.end();

    label_about.setGeometry(QRect(0, height() - 40, width(), 20));
    label_about.SetImageBack(image_my, label_about.rect(), QRect(0, height() - 40, width(), 20));
    auto_tarirovra->SetImageBack(image_my, auto_tarirovra->geometry());
    for(int i = 0; i < 4; i++)
        buttons_enabled_pb[i]->SetImageBack(image_my, buttons_enabled_pb[i]->geometry());
}
