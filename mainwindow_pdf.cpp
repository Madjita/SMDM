#include <mainwindow.h>
#include "ui_mainwindow.h"


void MainWindow::CreatePDF()
{
    //Коннекты для кода результатов
    QObject::connect(ui->pushButton_GoBack,SIGNAL(clicked(bool)), this, SLOT(GoBack()),Qt::DirectConnection);
    QObject::connect(ui->pushButton_GoNext,SIGNAL(clicked(bool)), this, SLOT(GoNext()),Qt::DirectConnection);
    QObject::connect(ui->pushButton_GoBack,SIGNAL(clicked(bool)), this, SLOT(GoBack()),Qt::DirectConnection);
    QObject::connect(ui->pushButton_GoBack_2,SIGNAL(clicked(bool)), this, SLOT(GoBack2()),Qt::DirectConnection);

    //Инициализация кнопки для PDF
    Button_PDF = new QPushButton("PDF");
    Button_PDF->setToolTip(tr("Сгенерировать отчет PDF")); // задание всплывающей подсказки


    ui->mainToolBar->addWidget(Button_PDF);

}

QVector<QSqlQuery> MainWindow::CreateVectorResultPDF_Regylirovka()
{
    int count_td =0; //колонки
    int count_tr =0; //строчки

    //Лист результатов
    QVector<QSqlQuery> List;

    //Запрос на количество Этапов для формирования колонок
    QSqlQuery sql;

    //Запрос на получение количества строчек
    QSqlQuery sql3;

    //Запрос на получение данных
    QSqlQuery sql_PSI;


    sql = BD->zapros("SELECT DISTINCT Etap.Etaps"
                     " FROM Etap"
                     " WHERE Etap.[Регулировка] = '1'");

    do
    {
        count_td++;

    }while(sql.next() != NULL);


    //Запрос на получение количества строчек
    sql3 = BD->zapros("SELECT DISTINCT [Имя проверки],[Описание]"
                      " FROM Proverka"
                      " JOIN Device ON Device.IdDevice=DeviceSerial.IdDevice"
                      " JOIN DeviceSerial ON Link.IdDevice == DeviceSerial.IdSerial"
                      " JOIN LInk ON Link.IdEtap == Etap.IdEtap"
                      " JOIN Etap ON Link.IdProverka == Proverka.IdProverka"
                      " JOIN Result ON Link.IdLink == Result.IdLink"
                      " Where Device.[Название устройства] = '"+EtapGroup_Device->currentText()+"'"
                                                                                                " AND DeviceSerial.[Серийный номер] = '"+NDevice_ComboBox->currentText()+"'");


    QString SELECT ;





    do
    {

        SELECT += sql3.value(0).toString()+",";
        count_tr++;
    }while(sql3.next() !=NULL);

    SELECT.remove(SELECT.count()-1,1);

    //Формирование запроса для ПСИ
    QString PSI;


    sql.first();

    QString lol ;

    for(int i=0;i < count_tr;i++)
    {
        PSI.clear();

        PSI += "SELECT DISTINCT ";

        PSI +=SELECT;

        PSI +=        " FROM Proverka"
                      " JOIN Device ON Device.IdDevice=DeviceSerial.IdDevice"
                      " JOIN DeviceSerial ON Link.IdDevice == DeviceSerial.IdSerial"
                      " JOIN LInk ON Link.IdEtap == Etap.IdEtap"
                      " JOIN Etap ON Link.IdProverka == Proverka.IdProverka"
                      " JOIN Result ON Link.IdLink == Result.IdLink"
                      " Where Device.[Название устройства] = '"+EtapGroup_Device->currentText()+"'"
                                                                                                " AND DeviceSerial.[Серийный номер] = '"+NDevice_ComboBox->currentText()+"'"
                                                                                                                                                                         " AND Result.[Этап] = 'Регулировка'";

        //  qDebug () << sql->value(0).toString();

        lol = sql.value(0).toString();

        PSI += " AND Etap.Etaps = '"+sql.value(0).toString()+"'";

        sql_PSI = BD->zapros(PSI);


        List.append(sql_PSI);

        sql.next();
    }

    return List;

}

QVector<QSqlQuery> MainWindow::CreateVectorResultPDF()
{
    int count_td =0; //колонки
    int count_tr =0; //строчки

    //Лист результатов
    QVector<QSqlQuery> List;

    //Запрос на количество Этапов для формирования колонок
    QSqlQuery sql;

    //Запрос на получение количества строчек
    QSqlQuery sql3;

    //Запрос на получение данных
    QSqlQuery sql_PSI;


    sql = BD->zapros("SELECT DISTINCT Etap.Etaps"
                     " FROM Etap"
                     " WHERE"
                     " Etap.[ПСИ] = '1'");

    do
    {
        count_td++;

    } while(sql.next() != NULL);


    //Запрос на получение количества строчек
    sql3 = BD->zapros("SELECT DISTINCT [Имя проверки],[Описание]"
                      " FROM Proverka"
                      " JOIN Device ON Device.IdDevice=DeviceSerial.IdDevice"
                      " JOIN DeviceSerial ON Link.IdDevice == DeviceSerial.IdSerial"
                      " JOIN LInk ON Link.IdEtap == Etap.IdEtap"
                      " JOIN Etap ON Link.IdProverka == Proverka.IdProverka"
                      " JOIN Result ON Link.IdLink == Result.IdLink"
                      " Where Device.[Название устройства] = '"+EtapGroup_Device->currentText()+"'"
                                                                                                " AND DeviceSerial.[Серийный номер] = '"+NDevice_ComboBox->currentText()+"'");


    QString SELECT ;

    list_id_Proverki_Find.clear();


    do
    {
        list_id_Proverki_Find.append(sql3.value(1).toString());

        SELECT += sql3.value(0).toString()+",";
        count_tr++;
    } while(sql3.next() !=NULL);

    SELECT.remove(SELECT.count()-1,1);

    //Формирование запроса для ПСИ
    QString PSI;


    sql.first();

    for(int i=0;i < count_tr;i++)
    {
        PSI.clear();

        PSI += "SELECT DISTINCT ";

        PSI +=SELECT;

        PSI +=        " FROM Proverka"
                      " JOIN Device ON Device.IdDevice=DeviceSerial.IdDevice"
                      " JOIN DeviceSerial ON Link.IdDevice == DeviceSerial.IdSerial"
                      " JOIN LInk ON Link.IdEtap == Etap.IdEtap"
                      " JOIN Etap ON Link.IdProverka == Proverka.IdProverka"
                      " JOIN Result ON Link.IdLink == Result.IdLink"
                      " Where Device.[Название устройства] = '"+EtapGroup_Device->currentText()+"'"
                                                                                                " AND DeviceSerial.[Серийный номер] = '"+NDevice_ComboBox->currentText()+"'"
                                                                                                                                                                         " AND Result.[Этап] = 'ПСИ'";

        qDebug () << sql.value(0).toString().toLatin1();

        PSI += " AND Etap.Etaps = '"+sql.value(0).toString()+"'";



        PSI +=  " AND Result.Date = (SELECT MAX(Result.Date)"
                " FROM Proverka"
                " JOIN Device ON Device.IdDevice=DeviceSerial.IdDevice"
                " JOIN DeviceSerial ON Link.IdDevice == DeviceSerial.IdSerial"
                " JOIN LInk ON Link.IdEtap == Etap.IdEtap"
                " JOIN Etap ON Link.IdProverka == Proverka.IdProverka"
                " JOIN Result ON Link.IdLink == Result.IdLink"
                " Where Device.[Название устройства] = '"+EtapGroup_Device->currentText()+"'"
                                                                                          " AND DeviceSerial.[Серийный номер] = '"+NDevice_ComboBox->currentText()+"'"
                                                                                                                                                                   " AND Result.[Этап] = 'ПСИ'"
                                                                                                                                                                   " AND Etap.Etaps = '"+sql.value(0).toString()+"')";





        sql_PSI = BD->zapros(PSI);

        qDebug () << SELECT;



        List.append(sql_PSI);

        sql.next();
    }







    return List;

}
QVector<double> MainWindow::CreateVectorResultPDF_PSI_AchH()
{
    int count_td =0; //колонки
    int count_tr =0; //строчки

    //Лист результатов
    QVector<double> List;

    //Запрос на количество Этапов для формирования колонок
    QSqlQuery sql;

    //Запрос на получение количества строчек
    QSqlQuery sql3;

    //Запрос на получение данных
    QSqlQuery sql_PSI;


    sql = BD->zapros("SELECT DISTINCT Etap.Etaps"
                     " FROM Etap"
                     " WHERE"
                     " Etap.[ПСИ] = '1'");

    do
    {
        count_td++;
    }while(sql.next() != NULL);


    //Запрос на получение количества строчек
    sql3 = BD->zapros("SELECT DISTINCT [Имя проверки],[Описание]"
                      " FROM Proverka"
                      " JOIN Device ON Device.IdDevice=DeviceSerial.IdDevice"
                      " JOIN DeviceSerial ON Link.IdDevice == DeviceSerial.IdSerial"
                      " JOIN LInk ON Link.IdEtap == Etap.IdEtap"
                      " JOIN Etap ON Link.IdProverka == Proverka.IdProverka"
                      " JOIN Result ON Link.IdLink == Result.IdLink"
                      " Where Device.[Название устройства] = '"+EtapGroup_Device->currentText()+"'"
                                                                                                " AND DeviceSerial.[Серийный номер] = '"+NDevice_ComboBox->currentText()+"'");


    QString SELECT ;

    do
    {
        SELECT += sql3.value(0).toString()+",";
        count_tr++;

    }while(sql3.next() !=NULL);

    SELECT.remove(SELECT.count()-1,1);

    //Формирование запроса для ПСИ
    QString PSI;


    sql.first();
    sql3.first();

    QVector<double> list; //Вектор для усреднения

    double sredneeNeravnomernost =0;

    for(int i=0;i < count_tr;i++)
    {
        sredneeNeravnomernost =0;
        list.clear();

        for(int j=1;j < 65;j++)
        {
            PSI.clear();

            // Проверка на второй элимент Power или нет?


            PSI += "SELECT ";

            PSI +=" MAX([Неравномерность])";

            PSI +=        " FROM Proverka"
                          " JOIN Device ON Device.IdDevice=DeviceSerial.IdDevice"
                          " JOIN DeviceSerial ON Link.IdDevice == DeviceSerial.IdSerial"
                          " JOIN LInk ON Link.IdEtap == Etap.IdEtap"
                          " JOIN Etap ON Link.IdProverka == Proverka.IdProverka"
                          " JOIN Result ON Link.IdLink == Result.IdLink"
                          " JOIN GraphPoint ON Link.IdLink == GraphPoint.IdResult"
                          " Where Device.[Название устройства] = '"+EtapGroup_Device->currentText()+"'"
                                                                                                    " AND DeviceSerial.[Серийный номер] = '"+NDevice_ComboBox->currentText()+"'"
                                                                                                                                                                             " AND Result.[Этап] = 'ПСИ'"
                                                                                                                                                                             " AND Proverka.[Имя проверки] = 'Frequency'"
                                                                                                                                                                             " AND GraphPoint.NumberGraph = '"+QString::number(j)+"'";


            PSI += " AND Etap.Etaps = '"+sql.value(0).toString()+"'";

            PSI += " AND GraphPoint.DataProverki = (SELECT MAX(Result.Date)"
                   " FROM Proverka"
                   " JOIN Device ON Device.IdDevice=DeviceSerial.IdDevice"
                   " JOIN DeviceSerial ON Link.IdDevice == DeviceSerial.IdSerial"
                   " JOIN LInk ON Link.IdEtap == Etap.IdEtap"
                   " JOIN Etap ON Link.IdProverka == Proverka.IdProverka"
                   " JOIN Result ON Link.IdLink == Result.IdLink"
                   " Where Device.[Название устройства] = '"+EtapGroup_Device->currentText()+"'"
                                                                                             " AND DeviceSerial.[Серийный номер] = '"+NDevice_ComboBox->currentText()+"'"
                                                                                                                                                                      " AND Result.[Этап] = 'ПСИ'"
                                                                                                                                                                      " AND Etap.[Etaps] = '"+sql.value(0).toString()+"')"
                                                                                                                                                                                                                      " AND Result.Date = (SELECT MAX(Result.Date)"
                                                                                                                                                                                                                      " FROM Proverka"
                                                                                                                                                                                                                      " JOIN Device ON Device.IdDevice=DeviceSerial.IdDevice"
                                                                                                                                                                                                                      " JOIN DeviceSerial ON Link.IdDevice == DeviceSerial.IdSerial"
                                                                                                                                                                                                                      " JOIN LInk ON Link.IdEtap == Etap.IdEtap"
                                                                                                                                                                                                                      " JOIN Etap ON Link.IdProverka == Proverka.IdProverka"
                                                                                                                                                                                                                      " JOIN Result ON Link.IdLink == Result.IdLink"
                                                                                                                                                                                                                      " Where Device.[Название устройства] = '"+EtapGroup_Device->currentText()+"'"
                                                                                                                                                                                                                                                                                                " AND DeviceSerial.[Серийный номер] = '"+NDevice_ComboBox->currentText()+"'"
                                                                                                                                                                                                                                                                                                                                                                         " AND Result.[Этап] = 'ПСИ'"
                                                                                                                                                                                                                                                                                                                                                                         " AND Etap.[Etaps] = '"+sql.value(0).toString()+"')";

            sql_PSI = BD->zapros(PSI);

            list.append(sql_PSI.value(0).toDouble());

            sredneeNeravnomernost += sql_PSI.value(0).toDouble();

        }



        List.append(sredneeNeravnomernost/list.count());

        sql.next();


    }

    return List;
}

