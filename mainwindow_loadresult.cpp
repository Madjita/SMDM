#include <mainwindow.h>
#include "ui_mainwindow.h"

#define NERAVN 4

#define KF_l 19.5
#define KF_U 21.5


void MainWindow::DisebleElementsRetult()
{

    Button_StartProverka->setDisabled(1);
    GroupBox_RadioButton->setDisabled(1);
    EtapGroup_Device->setDisabled(1);
    EtapGroup->setDisabled(1);
    NDevice_ComboBox->setDisabled(1);
    Button_EndOpros->setDisabled(1);
    Button_StartOpros->setDisabled(1);
    Button_Result->setDisabled(1);
    Button_Regylirovka->setDisabled(1);
}

void MainWindow::Result()
{

    DisebleElementsRetult();

    ui->textEdit_PutBlock->clear();

    ui->stackedWidget->setCurrentIndex(1);

    QSqlQuery sql;

    sql = BD->zapros("SELECT * FROM Device");

    do
    {
        ui->textEdit_PutBlock->insertPlainText(sql.value(1).toString()+"\n");
    }while (sql.next());


    connect(ui->textEdit_PutBlock,&QTextEdit::cursorPositionChanged, this,&MainWindow::PutBlock);
    connect(ui->textEdit_PutSerial,&QTextEdit::cursorPositionChanged, this, &MainWindow::PutSer);
    connect(ui->textEdit_PutEtap,&QTextEdit::cursorPositionChanged, this, &MainWindow::putEtap);
    connect(ui->textEdit_PutProverka,&QTextEdit::cursorPositionChanged, this, &MainWindow::putProverka);
    //connect(ui->textEdit_AB,SIGNAL(cursorPositionChanged()), this, SLOT(PutRegyl())); // регулировка

}


void MainWindow::PutBlock()
{
    ui->textEdit_PutSerial->clear();

    int lineNumber=0;
    QTextEdit::ExtraSelection selection;
    QTextCursor cursor = QTextCursor(ui->textEdit_PutBlock->document());
    lineNumber = ui->textEdit_PutBlock->textCursor().blockNumber();
    cursor.movePosition(QTextCursor::Start);
    cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor,lineNumber);
    cursor.select(QTextCursor::LineUnderCursor);
    QTextCharFormat format;
    format.setForeground(Qt::white);
    format.setBackground(Qt::blue);
    selection.cursor = cursor;
    selection.format = format;
    ui->textEdit_PutBlock->setExtraSelections(QList<QTextEdit::ExtraSelection>() << selection);

    PutBlok = ui->textEdit_PutBlock->textCursor().block().text(); //Выбранный текст.

    QSqlQuery sql;

    sql = BD->zapros("SELECT * FROM DeviceSerial JOIN Device ON Device.IdDevice=DeviceSerial.IdDevice WHERE Device.[Название устройства]='"+PutBlok+"'");

    do
    {
        ui->textEdit_PutSerial->insertPlainText(sql.value(2).toString()+"\n");
    }while (sql.next());

}


void MainWindow::PutSer()
{
    ui->textEdit_PutEtap->clear();

    int lineNumber=0;
    QTextEdit::ExtraSelection selection;
    QTextCursor cursor = QTextCursor(ui->textEdit_PutSerial->document());
    lineNumber = ui->textEdit_PutSerial->textCursor().blockNumber();
    cursor.movePosition(QTextCursor::Start);
    cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor,lineNumber);
    cursor.select(QTextCursor::LineUnderCursor);
    QTextCharFormat format;
    format.setForeground(Qt::white);
    format.setBackground(Qt::blue);
    selection.cursor = cursor;
    selection.format = format;
    ui->textEdit_PutSerial->setExtraSelections(QList<QTextEdit::ExtraSelection>() << selection);

    PutBlok = ui->textEdit_PutBlock->textCursor().block().text(); //Выбранный текст.

    PutSerial = ui->textEdit_PutSerial->textCursor().block().text(); //Выбранный текст.

    bool flagPSI = false;
    bool flagPi = false;
    bool flagRegyl = false;

    if(PutSerial!="")
    {
        QSqlQuery sql;

        sql = BD->zapros("SELECT DISTINCT [Этап] "
                  " FROM Proverka"
                  " JOIN Device ON Device.IdDevice == DeviceSerial.IdDevice"
                  " JOIN DeviceSerial ON Link.IdDevice == DeviceSerial.IdSerial"
                  " JOIN LInk ON Link.IdEtap == Etap.IdEtap"
                  " JOIN Etap ON Link.IdProverka == Proverka.IdProverka"
                  " JOIN Result ON Link.IdLink == Result.IdLink"
                  " Where Device.[Название устройства] = '"+PutBlok+"'"
                  " AND DeviceSerial.[Серийный номер] = '"+PutSerial+"'");

        sql.first();

        do
        {

            if(sql.value(0).toString() == "ПСИ" )
            {
                flagPSI = true;
            }
            if(sql.value(0).toString() == "ПИ")
            {
                flagPi = true;
            }
            if(sql.value(0).toString() == "Регулировка")
            {
                flagRegyl =true;
            }
        }while(sql.next());



        if(flagPSI)
        {
            ui->textEdit_PutEtap->insertPlainText("ПСИ\n");
        }
        if(flagPi)
        {
            ui->textEdit_PutEtap->insertPlainText("ПИ\n");
        }
        if(flagRegyl)
        {
            ui->textEdit_PutEtap->insertPlainText("Регулировка\n");
        }

    }
}

void MainWindow::putEtap()
{
    ui->textEdit_PutProverka->clear();

    int lineNumber=0;
    QTextEdit::ExtraSelection selection;
    QTextCursor cursor = QTextCursor(ui->textEdit_PutEtap->document());
    lineNumber = ui->textEdit_PutEtap->textCursor().blockNumber();
    cursor.movePosition(QTextCursor::Start);
    cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor,lineNumber);
    cursor.select(QTextCursor::LineUnderCursor);
    QTextCharFormat format;
    format.setForeground(Qt::white);
    format.setBackground(Qt::blue);
    selection.cursor = cursor;
    selection.format = format;
    ui->textEdit_PutEtap->setExtraSelections(QList<QTextEdit::ExtraSelection>() << selection);

    PutEtap = ui->textEdit_PutEtap->textCursor().block().text(); //Выбранный текст.

    QSqlQuery sql;

    if(PutEtap !="")
    {

    sql = BD->zapros("SELECT DISTINCT Result.Date , Etap.Etaps"
                     " FROM Proverka"
                     " JOIN Device ON Device.IdDevice == DeviceSerial.IdDevice"
                     " JOIN DeviceSerial ON Link.IdDevice == DeviceSerial.IdSerial"
                     " JOIN LInk ON Link.IdEtap == Etap.IdEtap"
                     " JOIN Etap ON Link.IdProverka == Proverka.IdProverka"
                     " JOIN Result ON Link.IdLink == Result.IdLink"
              " Where Device.[Название устройства] = '"+PutBlok+"'"
              " AND DeviceSerial.[Серийный номер] = '"+PutSerial+"'"
              " AND Result.[Этап] = '"+PutEtap+"'");
              //" AND (Result.ElectricalPower = 'Cоответствует' OR Result.ElectricalPower = 'Не соответствует')");


    qDebug () << PutBlok << " " << PutSerial << " " << PutEtap << " " << sql.value(0).toString();

    do
    {
        if(sql.value(0).toString() != "")
        {
            ui->textEdit_PutProverka->insertPlainText(sql.value(0).toString()+" / "+sql.value(1).toString()+"\n");
        }
    }while (sql.next());

    }

}

void MainWindow::putProverka()
{
    int lineNumber=0;
    QTextEdit::ExtraSelection selection;
    QTextCursor cursor = QTextCursor(ui->textEdit_PutProverka->document());
    lineNumber = ui->textEdit_PutProverka->textCursor().blockNumber();
    cursor.movePosition(QTextCursor::Start);
    cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor,lineNumber);
    cursor.select(QTextCursor::LineUnderCursor);
    QTextCharFormat format;
    format.setForeground(Qt::white);
    format.setBackground(Qt::blue);
    selection.cursor = cursor;
    selection.format = format;
    ui->textEdit_PutProverka->setExtraSelections(QList<QTextEdit::ExtraSelection>() << selection);

    auto p = ui->textEdit_PutProverka->textCursor().block().text().split(QRegExp(" / "),QString::SkipEmptyParts);

    if(p.count() > 0)
    {
        PutData = p[0]; //Выбранный текст.
        PutProverka = p[1];
    }

}


