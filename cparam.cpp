#include "cparam.h"

CParam param;

CParam::CParam(): settings("Кучинский", "Весы")//, koof8(1.1)
{
    koof_defect = SElementFourth<int>(1, 14, 13, 1);
    ReadSettings();
    count ++;
}
CParam::~CParam()
{
    SetSettings();
}
void CParam::SetSettings()
{
    param_u[0] = arch_compressx_pb;
    param_u[1] = arch_compressy_pb;
    param_u[2] = com_number;
    param_u[3] = monitor_value;
    param_u[4] = monitor_compressx_pb;
    param_u[5] = monitor_compressy_pb;
    param_u[30] = auto_set_pb_null;
    param_u[31] = arch_together;
    param_u[32] = auto_tarirovka;
    param_u[33] = koof8;
    param_u[34] = koof_vl_80;
    for(unsigned i = 0; i < 4; i++)
    {
        param_i[i] = pb_null[i];
        param_i[4+i] = koof_c[i];
        param_i[8+i] = rast_a[i];
        param_u[6+i] = onoff_pb[i];
        param_u[10+i] = koof_a[i];
        param_u[14+i] = koof_b[i];        
        param_u[18+i] = arch_onoff_pb[i];
        param_u[22+i] = NS_PB[i];
        param_u[26+i] = porog_defect[i];
    }

    settings.setValue("/count", count);
    for(unsigned i = 0; i < size_param; i ++)
    {
        settings.setValue("/uparam" + QString::number(i, 16), param_u[i]);
        settings.setValue("/iparam" + QString::number(i, 16), param_i[i]);
    }
}
void CParam::ReadSettings()
{
    count = settings.value("/count", 0).toUInt();
    for(unsigned i = 0; i < size_param; i ++)
    {
        param_u[i] = settings.value("/uparam" + QString::number(i, 16), 0).toUInt();
        param_i[i] = settings.value("/iparam" + QString::number(i, 16), 0).toInt();
    }
    arch_compressx_pb = (!param_u[0])? 1 : (param_u[0] > 100)? 100 : param_u[0];
    arch_compressy_pb = (!param_u[1])? 1 : (param_u[1] > 500)? 500 : param_u[1];
    com_number = (param_u[2] > 10)? 10 : param_u[2];
    monitor_value = (param_u[3] > 6)? 0 : param_u[3];
    monitor_compressx_pb = (!param_u[4])? 1 : (param_u[4] > 100)? 100 : param_u[4];
    monitor_compressy_pb = (!param_u[5])? 1 : (param_u[5] > 300)? 300 : param_u[5];
    auto_set_pb_null = param_u[30];
    arch_together = param_u[31];
    auto_tarirovka = param_u[32];
    koof8 = param_u[33];
    koof_vl_80 = param_u[34];
    for(unsigned i = 0; i < 4; i++)
    {
        pb_null[i] = param_i[i];
        koof_c[i] = (param_i[4+i] < -999)? -999 : (param_i[4+i] > 999)? 999 : param_i[4+i];
        rast_a[i] = param_i[8+i];
        onoff_pb[i] = param_u[6+i];
        koof_a[i] = (param_u[10+i] < 70)? 70 : (param_u[10+i] > 150)? 150 : param_u[10+i];
        koof_b[i] = (param_u[14+i] < 1)? 1 : (param_u[14+i] > 999)? 999 : param_u[14+i];
        arch_onoff_pb[i] = param_u[18+i];
        NS_PB[i] = param_u[22+i];
        porog_defect[i] = (param_u[26+i] > 99)? 99 : param_u[26+i];
    }
    auto_set_pb_null = param_u[30];
}
SElementFourth<double> CParam::GetKoofAReal()
{
    SElementFourth<double> ret;
  //  QMutexLocker locker(&mutex);
    for(int i = 0; i < 4; i++)
        ret[i] = 0.0001 * static_cast<double>(koof_a[i]);
    return ret;
}
SElementFourth<double> CParam::GetKoofBReal()
{
    SElementFourth<double> ret;
    for(int i = 0; i < 4; i++)
        ret[i] = 0.001 * static_cast<double>(koof_b[i]);
    return ret;
}