QList<QStringList> MainWindow::sql_listEtap(QString blokName, QString serial)
{

    QString mod;
    QString etap;

    int count_td =0; //колонки
    int count_tr =0; //строчки

    //Лист результатов
    QVector<QSqlQuery> List;

    //Запрос на количество Этапов для формирования колонок
    QSqlQuery sql;

    //Запрос на получение количества строчек
    QSqlQuery sql3;

    //Запрос на получение данных
    QSqlQuery sql_PSI;


    sql = BD->zapros("SELECT DISTINCT Etap.Etaps"
                     " FROM Etap"
                     " WHERE"
                     " Etap.[Регулировка] = '1'");

    list_etaps.clear();
    list_data.clear();

    do
    {
        list_etaps.append(sql.value(0).toString());
        count_td++;
    } while(sql.next() != NULL);


    //Запрос на получение количества строчек
    sql3 = BD->zapros("SELECT DISTINCT [Имя проверки],[Описание]"
                      " FROM Proverka"
                      " JOIN Device ON Device.IdDevice=DeviceSerial.IdDevice"
                      " JOIN DeviceSerial ON Link.IdDevice == DeviceSerial.IdSerial"
                      " JOIN LInk ON Link.IdEtap == Etap.IdEtap"
                      " JOIN Etap ON Link.IdProverka == Proverka.IdProverka"
                      " JOIN Result ON Link.IdLink == Result.IdLink"
                      " Where Device.[Название устройства] = '"+EtapGroup_Device->currentText()+"'"
                                                                                                " AND DeviceSerial.[Серийный номер] = '"+NDevice_ComboBox->currentText()+"'");


    QString SELECT ;

    list_id_Proverki_Find.clear();
    list_Proverki_Find.clear();


    do
    {
        list_id_Proverki_Find.append(sql3.value(1).toString());
        list_Proverki_Find.append(sql3.value(0).toString());

        count_tr++;

    } while(sql3.next() !=NULL);

    QString sqlStr = "";

    QList<QStringList> result;


    for(int i=0; i < list_etaps.count();i++)
    {
        for(int j=0; j < list_Proverki_Find.count();j++)
        {

            qDebug () << list_Proverki_Find.value(j);

            sqlStr += "SELECT  "+list_Proverki_Find.value(j)+",Proverka.[Имя проверки],Result.Date"
                                                             " FROM Proverka"
                                                             " JOIN Device ON Device.IdDevice == DeviceSerial.IdDevice"
                                                             " JOIN DeviceSerial ON Link.IdDevice == DeviceSerial.IdSerial"
                                                             " JOIN LInk ON Link.IdEtap == Etap.IdEtap"
                                                             " JOIN Etap ON Link.IdProverka == Proverka.IdProverka"
                                                             " JOIN Result ON Link.IdLink == Result.IdLink"
                                                             " Where Device.[Название устройства] = '" + blokName+"' "
                                                                                                                  " AND DeviceSerial.[Серийный номер] = '" + serial+"' "
                                                                                                                                                                    " AND Result.[Этап] = 'Регулировка'"
                                                                                                                                                                    " AND Etap.Etaps = '" + list_etaps.value(i)+"' "
                                                                                                                                                                                                                " AND Result.Date = (SELECT MAX(Result.Date)"
                                                                                                                                                                                                                " FROM Proverka"
                                                                                                                                                                                                                " JOIN Device ON Device.IdDevice=DeviceSerial.IdDevice"
                                                                                                                                                                                                                " JOIN DeviceSerial ON Link.IdDevice == DeviceSerial.IdSerial"
                                                                                                                                                                                                                " JOIN LInk ON Link.IdEtap == Etap.IdEtap"
                                                                                                                                                                                                                " JOIN Etap ON Link.IdProverka == Proverka.IdProverka"
                                                                                                                                                                                                                " JOIN Result ON Link.IdLink == Result.IdLink"
                                                                                                                                                                                                                " Where Device.[Название устройства] = '" + blokName+"' "
                                                                                                                                                                                                                                                                     " AND DeviceSerial.[Серийный номер] = '" + serial+"' "
                                                                                                                                                                                                                                                                                                                       " AND Result.[Этап] = 'Регулировка'"
                                                                                                                                                                                                                                                                                                                       " AND Etap.Etaps = '" + list_etaps.value(i)+"' )"
                                                                                                                                                                                                                                                                                                                                                                   " AND "+list_Proverki_Find.value(j)+" IS NOT NULL ";

            if(j!=list_Proverki_Find.count()-1)
            {
                sqlStr += " UNION ALL ";
            }


            qDebug() << sqlStr;





        }

        sql = BD->zapros(sqlStr);

        QStringList list;
        list_data.append(sql.value(2).toString());

        do
        {
            list.append(sql.value(0).toString());


        }while(sql.next() != NULL);

        result.append(list);

        sqlStr.clear();

    }


    ///////////////////////////////
    //Регулировка

    sql = BD->zapros("SELECT DISTINCT Etap.Etaps"
                     " FROM Etap"
                     " WHERE"
                     " Etap.[ПСИ] = '1'");

    list_etaps.clear();

    do
    {
        list_etaps.append(sql.value(0).toString());
    } while(sql.next() != NULL);


    sqlStr = "";

    for(int i=0; i < list_etaps.count();i++)
    {
        for(int j=0; j < list_Proverki_Find.count();j++)
        {

            qDebug () << list_Proverki_Find.value(j);

            sqlStr += "SELECT  "+list_Proverki_Find.value(j)+",Proverka.[Имя проверки],Result.Date"
                                                             " FROM Proverka"
                                                             " JOIN Device ON Device.IdDevice == DeviceSerial.IdDevice"
                                                             " JOIN DeviceSerial ON Link.IdDevice == DeviceSerial.IdSerial"
                                                             " JOIN LInk ON Link.IdEtap == Etap.IdEtap"
                                                             " JOIN Etap ON Link.IdProverka == Proverka.IdProverka"
                                                             " JOIN Result ON Link.IdLink == Result.IdLink"
                                                             " Where Device.[Название устройства] = '" + blokName+"' "
                                                                                                                  " AND DeviceSerial.[Серийный номер] = '" + serial+"' "
                                                                                                                                                                    " AND Result.[Этап] = 'ПСИ'"
                                                                                                                                                                    " AND Etap.Etaps = '" + list_etaps.value(i)+"' "
                                                                                                                                                                                                                " AND Result.Date = (SELECT MAX(Result.Date)"
                                                                                                                                                                                                                " FROM Proverka"
                                                                                                                                                                                                                " JOIN Device ON Device.IdDevice=DeviceSerial.IdDevice"
                                                                                                                                                                                                                " JOIN DeviceSerial ON Link.IdDevice == DeviceSerial.IdSerial"
                                                                                                                                                                                                                " JOIN LInk ON Link.IdEtap == Etap.IdEtap"
                                                                                                                                                                                                                " JOIN Etap ON Link.IdProverka == Proverka.IdProverka"
                                                                                                                                                                                                                " JOIN Result ON Link.IdLink == Result.IdLink"
                                                                                                                                                                                                                " Where Device.[Название устройства] = '" + blokName+"' "
                                                                                                                                                                                                                                                                     " AND DeviceSerial.[Серийный номер] = '" + serial+"' "
                                                                                                                                                                                                                                                                                                                       " AND Result.[Этап] = 'ПСИ'"
                                                                                                                                                                                                                                                                                                                       " AND Etap.Etaps = '" + list_etaps.value(i)+"' )"
                                                                                                                                                                                                                                                                                                                                                                   " AND "+list_Proverki_Find.value(j)+" IS NOT NULL ";

            if(j!=list_Proverki_Find.count()-1)
            {
                sqlStr += " UNION ALL ";
            }


            qDebug() << sqlStr;





        }

        sql = BD->zapros(sqlStr);




        QStringList list;
        list_data.append(sql.value(2).toString());

        do
        {
            list.append(sql.value(0).toString());


        }while(sql.next() != NULL);

        result.append(list);

        sqlStr.clear();

    }

    return result;

}

