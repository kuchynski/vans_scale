#include "cresultdefect.h"
#include <QBoxLayout>

CResultDefect::CResultDefect(QWidget *parent) :
    QWidget(parent), image(CImage::esPictureOnly, this), image_defect(this), check_axel(this)
{
    QBoxLayout *pbx = new QBoxLayout(QBoxLayout::TopToBottom);
    pbx->setMargin(0);
    pbx->setSpacing(0);
    QWidget::setLayout(pbx);

    image_defect.setMaximumHeight(220);
    image_defect.setMinimumHeight(220);
    pbx->addWidget(&image_defect, 1);
    QObject::connect(&image_defect, SIGNAL(IAmReSize()), this, SLOT(OnReSizeImageDefect()));

    CChecks::SButtonsInit chek_init;
    chek_init.direction = CChecks::SButtonsInit::edHorizontal;
    chek_init.font = this->font();
    chek_init.font.setPixelSize(15);
    chek_init.color_backgraund = QColor(0, 30, 185, 255);
    chek_init.color_backgraund_checked = QColor(100, 100, 255, 255);
    chek_init.color_backgraund2 = QColor(185, 30, 185, 255);
    chek_init.color_backgraund_checked2 = QColor(255, 100, 255, 255);
  //  chek_init.color_backgraund2 = QColor(250, 30, 50, 255);
//    chek_init.color_backgraund_checked2 = QColor(255, 100, 100, 255);
    chek_init.color_text = Qt::white;
    chek_init.color_text_checked = Qt::red;
    check_axel.setMaximumHeight(40);
    check_axel.setMinimumHeight(40);
    check_axel.SetInitStruct(chek_init);
  //  QObject::connect(&check_axel, SIGNAL(PressButton(unsigned,bool)), this, SLOT(OnCheckedAxel(unsigned,bool)));
    QObject::connect(&check_axel, SIGNAL(IAmReSize()), this, SLOT(OnReSizeCheckAxel()));
    pbx->addWidget(&check_axel, 2);

    pbx->addWidget(&image, 3);
    QObject::connect(&image, SIGNAL(Drag(int)), this, SLOT(DragImage(int)));
    QObject::connect(&image, SIGNAL(WheelX(int)), this, SLOT(CompressXImage(int)));
    QObject::connect(&image, SIGNAL(WheelY(int)), this, SLOT(CompressYImage(int)));
    QObject::connect(&image, SIGNAL(DoubleClick()), this, SLOT(DoubleClickImage()));
    QObject::connect(&image, SIGNAL(IAmReSize()), this, SLOT(OnReSizeImage()));

    setAttribute(Qt::WA_PaintOnScreen);
}
void CResultDefect::SetVisible(bool v)
{
    if(!v)
    {
        image_defect.ClearImage();
        image.ClearImage();
    }
    check_axel.setVisible(v);
}
void CResultDefect::OnReSizeImage()
{
    QRect rimage1 = image.rect();
    QRect rimage2(0, image.y(), width(), rimage1.height());
    image.SetImageBack(image_back, rimage1, rimage2);
}
void CResultDefect::OnReSizeImageDefect()
{
    QRect rcheck1 = image_defect.rect();
    QRect rcheck2(0, image_defect.y(), width(), rcheck1.height());
    image_defect.SetImageBack(image_back, rcheck1, rcheck2);
}
void CResultDefect::OnReSizeCheckAxel()
{
    check_axel.SetImageBack(image_back, check_axel.geometry());
}
void CResultDefect::resizeEvent(QResizeEvent *r)
{
    emit IAmReSize();
}
void CResultDefect::SetImageBack(QImage &ib, QRect &r)
{
    image_back = QImage(width(), height(), QImage::Format_RGB32);
    QPainter painter(&image_back);
    painter.drawImage(rect(), ib, r);
    painter.end();
    OnReSizeImage();
    OnReSizeImageDefect();
    OnReSizeCheckAxel();
}
