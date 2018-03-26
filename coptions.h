#ifndef COPTIONS_H
#define COPTIONS_H

#include <QWidget>
#include <QtGui>
#include "cparam.h"
#include "cchecks.h"
#include "cspinbox.h"
#include "cpboptions.h"
#include "clabelchange.h"

class COptions : public QWidget
{
    Q_OBJECT
private:
    QPainter painter;
    QImage image_my;

  //  CChecks buttons_auto_null;
    CChecks *buttons_enabled_pb[4];
    CSpinBox *spin_box_rast[3];
    CPBOptions *pb_options[4];
    CLabelChange label_about;
    CChecks *auto_tarirovra;
    CSpinBox *spin_box_k8, *spin_box_vl80;

    void SetKoof(unsigned pb, unsigned n, int value);

public:
    explicit COptions(QWidget *parent = 0);

    void NewKoof();
    void paintEvent(QPaintEvent *p);
    void resizeEvent(QResizeEvent *r);
signals:

public slots:
    void OnCheckedNSPB0(bool);
    void OnCheckedNSPB1(bool);
    void OnCheckedNSPB2(bool);
    void OnCheckedNSPB3(bool);
    void OnCheckedAutosetPbNull(unsigned n, bool s);
    void OnCheckedEnabledPb0(unsigned n, bool s);
    void OnCheckedEnabledPb1(unsigned n, bool s);
    void OnCheckedEnabledPb2(unsigned n, bool s);
    void OnCheckedEnabledPb3(unsigned n, bool s);
    void OnCheckedAutoTarirovka(unsigned n, bool s);
    void OnRastA0ValueChanged(int value);
    void OnRastA1ValueChanged(int value);
    void OnRastA2ValueChanged(int value);
    void OnChangedKoofPB0(unsigned n, int value);
    void OnChangedKoofPB1(unsigned n, int value);
    void OnChangedKoofPB2(unsigned n, int value);
    void OnChangedKoofPB3(unsigned n, int value);
    void OnKoof8ValueChanged(int value);
    void OnKoofVl80ValueChanged(int value);
};

#endif // COPTIONS_H