QVector<double> MainWindow::CreateVectorResultPDF_Regylirovka_AchH()
{
    int count_td =0; //колонки
    int count_tr =0; //строчки

    //Лист результатов
    QVector<double> List;

    //Запрос на количество Этапов для формирования колонок
    QSqlQuery sql;

    //Запрос на получение количества строчек
    QSqlQuery sql3;

    //Запрос на получение данных
    QSqlQuery sql_PSI;


    sql = BD->zapros("SELECT DISTINCT Etap.Etaps"
                     " FROM Etap"
                     " WHERE"
                     " Etap.[Регулировка] = '1'");

    do
    {
        count_td++;
    }while(sql.next() != NULL);


    //Запрос на получение количества строчек
    sql3 = BD->zapros("SELECT DISTINCT [Имя проверки],[Описание]"
                      " FROM Proverka"
                      " JOIN Device ON Device.IdDevice=DeviceSerial.IdDevice"
                      " JOIN DeviceSerial ON Link.IdDevice == DeviceSerial.IdSerial"
                      " JOIN LInk ON Link.IdEtap == Etap.IdEtap"
                      " JOIN Etap ON Link.IdProverka == Proverka.IdProverka"
                      " JOIN Result ON Link.IdLink == Result.IdLink"
                      " Where Device.[Название устройства] = '"+EtapGroup_Device->currentText()+"'"
                                                                                                " AND DeviceSerial.[Серийный номер] = '"+NDevice_ComboBox->currentText()+"'");


    QString SELECT ;

    do
    {
        SELECT += sql3.value(0).toString()+",";
        count_tr++;

    }while(sql3.next() !=NULL);

    SELECT.remove(SELECT.count()-1,1);

    //Формирование запроса для ПСИ
    QString PSI;


    sql.first();
    sql3.first();

    QVector<double> list; //Вектор для усреднения

    double sredneeNeravnomernost =0;

    for(int i=0;i < count_tr;i++)
    {
        sredneeNeravnomernost =0;
        list.clear();

        for(int j=1;j < 65;j++)
        {
            PSI.clear();

            // Проверка на второй элимент Power или нет?


            PSI += "SELECT ";

            PSI +=" MAX([Неравномерность])";

            PSI +=        " FROM Proverka"
                          " JOIN Device ON Device.IdDevice=DeviceSerial.IdDevice"
                          " JOIN DeviceSerial ON Link.IdDevice == DeviceSerial.IdSerial"
                          " JOIN LInk ON Link.IdEtap == Etap.IdEtap"
                          " JOIN Etap ON Link.IdProverka == Proverka.IdProverka"
                          " JOIN Result ON Link.IdLink == Result.IdLink"
                          " JOIN GraphPoint ON Link.IdLink == GraphPoint.IdResult"
                          " Where Device.[Название устройства] = '"+EtapGroup_Device->currentText()+"'"
                                                                                                    " AND DeviceSerial.[Серийный номер] = '"+NDevice_ComboBox->currentText()+"'"
                                                                                                                                                                             " AND Result.[Этап] = 'Регулировка'"
                                                                                                                                                                             " AND Proverka.[Имя проверки] = 'Frequency'"
                                                                                                                                                                             " AND GraphPoint.NumberGraph = '"+QString::number(j)+"'";


            PSI += " AND Etap.Etaps = '"+sql.value(0).toString()+"'";

            PSI += " AND GraphPoint.DataProverki = (SELECT MAX(Result.Date)"
                   " FROM Proverka"
                   " JOIN Device ON Device.IdDevice=DeviceSerial.IdDevice"
                   " JOIN DeviceSerial ON Link.IdDevice == DeviceSerial.IdSerial"
                   " JOIN LInk ON Link.IdEtap == Etap.IdEtap"
                   " JOIN Etap ON Link.IdProverka == Proverka.IdProverka"
                   " JOIN Result ON Link.IdLink == Result.IdLink"
                   " Where Device.[Название устройства] = '"+EtapGroup_Device->currentText()+"'"
                                                                                             " AND DeviceSerial.[Серийный номер] = '"+NDevice_ComboBox->currentText()+"'"
                                                                                                                                                                      " AND Result.[Этап] = 'Регулировка'"
                                                                                                                                                                      " AND Etap.[Etaps] = '"+sql.value(0).toString()+"')"
                                                                                                                                                                                                                      " AND Result.Date = (SELECT MAX(Result.Date)"
                                                                                                                                                                                                                      " FROM Proverka"
                                                                                                                                                                                                                      " JOIN Device ON Device.IdDevice=DeviceSerial.IdDevice"
                                                                                                                                                                                                                      " JOIN DeviceSerial ON Link.IdDevice == DeviceSerial.IdSerial"
                                                                                                                                                                                                                      " JOIN LInk ON Link.IdEtap == Etap.IdEtap"
                                                                                                                                                                                                                      " JOIN Etap ON Link.IdProverka == Proverka.IdProverka"
                                                                                                                                                                                                                      " JOIN Result ON Link.IdLink == Result.IdLink"
                                                                                                                                                                                                                      " Where Device.[Название устройства] = '"+EtapGroup_Device->currentText()+"'"
                                                                                                                                                                                                                                                                                                " AND DeviceSerial.[Серийный номер] = '"+NDevice_ComboBox->currentText()+"'"
                                                                                                                                                                                                                                                                                                                                                                         " AND Result.[Этап] = 'Регулировка'"
                                                                                                                                                                                                                                                                                                                                                                         " AND Etap.[Etaps] = '"+sql.value(0).toString()+"')";

            sql_PSI = BD->zapros(PSI);

            list.append(sql_PSI.value(0).toDouble());

            sredneeNeravnomernost += sql_PSI.value(0).toDouble();

        }



        List.append(sredneeNeravnomernost/list.count());

        sql.next();


    }

    return List;
}

