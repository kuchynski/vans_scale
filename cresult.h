#ifndef CRESULT_H
#define CRESULT_H

#include <QWidget>
#include <QtGui>
#include "CAllWindows.h"
#include "cresulttare.h"
#include "cresultdefect.h"


class CResult : public CAllWindows
{
  //  Q_OBJECT
public:
    explicit CResult(QWidget *parent = 0);
    void SetVisible(bool v);
    void SetImageBack(QImage &ib, QRect &r);
    CImage* GetImageTarePtr() {return result_tare.GetImagePtr();}
    CImage* GetImageRealPtr() {return result_defect.GetImageRealPtr();}
    CImageDefect* GetImageDefectPtr() {return result_defect.GetImageDefectPtr();}
    CButtonsCheck* GetCheckAxel() {return result_defect.GetCheckAxel();}
private:
    CResultTare result_tare;
    CResultDefect result_defect;
signals:

public slots:
    void IAmReSizeFromTare() {emit IAmReSize();}
    void DragImageFromTare(int s) {emit DragImageTare(s);}
    void CompressXImageFromTare(int s){emit CompressXImageTare(s);}
    void CompressYImageFromTare(int s){emit CompressYImageTare(s);}
};

#endif
