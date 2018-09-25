#include <log.h>
#include <windows.h>
#include <QDebug>

Log::Log(QObject *parent) : QObject(parent)
{
    //SQL = new QSqlQueryModel();

//    this->moveToThread(new QThread());
//    connect(this->thread(),&QThread::started,this,&Log::process_start);
//    this->thread()->start();

}

void Log::setBD(BData *_BD)
{
    BD = _BD;
}

void Log::process_start()
{

}

void Log::SetDataStart(QDateTime data)
{
    DataStart = data;
}

// Функция Лог приложения
void Log::log_wr_message(QString msg)
{
    BD->zaprosVoid("UPDATE Log SET [Data]='"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"',[Text]='"+msg+"' WHERE DataStart ='"+DataStart.toString("dd.MM.yyyy  hh:mm:ss")+"' AND BlockName='Вывод'");

}

// Функция Лог Источника питания
void Log::log_wr_message_Ist(QString msg)
{
    BD->zaprosVoid("UPDATE Log SET [Data]='"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"',[Text]='"+msg+"' WHERE DataStart ='"+DataStart.toString("dd.MM.yyyy  hh:mm:ss")+"' AND BlockName='Источник питания'");
}

// Функция Лог Генератора
void Log::log_wr_message_Micran(QString msg)
{

    BD->zaprosVoid("UPDATE Log SET [Data]='"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"',[Text]='"+msg+"' WHERE DataStart ='"+DataStart.toString("dd.MM.yyyy  hh:mm:ss")+"' AND BlockName='Микран'");
}


// Функция Лог Пульт
void Log::log_wr_message_TP(QString msg)
{



    BD->zaprosVoid("UPDATE Log SET [Data]='"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"',[Text]='"+msg+"' WHERE DataStart ='"+DataStart.toString("dd.MM.yyyy  hh:mm:ss")+"' AND BlockName='Пульт ТП-СМДМ'");


}

// Функция Лог Блока
void Log::log_wr_message_Block(QString msg)
{

    BD->zaprosVoid("UPDATE Log SET [Data]='"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"',[Text]='"+msg+"' WHERE DataStart ='"+DataStart.toString("dd.MM.yyyy  hh:mm:ss")+"' AND BlockName='Блок'");

}

// Функция Лог Анализатора
void Log::log_wr_message_N9000(QString msg)
{

    BD->zaprosVoid("UPDATE Log SET [Data]='"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"',[Text]='"+msg+"' WHERE DataStart ='"+DataStart.toString("dd.MM.yyyy  hh:mm:ss")+"' AND BlockName='Анализатор'");


}
