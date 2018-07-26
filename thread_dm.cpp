#include "thread_dm.h"


#include <mainwindow.h>
#include <QString.h>
#include <QPalette>


thread_DM::thread_DM()
{

}

thread_DM::thread_DM(Micran_Gen*  Micran1Get,N9000A*   N9000Get,PowerSourse*  HMP2020Get,TP_SMDM* TP_SMDMGet,QTableWidget* viewGet):
    flagRegylK0(false),
    flagRegyl_A(false),
    flagRegyl_B(false),
    flagEnd(false)
{

    Micran1 = Micran1Get;

    N9000 = N9000Get;

    HMP2020 = HMP2020Get;

    TP = TP_SMDMGet;

    view = viewGet;


    GnFREQuencyStart = 950;
    GnFREQuencyEnd = 2150;
    GnFREQuencyStep = 50;
    GnPower    = 0;

    log = "";

    this->moveToThread(new QThread());

    connect(this->thread(),&QThread::started,this,&thread_DM::process_start);

    this->thread()->start();

}

void thread_DM::process_start()
{

}

void thread_DM::SetUi(  Ui::Power* win_powerGet,
                        Ui::Frequency* win_frequencyGet,
                        Ui::TransferCoefficient* win_transferCoefficientGet,
                        Ui::SwitchingSignalDM* win_switchingSignalDMGet)
{

    win_power = win_powerGet;
    win_frequency =win_frequencyGet;
    win_transferCoefficient =win_transferCoefficientGet;
    win_switchingSignalDM = win_switchingSignalDMGet;

}


void thread_DM::SetIdLink(QSqlQueryModel *LinkGet)
{
    Link = LinkGet;
}

void thread_DM::SetEtap(QString etapGet)
{
    etap = etapGet;
}

void thread_DM::setListRegyl(QVector<QVector<double>>* ListRegylGet)
{
    ListRegyl = ListRegylGet;
}

void thread_DM::MyClear()
{
    TP->LogClear();
    Micran1->LogClear();
    HMP2020->LogClear();
    N9000->LogClear();

    x.clear();
    y.clear();
    x1.clear();
    y1.clear();
    ListMin.clear();
    ListMax.clear();
    ListNEravn.clear();
    ListX.clear();
    ListY.clear();
    x10MGh.clear();
    y10MGh.clear();


    emit LogClear();

}


void thread_DM::Ysilenie(int ysilenie,int out)
{
    char a[18];

    for(int i=0;i<18;i++)
    {
        a[i] = NULL;
    }

    a[0]=0x64;
    a[2]=0x80;
    a[5]=0x12;
    a[6]=0x31;
    a[7]=0x44;
    a[8]=0x01;
    a[9]=0x02;
    a[14]=0xff;
    a[15]=0xff;
    a[16]=0xff;
    a[17]=0xff;

    for(int i=0;i<18;i++)
    {
        if(i==(out+9))
        {
            a[i]= ysilenie*2+20;
        }
    }


    p_udpSocketOut->writeDatagram(a,18,QHostAddress("10.12.0.4"),30021);
    emit Log("Установлен коэфициент усиления "+ QString::number(ysilenie) +" на выход "+QString::number(out) +".\n");
}

void thread_DM::Rele_Kom(int in,int out)
{
    if(in > 10)
    {
        in -=2;
    }
    char a[34];

    for(int i=0; i< 34;i++)
    {
        a[i]=NULL;
    }

    a[0]=0x65;
    a[2]=0x80;
    a[5]=0x23;
    a[6]=0x31;
    a[7]=0x44;
    a[8]=0x01;
    a[9]=0x01;


    for(int i=0; i< 34;i++)
    {
        if(i==(in+17))
        {
            switch (out)
            {
            case 1:
                a[i]=0x01;
                break;
            case 2:
                a[i]=0x02;
                break;
            case 3:
                a[i]=0x03;
                break;
            case 4:
                a[i]=0x04;
                break;
            default:
                break;
            }
        }
    }

    p_udpSocketOut->writeDatagram(a,34,QHostAddress("10.12.0.4"),30021);
    emit Log("Переключен реле коммутатора с  "+ QString::number(in) +" на выход "+QString::number(out) +".\n");

}

void thread_DM::Rele_Kom_10MGH(int out)
{
    char a[34];

    for(int i=0; i< 34;i++)
    {
        a[i]=NULL;
    }

    a[0]=0x64;
    a[2]=0x80;
    a[5]=0x23;
    a[6]=0x31;
    a[7]=0x44;
    a[8]=0x01;
    a[9]=0x01;


    for(int i=0; i< 34;i++)
    {
        if(i==(out+13))
        {
            switch (out)
            {
            case 1:
                a[i]=0x01;
                break;
            case 2:
                a[i]=0x01;
                break;
            case 3:
                a[i]=0x01;
                break;
            case 4:
                a[i]=0x01;
                break;
            default:
                break;
            }
        }
    }

    p_udpSocketOut->writeDatagram(a,34,QHostAddress("10.12.0.4"),30021);
    emit Log("Переключен реле коммутатора с  "+ QString::number(out) +" на 10МГЦ.\n");
}

void thread_DM::Work()
{



    flagRegylK0=false;
    flagRegyl_A =false;
    flagRegyl_B=false;


    flagEnd = true;
    flag_6_proverki = false;
    TP->flag_6_proverki=false;

    //TP->SetTp();

    MyClear();

    p_udpSocketOut = new QUdpSocket();
    p_udpSocketOut->bind(30021);

    dateStart = QDateTime::currentDateTime();

    SQL_OneProverka = new QSqlQueryModel();


    SQL_OneProverka->setQuery("INSERT INTO Log (Data,DataStart,IdResult,BlockName) VALUES('"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"','"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(0,0), Qt::EditRole).toString()+"','Источник питания')");
    SQL_OneProverka->setQuery("INSERT INTO Log (Data,DataStart,IdResult,BlockName) VALUES('"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"','"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(0,0), Qt::EditRole).toString()+"','Микран')");
    SQL_OneProverka->setQuery("INSERT INTO Log (Data,DataStart,IdResult,BlockName) VALUES('"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"','"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(0,0), Qt::EditRole).toString()+"','Пульт ТП-СМДМ')");
    SQL_OneProverka->setQuery("INSERT INTO Log (Data,DataStart,IdResult,BlockName) VALUES('"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"','"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(0,0), Qt::EditRole).toString()+"','Блок')");
    SQL_OneProverka->setQuery("INSERT INTO Log (Data,DataStart,IdResult,BlockName) VALUES('"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"','"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(0,0), Qt::EditRole).toString()+"','Анализатор')");
    SQL_OneProverka->setQuery("INSERT INTO Log (Data,DataStart,IdResult,BlockName) VALUES('"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"','"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(0,0), Qt::EditRole).toString()+"','Вывод')");

    emit SetDataStart(dateStart);


    if(ListProv.value(0)==true || ListProv.value(1)==true)
    {
        StartProverka_1_And_2();
    }
    if(ListProv.value(2)==true)
    {
        StartProverka3();
    }
    if(ListProv.value(3)==true)
    {
        StartProverka4();
        x.clear();
        y.clear();
    }
    if(ListProv.value(4)==true)
    {
        StartProverka5();
        PerestroykaX.clear();
        x1.clear();
        y1.clear();
    }
    if(ListProv.value(5)==true)
    {
        StartProverka6();
    }
    if(ListProv.value(6)==true)
    {
        StartProverka7();
    }
    if(ListProv.value(7)==true)
    {
        StartProveroka8();
    }
    END();

    flagEnd = false;

}