//Функция возвращения на выбор проверки либо на сами проверки
void MainWindow::GoBack()
{

    GroupBox_RadioButton->setDisabled(0);
    EtapGroup_Device->setDisabled(0);
    EtapGroup->setDisabled(0);
    NDevice_ComboBox->setDisabled(0);
    //Button_EndOpros->setDisabled(0);
    Button_StartOpros->setDisabled(0);
    Button_Result->setDisabled(0);
    Button_Regylirovka->setDisabled(0);
    ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex()-1);

}

//Функция возвращения на выбор проверки либо на сами проверки
void MainWindow::GoBack2()
{

    ui->stackedWidget->setCurrentIndex(ui->stackedWidget->currentIndex()-1);


}

//Функция загрузки просмоторта выбранного результата
void MainWindow::GoNext()
{

    //qDebug() << PutBlok;
    //qDebug() << PutSerial;

    ui->stackedWidget->setCurrentIndex(2);

    LoadResult();

    QObject::connect(win_frequency->tableWidgetAChH,SIGNAL(cellClicked(int,int)),this,SLOT(ClickCheckBoxGraf_winReuslt(int,int)),Qt::DirectConnection);
    QObject::connect(win_transferCoefficient->tableWidgetPerestrouka,SIGNAL(cellClicked(int,int)),this,SLOT(ClickCheckBoxGraf_winReuslt2(int,int)),Qt::DirectConnection);


}


QSqlQuery MainWindow::Find(int point)
{
    QSqlQuery sql;

    sql = BD->zapros("SELECT DISTINCT Result.IdLink, [Имя проверки]"
              " FROM Proverka"
              " JOIN Device ON Device.IdDevice=DeviceSerial.IdDevice"
              " JOIN DeviceSerial ON Link.IdDevice == DeviceSerial.IdSerial"
              " JOIN LInk ON Link.IdEtap == Etap.IdEtap"
              " JOIN Etap ON Link.IdProverka == Proverka.IdProverka"
              " JOIN Result ON Link.IdLink == Result.IdLink"
              " Where Device.[Название устройства] = '"+PutBlok+"'"
              " AND DeviceSerial.[Серийный номер] = '"+PutSerial+"'"
              " AND Result.[Этап] = '"+PutEtap+"'"
              " AND Etap.Etaps = '"+PutProverka+"'"
              " AND Result.Date = '"+PutData+"'"
              " AND Proverka.[Имя проверки] = 'Frequency'");


    QSqlQuery sql1;

    sql.first();


    sql1 = BD->zapros("SELECT DISTINCT [X],[Y],[Неравномерность],[Максимум],[Минимум]"
               " FROM Proverka"
               " JOIN Device ON Device.IdDevice=DeviceSerial.IdDevice"
               " JOIN DeviceSerial ON Link.IdDevice == DeviceSerial.IdSerial"
               " JOIN LInk ON Link.IdEtap == Etap.IdEtap"
               " JOIN Etap ON Link.IdProverka == Proverka.IdProverka"
               " JOIN Result ON Link.IdLink == Result.IdLink"
               " JOIN GraphPoint ON Result.IdLink == GraphPoint.IdResult"
               " Where Device.[Название устройства] = '"+PutBlok+"'"
               " AND DeviceSerial.[Серийный номер] = '"+PutSerial+"'"
               " AND Result.[Этап] = '"+PutEtap+"'"
               " AND Etap.Etaps = '"+PutProverka+"'"
               " AND Result.Date = '"+PutData+"'"
               " AND GraphPoint.DataProverki = '"+PutData+"'"
               " AND Result.IdLink = "+sql.value(0).toString()+""
               " AND GraphPoint.NumberGraph = '"+QString::number(point)+"'"
               " ORDER BY GraphPoint.[IdGraph]");

    sql1.first();



    return sql1;
}


QSqlQuery MainWindow::Find2(int point)
{
    QSqlQuery sql;

    sql = BD->zapros("SELECT DISTINCT Result.IdLink, [Имя проверки]"
              " FROM Proverka"
              " JOIN Device ON Device.IdDevice=DeviceSerial.IdDevice"
              " JOIN DeviceSerial ON Link.IdDevice == DeviceSerial.IdSerial"
              " JOIN LInk ON Link.IdEtap == Etap.IdEtap"
              " JOIN Etap ON Link.IdProverka == Proverka.IdProverka"
              " JOIN Result ON Link.IdLink == Result.IdLink"
              " Where Device.[Название устройства] = '"+PutBlok+"'"
              " AND DeviceSerial.[Серийный номер] = '"+PutSerial+"'"
              " AND Result.[Этап] = '"+PutEtap+"'"
              " AND Etap.Etaps = '"+PutProverka+"'"
              " AND Result.Date = '"+PutData+"'"
              " AND [Имя проверки] = 'TransferCoefficient'");

   // qDebug() << "kldh;fas;ldashf;lashdf = " << PutProverka;
   // qDebug() << "TransferCoefficient = "<< sql.value(0).toString();

    QSqlQuery sql1;

    sql1 = BD->zapros("SELECT DISTINCT [X],[Y],[Коэфициент передачи]"
               " FROM Proverka"
               " JOIN Device ON Device.IdDevice=DeviceSerial.IdDevice"
               " JOIN DeviceSerial ON Link.IdDevice == DeviceSerial.IdSerial"
               " JOIN LInk ON Link.IdEtap == Etap.IdEtap"
               " JOIN Etap ON Link.IdProverka == Proverka.IdProverka"
               " JOIN Result ON Link.IdLink == Result.IdLink"
               " JOIN GraphPoint ON Result.IdLink == GraphPoint.IdResult"
               " Where Device.[Название устройства] = '"+PutBlok+"'"
               " AND DeviceSerial.[Серийный номер] = '"+PutSerial+"'"
               " AND Result.[Этап] = '"+PutEtap+"'"
               " AND Etap.Etaps = '"+PutProverka+"'"
               " AND Result.Date = '"+PutData+"'"
               " AND GraphPoint.DataProverki = '"+PutData+"'"
               " AND Result.IdLink = "+sql.value(0).toString()+""
               " AND GraphPoint.NumberGraph = '"+QString::number(point)+"'"
               " ORDER BY GraphPoint.[IdGraph]");


    return sql1;
}

