#ifndef THREAD_SM_H
#define THREAD_SM_H

#include <visa.h>
#include <micran_gen.h>
#include <n9000a.h>
#include <powersourse.h>
#include <QDebug>
#include "qcustomplot.h"
#include <QAbstractSocket>
#include <QtNetwork>

#include <QObject>

#include <QPixmap>

#include <QSqlQueryModel>
#include "tp_smdm.h"

#include "ui_power.h"
#include "ui_frequency.h"
#include "ui_transfercoefficient.h"
#include "ui_switchingsignalsm.h"

#include <QTextEdit>

class thread_SM  : public QObject
{
    Q_OBJECT

public:
    explicit thread_SM();
    explicit thread_SM(Micran_Gen*  Micran1Get,N9000A*   N9000Get,PowerSourse*  HMP2020Get,TP_SMDM* TP_SMDMGet,QTableWidget* viewGet);
    ~thread_SM();

    void SetUi( Ui::Power* win_powerGet,
                Ui::Frequency* win_frequencyGet,
                Ui::TransferCoefficient* win_transferCoefficientGet,
                Ui::SwitchingSignalSM* win_switchingSignalSMGet);




public:
    QSqlQueryModel* Link;
    QSqlQueryModel* ProverkaList;

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

    bool flag,flagEnd;
    bool flag_6_proverki;
    bool FlagGoodAchH;
    bool FlagGood5;
    bool FlagGood5Eshe;
    bool FlagGood10Mhz;
    bool Start5Proverka;
    bool flagRegylK0;

    double min;
    double max;
    double NeravnACHX;

    QSqlQueryModel *SQL_OneProverka;
    QDateTime date;
    QDateTime dateStart;
    QTextEdit* log_text_Block;

    QUdpSocket* p_udpSocketOut; // UDP soket для Переключения каналов реле Коммутатора СМ16-4

    QMutex m_mutex;

    QSemaphore sem,sem2,sem3;

    bool flagRegyl_A;
    bool flagRegyl_B;

    int releA;
    int releB;

    QVector<bool> ListProv;

    QFile* file;


    void MyClear();

    void StratRegyl();
    void ProverkaRegyl();

    void StartProverka_1_And_2();
    void StartProverka3();
    void StartProverka4();
    void StartProverka5();
    void StartProverka6();
    void StartProverka7();
    void StartProveroka8();
    void END();





    void Ysilenie(int ysilenie,int out);
    void SetIdLink(QSqlQueryModel* LinkGet);
    void SetEtap(QString etapGet);

    void Rele_Kom(int in,int out);
    void Rele_Kom_10MGH(int out);
    void AchH (int in,int out);
    void AchHRegyl();
    void ProverkaAchH(int A);
    void Proverka10MGU();
    void Proverka_5(int,int in,int out);


    bool AChHAddPoint;

    QString log;

    double Pogreshnost;

    QVector<QVector<double>>* ListRegyl;

    int ReleA,ReleB;

    int Rele(int);

    void setListRegyl(QVector<QVector<double>>* ListRegylGet);


public slots:
    void Work();
    void SetProv(int r, int c);
    void Set_log_text(QTextEdit*);
    void process_start();

    void Ypravlenie(int in,int out);
    void perecl(char* a,int index,int out);

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

    void CreateGraphfrenq();

    void SQL_add(QString);

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
    Ui::SwitchingSignalSM* win_switchingSignalSM;


    QTextEdit* log_text;



};

#endif // THREAD_SM_H
