#ifndef POWERSOURSE_H
#define POWERSOURSE_H

#include <visa.h>

#include <QString>
#include <QTextEdit>
#include <QApplication>
#include <QObject>

class PowerSourse : public QObject
{
       Q_OBJECT

public:
    ViSession vi;
    int viStatus;
    ViRsrc viAddres;
    bool connected;
    ViSession defaultRM;

    PowerSourse();

    ~PowerSourse();

    void SetFreq();
    void SetPower();
    void Reset();
    void SetSVCh(bool);
    void Kalibrovka();

 public slots:
     void Connect(QString ip);
     void DisConnect();

     void process_start();

     //Команды которые только задают значения
     void write_VOLTage(int v);
     void write_OUTPut(bool);

     //Команды которые возвращают значения
     QString query_VOLTage();
     QString query_ERRor();    //  Команда проверки на ошибки запроса
     QString query_OUTPut();
     QString query_MEASure_CURRent();
     QString query_MEASure_VOLTage();

signals:
    void Log(QString);
    void Log2(QString);
    void LogClear();
};

#endif // POWERSOURSE_H