//Функция загрузки и компановки окна результатов
void MainWindow::LoadResult()
{

    if(ListMin.count() > 0)
    {
        ListMin.clear();
        ListMax.clear();
        ListNEravn.clear();
        ListX.clear();
        ListY.clear();

        ListPerestrouka.clear();
        ListX1.clear();
        ListY1.clear();
        PerestroykaX.clear();
    }

    list = new QList<QSqlQuery>();

    if(list->count() >0)
    {
        list->clear();
    }

    QSqlQuery sql;

    sql = BD->zapros("SELECT Result.IdLink, [Имя проверки]"
              " FROM Proverka"
              " JOIN Device ON Device.IdDevice=DeviceSerial.IdDevice"
              " JOIN DeviceSerial ON Link.IdDevice == DeviceSerial.IdSerial"
              " JOIN LInk ON Link.IdEtap == Etap.IdEtap"
              " JOIN Etap ON Link.IdProverka == Proverka.IdProverka"
              " JOIN Result ON Link.IdLink == Result.IdLink"
              " Where Device.[Название устройства] = '"+PutBlok+"'"
             " AND DeviceSerial.[Серийный номер] = '"+PutSerial+"'"
             " AND Result.[Этап] = '"+PutEtap+"'"
             " AND Etap.Etaps = '"+PutProverka+"'"
             " AND Result.Date = '"+PutData+"'"
             " AND [Имя проверки] = 'ElectricalPower'"
             " ORDER BY Result.IdLink");



    QString logId = sql.value(0).toString();

    sql = BD->zapros("SELECT Text, BlockName, DataStart"
              " FROM Proverka"
              " JOIN Device ON Device.IdDevice=DeviceSerial.IdDevice"
              " JOIN DeviceSerial ON Link.IdDevice == DeviceSerial.IdSerial"
              " JOIN LInk ON Link.IdEtap == Etap.IdEtap"
              " JOIN Etap ON Link.IdProverka == Proverka.IdProverka"
              " JOIN Result ON Link.IdLink == Result.IdLink"
              " JOIN Log ON Result.IdLink == Log.IdResult"
              " Where Device.[Название устройства] = '"+PutBlok+"'"
              " AND DeviceSerial.[Серийный номер] = '"+PutSerial+"'"
              " AND Result.[Этап] = '"+PutEtap+"'"
              " AND Etap.Etaps = '"+PutProverka+"'"
              " AND Log.DataStart = '"+PutData+"'"
              " AND Result.IdLink = '"+logId+"'"
              " ORDER BY Log.[IdLog]");


    log_text_Ist->clear();
    log_text_Ist->insertPlainText(sql.value(0).toString());

    sql.next();

    log_text_Micran->clear();
    log_text_Micran->insertPlainText(sql.value(0).toString());

     sql.next();

     log_text_TP->clear();
     log_text_TP->insertPlainText(sql.value(0).toString());

     sql.next();

     log_text_Block->clear();
     log_text_Block->insertPlainText(sql.value(0).toString());

     sql.next();

     log_text_N9000->clear();
     log_text_N9000->insertPlainText(sql.value(0).toString());

     sql.next();

     log_text->clear();
     log_text->insertPlainText(sql.value(0).toString());


     sql = BD->zapros("SELECT Result.IdLink, [Имя проверки]"
               " FROM Proverka"
               " JOIN Device ON Device.IdDevice=DeviceSerial.IdDevice"
               " JOIN DeviceSerial ON Link.IdDevice == DeviceSerial.IdSerial"
               " JOIN LInk ON Link.IdEtap == Etap.IdEtap"
               " JOIN Etap ON Link.IdProverka == Proverka.IdProverka"
               " JOIN Result ON Link.IdLink == Result.IdLink"
               " Where Device.[Название устройства] = '"+PutBlok+"'"
              " AND DeviceSerial.[Серийный номер] = '"+PutSerial+"'"
              " AND Result.[Этап] = '"+PutEtap+"'"
              " AND Etap.Etaps = '"+PutProverka+"'"
              " AND Result.Date = '"+PutData+"'"
              " AND [Имя проверки] = 'Power'"
              " ORDER BY Result.IdLink");



    IdLink = sql.value(0).toString();

    sql = BD->zapros("SELECT Result.Power"
              " FROM Proverka"
              " JOIN Device ON Device.IdDevice=DeviceSerial.IdDevice"
              " JOIN DeviceSerial ON Link.IdDevice == DeviceSerial.IdSerial"
              " JOIN LInk ON Link.IdEtap == Etap.IdEtap"
              " JOIN Etap ON Link.IdProverka == Proverka.IdProverka"
              " JOIN Result ON Link.IdLink == Result.IdLink"
              " Where Device.[Название устройства] = '"+PutBlok+"'"
              " AND DeviceSerial.[Серийный номер] = '"+PutSerial+"'"
              " AND Result.[Этап] = '"+PutEtap+"'"
              " AND Etap.Etaps = '"+PutProverka+"'"
              " AND Result.Date = '"+PutData+"'"
              " AND Result.IdLink = "+IdLink+"");




    LoadTableResultat();


   win_frequency->setupUi(ProverkaList3);

   win_transferCoefficient->setupUi(ProverkaList4);


    win_frequency->tableWidgetAChH->setRowCount(5);
    win_frequency->tableWidgetAChH->setColumnCount(13);

    int point=0;

    QVector<double> RepairListX;
    QVector<double> RepairListY;
    QVector<double> RepairListNEravn;
    QVector<double> RepairListMax;
    QVector<double> RepairListMin;

    QVector<double> RepairListPerestrouka;
    QVector<double> RepairListX1;
    QVector<double> RepairListY1;
    QVector<double> RepairListPerestroykaX;

    bool flag = false;




    for(int i=0;i<win_frequency->tableWidgetAChH->columnCount();i++)
    {
        win_frequency->tableWidgetAChH->setColumnWidth(i,50);

        for(int j=0;j<win_frequency->tableWidgetAChH->rowCount();j++)
        {
            point++;

            QTableWidgetItem * item = new QTableWidgetItem();

            win_frequency->tableWidgetAChH->setItem(j,i,item);

            if(point!=65) //убираем 65 чекбокс в таблице
            {

                item->setText(QString::number(point));

                win_frequency->tableWidgetAChH->item(j,i)->setCheckState(Qt::Unchecked);
                list->append(Find(point));

                RepairListX.clear();
                RepairListY.clear();
                RepairListNEravn.clear();
                RepairListMax.clear();
                RepairListMin.clear();

                do
                {
                    RepairListX.append(list->last().value(0).toInt());
                    RepairListY.append(list->last().value(1).toDouble());
                    RepairListNEravn.append(list->last().value(2).toDouble());
                    RepairListMax.append(list->last().value(3).toDouble());
                    RepairListMin.append(list->last().value(4).toDouble());

                    if(list->last().value(2).toDouble() >= NERAVN)
                    {
                        win_frequency->tableWidgetAChH->item(j,i)->setBackground(QBrush(Qt::red));
                        flag = true;
                    }

                } while(list->last().next());

                if(!flag)
                {
                    win_frequency->tableWidgetAChH->item(j,i)->setBackground(QBrush(Qt::green));
                }

                flag = false;

                ListX.append(RepairListX);
                ListY.append(RepairListY);
                ListNEravn.append(RepairListNEravn);
                ListMax.append(RepairListMax);
                ListMin.append(RepairListMin);

            }
            else
            {
                win_frequency->tableWidgetAChH->item(j,i)->setFlags(Qt::NoItemFlags);
            }
        }
    }

    //qDebug() << list->count();
    //qDebug() << ListX.count();
    //Загрузка графика Перестройки
    if(PutBlok =="СМ16-4")
    {
        win_transferCoefficient->tableWidgetPerestrouka->setRowCount(4);
        win_transferCoefficient->tableWidgetPerestrouka->setColumnCount(2);

        point =0;
        win_transferCoefficient->Graph->clearGraphs();
        for(int i=0;i<=2;i++)
        {
            for(int j=0;j<4;j++)
            {
                win_transferCoefficient->Graph->addGraph();
                win_transferCoefficient->Graph->graph(point)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
                win_transferCoefficient->Graph->graph(point)->setPen(QPen(qrand())); // line color blue for first graph
                win_transferCoefficient->Graph->graph(point)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue
                point++;
            }
        }

    }
    if(PutBlok =="ДМ4-16")
    {
        win_transferCoefficient->tableWidgetPerestrouka->setRowCount(16);
        win_transferCoefficient->tableWidgetPerestrouka->setColumnCount(2);

        point =0;
        win_transferCoefficient->Graph->clearGraphs();
        for(int i=0;i<=2;i++)
        {
            for(int j=0;j<16;j++)
            {
                win_transferCoefficient->Graph->addGraph();
                win_transferCoefficient->Graph->graph(point)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
                win_transferCoefficient->Graph->graph(point)->setPen(QPen(qrand())); // line color blue for first graph
                win_transferCoefficient->Graph->graph(point)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue
                point++;
            }
        }
    }
    if(PutBlok =="СМДМ8-4")
    {
        win_transferCoefficient->tableWidgetPerestrouka->setRowCount(10);
        win_transferCoefficient->tableWidgetPerestrouka->setColumnCount(2);

        point =0;
        win_transferCoefficient->Graph->clearGraphs();
        for(int i=0;i<=2;i++)
        {
            for(int j=0;j<10;j++)
            {
                win_transferCoefficient->Graph->addGraph();
                win_transferCoefficient->Graph->graph(point)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
                win_transferCoefficient->Graph->graph(point)->setPen(QPen(qrand())); // line color blue for first graph
                win_transferCoefficient->Graph->graph(point)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue
                point++;
            }
        }

    }

    //qDebug() <<"win_transferCoefficient->Graph->graphCount() " <<  win_transferCoefficient->Graph->graphCount();
    flag = false;
    point=0;

    for(int i=0;i<win_transferCoefficient->tableWidgetPerestrouka->columnCount();i++)
    {
        win_transferCoefficient->tableWidgetPerestrouka->setColumnWidth(i,40);

        for(int j=0;j<win_transferCoefficient->tableWidgetPerestrouka->rowCount();j++)
        {
            point++;

            QTableWidgetItem * item = new QTableWidgetItem();
            win_transferCoefficient->tableWidgetPerestrouka->setItem(j,i,item);


            item->setText(QString::number(point));

            win_transferCoefficient->tableWidgetPerestrouka->item(j,i)->setCheckState(Qt::Unchecked);
            sql = Find2(point);

            RepairListPerestrouka.clear();
            RepairListX1.clear();
            RepairListY1.clear();
            RepairListPerestroykaX.clear();

            int kol =-10;
            double interval =0;
            do
            {
                RepairListX1.append(sql.value(0).toDouble());
                RepairListY1.append(sql.value(1).toDouble());
                RepairListPerestrouka.append(sql.value(2).toDouble());
                RepairListPerestroykaX.append(kol);
                kol++;


                if(RepairListY1.count() < 20)
                {
                        continue ;
                }

                interval = qAbs(qAbs(RepairListY1.last())-qAbs(RepairListY1.first()));

            } while (sql.next());

            qDebug() << RepairListY1.count() << ") " <<  interval;

            if(interval  > KF_l && interval < KF_U)
            {
                win_transferCoefficient->tableWidgetPerestrouka->item(j,i)->setBackground(QBrush(Qt::green));
            }
            else
            {
               win_transferCoefficient->tableWidgetPerestrouka->item(j,i)->setBackground(QBrush(Qt::red));
            }

            flag = false;

            ListX1.append(RepairListX1);
            ListY1.append(RepairListY1);
            ListPerestrouka.append(RepairListPerestrouka);
            PerestroykaX.append(RepairListPerestroykaX);


        }
    }


    LoadingStartGraphResult();
}

