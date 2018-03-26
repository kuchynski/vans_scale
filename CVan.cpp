//---------------------------------------------------------------------------
#include <stdio.h>
#include "CVan.h"

//---------------------------------------------
CVan::CVan()
{
  Clear();
}
//---------------------------------------------
CVan::CVan(CFileBin &f)
{
    if(!f.Read(&begin_axel, sizeof(begin_axel), 1)) throw;
    if(!f.Read(&size, sizeof(size), 1)) throw;
    type = CString(f);
    if(!f.Read(&lokomotiv, sizeof(lokomotiv), 1)) throw;
}
//---------------------------------------------
CVan::CVan(const CVan &v)
{
  *this = v;
}
//-------------------------------------------------------------
CVan::CVan(const int &b): begin_axel(b), size(1)
{}
//---------------------------------------------
CVan& CVan::operator=(const CVan& v)
{
  begin_axel = v.begin_axel;
  size = v.size;
  ps = v.ps;
  type = v.type;
  lokomotiv = v.lokomotiv;
  return *this;
}
//-------------------------------------------------------------
void CVan::SaveToFile(CFileBin &f)
{
    if(!f.Write(&begin_axel, sizeof(begin_axel), 1)) throw 0;
    if(!f.Write(&size, sizeof(size), 1)) throw 0;
    type.SaveToFile(f);
    if(!f.Write(&lokomotiv, sizeof(lokomotiv), 1)) throw 0;
}
//-------------------------------------------------------------
void CVan::Clear()
{
  begin_axel = 0;
  size = 0;
  type = "";
  lokomotiv = 0;
}
//-------------------------------------------------------------
bool CVan::IsMy(const unsigned &n)const
{
  return ((n >= begin_axel) && (n <= (begin_axel + size - 1)));
}
//---------------------------------------------
double CVan::GetSpeedDouble(const CStack <SAxel> &axels) const
{
  double speed = 0;
  for(unsigned i = 0; i < size; i ++)
    speed += axels[begin_axel+i].speed;
  speed = (size)? speed / size : 0;
  //double speed_mc = (speed)? (1728/(2*(double)speed)) : 0;
  double speed_kmch = speed * 3.6;
  return speed_kmch;
}
//---------------------------------------------
QString CVan::GetSpeed(const CStack <SAxel> &axels) const
{
  double speed = GetSpeedDouble(axels);
  if(speed)
      return QString::number(speed, 'f', 1);// GetFloatToStr(speed, 1);
  return "0";
}
//---------------------------------------------
int CVan::GetCoordinate(const CStack <SAxel> &axels, const unsigned &n, unsigned na)const
{
  if(na > (size-1)) na = size - 1;
  return axels[begin_axel+na].coordinate[n];
}
//---------------------------------------------
/*QString CVan::GetFloatToStr(double d, const int &n)const
{
    return QString::number(d, 'f'', n');
  int ret1 = (int)d;
  int ret2 = 0; 
  if(n <= 0)
    return "0";
  d -= static_cast<int>(d);

  for(int i = 0; i < n; i ++)
  {
    d *= 10;
    ret2 = ret2 * 10 + (int)d;
    d -= static_cast<int>(d);
  }
  if(d*10 >= 5)
    ret2 ++;
  if(ret2 >= pow10(n))
  {  ret1 ++; ret2 = 0;}
  return QString::number(ret1) + "," + QString::number(ret2);
}*/
//---------------------------------------------
QString CVan::MyFloatToStr(int ch)const
{
  QString ret = QString::number(ch/1000, 10) + ",";
  ch = abs(ch);
  int add = ch/10-(ch/1000)*100;
  if(add == 0) return ret + "00";
  if(add < 10) return ret + "0" + QString::number(add);
  else return ret + QString::number(add);
}
//---------------------------------------------
QString CVan::MyFloatToStr1(int ch)const
{
  int ch2 = ch / 1000;
  QString ret = QString::number(ch2, 10) + ".";
  //if(ch2 < 100) ret = " " + ret;
  if(ch2 < 10) ret = " " + ret;
  ch = abs(ch);
  int add = ch/100-(ch/1000)*10;
  if(add == 0) return ret + "0";
  else return ret + QString::number(add);
}
//---------------------------------------------
QString CVan::MyFloatToStrKg(int ch)const
{
  QString r = CString(ch, 6).GetQString();
  return r;
/*  QString ret = QString::number(ch/1000) + ".";
  ch = abs(ch);
  int add = ch/10-(ch/1000)*100;
  if(add == 0) return ret + "00";
  if(add < 10) return ret + "0" + QString::number(add);
  else return ret + QString::number(add);*/
}
//---------------------------------------------
int CVan::GetTare(const CStack <SAxel> &axels, const unsigned &naxel, const unsigned &ndat) const
{
  return GetTareProgib(axels, naxel, ndat, false);
}
//---------------------------------------------------------------------------
int CVan::GetProgib(const CStack <SAxel> &axels, const unsigned &naxel, const unsigned &ndat) const
{
  return GetTareProgib(axels, naxel, ndat, true);
}
//---------------------------------------------
QString CVan::GetTareFile(const CStack <SAxel> &axels) const
{
  QString ret = GetTareStr(axels, 100, 100, true) + " " +
                GetTareStr(axels, 100, 103, true) + " " +
                GetTareStr(axels, 100, 104, true) + " " +
                GetTareStr(axels, 101, 100, true) + " " +
                GetTareStr(axels, 102, 100, true) + " ";
  for(unsigned i = 0; i < size; i ++)
  {
    for(unsigned j = 0; j < 4; j ++)
      ret += (GetTareStr(axels, i, j, true) + "-");
    ret += " ";
  }
  return ret;
}
//---------------------------------------------
QString CVan::GetTareStr(const CStack <SAxel> &axels, const unsigned &naxel, const unsigned &ndat, const bool &kg) const
{
  return (kg)? MyFloatToStrKg(GetTareProgib(axels, naxel, ndat, false)) : MyFloatToStr(GetTareProgib(axels, naxel, ndat, false));
}
//---------------------------------------------
QString CVan::GetTareStr1(const CStack <SAxel> &axels, const unsigned &naxel, const unsigned &ndat) const
{
  return MyFloatToStr1(GetTareProgib(axels, naxel, ndat, false));
}
//---------------------------------------------------------------------------
QString CVan::GetProgibStr(const CStack <SAxel> &axels, const unsigned &naxel, const unsigned &ndat) const
{
  return QString::number(GetTareProgib(axels, naxel, ndat, true));
}
//---------------------------------------------------------------------------
int CVan::GetTareProgib(const CStack <SAxel> &axels, const unsigned &naxel, const unsigned &ndat, const bool &tp) const
{  // naxel = 100(всё), 101(1 тележка), 102(2 тележка)
   // ndat  = 100(всё), 101(0, 2), 102(1, 3), 103(0,1), 104(2,3)
  if((naxel > size) && ((naxel < 100) || (naxel > 102)))
    return 0;
  if((ndat > 3) && ((ndat < 100) || (ndat > 104)))
    return 0;
  int axel_begin = naxel, axel_end = naxel;
  switch(naxel)
  {
    case 100: axel_begin = 0; axel_end = size - 1; break;
    case 101: axel_begin = 0; axel_end = size / 2 - 1; break;
    case 102: axel_begin = size / 2; axel_end = size - 1; break;
  }
  bool dat_enabled[4] = {false, false, false, false};
//  int dat_kol = 2;
  switch(ndat)
  {
    case 100: dat_enabled[0] = true; dat_enabled[1] = true;
              dat_enabled[2] = true; dat_enabled[3] = true; break;
    case 101: dat_enabled[0] = true; dat_enabled[2] = true; break;
    case 102: dat_enabled[1] = true; dat_enabled[3] = true; break;
    case 103: dat_enabled[0] = true; dat_enabled[1] = true; break;
    case 104: dat_enabled[2] = true; dat_enabled[3] = true; break;
    default: dat_enabled[ndat] = true;// dat_kol = 1;
  }
  int ret = 0;
  for(int i = axel_begin; i <= axel_end; i ++)
  {
    int dk1 = 0; // 1 сторона
    int ret1 = 0;
    for(int j = 0; j < 2; j ++)
    {
      if(dat_enabled[j])
      {
        int t = (tp)? axels[begin_axel+i].progib[j] : axels[begin_axel+i].tare[j];
//        if((t != -65000) && (t > 1000) && (t < 50000))
        if(t != -65000)
        {
          ret1 += t;
          dk1 ++;
        }
      }
    }
    int dk2 = 0; // 2 сторона
    int ret2 = 0;
    for(int j = 2; j < 4; j ++)
    {
      if(dat_enabled[j])
      {
        int t = (tp)? axels[begin_axel+i].progib_simple[j] : axels[begin_axel+i].tare[j];
//        if((t != -65000) && (t > 1000) && (t < 50000))
//        if((t != -65000) && (t < 50000))
        if(t != -65000)
        {
          ret2 += t;
          dk2 ++;
        }
      }
    }
    if(!dk2 && naxel >= 100 && ndat == 100)
      ret1 *= 2;
    if(!dk1 && naxel >= 100 && ndat == 100)
      ret2 *= 2;
    if(dk1)
      ret += ret1 / dk1;
    if(dk2)
      ret += ret2 / dk2;
  }
  if(size == 8)
      ret = static_cast<int>(static_cast<double>(ret) * param.GetKoof8Double());

  return ret;
//  return ret / dat_kol;
}
//---------------------------------------------------------------------------
unsigned CVan::GetKolDefect(const CStack <CDefect> &defects, const unsigned naxel, const unsigned ndat)const
{// naxel = 100(всё)
 // ndat  = 12(всё), 13(1 сторона), 14(2 сторона)
    if(ndat > 14)
        return 0;
    if((naxel > size) && (naxel != 100))
        return 0;
    int axel_begin = (naxel == 100)? begin_axel : begin_axel + naxel;
    int axel_end = (naxel == 100)? begin_axel + size : axel_begin + 1;
    unsigned ret = 0;
    for(unsigned i = axel_begin; i < axel_end; i ++)
        if(defects[i].GetKolDefect(ndat))
            ret ++;
    return ret;
}
CString CVan::GetDefectFile(const CStack <CDefect> &defects) const
{
  CString ret1 = "";
  CString ret2 = "";
  for(int i = 0; i < size; i ++)
  {
    int d1 = defects[begin_axel+i].NewGetDefectProcent(13);// первая сторона
    int d2 = defects[begin_axel+i].NewGetDefectProcent(14);// вторая сторона
    ret1 += ((d1)? (CString("+") + CString(d1, 2)) : CString("-00"));
    ret2 += ((d2)? (CString("+") + CString(d2, 2)) : CString("-00"));
  }
  return ret1 + " " + ret2;
}

