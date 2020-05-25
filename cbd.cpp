#include "cbd.h"
#include <QBoxLayout>
#include <QPrintDialog>

CBD::CBD(QWidget *parent): QWidget(parent),
    calendar(this), buttons(this), table_sostavs(this), table_vans(this), result(this), check_pb(this),
    file_path(QDir::current().absolutePath()), proc_pdf(this) //image(CImage::esVesAndPicture, this), check_pb(this)
{
    QBoxLayout *pbx = new QBoxLayout(QBoxLayout::LeftToRight);
    QBoxLayout *pbx1 = new QBoxLayout(QBoxLayout::TopToBottom);
    QBoxLayout *pbx2 = new QBoxLayout(QBoxLayout::TopToBottom);
    QBoxLayout *pbx3 = new QBoxLayout(QBoxLayout::TopToBottom);
    pbx->setMargin(0);
    pbx1->setMargin(0);
    pbx2->setMargin(0);
    pbx3->setMargin(0);
    pbx->setSpacing(0);
    pbx1->setSpacing(0);
    pbx2->setSpacing(0);
    pbx3->setSpacing(0);
    w1 = new QWidget();
    w2 = new QWidget();
    w3 = new QWidget();
    w1->setLayout(pbx1);
    w2->setLayout(pbx2);
    w3->setLayout(pbx3);
    w1->setMinimumWidth(230);
    w1->setMaximumWidth(230);
    w2->setMinimumWidth(260);
    w2->setMaximumWidth(260);
    pbx->addWidget(w1, 1);
    pbx->addWidget(w2, 2);
    pbx->addWidget(w3, 2);
    QWidget::setLayout(pbx);

    calendar.setMaximumHeight(150);
    calendar.setMinimumHeight(150);
    calendar.setFirstDayOfWeek(Qt::Monday);
    calendar.setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    QPalette palette = calendar.palette();
    palette.setColor(QPalette::Base, QColor(0, 30, 185, 100));
    palette.setColor(QPalette::Text, QColor(255, 255, 255, 255));
    palette.setColor(QPalette::AlternateBase, QColor(0, 0, 200, 155));
    //palette.setColor(QPalette::ToolTipText, QColor(255, 255, 0, 255));
    calendar.setPalette(palette);
    QObject::connect(&calendar, SIGNAL(selectionChanged()), this, SLOT(CalendarChanged()));
    pbx1->addWidget(&calendar, 1);
    calendar.show();

    CTable::STableInit table_init;
    table_init.color_backgraund = QColor(0, 30, 185, 255);
//    table_init.color_backgraund_checked = QColor(150, 155, 255, 255);
    table_init.color_backgraund_checked = QColor(100, 150, 255, 255);
    table_init.color_backgraund_button = QColor(0, 30, 185, 255);
    table_init.color_line_checked = QColor(200, 200, 255, 255);
    table_init.color_text_checked = QColor(255, 255, 0, 255);
    table_init.color_text_title = QColor(255, 255, 0, 255);
//    table_init.color_backgraund_button = QColor(50, 100, 255, 255);
    table_init.color_backgraund_title = QColor(0, 0, 200, 255);
    table_init.font = this->font();
    table_init.font.setPixelSize(12);
    table_init.hieght_line = 20;
    QList<unsigned> width_line;
    width_line << 50 << 45 << 40 << 40 << 50;
    table_init.width_line = width_line;
    table_sostavs.SetInitStruct(table_init);
    QList<QString> t;
    t << "time" << "dir." << "loc." << "car" << "weight";
    table_sostavs.SetTitle(t);
    QObject::connect(&table_sostavs, SIGNAL(ClickLine(unsigned)), this, SLOT(TableSostavClicked(unsigned)));
    QObject::connect(&table_sostavs, SIGNAL(IAmReSize()), this, SLOT(OnReSizeTableSostav()));
    pbx1->addWidget(&table_sostavs, 2);
    table_sostavs.resize(300, 2000);

    QList<unsigned> width_line2;
//    width_line2 << 25 << 38 << 61 << 43 << 43 << 43;
    width_line2 << 25 << 40 << 61 << 63 << 64;
    table_init.width_line = width_line2;
    table_vans.SetInitStruct(table_init);
//    table_vans.setFocusPolicy(Qt::ClickFocus);
    QList<QString> t2;
    t2 << "#" << "axes" << "speed" << "weight" << "defect";
    table_vans.SetTitle(t2);
    pbx2->addWidget(&table_vans, 1);
    table_vans.resize(300, 2000);
    QObject::connect(&table_vans, SIGNAL(ClickLine(unsigned)), this, SLOT(TableVanClicked(unsigned)));
    QObject::connect(&table_vans, SIGNAL(IAmReSize()), this, SLOT(OnReSizeTableVan()));

    buttons.setMaximumHeight(180);
    buttons.setMinimumHeight(180);
    CButtons::SButtonsInit but_init;
    but_init.font = this->font();
    but_init.font.setPixelSize(15);
    but_init.color_backgraund = QColor(0, 30, 185, 255);
    but_init.color_backgraund_checked = QColor(100, 100, 255, 255);
//    but_init.color_backgraund2 = QColor(0, 30, 185, 255);
  //  but_init.color_backgraund_checked2 = QColor(100, 100, 255, 255);
    but_init.color_text = Qt::white;
    buttons.SetInitStruct(but_init);
    buttons.AddButton(CButtons::SButton("calibrate"));
    buttons.AddButton(CButtons::SButton("hide"));
    buttons.AddButton(CButtons::SButton("configure printing"));
    buttons.AddButton(CButtons::SButton("print"));
    buttons.AddButton(CButtons::SButton("print pdf"));
    buttons.AddButton(CButtons::SButton("delete"));
    QObject::connect(&buttons, SIGNAL(PressButton(unsigned, bool)), this, SLOT(OnButtonsClick(unsigned, bool)));
    QObject::connect(&buttons, SIGNAL(IAmReSize()), this, SLOT(OnReSizeButtons()));
    pbx1->addWidget(&buttons, 3);
    buttons.Paint();

    check_pb.setMaximumHeight(40);
    check_pb.setMinimumHeight(40);
    CChecks::SButtonsInit chek_init;
    chek_init.direction = CChecks::SButtonsInit::edHorizontal;
    chek_init.font = this->font();
    chek_init.font.setPixelSize(15);
    chek_init.color_backgraund = QColor(0, 30, 185, 255);
    chek_init.color_backgraund_checked = QColor(100, 100, 255, 255);
    chek_init.color_text = Qt::white;
    check_pb.SetInitStruct(chek_init);
    check_pb.AddButton(CChecks::SButton("RB1"));
    check_pb.AddButton(CChecks::SButton("RB2"));
    check_pb.AddButton(CChecks::SButton("RB3"));
    check_pb.AddButton(CChecks::SButton("RB4"));
    for(int i = 0; i < 4; i++)
        check_pb.SetChecked(i, (param.GetArchonoffPb(i)));
    QObject::connect(&check_pb, SIGNAL(PressButton(unsigned,bool)), this, SLOT(OnCheckedPB(unsigned,bool)));
    QObject::connect(&check_pb, SIGNAL(IAmReSize()), this, SLOT(OnReSizeCheckPB()));
    pbx3->addWidget(&result, 1);
    pbx3->addWidget(&check_pb, 2);
    check_pb.Paint();

    QObject::connect(&result, SIGNAL(DragImageTare(int)), this, SLOT(DragImage(int)));
    QObject::connect(&result, SIGNAL(CompressXImageTare(int)), this, SLOT(CompressXImage(int)));
    QObject::connect(&result, SIGNAL(CompressYImageTare(int)), this, SLOT(CompressYImage(int)));
    QObject::connect(&result, SIGNAL(IAmReSize()), this, SLOT(OnResizeResult()));
    image_tare = result.GetImageTarePtr();
    image_real = result.GetImageRealPtr();
    image_defect = result.GetImageDefectPtr();
    check_axel = result.GetCheckAxel();
    QObject::connect(check_axel, SIGNAL(PressButton(unsigned,bool)), this, SLOT(OnCheckedAxel(unsigned,bool)));
    QObject::connect(image_tare, SIGNAL(DoubleClick()), this, SLOT(DoubleClickImage()));
    QObject::connect(image_real, SIGNAL(DoubleClick()), this, SLOT(DoubleClickImage()));

    setAttribute(Qt::WA_PaintOnScreen);
    w1->setAttribute(Qt::WA_PaintOnScreen);
    w2->setAttribute(Qt::WA_PaintOnScreen);
    w3->setAttribute(Qt::WA_PaintOnScreen);
    result.setAttribute(Qt::WA_PaintOnScreen);
    CalendarChanged();

    printer.setOrientation(QPrinter::Portrait);
    printer.setPageSize(QPrinter::A4);
}
void CBD::Clear()
{
    CStack<CSostav>::Clear();
    activ_sostav = -1;
    table_sostavs.Clear(true);
    table_vans.Clear(true);
    result.SetVisible(false);
    check_pb.setVisible(false);
    dial_arch = 0;
    check_axel->SubButton();
}
bool CBD::SetDir(const QDate &_date)
{
    int year = _date.year();
    int mon = _date.month();
    int day = _date.day();
    if(!dir.cd(file_path))
          return false;
    QString dir_name1(QString("%1_%2").arg(year, 4, 10, QLatin1Char('0')).arg(mon, 2, 10, QLatin1Char('0')));
    QString dir_name2(QString("%1").arg(day, 2, 10, QLatin1Char('0')));
    if(!dir.cd(dir_name1))
      return false;
    if(!dir.cd(dir_name2))
      return false;
    tek_file_path = dir.absolutePath();
    return true;
}
void CBD::CalendarChanged()
{
    //ui->groupBox_13->setVisible(bd.GetSize() > 0);
    //ui->groupBox_12->setVisible(bd.GetSize() > 0);
    date = calendar.selectedDate();
    Clear();
    if(!SetDir(date))
    {
        PaintGridSostav();
        return;
    }
    QStringList list(QString("*.so"));
    QStringList sr = dir.entryList(list, QDir::Files);
    for(int i = 0; i < sr.size(); i ++)
    {
        QString file_name = tek_file_path + "/" + sr[i];
        CSostav s;
        try
        {
            s = CSostav(file_name);
        }
        catch(CSostav *cs){}
        if(s.GetQDate() == date)
            Push(s);
    }
    activ_sostav = 0;
    PaintGridSostav();
}
bool CBD::YesOrNoYearMonDay(const QDate &date)
{
    if(!SetDir(date))
        return false;

    QStringList list(QString("*.so"));
    QStringList sr = dir.entryList(list, QDir::Files);

    return sr.size() != 0;
}
//---------------------------------------------
void CBD::PaintGridSostav()
{
    table_sostavs.Clear();
    for(unsigned i = 0; i < GetSize(); i ++)
    {
        QList<QString> l;
        (*this)[i].PaintSostav(l);
        table_sostavs.AddLine(l);
    }
    table_sostavs.SetChecked(0);
    table_sostavs.Paint();
    TableSostavClicked(0);
}
void CBD::SetActivVan(unsigned row)
{
    const unsigned kol_axel = (*this)[activ_sostav].GetSizeVan(row);
    unsigned first_os_with_defect = 1000;
    check_axel->SubButton();
    for(unsigned i = 0; i < kol_axel; i ++)
    {
        unsigned os = ((*this)[activ_sostav].GetDirection())? i : kol_axel - (i+1);
        check_axel->AddButton(CChecks::SButton(QString::number(os+1) + " axel"));
    }
    for(unsigned i = 0; i < kol_axel; i ++)
    {
        unsigned os = ((*this)[activ_sostav].GetDirection())? i : kol_axel - (i+1);
        bool def = (*this)[activ_sostav].GetDefect(row, i, 12);
        check_axel->SetChecked2(os, def);
        if(def && (first_os_with_defect == 1000))
            first_os_with_defect = os;
    }
    if(first_os_with_defect == 1000)
        first_os_with_defect = ((*this)[activ_sostav].GetDirection())? 0 : kol_axel - 1;
    activ_axel = ((*this)[activ_sostav].GetDirection())? first_os_with_defect : kol_axel - (first_os_with_defect + 1);
    check_axel->SetChecked(first_os_with_defect);
    check_axel->Paint();
}
//---------------------------------------------
void CBD::TableSostavClicked(unsigned row, const bool &deactiv)
{
    if(!GetSize())
    {
        Clear();
        buttons.setVisible(false);
//        check_pb.setVisible(false);
        return;
    }
    buttons.setVisible(true);
    result.SetVisible(true);
    check_pb.setVisible(true);
    if(row >= GetSize())
        row = GetSize() - 1;
//    if(row < 0) return;
    if(deactiv)
        if((activ_sostav >= 0) && (activ_sostav < (int)GetSize()))
            (*this)[activ_sostav].SetDeActiv();
    activ_sostav = row;
    (*this)[activ_sostav].SetActiv(&table_vans, image_tare, image_real, dial_arch, true);
    (*this)[activ_sostav].PicDefect(image_defect, activ_axel);
}
//---------------------------------------------
void CBD::TableVanClicked(unsigned row)
{
    if(activ_sostav < 0 || activ_sostav >= (int)GetSize())
        return;
    SetActivVan(row);
    (*this)[activ_sostav].SetActivVan(image_tare, image_real, dial_arch, row);
    (*this)[activ_sostav].PicDefect(image_defect, activ_axel);
}
void CBD::OnCheckedAxel(unsigned n, bool s)
{
    const unsigned kol_axel = (*this)[activ_sostav].GetSizeVan();
    activ_axel = ((*this)[activ_sostav].GetDirection())? n : kol_axel - (n + 1);
    (*this)[activ_sostav].PicDefect(image_defect, activ_axel);
}
//---------------------------------------------
void CBD::RePicGraph()
{
    if(activ_sostav < 0 || activ_sostav >= GetSize())
    {
      //  image.Paint();
        return;
    }
    (*this)[activ_sostav].PicGraphik(image_tare, dial_arch, false, false);
    (*this)[activ_sostav].PicGraphik(image_real, dial_arch, true, false);
    (*this)[activ_sostav].PicDefect(image_defect, activ_axel);
}
//---------------------------------------------------------------------------
bool CBD::AddSostav(CSostav &sostav)
{
    QDir dir_s;
    CDateTime date_time = sostav.GetDateTime();
    int year = date_time.GetYear();
    int mon = date_time.GetMon();
    int day = date_time.GetDay();
    if(!dir_s.cd(file_path))
          return false;
    QString dir_name1(QString("%1_%2").arg(year, 4, 10, QLatin1Char('0')).arg(mon, 2, 10, QLatin1Char('0')));
    QString dir_name2(QString("%1").arg(day, 2, 10, QLatin1Char('0')));
    if(!dir_s.cd(dir_name1))
    {
        if(!dir_s.mkdir(dir_name1))
            return false;
        if(!dir_s.cd(dir_name1))
            return false;
    }
    if(!dir_s.cd(dir_name2))
    {
        if(!dir_s.mkdir(dir_name2))
            return false;
        if(!dir_s.cd(dir_name2))
            return false;
    }
    QString file_name = dir_s.absolutePath() + "/" + date_time.GetTime2();

    bool ret = true;
    if(!sostav.SaveToFile(file_name + ".so"))
        ret = false;
    if(!sostav.SaveToFileTxt(file_name + ".dtv"))
        ret = false;
    if(!sostav.SaveToFileDefectTxt(file_name + ".dtvd"))
        ret = false;
    if(!sostav.SaveToFileBin(file_name + ".bin"))
        ret = false;

    if(ret && (date == sostav.GetQDate()))
    {
        try
        {
            CSostav s(file_name + ".so");
            Push(s);
            QList<QString> l;
            (*this)[GetSize()-1].PaintSostav(l);
            table_sostavs.AddLine(l);
            table_sostavs.Paint();
            if(GetSize() == 1)
                TableSostavClicked(0);
        }
        catch(CSostav *cs){}
    }
    return ret;
}
void CBD::ReturnByTarirovka(const CSostav &s)
{
    if(activ_sostav < 0 || activ_sostav >= GetSize())
        return;
    (*this)[activ_sostav] = s;
    (*this)[activ_sostav].SaveToFile();
    QList<QString> l;
    (*this)[activ_sostav].PaintSostav(l);
    table_sostavs.SetLine(l, activ_sostav, true);
    TableSostavClicked(activ_sostav);
}
void CBD::DoubleClickImage()
{
    param.SetArchTogether(!param.GetArchTogether());
    RePicGraph();
}
//---------------------------------------------
void CBD::OnButtonsClick(unsigned n, bool s)
{
    if(activ_sostav < 0 || activ_sostav >= GetSize())
        return;
    switch(n)
    {
        case 0: emit Tarirovka((*this)[activ_sostav]); break;
        case 1: CloseSostav(); break;
        case 2: PrintDiallog(); break;
        case 3: PrintSostav(); break;
        case 4: PrintPdfSostav(); break;
        case 5: //удалить
            QString delete_file_name = (*this)[activ_sostav].GetFileName();
            CloseSostav();
            QFile::remove(delete_file_name);
            break;
    }
}
void CBD::OnCheckedPB(unsigned n, bool s)
{
    param.SetArchonoffPb(n, (s)? 1 : 0);
    RePicGraph();
}
void CBD::PrintDiallog()
{
    if(activ_sostav < 0 || activ_sostav >= GetSize())
        return;
    QPrintDialog print_diallog(&printer, this);
    print_diallog.setMinMax(1, 1);
    printer.setOrientation(QPrinter::Portrait);
    printer.setPageSize(QPrinter::A4);
    print_diallog.exec();
}
void CBD::PrintSostav()
{
    printer.setOrientation(QPrinter::Portrait);
    printer.setPageSize(QPrinter::A4);
    if(activ_sostav < 0 || activ_sostav >= GetSize())
        return;
    printer.setDocName((*this)[activ_sostav].GetFileName());
    (*this)[activ_sostav].Print(&printer);
}
void CBD::PrintPdfSostav()
{
    QPrinter printer_pdf;
    printer_pdf.setOrientation(QPrinter::Portrait);
    printer_pdf.setPageSize(QPrinter::A4);
    printer_pdf.setOutputFormat(QPrinter::PdfFormat);
    printer_pdf.setOutputFileName((*this)[activ_sostav].GetFileName() + ".pdf");
    printer_pdf.setDocName((*this)[activ_sostav].GetFileName());
    (*this)[activ_sostav].Print(&printer_pdf);
//    proc_pdf.start("cmd");
  //  proc_pdf.start((*this)[activ_sostav].GetFileName() + ".pdf");
}
void CBD::CloseSostav()
{
    (*this).Erase(activ_sostav);
    table_sostavs.SubLine(activ_sostav);
    TableSostavClicked(activ_sostav, false);
    table_sostavs.SetChecked(activ_sostav);
}
void CBD::DragImage(int s)
{
    if(activ_sostav < 0 || activ_sostav >= GetSize())
        return;
    int max_dial = (*this)[activ_sostav].GetSizeData();
    if((*this)[activ_sostav].GetDirection())
        s = -s;
    const double compress_hor = param.GetArchCompressxPb() / 10.0;
    const double tmp = compress_hor * (double)s;
    dial_arch += tmp;
    if(dial_arch < 0)
        dial_arch = 0;
    if(dial_arch >= max_dial)
        dial_arch = max_dial - 1;
    RePicGraph();
}
void CBD::CompressXImage(int s)
{
    if(activ_sostav < 0 || activ_sostav >= GetSize())
        return;
    int cx = param.GetArchCompressxPb() + s;
    if(cx < 1)
        cx = 1;
    if(cx > 100)
        cx = 100;
 //   int begin_pic = dial_arch;
    int center_pic = dial_arch + (image_tare->GetWidth() * param.GetArchCompressxPb()/10) / 2;
  //  int center_pic = (begin_pic + end_pic)/2;
    param.SetArchCompressxPb(cx);
    dial_arch = center_pic - (image_tare->GetWidth() * param.GetArchCompressxPb()/10) / 2;
    RePicGraph();
}
void CBD::CompressYImage(int s)
{
    if(activ_sostav < 0 || activ_sostav >= GetSize())
        return;
    int cy = param.GetArchCompressyPb() + s;
    if(cy < 50)
        cy = 50;
    if(cy > 500)
        cy = 500;
    param.SetArchCompressyPb(cy);
    RePicGraph();
}
//---------------------------------------------
void CBD::paintEvent(QPaintEvent *p)
{
   // QRect rcalendar1 = calendar.rect();
    //painter.begin(this);
    //painter.drawImage(rcalendar1, image_my, rcalendar1);
    //painter.end();

//    QWidget::paintEvent(p);
/*    if(!QWidget::isVisible())
        return;
    painter.begin(this);
    painter.drawImage(p->rect(), image_my, p->rect());
    painter.end();*/
}
void CBD::OnReSizeTableSostav()
{
    QRect r1 = w1->frameGeometry();
    QRect r(r1.x(), r1.y() + calendar.height(), table_sostavs.width(), table_sostavs.height());
    table_sostavs.SetImageBack(image_my, r);
}
void CBD::OnReSizeTableVan()
{
    QRect r2 = w2->frameGeometry();
    QRect r(r2.x(), r2.y(), table_vans.width(), table_vans.height());
    table_vans.SetImageBack(image_my, r);
}
void CBD::OnResizeResult()
{
    QRect r = result.frameGeometry();
    result.SetImageBack(image_my, r);
}
void CBD::OnReSizeCheckPB()
{
    QRect rcheck1 = check_pb.rect();
    QRect r3 = frameGeometry();
    QRect rcheck2(r3.x(), r3.height() - rcheck1.height(), rcheck1.width(), rcheck1.height());
    check_pb.SetImageBack(image_my, rcheck2);
}
void CBD::OnReSizeButtons()
{
    QRect rbuttons1 = buttons.rect();
    QRect rb = w1->frameGeometry();
    QRect rbuttons2(rb.x(), rb.height() - rbuttons1.height(), rbuttons1.width(), rbuttons1.height());
    buttons.SetImageBack(image_my, rbuttons2);
}
void CBD::resizeEvent(QResizeEvent *r)
{
//    if(!QWidget::isVisible())
  //      return;
  //  QWidget::resizeEvent(r);
    image_my = QImage(width(), height(), QImage::Format_RGB32);
    painter.begin(&image_my);
//    QLinearGradient gradient(0, 0, width(), width());//height());
    QLinearGradient gradient(0, 0, 0, height());
    QRect rect1(0, 0, width(), height());
    painter.setBrush(Qt::gray);
    painter.drawRect(rect1);
    QColor cg = QColor(0, 30, 185, 255);
//    QColor cg = QColor(200, 30, 0, 255);
//    int stap_gr[8] = {255, 120, 140, 200, 230, 180, 200, 240};
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

    RePicGraph();
    OnReSizeTableSostav();
    OnReSizeTableVan();
    OnReSizeButtons();
    OnResizeResult();
    OnReSizeCheckPB();
}