//Добавление графиков
void MainWindow::AddGraphResult()
{
    int point =0;

    win_frequency->ProverkaGraph->clearGraphs();

    for(int i=0;i<5;i++)
    {
        for(int j=0;j<13;j++)
        {
            // point++; изменил
            win_frequency->ProverkaGraph->addGraph();
            win_frequency->ProverkaGraph->graph(point)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
            win_frequency->ProverkaGraph->graph(point)->setPen(QPen(qrand())); // line color blue for first graph
            win_frequency->ProverkaGraph->graph(point)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue
            point++;
        }
    }
}

//Функция загрузки и компановки графиков с таблицами
void MainWindow::LoadingStartGraphResult()
{
    win_frequency->ProverkaGraph->xAxis2->setVisible(true);
    win_frequency->ProverkaGraph->xAxis2->setTickLabels(false);
    win_frequency->ProverkaGraph->xAxis->setLabel("Частота(МГц)");
    win_frequency->ProverkaGraph->yAxis->setLabel("дБ");

    win_transferCoefficient->Graph->xAxis2->setVisible(true);
    win_transferCoefficient->Graph->xAxis2->setTickLabels(false);
    win_transferCoefficient->Graph->xAxis->setLabel("Установленные(дБ))");
    win_transferCoefficient->Graph->yAxis->setLabel("Измеренные(дБ)");

    AddGraphResult();

    win_frequency->ProverkaGraph->xAxis->setRange(900,2200);
    win_frequency->ProverkaGraph->yAxis->setRange(-10,2);

    win_transferCoefficient->Graph->xAxis->setRange(-12,12);
    win_transferCoefficient->Graph->yAxis->setRange(-30,14);

    win_frequency->ProverkaGraph->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    win_transferCoefficient->Graph->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    win_frequency->ProverkaGraph->replot();
    win_transferCoefficient->Graph->replot();
}


