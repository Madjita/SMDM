#ifndef LOG_H
#define LOG_H

#include <QObject>
#include <QSqlQuery>
#include <QDateTime>
#include <QSqlQueryModel>

#include <QThread>

class Log : public QObject
{
    Q_OBJECT
public:
    explicit Log(QObject *parent = 0);

    QDateTime DataStart;
    QSqlQueryModel* SQL;
signals:

public slots:
    void log_wr_message(QString msg);       // Функция Лог приложения
    void log_wr_message_Ist(QString msg);   // Функция Лог Источника питания
    void log_wr_message_Micran(QString msg);   // Функция Лог Микрана (генератора)
    void log_wr_message_TP(QString msg);        // Функция Лог Пульта
    void log_wr_message_Block(QString msg);    // Функция Лог Блока
    void log_wr_message_N9000(QString msg);    // Функция Лог Анализатора
    void SetDataStart(QDateTime);
    void process_start();


};

#endif // LOG_H
