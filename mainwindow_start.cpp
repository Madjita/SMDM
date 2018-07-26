#include <mainwindow.h>
#include "ui_mainwindow.h"

void MainWindow::DisebleElements()
{

    Button_StartProverka->setDisabled(1);
    GroupBox_RadioButton->setDisabled(1);
    EtapGroup_Device->setDisabled(1);
    EtapGroup->setDisabled(1);
    NDevice_ComboBox->setDisabled(1);
    Button_EndOpros->setDisabled(1);
    ui->view->setDisabled(1);
    win_frequency->tableWidgetAChH->setDisabled(1);
    win_transferCoefficient->tableWidgetPerestrouka->setDisabled(1);

}

//Добавление графиков
void MainWindow::MyAddGraph(Ui::Frequency* win_frequency, Ui::TransferCoefficient* win_transferCoefficient)
{
    win_frequency->ProverkaGraph->clearGraphs();
    win_transferCoefficient->Graph->clearGraphs();
    int point =0;

    for(int i=0;i<13;i++)
    {
        for(int j=0;j<5;j++)
        {
            // point++; изменил
            win_frequency->ProverkaGraph->addGraph();
            win_frequency->ProverkaGraph->graph(point)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
            win_frequency->ProverkaGraph->graph(point)->setPen(QPen(qrand())); // line color blue for first graph
            win_frequency->ProverkaGraph->graph(point)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue
            point++;
        }
    }

    point =0;

    for(int i=0;i<4;i++)
    {
        for(int j=0;j<2;j++)
        {
            // point++; изменил
            win_transferCoefficient->Graph->addGraph();
            win_transferCoefficient->Graph->graph(point)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
            win_transferCoefficient->Graph->graph(point)->setPen(QPen(qrand())); // line color blue for first graph
            win_transferCoefficient->Graph->graph(point)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue
            point++;
        }
    }
}


