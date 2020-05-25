#ifndef CBD_H
#define CBD_H

//#include<QIODevice>
#include<QDate>
#include<QDir>
#include<QFile>
#include <QString>
#include <QStringList>
#include <QTableWidget>
#include "CStack.h"
#include "CSostav.h"
#include <QWidget>
#include <QtGui>
#include <QCalendarWidget>
#include "cbuttons.h"
#include "ctable.h"
#include "cchecks.h"
#include "cresult.h"

class CBD: public QWidget, public CStack <CSostav>
{
    Q_OBJECT
private:
    QCalendarWidget calendar;
    CButtons buttons;
    CTable table_sostavs;
    CTable table_vans;
    CResult result;
    CImage *image_tare, *image_real;
    CImageDefect *image_defect;
    CChecks check_pb;
    CButtonsCheck *check_axel;

    QString file_path, tek_file_path;
    int dial_arch;
    QDate date;
    QDir dir;
    int activ_sostav;
    unsigned activ_axel;
    QImage image_my;
    QPainter painter;
    QWidget *w1, *w2, *w3;
    QPrinter printer;
    QProcess proc_pdf;

    bool YesOrNoYearMonDay(const QDate &_date);
    void Clear();
    bool SetDir(const QDate &_date);
    void PaintGridSostav();
public:
    explicit CBD(QWidget *parent = 0);

    void RePicGraph();
    bool AddSostav(CSostav &sostav);
    CSostav GetActivSostav();
    void PrintSostav();
    void PrintDiallog();
    void PrintPdfSostav();
    void CloseSostav();
    void ReturnByTarirovka(const CSostav &s);
    void SetActivVan(unsigned row);

    void paintEvent(QPaintEvent *p);
    void resizeEvent(QResizeEvent *r);
public slots:
    void TableSostavClicked(unsigned row, const bool &deactiv = true);
    void TableVanClicked(unsigned row);
    void OnButtonsClick(unsigned n, bool s);
    void CalendarChanged();
    void OnCheckedPB(unsigned n, bool s);
    void DragImage(int s);
    void CompressXImage(int s);
    void CompressYImage(int s);
    void OnReSizeTableSostav();
    void OnReSizeTableVan();
    void OnResizeResult();
    void OnReSizeButtons();
    void OnReSizeCheckPB();
    void OnCheckedAxel(unsigned n, bool s);
    void DoubleClickImage();
signals:
    void Tarirovka(CSostav &s);
};

#endif // CBD_H