QVector<QSqlQuery> MainWindow::CreateVectorResultPDF_Regylirovka_power()
{
    int count_td =0; //колонки
    int count_tr =0; //строчки

    //Лист результатов
    QVector<QSqlQuery> List;

    //Запрос на количество Этапов для формирования колонок
    QSqlQuery sql;

    //Запрос на получение количества строчек
    QSqlQuery sql3;

    //Запрос на получение данных
    QSqlQuery sql_PSI;


    sql = BD->zapros("SELECT DISTINCT Etap.Etaps"
                     " FROM Etap"
                     " WHERE"
                     " Etap.[Регулировка] = '1'");

    do
    {
        count_td++;
    }while(sql.next() != NULL);


    //Запрос на получение количества строчек
    sql3 = BD->zapros("SELECT DISTINCT [Имя проверки],[Описание]"
                      " FROM Proverka"
                      " JOIN Device ON Device.IdDevice=DeviceSerial.IdDevice"
                      " JOIN DeviceSerial ON Link.IdDevice == DeviceSerial.IdSerial"
                      " JOIN LInk ON Link.IdEtap == Etap.IdEtap"
                      " JOIN Etap ON Link.IdProverka == Proverka.IdProverka"
                      " JOIN Result ON Link.IdLink == Result.IdLink"
                      " Where Device.[Название устройства] = '"+EtapGroup_Device->currentText()+"'"
                                                                                                " AND DeviceSerial.[Серийный номер] = '"+NDevice_ComboBox->currentText()+"'");


    QString SELECT ;


    do
    {


        SELECT += sql3.value(0).toString()+",";
        count_tr++;

    }while(sql3.next() !=NULL);

    SELECT.remove(SELECT.count()-1,1);

    //Формирование запроса для ПСИ
    QString PSI;


    sql.first();
    sql3.first();

    for(int i=0;i < count_tr;i++)
    {
        PSI.clear();

        // Проверка на второй элимент Power или нет?


        PSI += "SELECT DISTINCT ";

        PSI +=" [PowerResult]";

        PSI +=        " FROM Proverka"
                      " JOIN Device ON Device.IdDevice=DeviceSerial.IdDevice"
                      " JOIN DeviceSerial ON Link.IdDevice == DeviceSerial.IdSerial"
                      " JOIN LInk ON Link.IdEtap == Etap.IdEtap"
                      " JOIN Etap ON Link.IdProverka == Proverka.IdProverka"
                      " JOIN Result ON Link.IdLink == Result.IdLink"
                      " JOIN Power ON Link.IdLink == Power.IdLink"
                      " Where Device.[Название устройства] = '"+EtapGroup_Device->currentText()+"'"
                                                                                                " AND DeviceSerial.[Серийный номер] = '"+NDevice_ComboBox->currentText()+"'"
                                                                                                                                                                         " AND Result.[Этап] = 'Регулировка'";


        PSI += " AND Etap.Etaps = '"+sql.value(0).toString()+"'";

        PSI += " AND Power.DataProverki = (SELECT MAX(Result.Date)"
               " FROM Proverka"
               " JOIN Device ON Device.IdDevice=DeviceSerial.IdDevice"
               " JOIN DeviceSerial ON Link.IdDevice == DeviceSerial.IdSerial"
               " JOIN LInk ON Link.IdEtap == Etap.IdEtap"
               " JOIN Etap ON Link.IdProverka == Proverka.IdProverka"
               " JOIN Result ON Link.IdLink == Result.IdLink"
               " Where Device.[Название устройства] = '"+EtapGroup_Device->currentText()+"'"
                                                                                         " AND DeviceSerial.[Серийный номер] = '"+NDevice_ComboBox->currentText()+"'"
                                                                                                                                                                  " AND Result.[Этап] = 'Регулировка'";

        PSI += " AND Etap.Etaps = '"+sql.value(0).toString()+"')";

        sql_PSI = BD->zapros(PSI);


        List.append(sql_PSI);

        sql.next();

    }

    return List;

}

QVector<QSqlQuery> MainWindow::CreateVectorResultPDF_power()
{
    int count_td =0; //колонки
    int count_tr =0; //строчки

    //Лист результатов
    QVector<QSqlQuery> List;

    //Запрос на количество Этапов для формирования колонок
    QSqlQuery sql;

    //Запрос на получение количества строчек
    QSqlQuery sql3;

    //Запрос на получение данных
    QSqlQuery sql_PSI;


    sql = BD->zapros("SELECT DISTINCT Etap.Etaps"
                     " FROM Etap"
                     " WHERE"
                     " Etap.[ПСИ] = '1'");

    do
    {
        count_td++;
    }while(sql.next() != NULL);


    //Запрос на получение количества строчек
    sql3 = BD->zapros("SELECT DISTINCT [Имя проверки],[Описание]"
                      " FROM Proverka"
                      " JOIN Device ON Device.IdDevice=DeviceSerial.IdDevice"
                      " JOIN DeviceSerial ON Link.IdDevice == DeviceSerial.IdSerial"
                      " JOIN LInk ON Link.IdEtap == Etap.IdEtap"
                      " JOIN Etap ON Link.IdProverka == Proverka.IdProverka"
                      " JOIN Result ON Link.IdLink == Result.IdLink"
                      " Where Device.[Название устройства] = '"+EtapGroup_Device->currentText()+"'"
                                                                                                " AND DeviceSerial.[Серийный номер] = '"+NDevice_ComboBox->currentText()+"'");


    QString SELECT ;

    do
    {
        SELECT += sql3.value(0).toString()+",";
        count_tr++;
    }while(sql3.next() !=NULL);

    SELECT.remove(SELECT.count()-1,1);

    //Формирование запроса для ПСИ
    QString PSI;


    sql.first();
    sql3.first();

    for(int i=0;i < count_tr;i++)
    {
        PSI.clear();

        // Проверка на второй элимент Power или нет?


        PSI += "SELECT DISTINCT ";

        PSI +=" [PowerResult],[IdPower], [Volt], [Tok]";

        PSI +=        " FROM Proverka"
                      " JOIN Device ON Device.IdDevice=DeviceSerial.IdDevice"
                      " JOIN DeviceSerial ON Link.IdDevice == DeviceSerial.IdSerial"
                      " JOIN LInk ON Link.IdEtap == Etap.IdEtap"
                      " JOIN Etap ON Link.IdProverka == Proverka.IdProverka"
                      " JOIN Result ON Link.IdLink == Result.IdLink"
                      " JOIN Power ON Link.IdLink == Power.IdLink"
                      " Where Device.[Название устройства] = '"+EtapGroup_Device->currentText()+"'"
                                                                                                " AND DeviceSerial.[Серийный номер] = '"+NDevice_ComboBox->currentText()+"'"
                                                                                                                                                                         " AND Result.[Этап] = 'ПСИ'";

        qDebug () <<"dsfkljsdafklj;lasdjfa   " << sql.value(0).toString();

        PSI += " AND Etap.Etaps = '"+sql.value(0).toString()+"'";

        PSI += " AND Power.DataProverki = (SELECT MAX(Result.Date)"
               " FROM Proverka"
               " JOIN Device ON Device.IdDevice=DeviceSerial.IdDevice"
               " JOIN DeviceSerial ON Link.IdDevice == DeviceSerial.IdSerial"
               " JOIN LInk ON Link.IdEtap == Etap.IdEtap"
               " JOIN Etap ON Link.IdProverka == Proverka.IdProverka"
               " JOIN Result ON Link.IdLink == Result.IdLink"
               " Where Device.[Название устройства] = '"+EtapGroup_Device->currentText()+"'"
                                                                                         " AND DeviceSerial.[Серийный номер] = '"+NDevice_ComboBox->currentText()+"'"
                                                                                                                                                                  " AND Result.[Этап] = 'ПСИ'";


        PSI += " AND Etap.Etaps = '"+sql.value(0).toString()+"')";

        sql_PSI = BD->zapros(PSI);



        QString SELECT2 ;

        QStringList lol;

        do
        {
            SELECT2 = sql_PSI.value(0).toString()+",";

            lol.append(SELECT2);

        }while(sql_PSI.next() !=NULL);

        sql_PSI.first();

        List.append(sql_PSI);

        sql.next();

    }

    return List;

}


