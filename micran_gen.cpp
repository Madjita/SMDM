#include "micran_gen.h"
#include <windows.h>
#include <QDebug>



//Конструктор
Micran_Gen::Micran_Gen()
    :connected(false)
{
   this->moveToThread(new QThread());

    connect(this->thread(),&QThread::started,this,&Micran_Gen::process_start);

    this->thread()->start();


}

void Micran_Gen::process_start()
{
      vi = NULL;
}



//Деструктор
Micran_Gen::~Micran_Gen()
{
    viClose (vi);
    viClose (defaultRM);
}

//Функция разъединения с устройством
void Micran_Gen::DisConnect()
{
    if(vi != NULL)
        viClose (vi);

    viClose (defaultRM);
}



//Функция Соединения с уйстройством
void Micran_Gen::Connect(QString ip)
{

    viStatus=viOpenDefaultRM(&defaultRM);                                       //Открытие сессии по умолчанию


    QString str  = "TCPIP::"+ip+"::8888::SOCKET::GEN";                         // Создаем строку с заданным ip адрессом для подключения к устрйоству (Генератору)

    // Присваиваем нужному типу ip адрес сформировавшейся строки

    qDebug () << "Micran_Gen viStatus = " << str;
    viStatus=viOpen(defaultRM, ViRsrc(qPrintable(str)), VI_NULL,VI_NULL,&vi); //VI_EXCLUSIVE_LOCK  5000 // Открытие сессии с устройством по заданному ip
    qDebug () << "Micran_Gen Log2";




    emit Log2("Соединение с устройство Г7М-20: ");                                   //Отправка сигнала для лога генератора

     qDebug () << "Micran_Gen";
    if(viStatus<VI_SUCCESS)                                                     // Проверка подключения к устройству
    {


        switch (viStatus) {


        case VI_ERROR_INV_OBJECT:

        {
              qDebug () << "Micran_Gen  ERROR = VI_ERROR_INV_OBJECT";
            break;
        }
        case VI_ERROR_INV_RSRC_NAME:
        {
              qDebug () << "Micran_Gen  ERROR = VI_ERROR_INV_RSRC_NAME";
            break;
        }
        case VI_ERROR_RSRC_NFOUND:
        {
             qDebug () << "Micran_Gen  ERROR = VI_ERROR_RSRC_NFOUND";
            break;
        }
        case VI_ERROR_TMO:
        {
             qDebug () << "Micran_Gen  ERROR = VI_ERROR_TMO";
            break;
        }


        }


        emit Log("===================\n");                                      //Отправка лога в основное окно логирования программы
        emit Log("[Соединение не установленно(ERROR)] \n");                     //Отправка лога в основное окно логирования программы
        emit Log("===================\n");                                      //Отправка лога в основное окно логирования программы
        emit Log2("[Соединение не установленно(ERROR)] \n");                    //Отправка лога в  окно  логирования генератора

        qDebug () << "Micran_Gen  ERROR";
    }
    else
    {
        emit Log("===================\n");                                      //Отправка лога в основное окно логирования программы
        emit Log("[Соединение установленно(ОК)] \n");                           //Отправка лога в основное окно логирования программы
        emit Log("===================\n");                                      //Отправка лога в основное окно логирования программы
        emit Log2("[Соединение установленно(ОК)] \n");                          //Отправка лога в  окно  логирования генератора
        connected = true;                                                       //Соединение установленно

        qDebug () << "Micran_Gen VI_SUCCESS";
    }



}




 QString Micran_Gen::SetPowerAndFreq(int freq,QString mode_Hz_kHz_MHz_GHz,int pow,QString mode_V_DBM)
 {
     QString error;

     write_SOURce_POWer_LEVel(pow,mode_V_DBM);

     error = query_ERRor();

     if(error == "+0,\"No error\"\n")
     {
         write_SOURce_FREQuency(freq,mode_Hz_kHz_MHz_GHz);

         error = query_ERRor();

         if(error == "+0,\"No error\"\n")
         {
             write_INITiate();

             write_OUTPut_STATe(true);
         }
         else
         {
            return error;
         }
     }
     else
     {
        return error;
     }

 }




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



//Эта команда очищает все структуры с информацией о состоянии прибора.
void Micran_Gen::write_CLS()
{
    viPrintf(vi, ViString("*CLS\r\n"));
}


void Micran_Gen::write_INITiate()
{
    viPrintf(vi, ViString("INITiate:CONTinuous ON\r\n"));
}

