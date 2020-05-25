#include "cresult.h"

CResult::CResult(QWidget *parent): CAllWindows(parent), result_tare(this), result_defect(this)
{
    SAllWindowsInit init_struct;
    init_struct.direction_frame = SAllWindowsInit::edHorizontal;
    init_struct.direction_widget = SAllWindowsInit::edHorizontal;
    init_struct.rewindow_time_all = 100;
    init_struct.rewindow_kol_step = 3;
    init_struct.frame_height = 35;
    init_struct.frame_width = 0;
    SetInitStruct(init_struct);

    QPixmap pix0("0.bmp");
    AddWidget(&result_tare, "weight", pix0);
    QPixmap pix1("1.bmp");
    AddWidget(&result_defect, "tread", pix1);

    QObject::connect(&result_tare, SIGNAL(IAmReSize()), this, SLOT(IAmReSizeFromTare()));
    QObject::connect(&result_tare, SIGNAL(DragImageTare(int)), this, SLOT(DragImageFromTare(int)));
    QObject::connect(&result_tare, SIGNAL(CompressXImageTare(int)), this, SLOT(CompressXImageFromTare(int)));
    QObject::connect(&result_tare, SIGNAL(CompressYImageTare(int)), this, SLOT(CompressYImageFromTare(int)));

    QObject::connect(&result_defect, SIGNAL(IAmReSize()), this, SLOT(IAmReSizeFromTare()));
    QObject::connect(&result_defect, SIGNAL(DragImageDefect(int)), this, SLOT(DragImageFromTare(int)));
    QObject::connect(&result_defect, SIGNAL(CompressXImageDefect(int)), this, SLOT(CompressXImageFromTare(int)));
    QObject::connect(&result_defect, SIGNAL(CompressYImageDefect(int)), this, SLOT(CompressYImageFromTare(int)));
}
void CResult::SetVisible(bool v)
{
    result_tare.SetVisible(v);
    result_defect.SetVisible(v);
}
void CResult::SetImageBack(QImage &ib, QRect &r)
{
    QRect r2(r.x(), r.y()+35, r.width(), r.height()-35);
    result_tare.SetImageBack(ib, r2);
    result_defect.SetImageBack(ib, r2);
}
