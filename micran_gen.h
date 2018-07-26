#ifndef MICRAN_GEN_H
#define MICRAN_GEN_H

#include <visa.h>

#include <QString>
#include <QObject>
#include <QThread>


class Micran_Gen : public QObject
{
     Q_OBJECT

public:

    ViSession vi;           // Номер сессии, задается прибором
    int viStatus;           // Статус сессии с прибором
    ViRsrc viAddres;        // IP адрес прибора
    bool connected;         // Подключение прибора
    ViSession defaultRM;    // Число для сессии по умолчанию

    Micran_Gen();   //Конструктор Класса генератора
    ~Micran_Gen();  // Диструктор класса генератора







public slots:
      void Connect(QString ip);     // Слот соединения с устройством , принимает ip адресс устройства
      void DisConnect();            // Разъединение связи с у стройством (генератором)


      void process_start();

      //Функция установки Частоты и мощности на генераторе
      QString SetPowerAndFreq(int freq,QString mode_Hz_kHz_MHz_GHz,int pow,QString mode_V_DBM);



      //Основные команды для установки частоты , мощности, включение выключение СВЧ и инициализации
      void write_SOURce_FREQuency_MODE(QString);    //Команда задаѐт режим генерации частоты:  CW|FIXed|SWEep|LIST.
      QString query_SOURce_FREQuency_MODE();        // Запрос на проверку режима генерации частоты
      void write_SOURce_POWer_LEVel(int pow,QString mode_V_DBM = "DBM"); //Установка мощности
      QString query_SOURce_POWer_LEVel();           //Запрос на проверку установленной мощности
      void write_SOURce_FREQuency(QString);         //Задание частоты сигнала источника
      void write_SOURce_FREQuency(int,QString);     //Задание частоты сигнала источника
      QString query_SOURce_FREQuency();                 //Запрос на установленную частоту сигнала источника
      void write_INITiate();
      void write_OUTPut_STATe(bool);                // Команда OUTPut:[STATe] осуществляет управление выходом СВЧ. ON–включить выход СВЧ, OFF–выключить выход СВЧ.
      QString query_OUTPut_STATe();                     //Запрос на проверку выхода СВЧ



      //Команды которые только задают значения
      void write_CLS();     //Эта команда очищает все структуры с информацией о состоянии прибора.
      void write_STATus_OPERation_ENABle();  //Задаѐт  маску битов, отражающихся в регистр событий состояний.
      void write_STATus_QUEStionable_ENABle();  //Задаѐт  маску битов, отражающихся в регистр событий состояний.
      void write_STATus_PRESet();

      void write_UNIT_POWer(QString);               // Выбор единиц задания параметров мощности: W(ватты), DBM(дБм)
      void write_SOURce_FREQuency_STARt(int);
      void write_SOURce_FREQuency_STOP(int);
      void write_SOURce_FREQuency_CENTer(int);
      void write_SOURce_FREQuency_SPAN(int);
      void write_SOURce_POWer_LEVel_IMMediate_AMPLitude(int,QString);       //Задание уровня мощности источника.
      void write_SOURce_POWer_ALC(bool);
      void write_SOURce_POWer_MODE(QString);
      void write_SOURce_POWer_STARt(int);
      void write_SOURce_POWer_STOP(int);
      void write_SOURce_POWer_CENTer(int);
      void write_SOURce_POWer_SPAN(int);
      void write_SOURce_POWer_ATTenuation(int);
      void write_SOURce_POWer_ATTenuation_AUTO(bool);
      void write_SOURce_SWEep_DWELl(int);
      void write_SOURce_SWEep_POINts(int);
      void write_SOURce_SWEep_STEP(int);
      void write_SOURce_LIST_DWELl(int,QString);
      void write_SOURce_LIST_FREQuency(int);


      //Команды которые возвращают значения

      QString query_ESR();      //Запрос регистра состояния стандартных событий (Standard Event Status Register).Возвращает значение регистра состояния стандартных событий в десятичной  системе счисления .
      QString query_IDN();      // Возвращает идентифицирующую прибор строку.
      QString query_OPC();      // Возвращает  «1» при завершении асинхронной операции.
      QString query_RST();      // Возвращает к заводским настройкам. Возвращаетприбор в заранее известную конфигурацию. Описание команд определяет состояние *RST для каждой команды.Команда *RST не влияет на структуры состояния (см. команду *CLS). P.s.:Ест много памяти часто не использовать
      QString query_STB();      // Команда возвращает значение байта состояния в десятичной системе счисления.
      QString query_TRG();      // Команда подачи сигнала системе запуска (см. TRIGger:SOURce)
      QString query_ERRor();    //  Команда проверки на ошибки запроса
      QString query_VERSion();  // Команда для запроса версии используемого SCPI драйвера прибора
      QString query_STATus_OPERation_ENABle();      // Читает маску битов, отражающихся в регистр событий состояний.
      QString query_STATus_OPERation_EVENt();       //Возвращает значение регистра событий.
      QString query_STATus_OPERation_CONDition();       //Запрос CONDition?возвращает значение регистров состояния OPERation –целое число в диапазоне от 0 до 32767.
      QString query_STATus_QUEStionable_ENABle(); // Читает маску битов, отражающихся в регистр событий состояний.
      QString query_STATus_QUEStionable_EVENt();        //Возвращает значение регистра событий.
      QString query_STATus_QUEStionable_CONDition();    //Запрос CONDition?возвращает значение регистров состояния OPERation –целое число в диапазоне от 0 до 32767.
      QString query_UNIT_POWer();                       //Запрос на проверку установки Выбора единиц задания параметров мощности: W(ватты), DBM(дБм)
      QString query_SOURce_FREQuency_STARt();           // Запрос на проверку установленной начальной частоты
      QString query_SOURce_FREQuency_STOP();            // Запрос на проверку установленной конечной частоты
      QString query_SOURce_FREQuency_CENTer();          // Запрос на проверку установленной центральной частоты
      QString query_SOURce_FREQuency_SPAN();            // Запрос на проверку установленной полосачастот
      QString query_SOURce_POWer_LEVel_IMMediate_AMPLitude();   //Запрос на проверку уровня мощности источника
      QString query_SOURce_POWer_ALC();
      QString query_SOURce_POWer_MODE();
      QString query_SOURce_POWer_STARt();           // Запрос на проверку установленной начальной частоты
      QString query_SOURce_POWer_STOP();            // Запрос на проверку установленной конечной частоты
      QString query_SOURce_POWer_CENTer();          // Запрос на проверку установленной центральной частоты
      QString query_SOURce_POWer_SPAN();
      QString query_SOURce_POWer_ATTenuation();
      QString query_SOURce_POWer_ATTenuation_AUTO();
      QString query_SOURce_SWEep_DWELl();
      QString query_SOURce_SWEep_POINts();
      QString query_SOURce_SWEep_STEP();
      QString query_SOURce_LIST_DWELl();
      QString query_SOURce_LIST_FREQuency();


signals:
     void Log(QString);
     void Log2(QString);
     void LogClear();


};


#endif // MICRAN_GEN_H
