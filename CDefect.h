//---------------------------------------------------------------------------
#ifndef CDefectH
#define CDefectH

#include "CBuffer.h"
#include "CStackDual.h".h"
#include "CStackFourth.h"
#include "cparam.h"
#include <math.h>

//---------------------------------------------------------------------------
class CDefect
{
        CBuffer<SElementFourth<short> > data_fft;
        CBuffer<SElementFourth<short> > data_real;
        SElementFourth<SElementDual<short> > size_defect;
        SElementFourth<SElementDual<short> > porog_defect;

        void Run(const double &speed, const int &tare);
  public:
        CDefect(){}
        CDefect(CFileBin &f);
        CDefect(const CBuffer<SElementFourth<short> > &dr, const CBuffer<SElementFourth<short> > &df,
                const double speed, const int tare);
        CDefect& operator=(const CDefect &s);

        void SaveToFile(CFileBin &f);

        void Paint(const unsigned &n, const bool &direction, const unsigned &width, QPointF* ptr_data1, QPointF* ptr_data2);
        int GetDefectProcent(const int &nd = 12) const;
        unsigned GetKolDefect(const unsigned &nd = 12) const;
        unsigned GetPorogDefect(const unsigned &nd) const {return (nd < 4)? porog_defect[nd][0] : 0;}
        unsigned NewGetDefectProcent(const int &nd) const;
        unsigned GetDefectSize(const unsigned &nd) const {return (nd < 4)? size_defect[nd][0] : 0;}
};
//---------------------------------------------------------------------------
#endif
