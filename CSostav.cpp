//---------------------------------------------------------------------------
#include "CSostav.h"
#include "csavefiletxt.h"

//#define Dtype_run CAutoschepka::ETRTeor
#define Dtype_run CAutoschepka::ETRSimple

#define DRastSimple 60
//---------------------------------------------------------------------------
CSostav::CSostav():file_name(""), load_all(false)
{
  Clear();
}
//---------------------------------------------
CSostav::CSostav(const CSostav &s)
{
  *this = s;
}
//---------------------------------------------
CSostav::~CSostav()
{
  Clear();
}
//---------------------------------------------------------------------------
CSostav& CSostav::operator=(const CSostav &s)
{
  Clear();
  axels = s.axels;
  vans = s.vans;
  autoschepkas = s.autoschepkas;
  koof_a = s.koof_a;
  koof_b = s.koof_b;
  koof_c = s.koof_c;
  date_time = s.date_time;
  direction = s.direction;
  activ_van = s.activ_van;
  data = s.data;
  data_bin = s.data_bin;
  file_name = s.file_name;
  uk_autosch = s.uk_autosch;
  data_real = s.data_real;
  porog_defect = s.porog_defect;
  defects = s.defects;
  load_all = s.load_all;
  return *this;
}
//---------------------------------------------
CSostav::CSostav(const QString &fn)
{
    file_name = fn;
    CFileBin f(file_name, CFileBin::ETRead);
    if(!f.GetState())
        throw this;
  try
  {
    date_time = CDateTime(f);
    if(!f.Read(&direction, sizeof(direction), 1)) throw this;
    if(!f.Read(&koof_a, sizeof(koof_a), 1)) throw this;
    if(!f.Read(&koof_b, sizeof(koof_b), 1)) throw this;
    if(!f.Read(&koof_c, sizeof(koof_c), 1)) throw this;
    int n;
    SAxel a;
    if(!f.Read(&n, sizeof(n), 1)) throw this;
    for(int i = 0; i < n; i ++)
    {
        if(!f.Read(&a, sizeof(a), 1)) throw this;
        axels << a;
    }
    if(!f.Read(&n, sizeof(n), 1)) throw this;
    for(int i = 0; i < n; i ++)
      vans << CVan(f);
    if(!f.Read(&n, sizeof(n), 1)) throw this;
    for(int i = 0; i < n; i ++)
      autoschepkas << CAutoschepka(f);
  }
  catch(int)
  {
    Clear();
    throw this;
  }

  activ_van = 0;
  load_all = false;
}
//---------------------------------------------
bool CSostav::LoadData()
{
    CFileBin f(file_name, CFileBin::ETRead);
    bool ret = false;
    if(f.GetState())
    {
        CDateTime d(f);
        SElementFourth<double> k_a;
        SElementFourth<double> k_b;
        SElementFourth<int> k_c;
        if(!f.Read(&direction, sizeof(direction), 1)) throw this;
        if(!f.Read(&k_a, sizeof(k_a), 1)) throw this;
        if(!f.Read(&k_b, sizeof(k_b), 1)) throw this;
        if(!f.Read(&k_c, sizeof(k_c), 1)) throw this;
        int n;
        SAxel a;
        if(!f.Read(&n, sizeof(n), 1)) throw this;
        for(int i = 0; i < n; i ++)
            if(!f.Read(&a, sizeof(a), 1)) throw this;
        if(!f.Read(&n, sizeof(n), 1)) throw this;
        CVan v;
        for(int i = 0; i < n; i ++)
          v = CVan(f);
        if(!f.Read(&n, sizeof(n), 1)) throw this;
        CAutoschepka au;
        for(int i = 0; i < n; i ++)
          au = CAutoschepka(f);

        ret = data.LoadFromFile(f);
        defects.Clear();
        if(f.Read(&n, sizeof(n), 1))
            if(n == 2)
            {
                ret = data_real.LoadFromFile(f);
                if(!f.Read(&n, sizeof(n), 1)) throw this;
                for(int i = 0; i < n; i ++)
                    defects << CDefect(f);
            }
        load_all = true;
    }
    return ret;
}
//---------------------------------------------
bool CSostav::SaveToFile()
{
    if(file_name != "")
        SaveToFile(file_name);
}
bool CSostav::SaveToFile(const QString &fn)
{
    file_name = fn;
   // CFileBin f(file_name, CFileBin::ETWrite);
    CFileBin f(fn, CFileBin::ETWrite);
    if(!f.GetState())
        return false;
    try
    {
        date_time.SaveToFile(f);
        if(!f.Write(&direction, sizeof(direction), 1)) throw 0;
        if(!f.Write(&koof_a, sizeof(koof_a), 1)) throw 0;
        if(!f.Write(&koof_b, sizeof(koof_b), 1)) throw 0;
        if(!f.Write(&koof_c, sizeof(koof_c), 1)) throw 0;
        int n = axels.GetSize();
        if(!f.Write(&n, sizeof(n), 1)) throw 0;
        for(int i = 0; i < n; i ++)
            if(!f.Write(&axels[i], sizeof(axels[i]), 1)) throw 0;
        n = vans.GetSize();
        if(!f.Write(&n, sizeof(n), 1)) throw 0;
        for(int i = 0; i < n; i ++)
            vans[i].SaveToFile(f);
        n = autoschepkas.GetSize();
        if(!f.Write(&n, sizeof(n), 1)) throw 0;
        for(int i = 0; i < n; i ++)
            autoschepkas[i].SaveToFile(f);
        data.SaveToFile(f);
        n = 2;
        if(!f.Write(&n, sizeof(n), 1)) throw 0;
        data_real.SaveToFile(f);
        n = defects.GetSize();
        if(!f.Write(&n, sizeof(n), 1)) throw 0;
        for(int i = 0; i < n; i ++)
            defects[i].SaveToFile(f);
        load_all = true;
    }
    catch(int)
    {
        return false;
    }
    return true;
}
//---------------------------------------------
bool CSostav::SaveToFileTxt(const QString &fn)const
{
    CString str(fn);
    FILE *f = fopen(str.GetPtrChar(), "wb");
    if(!f)
        return false;
    char c[100];
    CString str1 = (direction)? "<" : ">";
    CString str2;//(vans.GetSize());
    int kol_van = 0;
    for(unsigned i = 0; i < vans.GetSize(); i ++)
        if(!vans[i].GetLokomotiv())
            kol_van ++;
//    fprintf(f, "002\n%s %s\n", str1.GetPtrChar(), str2.GetPtrChar());
    fprintf(f, "002\n%s %d\n", str1.GetPtrChar(), kol_van);
    kol_van = 0;
    for(unsigned i = 0; i < vans.GetSize(); i ++)
    {
        if(vans[i].GetLokomotiv())
            str1 = "Л";
        else
            str1 = CString(++kol_van, 2);
        str2 = CString(vans[i].GetSize(), 2);
        CString str3(vans[i].GetSpeedDouble(axels), 2);
        CString str4(vans[i].GetTareFile(axels));
        fprintf(f, "%s %s %s %s\n", str1.GetPtrChar(), str2.GetPtrChar(), str3.GetPtrChar(), str4.GetPtrChar());
    }
    fclose(f);
    return true;
}
//---------------------------------------------
bool CSostav::SaveToFileDefectTxt(const QString &fn)const
{
    CString str(fn);
    FILE *f = fopen(str.GetPtrChar(), "wb");
    if(!f)
        return false;
    CString str1 = (direction)? "<" : ">";

    CString str2;//(vans.GetSize());
    int kol_van = 0;
    for(unsigned i = 0; i < vans.GetSize(); i ++)
        if(!vans[i].GetLokomotiv())
            kol_van ++;
    fprintf(f, "001\n%s %d\n", str1.GetPtrChar(), kol_van);
    kol_van = 0;

    for(unsigned i = 0; i < vans.GetSize(); i ++)
    {
        if(vans[i].GetLokomotiv())
            str1 = "Л";
        else
            str1 = CString(++kol_van, 2);
        str2 = CString(vans[i].GetSize(), 2);
        CString str3(vans[i].GetSpeedDouble(axels), 2);
        CString str4 = CString(vans[i].GetTare(axels, 100, 100) / 1000, 2);
        CString str5 = vans[i].GetDefectFile(defects);
//        QString qstr7 = vans[i].GetType();
        fprintf(f, "%s %s %s %s %s -00/00 -00/00 - \n", str1.GetPtrChar(), str2.GetPtrChar(), str3.GetPtrChar(), str4.GetPtrChar(), str5.GetPtrChar());
    }
    fclose(f);
    return true;
}
//---------------------------------------------------------------------------
bool CSostav::SaveToFileBin(const QString &fn)
{
    CFileBin f(fn, CFileBin::ETWrite);
    if(!f.GetState())
        return false;
    SElementFourth<int> pb_null = param.GetPbNull();
    f.Write(&pb_null, sizeof(pb_null), 1);
    data_bin.SaveToFile(f);
    return true;
}
//---------------------------------------------------------------------------
void CSostav::Clear()
{
    date_time  = CDateTime();
    axels.Clear();
    data.Clear();
    data_real.Clear();
    vans.Clear();
    defects.Clear();
    autoschepkas.Clear();
    direction = false;
    activ_van = 0;
    uk_van = 0;
    uk_autosch = 0;
    load_all = false;
}
//---------------------------------------------------------------------------
void CSostav::AddDataBin(CStack<unsigned char> &d)
{
  int size = d.GetSize();
  unsigned char *tdata = new unsigned char [size];
  unsigned char *uk = tdata;
  d.ClearPtr();
  for(int i = 0; i < size; i ++, uk ++, d.AddPtr(1))
    *uk = d.GetElementPtr();
  data_bin = CBuffer<unsigned char>(size, tdata);
  delete [] tdata;
  CFileBin f("last.bin", CFileBin::ETWrite);
  if(f.GetState())
    data_bin.SaveToFile(f);
}
//---------------------------------------------------------------------------
void CSostav::AddData(CStack4<int> &d_filter, CStack4<int> &d_real, CStack4<int> &d_defect)
{
    //  FILE *f = fopen("test_end.txt", "wt"); fclose(f);
    //  f = fopen("test_end.txt", "at"); fprintf(f, "\n%d = %d - %d - %d", size, q3, q2, q1); fclose(f);
  int size = d_filter.GetSize();
  if(!size)
    return;
  SElementFourth<short> *tdata = new SElementFourth<short> [size];
  SElementFourth<short> *uk1 = tdata;
  SElementFourth<short> *tdataf = new SElementFourth<short> [size];
  SElementFourth<short> *ukf1 = tdataf;
  SElementFourth<short> *tdatad = new SElementFourth<short> [size];
  SElementFourth<short> *ukd1 = tdatad;
  d_filter.ClearPtr();
  d_real.ClearPtr();
  d_defect.ClearPtr();
  for(int i = 0; i < size; i ++, uk1 ++, ukf1 ++, ukd1 ++, d_filter.AddPtr(1), d_real.AddPtr(1), d_defect.AddPtr(1))
  {
      SElementFourth<int> tmp_i(d_real.GetElementPtr());
      SElementFourth<short> tmp_sh(tmp_i[0], tmp_i[1], tmp_i[2], tmp_i[3]);
      *uk1 = tmp_sh;
      SElementFourth<int> tmp_if(d_filter.GetElementPtr());
      SElementFourth<short> tmp_shf(tmp_if[0], tmp_if[1], tmp_if[2], tmp_if[3]);
      *ukf1 = tmp_shf;
      SElementFourth<int> tmp_id(d_defect.GetElementPtr());
      SElementFourth<short> tmp_shd(tmp_id[0], tmp_id[1], tmp_id[2], tmp_id[3]);
      *ukd1 = tmp_shd;
  }
  data_real = CBuffer<SElementFourth<short> >(size, tdata);
 // data_real = CBuffer<SElementFourth<short> >(size, tdatad);
  data = CBuffer<SElementFourth<short> >(size, tdataf);
  CBuffer<SElementFourth<short> > data_defect;
  data_defect = CBuffer<SElementFourth<short> >(size, tdatad);
  delete [] tdata;
  delete [] tdataf;
  delete [] tdatad;

  //Тип вагона
  type_of_van.Clear();
  for(unsigned i = 0; i < vans.GetSize(); i ++)
  {
    const unsigned axel[2] = {vans[i].GetBegin(), vans[i].GetBegin()+vans[i].GetSize()-1};
    SElement12<int> tv(vans[i].GetSize(), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    if(vans[i].GetSize() > 8)
    {
        type_of_van.AddVan(tv);
        continue;
    }
    for(unsigned i = axel[0] + 1; i <= axel[1]; i ++)
      tv[i-axel[0]+1] = axels[i].coordinate[0] - axels[i-1].coordinate[0];
    type_of_van.AddVan(tv);
  }
  type_of_van.Run();
  for(unsigned i = 0; i < vans.GetSize(); i ++)
  {
    int lokomotiv;
    CString str = type_of_van.GetType(i, lokomotiv);
    vans[i].SetType(str, lokomotiv);
  }
  // Прогиб
    size = data.GetSize();
    for(unsigned i = 0; i < axels.GetSize(); i ++)
    {
        for(unsigned n = 0; n < 4; n ++)
        {
            int coordinate = axels[i].coordinate[n];
            int begin = coordinate - 50;
            int end = coordinate + 50;
            if(begin >= size)
                begin = size - 1;
            if(begin < 0)
                begin = 0;
            if(end >= size)
                end = size - 1;
            if(end < 0)
                end = 0;
            short max = 0;
            for(int j = begin; j <= end; j ++)
                if(max < data[j][n])
                {
                    max = data[j][n];
                    coordinate = j;
                }
            axels[i].coordinate[n] = coordinate;
            axels[i].progib[n] = max;
            int n1 = coordinate + DRastSimple;
            int n2 = coordinate - DRastSimple;
            if(n1 >= size)
                n1 = size - 1;
            if(n2 < 0)
                n2 = 0;
            axels[i].progib_simple[n] = max - (data[n1][n] + data[n2][n]) / 2;
        }
    }
  // Вес
    SetLastAutoschepka();
    for(unsigned i = 0; i < autoschepkas.GetSize(); i ++)
    {
        if(Dtype_run == CAutoschepka::ETRSimple)
            autoschepkas[i].RunSampel(axels, koof_b, koof_c, direction);
        else
            autoschepkas[i].RunClassic(Dtype_run, axels, koof_a, koof_b, koof_c, direction);
    }
    if(param.GetAutoTarirovka())
    {
        unsigned kol = 0;
        SElementFourth<int> tare = 0;
        for(unsigned i = 0; i < vans.GetSize(); i ++)
        {
            if(vans[i].GetLokomotiv() && vans[i].GetType() == QString::fromLocal8Bit("ВЛ-80"))
            {
                kol ++;
                for(unsigned j = 0; j < 4; j ++)
                    if(param.GetonoffPb(j))
                        tare[j] += vans[i].GetTare(axels, 100, j);
            }
        }
        if(kol)
        {
            SElementFourth<int> sred_tare;
            SElementFourth<double> koof;
            const double koof_vl80 = static_cast<double>(param.GetkoofVl80()) * 1000.0 / 2.0;
            for(unsigned j = 0; j < 4; j ++)
                if(param.GetonoffPb(j))
                {
                    sred_tare[j] = tare[j] / kol;
                    koof[j] = koof_vl80 / static_cast<double>(sred_tare[j]);
                }
            for(unsigned i = 0; i < axels.GetSize(); i ++)
                for(unsigned j = 0; j < 4; j ++)
                    if(param.GetonoffPb(j))
                        axels[i].tare[j] = static_cast<int>(static_cast<double>(axels[i].tare[j]) * koof[j]);
            for(unsigned j = 0; j < 4; j ++)
                if(param.GetonoffPb(j))
                    param.SetKoofB(j, static_cast<double>(param.GetKoofB(j)) * koof[j]);
            for(unsigned j = 0; j < 4; j ++)
                if(param.GetonoffPb(j))
                    koof_b[j] = (static_cast<double>(koof_b[j]) * koof[j]);
        }
    }
    // Дефект
    int tek_van = 0;
    defects.Clear();
    for(int i = 0; i < axels.GetSize(); i ++)
    {
        int tare = 0;
        int kol = 0;
        for(int j = 0; j < 4; j ++)
            if(param.GetonoffPb(j))
            {
                tare += (axels[i].tare[j] / 1000);
                kol ++;
            }
        if(kol)
            tare /= kol;
        unsigned size = (param.GetRastA(2) - param.GetRastA(1)) - param.GetRastA(0);

        CBuffer<SElementFourth<short> > out_real(size);
        CBuffer<SElementFourth<short> > out_fft(size);

        CFFT fft(data_defect, out_real, out_fft, axels[i].coordinate);
        defects << CDefect(out_real, out_fft, axels[i].speed, tare);
    }
}
//---------------------------------------------------------------------------
void CSostav::SetLastAutoschepka()
{
  int kol_axel_in_autoschepka = axels.GetSize()- uk_autosch;
  if(kol_axel_in_autoschepka)
  {
    autoschepkas << CAutoschepka(uk_autosch);
    autoschepkas[autoschepkas.GetSize()-1].SetSizeAxel(axels.GetSize() - uk_autosch);
  //  autoschepkas[autoschepkas.GetSize()-1].Run(axels, koof_a, koof_b, koof_c, direction);
  }
}
//---------------------------------------------------------------------------
void CSostav::AddAxel(const SAxel &a, const bool &new_autoschepka, const bool &new_van)
{
  axels << a;
  if(new_autoschepka)
  {
    autoschepkas << CAutoschepka(uk_autosch);
    int kol_axel_in_autoschepka = axels.GetSize()- uk_autosch;
    autoschepkas[autoschepkas.GetSize()-1].SetSizeAxel(kol_axel_in_autoschepka);
    uk_autosch = axels.GetSize();
  }
  if(new_van)
  {
    vans << CVan(uk_van);
    vans[vans.GetSize()-1].SetSizeAxel(axels.GetSize() - uk_van);
    uk_van = axels.GetSize();
  }
}
//---------------------------------------------------------------------------
void CSostav::Run()
{
    for(unsigned i = 0; i < autoschepkas.GetSize(); i ++)
        if(Dtype_run == CAutoschepka::ETRSimple)
            autoschepkas[i].RunSampel(axels, koof_b, koof_c, direction);
        else
            autoschepkas[i].RunClassic(Dtype_run, axels, koof_a, koof_b, koof_c, direction);
}
//---------------------------------------------
//---------------------------------------------
//---------------------------------------------
//---------------------------------------------
//---------------------------------------------
//---------------------------------------------
//---------------------------------------------
//---------------------------------------------
//---------------------------------------------
//---------------------------------------------
//---------------------------------------------
//---------------------------------------------
//---------------------------------------------
//---------------------------------------------
void CSostav::SetActiv(CTable *table_vans, CImage *image_arch, CImage *image_real, int &dial_arch, const bool type)
{
    if(!load_all)
        LoadData();
    PaintVan(table_vans, image_arch, image_real, dial_arch, type);
}
void CSostav::SetDeActiv()
{
    data.Clear();
    data_real.Clear();
    defects.Clear();
    load_all = false;
}
void CSostav::PaintSostav(QList<QString> &list)
{
    list.clear();
    list << date_time.GetTime();
    list << ((direction)? "<-" : "->");
    list << QString::number(GetKolLokomotivs());
    list << QString::number(GetKolVans());
    list << QString::number(GetTareAll()/1000);
}
//---------------------------------------------
void CSostav::PaintVan(CTable *table_vans, CImage *image_arch, CImage *image_real, int &dial_arch, const bool type)
{
    table_vans->Clear();
    unsigned kol_van = 0;
    for(unsigned i = 0; i < vans.GetSize(); i ++)
    {
        QList<QString> l;
        l << ((vans[i].GetLokomotiv())? vans[i].GetType() : QString::number(++kol_van));
        l << QString::number(vans[i].GetSize());
        l << vans[i].GetSpeed(axels);
        //l << QString::number(GetProgib(i, 100, 100)/10);
        l << vans[i].GetTareStr1(axels, 100, 100);
        if(type)
            l << ((vans[i].GetKolDefect(defects, 100, 12))? ("    X") : (""));
        else
        {
            l << vans[i].GetTareStr1(axels, 100, 103);
            l << vans[i].GetTareStr1(axels, 100, 104);
        }
        table_vans->AddLine(l);
    }
    SetActivVan(image_arch, image_real, dial_arch, activ_van);
    if(vans.GetSize())
        table_vans->SetChecked(activ_van);
}
void CSostav::SetActivVan(CImage *image_arch, CImage *image_real, int &dial_arch, const int n)
{    
    if(n >= vans.GetSize())
        return;
    if(n >= 0)
        activ_van = n;
    PicGraphik(image_arch, dial_arch, false, true);
    PicGraphik(image_real, dial_arch, true, true);
}
void CSostav::PicGraphik(CImage *image_arch, int &dial_arch, const bool real, const bool new_pos)
{
    CBuffer<SElementFourth<short> > &data_tmp = (real)? data_real : data;
    const int size = data_tmp.GetSize();
    if(!size)
    {
        image_arch->ClearImage();
        return;
    }
    if((activ_van < 0) || (activ_van >= vans.GetSize()))
    {
        image_arch->ClearImage();
        return;
    }
    if(new_pos)
    {
      int pos_new = vans[activ_van].GetCoordinate(axels, 0) - 100;
      dial_arch = (pos_new >= 0)? pos_new : 0;
    }
  //  else
       // dial_arch = 1000;

    //const int check = 0;//      int check = (check_con->Checked)? n : 0;
    const int sm_coord = dial_arch - vans[activ_van].GetCoordinate(axels, 0);
    const int width = image_arch->GetWidth();
    const double height = image_arch->GetHeight();
    const double compress_hor = param.GetArchCompressxPb() / 10.0;
    const double compress_vert = (double)param.GetArchCompressyPb() / 10.0;

    if(image_arch->GetState() == CImage::esVesAndPicture || image_arch->GetState() == CImage::esPictureOnly)
    {
        for(int n = 0; n < 4; n ++)
        {
            if(!param.GetArchonoffPb(n))
                continue;
            const int check = (param.GetArchTogether())? n : 0;
            QPointF* ptr_data = image_arch->GetPtrData(n);
            bool *my_van = image_arch->GetBoolMyVan();
            int begin = vans[activ_van].GetCoordinate(axels, check) - 200 + sm_coord;
    //        int begin = vans[activ_van].GetCoordinate(axels, n) - 200 + sm_coord;
            int begin_my_van = vans[activ_van].GetCoordinate(axels, n) - 150;
            int end_my_van = vans[activ_van].GetCoordinate(axels, n, vans[activ_van].GetSize()-1) + 150;
            for(double x = 0; x < width; x += 1.0)
            {
                bool koleso = false;
           // if(begin + x * compress_hor > size)
            //	  break;
                int end = (begin + x * compress_hor);
                int beg = begin + (x-1) * compress_hor;
                if(beg < 0) beg = 0;
                double pr = (beg < size)? data_tmp[beg][n] : 0;
                for(int j = beg+1; j < end; j ++)
                {
                    if(j < size)
                    pr += data_tmp[j][n];
                }
                if(end - beg)
                    pr /= (end - beg);
           //      pr /= compress_hor;
                int x2 = (!direction)? width-x : x;
                double y2 = (pr) / compress_vert;
                ptr_data[x2] = QPointF(x2, y2);
                my_van[x2] = (beg >= begin_my_van && end <= end_my_van);
            }
        }
    }
    if(image_arch->GetState() == CImage::esVesAndPicture || image_arch->GetState() == CImage::esVesOnly)
    {
        int number_str = 0;
        QString str1 = (vans[activ_van].GetLokomotiv())? QString::fromLocal8Bit("локомотив") : QString::fromLocal8Bit("вагон");
        image_arch->SetVes(number_str++, QString::fromLocal8Bit("cat вес.") + str1 +  QString::fromLocal8Bit("a.") + vans[activ_van].GetType());
        image_arch->SetVes(number_str++, str1 + " " + vans[activ_van].GetType());
        image_arch->SetVes(number_str++, vans[activ_van].GetTareStr(axels, 100, 100) + QString::fromLocal8Bit(" т - вес"));
        image_arch->SetVes(number_str++, vans[activ_van].GetTareStr(axels, 101, 100) + QString::fromLocal8Bit(" т - вес первой тележки"));
        image_arch->SetVes(number_str++, vans[activ_van].GetTareStr(axels, 102, 100) + QString::fromLocal8Bit(" т - вес второй тележки"));
        image_arch->SetVes(number_str++, QString::fromLocal8Bit("дат-|   оси"));
        QString str2 = QString::fromLocal8Bit("чики| все|");
        QString str3 = "----|----|";
        const int beg = (direction)? 0 : vans[activ_van].GetSize() - 1;
        const int end = (direction)? vans[activ_van].GetSize() : -1;
        const int step = (direction)? 1 : -1;
        for(int i = beg; i != end; i += step)
        {
            str2 += (" #" + QString::number(i+1) + "  ");
            str3 += ("-----");
        }
        image_arch->SetVes(number_str++, str2);
        image_arch->SetVes(number_str++, str3);
        for(int n = 0; n < 4; n ++)
        {
            QString str = (vans[activ_van].GetTareStr1(axels, 100, n)) + "|";
            for(int i = beg; i != end; i += step)
                str += (vans[activ_van].GetTareStr1(axels, i, n) + " ");
            image_arch->SetVes(number_str++, QString::fromLocal8Bit("пб") + QString::number(n+1) + " |" + str);
        }
        image_arch->SetVes(number_str++, QString::fromLocal8Bit("пб1   пб2   пб3   пб4"));
        image_arch->SetVes(number_str++, QString::number(GetTareDatAll(0)) + QString::fromLocal8Bit("т ") +
                                         QString::number(GetTareDatAll(1)) + QString::fromLocal8Bit("т ") +
                                         QString::number(GetTareDatAll(2)) + QString::fromLocal8Bit("т ") +
                                         QString::number(GetTareDatAll(3)) + QString::fromLocal8Bit("т "));
    }
    image_arch->Paint();
}
void CSostav::PicDefect(CImageDefect *image_arch, unsigned axel)
{
    if(activ_van >= vans.GetSize())
    {
        image_arch->ClearImage();
        return;
    }
    int na1 = vans[activ_van].GetBegin();
    int na2 = axel + na1;
    if(na2 >= defects.GetSize())
    {
        image_arch->ClearImage();
        return;
    }
    const unsigned width = image_arch->GetWidth();
    for(int n = 0; n < 4; n ++)
    {
        if(!param.GetArchonoffPb(n))
        {
            image_arch->SetDefectSize(n, 9876);
            continue;
        }
        QPointF* ptr_data1 = image_arch->GetPtrData1(n);
        QPointF* ptr_data2 = image_arch->GetPtrData2(n);
        image_arch->SetPorog(n, defects[na2].GetPorogDefect(n));
        image_arch->SetDefectSize(n, defects[na2].GetDefectSize(n));
        defects[na2].Paint(n, direction, width, ptr_data1, ptr_data2);
    }
    image_arch->Paint();
}
//---------------------------------------------
void CSostav::SetPS(const QString &ps_new1, const QString &ps_new2, const unsigned &n)
{
  if(vans.GetSize() && (n < vans.GetSize()))
    vans[n].SetPS(SElementDual<QString>(ps_new1, ps_new2));
}
//---------------------------------------------
QString CSostav::GetPS(const unsigned &n, const unsigned &n2)const
{
  if(n2>1 || n>=vans.GetSize())
    return "";
  return vans[n].GetPS(n2);
}
//---------------------------------------------------------------------------
unsigned CSostav::GetSizeVan(unsigned n)const
{
    if(n == 10000)
        n = activ_van;
    return (n>=vans.GetSize())? 0 : vans[n].GetSize();
}
//---------------------------------------------------------------------------
unsigned CSostav::GetKolVans()
{
    unsigned ret = 0;
    for(unsigned i = 0; i < vans.GetSize(); i ++)
      if(!vans[i].GetLokomotiv())
        ret ++;
    return ret;
}
//---------------------------------------------------------------------------
int CSostav::GetTare(const unsigned &n, const unsigned &naxel, const unsigned &ndat)const
{
  if(n>=vans.GetSize())
    return 0;
  return vans[n].GetTare(axels, naxel, ndat);
}
//---------------------------------------------------------------------------
int CSostav::GetTareAll(bool lok)const
{
  int ret = 0;
  for(unsigned i = 0; i < vans.GetSize(); i ++)
  {
    if(!lok && GetLokomotiv(i))
      continue;
    ret += vans[i].GetTare(axels, 100, 100);
  }
  return ret;
}
//---------------------------------------------------------------------------
int CSostav::GetTareDatAll(int dat, bool lok)const
{
  int ret = 0;
  for(unsigned i = 0; i < vans.GetSize(); i ++)
  {
    if(!lok && GetLokomotiv(i))
      continue;
    ret += vans[i].GetTare(axels, 100, dat);
  }
  return ret / 1000;
}
//---------------------------------------------------------------------------
QString CSostav::GetTareStr(const unsigned &n, const unsigned &naxel, const unsigned &ndat)const
{
  if(n>=vans.GetSize())
    return 0;
  return vans[n].GetTareStr(axels, naxel, ndat);
}
//---------------------------------------------------------------------------
int CSostav::GetProgib(const unsigned &n, const unsigned &naxel, const unsigned &ndat)const
{
  if(n>=vans.GetSize())
    return 0;
  return vans[n].GetProgib(axels, naxel, ndat);
}
//---------------------------------------------------------------------------
double CSostav::GetSpeedDouble(const unsigned &n) const
{
  if(n>=vans.GetSize())
    return 0;
  return vans[n].GetSpeedDouble(axels);
}
//---------------------------------------------
QString CSostav::GetSpeed(const unsigned &n) const
{
  if(n>=vans.GetSize())
    return 0;
  return vans[n].GetSpeed(axels);
}
//---------------------------------------------------------------------------
bool CSostav::GetLokomotiv(const unsigned &n)const
{
  if(n >= vans.GetSize())
    return false;
  return (vans[n].GetLokomotiv())? true : false;
}
//---------------------------------------------------------------------------
unsigned CSostav::GetKolDefect()
{
    unsigned ret = 0;
    for(unsigned i = 0; i < defects.GetSize(); i ++)
        if(defects[i].GetKolDefect())
            ret ++;
    return ret;
}
bool CSostav::GetDefect(const unsigned n, const unsigned a, const unsigned d)
{
    if(n >= vans.GetSize())
        return false;
    return vans[n].GetKolDefect(defects, a, d) != 0;
}
//---------------------------------------------------------------------------
void CSostav::GetData(CBuffer<SElementFourth<short> > &d)
{
  d = data;
}
//---------------------------------------------------------------------------
bool CSostav::GetAxel(const unsigned &n, SAxel &a)
{
  if(n >= axels.GetSize())
    return false;
  a = axels[n];
  return true;
}
void CSostav::Print(QPrinter *printer)//QPaintDevice *printer)
{
    QPainter painter(printer);
    QRect rect(painter.viewport());

    double bw = rect.width();// GetDeviceCaps(prn->Handle, HORZRES);
    double bh = rect.height();// GetDeviceCaps(prn->Handle, VERTRES);
    double width = 200;//GetDeviceCaps(prn->Handle, HORZSIZE);
    double height = 285;//GetDeviceCaps(prn->Handle, VERTSIZE);
    double mmw = bw / width;
    double mmh = bh / height;
    double mmpole = width / 21;
    //double base_shrift = (width > 250)? 12 : 9;
//    QFont font("Courier New", base_shrift+4, base_shrift+4);
    QFont font(painter.font());
//    font.setBold(true);

    font.setPointSize(16);
    painter.setFont(font);
    painter.setBrush(Qt::white);
    painter.setPen(Qt::black);



    unsigned kol_van = GetNumberOfVan();
    unsigned kol_van2 = 0;
    int y_line = 30;
    int y_step = 6;
    bool new_page = true;
    int kol_page = 1;
    for(unsigned i = 0; i < kol_van; i ++)
    {
        if(GetLokomotiv(i))
            continue;
        kol_van2 ++;
    }
    painter.drawText(mmpole*6*mmw, 7*mmh, QString::fromLocal8Bit("Протокол взвешивания подвижного состава"));
    painter.drawText(mmpole*3*mmw, 15*mmh, GetDate() + " " + GetTime());
    font.setPointSize(14); painter.setFont(font);
    painter.drawText(mmpole*3*mmw, 22*mmh, QString::fromLocal8Bit("Количество вагонов: ") +
                     QString::number(kol_van2) +
                     QString::fromLocal8Bit(". Общая масса ") +
                     QString::number(GetTareAll()/1000) +
                     QString::fromLocal8Bit(" т."));
    kol_van2 = 0;
    font.setPointSize(11); painter.setFont(font);
    for(unsigned i = 0; i < kol_van; i ++)
    {
        if(GetLokomotiv(i))
            continue;
        kol_van2 ++;
        if(new_page)//шапка
        {
            painter.drawText(mmpole*2*mmw, y_line*mmh, QString::fromLocal8Bit("№"));
            painter.drawText((mmpole*7)*mmw, y_line*mmh, QString::fromLocal8Bit("Скор., км/ч"));
            painter.drawText(mmpole*10*mmw, y_line*mmh, QString::fromLocal8Bit("Вес 1 сторона,т"));
            painter.drawText(mmpole*14*mmw, y_line*mmh, QString::fromLocal8Bit("Вес 2 сторона,т"));
            painter.drawText(mmpole*18*mmw, y_line*mmh, QString::fromLocal8Bit("Вес,т"));
            PaintCells(painter, y_line-y_step/2, y_line+y_step/2, mmpole, mmw, mmh);
            painter.drawText(mmpole*20*mmw, 280*mmh, QString::number(kol_page++));
            new_page = false;
        }
        QString str_speed = GetSpeed(i);
        QString str_progib = QString::number(GetProgib(i, 100, 100));
        QString str_tare = GetTareStr(i, 100, 100);
        QString str_tare1 = GetTareStr(i, 100, 103);
        QString str_tare2 = GetTareStr(i, 100, 104);
        y_line+=y_step;
        painter.drawText(mmpole*2*mmw,  y_line*mmh, QString::number(kol_van2));
        painter.drawText(mmpole*7*mmw, y_line*mmh, str_speed);
        painter.drawText(mmpole*10*mmw, y_line*mmh, str_tare1);
        painter.drawText(mmpole*14*mmw, y_line*mmh, str_tare2);
        painter.drawText(mmpole*18*mmw, y_line*mmh, str_tare);
//        painter.drawText(mmpole*18*mmw, y_line*mmh, str_progib);
        PaintCells(painter, y_line-y_step/2, y_line+y_step/2, mmpole, mmw, mmh);
        if(y_line > 277)
        {
            printer->newPage();
            y_line = 10;
            new_page = true;
        }
    }
}

void CSostav::PaintCells(QPainter &painter, const int &y1, const int &y2, double &mmpole, double &mmw, double &mmh)
{
    painter.drawLine((mmpole*2 - 2)*mmw,  (y1-1)*mmh, (mmpole*20 - 2)*mmw, (y1-1)*mmh);
    painter.drawLine((mmpole*2 - 2)*mmw,  (y2-1)*mmh, (mmpole*20 - 2)*mmw, (y2-1)*mmh);
    painter.drawLine((mmpole*2 - 2)*mmw,  (y1-1)*mmh, (mmpole*2 - 2)*mmw,  (y2-1)*mmh);
    painter.drawLine((mmpole*7 - 2)*mmw,  (y1-1)*mmh, (mmpole*7 - 2)*mmw,  (y2-1)*mmh);
    painter.drawLine((mmpole*10 - 2)*mmw, (y1-1)*mmh, (mmpole*10 - 2)*mmw, (y2-1)*mmh);
    painter.drawLine((mmpole*14 - 2)*mmw, (y1-1)*mmh, (mmpole*14 - 2)*mmw, (y2-1)*mmh);
    painter.drawLine((mmpole*18 - 2)*mmw, (y1-1)*mmh, (mmpole*18 - 2)*mmw, (y2-1)*mmh);
    painter.drawLine((mmpole*20 - 2)*mmw, (y1-1)*mmh, (mmpole*20 - 2)*mmw, (y2-1)*mmh);
}
