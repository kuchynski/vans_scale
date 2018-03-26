//---------------------------------------------------------------------------
#ifndef CBlocH
#define CBlocH
//---------------------------------------------------------------------------
#include "CFilter.h"
#include "CAutoschepka.h"
#include "CSostav.h"
#include "SStateBloc.h"
#include "cparam.h"
#include "cprocessoroftare.h"

#include "CTurn.h"

class CBloc
{
private:
    SElementFourth<int> from_opt_to_DLP_obl[2];

    SElementFourth<CFilter> filter_dlpnull, filter_ves, filter_ves2;

    CSostav sostav, ready_sostav;
    SElementFourth<int> dlpnull, current_dlpnull, mas_dlp_null[10], change_dlp_null;
    bool set_dlp_null;
    CStack4 <int> data_in;
    int direction, st, koleso1, koleso2, koleso3, koleso_van;
    CTurn<int> begin_koleso;
    double speed;
    double timer_convert;
    int incr;
    unsigned kirunak;
    double from_1pedal_to2;
    int kol_van;
    CStackFourth convert_data;
    double speed_cm;
    bool prev_new_autoschepka;

    int rast, rast_autoschepka;

    CProcessorOfTare *processor_of_tare;
    int time_work_of_processor;

    int ConvertData(const int &type, const double &distance_cm = 0);    
    void DeleteProcessor();

public:
    CBloc();
    ~CBloc();

    bool IRQState(SStateBloc &sb, const unsigned &k, CStack<unsigned char> &data_bin);
    bool IRQKoleso(const unsigned &st_time_koleso, const unsigned &pedal);
    SElementFourth<int> IRQPB(const SElementFourth<int> &z);
    void IRQNewSecond();
    void IRQEnd();
    void Clear(const bool work = true);
    SElementFourth<int>& GetDlpNullFromPort() { return current_dlpnull; }
    void SetOptions();
    bool IsNewSostav();
    void GetSostav(CSostav &s) {s = ready_sostav; ready_sostav.Clear();}
};
//-------------------------------------------------------------
#endif