void MainWindow::LoadTableResultat()
{

  //  QSqlQuery *SQL;

    QSqlQueryModel *SQL_chekProverka;

    QSqlQueryModel *model = nullptr;

    if(PutEtap == "ПСИ")
    {
        model = BD->zaprosQueryModel("SELECT DISTINCT [Имя проверки],[Размерность]"
                        " FROM Proverka"
                        " JOIN Device ON Device.IdDevice=DeviceSerial.IdDevice"
                        " JOIN DeviceSerial ON Link.IdDevice == DeviceSerial.IdSerial"
                        " JOIN LInk ON Link.IdEtap == Etap.IdEtap"
                        " JOIN Etap ON Link.IdProverka == Proverka.IdProverka"
                        " Where Device.[Название устройства] = '"+PutBlok+"'"
                        " AND DeviceSerial.[Серийный номер] = '"+PutSerial+"'"
                        " AND Etap.Etaps = '" +PutProverka+"'"
                        " AND Etap.ПСИ = 1");

        SQL_chekProverka = BD->zaprosQueryModel("SELECT DISTINCT [Описание]"
                                   "FROM Proverka"
                                   " JOIN Device ON Device.IdDevice=DeviceSerial.IdDevice"
                                   " JOIN DeviceSerial ON Link.IdDevice == DeviceSerial.IdSerial"
                                   " JOIN LInk ON Link.IdEtap == Etap.IdEtap"
                                   " JOIN Etap ON Link.IdProverka == Proverka.IdProverka Where Device.[Название устройства] = '"+PutBlok+"'"
                                   " AND DeviceSerial.[Серийный номер] = '"+PutSerial+"'"
                                   " AND Etap.Etaps = '" +PutProverka+"'"
                                   " AND Etap.ПСИ = 1");

    }

    if(PutEtap == "ПИ")
    {
        model = BD->zaprosQueryModel("SELECT DISTINCT [Имя проверки],[Размерность]"
                        "FROM Proverka"
                        " JOIN Device ON Device.IdDevice=DeviceSerial.IdDevice"
                        " JOIN DeviceSerial ON Link.IdDevice == DeviceSerial.IdSerial"
                        " JOIN LInk ON Link.IdEtap == Etap.IdEtap"
                        " JOIN Etap ON Link.IdProverka == Proverka.IdProverka Where Device.[Название устройства] = '"+PutBlok+"'"
                        " AND DeviceSerial.[Серийный номер] = '"+PutSerial+"'"
                        " AND Etap.Etaps = '" +PutProverka+"'"
                        " AND Etap.ПИ = 1");

        SQL_chekProverka = BD->zaprosQueryModel("SELECT DISTINCT [Описание]"
                                   "FROM Proverka"
                                   " JOIN Device ON Device.IdDevice=DeviceSerial.IdDevice"
                                   " JOIN DeviceSerial ON Link.IdDevice == DeviceSerial.IdSerial"
                                   " JOIN LInk ON Link.IdEtap == Etap.IdEtap"
                                   " JOIN Etap ON Link.IdProverka == Proverka.IdProverka"
                                   " Where Device.[Название устройства] = '"+PutBlok+"'"
                                   " AND DeviceSerial.[Серийный номер] = '"+PutSerial+"'"
                                   " AND Etap.Etaps = '" +PutProverka+"'"
                                   " AND Etap.ПИ = 1");
    }

    if(PutEtap == "Регулировка")
    {
        model = BD->zaprosQueryModel("SELECT DISTINCT [Имя проверки],[Размерность]"
                        "FROM Proverka"
                        " JOIN Device ON Device.IdDevice=DeviceSerial.IdDevice"
                        " JOIN DeviceSerial ON Link.IdDevice == DeviceSerial.IdSerial"
                        " JOIN LInk ON Link.IdEtap == Etap.IdEtap"
                        " JOIN Etap ON Link.IdProverka == Proverka.IdProverka"
                        " Where Device.[Название устройства] = '"+PutBlok+"'"
                        " AND DeviceSerial.[Серийный номер] = '"+PutSerial+"'"
                        " AND Etap.Etaps = '" +PutProverka+"'"
                        " AND Etap.[Регулировка] = 1");

        SQL_chekProverka = BD->zaprosQueryModel("SELECT DISTINCT [Описание]"
                                   "FROM Proverka"
                                   " JOIN Device ON Device.IdDevice=DeviceSerial.IdDevice"
                                   " JOIN DeviceSerial ON Link.IdDevice == DeviceSerial.IdSerial"
                                   " JOIN LInk ON Link.IdEtap == Etap.IdEtap"
                                   " JOIN Etap ON Link.IdProverka == Proverka.IdProverka"
                                   " Where Device.[Название устройства] = '"+PutBlok+"'"
                                   " AND DeviceSerial.[Серийный номер] = '"+PutSerial+"'"
                                   " AND Etap.Etaps = '" +PutProverka+"'"
                                   " AND Etap.[Регулировка] = 1");
    }


    if(model != nullptr)
    {
        if (model->lastError().isValid())
        {
            qDebug() << model->lastError();
        }



    ui->view_2->setObjectName(QStringLiteral("Table"));
    ui->view_2->setRowCount(model->rowCount());
    ui->view_2->setColumnCount(model->columnCount());



    ui->tabWidget_2->clear();

    QPixmap R1_Gray(":/images/circle_gray.png");
    QPixmap R2_Green(":/images/circle_zel.png");
    QPixmap R3_Red(":/images/circle_kras.png");

   // QWidget *ProverkaList1 = new QWidget(); // окно проверки
    QWidget *ProverkaList2 = new QWidget(); // окно проверки
    //QWidget *ProverkaList3 = new QWidget(); // окно проверки
   // QWidget *ProverkaList4 = new QWidget(); // окно проверки
    QWidget *ProverkaList5 = new QWidget(); // окно проверки

    //qDebug() << model->columnCount() <<"dsfa" <<  model->rowCount();

    for(int i=0;i< ui->view_2->columnCount();i++)
    {
        QTableWidgetItem * itemHeader = new QTableWidgetItem();

        itemHeader->setText( model->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());
        ui->view_2->setHorizontalHeaderItem(i,itemHeader);


//        sql.exec("SELECT "+model->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString()+""
//                       " FROM Proverka"
//                       " JOIN Device ON Device.IdDevice=DeviceSerial.IdDevice"
//                       " JOIN DeviceSerial ON Link.IdDevice == DeviceSerial.IdSerial"
//                       " JOIN LInk ON Link.IdEtap == Etap.IdEtap"
//                       " JOIN Etap ON Link.IdProverka == Proverka.IdProverka"
//                       " JOIN Result ON Link.IdLink == Result.IdLink"
//                       " Where Device.[Название устройства] = '"+PutBlok+"'"
//                       " AND DeviceSerial.[Серийный номер] = '"+PutSerial+"'"
//                       " AND Result.[Этап] = '"+PutEtap+"'"
//                       " AND Etap.Etaps = '"+PutProverka+"'"
//                       " AND Result.Data = '"+PutData+"'");
//        sql.next();

//        if(sql.value(0).toString() == NULL)
//        {

//        }
//        else
//        {
//            if(sql.value(0).toString() == "Cоответствует")
//            {
//                  ui->view_2->setHorizontalHeaderItem(i,itemHeader);
//                  ui->view_2->item(i,0)->setBackgroundColor(Qt::green);
//            }
//            else
//            {
//                if(sql.value(0).toString() == "Не соотвутствует")
//                {
//                    ui->view_2->setHorizontalHeaderItem(i,itemHeader);
//                    ui->view_2->item(i,0)->setBackgroundColor(Qt::red);
//                }

//            }
//        }



        for(int j=0;j < ui->view_2->rowCount();j++)
        {

            QModelIndex newIndex = model->index(j,i);

            QTableWidgetItem * item = new QTableWidgetItem();

            item->setText(model->data(newIndex, Qt::EditRole).toString());

            // item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEditable);


            if(i==0)
            {
                item->setToolTip(SQL_chekProverka->data(newIndex,Qt::EditRole).toString());
            }

            ui->view_2->setItem(j,i,item);

            if(i==0)
            {
                ui->view_2->item(j,0)->setCheckState(Qt::Checked);


                if(ui->view_2->item(j,0)->text() == "Power")
                {
                    win_power_2->setupUi(ProverkaList2);

                    QSqlQuery sql;

                    sql = BD->zapros("SELECT DISTINCT Power.Sootvetstvie,Power.PowerResult,Power.Volt,Power.Tok"
                              " FROM Proverka"
                              " JOIN Device ON Device.IdDevice=DeviceSerial.IdDevice"
                              " JOIN DeviceSerial ON Link.IdDevice == DeviceSerial.IdSerial"
                              " JOIN LInk ON Link.IdEtap == Etap.IdEtap"
                              " JOIN Etap ON Link.IdProverka == Proverka.IdProverka"
                              " JOIN Result ON Link.IdLink == Result.IdLink"
                              " JOIN Power ON Result.IdLink == Power.IdLink"
                              " Where Device.[Название устройства] = '"+PutBlok+"'"
                              " AND DeviceSerial.[Серийный номер] = '"+PutSerial+"'"
                              " AND Result.[Этап] = '"+PutEtap+"'"
                              " AND Etap.Etaps = '"+PutProverka+"'"
                              " AND Power.DataProverki = '"+PutData+"'"
                              " AND Result.IdLink = "+IdLink+""
                              " ORDER BY Power.[IdPower]");


                    win_power_2->lcdNumber_10V_Bt->display(sql.value(1).toDouble());
                    win_power_2->lcdNumber_10V_V->display(sql.value(2).toDouble());
                    win_power_2->lcdNumber_10V_I->display(sql.value(3).toDouble());

                    if(sql.value(0) == "Cоответствует")
                    {
                        win_power_2->pushButton_10V->setIcon(R2_Green);
                        win_power_2->pushButton_10V->setIconSize(win_power_2->pushButton_10V->size());
                    }
                    else
                    {
                        win_power_2->pushButton_10V->setIcon(R3_Red);
                        win_power_2->pushButton_10V->setIconSize(win_power_2->pushButton_10V->size());
                    }

                    sql.next();

                    win_power_2->lcdNumber_32V_Bt->display(sql.value(1).toDouble());
                    win_power_2->lcdNumber_32V_V->display(sql.value(2).toDouble());
                    win_power_2->lcdNumber_32V_I->display(sql.value(3).toDouble());

                    if(sql.value(0) == "Cоответствует")
                    {
                        win_power_2->pushButton_32V->setIcon(R2_Green);
                        win_power_2->pushButton_32V->setIconSize(win_power_2->pushButton_32V->size());
                    }
                    else
                    {
                        win_power_2->pushButton_32V->setIcon(R3_Red);
                        win_power_2->pushButton_32V->setIconSize(win_power_2->pushButton_32V->size());
                    }

                    sql.next();

                    win_power_2->lcdNumber_27V_Bt->display(sql.value(1).toDouble());
                    win_power_2->lcdNumber_27V_V->display(sql.value(2).toDouble());
                    win_power_2->lcdNumber_27V_I->display(sql.value(3).toDouble());

                    if(sql.value(0) == "Cоответствует")
                    {
                        win_power_2->pushButton_27V->setIcon(R2_Green);
                        win_power_2->pushButton_27V->setIconSize(win_power_2->pushButton_27V->size());
                    }
                    else
                    {
                        win_power_2->pushButton_27V->setIcon(R3_Red);
                        win_power_2->pushButton_27V->setIconSize(win_power_2->pushButton_27V->size());
                    }


                    ui->tabWidget_2->addTab(ProverkaList2,ui->view_2->item(j,0)->text());

                }

                if(ui->view_2->item(j,0)->text() == "Frequency")
                {

                   // win_frequency->setupUi(ProverkaList3);

                    ui->tabWidget_2->addTab(ProverkaList3,ui->view_2->item(j,0)->text());

                }

                if(ui->view_2->item(j,0)->text() == "TransferCoefficient")
                {

                  //  win_transferCoefficient->setupUi(ProverkaList4);

                    ui->tabWidget_2->addTab(ProverkaList4,ui->view_2->item(j,0)->text());

                }

                if(ui->view_2->item(j,0)->text() == "SwitchingSignalSM")
                {

                    win_switchingSignalSM_2->setupUi(ProverkaList5);

                    QSqlQuery sql ;


                    sql = BD->zapros("SELECT Result.IdLink, [Имя проверки]"
                              " FROM Proverka"
                              " JOIN Device ON Device.IdDevice=DeviceSerial.IdDevice"
                              " JOIN DeviceSerial ON Link.IdDevice == DeviceSerial.IdSerial"
                              " JOIN LInk ON Link.IdEtap == Etap.IdEtap"
                              " JOIN Etap ON Link.IdProverka == Proverka.IdProverka"
                              " JOIN Result ON Link.IdLink == Result.IdLink"
                              " Where Device.[Название устройства] = '"+PutBlok+"'"
                              " AND DeviceSerial.[Серийный номер] = '"+PutSerial+"'"
                              " AND Result.[Этап] = '"+PutEtap+"'"
                              " AND Etap.Etaps = '"+PutProverka+"'"
                              " AND Result.Date = '"+PutData+"'"
                              " AND Proverka.[Имя проверки] = 'SwitchingSignalSM'");



                    sql = BD->zapros("SELECT DISTINCT GraphPoint.NumberGraph,GraphPoint.Y,GraphPoint.[Вход],GraphPoint.[Выход]"
                              " FROM Proverka"
                              " JOIN Device ON Device.IdDevice=DeviceSerial.IdDevice"
                              " JOIN DeviceSerial ON Link.IdDevice == DeviceSerial.IdSerial"
                              " JOIN LInk ON Link.IdEtap == Etap.IdEtap"
                              " JOIN Etap ON Link.IdProverka == Proverka.IdProverka"
                              " JOIN Result ON Link.IdLink == Result.IdLink"
                              " JOIN GraphPoint ON Result.IdLink == GraphPoint.IdResult"
                              " Where Device.[Название устройства] = '"+PutBlok+"'"
                              " AND DeviceSerial.[Серийный номер] = '"+PutSerial+"'"
                              " AND Result.[Этап] = '"+PutEtap+"'"
                              " AND Etap.Etaps = '"+PutProverka+"'"
                              " AND GraphPoint.DataProverki = '"+PutData+"'"
                              " AND Result.IdLink = "+sql.value(0).toString()+""
                              " ORDER BY GraphPoint.[IdGraph]");


                    win_switchingSignalSM_2->lcdNumber->display(sql.value(1).toDouble());

                    if(sql.value(1).toDouble() < -20)
                    {
                        win_switchingSignalSM_2->pushButton_1->setIcon(R3_Red);
                        win_switchingSignalSM_2->pushButton_1->setIconSize(win_switchingSignalSM_2->pushButton_1->size());
                    }
                    else
                    {
                        win_switchingSignalSM_2->pushButton_1->setIcon(R2_Green);
                        win_switchingSignalSM_2->pushButton_1->setIconSize(win_switchingSignalSM_2->pushButton_1->size());
                    }

                    sql.next();

                    win_switchingSignalSM_2->lcdNumber_2->display(sql.value(1).toDouble());

                    if(sql.value(1).toDouble() < -20)
                    {
                        win_switchingSignalSM_2->pushButton_2->setIcon(R3_Red);
                        win_switchingSignalSM_2->pushButton_2->setIconSize(win_switchingSignalSM_2->pushButton_2->size());
                    }
                    else
                    {
                        win_switchingSignalSM_2->pushButton_2->setIcon(R2_Green);
                        win_switchingSignalSM_2->pushButton_2->setIconSize(win_switchingSignalSM_2->pushButton_2->size());
                    }

                    sql.next();

                    win_switchingSignalSM_2->lcdNumber_3->display(sql.value(1).toDouble());

                    if(sql.value(1).toDouble() < -20)
                    {
                        win_switchingSignalSM_2->pushButton_3->setIcon(R3_Red);
                        win_switchingSignalSM_2->pushButton_3->setIconSize(win_switchingSignalSM_2->pushButton_3->size());
                    }
                    else
                    {
                        win_switchingSignalSM_2->pushButton_3->setIcon(R2_Green);
                        win_switchingSignalSM_2->pushButton_3->setIconSize(win_switchingSignalSM_2->pushButton_3->size());
                    }

                    sql.next();

                    win_switchingSignalSM_2->lcdNumber_4->display(sql.value(1).toDouble());

                    if(sql.value(1).toDouble() < -20)
                    {
                        win_switchingSignalSM_2->pushButton_4->setIcon(R3_Red);
                        win_switchingSignalSM_2->pushButton_4->setIconSize(win_switchingSignalSM_2->pushButton_4->size());
                    }
                    else
                    {
                        win_switchingSignalSM_2->pushButton_4->setIcon(R2_Green);
                        win_switchingSignalSM_2->pushButton_4->setIconSize(win_switchingSignalSM_2->pushButton_4->size());
                    }


                    ui->tabWidget_2->addTab(ProverkaList5,ui->view_2->item(j,0)->text());

                }

                if(ui->view_2->item(j,0)->text() == "SwitchingSignalDM") //Другое нужно
                {

                    win_switchingSignalDM_2->setupUi(ProverkaList5);

                    QSqlQuery sql;


                    sql = BD->zapros("SELECT Result.IdLink, [Имя проверки]"
                              " FROM Proverka"
                              " JOIN Device ON Device.IdDevice=DeviceSerial.IdDevice"
                              " JOIN DeviceSerial ON Link.IdDevice == DeviceSerial.IdSerial"
                              " JOIN LInk ON Link.IdEtap == Etap.IdEtap"
                              " JOIN Etap ON Link.IdProverka == Proverka.IdProverka"
                              " JOIN Result ON Link.IdLink == Result.IdLink"
                              " Where Device.[Название устройства] = '"+PutBlok+"'"
                              " AND DeviceSerial.[Серийный номер] = '"+PutSerial+"'"
                              " AND Result.[Этап] = '"+PutEtap+"'"
                              " AND Etap.Etaps = '"+PutProverka+"'"
                              " AND Result.Date = '"+PutData+"'"
                              " AND Proverka.[Имя проверки] = 'SwitchingSignalDM'");


                    sql = BD->zapros("SELECT DISTINCT GraphPoint.NumberGraph,GraphPoint.Y,GraphPoint.[Вход],GraphPoint.[Выход]"
                              " FROM Proverka"
                              " JOIN Device ON Device.IdDevice=DeviceSerial.IdDevice"
                              " JOIN DeviceSerial ON Link.IdDevice == DeviceSerial.IdSerial"
                              " JOIN LInk ON Link.IdEtap == Etap.IdEtap"
                              " JOIN Etap ON Link.IdProverka == Proverka.IdProverka"
                              " JOIN Result ON Link.IdLink == Result.IdLink"
                              " JOIN GraphPoint ON Result.IdLink == GraphPoint.IdResult"
                              " Where Device.[Название устройства] = '"+PutBlok+"'"
                              " AND DeviceSerial.[Серийный номер] = '"+PutSerial+"'"
                              " AND Result.[Этап] = '"+PutEtap+"'"
                              " AND Etap.Etaps = '"+PutProverka+"'"
                              " AND GraphPoint.DataProverki = '"+PutData+"'"
                              " AND Result.IdLink = "+sql.value(0).toString()+""
                              " ORDER BY GraphPoint.[IdGraph]");


                    win_switchingSignalDM_2->lcdNumber->display(sql.value(1).toDouble());

                    if(sql.value(1).toDouble() < -20)
                    {
                        win_switchingSignalDM_2->pushButton_1->setIcon(R3_Red);
                        win_switchingSignalDM_2->pushButton_1->setIconSize(win_switchingSignalDM_2->pushButton_1->size());
                    }
                    else
                    {
                        win_switchingSignalDM_2->pushButton_1->setIcon(R2_Green);
                        win_switchingSignalDM_2->pushButton_1->setIconSize(win_switchingSignalDM_2->pushButton_1->size());
                    }

                    sql.next();

                    win_switchingSignalDM_2->lcdNumber_2->display(sql.value(1).toDouble());

                    if(sql.value(1).toDouble() < -20)
                    {
                        win_switchingSignalDM_2->pushButton_2->setIcon(R3_Red);
                        win_switchingSignalDM_2->pushButton_2->setIconSize(win_switchingSignalDM_2->pushButton_1->size());
                    }
                    else
                    {
                        win_switchingSignalDM_2->pushButton_2->setIcon(R2_Green);
                        win_switchingSignalDM_2->pushButton_2->setIconSize(win_switchingSignalDM_2->pushButton_1->size());
                    }

                    sql.next();

                    win_switchingSignalDM_2->lcdNumber_3->display(sql.value(1).toDouble());

                    if(sql.value(1).toDouble() < -20)
                    {
                        win_switchingSignalDM_2->pushButton_3->setIcon(R3_Red);
                        win_switchingSignalDM_2->pushButton_3->setIconSize(win_switchingSignalDM_2->pushButton_3->size());
                    }
                    else
                    {
                        win_switchingSignalDM_2->pushButton_3->setIcon(R2_Green);
                        win_switchingSignalDM_2->pushButton_3->setIconSize(win_switchingSignalDM_2->pushButton_3->size());
                    }

                    sql.next();

                    win_switchingSignalDM_2->lcdNumber_4->display(sql.value(1).toDouble());

                    if(sql.value(1).toDouble() < -20)
                    {
                        win_switchingSignalDM_2->pushButton_4->setIcon(R3_Red);
                        win_switchingSignalDM_2->pushButton_4->setIconSize(win_switchingSignalDM_2->pushButton_4->size());
                    }
                    else
                    {
                        win_switchingSignalDM_2->pushButton_4->setIcon(R2_Green);
                        win_switchingSignalDM_2->pushButton_4->setIconSize(win_switchingSignalDM_2->pushButton_4->size());
                    }

                    ui->tabWidget_2->addTab(ProverkaList5,ui->view_2->item(j,0)->text());

                }

                if(ui->view_2->item(j,0)->text() == "SwitchingSignalSMDM") //Другое нужно
                {

                    win_switchingSignalSMDM_2->setupUi(ProverkaList5);

                    QSqlQuery sql;


                    sql = BD->zapros("SELECT Result.IdLink, [Имя проверки]"
                              " FROM Proverka"
                              " JOIN Device ON Device.IdDevice=DeviceSerial.IdDevice"
                              " JOIN DeviceSerial ON Link.IdDevice == DeviceSerial.IdSerial"
                              " JOIN LInk ON Link.IdEtap == Etap.IdEtap"
                              " JOIN Etap ON Link.IdProverka == Proverka.IdProverka"
                              " JOIN Result ON Link.IdLink == Result.IdLink"
                              " Where Device.[Название устройства] = '"+PutBlok+"'"
                              " AND DeviceSerial.[Серийный номер] = '"+PutSerial+"'"
                              " AND Result.[Этап] = '"+PutEtap+"'"
                              " AND Etap.Etaps = '"+PutProverka+"'"
                              " AND Result.Date = '"+PutData+"'"
                              " AND Proverka.[Имя проверки] = 'SwitchingSignalSMDM'");



                    sql = BD->zapros("SELECT DISTINCT GraphPoint.NumberGraph,GraphPoint.Y,GraphPoint.[Вход],GraphPoint.[Выход]"
                              " FROM Proverka"
                              " JOIN Device ON Device.IdDevice=DeviceSerial.IdDevice"
                              " JOIN DeviceSerial ON Link.IdDevice == DeviceSerial.IdSerial"
                              " JOIN LInk ON Link.IdEtap == Etap.IdEtap"
                              " JOIN Etap ON Link.IdProverka == Proverka.IdProverka"
                              " JOIN Result ON Link.IdLink == Result.IdLink"
                              " JOIN GraphPoint ON Result.IdLink == GraphPoint.IdResult"
                              " Where Device.[Название устройства] = '"+PutBlok+"'"
                              " AND DeviceSerial.[Серийный номер] = '"+PutSerial+"'"
                              " AND Result.[Этап] = '"+PutEtap+"'"
                              " AND Etap.Etaps = '"+PutProverka+"'"
                              " AND GraphPoint.DataProverki = '"+PutData+"'"
                              " AND Result.IdLink = "+sql.value(0).toString()+""
                              " ORDER BY GraphPoint.[IdGraph]");



                    win_switchingSignalSMDM_2->lcdNumber_PRD->display(sql.value(1).toDouble());


                    if(sql.value(1).toDouble() < -20)
                    {
                        win_switchingSignalSMDM_2->pushButton_PRD_1->setIcon(R3_Red);
                        win_switchingSignalSMDM_2->pushButton_PRD_1->setIconSize(win_switchingSignalSMDM_2->pushButton_PRD_1->size());
                    }
                    else
                    {
                        win_switchingSignalSMDM_2->pushButton_PRD_1->setIcon(R2_Green);
                        win_switchingSignalSMDM_2->pushButton_PRD_1->setIconSize(win_switchingSignalSMDM_2->pushButton_PRD_1->size());
                    }

                    sql.next();

                    win_switchingSignalSMDM_2->lcdNumber_PRD_2->display(sql.value(1).toDouble());

                    if(sql.value(1).toDouble() < -20)
                    {
                        win_switchingSignalSMDM_2->pushButton_PRD_2->setIcon(R3_Red);
                        win_switchingSignalSMDM_2->pushButton_PRD_2->setIconSize(win_switchingSignalSMDM_2->pushButton_PRD_2->size());

                    }
                    else
                    {
                        win_switchingSignalSMDM_2->pushButton_PRD_2->setIcon(R2_Green);
                        win_switchingSignalSMDM_2->pushButton_PRD_2->setIconSize(win_switchingSignalSMDM_2->pushButton_PRD_2->size());

                    }

                    sql.next();

                    win_switchingSignalSMDM_2->lcdNumber_PRD_3->display(sql.value(1).toDouble());

                    if(sql.value(1).toDouble() < -20)
                    {
                        win_switchingSignalSMDM_2->pushButton_PRD_3->setIcon(R3_Red);
                        win_switchingSignalSMDM_2->pushButton_PRD_3->setIconSize(win_switchingSignalSMDM_2->pushButton_PRD_3->size());

                    }
                    else
                    {
                        win_switchingSignalSMDM_2->pushButton_PRD_3->setIcon(R2_Green);
                        win_switchingSignalSMDM_2->pushButton_PRD_3->setIconSize(win_switchingSignalSMDM_2->pushButton_PRD_3->size());

                    }

                    sql.next();

                    win_switchingSignalSMDM_2->lcdNumber_PRD_4->display(sql.value(1).toDouble());

                    if(sql.value(1).toDouble() < -20)
                    {
                        win_switchingSignalSMDM_2->pushButton_PRD_4->setIcon(R3_Red);
                        win_switchingSignalSMDM_2->pushButton_PRD_4->setIconSize(win_switchingSignalSMDM_2->pushButton_PRD_4->size());

                    }
                    else
                    {
                        win_switchingSignalSMDM_2->pushButton_PRD_4->setIcon(R2_Green);
                        win_switchingSignalSMDM_2->pushButton_PRD_4->setIconSize(win_switchingSignalSMDM_2->pushButton_PRD_4->size());
                    }

                    sql.next();

                    win_switchingSignalSMDM_2->lcdNumber_PRM->display(sql.value(1).toDouble());

                    if(sql.value(1).toDouble() < -20)
                    {
                        win_switchingSignalSMDM_2->pushButton_PRM_1->setIcon(R3_Red);
                        win_switchingSignalSMDM_2->pushButton_PRM_1->setIconSize(win_switchingSignalSMDM_2->pushButton_PRM_1->size());

                    }
                    else
                    {
                        win_switchingSignalSMDM_2->pushButton_PRM_1->setIcon(R2_Green);
                        win_switchingSignalSMDM_2->pushButton_PRM_1->setIconSize(win_switchingSignalSMDM_2->pushButton_PRM_1->size());
                    }

                    sql.next();

                    win_switchingSignalSMDM_2->lcdNumber_PRM_2->display(sql.value(1).toDouble());

                    if(sql.value(1).toDouble() < -20)
                    {

                        win_switchingSignalSMDM_2->pushButton_PRM_2->setIcon(R3_Red);
                        win_switchingSignalSMDM_2->pushButton_PRM_2->setIconSize(win_switchingSignalSMDM_2->pushButton_PRM_2->size());
                    }
                    else
                    {
                        win_switchingSignalSMDM_2->pushButton_PRM_2->setIcon(R2_Green);
                        win_switchingSignalSMDM_2->pushButton_PRM_2->setIconSize(win_switchingSignalSMDM_2->pushButton_PRM_2->size());
                    }

                    sql.next();

                    win_switchingSignalSMDM_2->lcdNumber_PRM_3->display(sql.value(1).toDouble());

                    if(sql.value(1).toDouble() < -20)
                    {
                        win_switchingSignalSMDM_2->pushButton_PRM_3->setIcon(R3_Red);
                        win_switchingSignalSMDM_2->pushButton_PRM_3->setIconSize(win_switchingSignalSMDM_2->pushButton_PRM_3->size());
                    }
                    else
                    {
                        win_switchingSignalSMDM_2->pushButton_PRM_3->setIcon(R2_Green);
                        win_switchingSignalSMDM_2->pushButton_PRM_3->setIconSize(win_switchingSignalSMDM_2->pushButton_PRM_3->size());
                    }

                    sql.next();

                    win_switchingSignalSMDM_2->lcdNumber_PRM_4->display(sql.value(1).toDouble());

                    if(sql.value(1).toDouble() < -20)
                    {
                        win_switchingSignalSMDM_2->pushButton_PRM_4->setIcon(R3_Red);
                        win_switchingSignalSMDM_2->pushButton_PRM_4->setIconSize(win_switchingSignalSMDM_2->pushButton_PRM_4->size());
                    }
                    else
                    {
                        win_switchingSignalSMDM_2->pushButton_PRM_4->setIcon(R2_Green);
                        win_switchingSignalSMDM_2->pushButton_PRM_4->setIconSize(win_switchingSignalSMDM_2->pushButton_PRM_4->size());
                    }


                    ui->tabWidget_2->addTab(ProverkaList5,ui->view_2->item(j,0)->text());

                }

                ui->view_2->item(j,0)->setBackground(QBrush(QColor(158, 207, 207, 20)));
            }


        }
    }

    ui->view_2->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->view_2->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

      }

}


