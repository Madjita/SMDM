#include "bdata.h"
#include <QTimer>
#include <QTime>

BData::BData()
{

//    this->moveToThread(new QThread());
//    connect(this->thread(),&QThread::started,this,&BData::process_start);
//    this->thread()->start();

}


QString BData::Connect_BD()
{
    if (!QSqlDatabase::drivers().contains("QSQLITE"))
    {
        //Вывести ошибку подключения
        qDebug() << "This demo needs the QODBC driver";
    }
       // QMessageBox::critical(BData::pa, "Unable to load database", "This demo needs the QODBC driver");

    db = QSqlDatabase::addDatabase("QSQLITE");
    QDir::setCurrent(QCoreApplication::applicationDirPath());
    QString pathToDB = QDir::currentPath()+QString("/SMDM_DB.db");
    db.setDatabaseName(pathToDB);
    QFileInfo checkFile(pathToDB);

    if(! db.open())
    {
        qDebug()<<"ERROR: "<<QSqlError( db.lastError()).text();
          Log("ERROR: "+ QSqlError( db.lastError()).text()+"\n");
          Log("===================\n");

         return "false";
    }
    else
    {
         query = new QSqlQuery(db);

            qDebug()<<"Ok";
          Log("База данных загружена [OK]. Рабочая папка:\n");
          Log( db.databaseName()+"\n");
          Log("===================\n");
    }

    QTime midnight(0,0,0);
    qsrand(midnight.secsTo(QTime::currentTime()));

    timer = new QTimer();
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(work()));


    return "true";
}



void BData::StartTimer()
{
    timer->start(300);
}

void BData::process_start()
{

}

void BData::addLink()
{


    QSqlQueryModel *Devi = new QSqlQueryModel();
    QSqlQueryModel *sd = new QSqlQueryModel();
    QSqlQueryModel *sd1 = new QSqlQueryModel();
    QSqlQueryModel *sd2 = new QSqlQueryModel();
    QSqlQueryModel *sd3 = new QSqlQueryModel();

    QSqlQueryModel *lol = new QSqlQueryModel();

    Devi->setQuery("SELECT * FROM Device");

    sd->setQuery("SELECT * FROM DeviceSerial");

    sd1->setQuery("SELECT * FROM Etap");

    sd2->setQuery("SELECT * FROM Proverka");

    sd3->setQuery("SELECT * FROM Conditious");

    qDebug () << "L<DOLFAJFLSASFJIASHJFLIASF";

    qDebug () << "Device: " << sd->rowCount();
    qDebug () << "Etap: " << sd1->rowCount();
    qDebug () << "Proverka: " << sd2->rowCount();
    qDebug () << "Conditious: " << sd3->rowCount();


    bool fgdsg = false;


    for(int i = 0;  i < sd->rowCount();i++)
    {
        for(int j=0;j < sd1->rowCount();j++)
        {
            fgdsg = false;
            for(int k=0; k < sd2->rowCount();k++)
            {
                if((j+1)!=1 && (k+1)==10)
                {
                    break;
                }

                if((k+1) == 6 && (sd->index(i,1).data(Qt::DisplayRole).toString())=="1")
                {
                    lol->setQuery("INSERT INTo Link (IdDevice,IdEtap,IdProverka)VALUES ("+QString::number(i+1)+","+QString::number(j+1)+","+QString::number(k+1)+")");
                    fgdsg=true;
                }
                else
                {
                    if((k+1) == 7 && (sd->index(i,1).data(Qt::DisplayRole).toString())=="2")
                    {
                        lol->setQuery("INSERT INTo Link (IdDevice,IdEtap,IdProverka)VALUES ("+QString::number(i+1)+","+QString::number(j+1)+","+QString::number(k+1)+")");
                        fgdsg=true;
                    }
                    else
                    {
                        if((k+1) == 8 && (sd->index(i,1).data(Qt::DisplayRole).toString())=="3")
                        {
                            lol->setQuery("INSERT INTo Link (IdDevice,IdEtap,IdProverka)VALUES ("+QString::number(i+1)+","+QString::number(j+1)+","+QString::number(k+1)+")");
                            fgdsg=true;
                        }
                        else
                        {
                            if((k+1) < 6)
                            {
                                lol->setQuery("INSERT INTo Link (IdDevice,IdEtap,IdProverka,IdConditious)VALUES ("+QString::number(i+1)+","+QString::number(j+1)+","+QString::number(k+1)+","+QString::number(k+1)+")");
                            }
                            else
                            {
                                if( fgdsg==true && (k+1)>8 )
                                {
                                    lol->setQuery("INSERT INTo Link (IdDevice,IdEtap,IdProverka)VALUES ("+QString::number(i+1)+","+QString::number(j+1)+","+QString::number(k+1)+")");
                                }
                            }
                        }
                    }
                }

            }
        }
    }


    qDebug () << "RND";

}


//Создать таблицу в БД
QString BData::create(QString name, QVector<QString> list)
{
    QString data = "CREATE TABLE "+name+" (";

    for(int i=0;i<list.count();i++)
    {
        data +=list[i];
    }

     data +=")";

    query->exec(data);



    QString error = query->lastError().databaseText();

    if(error != "")
    {
        return error;
    }

    return 0;
}


QSqlQuery BData::zapros(QString data)
{
    if(query == NULL)
    {
        query = new QSqlQuery(db);
    }



    query->exec(data);

    QString error = query->lastError().databaseText();

    if(error != "")
    {
        return *query;
    }


    QSqlQuery q = *query;

    q.first();

    return q;
}



void BData::work()
{
    QSqlQuery q =  zapros("SELECT * FROM GraphPoint");


    emit Log(q.value(1).toString());

    q.next();
    //qDebug() << "END ======================================================";

}


QSqlQueryModel* BData::zaprosQueryModel(QString data)
{
    //Загрузка Этапов

    queryModel = new QSqlQueryModel();

    queryModel->setQuery(data);


    return queryModel;
}