void thread_DM::StartProverka_1_And_2()
{
    QPixmap R1_Gray(":/images/circle_gray.png");
    QPixmap R2_Green(":/images/circle_zel.png");
    QPixmap R3_Red(":/images/circle_kras.png");

    HMP2020->Log("=================\n");
    HMP2020->Log("Старт 1 и 2 проверки.\n");
    TP->Log2("Старт 1 и 2 проверки.\n");

    emit StartPower(0);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //1 и 2  Проверка дописать условие при котором будут опрашиваться комутатор!
    char Tok[6];
    char Volt[6];
    bool flagGood = true;

    view->item(0,0)->setBackgroundColor(Qt::yellow);
    view->item(1,0)->setBackgroundColor(Qt::yellow);
    view->reset();

    viPrintf(HMP2020->vi, "VOLTage 10 V\r\n");
    HMP2020->Log("Установить Напряжение: 10 V.\n");
    viPrintf(HMP2020->vi, "OUTPut 1\r\n");
    HMP2020->Log("Включить подачу напряжения.\n");

    Sleep(1000);
    viQueryf(HMP2020->vi, "MEASure:CURRent?\n", "%T", Tok);
    HMP2020->Log("Измеренный Ток: "+QString(Tok));

    double P = QString(Tok).toDouble();

    viQueryf(HMP2020->vi, "MEASure:VOLTage?\n", "%T", Volt);
    HMP2020->Log("Измеренное Напряжение: "+QString(Volt));

    P*=QString(Volt).toDouble();

    HMP2020->Log("Посчитанная Мощнасть: "+QString::number(P)+"\n");

    win_power->lcdNumber_10V_I->display(QString(Tok).toDouble());
    win_power->lcdNumber_10V_V->display(QString(Volt).toDouble());
    win_power->lcdNumber_10V_Bt->display(P);

    if(P > 35)
    {
        SQL_OneProverka->setQuery("INSERT INTO Power (Data,DataProverki,IdLink,PowerResult,Volt,Tok,Sootvetstvie) VALUES('"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"','"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(1,0), Qt::EditRole).toString()+"','"+QString::number(P)+"','"+QString::number(win_power->lcdNumber_10V_V->value())+"','"+QString::number(win_power->lcdNumber_10V_I->value())+"','Не соответствует')");

        view->item(0,0)->setBackgroundColor(Qt::red);
        view->item(1,0)->setBackgroundColor(Qt::red);
        HMP2020->Log("Напряжение 10 V: Несоответствует\n");
        HMP2020->Log2("Напряжение 10 V: Несоответствует\n");

        win_power->pushButton_10V->setIcon(R3_Red);
        view->reset();

        flagGood = false;
    }
    else
    {
        SQL_OneProverka->setQuery("INSERT INTO Power (Data,DataProverki,IdLink,PowerResult,Volt,Tok,Sootvetstvie) VALUES('"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"','"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(1,0), Qt::EditRole).toString()+"','"+QString::number(P)+"','"+QString::number(win_power->lcdNumber_10V_V->value())+"','"+QString::number(win_power->lcdNumber_10V_I->value())+"','Cоответствует')");


        view->item(0,0)->setBackgroundColor(Qt::green);
        view->item(1,0)->setBackgroundColor(Qt::green);
        HMP2020->Log("Напряжение 10 V: Соответствует\n");
        HMP2020->Log2("Напряжение 10 V: Соответствует\n");

        win_power->pushButton_10V->setIcon(R2_Green);
        view->reset();
    }

    viPrintf(HMP2020->vi, "OUTPut 0\r\n");
    HMP2020->Log("Выключить подачу напряжения.\n");

    HMP2020->Log("=================\n");

    viPrintf(HMP2020->vi, "VOLTage 32 V\r\n");
    HMP2020->Log("Установить Напряжение: 32 V.\n");
    viPrintf(HMP2020->vi, "OUTPut 1\r\n");
    HMP2020->Log("Включить подачу напряжения.\n");

    Sleep(1000);

    viQueryf(HMP2020->vi, "MEASure:CURRent?\n", "%T", Tok);
    HMP2020->Log("Измеренный Ток: "+QString(Tok));

    P = QString(Tok).toDouble();

    viQueryf(HMP2020->vi, "MEASure:VOLTage?\n", "%T", Volt);
    HMP2020->Log("Измеренное Напряжение: "+QString(Volt));

    P*=QString(Volt).toDouble();

    HMP2020->Log("Посчитанная Мощнасть: "+QString::number(P)+"\n");


    win_power->lcdNumber_32V_I->display(QString(Tok).toDouble());
    win_power->lcdNumber_32V_V->display(QString(Volt).toDouble());
    win_power->lcdNumber_32V_Bt->display(P);




    if(P > 35)
    {
        flagGood = false;

        SQL_OneProverka->setQuery("INSERT INTO Power (Data,DataProverki,IdLink,PowerResult,Volt,Tok,Sootvetstvie) VALUES('"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"','"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(1,0), Qt::EditRole).toString()+"','"+QString::number(P)+"','"+QString::number(win_power->lcdNumber_32V_V->value())+"','"+QString::number(win_power->lcdNumber_32V_I->value())+"','Не соответствует')");

        view->item(0,0)->setBackgroundColor(Qt::red);
        view->item(1,0)->setBackgroundColor(Qt::red);
        HMP2020->Log("Напряжение 32 V: Несоответствует\n");
        HMP2020->Log2("Напряжение 32 V: Несоответствует\n");
        win_power->pushButton_32V->setIcon(R3_Red);
        view->reset();
    }
    else
    {
        SQL_OneProverka->setQuery("INSERT INTO Power (Data,DataProverki,IdLink,PowerResult,Volt,Tok,Sootvetstvie) VALUES('"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"','"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(1,0), Qt::EditRole).toString()+"','"+QString::number(P)+"','"+QString::number(win_power->lcdNumber_32V_V->value())+"','"+QString::number(win_power->lcdNumber_32V_I->value())+"','Cоответствует')");

        view->item(0,0)->setBackgroundColor(Qt::green);
        view->item(1,0)->setBackgroundColor(Qt::green);
        HMP2020->Log("Напряжение 32 V: Соответствует\n");
        HMP2020->Log2("Напряжение 32 V: Соответствует\n");

        win_power->pushButton_32V->setIcon(R2_Green);

        view->reset();
    }

    viPrintf(HMP2020->vi, "OUTPut 0\r\n");
    HMP2020->Log("Включить подачу напряжения.\n");
    HMP2020->Log("=================\n");
    viPrintf(HMP2020->vi, "VOLTage 27 V\r\n");
    HMP2020->Log("Установить Напряжение: 27 V.\n");
    viPrintf(HMP2020->vi, "OUTPut 1\r\n");
    HMP2020->Log("Включить подачу напряжения.\n");

    Sleep(1000);

    viQueryf(HMP2020->vi, "MEASure:CURRent?\n", "%T", Tok);
    HMP2020->Log("Измеренный Ток: "+QString(Tok));

    P = QString(Tok).toDouble();

    viQueryf(HMP2020->vi, "MEASure:VOLTage?\n", "%T", Volt);
    HMP2020->Log("Измеренное Напряжение: "+QString(Volt));

    P*=QString(Volt).toDouble();

    HMP2020->Log("Посчитанная Мощнасть: "+QString::number(P)+"\n");


    win_power->lcdNumber_27V_I->display(QString(Tok).toDouble());
    win_power->lcdNumber_27V_V->display(QString(Volt).toDouble());
    win_power->lcdNumber_27V_Bt->display(P);


    if(P > 35)
    {
        flagGood = false;

        SQL_OneProverka->setQuery("INSERT INTO Power (Data,DataProverki,IdLink,PowerResult,Volt,Tok,Sootvetstvie) VALUES('"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"','"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(1,0), Qt::EditRole).toString()+"','"+QString::number(P)+"','"+QString::number(win_power->lcdNumber_27V_V->value())+"','"+QString::number(win_power->lcdNumber_27V_I->value())+"','Не соответствует')");

        view->item(0,0)->setBackgroundColor(Qt::red);
        view->item(1,0)->setBackgroundColor(Qt::red);
        HMP2020->Log("Напряжение 27 V: Несоответствует\n");
        HMP2020->Log2("Напряжение 27 V: Несоответствует\n");

        win_power->pushButton_27V->setIcon(R3_Red);
        view->reset();
    }
    else
    {
        SQL_OneProverka->setQuery("INSERT INTO Power (Data,DataProverki,IdLink,PowerResult,Volt,Tok,Sootvetstvie) VALUES('"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"','"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(1,0), Qt::EditRole).toString()+"','"+QString::number(P)+"','"+QString::number(win_power->lcdNumber_27V_V->value())+"','"+QString::number(win_power->lcdNumber_27V_I->value())+"','Cоответствует')");

        view->item(0,0)->setBackgroundColor(Qt::green);
        view->item(1,0)->setBackgroundColor(Qt::green);
        HMP2020->Log("Напряжение 27 V: Соответствует\n");
        HMP2020->Log2("Напряжение 27 V: Соответствует\n");

        win_power->pushButton_27V->setIcon(R2_Green);
        view->reset();
    }

    if(flagGood==false)
    {
        SQL_OneProverka->setQuery("INSERT INTO Result (Date,IdLink,ElectricalPower,[Этап]) VALUES('"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(0,0), Qt::EditRole).toString()+"','Не соответствует','"+etap+"')");
        SQL_OneProverka->setQuery("INSERT INTO Result (Date,IdLink,Power,[Этап]) VALUES('"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(1,0), Qt::EditRole).toString()+"','Не соответствует','"+etap+"')");
    }
    else
    {
        SQL_OneProverka->setQuery("INSERT INTO Result (Date,IdLink,ElectricalPower,[Этап]) VALUES('"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(0,0), Qt::EditRole).toString()+"','Cоответствует','"+etap+"')");
        SQL_OneProverka->setQuery("INSERT INTO Result (Date,IdLink,Power,[Этап]) VALUES('"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(1,0), Qt::EditRole).toString()+"','Cоответствует','"+etap+"')");
    }


    HMP2020->Log("Конец 1 и 2 проверки\n");
    HMP2020->Log2("Конец 1 и 2 проверки\n");
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

