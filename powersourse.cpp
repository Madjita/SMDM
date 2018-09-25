#include <powersourse.h>
#include <windows.h>
#include <QDebug>

#include <QThread>

PowerSourse::PowerSourse()
{

    this->moveToThread(new QThread());

    connect(this->thread(),&QThread::started,this,&PowerSourse::process_start);

    this->thread()->start();
}


PowerSourse::~PowerSourse()
{
    viClose (vi);
    viClose (defaultRM);
}

void PowerSourse::DisConnect()
{
     viClose (vi);
     viClose (defaultRM);
}

void PowerSourse::process_start()
{
     connected = false;
}


void PowerSourse::Connect(QString ip)
{
    DisConnect();

    viStatus=viOpenDefaultRM(&defaultRM);

    QString str  = "TCPIP0::"+ip+"::5025::SOCKET";

  //  viAddres = ViRsrc(qPrintable(str));                                         // Присваиваем нужному типу ip адрес сформировавшейся строки

    viStatus=viOpen(defaultRM,  ViRsrc(qPrintable(str)), ViAccessMode(VI_NULL),ViUInt32(VI_NULL),&vi);  // Открытие сессии с устройством по заданному ip

    emit Log2("Соединение с устройством ROHDE&SCHWARZ HMP2020: ");

    if(viStatus<VI_SUCCESS)
    {

        emit Log("===================\n");
        emit Log("[Соединение не установленно(ERROR)] \n");
        emit Log("===================\n");

        emit Log2("[Соединение не установленно(ERROR)] \n");
    }
    else
    {

        emit Log("===================\n");
        emit Log("[Соединение установленно(ОК)] \n");
        emit Log("===================\n");
        emit Log2("[Соединение установленно(ОК)] \n");

        write_OUTPut(false);

        connected = true;
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Команда на проверку ошибок в запросе
QString PowerSourse::query_ERRor()
{
    char buff[1024] = "";
    viQueryf(vi,const_cast<ViString>("SYSTem:ERRor?\n"),const_cast<ViString>("%T"),buff);


   return qPrintable(buff);

}


void PowerSourse::write_VOLTage(int v)
{
    viPrintf(vi, const_cast<ViString>("VOLTage %d V\r\n"),v);
}


void PowerSourse::write_OUTPut(bool mod)
{
    viPrintf(vi, const_cast<ViString>("OUTPut %d\r\n"),mod);
}


QString PowerSourse::query_VOLTage()
{
    char buff[1024] = "";
    viQueryf(vi,const_cast<ViString>("VOLTage?\n"),const_cast<ViString>("%T"),buff);

    QString error = query_ERRor();

    if(error == "0, \"No error\"\n")
    {
        return qPrintable(buff);
    }
    else
    {
        return error;
    }
}

QString PowerSourse::query_OUTPut()
{
    char buff[1024] = "";
    viQueryf(vi,const_cast<ViString>("OUTPut?\n"),const_cast<ViString>("%T"),buff);

    QString error = query_ERRor();

    if(error == "0, \"No error\"\n")
    {
        return qPrintable(buff);
    }
    else
    {
        return error;
    }
}

QString PowerSourse::query_MEASure_CURRent()
{
    char buff[1024] = "";
    viQueryf(vi,const_cast<ViString>("MEASure:CURRent?\n"),const_cast<ViString>("%T"),buff);

    QString error = query_ERRor();

    if(error == "0, \"No error\"\n")
    {
        return qPrintable(buff);
    }
    else
    {
        return error;
    }
}

QString PowerSourse::query_MEASure_VOLTage()
{
    char buff[1024] = "";
    viQueryf(vi,const_cast<ViString>("MEASure:VOLTage?\n"),const_cast<ViString>("%T"),buff);

    QString error = query_ERRor();

    if(error == "0, \"No error\"\n")
    {
        return qPrintable(buff);
    }
    else
    {
        return error;
    }
}