//Запрос регистра состояния стандартных событий (Standard Event Status Register).Возвращает значение регистра состояния стандартных событий в десятичной  системе счисления .
QString Micran_Gen::query_ESR()
{
    char buff[1024] = "";
    viQueryf(vi,ViString("*ESR?\n"),ViString("%T"),buff);

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

//Возвращает идентифицирующую прибор строку.
/*
 * Возвращает идентифицирующую прибор строку. Строка идентификации состоитиз четырѐх полей разделѐнных запятой:
Поле 1, наименование производителя, «Micran»;
Поле 2, модель устройства, содержитмодель устройства латиницей, может также включатьинформацию о поддерживаемых опциях;
Поле 3, серийный номер устройства;
Поле 4, версия модулей системы.
 * */
QString Micran_Gen::query_IDN()
{
    char buff[1024] = "";
    viQueryf(vi,ViString("*IDN?\n"),ViString("%T"),buff);

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

// Возвращает  «1» при завершении асинхронной операции.
QString Micran_Gen::query_OPC()
{
    char buff[1024] = "";
    viQueryf(vi,ViString("*OPC?\n"),ViString("%T"),buff);

    return qPrintable(buff);
}

/* Возвращает к заводским настройкам. Возвращаетприбор в заранее известную конфигурацию.
 * Описание команд определяет состояние *RST для каждой команды.Команда *RST не влияет на структуры состояния (см. команду *CLS).
 *  P.s.:Ест много памяти часто не использовать
 * */
QString Micran_Gen::query_RST()
{
    char buff[1024] = "";
    viQueryf(vi,ViString("*RST?\n"),ViString("%T"),buff);

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


// Команда возвращает значение байта состояния в десятичной системе счисления.
QString Micran_Gen::query_STB()
{
    char buff[1024] = "";
    viQueryf(vi,ViString("*STB?\n"),ViString("%T"),buff);

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


//Команда подачи сигнала системе запуска (см. TRIGger:SOURce)
QString Micran_Gen::query_TRG()
{
    char buff[1024] = "";
    viQueryf(vi,ViString("*TRG?\n"),ViString("%T"),buff);

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


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Подсисстема SYSTem */
////////////////

//Команда на проверку ошибок в запросе
QString Micran_Gen::query_ERRor()
{
    char buff[1024] = "";
    viQueryf(vi,ViString("SYSTem:ERRor?\n"),ViString("%T"),buff);


   return qPrintable(buff);

}


//Команда возвращает версию SCPI драйвера прибора
QString Micran_Gen::query_VERSion()
{
    char buff[1024] = "";
    viQueryf(vi,ViString("SYSTem:VERSion?\n"),ViString("%T"),buff);

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


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Подсисстема STATus */


////////////////////////////////
/*STATus:OPERation */
////////////////////////////////
//Читает маску битов, отражающихся в регистр событий состояний.
QString Micran_Gen::query_STATus_OPERation_ENABle()
{
    char buff[1024] = "";
    viQueryf(vi,ViString("STATus:OPERation:ENABle?\n"),ViString("%T"),buff);

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

//Читает маску битов, отражающихся в регистр событий состояний.
void Micran_Gen::write_STATus_OPERation_ENABle()
{
    viPrintf(vi, ViString("STATus:OPERation:ENABle\r\n"));
}

/*Возвращает значение регистра событий.
         * Регистр событий отражает значение регистра состояния с учѐтом маски битов.
         * После чтения значения регистра событий его значение сбрасывается в ноль.
        */
QString Micran_Gen::query_STATus_OPERation_EVENt()
{
    char buff[1024] = "";
    viQueryf(vi,ViString("STATus:OPERation:EVENt?\n"),ViString("%T"),buff);

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

//Запрос CONDition?возвращает значение регистров состояния OPERation –целое число в диапазоне от 0 до 32767.
QString Micran_Gen::query_STATus_OPERation_CONDition()
{
    char buff[1024] = "";
    viQueryf(vi,ViString("STATus:OPERation:CONDition?\n"),ViString("%T"),buff);

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
////////////////////////////////
/*STATus:QUEStionable */
////////////////////////////////
//Читает маску битов, отражающихся в регистр событий состояний.
QString Micran_Gen::query_STATus_QUEStionable_ENABle()
{
    char buff[1024] = "";
    viQueryf(vi,ViString("STATus:QUEStionable:ENABle?\n"),ViString("%T"),buff);

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

//Читает маску битов, отражающихся в регистр событий состояний.
void Micran_Gen::write_STATus_QUEStionable_ENABle()
{
    viPrintf(vi, ViString("STATus:QUEStionable:ENABle\r\n"));
}

/*Возвращает значение регистра событий.
         * Регистр событий отражает значение регистра состояния с учѐтом маски битов.
         * После чтения значения регистра событий его значение сбрасывается в ноль.
        */
QString Micran_Gen::query_STATus_QUEStionable_EVENt()
{
    char buff[1024] = "";
    viQueryf(vi,ViString("STATus:QUEStionable:EVENt?\n"),ViString("%T"),buff);

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

//Запрос CONDition?возвращает значение регистров состояния OPERation –целое число в диапазоне от 0 до 32767.
QString Micran_Gen::query_STATus_QUEStionable_CONDition()
{
    char buff[1024] = "";
    viQueryf(vi,ViString("STATus:QUEStionable:CONDition?\n"),ViString("%T"),buff);

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

//Читает маску битов, отражающихся в регистр событий состояний.
void Micran_Gen::write_STATus_PRESet()
{
    viPrintf(vi, ViString("STATus:PRESet\r\n"));
}

//////////////////////////////////////////////////////////////////////////////////////////////
/*Подсистема OUTPut  ON|OFF|1|0 */


//Команда OUTPut:[STATe] осуществляет управление выходом СВЧ. ON–включить выход СВЧ, OFF–выключить выход СВЧ.
void Micran_Gen::write_OUTPut_STATe(bool state)
{
   QString  mode;
   if(state)
   {
       mode = "ON";
   }
   else
   {
        mode = "OFF";
   }

    viPrintf(vi, ViString("OUTPut:STATe %s\r\n"),qPrintable(mode));


}

//Запрос на проверку выхода СВЧ
QString Micran_Gen::query_OUTPut_STATe()
{
    char buff[1024] = "";
    viQueryf(vi,ViString("OUTPut:STATe?\n"),ViString("%T"),buff);

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

//////////////////////////////////////////////////////////////////////////////////////////////
/*Подсистема UNIT */

//Команда установки Выбора единиц задания параметров мощности: W(ватты), DBM(дБм)
void Micran_Gen::write_UNIT_POWer(QString DBMorW = "DMB")
{
    viPrintf(vi, ViString("UNIT:POWer %s\r\n"),qPrintable(DBMorW));
}

//Запрос на проверку установки Выбора единиц задания параметров мощности: W(ватты), DBM(дБм)
QString Micran_Gen::query_UNIT_POWer()
{
    char buff[1024] = "";
    viQueryf(vi,ViString("UNIT:POWer?\n"),ViString("%T"),buff);

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



/////////////////////////////////////////////////////////////////////////////////////////////////////
/*Подсистема SOURce */


//FREQuency - задание параметров частоты

//Команда установки частоты сигнала
void Micran_Gen::write_SOURce_FREQuency(QString FREQuency_MAXimum_MINimum_DEFault = "DEFault")
{
    viPrintf(vi, ViString("SOURce:FREQuency %s\r\n"),qPrintable(FREQuency_MAXimum_MINimum_DEFault));
}

//Команда установки частоты сигнала
void Micran_Gen::write_SOURce_FREQuency(int freq,QString mode_Hz_kHz_MHz_GHz  = "Hz")
{
    viPrintf(vi, ViString("SOURce:FREQuency %d %s\r\n"),freq,qPrintable(mode_Hz_kHz_MHz_GHz));
}


//Запрос на проверку установки частоты сигнала
QString Micran_Gen::query_SOURce_FREQuency()
{
    char buff[1024] = "";
    viQueryf(vi,ViString("SOURce:FREQuency?\n"),ViString("%T"),buff);

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

//Команда задаѐт режим генерации частоты:  CW|FIXed|SWEep|LIST.
void Micran_Gen::write_SOURce_FREQuency_MODE(QString mode_CW_FIXed_SWEep_LIST = "FIXed")
{
    viPrintf(vi, ViString("SOURce:FREQuency:MODE %s\r\n"),qPrintable(mode_CW_FIXed_SWEep_LIST));
}

// Запрос на проверку режима генерации частоты
QString Micran_Gen::query_SOURce_FREQuency_MODE()
{
    char buff[1024] = "";
    viQueryf(vi,ViString("SOURce:FREQuency:MODE?\n"),ViString("%T"),buff);

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

//Команда задаѐт начальная частота
void Micran_Gen::write_SOURce_FREQuency_STARt(int freq)
{
    viPrintf(vi, ViString("SOURce:FREQuency:STARt %d\r\n"),&freq);
}

// Запрос на проверку установленной начальной частоты
QString Micran_Gen::query_SOURce_FREQuency_STARt()
{
    char buff[1024] = "";
    viQueryf(vi,ViString("SOURce:FREQuency:STARt?\n"),ViString("%T"),buff);

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

//Команда задаѐт конечная частота
void Micran_Gen::write_SOURce_FREQuency_STOP(int freq)
{
    viPrintf(vi, ViString("SOURce:FREQuency:STOP %d\r\n"),&freq);
}

// Запрос на проверку установленной конечной частоты
QString Micran_Gen::query_SOURce_FREQuency_STOP()
{
    char buff[1024] = "";
    viQueryf(vi,ViString("SOURce:FREQuency:STOP?\n"),ViString("%T"),buff);

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
//Команда задаѐт центральная частота
void Micran_Gen::write_SOURce_FREQuency_CENTer(int freq)
{
    viPrintf(vi, ViString("SOURce:FREQuency:CENTer %d\r\n"),&freq);
}

// Запрос на проверку установленной центральной частоты
QString Micran_Gen::query_SOURce_FREQuency_CENTer()
{
    char buff[1024] = "";
    viQueryf(vi,ViString("SOURce:FREQuency:CENTer?\n"),ViString("%T"),buff);

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
//Команда задаѐт полосачастот
void Micran_Gen::write_SOURce_FREQuency_SPAN(int freq)
{
    viPrintf(vi, ViString("SOURce:FREQuency:SPAN %d\r\n"),&freq);
}

// Запрос на проверку установленной полосачастот
QString Micran_Gen::query_SOURce_FREQuency_SPAN()
{
    char buff[1024] = "";
    viQueryf(vi,ViString("SOURce:FREQuency:SPAN?\n"),ViString("%T"),buff);

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

//POWer - Подчистема управления мощностью сигнала источника



//Задание уровня мощности
void Micran_Gen::write_SOURce_POWer_LEVel(int pow,QString mode_V_DBM)
{

    viPrintf(vi, ViString("SOURce:POWer:LEVel %d %s\r\n"),pow,qPrintable(mode_V_DBM));
}

QString Micran_Gen::query_SOURce_POWer_LEVel()
{
    char buff[1024] = "";
    viQueryf(vi,ViString("SOURce:POWer:LEVel?\n"),ViString("%T"),buff);

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


//Задание уровня мощности источника.
void Micran_Gen::write_SOURce_POWer_LEVel_IMMediate_AMPLitude(int freq,QString mode_Hz_kHz_MHz_GHz = "Hz")
{
    viPrintf(vi, ViString("SOURce:POWer:LEVel:IMMediate:AMPLitude %d %T\r\n"),&freq,&mode_Hz_kHz_MHz_GHz);
}

QString Micran_Gen::query_SOURce_POWer_LEVel_IMMediate_AMPLitude()
{
    char buff[1024] = "";
    viQueryf(vi,ViString("SOURce:POWer:LEVel:IMMediate:AMPLitude?\n"),ViString("%T"),buff);

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

//Управление автоматической регулировкой мощности источника.
void Micran_Gen::write_SOURce_POWer_ALC(bool mode = true)
{
    viPrintf(vi, ViString("SOURce:POWer:ALC %d\r\n"),&mode);
}

QString Micran_Gen::query_SOURce_POWer_ALC()
{
    char buff[1024] = "";
    viQueryf(vi,ViString("SOURce:POWer:ALC?\n"),ViString("%T"),buff);

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

void Micran_Gen::write_SOURce_POWer_MODE(QString mode_FIXed_SWEep_LIST = "FIXed")
{
    viPrintf(vi, ViString("SOURce:POWer:MODE %T\r\n"),&mode_FIXed_SWEep_LIST);
}

QString Micran_Gen::query_SOURce_POWer_MODE()
{
    char buff[1024] = "";
    viQueryf(vi,ViString("SOURce:POWer:MODE?\n"),ViString("%T"),buff);

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

//Команда задаѐт начальная мощности
void Micran_Gen::write_SOURce_POWer_STARt(int freq)
{
    viPrintf(vi, ViString("SOURce:POWer:STARt %d\r\n"),freq);
}

// Запрос на проверку установленной начальной мощности
QString Micran_Gen::query_SOURce_POWer_STARt()
{
    char buff[1024] = "";
    viQueryf(vi,ViString("SOURce:POWer:STARt?\n"),ViString("%T"),buff);

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

//Команда задаѐт конечная мощности
void Micran_Gen::write_SOURce_POWer_STOP(int freq)
{
    viPrintf(vi, ViString("SOURce:POWer:STOP %d\r\n"),freq);
}

// Запрос на проверку установленной конечной мощности
QString Micran_Gen::query_SOURce_POWer_STOP()
{
    char buff[1024] = "";
    viQueryf(vi,ViString("SOURce:POWer:STOP?\n"),ViString("%T"),buff);

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
//Команда задаѐт центральная мощность
void Micran_Gen::write_SOURce_POWer_CENTer(int freq)
{
    viPrintf(vi, ViString("SOURce:POWer:CENTer %d\r\n"),freq);
}

// Запрос на проверку установленной центральной мощности
QString Micran_Gen::query_SOURce_POWer_CENTer()
{
    char buff[1024] = "";
    viQueryf(vi,ViString("SOURce:POWer:CENTer?\n"),ViString("%T"),buff);

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
//Команда задаѐт полосамощности
void Micran_Gen::write_SOURce_POWer_SPAN(int freq)
{
    viPrintf(vi, ViString("SOURce:POWer:SPAN %d\r\n"),freq);
}

// Запрос на проверку установленной полосамощности
QString Micran_Gen::query_SOURce_POWer_SPAN()
{
    char buff[1024] = "";
    viQueryf(vi,ViString("SOURce:POWer:SPAN?\n"),ViString("%T"),buff);

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

//Команда Подсистема отвечает за управления аттенюатором.
void Micran_Gen::write_SOURce_POWer_ATTenuation(int power)
{
    viPrintf(vi, ViString("SOURce:POWer:ATTenuation %d\r\n"),power);
}

// Запрос на проверку установленной Подсистема отвечает за управления аттенюатором.
QString Micran_Gen::query_SOURce_POWer_ATTenuation()
{
    char buff[1024] = "";
    viQueryf(vi,ViString("SOURce:POWer:ATTenuation?\n"),ViString("%T"),buff);

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

//Команда Подсистема отвечает за управления аттенюатором.
void Micran_Gen::write_SOURce_POWer_ATTenuation_AUTO(bool mode)
{
    viPrintf(vi, ViString("SOURce:POWer:ATTenuation:AUTO %d\r\n"),mode);
}


// Запрос на проверку установленной Подсистема отвечает за управления аттенюатором.
QString Micran_Gen::query_SOURce_POWer_ATTenuation_AUTO()
{
    char buff[1024] = "";
    viQueryf(vi,ViString("SOURce:POWer:ATTenuation:AUTO?\n"),ViString("%T"),buff);

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

//Команда Подсистема отвечает за управления аттенюатором.
void Micran_Gen::write_SOURce_SWEep_DWELl(int time)
{
    viPrintf(vi, ViString("SOURce:SWEep:DWELl %d\r\n"),time);
}


// Запрос на проверку установленной Подсистема отвечает за управления аттенюатором.
QString Micran_Gen::query_SOURce_SWEep_DWELl()
{
    char buff[1024] = "";
    viQueryf(vi,ViString("SOURce:SWEep:DWELl?\n"),ViString("%T"),buff);

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


void Micran_Gen::write_SOURce_SWEep_POINts(int point)
{
    viPrintf(vi, ViString("SOURce:SWEep:POINts %d\r\n"),point);
}



QString Micran_Gen::query_SOURce_SWEep_POINts()
{
    char buff[1024] = "";
    viQueryf(vi,ViString("SOURce:SWEep:POINts?\n"),ViString("%T"),buff);

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




void Micran_Gen::write_SOURce_SWEep_STEP(int step)
{
    viPrintf(vi, ViString("SOURce:SWEep:STEP %d %s\r\n"),step);
}


QString Micran_Gen::query_SOURce_SWEep_STEP()
{
    char buff[1024] = "";
    viQueryf(vi,ViString("SOURce:SWEep:STEP?\n"),ViString("%T"),buff);

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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Micran_Gen::write_SOURce_LIST_DWELl(int time,QString mode = "")
{
    viPrintf(vi, ViString("SOURce:LIST:DWELl %d %s\r\n"),time,qPrintable(mode));
}


QString Micran_Gen::query_SOURce_LIST_DWELl()
{
    char buff[1024] = "";
    viQueryf(vi,ViString("SOURce:LIST:DWELl?\n"),ViString("%T"),buff);

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


void Micran_Gen::write_SOURce_LIST_FREQuency(int freq)
{
    viPrintf(vi, ViString("SOURce:LIST:FREQuency %d \r\n"),freq);
}


QString Micran_Gen::query_SOURce_LIST_FREQuency()
{
    char buff[1024] = "";
    viQueryf(vi,ViString("SOURce:LIST:FREQuency?\n"),ViString("%T"),buff);

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
