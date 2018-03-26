#include "ccommander.h"

CCommander::CCommander(QWidget *parent): CAllWindows(parent),
    bd(this), input(this), options(this), tarirovka(this)
{
    tarirovka.setVisible(false);
    QObject::connect(&input, SIGNAL(NewSostav()), this, SLOT(OnNewSostav()));
    QObject::connect(this, SIGNAL(NewWindow(unsigned)), this, SLOT(OnPressButton(unsigned)));
    QObject::connect(&bd, SIGNAL(Tarirovka(CSostav &)), this, SLOT(OnBeginTarirovka(CSostav &)));
    QObject::connect(&tarirovka, SIGNAL(EndTarirovka(CSostav&, bool)), this, SLOT(OnEndTarirovka(CSostav&, bool)));
    QObject::connect(&tarirovka, SIGNAL(NewKoof(SElementFourth<uint>,SElementFourth<uint>,SElementFourth<int>)), this, SLOT(OnNewKoof(SElementFourth<uint>,SElementFourth<uint>,SElementFourth<int>)));
    SAllWindowsInit init_struct;
    init_struct.direction_frame = SAllWindowsInit::edHorizontal;
    init_struct.direction_widget = SAllWindowsInit::edHorizontal;
    init_struct.rewindow_time_all = 200;
    init_struct.rewindow_kol_step = 5;
    init_struct.frame_height = 35;
    init_struct.frame_width = 0;
    SetInitStruct(init_struct);

    QPixmap pix0("0.bmp");
    AddWidget(&bd, QString::fromLocal8Bit("архив"), pix0);
    QPixmap pix1("1.bmp");
    AddWidget(&input, QString::fromLocal8Bit("монитор"), pix1);
    QPixmap pix2("2.bmp");
    AddWidget(&options, QString::fromLocal8Bit("настройки"), pix2);
 //   QPixmap pix3("4.png");
 //   AddWidget(&options, QString::fromLocal8Bit("выход"), pix3);

}
void CCommander::OnNewSostav()
{
    CSostav sostav;
    input.GetSostav(sostav);
    bd.AddSostav(sostav);
}

void CCommander::OnPressButton(const unsigned n)
{
    if(n == 2)
        options.NewKoof();
    else if(n == 3)
        exit(1);
}
void CCommander::OnBeginTarirovka(CSostav &s)
{
    tarirovka.Begin(s);
    QPixmap pix("5.bmp");
    RePlaceWidget(0, &tarirovka, QString::fromLocal8Bit("тарировка"), pix);
}
void CCommander::OnEndTarirovka(CSostav &s, bool res)
{
    if(res)
        bd.ReturnByTarirovka(s);
    QPixmap pix("0.bmp");
    RePlaceWidget(0, &bd, QString::fromLocal8Bit("архив"), pix);
}
void CCommander::OnNewKoof(SElementFourth<unsigned> a, SElementFourth<unsigned> b, SElementFourth<int> c)
{
    param.SetKoofA(a);
    param.SetKoofB(b);
    param.SetKoofC(c);
    options.NewKoof();
}
