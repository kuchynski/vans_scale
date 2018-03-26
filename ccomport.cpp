#include "ccomport.h"
#define port_speed 115200
//#define port_speed 100000

CComPort::CComPort(QObject *parent) : QThread(parent),
    number_port(0), new_number_port(0), hCom(0), //port(0),
    enabled_sostav(false), f(0)
{
}
CComPort::~CComPort()
{
    DisConnect(cpsDisConnect);
}

void CComPort::run()
{
    SetState(cpsNoState);
    st_koleso[0] = st_koleso[1] = 0;
    BYTE byte;
    bool ok = false;
    while(1)
    {
        if(IsNewNumberPort())
        {
            DisConnect(cpsDisConnect);
            if(number_port == 100)//завершить поток
                break;
            ok = false;
            Connect();
        }
        if(state == cpsConnect)
        {
            if(IsNewSecond())
            {
                bloc.IRQNewSecond();
                state_bloc.enabled = (st_packet > 25);
                st_packet = 0;
                if(!state_bloc.enabled && (number_port < 10))
                {
                    bad_second ++;
                    if(port_speed == 100000)
                    {
                        if(bad_second > 1)
                            DisConnect(cpsReload);
                    }
                    else
                        emit NewState(cpsConnect2);
                }
                else
                {
                    bad_second = 0;
                    if(port_speed == 115200)
                        emit NewState(cpsConnect1);
                }
            }
            if(bloc.IsNewSostav())
                OnNewSostav();
            if(!ok)
                WaitEndMessage();
            ok = false;
            if(ReadByte(&byte))
            {
                switch(byte)
                {
                    case 0x30: // Состояние
                        ok = PacketState();
                        break;
                    case 0x73: // Путевой блок
                        ok = PacketPB();
                        break;
                    default : break;
                }
            }
        }
        else
            msleep(100);
    }
}
void CComPort::Connect()
{
    if(!number_port)
    {
        SetState(cpsNoState);
        return;
    }
    int ok = false;
    if(number_port < 10)
    {
        do{
            WCHAR name[6] = {'C', 'O', 'M', '0' + number_port};
            COMMCONFIG ComCfg;
            COMMTIMEOUTS CommTimeouts;
            BOOL fSuccess;

            ComCfg.dwSize = sizeof(COMMCONFIG);
            ComCfg.wVersion = 1;
            ComCfg.dwProviderSubType = PST_RS232;
            ComCfg.dwProviderOffset = 0;
            ComCfg.dwProviderSize = 0;
            //t = (kol*ReadTotalTimeoutMultiplier + ReadTotalTimeoutConstant)
            CommTimeouts.ReadIntervalTimeout = 100;//между двумя символами
            CommTimeouts.ReadTotalTimeoutMultiplier = 1;
            CommTimeouts.ReadTotalTimeoutConstant = 100;
            CommTimeouts.WriteTotalTimeoutMultiplier = 0;
            CommTimeouts.WriteTotalTimeoutConstant = 0;
            SetDefaultCommConfig(name, &ComCfg, sizeof(ComCfg));
            hCom = CreateFile(name, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
            if(hCom == INVALID_HANDLE_VALUE)
                break;
            GetCommState(hCom, &ComCfg.dcb);
            ComCfg.dcb.ByteSize = 8;
            ComCfg.dcb.Parity = NOPARITY;
            ComCfg.dcb.StopBits = ONESTOPBIT;
            ComCfg.dcb.BaudRate = port_speed;
            SetCommState(hCom, &ComCfg.dcb);
            SetCommTimeouts(hCom, &CommTimeouts);
            SetupComm(hCom, 100000, 0);
            PurgeComm(hCom, PURGE_RXCLEAR);
            ok = true;
        }while(0);

      /*  port = new QextSerialPort("COM" + QString::number(number_port));//, QextSerialPort::EventDriven); //Polling);
        ok = port->open(QIODevice::ReadOnly);//QIODevice:: ReadWrite);
        port->setBaudRate(BAUD100000);
        port->setFlowControl(FLOW_OFF);//FLOW_HARDWARE);//FLOW_OFF FLOW_XONXOFF);
        port->setParity(PAR_NONE);
        port->setDataBits(DATA_8);
        port->setStopBits(STOP_1);
        port->setTimeout(1);*/
    }
    else
    {
        f = fopen("test.bin", "rb");
        ok = (f)? 1 : 0;
    }
    if(!ok)
    {
        CloseHandle(hCom);
        hCom = 0;
//        delete port;
//        port = 0;
        SetState(cpsError);
        return;
    }
    st_packet = 0;
    bad_second = 0;
    SetState(cpsConnect);
    return;
}
void CComPort::DisConnect(eComPortState new_state)
{
  //  if(port)
    if(hCom)
    {
        CloseHandle(hCom);
        hCom = 0;
//        port->close();
       // delete port;
  //      port = 0;
    }
    if(f)
    {
        fclose(f);
        f = 0;
    }
    data_bin.Clear();
    bloc.Clear(false);
    enabled_sostav = false;
    state_bloc.enabled = false;
    state_bloc.connect_pb = 0;
    state_bloc.state_pedal = 0;
    SetState(new_state);
    return;
}
bool CComPort::IsNewNumberPort(const bool &set)
{
    if(new_number_port == number_port)
        return false;
    if(set)
    {
        if(new_number_port > 100)
            new_number_port -= 100;
        number_port = new_number_port;
    }
    return true;
}
bool CComPort::IsNewSecond()
{
    if(new_second == second)
        return false;
    second = new_second;
    return true;
}
//---------------------------------------------------------------------------
bool CComPort::PacketState()
{
   // if(number_port >= 10)
   //     msleep(1);
    BYTE bytes[10];
    if(!ReadPacket(bytes, 10))
      return false;
    state_bloc.state_pedal = (bytes[0] >> 4) ^ 0x03;
    state_bloc.connect_pb = (bytes[0] & 0xF) ^ 0xF;
    SElementDual<unsigned> pedal_state(bytes[1], bytes[2]);
    for(int i = 0; i < 2; i++)
      state_bloc.data_dm[i] = (pedal_state[i] & 0x80)? static_cast<int>(pedal_state[i] | 0xFFFFFF00) : static_cast<int>(pedal_state[i]);
    BYTE kirunak = bytes[3];
    if(kirunak > 0x02)
        return false;
    bool new_enabled = (kirunak > 0);
    enabled_sostav = new_enabled;
    state_bloc.enabled = true;
    if(bloc.IRQState(state_bloc, kirunak, data_bin))
        data_bin.Clear();
    unsigned new_st_koleso[2] = {(unsigned)(bytes[7] & 0x0F), (unsigned)(bytes[7] >> 4)};
    if((new_st_koleso[0] != st_koleso[0]) || (new_st_koleso[1] != st_koleso[1]))
    {
        unsigned st_time_koleso = (unsigned)((DWORD)bytes[4] + ((DWORD)bytes[5] << 8) + ((DWORD)(bytes[6] & 0x0F) << 16));
       // unsigned pedal = (unsigned)(bytes[6] >> 4);
        if(kirunak && (new_st_koleso[0] != st_koleso[0]))
        {
            bloc.IRQKoleso(st_time_koleso, 0);
           // if(bloc.IRQKoleso(st_time_koleso, pedal, st_koleso))
           // {}//    new_sostav = true;
           // pedal = (pedal == 0)? 1 : 0;
            st_time_koleso = 0;
        }
        if(kirunak && (new_st_koleso[1] != st_koleso[1]))
            bloc.IRQKoleso(st_time_koleso, 1);
           // if(bloc.IRQKoleso(st_time_koleso, pedal, st_koleso))
           // {}//    new_sostav = true;
        for(int i = 0; i < 2; i++)
            st_koleso[i] = new_st_koleso[i];
    }
    st_packet ++;
  //  if(new_sostav)
  //      OnNewSostav();
    return true;
}
//---------------------------------------------------------------------------
bool CComPort::PacketPB()
{
    BYTE bytes[10];

    if(!ReadPacket(bytes, 10))
        return false;

    unsigned ub[8];
    for(int i = 0; i < 8; i ++)
        ub[i] = static_cast<unsigned>(bytes[i]);
    SElementFourth<int> data_pb;
    for(int i = 0; i < 4; i ++)
    {
        unsigned z = (ub[i*2+1] << 8) + ub[i*2];
        if(z & 0x8000)
            data_pb[i] = static_cast<int>(z | 0xFFFF0000);
        else
            data_pb[i] = static_cast<int>(z);
        state_bloc.data_pb[i] = (param.GetNSPb(i))? data_pb[i] : -data_pb[i];
    }
    state_bloc.pb_null = bloc.IRQPB(state_bloc.data_pb);
    return true;
}
//---------------------------------------------------------------------------
bool CComPort::WaitEndMessage()
{
    BYTE byte;
    int st_bad_mes = 0;
    int st_no_read = 0;
    do{
    //    if(!NewReadPort(byte))
        if(!ReadByte(&byte))
            st_no_read ++;
        if(byte == 0x22)
            return true;
        if(IsNewNumberPort(false))
            return false;
        st_bad_mes ++;
        if((st_bad_mes > 50) || (st_no_read > 10))
        {
            return false;
        }
    }while(1);
    return true;
}
//---------------------------------------------------------------------------
bool CComPort::InByte(BYTE *byte, unsigned char kol)
{
    char w;
    DWORD n1 = kol, n2 = 0;

    bool ret = ReadFile(hCom, byte, n1, &n2, NULL);
//    int ret =  port->read(&w, 1);
  //  if((ret == true) && n2 == n1)
  //      n2 = 1;
  //  else
  //      n2 = 0;
//    byte = w;
    return ret && (n2 == n1);
//  return (ret == 1);
}
//-------------------------------------------------------------
bool CComPort::ReadByte(BYTE *byte, unsigned char kol)
{
    if(number_port < 10)
    {
        if(InByte(byte, kol))
        {
            if(enabled_sostav)
                for(unsigned i = 0; i < kol; i++)
                    data_bin << byte[i];
            return true;
        }
    }
    else
    {
        if(fread(&byte, 1, kol, f) == 1)
          return true;
        else
          bloc.IRQEnd();
    }
    return false;
}
//---------------------------------------------------------------------------
bool CComPort::ReadPacket(BYTE *byte, const int &kol)
{
    BYTE crc = 0x55;
    if(!ReadByte(byte, kol))
      return false;
    for(int i = 0; i < (kol-2); i ++)
      crc = crc ^ byte[i];
    if((byte[kol-2] != crc) || (byte[kol-1] != 0x22))
      return false;
    return true;
}
//---------------------------------------------------------------------------