//Функция обработки нажатия на чекбокс  в таблице результатов для отображения графика
void MainWindow::ClickCheckBoxGraf_winReuslt(int r,int c)
{
    int point =0;


    if( win_frequency->tableWidgetAChH->item(r,c)->flags() != Qt::NoItemFlags)
    {

        if(win_frequency->tableWidgetAChH->item(r,c)->checkState()==Qt::Checked)
        {

            point =0;
            for(int i=0;i < win_frequency->tableWidgetAChH->columnCount();i++)
            {
                for(int j=0;j < win_frequency->tableWidgetAChH->rowCount();j++)
                {
                    win_frequency->ProverkaGraph->graph(point)->clearData();
                    if(point <64)
                    {
                        if(i == c && j == r)
                        {
                            win_frequency->tableWidgetAChH->item(j,i)->setCheckState(Qt::Checked);
                        }
                        else
                        {
                            win_frequency->tableWidgetAChH->item(j,i)->setCheckState(Qt::Unchecked);
                        }
                        point++;
                    }
                }
            }
            win_frequency->tableWidgetAChH->reset();
            point =  PoiskGraph_2(r,c);

            win_frequency->ProverkaGraph->xAxis->setRange(ListX[point-1].first()-50,ListX[point-1].last()+50);
             win_frequency->ProverkaGraph->yAxis->setRange(-10,2);


            win_frequency->ProverkaGraph->graph(point)->setData(ListX[point-1],ListY[point-1]);

            win_frequency->ProverkaGraph->clearItems();

            SetMinMaxGraphResult(ListMin[point-1].last(),ListMax[point-1].last(),ListNEravn[point-1].last(),ListX[point-1],ListY[point-1]);
            win_frequency->ProverkaGraph->replot();

        }

        if(win_frequency->tableWidgetAChH->item(r,c)->checkState()==Qt::Unchecked)
        {
            win_frequency->ProverkaGraph->clearItems();

            for(int i=0;i <  win_frequency->ProverkaGraph->graphCount();i++)
            {
                win_frequency->ProverkaGraph->graph(i)->clearData();
            }

        }
        win_frequency->ProverkaGraph->replot();
    }
}

