#ifndef TP_SMDM_H
#define TP_SMDM_H


#include <QString>
#include <QTextEdit>
#include <QApplication>
#include <QThread>

#include <QtNetwork>
#include <QObject>
#include <QSqlQueryModel>

//Библиотеки устройств
#include "micran_gen.h"             //Класс Генератора Micran
#include "n9000a.h"                 //Класс Анализатора N9000


#include "ui_frequency.h"
#include "ui_transfercoefficient.h"

#include <Windows.h>
#include <QRegExpValidator>


class TP_SMDM : public QObject
{
    Q_OBJECT

public:
    TP_SMDM();
    ~TP_SMDM();

    QString ipTP;
    QString ipComputer;
    QTextEdit* log_text_TP; //Лог пульта
    //Ui::Form* win_pyItimage;

    bool flag_6_proverki;

     //new
    QString IPaddrPLATA  = "10.12.1.66";
    QString lastIP = "111.111.111.111";
    char myIPchar[4] = {0, 0, 0, 0};
    char selection[4] = {0, 0, 0, 0};
    int noupint[3] = {0, 222, 173};
    char noup[3] = {0, 0, 0};
    bool SetIPdone = false;
    QStringList list;
    QString currentIPinComboBox;
    QString newIP="";

    QStringList list_k1;
    QStringList list_k2;
    QStringList list_k3;
    QStringList list_k4;

     int rele_seichas;
    //============================================


    bool flag,flagEnd;
    bool FlagGoodAchH;
    bool FlagGood5;
    bool FlagGood5Eshe;
    bool FlagGood10Mhz;
    bool Start5Proverka;
    bool flagRegylK0;

    QFile* file;
    QUdpSocket *p_udpSocketOut;
    QSqlQueryModel *SQL_OneProverka;

    QDateTime date;
    QDateTime dateStart;

    QMutex m_mutex;

    QSemaphore sem,sem2,sem3;

    int  GnFREQuencyStart;
    int  GnFREQuencyEnd;
    int  GnFREQuencyStep;
    int  GnPower;

    double min;
    double max;
    double NeravnACHX;

    bool flagRegyl_A;
    bool flagRegyl_B;

    int releA;
    int releB;

    QVector<QVector<double>>* ListRegyl;

    QVector<double> x;
    QVector<double> y;
    QVector<double> x1;
    QVector<double> y1;
    QVector<double> ListMin,ListMax,ListNEravn;

    QVector<QVector<double>> ListMin2,ListMax2,ListNEravn2;

    QVector<QVector<double>> ListX;
    QVector<QVector<double>> ListY;
    QVector<double> x10MGh,y10MGh;
    QVector<QVector<double>> ListPerestrouka;
    QVector<QVector<double>> Listy1;
    QVector<double> PerestroykaX;


    QString log;

    QString myIP = "";

    QUdpSocket *udpsocket20;
    QUdpSocket *udpsocket21;

    QUdpSocket* udpsocket; // UDP soket Для Переключения реле Пульта "A1"выход по "A9"
    QUdpSocket* udpsocket1; // UDP soket Для чтения информации с сети от пульта по A выходу (Пульт -> коммутатор)
    QUdpSocket *udpsocket2; // UDP soket Для Переключения реле Пульта "A10"выхода по "A18"


    QUdpSocket* udpsocket3;
    QUdpSocket* udpsocket4; // UDP soket Для чтения информации с сети от пульта по B выходу (Пульт <- коммутатор)
    QUdpSocket* udpsocket5; // UDP soket Для Переключения реле Пульта "В1"выход до B"9"
    QUdpSocket* udpsocket6; // UDP soket Для Переключения реле Пульта B"10" выхода до B"18"
    QUdpSocket* udpsocket7;
    QUdpSocket* udpsocket8; // UDP soket Для Переключения реле Пульта A0 соединить с A выход
    QUdpSocket* udpsocket9; // UDP soket Для Переключения реле Пульта B0 соединить с B выход

     //new
    QUdpSocket* udpsocket65533; //Сокет для поиска пульта и Реле K1
    QUdpSocket* udpsocket65510; //Сокет для чтения Ip пульта
    QUdpSocket* udpsocket65500;
    QUdpSocket* udpsocket65400;


    QUdpSocket *udpsocket65531; // Реле K2
    QUdpSocket *udpsocket65526; // Реле K3
    QUdpSocket *udpsocket65523; // Реле K4

    QUdpSocket *udpsocket65521;
    QUdpSocket *udpsocket65520;


    QUdpSocket* udpsocket65401;
    QUdpSocket* udpsocket65511;
     //============================================




    char a[10],b[10]; // Массиф реле каждый элимент означает одно реле в пульте. Например 10 реле {1,0,0,1,0,0,0,0,0,0} означает что включен 1 реле и 4 реле


    void DisConnectToSoket();
    void SetTp();
    bool ReleA(int A);
    bool ReleB(int A);

    bool ReleB(char a[10],int rele);

    void AchHRegyl();
    void AchH();


    void END();
    void StratRegyl();

    void SetUi( Ui::Frequency* win_frequencyGet,
               Ui::TransferCoefficient* win_transferCoefficientGet,Micran_Gen*  Micran1Get,N9000A*   N9000Get);

    Micran_Gen*  Micran1;
    N9000A*   N9000;

public slots:
    void process_start();

    bool readDatagramK1();
    bool readDatagramK2();
    bool readDatagramK3();
    bool readDatagramK4();
    bool readDatagramNaZapros(QUdpSocket* udp);
    void Connect();

    void Kolibrovka();

    //new
    void zapros_readip();
    void readIP();
    void setIP(QString);
    void selection_pult();
    void selection_pult_ok();
    void miganie();
    void dHCP();
    void current_IP(int id);
    void IPvFILE();
    void IPfromFILE();
    void osvobodit_pult();

    void rele_portON(QStringList, int);
    void zaprosSOST_rele(int rele);
    void readDatagram_rele(int);
    void generatorON(int);
    void analizatorON(int);
     //============================================
signals:
    void Log(QString);
    void Log2(QString);
    void SetImage(QByteArray);
    void SetImage2(QByteArray);
    void SetImage3(QByteArray);
    void SetImage4(QByteArray);
    void LogClear();
    void StartPower(int);

public:
    Ui::Frequency* win_frequency;   // в форме electricalpower имя electricalpower, поэтому и тип описан как electricalpower. Но обязательно нужен #include "ui_electricalpower.h"
    Ui::TransferCoefficient* win_transferCoefficient;   // в форме electricalpower имя electricalpower, поэтому и тип описан как electricalpower. Но обязательно нужен #include "ui_electricalpower.h"


};

#endif // TP_SMDM_H