// Отчет PDF
void MainWindow::PDF()
{




    //        QTextDocument doc;
    //        QString table_html;
    //        QString table_style;

    //        QVector<QSqlQuery> List;


    //        QVector<QSqlQuery> List2;

    //        QVector<QSqlQuery> List3; // Регулировка данные результата

    //        QVector<QSqlQuery> List4; // Регулировка данные результата power

    //        QVector<double> ListNearavnomernost_Regylirovka;
    //        QVector<double> ListNearavnomernost_PSI;

    //        List = CreateVectorResultPDF();

    //        List2 =  CreateVectorResultPDF_power();

    //        List3 = CreateVectorResultPDF_Regylirovka();

    //        List4 = CreateVectorResultPDF_Regylirovka_power();

    //        ListNearavnomernost_Regylirovka   = CreateVectorResultPDF_Regylirovka_AchH();

    //        ListNearavnomernost_PSI = CreateVectorResultPDF_PSI_AchH();

    //        int count_td_default = 7;  //Колонка под : №, Наименование параметра, Требования, Этап регулировки: НУ,-40,+50,после тех.тренеровки в НУ
    //        int count_td_default_regylirovka = 3;  //Колонка под : №, Наименование параметра, Требования
    //        int count_td = 0;   //Количество колонок по ПСИ

    //        int count_tr_default = 2; //Строчки шапка

    //        int count_tr = 0; //Строчки








    //        table_style= QString("<style>"
    //                             " html,body {margin: 0;padding: 0;}"
    //                             " h1 {margin-bottom: 0px; text:8pt;}"
    //                             " h2 {margin-top: 0px; text:6pt;}"
    //                             "table {margin-top: 20px;  border-collapse: collapse; border: 5px solid  black; }"  /* background-color: silver;*/
    //                             "td,th {text-align: center; vertical-align: middle;border: 5px solid black;}"
    //                             //                         "td,th {text-align: center; vertical-align: middle; border: 5px solid black;border-collapse: collapse;}"
    //                             //                       "p {margin: 0px;}"
    //                             "td {}" //word-wrap:break-word;
    //                             "tr {}" //word-wrap:break-word;
    //                             "</style>");

    //        table_style.clear();

    //        table_style= QString("<style>"
    //                             "html,body {margin: 0;padding: 0;}"
    //                             "p {margin: 0px;}"
    //                             "table {word-wrap:break-word; text-align: center; vertical-align: middle; border-collapse: collapse; border: 10px solid  black; }"
    //                             "td,th {text-align: center;}"
    //                             "</style>");

    //        table_html =table_style;

    //        table_html += QString(" <center>"
    //                              "<h1>"+EtapGroup_Device->currentText()+" №"+NDevice_ComboBox->currentText()+"</h1>"
    //                    "<h2>Электронный протокол предварительных и приемосдаточных испытаний</h2>"
    //                    "</center>\n");



    //        //Запрос на количество Этапов для формирования колонок
    //        QSqlQuery sql;


    //        //Запрос на данные по Требованиям
    //        QSqlQuery sql2;
    //        //Запрос на данные по Наименованию параметра
    //        QSqlQuery sql3;

    //        //Запрос на данные по ПСИ
    //        QSqlQuery sql_PSI;


    //        sql = BD->zapros("SELECT DISTINCT Etap.Etaps"
    //                         " FROM Etap"
    //                         " WHERE"
    //                         " Etap.[ПСИ] = '1'");


    //        table_html +=  " <table   width=\"100%\" border=\"1\" cellpadding=\"0\" cellspacing=\"0\" >"; //cellpadding=\"1\"



    //        do
    //        {
    //            count_td++;
    //        }while(sql.next() != NULL);



    //        //Формирование шапки 1 объединенная строка
    //        table_html +=  " <tr >"  //bgcolor=\"#FFFFFF\"
    //                " <td rowspan=\"2\">№</td>"
    //                " <td rowspan=\"2\" >Наименование параметра</td>"
    //                " <td rowspan=\"2\" >Требования</td>"
    //                " <td colspan=\"3\" >Этап регулировки </td>"
    //                " <td rowspan=\"2\" >После тех. тренеровки в НУ</td>"
    //                " <td colspan=\""+QString::number(count_td)+"\" >ПСИ</td>"
    //                 " </tr>";


    //        table_html +=   " <tr >" //bgcolor=\"#FFFFFF\"
    //                " <td >НУ</td>"
    //                " <td >-40</td>"
    //                " <td >+50</td>";

    //        //Формирование шапки 2 объединенная строка

    //        sql.first();

    //        for(int i=0;i<count_td;i++)
    //        {

    //            table_html += "<td >"+sql.value(0).toString()+"</td>\n";
    //            sql.next();
    //        }

    //        table_html += " </tr>";

    //        count_td +=count_td_default;  //Сложение колонок для вычисления всех колонок

    //        qDebug() << "Columns sql = " << count_td;


    //        //Запрос на данные по требованиям
    //        sql2 = BD->zapros("SELECT DISTINCT [Норма],[Минимум],[Максимум],[Допуск -],[Допуск +]"
    //                          " FROM Proverka"
    //                          " JOIN Device ON Device.IdDevice=DeviceSerial.IdDevice"
    //                          " JOIN DeviceSerial ON Link.IdDevice == DeviceSerial.IdSerial"
    //                          " JOIN LInk ON Link.IdEtap == Etap.IdEtap"
    //                          " JOIN Etap ON Link.IdProverka == Proverka.IdProverka"
    //                          " JOIN Conditious ON Link.IdConditious == Conditious.IdConditious"
    //                          " Where Device.[Название устройства] = '"+EtapGroup_Device->currentText()+"'"
    //              " AND DeviceSerial.[Серийный номер] = '"+NDevice_ComboBox->currentText()+"'");



    //        //Запрос на получение количества строчек
    //        sql3 = BD->zapros("SELECT DISTINCT [Имя проверки],[Описание]"
    //                          " FROM Proverka"
    //                          " JOIN Device ON Device.IdDevice=DeviceSerial.IdDevice"
    //                          " JOIN DeviceSerial ON Link.IdDevice == DeviceSerial.IdSerial"
    //                          " JOIN LInk ON Link.IdEtap == Etap.IdEtap"
    //                          " JOIN Etap ON Link.IdProverka == Proverka.IdProverka"
    //                          " JOIN Result ON Link.IdLink == Result.IdLink"
    //                          " Where Device.[Название устройства] = '"+EtapGroup_Device->currentText()+"'"
    //              " AND DeviceSerial.[Серийный номер] = '"+NDevice_ComboBox->currentText()+"'");



    //        do
    //        {
    //            count_tr++;
    //        } while(sql3.next() !=NULL);


    //        sql.first();
    //        sql3.first();
    //        sql2.first();

    //        double achh = 0;

    //        for(int i=0;i<count_tr;i++) // строки
    //        {
    //            table_html +=   " <tr>"; //bgcolor=\"#FFFFFF\"

    //            for(int j=0;j<count_td;j++) //колонки
    //            {
    //                switch (j)
    //                {
    //                case 0:
    //                    table_html += " <td >"+QString::number(i+1)+"</td>";
    //                    break;
    //                case 1:
    //                    table_html += " <td >"+sql3.value(1).toString()+"</td>";
    //                    break;
    //                case 2:
    //                    if(sql2.isSelect())
    //                    {
    //                        table_html += "<td >";

    //                        if(sql2.value(0).toString() != NULL)
    //                        {
    //                            table_html +=  "<p>Норма: "+sql2.value(0).toString()+" </p>";
    //                        }
    //                        if(sql2.value(1).toString() != NULL)
    //                        {
    //                            table_html +=  " <p>Минимум: "+sql2.value(1).toString()+" </p>";
    //                        }
    //                        if(sql2.value(2).toString() != NULL)
    //                        {
    //                            table_html +=  " <p>Максимум: "+sql2.value(2).toString()+" </p>";
    //                        }
    //                        if(sql2.value(3).toString() != NULL)
    //                        {
    //                            table_html +=  " <p>Допуск:["+sql2.value(3).toString()+";"+sql2.value(4).toString()+"]</p>";
    //                        }
    //                        table_html +=  " </td>";
    //                    }
    //                    else
    //                    {
    //                        table_html += " <td ></td>";
    //                    }
    //                    break;
    //                case 3:
    //                    if(i == 1)
    //                    {
    //                        table_html += " <td >"+List4.value(j-count_td_default_regylirovka).value(0).toString()+"</td>";
    //                    }
    //                    else
    //                    {
    //                        if(i ==3)
    //                        {
    //                            achh = ListNearavnomernost_Regylirovka.value(j-count_td_default_regylirovka);
    //                            if(achh > 0 )
    //                            {
    //                                table_html += " <td >"+QString::number(achh)+"</td>";
    //                            }
    //                            else
    //                            {
    //                                table_html += " <td ></td>";
    //                            }
    //                        }
    //                        else
    //                        {
    //                            table_html += " <td >"+List3.value(j-count_td_default_regylirovka).value(i).toString()+"</td>";
    //                        }
    //                    }
    //                    List3.value(j-count_td_default_regylirovka).next();
    //                    break;
    //                case 4:
    //                    if(i == 1)
    //                    {
    //                        table_html += " <td >"+List4.value(j-count_td_default_regylirovka).value(0).toString()+"</td>";
    //                    }
    //                    else
    //                    {
    //                        if(i ==3)
    //                        {
    //                            achh = ListNearavnomernost_Regylirovka.value(j-count_td_default_regylirovka);
    //                            if(achh > 0 )
    //                            {
    //                                table_html += " <td >"+QString::number(achh)+"</td>";
    //                            }
    //                            else
    //                            {
    //                                table_html += " <td ></td>";
    //                            }
    //                        }
    //                        else
    //                        {
    //                            table_html += " <td >"+List3.value(j-count_td_default_regylirovka).value(i).toString()+"</td>";
    //                        }
    //                    }
    //                    List3.value(j-count_td_default_regylirovka).next();
    //                    break;
    //                case 5:
    //                    if(i == 1)
    //                    {
    //                        table_html += " <td >"+List4.value(j-count_td_default_regylirovka).value(0).toString()+"</td>";
    //                    }
    //                    else
    //                    {
    //                        if(i ==3)
    //                        {
    //                            achh = ListNearavnomernost_Regylirovka.value(j-count_td_default_regylirovka);
    //                            if(achh > 0 )
    //                            {
    //                                table_html += " <td >"+QString::number(achh)+"</td>";
    //                            }
    //                            else
    //                            {
    //                                table_html += " <td ></td>";
    //                            }
    //                        }
    //                        else
    //                        {
    //                            table_html += " <td >"+List3.value(j-count_td_default_regylirovka).value(i).toString()+"</td>";
    //                        }
    //                    }
    //                    List3.value(j-count_td_default_regylirovka).next();
    //                    break;
    //                case 6:
    //                    if(i == 1)
    //                    {
    //                        table_html += " <td >"+List4.value(j-count_td_default_regylirovka).value(0).toString()+"</td>";
    //                    }
    //                    else
    //                    {
    //                        if(i ==3)
    //                        {
    //                            achh = ListNearavnomernost_Regylirovka.value(j-count_td_default_regylirovka);

    //                            if(achh > 0 )
    //                            {
    //                                table_html += " <td >"+QString::number(achh)+"</td>";
    //                            }
    //                            else
    //                            {
    //                                table_html += " <td ></td>";
    //                            }

    //                        }
    //                        else
    //                        {
    //                            table_html += " <td >"+List3.value(j-count_td_default_regylirovka).value(i).toString()+"</td>";
    //                        }
    //                    }
    //                    List3.value(j-count_td_default_regylirovka).next();
    //                    break;
    //                case 7:
    //                    if(i == 1)
    //                    {
    //                        table_html += " <td >"+List2.value(j-count_td_default).value(0).toString()+"</td>";
    //                    }
    //                    else
    //                    {
    //                        if(i ==3)
    //                        {
    //                            achh = ListNearavnomernost_PSI.value(j-count_td_default);
    //                            if(achh > 0 )
    //                            {
    //                                table_html += " <td >"+QString::number(achh)+"</td>";
    //                            }
    //                            else
    //                            {
    //                                table_html += " <td ></td>";
    //                            }
    //                        }
    //                        else
    //                        {
    //                            // table_html += " <td >"+List.value(j-count_td_default).value(i).toString().remove(5,15)+"</td>";
    //                            table_html += " <td >"+List.value(j-count_td_default).value(i).toString()+"</td>";
    //                        }
    //                    }
    //                    List.value(j-count_td_default).next();
    //                    break;
    //                case 8:
    //                    if(i == 1)
    //                    {
    //                        table_html += " <td >"+List2.value(j-count_td_default).value(0).toString()+"</td>";
    //                    }
    //                    else
    //                    {
    //                        if(i ==3)
    //                        {
    //                            achh = ListNearavnomernost_PSI.value(j-count_td_default);
    //                            if(achh > 0 )
    //                            {
    //                                table_html += " <td >"+QString::number(achh)+"</td>";
    //                            }
    //                            else
    //                            {
    //                                table_html += " <td ></td>";
    //                            }
    //                        }
    //                        else
    //                        {
    //                            // table_html += " <td >"+List.value(j-count_td_default).value(i).toString().remove(5,15)+"</td>";
    //                            table_html += " <td >"+List.value(j-count_td_default).value(i).toString()+"</td>";
    //                        }
    //                    }
    //                    List.value(j-count_td_default).next();
    //                    break;
    //                case 9:
    //                    if(i == 1)
    //                    {
    //                        table_html += " <td >"+List2.value(j-count_td_default).value(0).toString()+"</td>";
    //                    }
    //                    else
    //                    {
    //                        if(i ==3)
    //                        {
    //                            achh = ListNearavnomernost_PSI.value(j-count_td_default);
    //                            if(achh > 0 )
    //                            {
    //                                table_html += " <td >"+QString::number(achh)+"</td>";
    //                            }
    //                            else
    //                            {
    //                                table_html += " <td ></td>";
    //                            }
    //                        }
    //                        else
    //                        {
    //                            // table_html += " <td >"+List.value(j-count_td_default).value(i).toString().remove(5,15)+"</td>";
    //                            table_html += " <td >"+List.value(j-count_td_default).value(i).toString()+"</td>";
    //                        }
    //                    }
    //                    List.value(j-count_td_default).next();
    //                    break;
    //                case 10:
    //                    if(i == 1)
    //                    {
    //                        table_html += " <td >"+List2.value(j-count_td_default).value(0).toString()+"</td>";
    //                    }
    //                    else
    //                    {
    //                        if(i ==3)
    //                        {
    //                            achh = ListNearavnomernost_PSI.value(j-count_td_default);
    //                            if(achh > 0 )
    //                            {
    //                                table_html += " <td >"+QString::number(achh)+"</td>";
    //                            }
    //                            else
    //                            {
    //                                table_html += " <td ></td>";
    //                            }
    //                        }
    //                        else
    //                        {
    //                            // table_html += " <td >"+List.value(j-count_td_default).value(i).toString().remove(5,15)+"</td>";
    //                            table_html += " <td >"+List.value(j-count_td_default).value(i).toString()+"</td>";
    //                        }
    //                    }
    //                    List.value(j-count_td_default).next();
    //                    break;
    //                case 11:
    //                    if(i == 1)
    //                    {
    //                        table_html += " <td >"+List2.value(j-count_td_default).value(0).toString()+"</td>";
    //                    }
    //                    else
    //                    {
    //                        if(i ==3)
    //                        {
    //                            achh = ListNearavnomernost_PSI.value(j-count_td_default);
    //                            if(achh > 0 )
    //                            {
    //                                table_html += " <td >"+QString::number(achh)+"</td>";
    //                            }
    //                            else
    //                            {
    //                                table_html += " <td ></td>";
    //                            }
    //                        }
    //                        else
    //                        {
    //                            // table_html += " <td >"+List.value(j-count_td_default).value(i).toString().remove(5,15)+"</td>";
    //                            table_html += " <td >"+List.value(j-count_td_default).value(i).toString()+"</td>";
    //                        }
    //                    }
    //                    List.value(j-count_td_default).next();
    //                    break;
    //                case 12:
    //                    if(i == 1)
    //                    {
    //                        table_html += " <td >"+List2.value(j-count_td_default).value(0).toString()+"</td>";
    //                    }
    //                    else
    //                    {
    //                        if(i ==3)
    //                        {
    //                            achh = ListNearavnomernost_PSI.value(j-count_td_default);
    //                            if(achh > 0 )
    //                            {
    //                                table_html += " <td >"+QString::number(achh)+"</td>";
    //                            }
    //                            else
    //                            {
    //                                table_html += " <td ></td>";
    //                            }
    //                        }
    //                        else
    //                        {
    //                            // table_html += " <td >"+List.value(j-count_td_default).value(i).toString().remove(5,15)+"</td>";
    //                            table_html += " <td >"+List.value(j-count_td_default).value(i).toString()+"</td>";
    //                        }
    //                    }
    //                    List.value(j-count_td_default).next();
    //                    break;
    //                case 13:
    //                    if(i == 1)
    //                    {
    //                        table_html += " <td >"+List2.value(j-count_td_default).value(0).toString()+"</td>";
    //                    }
    //                    else
    //                    {
    //                        if(i ==3)
    //                        {
    //                            achh = ListNearavnomernost_PSI.value(j-count_td_default);
    //                            if(achh > 0 )
    //                            {
    //                                table_html += " <td >"+QString::number(achh)+"</td>";
    //                            }
    //                            else
    //                            {
    //                                table_html += " <td ></td>";
    //                            }
    //                        }
    //                        else
    //                        {
    //                            // table_html += " <td >"+List.value(j-count_td_default).value(i).toString().remove(5,15)+"</td>";
    //                            table_html += " <td >"+List.value(j-count_td_default).value(i).toString()+"</td>";
    //                        }
    //                    }
    //                    List.value(j-count_td_default).next();
    //                    break;

    //                default:
    //                    table_html += " <td ></td>";
    //                    break;
    //                }

    //            }

    //            sql.next();
    //            sql2.next();
    //            sql3.next();
    //            table_html += " </tr>";
    //        }


    //        table_html += " </table>";


    //        doc.setHtml(table_html);


    //        QString fileName = QFileDialog::getSaveFileName((QWidget* )0, "Export PDF", QString(), "*.pdf");
    //        if (QFileInfo(fileName).suffix().isEmpty()) { fileName.append(".pdf"); }

    //        QPrinter printer(QPrinter::HighResolution);
    //        printer.setOutputFormat(QPrinter::PdfFormat);
    //        printer.setPaperSize(QPrinter::A4);
    //        printer.setOutputFileName(fileName);

    //        printer.setOrientation(QPrinter::Landscape);
    //        // printer.setOrientation(QPrinter::Portrait);



    //        //doc.setPageSize(printer.pageRect().size()); // This is necessary if you want to hide the page number
    //        doc.print(&printer);


    slot_CreateWord();




}



