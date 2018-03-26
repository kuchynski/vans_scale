#ifndef CTARIROVKA_H
#define CTARIROVKA_H

#include <QWidget>
#include <QtGui>
#include "CSostav.h"
#include "cbuttons.h"
#include "cpboptions.h"

class CTarirovka : public QWidget
{
    Q_OBJECT
private:
    CSostav sostav;
    CButtons buttons;
    CButtons buttons_work;
    CTable table_vans;
    CImage pic_ves;
    QLabel *label_e[5];
    CPBOptions *pb_options[4];
    SElementFourth<unsigned> koof_a;
    SElementFourth<unsigned> koof_b;
    SElementFourth<int> koof_c;
    QImage image_my;
    QPainter painter;

    void SetKoof(unsigned pb, unsigned n, int value);
    void PicSostav();
    void OnReSizeTableVan();
    void OnReSizeImage();
    void OnReSizeButtons();
    void OnReSizeButtons2();
    void Work();
public:
    explicit CTarirovka(QWidget *parent = 0);

    void Begin(CSostav &s);
    void paintEvent(QPaintEvent *p);
    void resizeEvent(QResizeEvent *r);
public slots:
    void TableVanClicked(unsigned);
    void OnButtonsClick(unsigned, bool);
    void OnButtonsWorkClick(unsigned, bool);
    void OnChangedKoofPB0(unsigned n, int value);
    void OnChangedKoofPB1(unsigned n, int value);
    void OnChangedKoofPB2(unsigned n, int value);
    void OnChangedKoofPB3(unsigned n, int value);

signals:
    void EndTarirovka(CSostav &s, bool res);
    void NewKoof(SElementFourth<unsigned> a, SElementFourth<unsigned> b, SElementFourth<int> c);
};

#endif // CTARIROVKA_H