int PoiskX1(double y,QVector<double> list_y,QVector<double> list_x)
{
    for(int i=0; i < list_y.count();i++)
    {
        if(y == list_y[i])
        {
            return list_x[i];
        }
    }
}

//q - Это либо Min либо Max , а flag_max -флаг для максимума
void MainWindow::SetMinMaxGraphResult(double min,double max, double Neravnomernost,QVector<double> x,QVector<double> y)
{
    //Min
   win_frequency->ProverkaGraph->clearItems();
   textLabel_Min = new QCPItemText(win_frequency->ProverkaGraph);
   win_frequency->ProverkaGraph->addItem(textLabel_Min);
   arrow_Min = new QCPItemLine(win_frequency->ProverkaGraph);
   win_frequency->ProverkaGraph->addItem(arrow_Min);
   textLabel_Min->setPositionAlignment(Qt::AlignTop|Qt::AlignCenter);
   textLabel_Min->position->setCoords(1800,1);
   arrow_Min->start->setParentAnchor(textLabel_Min->bottom);
   textLabel_Min->position->setType(QCPItemPosition::ptAxisRectRatio);
   textLabel_Min->setText("Min: "+QString::number(min)+" дБ");
   textLabel_Min->setFont(QFont(font().family(),10));
   textLabel_Min->setPen(QPen(Qt::black));
   arrow_Min->end->setCoords(PoiskX1(min,y,x),min);
   arrow_Min->setHead(QCPLineEnding::esSpikeArrow);

    //Мах
   textLabel = new QCPItemText(win_frequency->ProverkaGraph);
   win_frequency->ProverkaGraph->addItem(textLabel);
   arrow = new QCPItemLine(win_frequency->ProverkaGraph);
   win_frequency->ProverkaGraph->addItem(arrow);
   textLabel->setPositionAlignment(Qt::AlignTop|Qt::AlignCenter);
   textLabel->position->setCoords(1300,1);
   arrow->start->setParentAnchor(textLabel->bottom);
   textLabel->position->setType(QCPItemPosition::ptAxisRectRatio);
   textLabel->setText("Max: "+QString::number(max)+" дБ");
   textLabel->setFont(QFont(font().family(),10));
   textLabel->setPen(QPen(Qt::black));
   arrow->end->setCoords(PoiskX1(max,y,x),max);
   arrow->setHead(QCPLineEnding::esSpikeArrow);

    //Неравномерность
   textLabel_Neravnomernost = new QCPItemText(win_frequency->ProverkaGraph);
   win_frequency->ProverkaGraph->addItem(textLabel_Neravnomernost);
   textLabel_Neravnomernost->setPositionAlignment(Qt::AlignTop|Qt::AlignCenter);
   textLabel_Neravnomernost->position->setCoords(1500,1.5);
   textLabel_Neravnomernost->position->setType(QCPItemPosition::ptAxisRectRatio);
   textLabel_Neravnomernost->setText("Неравномерность: "+QString::number(Neravnomernost)+" дБ");
   textLabel_Neravnomernost->setFont(QFont(font().family(),10));
   textLabel_Neravnomernost->setPen(QPen(Qt::black));

}