void thread_DM::StartProverka3()
{
    //Старт 3 првоерки
    N9000->Log("=================\n");
    N9000->Log("Старт 3 проверки.\n");
    N9000->Log2("Старт 3 проверки.\n");
    view->item(2,0)->setBackgroundColor(Qt::yellow);
    view->reset();

    flag=true;

    viPrintf(N9000->vi, "AVER:STAT OFF\n");
    N9000->Log("Отключаем усреднение: AVER.\n");

    viPrintf(N9000->vi, "CALC:MARK:MODE NORM\n");
    N9000->Log("Установить режим маркера: NORM.\n");


    viPrintf(N9000->vi, "DISP:WIND:TRAC:Y:RLEV 12dBm\n");
    N9000->Log("Установить Y: 12 dBm.\n");

    //Задает полосу просмотра для отображения сигнала на анализаторе....
    viPrintf(N9000->vi, "FREQuency:STARt 950 MHz\r\n");
    N9000->Log("Установить начальную частоту просмотра: 950 MHz.\n");

    viPrintf(N9000->vi, "FREQuency:STOP 2150 MHz\r\n");
    N9000->Log("Установить конечную частоту просмотра: 2150 MHz.\n");


    if(!flag)
    {
        view->item(2,0)->setBackgroundColor(Qt::red);
        view->reset();

        N9000->Log2("Несоответствует\n");

        SQL_OneProverka->setQuery("INSERT INTO Result (Date,IdLink,LimitDeviationOfGain,[Этап]) VALUES('"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(2,0), Qt::EditRole).toString()+"','Не Соответствует','"+etap+"')");
    }
    else
    {
        view->item(2,0)->setBackgroundColor(Qt::green);
        view->reset();

        N9000->Log2("Соответствует\n");

        SQL_OneProverka->setQuery("INSERT INTO Result (Date,IdLink,LimitDeviationOfGain,[Этап]) VALUES('"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(2,0), Qt::EditRole).toString()+"','Cоответствует','"+etap+"')");

    }

    qDebug () << "TP->selection_pult();";

    TP->selection_pult();

    qDebug () << "end TP->selection_pult();";

    TP->ReleA(0);
    TP->ReleB(0);
    TP->ReleA(22);
    TP->ReleB(22);

    N9000->Log("Конец 3 проверки.\n");
    N9000->Log2("Конец 3 проверки.\n");
}

void thread_DM::StartProverka4()
{

    Micran1->Log("=================\n");
    Micran1->Log("Старт 4 проверки.\n");
    TP->Log("=================\n");
    TP->Log("Старт 4 проверки.\n");
    N9000->Log("=================\n");
    N9000->Log("Старт 4 проверки.\n");

    Micran1->Log2("Старт 4 проверки.\n");
    emit Log("Старт 4 проверки.\n");

    emit StartFrequency(1);

    view->item(3,0)->setBackgroundColor(Qt::yellow);
    view->reset();

    win_frequency->tableWidgetAChH->setRowCount(5);
    win_frequency->tableWidgetAChH->setColumnCount(13);

    int point=0;

    for(int i=0;i<win_frequency->tableWidgetAChH->columnCount();i++)
    {
        win_frequency->tableWidgetAChH->setColumnWidth(i,40);

        for(int j=0;j<win_frequency->tableWidgetAChH->rowCount();j++)
        {
            point++;

            if(point!=65) //убираем 65 чекбокс в таблице
            {
                QTableWidgetItem * item = new QTableWidgetItem();

                item->setText(QString::number(point));

                win_frequency->tableWidgetAChH->setItem(j,i,item);

                win_frequency->tableWidgetAChH->item(j,i)->setCheckState(Qt::Unchecked);
                win_frequency->tableWidgetAChH->item(j,i)->setBackground(QBrush(Qt::yellow));
            }
        }
    }

    viPrintf(Micran1->vi, "OUTPut:STATe ON\r\n");
    Micran1->Log("Включить выход ВЧ.\n");

    FlagGoodAchH = true;

    for(int i=1;i<=4;i++)
    {
        Ysilenie(0,i); // Исправить для блока ДМ
    }

    for(int i=1;i <=18;i++) //Поменять i на 1
    {
        if(i==9 || i==10)
        {
            TP->ReleB(0);
        }
        else
        {
            ProverkaAchH(i); // i Вход у пульта
        }
    }

    if(FlagGoodAchH == true)
    {
        view->item(3,0)->setBackgroundColor(Qt::green);
        view->reset();

        N9000->Log2("Соответствует\n");

        SQL_OneProverka->setQuery("INSERT INTO Result (Date,IdLink,Frequency,[Этап]) VALUES('"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(3,0), Qt::EditRole).toString()+"','Cоответствует','"+etap+"')");

    }
    else
    {
        view->item(3,0)->setBackgroundColor(Qt::red);
        view->reset();

        N9000->Log2("Несоответствует\n");

        SQL_OneProverka->setQuery("INSERT INTO Result (Date,IdLink,Frequency,[Этап]) VALUES('"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(3,0), Qt::EditRole).toString()+"','Не соответствует','"+etap+"')");
    }

    TP->ReleA(0);
    TP->ReleB(0);
    TP->ReleA(22);
    TP->ReleB(22);

    Micran1->Log("Конец 4 проверки.\n");
    TP->Log("Конец 4 проверки.\n");
    N9000->Log("Конец 4 проверки.\n");
    Micran1->Log2("Конец 4 проверки.\n");
    emit Log("Конец 4 проверки.\n");
}

