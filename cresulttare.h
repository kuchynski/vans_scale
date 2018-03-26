#ifndef CRESULTTARE_H
#define CRESULTTARE_H

#include <QWidget>
#include "cchecks.h"
#include "cimage.h"

class CResultTare : public QWidget
{
    Q_OBJECT
private:
    CImage image;
    QImage image_back;
public:
    explicit CResultTare(QWidget *parent = 0);
    void SetVisible(bool v);
    void SetImageBack(QImage &ib, QRect &r);
    CImage* GetImagePtr() {return &image;}
protected:
    virtual void resizeEvent(QResizeEvent *r);

signals:
    void IAmReSize();
    void DragImageTare(int s);
    void CompressXImageTare(int s);
    void CompressYImageTare(int s);

public slots:
    void DragImage(int s){emit DragImageTare(s);}
    void CompressXImage(int s) {emit CompressXImageTare(s);}
    void CompressYImage(int s){emit CompressYImageTare(s);}
    void OnReSizeImage();
};

#endif
