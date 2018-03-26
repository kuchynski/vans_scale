#ifndef CPARAM_H
#define CPARAM_H

#include <QString>
#include <QSettings>
#include <QMutex>
#include "CStackFourth.h"

#define size_param 35

class CParam
{
public:
    CParam();
    ~CParam();

    void SetArchCompressxPb(const unsigned &u) {arch_compressx_pb = u;}
    void SetArchCompressyPb(const unsigned &u) {arch_compressy_pb = u;}
    void SetComNumber(const unsigned &u) {com_number = u;}
    void SetMonitorValue(const unsigned &u) {monitor_value = u;}
    void SetMonitorCompressX(const unsigned &u) {monitor_compressx_pb = u;}
    void SetMonitorCompressY(const unsigned &u) {monitor_compressy_pb = u;}
    void SetonoffPb(const unsigned &n, const unsigned &u) {if(n < 4) onoff_pb[n] = u;}
    void SetNSPb(const unsigned &n, const unsigned &u) {if(n < 4) NS_PB[n] = u;}
    void SetArchonoffPb(const unsigned &n, const unsigned &u) {if(n < 4) arch_onoff_pb[n] = u;}
    void SetAutoSetPBNull(const unsigned &u) {auto_set_pb_null = u;}
    void SetKoofA(const unsigned &n, const unsigned &u) {QMutexLocker locker(&mutex); if(n < 4) koof_a[n] = u;}
    void SetPorogDefect(const unsigned &n, const unsigned &u) {QMutexLocker locker(&mutex); if(n < 4) porog_defect[n] = u;}
    void SetKoofB(const unsigned &n, const unsigned &u) {QMutexLocker locker(&mutex); if(n < 4) koof_b[n] = u;}
    void SetKoofC(const unsigned &n, const int &u) {QMutexLocker locker(&mutex); if(n < 4) koof_c[n] = u;}
    void SetKoofA(SElementFourth<unsigned> u) {QMutexLocker locker(&mutex); koof_a = u;}
    void SetKoofB(SElementFourth<unsigned> u) {QMutexLocker locker(&mutex); koof_b = u;}
    void SetKoofC(SElementFourth<int> u) {QMutexLocker locker(&mutex); koof_c = u;}
    void SetPbNull(const SElementFourth<int> &u) {QMutexLocker locker(&mutex); pb_null = u;}
    void SetRastA(const unsigned &n, const int &u) {if(n < 4) rast_a[n] = u;}
    void SetArchTogether(const bool &n) {arch_together = (n)? 1 : 0;}
    SElementFourth<int> GetKoofDefect() const {return koof_defect;}
    void SetAutoTarirovka(const bool s) {auto_tarirovka = (s)? 1 : 0;}

    unsigned GetArchCompressxPb() {return arch_compressx_pb;}
    unsigned GetArchCompressyPb() {return arch_compressy_pb;}
    unsigned GetComNumber() {return com_number;}
    unsigned GetMonitorValue() {return monitor_value;}
    unsigned GetMonitorCompressX() {return monitor_compressx_pb;}
    unsigned GetMonitorCompressY() {return monitor_compressy_pb;}
    unsigned GetonoffPb(const unsigned &n) {return (n < 4)? onoff_pb[n] : 0;}
    unsigned GetArchonoffPb(const unsigned &n) {return (n < 4)? arch_onoff_pb[n] : 0;}
    SElementFourth<unsigned> GetArchonoffPb() {return arch_onoff_pb;}
    SElementFourth<int> GetRastA() {return rast_a;}
    int GetRastA(const unsigned &n)const {return (n < 4)? rast_a[n] : 0;}
    unsigned GetNSPb(const unsigned &n) {return (n < 4)? NS_PB[n] : 0;}
    unsigned GetAutoSetPBNull() {return auto_set_pb_null;}
    unsigned GetKoofA(const unsigned &n) {return (n < 4)? koof_a[n] : 0;}
    unsigned GetKoofB(const unsigned &n) {return (n < 4)? koof_b[n] : 0;}
    unsigned GetPorogDefect(const unsigned &n) {return (n < 4)? porog_defect[n] : 0;}
    int GetKoofC(const unsigned &n) {return (n < 4)? koof_c[n] : 0;}
   // int GetPbNull(const unsigned &n) {QMutexLocker locker(&mutex); return (n < 4)? pb_null[n] : 0;}
    SElementFourth<int> GetPbNull() {return pb_null;}
    int GetPbNull(const unsigned &n) {return (n < 4)? pb_null[n] : 0;}
    bool GetArchTogether()const {return (arch_together)? true : false;}
    bool GetAutoTarirovka()const {return auto_tarirovka != 0;}

    SElementFourth<double> GetKoofAReal();
    SElementFourth<double> GetKoofBReal();
    SElementFourth<int> GetKoofCReal() {return koof_c;}// * 10;}
    SElementFourth<unsigned> GetPorogDefect() const {return porog_defect;}


    void SetKoof8(const unsigned k) {if(k < 100) koof8 = k;}
    unsigned GetKoof8()const {return koof8;}
    double GetKoof8Double()const {return static_cast<double>(koof8) / 99.0 + 1.0;}
    void SetKoofVl80(const unsigned k) {if(k <= 110 && k >= 80) koof_vl_80 = k;}
    unsigned GetkoofVl80()const {return koof_vl_80;}

    void SetSettings();
private:

    QSettings settings;
    QMutex mutex;
    unsigned param_u[size_param];
    int param_i[size_param];

    unsigned count;
    unsigned arch_compressx_pb, arch_compressy_pb, com_number, arch_together;
    unsigned monitor_value, monitor_compressx_pb, monitor_compressy_pb, auto_set_pb_null;
    unsigned auto_tarirovka;
    SElementFourth<unsigned> onoff_pb, arch_onoff_pb, NS_PB, koof_a, koof_b, porog_defect;
    SElementFourth<int> rast_a;
    SElementFourth<int> koof_c, pb_null;
    SElementFourth<int> koof_defect;
    unsigned koof8, koof_vl_80;

    void ReadSettings();
};
extern CParam param;

#endif // CPARAM_H