void thread_DM::ProverkaAchH(int A)
{


    TP->ReleB(A);

    if(A>=0 && A<=10)
    {
        ReleB = A;
    }
    else
    {
        ReleB = A-1;
    }

    int Vbix =0;

    //Настройка реле  пульта

    while(Vbix < 4)
    {
        // Написать код для установки выхода на комутатор! (Вход1 -> Выход1)
        switch (Vbix)
        {
        case 0:
            TP->ReleA(6); //Соединение реле на пульте A0-A6
            Rele_Kom(A,1); // Соединение реле на коммутаторе ДМ4-16 "B" выход с 1 входом
            ReleA = Rele(6);
            AchH();
            ListMin2.append(ListMin),ListMax2.append(ListMax),ListNEravn2.append(ListNEravn);
            if(A > 10)
                SQL_OneProverka->setQuery("UPDATE GraphPoint SET [Вход]='6',[Выход]='"+QString::number(A-1)+"' WHERE IdResult ='"+Link->data(Link->index(3,0), Qt::EditRole).toString()+"' AND NumberGraph='"+QString::number(ListX.count())+"'");
            else
                SQL_OneProverka->setQuery("UPDATE GraphPoint SET [Вход]='6',[Выход]='"+QString::number(A)+"' WHERE IdResult ='"+Link->data(Link->index(3,0), Qt::EditRole).toString()+"' AND NumberGraph='"+QString::number(ListX.count())+"'");
            break;
        case 1:
            TP->ReleA(7); //Соединение реле на пульте A0-A7
            Rele_Kom(A,2); // Соединение реле на коммутаторе ДМ4-16 "B" выход с 2 входом
            ReleA = Rele(7);
            AchH();
            ListMin2.append(ListMin),ListMax2.append(ListMax),ListNEravn2.append(ListNEravn);
            if(A > 10)
                SQL_OneProverka->setQuery("UPDATE GraphPoint SET [Вход]='7',[Выход]='"+QString::number(A-1)+"' WHERE IdResult ='"+Link->data(Link->index(3,0), Qt::EditRole).toString()+"' AND NumberGraph='"+QString::number(ListX.count())+"'");
            else
                SQL_OneProverka->setQuery("UPDATE GraphPoint SET [Вход]='7',[Выход]='"+QString::number(A)+"' WHERE IdResult ='"+Link->data(Link->index(3,0), Qt::EditRole).toString()+"' AND NumberGraph='"+QString::number(ListX.count())+"'");
            break;
        case 2:
            TP->ReleA(8); //Соединение реле на пульте A0-A8
            Rele_Kom(A,3); // Соединение реле на коммутаторе ДМ4-16 "B" выход с 3 входом
            ReleA = Rele(8);
            AchH();
            ListMin2.append(ListMin),ListMax2.append(ListMax),ListNEravn2.append(ListNEravn);
            if(A > 10)
                SQL_OneProverka->setQuery("UPDATE GraphPoint SET [Вход]='8',[Выход]='"+QString::number(A-1)+"' WHERE IdResult ='"+Link->data(Link->index(3,0), Qt::EditRole).toString()+"' AND NumberGraph='"+QString::number(ListX.count())+"'");
            else
                SQL_OneProverka->setQuery("UPDATE GraphPoint SET [Вход]='8',[Выход]='"+QString::number(A)+"' WHERE IdResult ='"+Link->data(Link->index(3,0), Qt::EditRole).toString()+"' AND NumberGraph='"+QString::number(ListX.count())+"'");
            break;
        case 3:
            TP->ReleA(9); //Соединение реле на пульте A0-A9
            Rele_Kom(A,4); // Соединение реле на коммутаторе ДМ4-16 "B" выход с 4 входом
            ReleA = Rele(9);
            AchH();
            ListMin2.append(ListMin),ListMax2.append(ListMax),ListNEravn2.append(ListNEravn);
            if(A > 10)
                SQL_OneProverka->setQuery("UPDATE GraphPoint SET [Вход]='9',[Выход]='"+QString::number(A-1)+"' WHERE IdResult ='"+Link->data(Link->index(3,0), Qt::EditRole).toString()+"' AND NumberGraph='"+QString::number(ListX.count())+"'");
            else
                SQL_OneProverka->setQuery("UPDATE GraphPoint SET [Вход]='9',[Выход]='"+QString::number(A-1)+"' WHERE IdResult ='"+Link->data(Link->index(3,0), Qt::EditRole).toString()+"' AND NumberGraph='"+QString::number(ListX.count())+"'");
            break;
        }
        Vbix++;

    }

    //////////////////////////////////////////////////////////////////////////////

    //Написать код для переключения Трактов коммутатора с 1 до 4 !

    //Написать код для установки выхода на комутатор! (Вход1 -> Выход1)

    //////////////////////////////////////////////////////////////////////////
}

int thread_DM::Rele(int B)
{
    if(B>=0 && B<=10)
    {
        ReleB = B;
    }
    else
    {
        ReleB = B-1;
    }
    return  ReleB;
}



void thread_DM::AchH()
{
    N9000->Log("=================\n");
    Micran1->Log("=================\n");


    view->item(3,0)->setBackgroundColor(Qt::yellow);
    view->reset();


    double dResultY = 0;
    double dResultX = 0;
    char buff[9024];

    x.clear();
    y.clear();
    int end=GnFREQuencyStart;
    int point=0;


    for(int i=0;i<win_frequency->tableWidgetAChH->columnCount();i++)
    {

        for(int j=0;j<win_frequency->tableWidgetAChH->rowCount();j++)
        {
            if(point!=64)
            {
                win_frequency->tableWidgetAChH->item(j,i)->setCheckState(Qt::Unchecked);
            }
            if(ListX.count()==point)
            {
                win_frequency->tableWidgetAChH->item(j,i)->setCheckState(Qt::Checked);
            }
            point++;
        }
    }


    point=0;
    flag=true;
    //4 Проверка АЧХ
    while(GnFREQuencyEnd >= end)
    {

        //Задает полосу просмотра для отображения сигнала на анализаторе....
        viPrintf(N9000->vi, "FREQuency:STARt %d MHz\r\n",(end-GnFREQuencyStep));
        N9000->Log("Установить начальную частоту просмотра "+QString::number(end-GnFREQuencyStep)+" MHz\n");

        viPrintf(N9000->vi, "FREQuency:STOP %d MHz\r\n",(end+GnFREQuencyStep));
        N9000->Log("Установить конечную частоту просмотра "+QString::number(end+GnFREQuencyStep)+" MHz\n");



        viPrintf(Micran1->vi, "SOURce:FREQuency:MODE FIXed\r\n");
        Micran1->Log("Установить MODE: FIXed\n");


        // Set power level value
        viPrintf(Micran1->vi, "SOURce:POWer:LEVel %d DBM\r\n", GnPower);
        Micran1->Log("Установить Мощность: "+QString::number(GnPower)+" dBm\n");

        viPrintf(Micran1->vi, "SOURce:FREQuency %d MHz\r\n",end);
        Micran1->Log("Установить Частоту: "+QString::number(end)+" MHz\n");

        // qDebug()<<"FREQuency = "<< end;
        // Set trigger automatic mode
        viPrintf(Micran1->vi, "INITiate:CONTinuous ON\r\n");

        // Set power out on
        viPrintf(Micran1->vi, "OUTPut:STATe ON\r\n"); // Включить/Выключить выход СВЧ
        Micran1->Log("Включить выход ВЧ.\n");

        // Check errors
        viQueryf(Micran1->vi,"SYSTem:ERRor?\n","%T",buff);
        Micran1->Log("Ошибка: "+QString(buff)+"\n");


        Sleep(150);

        viPrintf(N9000->vi, "CALC:MARK:MAX\n");
        N9000->Log("Захватить максимум.\n");
        viQueryf(N9000->vi, "CALC:MARK:Y?\n", "%lf", &dResultY);
        N9000->Log("Считанный Y :"+QString::number(dResultY)+" дб\n");
        viQueryf(N9000->vi, "CALC:MARK:X?\n", "%lf", &dResultX);
        N9000->Log("Считанный X :"+QString::number(dResultX/pow(10,6))+" MHz\n");




        x.append(dResultX/pow(10,6));
        dResultY -=GnPower;

        //разкоментировать когда добавлю калибровку
        double ot = ListRegyl->value(ReleB).value(y.count());
        double ot2 = ListRegyl->value(ReleA).value(y.count());
        double PosleRegyl = dResultY - ot - ot2;

        dResultY = PosleRegyl; // положили в переменную число после регулировки для не изменения в запросе переменной

        y.append(PosleRegyl);

        auto min1 = std::min_element(y.begin(),y.end());
        auto max1 = std::max_element(y.begin(),y.end());

        min = (*min1);
        max = (*max1);

        NeravnACHX = (*max1)-(*min1);


        end+=GnFREQuencyStep;
        date = QDateTime::currentDateTime();
        SQL_OneProverka->setQuery("INSERT INTO GraphPoint (Data,IdResult,X,Y,NumberGraph,Неравномерность,Минимум,Максимум,DataProverki) VALUES('"+date.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(3,0), Qt::EditRole).toString()+"','"+QString::number(dResultX/pow(10,6))+"','"+QString::number(dResultY)+"','"+QString::number(ListX.count()+1)+"','"+QString::number(NeravnACHX)+"','"+QString::number(min)+"','"+QString::number(max)+"','"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"')");

        qDebug() << "signal  updateGraph";
        emit updateGraph();

        sem.acquire();

    }

    if(NeravnACHX >2)
    {
        // qDebug()<< "Ошибка! Неравномерность АЧХ "<<NeravnACHX<<" > 2";


        view->item(3,0)->setBackgroundColor(Qt::red);
        view->reset();

        FlagGoodAchH = false;
    }
    else
    {
        // qDebug()<< "Ок! Неравномерность АЧХ "<<NeravnACHX<<" < 2";
        view->item(3,0)->setBackgroundColor(Qt::green);
        view->reset();

    }

    ListX.append(x);
    ListY.append(y);

    ListMin.append(min);
    ListMax.append(max);
    ListNEravn.append(NeravnACHX);


    for(int i=0;i<win_frequency->tableWidgetAChH->columnCount();i++)
    {
        for(int j=0;j<win_frequency->tableWidgetAChH->rowCount();j++)
        {
            point++;
            if(ListX.count()==point)
            {
                if(NeravnACHX >2)
                {
                    win_frequency->tableWidgetAChH->item(j,i)->setBackground(QBrush(Qt::red));

                }
                else
                {
                    win_frequency->tableWidgetAChH->item(j,i)->setBackground(QBrush(Qt::green));

                }
                break;
            }
        }
    }

    flag=false;

    N9000->Log("=================\n");
    Micran1->Log("=================\n");

}