void MainWindow::Proverka_Start()
{
    if(EtapGroup_PSI_RadioButton->isChecked() == true)
    {
        log_wr_message("Проверка блока: "+ EtapGroup_Device->currentText()+" на этапе: ПСИ ->"+EtapGroup->currentText()+"\n");
    }
    if(EtapGroup_PI_RadioButton->isChecked() == true)
    {
        log_wr_message("Проверка блока: "+ EtapGroup_Device->currentText()+" на этапе: ПИ ->"+EtapGroup->currentText()+"\n");
    }
    if(EtapGroup_Regylirovka_RadioButton->isChecked() == true)
    {
        log_wr_message("Проверка блока: "+ EtapGroup_Device->currentText()+" на этапе: Регулировка ->"+EtapGroup->currentText()+"\n");
    }



    QSqlQueryModel *SQL_FindIdLink = nullptr;

    SQL_FindIdLink = BD->zaprosQueryModel("SELECT [IdLink]"
                             " FROM Proverka"
                             " JOIN Device ON Device.IdDevice=DeviceSerial.IdDevice"
                             " JOIN DeviceSerial ON Link.IdDevice == DeviceSerial.IdSerial"
                             " JOIN LInk ON Link.IdEtap == Etap.IdEtap"
                             " JOIN Etap ON Link.IdProverka == Proverka.IdProverka"
                             " Where Device.[Название устройства] = '"+EtapGroup_Device->currentText()+"'"
                             " AND DeviceSerial.[Серийный номер] = '"+NDevice_ComboBox->currentText()+"'"
                             " AND Etap.Etaps = '"+EtapGroup->currentText()+"'");




    if(EtapGroup_Device->currentText() =="СМ16-4")
    {

        if(win_frequency->ProverkaGraph->graphCount()>64)
        {
            win_frequency->ProverkaGraph->clearGraphs();
            win_transferCoefficient->Graph->clearGraphs();
        }




        MyAddGraph(win_frequency,win_transferCoefficient);

        qDebug() << "Start ";

        threadSM->Set_log_text(log_text);




       // QObject::connect(this,&MainWindow::startSM,threadSM,&thread_SM::Work);


      //  threadSM->SetUi(win_power,win_frequency,win_transferCoefficient,win_switchingSignalSM);


//        QObject::connect(threadSM, &thread_SM::updateGraph, this, &MainWindow::UpdateGraph);


//         QObject::connect(threadSM, &thread_SM::updateGraphPerestrouyka, this, &MainWindow::UpdateGraphPerestrouyka);



//        QObject::connect(threadSM,SIGNAL(SetDataStart(QDateTime)), newLog, SLOT(SetDataStart(QDateTime)));

//        connect(threadSM,&thread_SM::end, this, &MainWindow::END);

//        QObject::connect(threadSM,&thread_SM::Log,this,&MainWindow::log_wr_message_Block);
//        QObject::connect(threadSM,&thread_SM::Log2,this,&MainWindow::log_wr_message_Block);

//        QObject::connect(threadSM,&thread_SM::StartFrequency,ui->tabWidget,&QTabWidget::setCurrentIndex);
//        QObject::connect(threadSM,&thread_SM::StartPower,ui->tabWidget,&QTabWidget::setCurrentIndex);
//        QObject::connect(threadSM,&thread_SM::StartPerestroyka,ui->tabWidget,&QTabWidget::setCurrentIndex);



//        QObject::connect(TP,&TP_SMDM::SetImage,this,&MainWindow::SetImage);
//        QObject::connect(TP,&TP_SMDM::SetImage2,this,&MainWindow::SetImage2);
//        QObject::connect(TP,&TP_SMDM::SetImage3,this,&MainWindow::SetImage3);
//        QObject::connect(TP,&TP_SMDM::SetImage4,this,&MainWindow::SetImage4);

//        QObject::connect(threadSM,&thread_SM::LogClear,this,&MainWindow::log_wr_message_Block_claer);

//        QObject::connect(threadSM,&thread_SM::CreateGraph,this,&MainWindow::CreateGraph2);

//        QObject::connect(threadSM,&thread_SM::CreateGraphfrenq,this,&MainWindow::CreateGraphFrenq,Qt::QueuedConnection);



//        QObject::connect(ui->view,SIGNAL(cellClicked(int,int)),threadSM,SLOT(SetProv(int,int)));

        threadSM->SetIdLink(SQL_FindIdLink);


        if(EtapGroup_PI_RadioButton->isChecked())
        {
            threadSM->SetEtap("ПИ");
        }

        if(EtapGroup_PSI_RadioButton->isChecked())
        {
            threadSM->SetEtap("ПСИ");
        }

        if(EtapGroup_Regylirovka_RadioButton->isChecked())
        {
            threadSM->SetEtap("Регулировка");
        }

        threadDM->flagEnd =false;
        threadSMDM->flagEnd = false;

        threadSM->flagEnd =true;

        threadSM->ListProv.clear();



        for(int i=0; i < ui->view->rowCount() ;i++)
        {
            if( ui->view->item(i,0)->flags() != Qt::NoItemFlags)
            {
                ui->view->item(i,0)->setBackground(QBrush(QColor(158, 207, 207, 20)));

                if(ui->view->item(i,0)->checkState()==Qt::Checked)
                {
                    threadSM->ListProv.append(true);
                }
                else
                {
                    threadSM->ListProv.append(false);
                }
            }
        }

        //timer->start(fps);





       emit startSM();

       DisebleElements();

    }

        if(EtapGroup_Device->currentText() =="ДМ4-16")
        {

            if(win_frequency->ProverkaGraph->graphCount()>64)
            {
                win_frequency->ProverkaGraph->clearGraphs();
            }

            MyAddGraph(win_frequency,win_transferCoefficient);

           // delete threadDM;

            threadDM = new thread_DM(Micran1,N9000,HMP2020,TP,ui->view);

            threadDM->setListRegyl(ListRegyl);

            //Перевели блок СМ в новый поток
           // potok2 = new MyPotok();

            threadDM->moveToThread(new QThread());

            QObject::connect(threadDM->thread(),&QThread::started,threadDM,&thread_DM::Work);

            connect(threadDM,&thread_DM::end, this, &MainWindow::END);


            threadDM->SetUi(win_power,win_frequency,win_transferCoefficient,win_switchingSignalDM);

            QObject::connect(threadDM,SIGNAL(SetDataStart(QDateTime)), newLog, SLOT(SetDataStart(QDateTime)));

            QObject::connect(threadDM,SIGNAL(Log(QString)),this,SLOT(log_wr_message_Block(QString)));
            QObject::connect(threadDM,SIGNAL(Log2(QString)),this,SLOT(log_wr_message_Block(QString)));

            QObject::connect(threadDM,SIGNAL(StartFrequency(int)),ui->tabWidget,SLOT(setCurrentIndex(int)));
            QObject::connect(threadDM,SIGNAL(StartPower(int)),ui->tabWidget,SLOT(setCurrentIndex(int)));
            QObject::connect(threadDM,SIGNAL(StartPerestroyka(int)),ui->tabWidget,SLOT(setCurrentIndex(int)));

            QObject::connect(TP,&TP_SMDM::SetImage,this,&MainWindow::SetImage);
            QObject::connect(TP,&TP_SMDM::SetImage2,this,&MainWindow::SetImage2);
            QObject::connect(TP,&TP_SMDM::SetImage3,this,&MainWindow::SetImage3);
            QObject::connect(TP,&TP_SMDM::SetImage4,this,&MainWindow::SetImage4);

            QObject::connect(threadDM->win_transferCoefficient->tableWidgetPerestrouka,SIGNAL(cellClicked(int,int)),this,SLOT(ClickCheckBoxGrafPerestrouka(int,int)));
            QObject::connect(threadDM->win_frequency->tableWidgetAChH,SIGNAL(cellClicked(int,int)),this,SLOT(ClickCheckBoxGrafAChH(int,int)));
            QObject::connect(threadDM,SIGNAL(LogClear()),this,SLOT(log_wr_message_Block_claer()));
            QObject::connect(threadDM,SIGNAL(CreateGraph()),this,SLOT(CreateGraph()));


            QObject::connect(threadDM, &thread_DM::updateGraph, this, &MainWindow::UpdateGraph);

            QObject::connect(threadDM, &thread_DM::updateGraphPerestrouyka, this, &MainWindow::UpdateGraphPerestrouyka);


        //    threadDM->win_frequency->ProverkaGraph->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
         //   threadDM->win_transferCoefficient->Graph->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

            threadDM->SetIdLink(SQL_FindIdLink);

            if(EtapGroup_PI_RadioButton->isChecked())
            {
                threadDM->SetEtap("ПИ");
            }

            if(EtapGroup_PSI_RadioButton->isChecked())
            {
                threadDM->SetEtap("ПСИ");
            }

            if(EtapGroup_Regylirovka_RadioButton->isChecked())
            {
                threadDM->SetEtap("Регулировка");
            }

            threadSM->flagEnd = false;

            threadSMDM->flagEnd = false;

            threadDM->flagEnd =true;

            threadDM->ListProv.clear();
            for(int i=0; i < ui->view->rowCount() ;i++)
            {
                if( ui->view->item(i,0)->flags() != Qt::NoItemFlags)
                {
                    ui->view->item(i,0)->setBackground(QBrush(QColor(158, 207, 207, 20)));

                    if(ui->view->item(i,0)->checkState()==Qt::Checked)
                    {
                        threadDM->ListProv.append(true);
                    }
                    else
                    {
                        threadDM->ListProv.append(false);
                    }
                }
            }


            //timer->start(fps);

            threadDM->thread()->start();

            DisebleElements();

        }

        if(EtapGroup_Device->currentText() =="СМДМ8-4")
        {

            if(win_frequency->ProverkaGraph->graphCount()>64)
            {
                win_frequency->ProverkaGraph->clearGraphs();
            }

            MyAddGraph(win_frequency,win_transferCoefficient);

           // delete threadSMDM;

            threadSMDM = new thread_SMDM(Micran1,N9000,HMP2020,TP,ui->view);

            threadSMDM->setListRegyl(ListRegyl);

            //Перевели блок СМ в новый поток
            //potok3 = new MyPotok();
           // threadSMDM->moveToThread(potok3);

            threadSMDM->moveToThread(new QThread());

            connect(threadSMDM->thread(),&QThread::started,threadSMDM,&thread_SMDM::Work);

            //QObject::connect(potok3,SIGNAL(started()),threadSMDM,SLOT(Work()));

            connect(threadSMDM,&thread_SMDM::end, this, &MainWindow::END);

            QObject::connect(threadSMDM,SIGNAL(SetDataStart(QDateTime)), newLog, SLOT(SetDataStart(QDateTime)));

            threadSMDM->SetUi(win_power,win_frequency,win_transferCoefficient,win_switchingSignalSMDM);
            QObject::connect(threadSMDM,SIGNAL(Log(QString)),this,SLOT(log_wr_message_Block(QString)));
            QObject::connect(threadSMDM,SIGNAL(Log2(QString)),this,SLOT(log_wr_message_Block(QString)));

            QObject::connect(threadSMDM,SIGNAL(StartFrequency(int)),ui->tabWidget,SLOT(setCurrentIndex(int)));
            QObject::connect(threadSMDM,SIGNAL(StartPower(int)),ui->tabWidget,SLOT(setCurrentIndex(int)));
            QObject::connect(threadSMDM,SIGNAL(StartPerestroyka(int)),ui->tabWidget,SLOT(setCurrentIndex(int)));

            QObject::connect(TP,&TP_SMDM::SetImage,this,&MainWindow::SetImage);
            QObject::connect(TP,&TP_SMDM::SetImage2,this,&MainWindow::SetImage2);
            QObject::connect(TP,&TP_SMDM::SetImage3,this,&MainWindow::SetImage3);
            QObject::connect(TP,&TP_SMDM::SetImage4,this,&MainWindow::SetImage4);

            QObject::connect(threadSMDM,SIGNAL(LogClear()),this,SLOT(log_wr_message_Block_claer()));
            QObject::connect(threadSMDM,SIGNAL(CreateGraph()),this,SLOT(CreateGraph3()));


            QObject::connect(threadSMDM, &thread_SMDM::updateGraph, this, &MainWindow::UpdateGraph);

            QObject::connect(threadSMDM, &thread_SMDM::updateGraphPerestrouyka, this, &MainWindow::UpdateGraphPerestrouyka);


            QObject::connect(threadSMDM->win_transferCoefficient->tableWidgetPerestrouka,SIGNAL(cellClicked(int,int)),this,SLOT(ClickCheckBoxGrafPerestrouka(int,int)));
            QObject::connect(threadSMDM->win_frequency->tableWidgetAChH,SIGNAL(cellClicked(int,int)),this,SLOT(ClickCheckBoxGrafAChH(int,int)));

          //  threadSMDM->win_frequency->ProverkaGraph->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
          //  threadSMDM->win_transferCoefficient->Graph->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);


            threadSMDM->SetIdLink(SQL_FindIdLink);

            if(EtapGroup_PI_RadioButton->isChecked())
            {
                threadSMDM->SetEtap("ПИ");
            }

            if(EtapGroup_PSI_RadioButton->isChecked())
            {
                threadSMDM->SetEtap("ПСИ");
            }

            if(EtapGroup_Regylirovka_RadioButton->isChecked())
            {
                threadSMDM->SetEtap("Регулировка");
            }

            threadSM->flagEnd = false;
            threadDM->flagEnd =false;

            threadSMDM->flagEnd = true;

            threadSMDM->ListProv.clear();
            for(int i=0; i < ui->view->rowCount() ;i++)
            {
                if( ui->view->item(i,0)->flags() != Qt::NoItemFlags)
                {
                    ui->view->item(i,0)->setBackground(QBrush(QColor(158, 207, 207, 20)));

                    if(ui->view->item(i,0)->checkState()==Qt::Checked)
                    {
                        threadSMDM->ListProv.append(true);
                    }
                    else
                    {
                        threadSMDM->ListProv.append(false);
                    }
                }
            }

            //timer->start(fps);

            threadSMDM->thread()->start();

            //potok3->start();

            DisebleElements();

        }

}

void MainWindow::END()
{


    qDebug () << "END ";

    emit Setlog_wr_message(log_text->toPlainText());
    emit Setlog_wr_message_Ist(log_text_Ist->toPlainText());
    emit Setlog_wr_message_Micran(log_text_Micran->toPlainText());
    emit Setlog_wr_message_TP(log_text_TP->toPlainText());
    emit Setlog_wr_message_Block("");
    emit Setlog_wr_message_N9000(log_text_N9000->toPlainText());

    qDebug () << "END MainWindow";

    Button_StartProverka->setDisabled(0);
    GroupBox_RadioButton->setDisabled(0);
    EtapGroup_Device->setDisabled(0);
    EtapGroup->setDisabled(0);
    NDevice_ComboBox->setDisabled(0);
    Button_EndOpros->setDisabled(0);
    ui->view->setDisabled(0);
    win_frequency->tableWidgetAChH->setDisabled(0);
    win_transferCoefficient->tableWidgetPerestrouka->setDisabled(0);
}
