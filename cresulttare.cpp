#include "cresulttare.h"
#include <QBoxLayout>

CResultTare::CResultTare(QWidget *parent) :
    QWidget(parent), image(CImage::esVesAndPicture, this)
{
    QBoxLayout *pbx = new QBoxLayout(QBoxLayout::TopToBottom);
    pbx->setMargin(0);
    pbx->setSpacing(0);
    setLayout(pbx);

    QObject::connect(&image, SIGNAL(Drag(int)), this, SLOT(DragImage(int)));
    QObject::connect(&image, SIGNAL(WheelX(int)), this, SLOT(CompressXImage(int)));
    QObject::connect(&image, SIGNAL(WheelY(int)), this, SLOT(CompressYImage(int)));
    QObject::connect(&image, SIGNAL(IAmReSize()), this, SLOT(OnReSizeImage()));

    pbx->addWidget(&image, 1);
}
void CResultTare::SetVisible(bool v)
{
    if(!v)
        image.ClearImage();
}
void CResultTare::OnReSizeImage()
{
    QRect rcheck1 = image.rect();
    QRect rcheck2(0, image.y(), width(), rcheck1.height());
    image.SetImageBack(image_back, rcheck1, rcheck2);
}
void CResultTare::resizeEvent(QResizeEvent *r)
{
    emit IAmReSize();
}
void CResultTare::SetImageBack(QImage &ib, QRect &r)
{
    image_back = QImage(width(), height(), QImage::Format_RGB32);
    QPainter painter(&image_back);
    painter.drawImage(rect(), ib, r);
    painter.end();
    OnReSizeImage();
}