void MainWindow::CreateGraphFrenq()
{

}


//Функция обработки нажатия на чекбокс  в таблице результатов для отображения графика
void MainWindow::ClickCheckBoxGraf_winReuslt2(int r,int c)
{
    int point =0;


    if( win_transferCoefficient->tableWidgetPerestrouka->item(r,c)->flags() != Qt::NoItemFlags)
    {

        if(win_transferCoefficient->tableWidgetPerestrouka->item(r,c)->checkState()==Qt::Checked)
        {

            point =0;

            for(int i=0;i < win_transferCoefficient->tableWidgetPerestrouka->columnCount();i++)
            {
                for(int j=0;j < win_transferCoefficient->tableWidgetPerestrouka->rowCount();j++)
                {
                    win_transferCoefficient->Graph->graph(point)->clearData();
                    if(i == c && j == r)
                    {
                        win_transferCoefficient->tableWidgetPerestrouka->item(j,i)->setCheckState(Qt::Checked);
                    }
                    else
                    {
                        win_transferCoefficient->tableWidgetPerestrouka->item(j,i)->setCheckState(Qt::Unchecked);
                    }
                    point++;

                }
            }
            win_transferCoefficient->tableWidgetPerestrouka->reset();
            point =  PoiskGraph2_2(r,c);

            win_transferCoefficient->Graph->xAxis->setRange(-12,12);
            win_transferCoefficient->Graph->yAxis->setRange(-30,14);


            win_transferCoefficient->Graph->graph(point)->setData(PerestroykaX[point],ListY1[point]);

            win_transferCoefficient->Graph->replot();

        }

        if(win_transferCoefficient->tableWidgetPerestrouka->item(r,c)->checkState()==Qt::Unchecked)
        {
            win_transferCoefficient->Graph->clearItems();

            for(int i=0;i < win_transferCoefficient->Graph->graphCount();i++)
            {
                win_transferCoefficient->Graph->graph(i)->clearData();
            }
        }
        win_transferCoefficient->Graph->replot();
    }
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Функции для поиска графиков в только что сделанной проверке

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Функция поиска графика по выбранному чекбоксу АЧХ
int MainWindow::PoiskGraph(int r,int c)
{
    int point =0;

    for(int i=0;i<win_frequency->tableWidgetAChH->columnCount();i++)
    {

        for(int j=0;j<win_frequency->tableWidgetAChH->rowCount();j++)
        {
            point++;
            if(j==r && i==c)
            {
                return point;
                break;
            }

        }
    }
}

//Функция поиска графика по выбранному чекбоксу АЧХ
int MainWindow::PoiskGraph_2(int r,int c)
{
    int point =0;

    for(int i=0;i<win_frequency->tableWidgetAChH->columnCount();i++)
    {

        for(int j=0;j<win_frequency->tableWidgetAChH->rowCount();j++)
        {
            point++;
            if(j==r && i==c)
            {
                return point;
                break;
            }

        }
    }
}

//Функция поиска графика по выбранному чекбоксу Перестройка
int MainWindow::PoiskGraph2_2(int r,int c)
{
    int point =0;

    for(int i=0;i<win_transferCoefficient->tableWidgetPerestrouka->columnCount();i++)
    {

        for(int j=0;j<win_transferCoefficient->tableWidgetPerestrouka->rowCount();j++)
        {
            if(j==r && i==c)
            {
                return point;
                break;
            }
            point++;
        }
    }
}