void SetCheck(thread_DM* threadDM)
{
    int point=0;
    for(int i=0;i<threadDM->win_transferCoefficient->tableWidgetPerestrouka->columnCount();i++)
    {

        for(int j=0;j<threadDM->win_transferCoefficient->tableWidgetPerestrouka->rowCount();j++)
        {

            if(point < 32)
            {
                threadDM->win_transferCoefficient->tableWidgetPerestrouka->item(j,i)->setCheckState(Qt::Unchecked);

                if(threadDM->ListPerestrouka.count()==point)
                {
                    threadDM->win_transferCoefficient->tableWidgetPerestrouka->item(j,i)->setCheckState(Qt::Checked);
                }
                point++;
            }
        }
    }
}

void thread_DM::StartProverka5()
{
    Start5Proverka = true;

    emit StartPerestroyka(2);

    //Обнуление реле
    TP->ReleA(0);
    TP->ReleA(21);


    Micran1->Log("=================\n");
    Micran1->Log("Старт 5 проверки.\n");
    TP->Log("=================\n");
    TP->Log("Старт 5 проверки.\n");
    N9000->Log("=================\n");
    N9000->Log("Старт 5 проверки.\n");
    N9000->Log2("Старт 5 проверки.\n");

    view->item(4,0)->setBackgroundColor(Qt::yellow);
    view->reset();
    date = QDateTime::currentDateTime();

    viPrintf(N9000->vi, "DISP:WIND:TRAC:Y:RLEV 12dBm\n");
    N9000->Log("Установить Y: 12 dBm.\n");


    viPrintf(N9000->vi, "FREQuency:STARt 1500 MHz\r\n");
    N9000->Log("Установить начальную частоту просмотра: 1500 MHz.\n");

    viPrintf(N9000->vi, "FREQuency:STOP 1600 MHz\r\n");
    N9000->Log("Установить конечную частоту просмотра: 1600 MHz.\n");

    //Обнуление реле
    TP->ReleA(0);
    TP->ReleA(22);
    TP->ReleB(0);
    TP->ReleB(22);

    win_transferCoefficient->tableWidgetPerestrouka->setRowCount(16);
    win_transferCoefficient->tableWidgetPerestrouka->setColumnCount(2);

   // int point=0;


   // emit CreateGraph();
   // sem3.acquire();


    qDebug () << " thread_DM updateGraphPerestrouyka ";

    emit CreateGraph();
    sem3.acquire();

    /*
    for(int i=0;i<win_transferCoefficient->tableWidgetPerestrouka->columnCount();i++)
    {
        win_transferCoefficient->tableWidgetPerestrouka->setColumnWidth(i,40);

        for(int j=0;j<win_transferCoefficient->tableWidgetPerestrouka->rowCount();j++)
        {
            point++;
            QTableWidgetItem * item = new QTableWidgetItem();

            item->setText(QString::number(point));

            win_transferCoefficient->tableWidgetPerestrouka->setItem(j,i,item);

            win_transferCoefficient->tableWidgetPerestrouka->item(j,i)->setCheckState(Qt::Unchecked);
            win_transferCoefficient->tableWidgetPerestrouka->item(j,i)->setBackground(QBrush(Qt::yellow));

            win_transferCoefficient->Graph->addGraph();
            win_transferCoefficient->Graph->graph(point-1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
            win_transferCoefficient->Graph->graph(point-1)->setPen(QPen(qrand())); // line color blue for first graph
            win_transferCoefficient->Graph->graph(point-1)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue
        }
    }
    */


    FlagGood5 = true;
    FlagGood5Eshe= true;


    for(int j=1;j<=4;j++)
    {
        TP->ReleA(j+5);
        for(int k=1;k <=16;k++)
        {
            if( k%2 == 0)
            {
                SetCheck(this);
                if(k >=10 )
                {
                    if(k==10)
                    {
                        TP->ReleB(0);
                    }
                    TP->ReleB(k+1);
                }
                else
                {
                    if(k == 2)
                    {
                        TP->ReleB(22);
                    }
                    TP->ReleB(k);
                }

                for(int i=-10;i<=10;i++) // -10 до 10
                {
                    Ysilenie(i,j);
                    if(k >=10 )
                    {
                        Proverka_5(i);
                        SQL_OneProverka->setQuery("UPDATE GraphPoint SET [Вход]='"+QString::number(j+5)+"',[Выход]='"+QString::number(k)+"' WHERE IdResult ='"+Link->data(Link->index(4,0), Qt::EditRole).toString()+"' AND NumberGraph='"+QString::number(ListPerestrouka.count()+1)+"'");
                    }
                    else
                    {
                        Proverka_5(i);
                        SQL_OneProverka->setQuery("UPDATE GraphPoint SET [Вход]='"+QString::number(j+5)+"',[Выход]='"+QString::number(k)+"' WHERE IdResult ='"+Link->data(Link->index(4,0), Qt::EditRole).toString()+"' AND NumberGraph='"+QString::number(ListPerestrouka.count()+1)+"'");
                    }
                }
                ListPerestrouka.append(y1); //Для отображения на графике подсчет количества графиков по выходам
                Listy1.append(PerestroykaX);
                if(FlagGood5)
                {
                    if(ListPerestrouka.count() <=16)
                    {
                        win_transferCoefficient->tableWidgetPerestrouka->item(ListPerestrouka.count()-1,0)->setBackground(Qt::green);
                    }
                    else
                    {
                        win_transferCoefficient->tableWidgetPerestrouka->item(ListPerestrouka.count()-17,1)->setBackground(Qt::green);
                    }
                }
                else
                {
                    if(ListPerestrouka.count() <=16)
                    {
                        win_transferCoefficient->tableWidgetPerestrouka->item(ListPerestrouka.count()-1,0)->setBackground(Qt::red);
                    }
                    else
                    {
                        win_transferCoefficient->tableWidgetPerestrouka->item(ListPerestrouka.count()-17,1)->setBackground(Qt::red);

                    }
                }
                y1.clear();
                PerestroykaX.clear();
            }

        }

    }


    if(FlagGood5Eshe == true)
    {
        view->item(3,0)->setBackgroundColor(Qt::green);
        view->reset();
        TP->Log2("Соответствует\n");

        SQL_OneProverka->setQuery("INSERT INTO Result (Date,IdLink,TransferCoefficient,[Этап]) VALUES('"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(4,0), Qt::EditRole).toString()+"','Cоотвутствует','"+etap+"')");

    }
    else
    {
        view->item(3,0)->setBackgroundColor(Qt::red);
        view->reset();
        TP->Log2("Несоответствует\n");

        SQL_OneProverka->setQuery("INSERT INTO Result (Date,IdLink,TransferCoefficient,[Этап]) VALUES('"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(4,0), Qt::EditRole).toString()+"','Не соответствует','"+etap+"')");
    }

    TP->ReleA(0);
    TP->ReleB(0);
    TP->ReleA(22);
    TP->ReleB(22);

    Micran1->Log("Конец 5 проверки.\n");
    TP->Log("Конец 5 проверки.\n");
    N9000->Log("Конец 5 проверки.\n");
    N9000->Log2("Конец 5 проверки.\n");

    Start5Proverka = false;
    FlagGood5 = false;

}

