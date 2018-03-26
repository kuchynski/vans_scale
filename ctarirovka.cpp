#include "ctarirovka.h"

CTarirovka::CTarirovka(QWidget *parent) : QWidget(parent), buttons(this), buttons_work(this),
    table_vans(this), pic_ves(CImage::esVesOnly, this)
{
    CTable::STableInit table_init;
    table_init.color_backgraund = QColor(0, 30, 185, 255);
    table_init.color_backgraund_checked = QColor(100, 150, 255, 255);
    table_init.color_backgraund_button = QColor(0, 30, 185, 255);
    table_init.color_line_checked = QColor(200, 200, 255, 255);
    table_init.color_text_checked = QColor(255, 255, 0, 255);
    table_init.color_text_title = QColor(255, 255, 0, 255);
    table_init.color_backgraund_title = QColor(0, 0, 200, 255);
    table_init.font = this->font();
    table_init.font.setPixelSize(12);
    table_init.hieght_line = 20;
    QList<unsigned> width_line2;
    width_line2 << 25 << 39 << 61 << 43 << 43 << 43;
    table_init.width_line = width_line2;
    table_vans.SetInitStruct(table_init);
    QList<QString> t2;
    t2 << QString::fromLocal8Bit("№") << QString::fromLocal8Bit("Осей") << QString::fromLocal8Bit("Скорость") <<
          QString::fromLocal8Bit("Вес") << QString::fromLocal8Bit("Вес1") << QString::fromLocal8Bit("Вес2");
    table_vans.SetTitle(t2);
    QObject::connect(&table_vans, SIGNAL(ClickLine(unsigned)), this, SLOT(TableVanClicked(unsigned)));

    CButtons::SButtonsInit but_init;
    but_init.font = this->font();
    but_init.font.setPixelSize(15);
    but_init.color_backgraund = QColor(0, 30, 185, 255);
    but_init.color_backgraund_checked = QColor(100, 100, 255, 255);
    but_init.color_text = Qt::white;
    buttons.SetInitStruct(but_init);
    buttons.AddButton(CButtons::SButton(QString::fromLocal8Bit("ok")));
    buttons.AddButton(CButtons::SButton(QString::fromLocal8Bit("отменить")));
    QObject::connect(&buttons, SIGNAL(PressButton(unsigned, bool)), this, SLOT(OnButtonsClick(unsigned, bool)));
    buttons.Paint();

    buttons_work.SetInitStruct(but_init);
    buttons_work.AddButton(CButtons::SButton(QString::fromLocal8Bit("пересчитать")));
    buttons_work.AddButton(CButtons::SButton(QString::fromLocal8Bit("установить в системные")));
    buttons_work.AddButton(CButtons::SButton(QString::fromLocal8Bit("установить из системных")));
    QObject::connect(&buttons_work, SIGNAL(PressButton(unsigned, bool)), this, SLOT(OnButtonsWorkClick(unsigned, bool)));
    buttons_work.Paint();

    setAttribute(Qt::WA_PaintOnScreen);



    QFont font2;
    font2.setPointSize(12);
    font2.setBold(true);
    font2.setWeight(75);
    QPalette palette7;
    QBrush brush7(QColor(245, 245, 255, 255));
    brush7.setStyle(Qt::SolidPattern);
    palette7.setBrush(QPalette::Active, QPalette::WindowText, brush7);
    palette7.setBrush(QPalette::Inactive, QPalette::WindowText, brush7);
    palette7.setBrush(QPalette::Disabled, QPalette::WindowText, brush7);

    int b_x[4] = {510, 630, 750, 870};
  /*  for(int i = 0; i < 4; i ++)
    {
        label_e[i] = new QLabel(this);
        label_e[i]->setGeometry(QRect(b_x[i]+25, 300, 40, 21));
        label_e[i]->setPalette(palette7);
        label_e[i]->setFont(font2);
        label_e[i]->setText(QString::fromLocal8Bit("ПБ") + QString::number(i+1));
    }
*/
    for(int i = 0; i < 4; i++)
    {
      //  pb_options[i] = new CPBOptions(false, 250, QString::fromLocal8Bit(""), this);
        pb_options[i] = new CPBOptions(false, 250, QString::fromLocal8Bit("кооф. ПБ") + QString::number(i+1), this);
        pb_options[i]->setGeometry(QRect(b_x[i], 275, 100, 250));
    }
    QObject::connect(pb_options[0], SIGNAL(ChangeKoof(unsigned, int)), this, SLOT(OnChangedKoofPB0(unsigned, int)));
    QObject::connect(pb_options[1], SIGNAL(ChangeKoof(unsigned, int)), this, SLOT(OnChangedKoofPB1(unsigned, int)));
    QObject::connect(pb_options[2], SIGNAL(ChangeKoof(unsigned, int)), this, SLOT(OnChangedKoofPB2(unsigned, int)));
    QObject::connect(pb_options[3], SIGNAL(ChangeKoof(unsigned, int)), this, SLOT(OnChangedKoofPB3(unsigned, int)));


    pic_ves.setVisible(true);
  //  pic_ves.setGeometry(QRect(520, 0, weight() - 520, 220));

}
void CTarirovka::Begin(CSostav &s)
{
    sostav = s;
    SElementFourth<double> a = sostav.GetKoof_A();
    SElementFourth<double> b = sostav.GetKoof_B();
    SElementFourth<int> c = sostav.GetKoof_C();
    for(int i = 0; i < 4; i++)
    {
        koof_a[i] = (unsigned)(a[i] * 10000.0);
        koof_b[i] = (unsigned)(b[i] * 1000.0);
        koof_c[i] = c[i];// / 10;
        pb_options[i]->SetKoof(koof_a[i], koof_b[i], koof_c[i], 0);
    }
    for(int i = 0; i < 4; i++)
    {
     //   label_e[i]->setVisible((param.GetonoffPb(i)));
        pb_options[i]->SetVisible((param.GetonoffPb(i)));
    }
    PicSostav();
}
void CTarirovka::PicSostav()
{
    QList<QString> l;
    sostav.PaintSostav(l);
    int tmp;
    sostav.SetActiv(&table_vans, &pic_ves, &pic_ves, tmp, false);
}
void CTarirovka::TableVanClicked(unsigned row)
{
    int tmp;
    sostav.SetActivVan(&pic_ves, &pic_ves, tmp, row);
}
void CTarirovka::SetKoof(unsigned pb, unsigned n, int value)
{
    switch(n)
    {
        case 0: koof_a[pb] = value; break;
        case 1: koof_b[pb] = value; break;
        case 2: koof_c[pb] = value; break;
    }
}
void CTarirovka::OnChangedKoofPB0(unsigned n, int value)
{
    SetKoof(0, n, value);
}
void CTarirovka::OnChangedKoofPB1(unsigned n, int value)
{
    SetKoof(1, n, value);
}
void CTarirovka::OnChangedKoofPB2(unsigned n, int value)
{
    SetKoof(2, n, value);
}
void CTarirovka::OnChangedKoofPB3(unsigned n, int value)
{
    SetKoof(3, n, value);
}
void CTarirovka::OnButtonsClick(unsigned n, bool s)
{
    switch(n)
    {
        case 0: emit EndTarirovka(sostav, true); break;
        case 1: emit EndTarirovka(sostav, false); break;
    }
    for(int i = 0; i < 4; i++)
        pb_options[i]->SetVisible(false);
}
void CTarirovka::OnButtonsWorkClick(unsigned n, bool s)
{
    switch(n)
    {
        case 0:
            Work();
            PicSostav();
            break;
        case 1:
            emit NewKoof(koof_a, koof_b, koof_c);
            break;
        case 2:
            for(int i = 0; i < 4; i++)
            {
                koof_a[i] = param.GetKoofA(i);
                koof_b[i] = param.GetKoofB(i);
                koof_c[i] = param.GetKoofC(i);
                pb_options[i]->SetKoof(koof_a[i], koof_b[i], koof_c[i], 0);
            }
            break;
    }
}
void CTarirovka::Work()
{
    SElementFourth<double> a;
    SElementFourth<double> b;
    SElementFourth<int> c;
    for(int i = 0; i < 4; i ++)
    {
        a[i] = 0.0001 * static_cast<double>(koof_a[i]);
        b[i] = 0.001 * static_cast<double>(koof_b[i]);
    }
    sostav.SetKoofA(a);
    sostav.SetKoofB(b);
    sostav.SetKoofC(koof_c);// * 10);
    sostav.Run();
}
void CTarirovka::OnReSizeTableVan()
{
    QRect r = QRect(230, 0, 260, height());
    table_vans.setGeometry(r);
    table_vans.SetImageBack(image_my, r);
}
void CTarirovka::OnReSizeImage()
{
    QRect r(490, 0, width() - 490, 260);
    pic_ves.setGeometry(r);
    QRect r1 = pic_ves.rect();
    pic_ves.SetImageBack(image_my, r1, r);
}
void CTarirovka::OnReSizeButtons2()
{
    buttons_work.setGeometry(QRect(625, 550, 230, 90));
    buttons_work.SetImageBack(image_my, buttons_work.geometry());
}
void CTarirovka::OnReSizeButtons()
{
    buttons.setGeometry(QRect(0, height()-60, 230, 60));
    buttons.SetImageBack(image_my, buttons.geometry());
}
void CTarirovka::paintEvent(QPaintEvent *p)
{
    if(!QWidget::isVisible())
        return;
    painter.begin(this);
    painter.drawImage(p->rect(), image_my, p->rect());
    painter.end();
}
void CTarirovka::resizeEvent(QResizeEvent *r)
{
//    if(!QWidget::isVisible())
  //      return;
  //  QWidget::resizeEvent(r);
    image_my = QImage(width(), height(), QImage::Format_RGB32);
    painter.begin(&image_my);
    QLinearGradient gradient(0, 0, 0, height());
    QRect rect1(0, 0, width(), height());
    painter.setBrush(Qt::gray);
    painter.drawRect(rect1);
    QColor cg = QColor(0, 30, 185, 255);
    int stap_gr[8] = {160, 220, 220, 220, 220, 220, 255};
    for(int i = 0; i < 7; i ++)
    {
        cg.setAlpha(stap_gr[i]);
        gradient.setColorAt((double)i / 6.0, cg);
    }
    painter.setPen(QPen(cg));
    painter.setBrush(gradient);
    painter.drawRect(rect1);
    painter.end();

    OnReSizeTableVan();
    OnReSizeImage();
    OnReSizeButtons2();
    OnReSizeButtons();
}
