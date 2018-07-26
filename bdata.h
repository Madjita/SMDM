#ifndef BDATA_H
#define BDATA_H

#include <QString>
#include <QObject>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDir>
#include <QCoreApplication>
#include <QSqlResult>
#include <QTableWidget>
#include <QSqlQueryModel>
#include <QTextEdit>
#include <QThread>

class BData : public QObject
{
    Q_OBJECT

public:
    BData();

    //Таймеры
    QTimer* timer;

    //База данных
    QSqlDatabase db;

    //Запрос к базе
    QSqlQuery* query;
    QSqlQueryModel* queryModel;


public slots:

    QString Connect_BD();
    QString create(QString,QVector<QString>);
    QSqlQuery zapros(QString);

    QSqlQueryModel* zaprosQueryModel(QString);
    void work();

    void StartTimer();

    void process_start();

    void addLink();



signals:
    QString str(QString);
    QString Log(QString);
    void Log2(QString);
    void LogClear();
};

#endif // BDATA_H