void thread_DM::Proverka_5(int KoefPeredachi)
{

    FlagGood5 =true;

    view->item(4,0)->setBackgroundColor(Qt::yellow);
    view->reset();

    double dResultY = 0;
    double dResultX = 0;
    char buff[9024];


    viPrintf(Micran1->vi, "SOURce:FREQuency:MODE FIXed\r\n");
    Micran1->Log("Установить MODE: FIXed\n");

    // Set power level value
    viPrintf(Micran1->vi, "SOURce:POWer:LEVel %d DBM\r\n",KoefPeredachi); // Установка Мощности для 1550 МГЦ.
    Micran1->Log("Установить Мощность: "+QString::number(KoefPeredachi)+" dBm\n");

    viPrintf(Micran1->vi, "SOURce:FREQuency 1550 MHz\r\n");
    Micran1->Log("Установить Частоту: 1550 MHz\n");


    // Set trigger automatic mode
    viPrintf(Micran1->vi, "INITiate:CONTinuous ON\r\n");

    // Set power out on
    viPrintf(Micran1->vi, "OUTPut:STATe ON\r\n"); // Включить/Выключить выход СВЧ
    Micran1->Log("Включить выход ВЧ.\n");

    // Check errors
    viQueryf(Micran1->vi,"SYSTem:ERRor?\n","%T",buff);
    Micran1->Log("Ошибка: "+QString(buff)+"\n");

    Sleep(200);

    viPrintf(N9000->vi, "CALC:MARK:MAX\n");
    N9000->Log("Захватить максимум.\n");
    viQueryf(N9000->vi, "CALC:MARK:Y?\n", "%lf", &dResultY);
    N9000->Log("Считанный Y :"+QString::number(dResultY)+" дб\n");
    viQueryf(N9000->vi, "CALC:MARK:X?\n", "%lf", &dResultX);
    N9000->Log("Считанный X :"+QString::number(dResultX/pow(10,6))+" MHz\n");



    x1.append(dResultX/pow(10,6));

    y1.append(dResultY);

    PerestroykaX.append(KoefPeredachi);


    date = QDateTime::currentDateTime();


    SQL_OneProverka->setQuery("INSERT INTO GraphPoint (Data,IdResult,X,Y,[Коэфициент передачи],NumberGraph,DataProverki) VALUES('"+date.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(4,0), Qt::EditRole).toString()+"','"+QString::number(dResultX/pow(10,6))+"','"+QString::number(dResultY)+"','"+QString::number(KoefPeredachi)+"','"+QString::number(ListPerestrouka.count()+1)+"','"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"')");

    emit updateGraphPerestrouyka();
    sem3.acquire();


    //Примерная мощность для ошибки
    if(dResultY > KoefPeredachi+2 || dResultY < KoefPeredachi-2)
    {
        view->item(4,0)->setBackgroundColor(Qt::red);
        view->reset();

        FlagGood5 = false;
        FlagGood5Eshe = false;
    }
    else
    {
        view->item(4,0)->setBackgroundColor(Qt::green);
        view->reset();

    }


}

void thread_DM:: StartProverka6()
{
    emit StartPower(3);
    // qDebug()<< "Старт 6 првоерки!";
    Micran1->Log("=================\n");
    Micran1->Log("Старт 6 проверки.\n");
    TP->Log("=================\n");
    TP->Log("Старт 6 проверки.\n");
    N9000->Log("=================\n");
    N9000->Log("Старт 6 проверки.\n");
    N9000->Log2("Старт 6 проверки.\n");


    view->item(5,0)->setBackgroundColor(Qt::yellow);
    view->reset();

    TP->ReleB(19);

    flag_6_proverki = true;
    TP->flag_6_proverki = true;

    viPrintf(N9000->vi, "AVER:STAT OFF\n");
    viPrintf(N9000->vi, "DISP:WIND:TRAC:Y:RLEV 10dBm\n");
    N9000->Log("Установить Y: 10 dBm.\n");
    viPrintf(N9000->vi, "CALC:MARK:MODE NORM\n");

    //Задает полосу просмотра для отображения сигнала на анализаторе....
    viPrintf(N9000->vi, "FREQuency:STARt 9 MHz\r\n");
    N9000->Log("Установить начальную частоту просмотра: 9 MHz.\n");

    viPrintf(N9000->vi, "FREQuency:STOP 11 MHz\r\n");
    N9000->Log("Установить конечную частоту просмотра: 11 MHz.\n");

    date = QDateTime::currentDateTime();

    FlagGood10Mhz = true;

    for(int i=6;i < 10;i++)
    {
        TP->ReleA(i);
        Rele_Kom_10MGH(i-5); // Соединение реле на коммутаторе ДМ4-16 c i входом и ПРМ 10 МГЦ
        Proverka10MGU();
        SQL_OneProverka->setQuery("UPDATE GraphPoint SET [Вход]='"+QString::number(i)+"',[Выход]='10 МГЦ' WHERE IdResult ='"+Link->data(Link->index(5,0), Qt::EditRole).toString()+"'AND NumberGraph='"+QString::number(x10MGh.count())+"'");
    }

    if(FlagGood10Mhz == true)
    {
        view->item(5,0)->setBackgroundColor(Qt::green);
        view->reset();
        date = QDateTime::currentDateTime();
        N9000->Log2("Cоответствует\n");
        SQL_OneProverka->setQuery("INSERT INTO Result (Date,IdLink,SwitchingSignalSM,[Этап]) VALUES('"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(5,0), Qt::EditRole).toString()+"','соответствует','"+etap+"')");

    }
    else
    {
        view->item(5,0)->setBackgroundColor(Qt::red);
        view->reset();
        date = QDateTime::currentDateTime();
        N9000->Log2("Несоответствует\n");
        SQL_OneProverka->setQuery("INSERT INTO Result (Date,IdLink,SwitchingSignalSM,[Этап]) VALUES('"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(5,0), Qt::EditRole).toString()+"','Не соответствует','"+etap+"')");
    }

    flag_6_proverki = false;
    TP->flag_6_proverki = false;

    TP->ReleA(0);
    TP->ReleB(0);
    TP->ReleA(22);
    TP->ReleB(22);

    Micran1->Log("Конец 6 проверки.\n");
    TP->Log("Конец 6 проверки.\n");
    N9000->Log("Конец 6 проверки.\n");
    N9000->Log2("Конец 6 проверки.\n");

}


