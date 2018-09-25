#include <n9000a.h>
#include <windows.h>
#include <QDebug>

#include <QThread>

N9000A::N9000A()
{

    this->moveToThread(new QThread());

    connect(this->thread(),&QThread::started,this,&N9000A::process_start);

    this->thread()->start();
}


N9000A::~N9000A()
{
    viClose (vi);
    viClose (defaultRM);
}

void N9000A::DisConnect()
{
    viClose (vi);
    viClose (defaultRM);
}

void N9000A::process_start()
{
   connected = false;
}

void N9000A::Connect(QString ip)
{

    DisConnect();

    viStatus=viOpenDefaultRM(&defaultRM);

    QString str  = "TCPIP0::"+ip+"::inst0::INSTR";

 //   viAddres = ViRsrc(qPrintable(str));                                         // Присваиваем нужному типу ip адрес сформировавшейся строки

    viStatus=viOpen(defaultRM,  ViRsrc(qPrintable(str)), ViAccessMode(VI_NULL),ViUInt32(VI_NULL),&vi);  // Открытие сессии с устройством по заданному ip

    Log2("Соединение с устройством N900A: ");
    if(viStatus<VI_SUCCESS)
    {

        emit Log("===================\n");
        emit Log("[Соединение не установленно(ERROR)] \n");
        emit Log("===================\n");

        emit Log2("[Соединение не установленно(ERROR)] \n");
    }
    else
    {
        connected = true;

        emit Log("===================\n");

        emit Log("[Соединение установленно(ОК)] \n");

        emit Log("===================\n");


        emit Log2("[Соединение установленно(ОК)] \n");

       // viPrintf(vi, const_cast<ViString>("*CLS"));
       // viPrintf(vi, const_cast<ViString>("*RST"));
        viPrintf(vi, const_cast<ViString>("SYSTem:PRESet"));

        viPrintf(vi, const_cast<ViString>("CALibration:AUTO OFF"));

        emit Log("Сброс настроек: Поумолчанию \n");

    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Команда на проверку ошибок в запросе
QString N9000A::query_ERRor()
{
    char buff[1024] = "";
    viQueryf(vi,const_cast<ViString>("SYSTem:ERRor?\n"),const_cast<ViString>("%T"),buff);


    return qPrintable(buff);

}

//Команда на проверку ошибок в запросе
QString N9000A::query_IDN()
{
    char buff[1024] = "";
    viQueryf(vi,const_cast<ViString>("*IDN?\n"),const_cast<ViString>("%T"),buff);

    QString error = query_ERRor();

    if(error == "+0,\"No error\"\n")
    {
        return qPrintable(buff);
    }
    else
    {
        return error;
    }
}


//Команда на проверку ошибок в запросе
QString N9000A::query_AVER_STAT()
{
    char buff[1024] = "";
    viQueryf(vi,const_cast<ViString>("AVER:STAT?\n"),const_cast<ViString>("%T"),buff);

    QString error = query_ERRor();

    if(error == "+0,\"No error\"\n")
    {
        return qPrintable(buff);
    }
    else
    {
        return error;
    }
}


void N9000A::write_AVER_STAT(bool mod)
{
    viPrintf(vi, const_cast<ViString>("AVER:STAT %d\n"),mod);
}

void N9000A::write_CALC_MARK_MODE(QString mod) //NORM
{
    viPrintf(vi, const_cast<ViString>("CALC:MARK:MODE %s\n"),qPrintable(mod));
}

//Команда на проверку ошибок в запросе
QString N9000A::query_CALC_MARK_MODE()
{
    char buff[1024] = "";
    viQueryf(vi,const_cast<ViString>("CALC:MARK:MODE?\n"),const_cast<ViString>("%T"),buff);

    QString error = query_ERRor();

    if(error == "+0,\"No error\"\n")
    {
        return qPrintable(buff);
    }
    else
    {
        return error;
    }
}

void N9000A::write_DISP_WIND_TRAC_Y_RLEV(int power) //NORM
{
    viPrintf(vi, const_cast<ViString>("DISP:WIND:TRAC:Y:RLEV %d dBm\n"),power);
}

//Команда на проверку ошибок в запросе
QString N9000A::query_DISP_WIND_TRAC_Y_RLEV()
{
    char buff[1024] = "";
    viQueryf(vi,const_cast<ViString>("DISP:WIND:TRAC:Y:RLEV?\n"),const_cast<ViString>("%T"),buff);

    QString error = query_ERRor();

    if(error == "+0,\"No error\"\n")
    {
        return qPrintable(buff);
    }
    else
    {
        return error;
    }
}



void N9000A::write_FREQuency_STARt(int freq, QString mode = "MHz")
{
    viPrintf(vi, const_cast<ViString>("FREQuency:STARt %d %s\n"),freq,qPrintable(mode));
}

QString N9000A::query_FREQuency_STARt()
{
    char buff[1024] = "";
    viQueryf(vi,const_cast<ViString>("FREQuency:STARt?\n"),const_cast<ViString>("%T"),buff);

    QString error = query_ERRor();

    if(error == "+0,\"No error\"\n")
    {
        return qPrintable(buff);
    }
    else
    {
        return error;
    }
}

void N9000A::write_FREQuency_STOP(int freq, QString mode = "MHz")
{
    viPrintf(vi, const_cast<ViString>("FREQuency:STOP %d %s\n"),freq,qPrintable(mode));
}

QString N9000A::query_FREQuency_STOP()
{
    char buff[1024] = "";
    viQueryf(vi,const_cast<ViString>("FREQuency:STOP?\n"),const_cast<ViString>("%T"),buff);

    QString error = query_ERRor();

    if(error == "+0,\"No error\"\n")
    {
        return qPrintable(buff);
    }
    else
    {
        return error;
    }
}


QString N9000A::query_CALC_MARK_Y()
{
    char buff[1024] = "";
    viQueryf(vi,const_cast<ViString>("CALC:MARK:Y?\n"),const_cast<ViString>("%T"),buff);

    QString error = query_ERRor();

    if(error == "+0,\"No error\"\n")
    {
        return qPrintable(buff);
    }
    else
    {
        return error;
    }
}

QString N9000A::query_CALC_MARK_X()
{
    char buff[1024] = "";
    viQueryf(vi,const_cast<ViString>("CALC:MARK:X?\n"),const_cast<ViString>("%T"),buff);

    QString error = query_ERRor();

    if(error == "+0,\"No error\"\n")
    {
        return qPrintable(buff);
    }
    else
    {
        return error;
    }
}
