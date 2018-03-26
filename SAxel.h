//---------------------------------------------------------------------------
#ifndef SAxelH
#define SAxelH
#include "CStackFourth.h"
//---------------------------------------------------------------------------
struct SAxel
{
        unsigned distance;
        SElementFourth<int> coordinate;
        double speed;
        SElementFourth<int> progib;
        SElementFourth<int> tare;
        SElementFourth<int> progib_simple;
   //     SElementFourth<int> tmp2[10];
        double om;
        SAxel() {}
        SAxel(double o, const unsigned &d, const SElementFourth<int> &c, double &s,
          const SElementFourth<int> &p, const SElementFourth<int> &t,
          const SElementFourth<int> &h);
        SAxel(const SAxel &a);
        SAxel& operator=(const SAxel& a);
};
//---------------------------------------------------------------------------
#endif
 