void thread_DM::Proverka10MGU()
{
    QPixmap R1_Gray(":/images/circle_gray.png");
    QPixmap R2_Green(":/images/circle_zel.png");
    QPixmap R3_Red(":/images/circle_kras.png");

    view->item(5,0)->setBackgroundColor(Qt::yellow);
    view->reset();

    double dResultY = 0;
    double dResultX = 0;
    char buff[9024];

    viPrintf(Micran1->vi, "SOURce:FREQuency:MODE FIXed\r\n");
    Micran1->Log("Установить MODE: FIXed\n");

    // Set power level value
    viPrintf(Micran1->vi, "SOURce:POWer:LEVel 0 DBM\r\n"); // Установка Мощности для 10 МГЦ.
    Micran1->Log("Установить Мощность: 0 dBm\n");

    viPrintf(Micran1->vi, "SOURce:FREQuency 10 MHz\r\n");
    Micran1->Log("Установить Частоту: 10 MHz\n");




    // Set trigger automatic mode
    viPrintf(Micran1->vi, "INITiate:CONTinuous ON\r\n");

    // Set power out on
    viPrintf(Micran1->vi, "OUTPut:STATe ON\r\n"); // Включить/Выключить выход СВЧ
    Micran1->Log("Включить выход ВЧ.\n");

    // Check errors
    viQueryf(Micran1->vi,"SYSTem:ERRor?\n","%T",buff);
    Micran1->Log("Ошибка: "+QString(buff)+"\n");

    Sleep(200);

    viPrintf(N9000->vi, "CALC:MARK:MAX\n");
    N9000->Log("Захватить максимум.\n");

    viQueryf(N9000->vi, "CALC:MARK:Y?\n", "%lf", &dResultY);
    N9000->Log("Считанный Y :"+QString::number(dResultY)+" дб\n");

    viQueryf(N9000->vi, "CALC:MARK:X?\n", "%lf", &dResultX);
    N9000->Log("Считанный X :"+QString::number(dResultX/pow(10,6))+" MHz\n");

    x10MGh.append(dResultX/pow(10,6));

    y10MGh.append(dResultY);

    date = QDateTime::currentDateTime();

    SQL_OneProverka->setQuery("INSERT INTO GraphPoint (Data,IdResult,X,Y,NumberGraph,DataProverki) VALUES('"+date.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(5,0), Qt::EditRole).toString()+"','"+QString::number(dResultX/pow(10,6))+"','"+QString::number(dResultY)+"','"+QString::number(x10MGh.count())+"','"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"')");

    if(x10MGh.count()==1)
    {
        win_switchingSignalDM->lcdNumber->display(dResultY);
    }
    if(x10MGh.count()==2)
    {
        win_switchingSignalDM->lcdNumber_2->display(dResultY);
    }
    if(x10MGh.count()==3)
    {
        win_switchingSignalDM->lcdNumber_3->display(dResultY);
    }
    if(x10MGh.count()==4)
    {
        win_switchingSignalDM->lcdNumber_4->display(dResultY);
    }


    //Примерная мощность для ошибки
    if(dResultY < -5)
    {
        if(x10MGh.count()==1)
        {
            win_switchingSignalDM->pushButton_1->setIcon(R3_Red);
        }
        if(x10MGh.count()==2)
        {
            win_switchingSignalDM->pushButton_2->setIcon(R3_Red);
        }
        if(x10MGh.count()==3)
        {
            win_switchingSignalDM->pushButton_3->setIcon(R3_Red);
        }
        if(x10MGh.count()==4)
        {
            win_switchingSignalDM->pushButton_4->setIcon(R3_Red);
        }

        view->item(5,0)->setBackgroundColor(Qt::red);
        view->reset();

        FlagGood10Mhz = false;
    }
    else
    {
        if(x10MGh.count()==1)
        {
            win_switchingSignalDM->pushButton_1->setIcon(R2_Green);
        }
        if(x10MGh.count()==2)
        {
            win_switchingSignalDM->pushButton_2->setIcon(R2_Green);
        }
        if(x10MGh.count()==3)
        {
            win_switchingSignalDM->pushButton_3->setIcon(R2_Green);
        }
        if(x10MGh.count()==4)
        {
            win_switchingSignalDM->pushButton_4->setIcon(R2_Green);
        }

        view->item(5,0)->setBackgroundColor(Qt::green);
        view->reset();
    }
}


void thread_DM::StartProverka7()
{
    // qDebug()<< "Старт 7 првоерки!";

    TP->Log("=================\n");
    TP->Log("Старт 7 проверки.\n");
    TP->Log2("Старт 7 проверки.\n");


    view->item(6,0)->setBackgroundColor(Qt::yellow);
    view->reset();

    //Обнуление реле Пульта
    TP->ReleA(0);
    TP->ReleB(0);
    TP->ReleA(21);
    TP->ReleB(21);

    char b[2];
    char a_prin[10];
    bool flag_prov=false;

    flag_prov = TP->ReleA(1);

    if(flag_prov == false)
    {
        view->item(6,0)->setBackgroundColor(Qt::red);
        view->reset();
        TP->Log2("Несоответствует\n");
        date = QDateTime::currentDateTime();
        SQL_OneProverka->setQuery("INSERT INTO Result (Date,IdLink,Ithernet,[Этап]) VALUES('"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(6,0), Qt::EditRole).toString()+"','Не соответствует','"+etap+"')");
    }
    else
    {
        view->item(6,0)->setBackgroundColor(Qt::green);
        view->reset();
        TP->Log2("Соответствует\n");
        date = QDateTime::currentDateTime();
        SQL_OneProverka->setQuery("INSERT INTO Result (Date,IdLink,Ithernet,[Этап]) VALUES('"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(6,0), Qt::EditRole).toString()+"','Cоответствует','"+etap+"')");
    }

    TP->Log("Конец 7 проверки.\n");
    TP->Log2("Конец 7 проверки.\n");
}

void thread_DM::StartProveroka8()
{

    if(Link->rowCount()==8)
    {
        //TP->Log2("=================\n");
        TP->Log2("Старт 8 проверки.\n");

        view->item(7,0)->setBackgroundColor(Qt::yellow);
        view->item(7,0)->setBackgroundColor(Qt::green);
        view->reset();
        date = QDateTime::currentDateTime();
        SQL_OneProverka->setQuery("INSERT INTO Result (Date,IdLink,LongTime,[Этап]) VALUES('"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(7,0), Qt::EditRole).toString()+"','Cоответствует','"+etap+"')");
        TP->Log2("Конец 8 проверки.\n");
    }

}

void thread_DM::END()
{
    // Send "Clear status" command
    viPrintf(Micran1->vi, "*CLS\r\n");
    Micran1->Log("Cброс статуса.\n");

    viPrintf(Micran1->vi, "OUTPut:STATe OFF\r\n");
    Micran1->Log("Включить выход ВЧ.\n");

    viPrintf(HMP2020->vi, "OUTPut:GENeral 0\r\n");

    TP->ReleA(0);
    TP->ReleB(0);
    TP->ReleA(21);
    TP->ReleB(21);

    /*
    TP->udpsocket->close(); // UDP soket Для Переключения реле Пульта "A1"выход по "A9"
    TP->udpsocket2->close(); // UDP soket Для Переключения реле Пульта "A10"выхода по "A18"
    TP->udpsocket5->close(); // UDP soket Для Переключения реле Пульта "В1"выход до B"9"
    TP->udpsocket6->close(); // UDP soket Для Переключения реле Пульта B"10" выхода до B"18"
    p_udpSocketOut->close(); // UDP soket для Переключения каналов реле Коммутатора СМ16-4

    */

    emit end();
}

