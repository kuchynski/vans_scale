//---------------------------------------------------------------------------
#ifndef CFFTH
#define CFFTH

//#include <StdCtrls.hpp>
//#include <ExtCtrls.hpp>
#include <stdio.h>
#include <math.h>
#include "CBuffer.h"
#include "CStackDual.h"
#include "CStackFourth.h"
//#include "CFilter.h"
#include "cparam.h"
using namespace std;
//---------------------------------------------------------------------------
class CFFT
{
    CBuffer<SElementFourth<double> > x, y;
    CBuffer<SElementFourth<double> > x1, y1;
    SElementFourth<int> begin;
    unsigned size, real_size;
    unsigned m;

    void RunForward(CBuffer<SElementFourth<short> > &in);
    void ClearFr(int fr1, int fr2);
    void ClearFr2(int b, int e);
    void RunBack(CBuffer<SElementFourth<short> > &out);
    void RunBackFr(CBuffer<SElementFourth<short> > &out);
  public:
    CFFT(CBuffer<SElementFourth<short> > &in, CBuffer<SElementFourth<short> > &out_real,
         CBuffer<SElementFourth<short> > &out_fft, SElementFourth<int> &center);
};
//---------------------------------------------------------------------------
#endif
