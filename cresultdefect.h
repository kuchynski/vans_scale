#ifndef CRESULTDEFECT_H
#define CRESULTDEFECT_H

#include <QWidget>
#include "cimagedefect.h"
#include "cimage.h"
#include "cbuttonscheck.h"

class CResultDefect : public QWidget
{
    Q_OBJECT
private:
    CImage image;
    CButtonsCheck check_axel;
    CImageDefect image_defect;
    QImage image_back;
public:
    explicit CResultDefect(QWidget *parent = 0);
    void SetVisible(bool v);
    void SetImageBack(QImage &ib, QRect &r);
    CImageDefect* GetImageDefectPtr() {return &image_defect;}
    CImage* GetImageRealPtr() {return &image;}
    CButtonsCheck* GetCheckAxel() {return &check_axel;}
protected:
    virtual void resizeEvent(QResizeEvent *r);
signals:
    void IAmReSize();
    void DragImageDefect(int s);
    void CompressXImageDefect(int s);
    void CompressYImageDefect(int s);
    void DoubleClickImageDefect();
//    void OnCheckedAxelDefect(unsigned n, bool s);

public slots:
  //  void OnCheckedAxel(unsigned n, bool s){emit OnCheckedAxelDefect(n, s);}
    void DoubleClickImage(){emit DoubleClickImageDefect();}
    void DragImage(int s){emit DragImageDefect(s);}
    void CompressXImage(int s) {emit CompressXImageDefect(s);}
    void CompressYImage(int s){emit CompressYImageDefect(s);}
    void OnReSizeImage();
    void OnReSizeImageDefect();
    void OnReSizeCheckAxel();
};

#endif // CRESULTDEFECT_H
