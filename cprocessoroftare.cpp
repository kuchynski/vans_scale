#include "cprocessoroftare.h"
#include "csavefiletxt.h"

CProcessorOfTare::CProcessorOfTare(CSostav &s, CStack4<int> &d, CStackFourth &cd, QObject *parent) :
    sostav(s), data_in(d), convert_data(cd), QThread(parent), state(psWork)
{
}
CProcessorOfTare::~CProcessorOfTare()
{
    FILE *f = fopen("test_proc.txt", "at"); fprintf(f, "-end "); fclose(f);
    data.Clear();
    data_real.Clear();
    data_defect.Clear();
}
void CProcessorOfTare::run()
{
//CSaveFileTxt processor_test(CString("processor_test.txt"), false, CString("_________________________"));
    FILE *f = fopen("test_proc.txt", "at"); fprintf(f, "\n-1 "); fclose(f);
    if(ConvertDataEnd())
    {
        f = fopen("test_proc.txt", "at"); fprintf(f, "-2 "); fclose(f);
//        processor_test.AddLineText(CString("1"));
        sostav.AddData(data, data_real, data_defect);
        f = fopen("test_proc.txt", "at"); fprintf(f, "-3 "); fclose(f);
//processor_test.AddLineText(CString("2"));
        //while(1){}
        state = psNewSostav;
    }
    else
    {
        state = psNoSostav;
        f = fopen("test_proc.txt", "at"); fprintf(f, "-4 "); fclose(f);
    }

//processor_test.AddLineText(CString("3"));
    f = fopen("test_proc.txt", "at"); fprintf(f, "-5 "); fclose(f);
    exec();
    f = fopen("test_proc.txt", "at"); fprintf(f, "-6 "); fclose(f);
//processor_test.AddLineText(CString("4"));
}
CProcessorOfTare::eProcessorState CProcessorOfTare::GetSostav(CSostav &s)
{
    if(state == psWork)
        return psWork;
    if(state == psEnd)
        return psEnd;

    if(state == psNewSostav)
    {
        state = psEnd;
        s = sostav;
        return  psNewSostav;
    }
    state = psEnd;
    return psNoSostav;
}
//-------------------------------------------------------------
bool CProcessorOfTare::ConvertDataEnd()
{
    bool ret = true;
    const int size = data_in.GetSize();//переводин из CStack4 в CBuffer
    SElementFourth<int> *tdata = new SElementFourth<int> [size];
    SElementFourth<int> *uk1 = tdata;
    data_in.ClearPtr();
    for(int i = 0; i < size; i ++, uk1 ++, data_in.AddPtr(1))
        *uk1 = data_in.GetElementPtr();
    SElementFourth<CFilter> filter, filter_defect;
    CStack<double> a, b;
    CStack<double> a2, b2;
    a << 1.0 << -3.65806030240188  << 5.03143353336761//Fs = 480; [b,a]=butter(4, 10*2/Fs)
      << -3.08322830175882 <<  0.71010389834159;
    b << 0.15551721780853 <<  0.62206887123412  << 0.93310330685117
      << 0.62206887123412  << 0.15551721780853;
    filter[0].SetAB(a, b);
    filter[0].MullB(1.0e-4);

    a2 << 1.0 << -3.65806030240188 <<  5.03143353336760//Fs = 480; [b,a]=butter(4, 10*2/Fs,'high'))
      << -3.08322830175881 <<  0.71010389834159;
    b2 << 0.84267662724187 << -3.37070650896747 <<  5.05605976345121
      << -3.37070650896747 <<  0.84267662724187;
    filter_defect[0].SetAB(a2, b2);
    /*a << 0.01000000000000 << -0.10796623403452  << 0.53481859117422//Fs = 120; [b,a]=butter(12, 3*2/Fs)
      << -1.60723268649426 <<  3.26348431830680 << -4.71669841073230
      << 4.97539897432994 << -3.85941147817410 <<  2.18488686190273
      << -0.88034622927926  << 0.23963638071306 << -0.03956680648161
      << 0.00299671877056;
    b << 0.00030927829200 <<  0.00371133950395 <<  0.02041236727172
      << 0.06804122423907 <<  0.15309275453791 <<  0.24494840726065
      << 0.28577314180409 <<  0.24494840726065 <<  0.15309275453791
      << 0.06804122423907  << 0.02041236727172 <<  0.00371133950395
      << 0.00030927829200;
    filter[0].SetAB(a, b);
    filter[0].MullB(1.0e-10);
    filter[0].MullA(1.0e+2);*/
    for(unsigned i = 1; i < 4; i ++)
    {
        filter[i] = filter[0];
        filter_defect[i] = filter_defect[0];
    }

 //   FILE *f = fopen("test_proc.txt", "at"); fprintf(f, ".1 "); fclose(f);
    for(unsigned n = 0; n < convert_data.GetSize(); n ++)
    {
        const double begin = static_cast<double>(convert_data[n][0]);
        const double end = static_cast<double>(convert_data[n][1]);
        const int end_k = convert_data[n][2];
        const double step = (end - begin) / static_cast<double>(end_k);
        int j, j_prev = -1;
        if((begin >= size) || (step >= 7))
        {
            ret = false;
            break;
        }
        SElementFourth<int> d = tdata[static_cast<int>(begin)];
        SElementFourth<int> d2 = d;
        double i = begin;
  //  f = fopen("test_proc.txt", "at"); fprintf(f, "..%d_%f ", end_k, step); fclose(f);
        for(int k = 0; k < end_k; i += step, k ++)
        {
            j = static_cast<int>(i);
            if(j >= size)
                break;
            d = tdata[j];
            d2 = d;
            data_real << d;
            for(unsigned n = 0; n < 4; n ++)
            {
                d2[n] = filter_defect[n] << d2[n];
                d[n] = filter[n] << d[n];
            }
            data << d;
            data_defect << d2;
        }
    }
    delete [] tdata;
    data_in.Clear();
    convert_data.Clear();
  //  f = fopen("test_proc.txt", "at"); fprintf(f, ".3 "); fclose(f);
    return ret;
}
