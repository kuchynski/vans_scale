#ifndef CPROCCESOROFTARE_H
#define CPROCCESOROFTARE_H

#include <QThread>
#include "CSostav.h"
#include "CFilter.h"

class CProcessorOfTare : public QThread
{
    Q_OBJECT
public:
    enum eProcessorState {psWork, psNoSostav, psNewSostav, psEnd};
private:
    CSostav sostav;
    eProcessorState state;
    CStack4<int> data, data_real, data_defect, data_in;
    CStackFourth convert_data;

    bool ConvertDataEnd();

public:
    explicit CProcessorOfTare(CSostav &s, CStack4<int> &d, CStackFourth &cd, QObject *parent = 0);
    ~CProcessorOfTare();

    void run();
    eProcessorState GetSostav(CSostav &s);
    eProcessorState GetState()const {return state;}

signals:

public slots:

};

#endif // CPROCCESOROFTARE_H