void MainWindow::slot_CreateWord()
{


    QString fileName = QFileDialog::getSaveFileName((QWidget* )0, "Export Word", QString());//,"*.docx;;*.pdf"




    QList<QStringList> list = sql_listEtap(EtapGroup_Device->currentText(),NDevice_ComboBox->currentText());

    //Запрос на данные по требованиям
    QSqlQuery sql2 = BD->zapros("SELECT DISTINCT [Норма],[Минимум],[Максимум],[Допуск -],[Допуск +]"
                                " FROM Proverka"
                                " JOIN Device ON Device.IdDevice=DeviceSerial.IdDevice"
                                " JOIN DeviceSerial ON Link.IdDevice == DeviceSerial.IdSerial"
                                " JOIN LInk ON Link.IdEtap == Etap.IdEtap"
                                " JOIN Etap ON Link.IdProverka == Proverka.IdProverka"
                                " JOIN Conditious ON Link.IdConditious == Conditious.IdConditious"
                                " Where Device.[Название устройства] = '"+EtapGroup_Device->currentText()+"'"
                                                                                                          " AND DeviceSerial.[Серийный номер] = '"+NDevice_ComboBox->currentText()+"'");


    sql2.first();


    QVector<QSqlQuery> List;


    QVector<QSqlQuery> List2;

    QVector<QSqlQuery> List3; // Регулировка данные результата

    QVector<QSqlQuery> List4; // Регулировка данные результата power

    QVector<double> ListNearavnomernost_Regylirovka;
    QVector<double> ListNearavnomernost_PSI;

    List = CreateVectorResultPDF();

    List2 =  CreateVectorResultPDF_power();

    List3 = CreateVectorResultPDF_Regylirovka();

    List4 = CreateVectorResultPDF_Regylirovka_power();

    ListNearavnomernost_Regylirovka   = CreateVectorResultPDF_Regylirovka_AchH();

    ListNearavnomernost_PSI = CreateVectorResultPDF_PSI_AchH();

    QStringList listPowerPSI;
    QStringList listPowerRegyl;

    for(int i =0; i < List2.count();i++)
    {
        if(List2.value(i).value(0).toString() != "")
        {
            listPowerPSI.append(List2.value(i).value(0).toString());
        }
    }

    for(int i =0; i < List4.count();i++)
    {
        if(List4.value(i).value(0).toString() != "")
        {
            listPowerRegyl.append(List4.value(i).value(0).toString());
        }
    }



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    QAxObject* WordApplication=new QAxObject("Word.Application"); // Создаю интерфейс к MSWord



    if(WordApplication == nullptr)
    {
          QMessageBox msgBox;
          msgBox.setText("Ошибка .");
          msgBox.setInformativeText("Не найден Microsoft Word. Установите Microsoft Word.");
          msgBox.setStandardButtons(QMessageBox::Ok);
          int ret = msgBox.exec();
          return;

    }

     WordApplication->setProperty( "Visible", true );


    QAxObject* WordDocuments = WordApplication->querySubObject( "Documents()" ); // Получаю интерфейсы к его подобъекту "коллекция открытых документов":

    WordDocuments->querySubObject( "Add()" ); // Создаю новый документ
    // отключение грамматики
    QAxObject* Grammatic = WordApplication->querySubObject("Options()");
    Grammatic->setProperty("CheckSpellingAsYouType(bool)", false); // отключение грамматики
    QAxObject* ActiveDocument = WordApplication->querySubObject("ActiveDocument()");
    QAxObject* Range = ActiveDocument->querySubObject("Range()");


    QAxObject* PageSetup = ActiveDocument->querySubObject("PageSetup()");
    PageSetup->setProperty( "LeftMargin", 20);
    PageSetup->setProperty( "RightMargin", 10);
    PageSetup->setProperty( "TopMargin", 15);
    PageSetup->setProperty( "BottomMargin", 35);

    //Портретный вид документа
    PageSetup->setProperty("Orientation", 1);



    //Шрифт
    QAxObject *font = Range->querySubObject("Font");
    QAxObject *selection = WordApplication->querySubObject("Selection()"); //выделяем область



    // selection->dynamicCall("TypeParagraph()");






    font->setProperty("Size", 10); //размер заголовка
    // font->setProperty("Bold", 2); //жирный
    font->setProperty("Name", "Times New Roman"); // шрифт

    QAxObject* pswds = selection->querySubObject ("ParagraphFormat()");
    pswds->dynamicCall ("SetAlignment (WdParagraphAlignment)", 1); // по центру
    pswds->setProperty("SpaceAfter",0); // Межстрочные  интервал
    selection->setProperty("Alignment", 1);
    selection->dynamicCall("TypeText(const QString&)",EtapGroup_Device->currentText() + " №"+ NDevice_ComboBox->currentText() +"\n"); //записываем текст
    selection->dynamicCall("TypeText(const QString&)","Электронный протокол предъявительских и приемосдаточных испытаний (ПСИ)\n");
    // selection->dynamicCall("TypeText(const QString&)","Регулировщик:\n");
    selection->dynamicCall("TypeParagraph()");




    QAxObject *selection2 = WordApplication->querySubObject("Selection()");
    QAxObject *font2 = selection->querySubObject("Font()");
    font2->setProperty("Size", 8);
    font2->setProperty("Bold", 0);  // не жирный
    font2->setProperty("Name", "Times New Roman");
    // QAxObject* pswds2 = selection2->querySubObject ("ParagraphFormat ()");
    //  pswds2->dynamicCall ("SetAlignment (WdParagraphAlignment)", 1); // по ширине
    // selection2->setProperty("Alignment", 1);



    // создание таблицы
    QAxObject* Tables = selection2->querySubObject("Tables()");
    // выдиление таблицы колонок и столбцов
    QAxObject* NewTable = Tables->querySubObject("Add(Range, NumRows, NumColumns, DefaultTableBehavior, AutoFitBehavior)",
                                                 selection2->dynamicCall("Range"),
                                                 list_id_Proverki_Find.count()+4, // строчек 8 + 4 ( 2 под шапку 2 под ФИО и роспись)
                                                 14, // 14 колонок
                                                 2,
                                                 2); //Range(Start,End)




    // Регулировка в НУ
    // Регулировка в - 40
    // Регулировка +50
    // После техн.тренировк
    /////////////////////////////

    //Этап регулировки
    QAxObject* StartCell  = NewTable->querySubObject("Cell(Row, Column)", 1, 3); // (ячейка C1)
    QAxObject* CellRange = StartCell->querySubObject("Range()");

    //Выравнивание по центру (шапка)
    StartCell->setProperty("VerticalAlignment", 1);

    CellRange->dynamicCall("InsertAfter(Text)", "Этап регулировки");

    QAxObject* EndCell  = NewTable->querySubObject("Cell(Row, Column)", 1, 7);

    //Объединение ячеек
    StartCell ->dynamicCall("Merge(QAxObject *)",EndCell->asVariant());


    StartCell  = NewTable->querySubObject("Cell(Row, Column)", 2, 3);
    //Выравнивание по центру (шапка)
    StartCell->setProperty("VerticalAlignment", 1);
    CellRange = StartCell->querySubObject("Range()");
    CellRange->dynamicCall("InsertAfter(Text)", "НУ");

    StartCell  = NewTable->querySubObject("Cell(Row, Column)", 2, 4);
    //Выравнивание по центру (шапка)
    StartCell->setProperty("VerticalAlignment", 1);
    CellRange = StartCell->querySubObject("Range()");
    CellRange->dynamicCall("InsertAfter(Text)", "При пониженной рабочей температуре");

    StartCell  = NewTable->querySubObject("Cell(Row, Column)", 2, 5);
    //Выравнивание по центру (шапка)
    StartCell->setProperty("VerticalAlignment", 1);
    CellRange = StartCell->querySubObject("Range()");
    CellRange->dynamicCall("InsertAfter(Text)", "НУ\n после пониженной рабочей температуры");

    StartCell  = NewTable->querySubObject("Cell(Row, Column)", 2, 6);
    //Выравнивание по центру (шапка)
    StartCell->setProperty("VerticalAlignment", 1);
    CellRange = StartCell->querySubObject("Range()");
    CellRange->dynamicCall("InsertAfter(Text)", "При повышенной рабочей температуре");

    StartCell  = NewTable->querySubObject("Cell(Row, Column)", 2, 7);
    //Выравнивание по центру (шапка)
    StartCell->setProperty("VerticalAlignment", 1);
    CellRange = StartCell->querySubObject("Range()");
    CellRange->dynamicCall("InsertAfter(Text)", "НУ\n после повышенной рабочей температуры");



    /////////////////////////////
    //Наименование параметра
    StartCell = NewTable->querySubObject("Cell(Row, Column)", 1, 1);
    //Выравнивание по центру (шапка)
    StartCell->setProperty("VerticalAlignment", 1);
    CellRange = StartCell->querySubObject("Range()");
    CellRange->dynamicCall("InsertAfter(Text)", "Наименование параметра");

    EndCell  = NewTable->querySubObject("Cell(Row, Column)", 2, 1);

    //Объединение ячеек
    StartCell ->dynamicCall("Merge(QAxObject *)",EndCell->asVariant());
    /////////////////////////////

    //Требования ПМ
    StartCell = NewTable->querySubObject("Cell(Row, Column)", 1, 2);
    //Выравнивание по центру (шапка)
    StartCell->setProperty("VerticalAlignment", 1);
    CellRange = StartCell->querySubObject("Range()");
    CellRange->dynamicCall("InsertAfter(Text)", "Требования ПМ");

    EndCell  = NewTable->querySubObject("Cell(Row, Column)", 2, 2);

    //Объединение ячеек
    StartCell ->dynamicCall("Merge(QAxObject *)",EndCell->asVariant());


    ////////////////////////////////////////

    //После техн. тренировки в НУ
    StartCell = NewTable->querySubObject("Cell(Row, Column)", 1, 4);
    //Выравнивание по центру (шапка)
    StartCell->setProperty("VerticalAlignment", 1);
    CellRange = StartCell->querySubObject("Range()");
    CellRange->dynamicCall("InsertAfter(Text)", "После техн. тренировки в НУ");

    EndCell  = NewTable->querySubObject("Cell(Row, Column)", 2, 8);

    //Объединение ячеек
    StartCell ->dynamicCall("Merge(QAxObject *)",EndCell->asVariant());

    ////////////////////////////////////////

    //Предъявительские и ПСИ
    StartCell  = NewTable->querySubObject("Cell(Row, Column)", 1, 5); // (ячейка C1)
    //Выравнивание по центру (шапка)
    StartCell->setProperty("VerticalAlignment", 1);
    CellRange = StartCell->querySubObject("Range()");

    CellRange->dynamicCall("InsertAfter(Text)", "Предъявительские и ПСИ");


    EndCell  = NewTable->querySubObject("Cell(Row, Column)", 1, 9);


    //Объединение ячеек
    StartCell ->dynamicCall("Merge(QAxObject *)",EndCell->asVariant());


    StartCell  = NewTable->querySubObject("Cell(Row, Column)", 2, 9);
    //Выравнивание по центру (шапка)
    StartCell->setProperty("VerticalAlignment", 1);
    CellRange = StartCell->querySubObject("Range()");
    CellRange->dynamicCall("InsertAfter(Text)", "НУ");

    StartCell  = NewTable->querySubObject("Cell(Row, Column)", 2, 10);
    //Выравнивание по центру (шапка)
    StartCell->setProperty("VerticalAlignment", 1);
    CellRange = StartCell->querySubObject("Range()");
    CellRange->dynamicCall("InsertAfter(Text)", "При пониженной рабочей температуре");

    StartCell  = NewTable->querySubObject("Cell(Row, Column)", 2, 11);
    //Выравнивание по центру (шапка)
    StartCell->setProperty("VerticalAlignment", 1);
    CellRange = StartCell->querySubObject("Range()");
    CellRange->dynamicCall("InsertAfter(Text)", "НУ\n после пониженной рабочей температуры");

    StartCell  = NewTable->querySubObject("Cell(Row, Column)", 2, 12);
    //Выравнивание по центру (шапка)
    StartCell->setProperty("VerticalAlignment", 1);
    CellRange = StartCell->querySubObject("Range()");
    CellRange->dynamicCall("InsertAfter(Text)", "При повышенной рабочей температуре");

    StartCell  = NewTable->querySubObject("Cell(Row, Column)", 2, 13);
    //Выравнивание по центру (шапка)
    StartCell->setProperty("VerticalAlignment", 1);
    CellRange = StartCell->querySubObject("Range()");
    CellRange->dynamicCall("InsertAfter(Text)", "НУ\n после повышенной рабочей температуры");

    // //////////////////////////////////////

    //После неприрывной работы
    StartCell = NewTable->querySubObject("Cell(Row, Column)", 1, 6);
    //Выравнивание по центру (шапка)
    StartCell->setProperty("VerticalAlignment", 1);
    CellRange = StartCell->querySubObject("Range()");
    CellRange->dynamicCall("InsertAfter(Text)", "После непрерывной работы");

    EndCell  = NewTable->querySubObject("Cell(Row, Column)", 2, 14);

    //Объединение ячеек
    StartCell ->dynamicCall("Merge(QAxObject *)",EndCell->asVariant());





    double achh = 0;
    int count_td_default_regylirovka = 3;


    for(int columnIt=1; columnIt <= 14;columnIt++)
    {
        //так как от 3 строчки то i=3 и проходим по массиву на i-3
        for(int i=3; i < list_id_Proverki_Find.count()+3;i++)
        {


            switch (columnIt) {
            case 1:
                StartCell = NewTable->querySubObject("Cell(Row, Column)", i, columnIt);
                StartCell->setProperty("VerticalAlignment", 1);

                CellRange = StartCell->querySubObject("Range()");
                //Выравнивание по левому краю
                CellRange->querySubObject("ParagraphFormat()")->setProperty("Alignment",0);

                CellRange->dynamicCall("InsertAfter(Text)", list_id_Proverki_Find.value(i-3));
                break;
            case 2:
                /////////////////////////////////
                //Заполнение строчки требования ПМ
                StartCell = NewTable->querySubObject("Cell(Row, Column)", i, columnIt);

                StartCell->setProperty("VerticalAlignment", 1);
                CellRange = StartCell->querySubObject("Range()");
                //Выравнивание по левому краю
                CellRange->querySubObject("ParagraphFormat()")->setProperty("Alignment",1);

                CellRange->querySubObject("Font()")->setProperty("Size", 7);

                if(sql2.isSelect())
                {

                    if(sql2.value(0).toString() != NULL)
                    {
                        CellRange->dynamicCall("InsertAfter(Text)",  "Норма: "+sql2.value(0).toString()+"\n");
                    }
                    if(sql2.value(1).toString() != NULL)
                    {
                        CellRange->dynamicCall("InsertAfter(Text)",  "Минимум: "+sql2.value(1).toString()+"\n");
                    }
                    if(sql2.value(2).toString() != NULL)
                    {
                        CellRange->dynamicCall("InsertAfter(Text)",  "Максимум: "+sql2.value(2).toString()+"\n");
                    }
                    if(sql2.value(3).toString() != NULL)
                    {
                        CellRange->dynamicCall("InsertAfter(Text)", "Допуск:\n["+sql2.value(3).toString()+";"+sql2.value(4).toString()+"]\n");
                    }

                    sql2.next();
                }
                break;
            default:
                StartCell = NewTable->querySubObject("Cell(Row, Column)", i, columnIt);
                StartCell->setProperty("VerticalAlignment", 1);

                CellRange = StartCell->querySubObject("Range()");
                //Выравнивание по левому краю
                CellRange->querySubObject("ParagraphFormat()")->setProperty("Alignment",1);
                CellRange->querySubObject("Font()")->setProperty("Size", 7);


                if(list.value(columnIt-3).value(i-3) == "")
                {
                    CellRange->dynamicCall("InsertAfter(Text)", "-");
                }
                else
                {

                    switch (i-3) {
                    case 1:
                    {
                        if((columnIt-3) < 6)
                        {
                            CellRange->dynamicCall("InsertAfter(Text)", list.value(columnIt-3).value(i-3)+"\n");
                            CellRange->dynamicCall("InsertAfter(Text)", listPowerRegyl.value(columnIt-3)+" Вт");
                        }
                        else
                        {
                            CellRange->dynamicCall("InsertAfter(Text)", list.value(columnIt-3).value(i-3)+"\n");
                            CellRange->dynamicCall("InsertAfter(Text)", listPowerPSI.value(columnIt-3-6)+" Вт");
                        }
                        break;
                    }
                    case 3:
                    {
                        if((columnIt-3) < 6)
                        {
                            CellRange->dynamicCall("InsertAfter(Text)", list.value(columnIt-3).value(i-3)+"\n");
                            CellRange->dynamicCall("InsertAfter(Text)", QString::number(ListNearavnomernost_Regylirovka.value(columnIt-3)));
                        }
                        else
                        {
                            CellRange->dynamicCall("InsertAfter(Text)", list.value(columnIt-3).value(i-3)+"\n");
                            CellRange->dynamicCall("InsertAfter(Text)", QString::number(ListNearavnomernost_PSI.value(columnIt-3-6))+" Дб");
                        }
                        break;
                    }
                    default:
                        CellRange->dynamicCall("InsertAfter(Text)", list.value(columnIt-3).value(i-3));
                        break;

                    }

                }
                break;
            }

        }
    }

    for(int columnIt=3; columnIt <= 14;columnIt++)
    {
        for(int i=3+list_id_Proverki_Find.count(); i <= list_id_Proverki_Find.count()+3;i++)
        {
            StartCell = NewTable->querySubObject("Cell(Row, Column)", i, columnIt);
            StartCell->setProperty("VerticalAlignment", 1);

            CellRange = StartCell->querySubObject("Range()");
            //Выравнивание по левому краю
            CellRange->querySubObject("ParagraphFormat()")->setProperty("Alignment",1);
            CellRange->querySubObject("Font()")->setProperty("Size", 7);


            if(list_data.value(columnIt-3) == "")
            {
                CellRange->dynamicCall("InsertAfter(Text)", "-");
            }
            else
            {
                CellRange->dynamicCall("InsertAfter(Text)", list_data.value(columnIt-3)+"\n");
            }
        }
    }


    StartCell = NewTable->querySubObject("Cell(Row, Column)", 11, 2);
    StartCell->setProperty("VerticalAlignment", 1);

    CellRange = StartCell->querySubObject("Range()");
    //Выравнивание по левому краю
    CellRange->querySubObject("ParagraphFormat()")->setProperty("Alignment",1);
    CellRange->dynamicCall("InsertAfter(Text)", "\n\n\n");

    StartCell = NewTable->querySubObject("Cell(Row, Column)", 11, 1);
    StartCell->setProperty("VerticalAlignment", 1);

    CellRange = StartCell->querySubObject("Range()");
    //Выравнивание по левому краю
    CellRange->querySubObject("ParagraphFormat()")->setProperty("Alignment",0);
    CellRange->dynamicCall("InsertAfter(Text)", "Дата");


    StartCell = NewTable->querySubObject("Cell(Row, Column)", 12, 2);
    StartCell->setProperty("VerticalAlignment", 1);

    CellRange = StartCell->querySubObject("Range()");
    //Выравнивание по левому краю
    CellRange->querySubObject("ParagraphFormat()")->setProperty("Alignment",1);
    CellRange->dynamicCall("InsertAfter(Text)", "\n\n\n");

    StartCell = NewTable->querySubObject("Cell(Row, Column)", 12, 1);
    StartCell->setProperty("VerticalAlignment", 1);

    CellRange = StartCell->querySubObject("Range()");
    //Выравнивание по левому краю
    CellRange->querySubObject("ParagraphFormat()")->setProperty("Alignment",0);
    CellRange->dynamicCall("InsertAfter(Text)", "Подпись");





    //ActiveDocument->dynamicCall("SaveAs2 (const QString&)", fileName);
    ActiveDocument->dynamicCall("ExportAsFixedFormat (const QString&,const QString&)", fileName.split('.').first(),"17");


    ActiveDocument->dynamicCall("Close (boolean)", false);
    WordApplication->dynamicCall("Quit (void)");


}

void MainWindow::addBD(QString data)
{
    BD->zaprosVoid(data);
}

