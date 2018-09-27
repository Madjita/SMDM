#ifndef THREAD_DM_H
#define THREAD_DM_H



#include <visa.h>
#include <micran_gen.h>
#include <n9000a.h>
#include <powersourse.h>
#include <QDebug>
#include "qcustomplot.h"
#include <QAbstractSocket>
#include <QtNetwork>

#include <QPixmap>


#include <QSqlQueryModel>
#include <tp_smdm.h>

#include "ui_power.h"
#include "ui_frequency.h"
#include "ui_transfercoefficient.h"
#include "ui_switchingsignaldm.h"

class thread_DM : public QObject
{
    Q_OBJECT

public:
    thread_DM();
    explicit thread_DM(Micran_Gen*  Micran1Get,N9000A*   N9000Get,PowerSourse*  HMP2020Get,TP_SMDM* TP_SMDMGet,QTableWidget* viewGet);

    void SetUi( Ui::Power* win_powerGet,
                Ui::Frequency* win_frequencyGet,
                Ui::TransferCoefficient* win_transferCoefficientGet,
                Ui::SwitchingSignalDM* win_switchingSignalDMGet);

public:
    QSqlQueryModel* Link;
    QSqlQueryModel* ProverkaList;
    QTimer *timer;
    QTimer *timer_perestroyka; //таймер для графика перестройки


    QVector<double> x;
    QVector<double> y;
    QVector<int> x1;
    QVector<double> y1;
    QVector<double> ListMin,ListMax,ListNEravn;
    QVector<QVector<double>> ListMin2,ListMax2,ListNEravn2;
    QVector<QVector<double>> ListX;
    QVector<QVector<double>> ListY;
    QVector<double> x10MGh,y10MGh;
    QVector<QVector<double>> ListPerestrouka;
    QVector<QVector<double>> Listy1;
    QVector<double> PerestroykaX;

    QVector<QVector<double>>* ListRegyl;

    bool flag,flagEnd;
    bool flag_6_proverki;
    bool FlagGoodAchH;
    bool FlagGood5;
    bool FlagGood5Eshe;
    bool FlagGood10Mhz;
    bool Start5Proverka;
    bool flagRegylK0;
    bool flagRegyl_A;
    bool flagRegyl_B;

    double min;
    double max;
    double NeravnACHX;

    QSqlQueryModel *SQL_OneProverka;
    QDateTime date;
    QDateTime dateStart;
    QTextEdit* log_text_Block;

    QUdpSocket* p_udpSocketOut; // UDP soket для Переключения каналов реле Коммутатора СМ16-4

    QString log;

    QMutex m_mutex;

    QSemaphore sem,sem2,sem3;

    int ReleA;
    int ReleB;

    int releA;
    int releB;

    QVector<bool> ListProv;

    void MyClear();
    void StartProverka_1_And_2();
    void StartProverka3();
    void StartProverka4();
    void StartProverka5();
    void StartProverka6();
    void StartProverka7();
    void StartProveroka8();
    void END();

    void ProverkaRegyl();
    void StratRegyl();
    void AchHRegyl();

    void Ysilenie(int ysilenie,int out);
    void SetIdLink(QSqlQueryModel* LinkGet);
    void SetEtap(QString etapGet);

    void Rele_Kom(int in,int out);
    void Rele_Kom_10MGH(int out);
    void AchH ();
    void ProverkaAchH(int A);
    void Proverka10MGU();
    void Proverka_5(int);

     void setListRegyl(QVector<QVector<double>>* ListRegylGet);
     int Rele(int);

public slots:
    void Work();
    void process_start();

signals:
    void Log(QString);
    void Log2(QString);
    void StartFrequency(int);
    void StartPower(int);
    void StartPerestroyka(int);
    void LogClear();
    void CreateGraph();
    void SetDataStart(QDateTime);
    void end();

    void updateGraph();
    void updateGraphPerestrouyka();

    void addBDZapros(QString);

public:
    Micran_Gen*     Micran1;
    N9000A*         N9000;
    PowerSourse*    HMP2020;
    TP_SMDM*        TP;

    int  GnFREQuencyStart;
    int  GnFREQuencyEnd;
    int  GnFREQuencyStep;
    int  GnPower;

    QString etap;
    QTableWidget* view;


public:
    Ui::Power* win_power;   // в форме electricalpower имя electricalpower, поэтому и тип описан как electricalpower. Но обязательно нужен #include "ui_electricalpower.h"
    Ui::Frequency* win_frequency;   // в форме electricalpower имя electricalpower, поэтому и тип описан как electricalpower. Но обязательно нужен #include "ui_electricalpower.h"
    Ui::TransferCoefficient* win_transferCoefficient;   // в форме electricalpower имя electricalpower, поэтому и тип описан как electricalpower. Но обязательно нужен #include "ui_electricalpower.h"
    Ui::SwitchingSignalDM* win_switchingSignalDM;

};

#endif // THREAD_DM_H