void thread_DM::StratRegyl()
{
    emit StartPower(1);

    TP->Log("=================\n");
    TP->Log("Старт Регулировки.\n");
    TP->Log2("Старт Регулировки.\n");
    Micran1->Log("=================\n");
    Micran1->Log("Старт Регулировки.\n");
    N9000->Log("=================\n");
    N9000->Log("Старт Регулировки.\n");
    emit Log("Старт Регулировки.\n");

    flag=true;

    viPrintf(N9000->vi, "AVER:STAT OFF\n");

    viPrintf(N9000->vi, "CALC:MARK:MODE NORM\n");
    N9000->Log("Установить режим маркера: NORM.\n");


    viPrintf(N9000->vi, "DISP:WIND:TRAC:Y:RLEV 12dBm\n");
    N9000->Log("Установить Y: 12 dBm.\n");

    //Задает полосу просмотра для отображения сигнала на анализаторе....
    viPrintf(N9000->vi, "FREQuency:STARt 950 MHz\r\n");
    N9000->Log("Установить начальную частоту просмотра: 950 MHz.\n");

    viPrintf(N9000->vi, "FREQuency:STOP 2150 MHz\r\n");
    N9000->Log("Установить конечную частоту просмотра: 2150 MHz.\n");

    win_frequency->tableWidgetAChH->clear();

    win_frequency->tableWidgetAChH->setRowCount(5);
    win_frequency->tableWidgetAChH->setColumnCount(8);

    int point=0;

    for(int i=0;i<win_frequency->tableWidgetAChH->columnCount();i++)
    {
        win_frequency->tableWidgetAChH->setColumnWidth(i,50);

        for(int j=0;j<win_frequency->tableWidgetAChH->rowCount();j++)
        {
            point++;

            if(point ==37 || point==38 || point ==39 || point ==40 ) //убираем 65 чекбокс в таблице
            {

            }
            else
            {
                QTableWidgetItem * item = new QTableWidgetItem();

                item->setText(QString::number(point));

                win_frequency->tableWidgetAChH->setItem(j,i,item);

                win_frequency->tableWidgetAChH->item(j,i)->setCheckState(Qt::Unchecked);
                win_frequency->tableWidgetAChH->item(j,i)->setBackground(QBrush(Qt::yellow));
            }
        }
    }

    SQL_OneProverka = new QSqlQueryModel();

    FlagGoodAchH = true;

    flagRegylK0=false;

    flagRegylK0=true;
    m_mutex.lock();
    sem2.acquire();


    SQL_OneProverka->setQuery("INSERT INTO Result (Date,IdLink,[Этап]) VALUES('"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(3,0), Qt::EditRole).toString()+"','"+etap+"')");


    AchHRegyl();// Промерить К0 кабель

    // Set power out on
    viPrintf(Micran1->vi, "OUTPut:STATe OFF\r\n"); // Включить/Выключить выход СВЧ
    Micran1->Log("Включить выход ВЧ.\n");

    ListX.clear();
    ListY.clear();

    ListMin.clear();
    ListMax.clear();
    ListNEravn.clear();


    releA =0;
    flagRegyl_A=false;
    flagRegyl_B=false;

    //Реле А
    for(int i=1;i <=19;i++) //Поменять i на 1
    {
        if( i==10)
        {
            TP->ReleA(0);
        }
        else
        {

            if(i <=9)
            {
                releA = i;
                qDebug() <<" i = "<< releA;
            }
            else
            {
                releA =i-1;

                qDebug() <<" i-1 = "<<  releA;
            }
            flagRegyl_A = true;
            m_mutex.lock();
            sem.acquire();
            TP->ReleA(i);

            qDebug() <<"После i = " <<  i;
            ProverkaRegyl(); // i Вход у пульта

            // Set power out on
            viPrintf(Micran1->vi, "OUTPut:STATe OFF\r\n"); // Включить/Выключить выход СВЧ
            Micran1->Log("Включить выход ВЧ.\n");


        }
    }

    TP->ReleA(22);

    //Реле Б
    for(int i=1;i <=19;i++) //Поменять i на 1
    {
        if( i==10)
        {
            TP->ReleB(0);
        }
        else
        {

            if(i <=9)
            {
                releA = i;
            }
            else
            {
                releA =i-1;
            }
            flagRegyl_B = true;
            m_mutex.lock();
            sem.acquire();
            TP->ReleB(i);

            qDebug() <<"После i = " <<  i;
            ProverkaRegyl(); // i Вход у пульта

            // Set power out on
            viPrintf(Micran1->vi, "OUTPut:STATe OFF\r\n"); // Включить/Выключить выход СВЧ
            Micran1->Log("Включить выход ВЧ.\n");

        }
    }
    TP->ReleB(22);



    TP->Log("Конец Регулировки.\n");
    TP->Log2("Конец Регулировки.\n");
    Micran1->Log("Конец Регулировки.\n");
    N9000->Log("Конец Регулировки.\n");
    emit Log("Конец Регулировки.\n");

}

void thread_DM::ProverkaRegyl()
{
    AchH();
}

void thread_DM::AchHRegyl()
{
    N9000->Log("=================\n");
    Micran1->Log("=================\n");


    double dResultY = 0;
    double dResultX = 0;
    char buff[9024];

    x.clear();
    y.clear();
    int end=GnFREQuencyStart;


    flag=true;
    //4 Проверка АЧХ
    while(GnFREQuencyEnd >= end)
    {

        //Задает полосу просмотра для отображения сигнала на анализаторе....
        viPrintf(N9000->vi, "FREQuency:STARt %d MHz\r\n",(end-GnFREQuencyStep));
        N9000->Log("Установить начальную частоту просмотра "+QString::number(end-GnFREQuencyStep)+" MHz\n");

        viPrintf(N9000->vi, "FREQuency:STOP %d MHz\r\n",(end+GnFREQuencyStep));
        N9000->Log("Установить конечную частоту просмотра "+QString::number(end+GnFREQuencyStep)+" MHz\n");


        viPrintf(Micran1->vi, "SOURce:FREQuency:MODE FIXed\r\n");
        Micran1->Log("Установить MODE: FIXed\n");


        // Set power level value
        viPrintf(Micran1->vi, "SOURce:POWer:LEVel %d DBM\r\n", GnPower);
        Micran1->Log("Установить Мощность: "+QString::number(GnPower)+" dBm\n");

        viPrintf(Micran1->vi, "SOURce:FREQuency %d MHz\r\n",end);
        Micran1->Log("Установить Частоту: "+QString::number(end)+" MHz\n");

        // qDebug()<<"FREQuency = "<< end;
        // Set trigger automatic mode
        viPrintf(Micran1->vi, "INITiate:CONTinuous ON\r\n");

        // Set power out on
        viPrintf(Micran1->vi, "OUTPut:STATe ON\r\n"); // Включить/Выключить выход СВЧ
        Micran1->Log("Включить выход ВЧ.\n");

        // Check errors
        viQueryf(Micran1->vi,"SYSTem:ERRor?\n","%T",buff);
        Micran1->Log("Ошибка: "+QString(buff)+"\n");


        Sleep(100);

        viPrintf(N9000->vi, "CALC:MARK:MAX\n");
        N9000->Log("Захватить максимум.\n");
        viQueryf(N9000->vi, "CALC:MARK:Y?\n", "%lf", &dResultY);
        N9000->Log("Считанный Y :"+QString::number(dResultY)+" дб\n");
        viQueryf(N9000->vi, "CALC:MARK:X?\n", "%lf", &dResultX);
        N9000->Log("Считанный X :"+QString::number(dResultX/pow(10,6))+" MHz\n");


        x.append(dResultX/pow(10,6));
        dResultY -=GnPower;
        y.append(dResultY);

        auto min1 = std::min_element(y.begin(),y.end());
        auto max1 = std::max_element(y.begin(),y.end());

        min = (*min1);
        max = (*max1);

        NeravnACHX = (*max1)-(*min1);


        end+=GnFREQuencyStep;
        date = QDateTime::currentDateTime();
        SQL_OneProverka->setQuery("INSERT INTO GraphPoint (Data,IdResult,X,Y,NumberGraph,Неравномерность,Минимум,Максимум,DataProverki) VALUES('"+date.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(3,0), Qt::EditRole).toString()+"','"+QString::number(dResultX/pow(10,6))+"','"+QString::number(dResultY)+"','K0','"+QString::number(NeravnACHX)+"','"+QString::number(min)+"','"+QString::number(max)+"','"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"')");

    }

    if(NeravnACHX >2)
    {
        qDebug()<< "Ошибка! Неравномерность АЧХ "<<NeravnACHX<<" > 2";

        FlagGoodAchH = false;
    }
    else
    {
        qDebug()<<"Ок! Неравномерность АЧХ "<<NeravnACHX<<" < 2";

    }

    ListX.append(x);
    ListY.append(y);

    ListMin.append(min);
    ListMax.append(max);
    ListNEravn.append(NeravnACHX);

    flag=false;

    N9000->Log("=================\n");
    Micran1->Log("=================\n");

}
