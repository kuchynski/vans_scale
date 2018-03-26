#ifndef CCOMMANDER_H
#define CCOMMANDER_H

#include <QWidget>
#include <QtGui>
#include "cbd.h"
#include "cmonitor.h"
#include "cinput.h"
#include "coptions.h"
#include "CAllWindows.h"
#include "ctarirovka.h"

class CCommander : public CAllWindows
{
public:
    explicit CCommander(QWidget *parent = 0);
private:
    CBD bd;
    CInput input;
    COptions options;
    CTarirovka tarirovka;

private slots:
    void OnNewSostav();
    void OnPressButton(unsigned n);
    void OnBeginTarirovka(CSostav &s);
    void OnEndTarirovka(CSostav &s, bool res);
    void OnNewKoof(SElementFourth<unsigned> a, SElementFourth<unsigned> b, SElementFourth<int> c);
};

#endif // CCOMMANDER_H
