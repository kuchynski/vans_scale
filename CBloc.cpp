//---------------------------------------------------------------------------
#include "CBloc.h"

#define max_time_processor 30
//---------------------------------------------------------------------------
CBloc::CBloc(): processor_of_tare(0), time_work_of_processor(0)
{
  set_dlp_null = false;
  dlpnull = 0;

  SetOptions();


  CStack<double> a1, b1;
  a1 << 1.0 << -3.96579438007005 << 5.89796693861409  //Fs = 480;   [b,a]=butter(4, 1*2/Fs, 'z')
     << -3.89854491737242 << 0.96637238769206;
  b1 << 0.01803979521320 << 0.07215918085279 << 0.10823877127919
     << 0.07215918085279 << 0.01803979521320;
  filter_dlpnull[0].SetAB(a1, b1);
  filter_dlpnull[0].MullB(1.0e-7);
  CStack<double> a2, b2;
/*  a2 << 1.00000000000000 << -7.49074112328288 << 25.49888241309042//Орша  //Fs = 480;   [b,a]=butter(10, 30*2/Fs)
     << -51.89642685435459 << 69.88127331032271 << -65.01151476567185
     << 42.29457724685609 << -18.99088446794065 <<  5.63012926905765
     << -0.99479143792835 <<  0.07952441049725;
  b2 << 0.00273443806662 <<  0.02734438066621  << 0.12304971299797
     << 0.32813256799457 <<  0.57423199399050  << 0.68907839278860
     << 0.57423199399050 <<  0.32813256799457  << 0.12304971299797
     << 0.02734438066621 <<  0.00273443806662;
  filter_ves[0].SetAB(a2, b2);
  filter_ves[0].MullB(1.0e-5);*/
/*  a2 << 0.01000000000000 << -0.08326777204347 <<  0.31321165677484  //Fs = 480;   [b,a]=butter(10, 20*2/Fs)
     << -0.70070044044380 <<  1.03226065565211 << -1.04618174652615
     << 0.73860383644891 << -0.35862952438299 <<  0.11459870944216
     << -0.02175941304554 <<  0.00186404503936;
  b2 << 0.00067533494907  << 0.00675334949069 <<  0.03039007270809
     << 0.08104019388823 <<  0.14182033930440 <<  0.17018440716529
     << 0.14182033930440 <<  0.08104019388823 <<  0.03039007270809
     << 0.00675334949069  << 0.00067533494907;
  filter_ves[0].SetAB(a2, b2);
  filter_ves[0].MullB(1.0e-6);
  filter_ves[0].MullA(1.0e+2);*/
  a2 << 1.000000000000 << -4.98494257353775 << 11.26104527514549//Fs = 480;   [b,a]=butter(8, 45*2/Fs)
     << -14.94636758544691 << 12.68998226126574 << -7.03453194555901
     << 2.48019888705460 << -0.50751946693396 <<  0.04607506387523;
  b2 << 0.00001539029634  << 0.00012312237073  << 0.00043092829756
     << 0.00086185659512 <<  0.00107732074390 <<  0.00086185659512
     << 0.00043092829756  << 0.00012312237073 <<  0.00001539029634;
  filter_ves[0].SetAB(a2, b2);
/*  CStack<double> a3, b3;
  a3 << 1.00000000000000 << -3.99657942627525 <<  5.98974412777420  //Fs = 480;   [b,a]=butter(4, 0.1*2/Fs,'high')
     << -3.98974997086573 <<  0.99658526936971;
  b3 << 0.99829117464281 << -3.99316469857123 <<  5.98974704785684
     << -3.99316469857123 <<  0.99829117464281;
  filter_ves2[0].SetAB(a3, b3);*/
  for(int i = 1; i < 4; i ++)
  {
    filter_dlpnull[i] = filter_dlpnull[0];
    filter_ves[i] = filter_ves[0];
  //  filter_ves2[i] = filter_ves2[0];
  }
  Clear(false);
  FILE *f1 = fopen("test.txt", "wt");
  fclose(f1);
}
//-------------------------------------------------------------
CBloc::~CBloc()
{
  Clear(false);
  DeleteProcessor();
}
//-------------------------------------------------------------
void CBloc::SetOptions()
{
  SElementFourth<int> a = param.GetRastA();
  a[3] = a[2] - a[1] - a[0];
  from_opt_to_DLP_obl[0][0] = from_opt_to_DLP_obl[0][2] = a[0];
  from_opt_to_DLP_obl[0][1] = from_opt_to_DLP_obl[0][3] = from_opt_to_DLP_obl[0][0] + a[3];// cm
  from_opt_to_DLP_obl[1][1] = from_opt_to_DLP_obl[1][3] = a[1];
  from_opt_to_DLP_obl[1][0] = from_opt_to_DLP_obl[1][2] = from_opt_to_DLP_obl[1][1] + a[3];// cm
  from_1pedal_to2 = a[2] / 100.0;

  dlpnull = param.GetPbNull();
  for(int i = 0; i < 10; i ++)
    mas_dlp_null[i] = dlpnull;
  set_dlp_null = true;
  for(int i = 0; i < 4; i ++)
    filter_dlpnull[i].SetNumber(dlpnull[i]);
}
//-------------------------------------------------------------
void CBloc::IRQNewSecond()
{
    for(int i = 1; i < 5; i ++)
        mas_dlp_null[i-1] = mas_dlp_null[i];
    mas_dlp_null[4] = current_dlpnull;
    if(time_work_of_processor)
        time_work_of_processor ++;
    if(time_work_of_processor > max_time_processor)
        DeleteProcessor();
}
//-------------------------------------------------------------
void CBloc::Clear(const bool work)
{
    if(sostav.GetNumberOfVan() && work)
    {
        sostav.SetDirection(direction == 2);
        sostav.SetKoofA(param.GetKoofAReal());
        sostav.SetKoofB(param.GetKoofBReal());
        sostav.SetKoofC(param.GetKoofCReal());
        ConvertData(1, speed_cm * 15.0);
        DeleteProcessor();
        processor_of_tare = new CProcessorOfTare(sostav, data_in, convert_data);
        time_work_of_processor = 1;
        processor_of_tare->start(QThread::LowestPriority);
        FILE *f = fopen("test_bloc.txt", "at"); fprintf(f, "\n-BEGIN "); fclose(f);
    }
    sostav.Clear();
    data_in.Clear();
    rast = 0;
    rast_autoschepka = 0;
    convert_data.Clear();
    direction = 0;
    st = 0;
    begin_koleso.Clear();
    timer_convert = 0;
    incr = 1;
    koleso1 = koleso2 = koleso3 = koleso_van = 0;
    speed = 0;
    kol_van = 0;
    prev_new_autoschepka = true;
}
//-------------------------------------------------------------
bool CBloc::IRQKoleso(const unsigned &st_time_koleso, const unsigned &pedal)
{
  st += st_time_koleso;
  timer_convert += st_time_koleso;
  switch(pedal)
  {
    case 0:
      koleso1 ++;
      break;
    case 1:
      koleso2 ++;
      break;
    default : return false;
  }
//WriteTestFile("Колесо: педаль " + IntToStr(pedal) + ", st = " + IntToStr(st_time_koleso));
  if(direction == 0)
  {
    if(pedal == 0)
      direction = 1;
    else if(pedal == 1)
      direction = 2;
    sostav.Clear();
    if(true)//if(param.GetAutoSetPBNull())
    {
      for(int i = 0; i < 4; i++)
        change_dlp_null[i] = mas_dlp_null[0][i] - dlpnull[i];
      dlpnull = mas_dlp_null[0];
//      SendMessage(OptionsForm->Handle, WM_USER+2, dlpnull[0], dlpnull[1]);
//      SendMessage(OptionsForm->Handle, WM_USER+3, dlpnull[2], dlpnull[3]);
    }
  }
  if((direction == 1 && pedal == 0) || (direction == 2 && pedal == 1)) //начало
  {
    begin_koleso << st;
  }
  else if((direction == 2 && pedal==0) || (direction == 1 && pedal == 1)) //конец
  {
    int begin;
    if(!begin_koleso.GetSize())
    {
        IRQEnd();
        return true;
    }
    begin_koleso >> begin;
    double time = ((double)st - (double)begin) / 1000.0;//сек
    if(!time)
      time = 1;
    speed = from_1pedal_to2 / time; // м/c
    if((speed < 0.5) || (speed > 30))
    {
      Clear(false);
      return false;
    }
    speed_cm = speed * 100.0;


    const double distance_cm = speed_cm * timer_convert / 1000.0;
//WriteTestFile("  Скорость: " + IntToStr((int)(speed*10.0)) + "; время = " + IntToStr((int)(time*1000.0))+"; дистанция, см = " + IntToStr((int)distance_cm));
    timer_convert = 0;


    koleso_van += incr;
    const bool new_vagon = !koleso_van;
    const bool new_avt = (koleso1 == koleso2) && !new_vagon;
    const double sm = 1 / speed_cm; // секунд на сантиметр
    const double om = sm * 1000.0;//отсчётов на сантиметр

    if(!sostav.GetNumberOfAxels())// запись графика ДО начала
      rast += ConvertData(0, speed_cm * 5.0);

    rast += ConvertData(1, distance_cm);
    int direction2 = (direction == 2)? 0 : 1;
    SElementFourth<int> coordinate = SElementFourth<int>(rast+35)-from_opt_to_DLP_obl[direction2];
  //  SElementFourth<unsigned> coordinate2 = from_opt_to_DLP_obl[direction2];
    SElementFourth<int> tmp = SElementFourth<int>(0);
    SElementFourth<int> progib = SElementFourth<int>(0);
    if(!prev_new_autoschepka)
      rast_autoschepka += distance_cm;
    else
      rast_autoschepka = 0;
    SAxel axel = SAxel(om, rast_autoschepka, coordinate, speed, progib, tmp, tmp);
    sostav.SetDirection(direction == 2);
    sostav.AddAxel(axel, new_avt, new_vagon);
    prev_new_autoschepka = new_avt;
    if(!koleso_van)
    {
      incr = 1;
      kol_van ++;
      //SendMessage(MainForm->Handle, WM_USER, 0, 0);
//WriteTestFile("                                                         Вагон " + IntToStr(kol_van));
      if(kol_van == 5)
        kol_van = kol_van;
    }
    else if(koleso1 == koleso2)
    {
      incr = -1;
    }
  }
  return false;
}
//-------------------------------------------------------------
bool CBloc::IRQState(SStateBloc &sb, const unsigned &k, CStack<unsigned char> &data_bin)
{
    bool ret = false;
    if(!k && direction)
    {
        sostav.AddDataBin(data_bin);
        data_bin.Clear();
        ret = true;
        IRQEnd();
    }
    sb.speed = speed;
    sb.direction = direction;
    sb.kol_of_van = kol_van;
    return ret;

/*    bool ret = false;
    sb.speed = speed;
    sb.direction = direction;
    sb.kol_of_van = kol_van;
    if(kirunak && !k)
    {
        sostav.AddDataBin(data_bin);
        IRQEnd();
        ret = true;
    }
    kirunak = k;
    return ret;*/
}
//-------------------------------------------------------------
void CBloc::IRQEnd()
{
    Clear();
}
//-------------------------------------------------------------
int CBloc::ConvertData(const int &type, const double &distance_cm)
{
  if(!distance_cm || !data_in.GetSize())
    return 0;
  int ret = 0;
  int begin;
  switch(type)
  {
    case 0:
      convert_data << SElementFourth<int>(0, 2400, distance_cm, 0);
      ret = distance_cm;
      for(int i = 0; i < 2400; i++)//, data_in.AddPtr(1))
        data_in[i] = data_in[i] - change_dlp_null;
      break;
    case 1:
      begin = (convert_data.GetSize())? convert_data[convert_data.GetSize()-1][1] : 0;
      convert_data << SElementFourth<int>(begin, data_in.GetSize(), distance_cm, 0);
      ret = distance_cm;
      break;
    default:
      ret = 0;
  }
  return ret;
}
//-------------------------------------------------------------
SElementFourth<int> CBloc::IRQPB(const SElementFourth<int> &z)
{
    if(!set_dlp_null)
        return current_dlpnull;
    SElementFourth<int> c, current = z;

    for(int i = 0; i < 4; i ++)
    {
        current_dlpnull[i] = filter_dlpnull[i] << current[i];
//    current[i] = filter_ves2[i] << current[i];  c[i] = filter_ves[i] << current[i];
        c[i] = (current[i] - dlpnull[i]);
//        c[i] = filter_ves[i] << (current[i] - dlpnull[i]);
    }

    if(direction)
        data_in << c;
    else
        data_in.PushMaxSize(c, 2400);
    return current_dlpnull;
}
//-------------------------------------------------------------
bool CBloc::IsNewSostav()
{
    FILE *f;
    if(processor_of_tare == 0)
        return false;
    if(processor_of_tare->GetSostav(ready_sostav) == CProcessorOfTare::psNewSostav)
    {
        DeleteProcessor();
        f = fopen("test_bloc.txt", "at"); fprintf(f, "-END "); fclose(f);
        return true;
    }
 //   f = fopen("test_bloc.txt", "at"); fprintf(f, "-"); fclose(f);
    return false;
}
void CBloc::DeleteProcessor()
{
    FILE *f;
    if(processor_of_tare)
    {
        f = fopen("test_bloc.txt", "at"); fprintf(f, "-d "); fclose(f);
        if(processor_of_tare->GetState() == CProcessorOfTare::psWork)
        {
            f = fopen("test_bloc.txt", "at"); fprintf(f, "-dbad1 "); fclose(f);
            processor_of_tare->terminate();
            f = fopen("test_bloc.txt", "at"); fprintf(f, "-dbad2 "); fclose(f);
            if(processor_of_tare->wait(2000))
            {
                f = fopen("test_bloc.txt", "at"); fprintf(f, "-dbad3 "); fclose(f);
                delete processor_of_tare;
            }
            f = fopen("test_bloc.txt", "at"); fprintf(f, "-dbad4 "); fclose(f);
        }
        else
        {
            f = fopen("test_bloc.txt", "at"); fprintf(f, "-dgood1 "); fclose(f);
            processor_of_tare->exit();
            f = fopen("test_bloc.txt", "at"); fprintf(f, "-dgood2 "); fclose(f);
            processor_of_tare->wait(2000);
            f = fopen("test_bloc.txt", "at"); fprintf(f, "-dgood3 "); fclose(f);
            delete processor_of_tare;
        }
        processor_of_tare = 0;
        time_work_of_processor = 0;
        f = fopen("test_bloc.txt", "at"); fprintf(f, "-dend "); fclose(f);
        return;
    }
    time_work_of_processor = 0;
    f = fopen("test_bloc.txt", "at"); fprintf(f, "-derror "); fclose(f);
}
//-------------------------------------------------------------



