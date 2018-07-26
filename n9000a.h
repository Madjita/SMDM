#ifndef N9000A_H
#define N9000A_H

#include <visa.h>


#include <QString>
#include <QTextEdit>
#include <QApplication>
#include <QObject>


class N9000A : public QObject
{
     Q_OBJECT

public:

    ViSession vi;
    int viStatus;
    ViRsrc viAddres;
    bool connected;
    ViSession defaultRM;


    N9000A();


    ~N9000A();




public slots:
    void Connect(QString ip);
    void DisConnect();

    void process_start();

    //Команды которые только задают значения
    void write_AVER_STAT(bool mod);
    void write_CALC_MARK_MODE(QString mode = "NORMAL");
    void write_DISP_WIND_TRAC_Y_RLEV(int);
    void write_FREQuency_STARt(int, QString);
    void write_FREQuency_STOP(int, QString);

    //Команды которые возвращают значения
    QString query_ERRor();    //  Команда проверки на ошибки запроса
    QString query_IDN();
    QString query_AVER_STAT();
    QString query_CALC_MARK_MODE();
    QString query_DISP_WIND_TRAC_Y_RLEV();
    QString query_FREQuency_STARt();
    QString query_FREQuency_STOP();
    QString query_CALC_MARK_Y();
    QString query_CALC_MARK_X();

 signals:
    void Log(QString);
    void Log2(QString);
    void LogClear();

};
//--------------------------


#endif // N9000A_H
