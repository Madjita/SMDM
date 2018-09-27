#include <thread_SMDM.h>


#include <mainwindow.h>
#include <QString.h>
#include <QPalette>

#define KF_l 19.5
#define KF_U 21.5
#define ACHH 4

thread_SMDM::thread_SMDM()
{

}


thread_SMDM::thread_SMDM(Micran_Gen*  Micran1Get,N9000A*   N9000Get,PowerSourse*  HMP2020Get,TP_SMDM* TP_SMDMGet,QTableWidget* viewGet):
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

    this->moveToThread(new QThread());
    connect(this->thread(),&QThread::started,this,&thread_SMDM::process_start);
    this->thread()->start();

}

void thread_SMDM::SetIdLink(QSqlQueryModel *LinkGet)
{
    Link = LinkGet;
}

void thread_SMDM::SetEtap(QString etapGet)
{
    etap = etapGet;
}

bool thread_SMDM::writePort()
{

    p_udpSocketOut->writeDatagram(a,50,QHostAddress("192.168.1.233"),30020);

    bool ok = false;
    ok = readDatagram_p_udpSocketOut();

    return ok;

}

void thread_SMDM::setListRegyl(QVector<QVector<double>>* ListRegylGet)
{
    ListRegyl = ListRegylGet;
}

int thread_SMDM::Rele(int B)
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


void thread_SMDM::MyClear()
{
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

    TP->LogClear();
    Micran1->LogClear();
    HMP2020->LogClear();
    N9000->LogClear();
    emit LogClear();

}

void thread_SMDM::Ysilenie_PRD(int ysilenie,int out)
{

    switch (out) {
    case 1:a[26]= static_cast<char>(ysilenie*2+20);break;
    case 2:a[27]= static_cast<char>(ysilenie*2+20);break;
    case 3:a[28]= static_cast<char>(ysilenie*2+20);break;
    case 4:a[29]= static_cast<char>(ysilenie*2+20);break;
    }


    for(int i=0;i<10;i++) //10 запросов
    {
        p_udpSocketOut->writeDatagram(a,50,QHostAddress("192.168.1.233"),30020);

        auto ok = readDatagram_p_udpSocketOut();

        if(ok)
        {
            emit Log("Установлен коэфициент усиления "+ QString::number(ysilenie) +" на выход "+QString::number(out) +".\n");
            return;
        }
    }

    //Если не пришел ответ.
    qDebug() << "ERROR OTVET SMDM";


}

void thread_SMDM::Ysilenie_PRM(int ysilenie,int out)
{
    switch (out) {
    case 1:a[30]= static_cast<char>(ysilenie*2+20);break;
    case 2:a[31]= static_cast<char>(ysilenie*2+20);break;
    case 3:a[32]= static_cast<char>(ysilenie*2+20);break;
    case 4:a[33]= static_cast<char>(ysilenie*2+20);break;
    }



    for(int i=0;i<10;i++) //10 запросов
    {
        p_udpSocketOut->writeDatagram(a,50,QHostAddress("192.168.1.233"),30020);

        auto ok = readDatagram_p_udpSocketOut();

        if(ok)
        {
            emit Log("Установлен коэфициент усиления "+ QString::number(ysilenie) +" на выход "+QString::number(out) +".\n");
            return;
        }
    }

    //Если не пришел ответ.
    qDebug() << "ERROR OTVET SMDM";

}

void thread_SMDM::Rele_Kom_PRD(int in,int out)
{
    count_replayMessage_PRD++;

    if(count_replayMessage_PRD == 1)
    {
        if(in > 10)
        {
            in -=2;
        }
    }

    for(int i=18;i < 26;i++)
    {
        a[i] ='\0';
    }


    for(int i=10; i< 18;i++)
    {
       // qDebug () << "i = " << i << "  in = " << (in+9);

        if(i==(in+1))//+17 Не знаю откуда это число но пусть будет на всякий случай
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
        else
        {
            a[i]='\0';
        }
    }

    bool ok = false;
    ok = writePort();

    if(ok)
    {
        emit Log("Переключен реле коммутатора с  "+ QString::number(in) +" на выход "+QString::number(out) +".\n");
        count_replayMessage_PRD =0;
        return;
    }
    else
    {
        //Если не пришел ответ.
        qDebug() << "ERROR OTVET SMDM PRD : " << count_replayMessage_PRD;

        if(count_replayMessage_PRD > 5)
        {

            count_replayMessage_PRD = 0;
            emit errorMessage("SMDM PRM");
            sem.acquire();
        }
        else
        {
            Rele_Kom_PRD(in,out);
        }

    }

}

void thread_SMDM::Rele_Kom_PRM(int in,int out)
{
    count_replayMessage_PRM++;

    if(count_replayMessage_PRM == 1)
    {
        if(in > 10)
        {
            in -=2;
        }
    }

    for(int i=10;i < 18;i++)
    {
        a[i] ='\0';
    }

    for(int i=18; i< 26;i++)
    {
       // qDebug () << "i = " << i << "  in = " << (in+9);

        if(i==(in+9))//+17 Не знаю откуда это число но пусть будет на всякий случай
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
        else
        {
            a[i]='\0';
        }

    }

    bool ok = false;
    ok = writePort();

    qDebug() << "LOL = " << ok << "N = " << count_replayMessage_PRM;

    if(ok)
    {
        emit Log("Переключен реле коммутатора с  "+ QString::number(in) +" на выход "+QString::number(out) +".\n");
        count_replayMessage_PRM =0;
        return;
    }
    else
    {
        //Если не пришел ответ.
        qDebug() << "ERROR OTVET SMDM PRM : " << count_replayMessage_PRM;

        if(count_replayMessage_PRM > 5)
        {
           emit errorMessage("SMDM PRM");
           count_replayMessage_PRM = 0;
           sem.acquire();
        }
        else
        {
            Rele_Kom_PRM(in,out);
        }
    }
}

void thread_SMDM::Rele_Kom_10MGH_PRD(int out)
{

    for(int i=10;i < 26;i++)
    {
        a[i] = '\0';
    }

    switch (out)
    {
    case 1:
        a[9]=0x01;
        break;
    case 2:
        a[9]=0x02;
        break;
    case 3:
        a[9]=0x03;
        break;
    case 4:
        a[9]=0x04;
        break;
    default:
        break;
    }


    for(int i=0;i<10;i++) //10 запросов
    {
        p_udpSocketOut->writeDatagram(a,50,QHostAddress("192.168.1.233"),30020);

        auto ok = readDatagram_p_udpSocketOut();

        if(ok)
        {
            emit Log("Переключен реле коммутатора с  "+ QString::number(out) +" на 10МГЦ.\n");
            return;
        }
    }

    //Если не пришел ответ.
    qDebug() << "ERROR OTVET SMDM";

}

void thread_SMDM::Rele_Kom_10MGH_PRM(int out)
{

    for(int i=10;i < 26;i++)
    {
        a[i] = '\0';
    }

    switch (out)
    {
    case 1:
        a[9]=0x05;
        break;
    case 2:
        a[9]=0x06;
        break;
    case 3:
        a[9]=0x07;
        break;
    case 4:
        a[9]=0x08;
        break;
    default:
        break;
    }


    for(int i=0;i<10;i++) //10 запросов
    {
        p_udpSocketOut->writeDatagram(a,50,QHostAddress("192.168.1.233"),30020);

        auto ok = readDatagram_p_udpSocketOut();

        if(ok)
        {
            emit Log("Переключен реле коммутатора с  "+ QString::number(out) +" на 10МГЦ.\n");
            return;
        }
    }

    //Если не пришел ответ.
    qDebug() << "ERROR OTVET SMDM";

}

void thread_SMDM::SetUi(  Ui::Power* win_powerGet,
                          Ui::Frequency* win_frequencyGet,
                          Ui::TransferCoefficient* win_transferCoefficientGet,
                          Ui::SwitchingSignalSMDM* win_switchingSignalSMGet)
{

    win_power = win_powerGet;
    win_frequency =win_frequencyGet;
    win_transferCoefficient =win_transferCoefficientGet;
    win_switchingSignalSMDM = win_switchingSignalSMGet;

}

void thread_SMDM::Work()
{


    flagRegylK0=false;
    flagRegyl_A =false;
    flagRegyl_B=false;

    flagEnd = true;
    flag_6_proverki = false;
    TP->flag_6_proverki=false;

    // TP->SetTp();

    MyClear();

    p_udpSocketOut = new QUdpSocket();
    p_udpSocketOut->bind(QHostAddress("192.168.1.128"),30020);

    dateStart = QDateTime::currentDateTime();

    SQL_OneProverka = new QSqlQueryModel();


    emit addBDZapros("INSERT INTO Log (Data,DataStart,IdResult,BlockName) VALUES('"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"','"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(0,0), Qt::EditRole).toString()+"','Источник питания')");
    emit addBDZapros("INSERT INTO Log (Data,DataStart,IdResult,BlockName) VALUES('"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"','"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(0,0), Qt::EditRole).toString()+"','Микран')");
    emit addBDZapros("INSERT INTO Log (Data,DataStart,IdResult,BlockName) VALUES('"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"','"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(0,0), Qt::EditRole).toString()+"','Пульт ТП-СМДМ')");
    emit addBDZapros("INSERT INTO Log (Data,DataStart,IdResult,BlockName) VALUES('"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"','"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(0,0), Qt::EditRole).toString()+"','Блок')");
    emit addBDZapros("INSERT INTO Log (Data,DataStart,IdResult,BlockName) VALUES('"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"','"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(0,0), Qt::EditRole).toString()+"','Анализатор')");
    emit addBDZapros("INSERT INTO Log (Data,DataStart,IdResult,BlockName) VALUES('"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"','"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(0,0), Qt::EditRole).toString()+"','Вывод')");

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

void thread_SMDM::process_start()
{
    count_replayMessage_PRM =0;
    count_replayMessage_PRD =0;
    count_replayMessage =0;


    for(int i=0;i<50;i++)
    {
        a[i] = '\0';
    }

    //шапка
    a[1]=0x66;
    a[2]=static_cast<char>(0x80);
    a[5]=0x32;
    a[6]=0x31;
    a[7]=0x44;

    //ПРД усиление
    a[26]=0*2+20; //В данном случае установил 0 дБ
    a[27]=0*2+20; //В данном случае установил 0 дБ
    a[28]=0*2+20; //В данном случае установил 0 дБ
    a[29]=0*2+20; //В данном случае установил 0 дБ

    //ПРМ усиление
    a[30]=0*2+20; //В данном случае установил 0 дБ
    a[31]=0*2+20; //В данном случае установил 0 дБ
    a[32]=0*2+20; //В данном случае установил 0 дБ
    a[33]=0*2+20; //В данном случае установил 0 дБ


}

void thread_SMDM::StartProverka_1_And_2()
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

    memset(Tok,0,sizeof (Tok));
    memset(Volt,0,sizeof (Volt));

    view->item(0,0)->setBackgroundColor(Qt::yellow);
    view->item(1,0)->setBackgroundColor(Qt::yellow);
    view->reset();

    viPrintf(HMP2020->vi, const_cast<ViString>("VOLTage 10 V\r\n"));
    HMP2020->Log("Установить Напряжение: 10 V.\n");
    viPrintf(HMP2020->vi, const_cast<ViString>("OUTPut 1\r\n"));
    HMP2020->Log("Включить подачу напряжения.\n");
    this->thread()->sleep(5);

    viQueryf(HMP2020->vi, const_cast<ViString>("MEASure:CURRent?\n"), const_cast<ViString>("%T"), Tok);
    HMP2020->Log("Измеренный Ток: "+QString(Tok));

    double P = QString(Tok).toDouble();

    win_power->lcdNumber_10V_I->display(P);

    viQueryf(HMP2020->vi, const_cast<ViString>("MEASure:VOLTage?\n"), const_cast<ViString>("%T"), Volt);
    HMP2020->Log("Измеренное Напряжение: "+QString(Volt));

    P*=QString(Volt).toDouble();

    HMP2020->Log("Посчитанная Мощнасть: "+QString::number(P)+"\n");

    double v = QString(Volt).toDouble();

    win_power->lcdNumber_10V_V->display(v);
    win_power->lcdNumber_10V_Bt->display(P);

    if(P > 35)
    {
        flagGood = false;
        emit addBDZapros("INSERT INTO Power (Data,DataProverki,IdLink,PowerResult,Volt,Tok,Sootvetstvie) VALUES('"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"','"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(1,0), Qt::EditRole).toString()+"','"+QString::number(P)+"','"+QString::number(win_power->lcdNumber_10V_V->value())+"','"+QString::number(win_power->lcdNumber_10V_I->value())+"','Не соответствует')");

        view->item(0,0)->setBackgroundColor(Qt::red);
        view->item(1,0)->setBackgroundColor(Qt::red);
        HMP2020->Log("Напряжение 10 V: Несоответствует\n");
        HMP2020->Log2("Напряжение 10 V: Несоответствует\n");

        win_power->pushButton_10V->setIcon(R3_Red);
        view->reset();
    }
    else
    {
        emit addBDZapros("INSERT INTO Power (Data,DataProverki,IdLink,PowerResult,Volt,Tok,Sootvetstvie) VALUES('"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"','"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(1,0), Qt::EditRole).toString()+"','"+QString::number(P)+"','"+QString::number(win_power->lcdNumber_10V_V->value())+"','"+QString::number(win_power->lcdNumber_10V_I->value())+"','Cоответствует')");


        view->item(0,0)->setBackgroundColor(Qt::green);
        view->item(1,0)->setBackgroundColor(Qt::green);
        HMP2020->Log("Напряжение 10 V: Соответствует\n");
        HMP2020->Log2("Напряжение 10 V: Соответствует\n");

        win_power->pushButton_10V->setIcon(R2_Green);
        view->reset();
    }

    memset(Tok,0,sizeof (Tok));
    memset(Volt,0,sizeof (Volt));

    viPrintf(HMP2020->vi, const_cast<ViString>("OUTPut 0\r\n"));
    HMP2020->Log("Выключить подачу напряжения.\n");

    HMP2020->Log("=================\n");

    viPrintf(HMP2020->vi, const_cast<ViString>("VOLTage 32 V\r\n"));
    HMP2020->Log("Установить Напряжение: 32 V.\n");
    viPrintf(HMP2020->vi, const_cast<ViString>("OUTPut 1\r\n"));
    HMP2020->Log("Включить подачу напряжения.\n");

    this->thread()->sleep(5);

    viQueryf(HMP2020->vi, const_cast<ViString>("MEASure:CURRent?\n"), const_cast<ViString>("%T"), Tok);
    HMP2020->Log("Измеренный Ток: "+QString(Tok));

    P = QString(Tok).toDouble();
    win_power->lcdNumber_32V_I->display(P);

    viQueryf(HMP2020->vi, const_cast<ViString>("MEASure:VOLTage?\n"), const_cast<ViString>("%T"), Volt);
    HMP2020->Log("Измеренное Напряжение: "+QString(Volt));

    P*=QString(Volt).toDouble();

    HMP2020->Log("Посчитанная Мощнасть: "+QString::number(P)+"\n");

    v = QString(Volt).toDouble();

    win_power->lcdNumber_32V_V->display(v);
    win_power->lcdNumber_32V_Bt->display(P);




    if(P > 35)
    {
        flagGood = false;

        emit addBDZapros("INSERT INTO Power (Data,DataProverki,IdLink,PowerResult,Volt,Tok,Sootvetstvie) VALUES('"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"','"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(1,0), Qt::EditRole).toString()+"','"+QString::number(P)+"','"+QString::number(win_power->lcdNumber_32V_V->value())+"','"+QString::number(win_power->lcdNumber_32V_I->value())+"','Не соответствует')");


        view->item(0,0)->setBackgroundColor(Qt::red);
        view->item(1,0)->setBackgroundColor(Qt::red);
        HMP2020->Log("Напряжение 32 V: Несоответствует\n");
        HMP2020->Log2("Напряжение 32 V: Несоответствует\n");
        win_power->pushButton_32V->setIcon(R3_Red);
        view->reset();
    }
    else
    {
        emit addBDZapros("INSERT INTO Power (Data,DataProverki,IdLink,PowerResult,Volt,Tok,Sootvetstvie) VALUES('"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"','"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(1,0), Qt::EditRole).toString()+"','"+QString::number(P)+"','"+QString::number(win_power->lcdNumber_32V_V->value())+"','"+QString::number(win_power->lcdNumber_32V_I->value())+"','Cоответствует')");

        view->item(0,0)->setBackgroundColor(Qt::green);
        view->item(1,0)->setBackgroundColor(Qt::green);
        HMP2020->Log("Напряжение 32 V: Соответствует\n");
        HMP2020->Log2("Напряжение 32 V: Соответствует\n");

        win_power->pushButton_32V->setIcon(R2_Green);

        view->reset();

    }

    memset(Tok,0,sizeof (Tok));
    memset(Volt,0,sizeof (Volt));

    viPrintf(HMP2020->vi, const_cast<ViString>("OUTPut 0\r\n"));
    HMP2020->Log("Включить подачу напряжения.\n");
    HMP2020->Log("=================\n");
    viPrintf(HMP2020->vi, const_cast<ViString>("VOLTage 27 V\r\n"));
    HMP2020->Log("Установить Напряжение: 27 V.\n");
    viPrintf(HMP2020->vi, const_cast<ViString>("OUTPut 1\r\n"));
    HMP2020->Log("Включить подачу напряжения.\n");

    this->thread()->sleep(5);

    viQueryf(HMP2020->vi, const_cast<ViString>("MEASure:CURRent?\n"), const_cast<ViString>("%T"), Tok);
    HMP2020->Log("Измеренный Ток: "+QString(Tok));

    P = QString(Tok).toDouble();
    win_power->lcdNumber_27V_I->display(P);

    viQueryf(HMP2020->vi, const_cast<ViString>("MEASure:VOLTage?\n"), const_cast<ViString>("%T"), Volt);
    HMP2020->Log("Измеренное Напряжение: "+QString(Volt));

    P*=QString(Volt).toDouble();

    HMP2020->Log("Посчитанная Мощнасть: "+QString::number(P)+"\n");

    v = QString(Volt).toDouble();

    win_power->lcdNumber_27V_V->display(v);
    win_power->lcdNumber_27V_Bt->display(P);


    if(P > 35)
    {
        flagGood = false;

        emit addBDZapros("INSERT INTO Power (Data,DataProverki,IdLink,PowerResult,Volt,Tok,Sootvetstvie) VALUES('"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"','"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(1,0), Qt::EditRole).toString()+"','"+QString::number(P)+"','"+QString::number(win_power->lcdNumber_27V_V->value())+"','"+QString::number(win_power->lcdNumber_27V_I->value())+"','Не соответствует')");


        view->item(0,0)->setBackgroundColor(Qt::red);
        view->item(1,0)->setBackgroundColor(Qt::red);
        HMP2020->Log("Напряжение 27 V: Несоответствует\n");
        HMP2020->Log2("Напряжение 27 V: Несоответствует\n");

        win_power->pushButton_27V->setIcon(R3_Red);
        view->reset();
    }
    else
    {
        emit addBDZapros("INSERT INTO Power (Data,DataProverki,IdLink,PowerResult,Volt,Tok,Sootvetstvie) VALUES('"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"','"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(1,0), Qt::EditRole).toString()+"','"+QString::number(P)+"','"+QString::number(win_power->lcdNumber_27V_V->value())+"','"+QString::number(win_power->lcdNumber_27V_I->value())+"','Cоответствует')");

        view->item(0,0)->setBackgroundColor(Qt::green);
        view->item(1,0)->setBackgroundColor(Qt::green);
        HMP2020->Log("Напряжение 27 V: Соответствует\n");
        HMP2020->Log2("Напряжение 27 V: Соответствует\n");

        win_power->pushButton_27V->setIcon(R2_Green);
        view->reset();
    }

    if(flagGood==false)
    {
        emit addBDZapros("INSERT INTO Result (Date,IdLink,ElectricalPower,[Этап]) VALUES('"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(0,0), Qt::EditRole).toString()+"','Не соответствует','"+etap+"')");
        emit addBDZapros("INSERT INTO Result (Date,IdLink,Power,[Этап]) VALUES('"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(1,0), Qt::EditRole).toString()+"','Не соответствует','"+etap+"')");
    }
    else
    {
        emit addBDZapros("INSERT INTO Result (Date,IdLink,ElectricalPower,[Этап]) VALUES('"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(0,0), Qt::EditRole).toString()+"','Cоответствует','"+etap+"')");
        emit addBDZapros("INSERT INTO Result (Date,IdLink,Power,[Этап]) VALUES('"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(1,0), Qt::EditRole).toString()+"','Cоответствует','"+etap+"')");
    }


    HMP2020->Log("Конец 1 и 2 проверки\n");
    HMP2020->Log2("Конец 1 и 2 проверки\n");
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

void thread_SMDM::StartProverka3()
{
    //Старт 3 првоерки
    N9000->Log("=================\n");
    N9000->Log("Старт 3 проверки.\n");
    N9000->Log2("Старт 3 проверки.\n");
    view->item(2,0)->setBackgroundColor(Qt::yellow);
    view->reset();

    flag=true;

    viPrintf(N9000->vi, const_cast<ViString>("AVER:STAT OFF\n"));
    N9000->Log("Отключаем усреднение: AVER.\n");

    viPrintf(N9000->vi, const_cast<ViString>("CALC:MARK:MODE NORM\n"));
    N9000->Log("Установить режим маркера: NORM.\n");


    viPrintf(N9000->vi, const_cast<ViString>("DISP:WIND:TRAC:Y:RLEV 12dBm\n"));
    N9000->Log("Установить Y: 12 dBm.\n");

    //Задает полосу просмотра для отображения сигнала на анализаторе....
    viPrintf(N9000->vi, const_cast<ViString>("FREQuency:STARt 950 MHz\r\n"));
    N9000->Log("Установить начальную частоту просмотра: 950 MHz.\n");

    viPrintf(N9000->vi, const_cast<ViString>("FREQuency:STOP 2150 MHz\r\n"));
    N9000->Log("Установить конечную частоту просмотра: 2150 MHz.\n");


    if(!flag)
    {
        view->item(2,0)->setBackgroundColor(Qt::red);
        view->reset();

        N9000->Log2("Несоответствует\n");

        emit addBDZapros("INSERT INTO Result (Date,IdLink,LimitDeviationOfGain,[Этап]) VALUES('"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(2,0), Qt::EditRole).toString()+"','Не Соответствует','"+etap+"')");
    }
    else
    {
        view->item(2,0)->setBackgroundColor(Qt::green);
        view->reset();

        N9000->Log2("Соответствует\n");

        emit addBDZapros("INSERT INTO Result (Date,IdLink,LimitDeviationOfGain,[Этап]) VALUES('"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(2,0), Qt::EditRole).toString()+"','Cоответствует','"+etap+"')");

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

void thread_SMDM::StartProverka4()
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
        win_frequency->tableWidgetAChH->setColumnWidth(i,50);

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


    date = QDateTime::currentDateTime();
    FlagGoodAchH = true;
    viPrintf(Micran1->vi, const_cast<ViString>("OUTPut:STATe ON\r\n"));
    Micran1->Log("Включить выход ВЧ.\n");

    // Проверка АЧХ Выходы ПРМ (удобней проходить по тому реле где больше проводов)
    for(int i=11;i <=18;i++)
    {
        ProverkaAchH_PRM(i); // i Вход у пульта
    }

    TP->ReleA(0);
    TP->ReleA(22);
    TP->ReleB(0);
    TP->ReleB(22);


    // Проверка АЧХ Входы ПРД
    for(int i=11;i <=18;i++)
    {
        ProverkaAchH(i); // i Вход у пульта
    }

    if(FlagGoodAchH == true)
    {
        view->item(3,0)->setBackgroundColor(Qt::green);
        view->reset();

        N9000->Log2("Соответствует\n");

        emit addBDZapros("INSERT INTO Result (Date,IdLink,Frequency,[Этап]) VALUES('"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(3,0), Qt::EditRole).toString()+"','Соответствует','"+etap+"')");
    }
    else
    {
        view->item(3,0)->setBackgroundColor(Qt::red);
        view->reset();

        N9000->Log2("Несоответствует\n");

        emit addBDZapros("INSERT INTO Result (Date,IdLink,Frequency,[Этап]) VALUES('"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(3,0), Qt::EditRole).toString()+"','Не соответствует','"+etap+"')");
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


bool thread_SMDM::readDatagram_p_udpSocketOut()
{
     bool flag = false;
    this->thread()->msleep(250);

    while(p_udpSocketOut->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(static_cast<int>(p_udpSocketOut->pendingDatagramSize()));
        QHostAddress sender;
        quint16 senderPort;
        p_udpSocketOut->readDatagram(datagram.data(),datagram.size(),&sender,&senderPort);


        qDebug() << "GET UDP Poket :" <<datagram;
        flag = true;

        if(datagram[7] == static_cast<char>(0x03))
        {
            qDebug() << "ERROR KS UDP Poket";
            flag = false;
        }
    }

    if(flag == false)
    {
        qDebug() << "NOT GET UDP Poket";
        return flag;
    }
    else
    {
        return flag;
    }


}




void thread_SMDM::ProverkaAchH_PRM(int A)
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
            Rele_Kom_PRM(A,1); // Соединение реле на коммутаторе СМДМ8-4 "PRM" выход с 1 входом PRM
            ReleA = Rele(6);
            AchH();
            static_cast<void>(ListMin2.append(ListMin)),static_cast<void>(ListMax2.append(ListMax)),ListNEravn2.append(ListNEravn);
            if(A > 10)
                emit addBDZapros("UPDATE GraphPoint SET [Вход]='6',[Выход]='"+QString::number(A-1)+"' WHERE IdResult ='"+Link->data(Link->index(3,0), Qt::EditRole).toString()+"' AND NumberGraph='"+QString::number(ListX.count())+"'");
            else
                emit addBDZapros("UPDATE GraphPoint SET [Вход]='6',[Выход]='"+QString::number(A)+"' WHERE IdResult ='"+Link->data(Link->index(3,0), Qt::EditRole).toString()+"' AND NumberGraph='"+QString::number(ListX.count())+"'");
            break;
        case 1:
            TP->ReleA(7); //Соединение реле на пульте A0-A7
            Rele_Kom_PRM(A,2); // Соединение реле на коммутаторе СМДМ8-4 "PRM" выход с 2 входом PRM
            ReleA = Rele(7);
            AchH();
            static_cast<void>(ListMin2.append(ListMin)),static_cast<void>(ListMax2.append(ListMax)),ListNEravn2.append(ListNEravn);
            if(A > 10)
                emit addBDZapros("UPDATE GraphPoint SET [Вход]='7',[Выход]='"+QString::number(A-1)+"' WHERE IdResult ='"+Link->data(Link->index(3,0), Qt::EditRole).toString()+"' AND NumberGraph='"+QString::number(ListX.count())+"'");
            else
                emit addBDZapros("UPDATE GraphPoint SET [Вход]='7',[Выход]='"+QString::number(A)+"' WHERE IdResult ='"+Link->data(Link->index(3,0), Qt::EditRole).toString()+"' AND NumberGraph='"+QString::number(ListX.count())+"'");
            break;
        case 2:
            TP->ReleA(8); //Соединение реле на пульте A0-A8
            Rele_Kom_PRM(A,3); // Соединение реле на коммутаторе СМДМ8-4 "PRM" выход с 3 входом PRM
            ReleA = Rele(8);
            AchH();
            static_cast<void>(ListMin2.append(ListMin)),static_cast<void>(ListMax2.append(ListMax)),ListNEravn2.append(ListNEravn);
            if(A > 10)
                emit addBDZapros("UPDATE GraphPoint SET [Вход]='8',[Выход]='"+QString::number(A-1)+"' WHERE IdResult ='"+Link->data(Link->index(3,0), Qt::EditRole).toString()+"' AND NumberGraph='"+QString::number(ListX.count())+"'");
            else
                emit addBDZapros("UPDATE GraphPoint SET [Вход]='8',[Выход]='"+QString::number(A)+"' WHERE IdResult ='"+Link->data(Link->index(3,0), Qt::EditRole).toString()+"' AND NumberGraph='"+QString::number(ListX.count())+"'");
            break;
        case 3:
            TP->ReleA(9); //Соединение реле на пульте A0-A9
            Rele_Kom_PRM(A,4); // Соединение реле на коммутаторе СМДМ8-4 "PRM" выход с 4 входом PRMs
            ReleA = Rele(9);
            AchH();
            static_cast<void>(ListMin2.append(ListMin)),static_cast<void>(ListMax2.append(ListMax)),ListNEravn2.append(ListNEravn);
            if(A > 10)
                emit addBDZapros("UPDATE GraphPoint SET [Вход]='9',[Выход]='"+QString::number(A-1)+"' WHERE IdResult ='"+Link->data(Link->index(3,0), Qt::EditRole).toString()+"' AND NumberGraph='"+QString::number(ListX.count())+"'");
            else
                emit addBDZapros("UPDATE GraphPoint SET [Вход]='9',[Выход]='"+QString::number(A)+"' WHERE IdResult ='"+Link->data(Link->index(3,0), Qt::EditRole).toString()+"' AND NumberGraph='"+QString::number(ListX.count())+"'");
            break;
        }
        Vbix++;
    }

}

void thread_SMDM::ProverkaAchH(int A)
{

    TP->ReleA(A);

    if(A>=0 && A<=10)
    {
        ReleA = A;
    }
    else
    {
        ReleA = A-1;
    }

    int Vbix =0;


    //Настройка реле  пульта

    while(Vbix < 4)
    {
        // Написать код для установки выхода на комутатор! (Вход1 -> Выход1)
        switch (Vbix)
        {
        case 0:
            TP->ReleB(6); //Соединение реле на пульте A0-A6
            Rele_Kom_PRD(A,1); // Соединение реле на коммутаторе СМДМ8-4 "PRD" вход с 1 выхдом PRD
            ReleB = Rele(6);
            AchH();
            ListMin2.append(ListMin),ListMax2.append(ListMax),ListNEravn2.append(ListNEravn);
            if(A > 10)
                emit addBDZapros("UPDATE GraphPoint SET [Вход]='"+QString::number(A-1)+"',[Выход]='6' WHERE IdResult ='"+Link->data(Link->index(3,0), Qt::EditRole).toString()+"' AND NumberGraph='"+QString::number(ListX.count())+"'");
            else
                emit addBDZapros("UPDATE GraphPoint SET [Вход]='"+QString::number(A)+"',[Выход]='6' WHERE IdResult ='"+Link->data(Link->index(3,0), Qt::EditRole).toString()+"' AND NumberGraph='"+QString::number(ListX.count())+"'");
            break;
        case 1:
            TP->ReleB(7); //Соединение реле на пульте A0-A7
            Rele_Kom_PRD(A,2); // Соединение реле на коммутаторе СМДМ8-4 "PRD" вход с 2 выхдом PRD
            ReleB = Rele(7);
            AchH();
            ListMin2.append(ListMin),ListMax2.append(ListMax),ListNEravn2.append(ListNEravn);
            if(A > 10)
                emit addBDZapros("UPDATE GraphPoint SET [Вход]='"+QString::number(A-1)+"',[Выход]='7' WHERE IdResult ='"+Link->data(Link->index(3,0), Qt::EditRole).toString()+"' AND NumberGraph='"+QString::number(ListX.count())+"'");
            else
                emit addBDZapros("UPDATE GraphPoint SET [Вход]='"+QString::number(A)+"',[Выход]='7' WHERE IdResult ='"+Link->data(Link->index(3,0), Qt::EditRole).toString()+"' AND NumberGraph='"+QString::number(ListX.count())+"'");
            break;
        case 2:
            TP->ReleB(8); //Соединение реле на пульте A0-A8
            Rele_Kom_PRD(A,3); // Соединение реле на коммутаторе СМДМ8-4 "PRD" вход с 3 выхдом PRD
            ReleB = Rele(8);
            AchH();
            ListMin2.append(ListMin),ListMax2.append(ListMax),ListNEravn2.append(ListNEravn);
            if(A > 10)
                emit addBDZapros("UPDATE GraphPoint SET [Вход]='"+QString::number(A-1)+"',[Выход]='8' WHERE IdResult ='"+Link->data(Link->index(3,0), Qt::EditRole).toString()+"' AND NumberGraph='"+QString::number(ListX.count())+"'");
            else
                emit addBDZapros("UPDATE GraphPoint SET [Вход]='"+QString::number(A)+"',[Выход]='8' WHERE IdResult ='"+Link->data(Link->index(3,0), Qt::EditRole).toString()+"' AND NumberGraph='"+QString::number(ListX.count())+"'");
            break;
        case 3:
            TP->ReleB(9); //Соединение реле на пульте A0-A9
            Rele_Kom_PRD(A,4); // Соединение реле на коммутаторе СМДМ8-4 "PRD" вход с 4 выхдом PRD
            ReleB = Rele(9);
            AchH();
            ListMin2.append(ListMin),ListMax2.append(ListMax),ListNEravn2.append(ListNEravn);
            if(A > 10)
                emit addBDZapros("UPDATE GraphPoint SET [Вход]='"+QString::number(A-1)+"',[Выход]='9' WHERE IdResult ='"+Link->data(Link->index(3,0), Qt::EditRole).toString()+"' AND NumberGraph='"+QString::number(ListX.count())+"'");
            else
                emit addBDZapros("UPDATE GraphPoint SET [Вход]='"+QString::number(A)+"',[Выход]='9' WHERE IdResult ='"+Link->data(Link->index(3,0), Qt::EditRole).toString()+"' AND NumberGraph='"+QString::number(ListX.count())+"'");
            break;
        }

        Vbix++;

    }

    //////////////////////////////////////////////////////////////////////////////

    //Написать код для переключения Трактов коммутатора с 1 до 4 !

    //Написать код для установки выхода на комутатор! (Вход1 -> Выход1)

    //////////////////////////////////////////////////////////////////////////
}

void thread_SMDM::AchH()
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
        viPrintf(N9000->vi, const_cast<ViString>("FREQuency:STARt %d MHz\r\n"),(end-GnFREQuencyStep));
        N9000->Log("Установить начальную частоту просмотра "+QString::number(end-GnFREQuencyStep)+" MHz\n");

        viPrintf(N9000->vi, const_cast<ViString>("FREQuency:STOP %d MHz\r\n"),(end+GnFREQuencyStep));
        N9000->Log("Установить конечную частоту просмотра "+QString::number(end+GnFREQuencyStep)+" MHz\n");



        viPrintf(Micran1->vi, const_cast<ViString>("SOURce:FREQuency:MODE FIXed\r\n"));
        Micran1->Log("Установить MODE: FIXed\n");


        // Set power level value
        viPrintf(Micran1->vi, const_cast<ViString>("SOURce:POWer:LEVel %d DBM\r\n"), GnPower);
        Micran1->Log("Установить Мощность: "+QString::number(GnPower)+" dBm\n");

        viPrintf(Micran1->vi, const_cast<ViString>("SOURce:FREQuency %d MHz\r\n"),end);
        Micran1->Log("Установить Частоту: "+QString::number(end)+" MHz\n");

        // qDebug()<<"FREQuency = "<< end;
        // Set trigger automatic mode
        viPrintf(Micran1->vi, const_cast<ViString>("INITiate:CONTinuous ON\r\n"));

        // Set power out on
        viPrintf(Micran1->vi, const_cast<ViString>("OUTPut:STATe ON\r\n")); // Включить/Выключить выход СВЧ
        Micran1->Log("Включить выход ВЧ.\n");

        // Check errors
        viQueryf(Micran1->vi,const_cast<ViString>("SYSTem:ERRor?\n"),const_cast<ViString>("%T"),buff);
        Micran1->Log("Ошибка: "+QString(buff)+"\n");


        this->thread()->msleep(50);

        viPrintf(N9000->vi, const_cast<ViString>("CALC:MARK:MAX\n"));
        N9000->Log("Захватить максимум.\n");
        viQueryf(N9000->vi, const_cast<ViString>("CALC:MARK:Y?\n"), const_cast<ViString>("%lf"), &dResultY);
        N9000->Log("Считанный Y :"+QString::number(dResultY)+" дб\n");
        viQueryf(N9000->vi, const_cast<ViString>("CALC:MARK:X?\n"), const_cast<ViString>("%lf"), &dResultX);
        N9000->Log("Считанный X :"+QString::number(dResultX/pow(10,6))+" MHz\n");

        x.append(dResultX/pow(10,6));
        dResultY -=GnPower;

        //разкоментировать когда добавлю калибровку
        double ot = ListRegyl->value(ReleB).value(y.count());
        double ot2 = ListRegyl->value(ReleA).value(y.count());
        double PosleRegyl = dResultY - ot - ot2;

        dResultY = PosleRegyl; // положили в переменную число после регулировки для не изменения в запросе переменной

        y.append(PosleRegyl);

        if(PosleRegyl < -60 )
        {
            qDebug() << "ERROR";
            //emit errorMessage("SMDM Ach");
//            this->thread()->wait();
        }




        //log_wr_message("Точка №"+ x.count()+" Частота(X)="+dResultX/pow(10,6)+"Кп(дБ)="+dResultY+"\n");

        auto min1 = std::min_element(y.begin(),y.end());

        auto max1 = std::max_element(y.begin(),y.end());


        min = (*min1);
        max = (*max1);

        NeravnACHX = (*max1)-(*min1);

        // qDebug()<< "Точка №" <<x.count() <<"Частота(X)="<< dResultX/pow(10,6)<<"MГц Кп(дБ)="<<dResultY<<"\n";
        // qDebug()<< "Минимум = "<<*min1 <<" Максимум="<<*max1;
        // qDebug()<< "Неравномерность АЧХ = "<<NeravnACHX;


        end+=GnFREQuencyStep;
        date = QDateTime::currentDateTime();
        emit addBDZapros("INSERT INTO GraphPoint (Data,IdResult,X,Y,NumberGraph,Неравномерность,Минимум,Максимум,DataProverki) VALUES('"+date.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(3,0), Qt::EditRole).toString()+"','"+QString::number(dResultX/pow(10,6))+"','"+QString::number(dResultY)+"','"+QString::number(ListX.count()+1)+"','"+QString::number(NeravnACHX)+"','"+QString::number(min)+"','"+QString::number(max)+"','"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"')");


        //qDebug() << "signal  updateGraph";
        emit updateGraph();

        sem.acquire();

    }

    if(NeravnACHX > ACHH)
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
                if(NeravnACHX > ACHH)
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


void SetCheck(thread_SMDM* threadSM)
{
    int point=0;
    for(int i=0;i<threadSM->win_transferCoefficient->tableWidgetPerestrouka->columnCount();i++)
    {

        for(int j=0;j<threadSM->win_transferCoefficient->tableWidgetPerestrouka->rowCount();j++)
        {

            if(point < 20)
            {
                threadSM->win_transferCoefficient->tableWidgetPerestrouka->item(j,i)->setCheckState(Qt::Unchecked);

                if(threadSM->ListPerestrouka.count()==point)
                {
                    threadSM->win_transferCoefficient->tableWidgetPerestrouka->item(j,i)->setCheckState(Qt::Checked);
                }
                point++;
            }
        }
    }
}

void thread_SMDM::StartProverka5()
{
    emit StartPerestroyka(2);

    //Обнуление реле
    TP->ReleA(0);
    TP->ReleA(22);
    TP->ReleB(0);
    TP->ReleB(22);

    Start5Proverka = true;

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

    viPrintf(N9000->vi, "DISP:WIND:TRAC:Y:RLEV 14dBm\n");
    N9000->Log("Установить Y: 14 dBm.\n");

    viPrintf(N9000->vi, "FREQuency:STARt 1500 MHz\r\n");
    N9000->Log("Установить начальную частоту просмотра: 1500 MHz.\n");

    viPrintf(N9000->vi, "FREQuency:STOP 1600 MHz\r\n");
    N9000->Log("Установить конечную частоту просмотра: 1600 MHz.\n");

    FlagGood5 = true;
    FlagGood5Eshe =true;

    win_transferCoefficient->tableWidgetPerestrouka->setRowCount(10);
    win_transferCoefficient->tableWidgetPerestrouka->setColumnCount(2);

    // emit CreateGraph();
    // sem3.acquire();

    // int point=0;

    emit CreateGraph();
    sem3.acquire();


    TP->ReleA(12);
    for(int j=1;j<=4;j++)
    {
        TP->ReleB(j+5);
        Rele_Kom_PRD(12,j);

        SetCheck(this);
        for(int i=-10;i<=10;i++)
        {
            Ysilenie_PRD(i,j);
            Proverka_5(i);
            emit addBDZapros("UPDATE GraphPoint SET [Вход]='11',[Выход]='"+QString::number(j+5)+"' WHERE IdResult ='"+Link->data(Link->index(4,0), Qt::EditRole).toString()+"' AND NumberGraph='"+QString::number(ListPerestrouka.count()+1)+"'");
        }
        ListPerestrouka.append(y1); //Для отображения на графике подсчет количества графиков по выходам
        Listy1.append(PerestroykaX);
        if(FlagGood5)
        {
            if(ListPerestrouka.count() <=10)
            {
                win_transferCoefficient->tableWidgetPerestrouka->item(ListPerestrouka.count()-1,0)->setBackground(Qt::green);
            }
            else
            {
                win_transferCoefficient->tableWidgetPerestrouka->item(ListPerestrouka.count()-11,1)->setBackground(Qt::green);
            }
        }
        else
        {
            if(ListPerestrouka.count() <=10)
            {
                win_transferCoefficient->tableWidgetPerestrouka->item(ListPerestrouka.count()-1,0)->setBackground(Qt::red);
            }
            else
            {
                win_transferCoefficient->tableWidgetPerestrouka->item(ListPerestrouka.count()-11,1)->setBackground(Qt::red);

            }
        }
        y1.clear();
        PerestroykaX.clear();
    }

    TP->ReleA(0);
    TP->ReleA(22);
    TP->ReleB(0);
    TP->ReleB(22);


    for(int j=1;j<=4;j++)
    {
        TP->ReleA(j+5);


        for(int k=11;k<=18;k++)
        {
            SetCheck(this);
            //четные числа
            if(k%2 == 0)
            {
                TP->ReleB(k);
                Rele_Kom_PRM(k,j);
                for(int i=-10;i<=10;i++)
                {
                    Ysilenie_PRM(i,j);


                    Proverka_5(i);
                    emit addBDZapros("UPDATE GraphPoint SET [Вход]='"+QString::number(j+5)+"',[Выход]='"+QString::number(k-1)+"' WHERE IdResult ='"+Link->data(Link->index(4,0), Qt::EditRole).toString()+"' AND NumberGraph='"+QString::number(ListPerestrouka.count()+1)+"'");

                }
                ListPerestrouka.append(y1); //Для отображения на графике подсчет количества графиков по выходам
                Listy1.append(PerestroykaX);
                if(FlagGood5)
                {
                    if(ListPerestrouka.count() <=10)
                    {
                        win_transferCoefficient->tableWidgetPerestrouka->item(ListPerestrouka.count()-1,0)->setBackground(Qt::green);
                    }
                    else
                    {
                        win_transferCoefficient->tableWidgetPerestrouka->item(ListPerestrouka.count()-11,1)->setBackground(Qt::green);
                    }
                }
                else
                {
                    if(ListPerestrouka.count() <=10)
                    {
                        win_transferCoefficient->tableWidgetPerestrouka->item(ListPerestrouka.count()-1,0)->setBackground(Qt::red);
                    }
                    else
                    {
                        win_transferCoefficient->tableWidgetPerestrouka->item(ListPerestrouka.count()-11,1)->setBackground(Qt::red);

                    }
                }
                y1.clear();
                PerestroykaX.clear();
            }
        }
    }


    TP->ReleA(0);
    TP->ReleA(22);
    TP->ReleB(0);
    TP->ReleB(22);

    if(FlagGood5Eshe == true)
    {
        view->item(4,0)->setBackgroundColor(Qt::green);
        view->reset();
        TP->Log2("Соответствует\n");
        emit addBDZapros("INSERT INTO Result (Date,IdLink,TransferCoefficient,[Этап]) VALUES('"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(4,0), Qt::EditRole).toString()+"','Cоотвутствует','"+etap+"')");

    }
    else
    {
        view->item(4,0)->setBackgroundColor(Qt::red);
        view->reset();
        TP->Log2("Несоответствует\n");
        emit addBDZapros("INSERT INTO Result (Date,IdLink,TransferCoefficient,[Этап]) VALUES('"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(4,0), Qt::EditRole).toString()+"','Не соответствует','"+etap+"')");
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

void thread_SMDM::Proverka_5(int KoefPeredachi)
{
    FlagGood5 =true;

    view->item(4,0)->setBackgroundColor(Qt::yellow);
    view->reset();

    double dResultY = 0;
    double dResultX = 0;
    char buff[9024];


    // Send "Clear status" command
    viPrintf(Micran1->vi, "*CLS\r\n");
    Micran1->Log("Cброс статуса.\n");

    viPrintf(Micran1->vi, "SOURce:FREQuency:MODE FIXed\r\n");
    Micran1->Log("Установить MODE: FIXed\n");

    // Set power level value
    //    viPrintf(Micran1->vi, "SOURce:POWer:LEVel %d DBM\r\n",KoefPeredachi); // Установка Мощности для 1550 МГЦ.
    //    Micran1->Log("Установить Мощность: "+QString::number(KoefPeredachi)+" dBm\n");
    viPrintf(Micran1->vi, "SOURce:POWer:LEVel -10 DBM\r\n"); // Установка Мощности для 1550 МГЦ.
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

    this->thread()->msleep(200);

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


    emit addBDZapros("INSERT INTO GraphPoint (Data,IdResult,X,Y,[Коэфициент передачи],NumberGraph,DataProverki) VALUES('"+date.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(4,0), Qt::EditRole).toString()+"','"+QString::number(dResultX/pow(10,6))+"','"+QString::number(dResultY)+"','"+QString::number(KoefPeredachi)+"','"+QString::number(ListPerestrouka.count()+1)+"','"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"')");

    emit updateGraphPerestrouyka();
    sem3.acquire();

    qDebug() <<" dResultY = "<< dResultY <<" y = " << y1.first()+(y1.count()-1)+2;



    if(y1.count() < 21)
    {
        return ;
    }

    auto interval = qAbs(qAbs(y1.last())-qAbs(y1.first()));


    if(interval  > KF_l && interval < KF_U)
    {
        view->item(4,0)->setBackgroundColor(Qt::green);
        view->reset();
    }
    else
    {
        view->item(4,0)->setBackgroundColor(Qt::red);
        view->reset();

        FlagGood5 = false;
        FlagGood5Eshe = false;
    }


}

void thread_SMDM:: StartProverka6()
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


    flag_6_proverki = true;
    TP->flag_6_proverki = true;

    TP->ReleA(10);
    TP->ReleB(10);
    TP->ReleB(19);

    viPrintf(N9000->vi, "AVER:STAT OFF\n");
    viPrintf(N9000->vi, "DISP:WIND:TRAC:Y:RLEV 14dBm\n");
    N9000->Log("Установить Y: 14 dBm.\n");
    viPrintf(N9000->vi, "CALC:MARK:MODE NORM\n");

    //Задает полосу просмотра для отображения сигнала на анализаторе....
    viPrintf(N9000->vi, "FREQuency:STARt 9 MHz\r\n");
    N9000->Log("Установить начальную частоту просмотра: 9 MHz.\n");

    viPrintf(N9000->vi, "FREQuency:STOP 11 MHz\r\n");
    N9000->Log("Установить конечную частоту просмотра: 11 MHz.\n");


    FlagGood10Mhz = true;



    for(int i=6;i < 10;i++)
    {

        TP->ReleB(i);

        Rele_Kom_10MGH_PRD(i-5); // Соединение реле на коммутаторе CМДДМ8-4 c i входом и ПРД 10 МГЦ
        Proverka10MGU();
        emit addBDZapros("UPDATE GraphPoint SET [Вход]='"+QString::number(i)+"',[Выход]='10 МГЦ' WHERE IdResult ='"+Link->data(Link->index(5,0), Qt::EditRole).toString()+"'AND NumberGraph='"+QString::number(x10MGh.count())+"'");
    }

    TP->ReleA(0);
    TP->ReleB(0);
    TP->ReleB(19);

    for(int i=6;i < 10;i++)
    {

        TP->ReleA(i);
        Rele_Kom_10MGH_PRM(i-5); // Соединение реле на коммутаторе CМДДМ8-4 c i входом и ПРМ 10 МГЦ
        Proverka10MGU();
        emit addBDZapros("UPDATE GraphPoint SET [Вход]='"+QString::number(i)+"',[Выход]='10 МГЦ' WHERE IdResult ='"+Link->data(Link->index(5,0), Qt::EditRole).toString()+"'AND NumberGraph='"+QString::number(x10MGh.count())+"'");
    }

    TP->ReleA(0);
    TP->ReleB(0);
    TP->ReleB(22);

    if(FlagGood10Mhz == true)
    {

        view->item(5,0)->setBackgroundColor(Qt::green);
        view->reset();
        N9000->Log2("Cоответствует\n");
        emit addBDZapros("INSERT INTO Result (Date,IdLink,SwitchingSignalSMDM,[Этап]) VALUES('"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(5,0), Qt::EditRole).toString()+"','Cоответствует','"+etap+"')");

    }
    else
    {

        view->item(5,0)->setBackgroundColor(Qt::red);
        view->reset();
        N9000->Log2("Несоответствует\n");
        emit addBDZapros("INSERT INTO Result (Date,IdLink,SwitchingSignalSMDM,[Этап]) VALUES('"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(5,0), Qt::EditRole).toString()+"','Не соответствует','"+etap+"')");
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


void thread_SMDM::Proverka10MGU()
{
    QPixmap R1_Gray(":/images/circle_gray.png");
    QPixmap R2_Green(":/images/circle_zel.png");
    QPixmap R3_Red(":/images/circle_kras.png");

    view->item(5,0)->setBackgroundColor(Qt::yellow);
    view->reset();

    double dResultY = 0;
    double dResultX = 0;
    char buff[9024];

    // Send "Clear status" command
    viPrintf(Micran1->vi, "*CLS\r\n");
    Micran1->Log("Cброс статуса.\n");

    viPrintf(Micran1->vi, "SOURce:FREQuency:MODE FIXed\r\n");
    Micran1->Log("Установить MODE: FIXed\n");

    // Set power level value
    viPrintf(Micran1->vi, "SOURce:POWer:LEVel -10 DBM\r\n"); // Установка Мощности для 10 МГЦ.
    Micran1->Log("Установить Мощность: -10 dBm\n");

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

    this->thread()->sleep(1);

    viPrintf(N9000->vi, "CALC:MARK:MAX\n");
    N9000->Log("Захватить максимум.\n");

    viQueryf(N9000->vi, "CALC:MARK:Y?\n", "%lf", &dResultY);
    N9000->Log("Считанный Y :"+QString::number(dResultY)+" дб\n");

    viQueryf(N9000->vi, "CALC:MARK:X?\n", "%lf", &dResultX);
    N9000->Log("Считанный X :"+QString::number(dResultX/pow(10,6))+" MHz\n");

    x10MGh.append(dResultX/pow(10,6));

    y10MGh.append(dResultY);

    date = QDateTime::currentDateTime();
    emit addBDZapros("INSERT INTO GraphPoint (Data,IdResult,X,Y,NumberGraph,DataProverki) VALUES('"+date.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(5,0), Qt::EditRole).toString()+"','"+QString::number(dResultX/pow(10,6))+"','"+QString::number(dResultY)+"','"+QString::number(x10MGh.count())+"','"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"')");


    if(x10MGh.count()==1)
    {
        win_switchingSignalSMDM->lcdNumber_PRD->display(dResultY);
    }
    if(x10MGh.count()==2)
    {
        win_switchingSignalSMDM->lcdNumber_PRD_2->display(dResultY);
    }
    if(x10MGh.count()==3)
    {
        win_switchingSignalSMDM->lcdNumber_PRD_3->display(dResultY);
    }
    if(x10MGh.count()==4)
    {
        win_switchingSignalSMDM->lcdNumber_PRD_4->display(dResultY);
    }
    if(x10MGh.count()==5)
    {
        win_switchingSignalSMDM->lcdNumber_PRM->display(dResultY);
    }
    if(x10MGh.count()==6)
    {
        win_switchingSignalSMDM->lcdNumber_PRM_2->display(dResultY);
    }
    if(x10MGh.count()==7)
    {
        win_switchingSignalSMDM->lcdNumber_PRM_3->display(dResultY);
    }
    if(x10MGh.count()==8)
    {
        win_switchingSignalSMDM->lcdNumber_PRM_4->display(dResultY);
    }



    //Примерная мощность для ошибки
    if(dResultY < -20)
    {
        if(x10MGh.count()==1)
        {
            win_switchingSignalSMDM->pushButton_PRD_1->setIcon(R3_Red);
        }
        if(x10MGh.count()==2)
        {
            win_switchingSignalSMDM->pushButton_PRD_2->setIcon(R3_Red);
        }
        if(x10MGh.count()==3)
        {
            win_switchingSignalSMDM->pushButton_PRD_3->setIcon(R3_Red);
        }
        if(x10MGh.count()==4)
        {
            win_switchingSignalSMDM->pushButton_PRD_4->setIcon(R3_Red);
        }
        if(x10MGh.count()==5)
        {
            win_switchingSignalSMDM->pushButton_PRM_1->setIcon(R3_Red);
        }
        if(x10MGh.count()==6)
        {
            win_switchingSignalSMDM->pushButton_PRM_2->setIcon(R3_Red);
        }
        if(x10MGh.count()==7)
        {
            win_switchingSignalSMDM->pushButton_PRM_3->setIcon(R3_Red);
        }
        if(x10MGh.count()==8)
        {
            win_switchingSignalSMDM->pushButton_PRM_4->setIcon(R3_Red);
        }


        view->item(5,0)->setBackgroundColor(Qt::red);
        view->reset();

        FlagGood10Mhz = false;
    }
    else
    {
        if(x10MGh.count()==1)
        {
            win_switchingSignalSMDM->pushButton_PRD_1->setIcon(R2_Green);
        }
        if(x10MGh.count()==2)
        {
            win_switchingSignalSMDM->pushButton_PRD_2->setIcon(R2_Green);
        }
        if(x10MGh.count()==3)
        {
            win_switchingSignalSMDM->pushButton_PRD_3->setIcon(R2_Green);
        }
        if(x10MGh.count()==4)
        {
            win_switchingSignalSMDM->pushButton_PRD_4->setIcon(R2_Green);
        }
        if(x10MGh.count()==5)
        {
            win_switchingSignalSMDM->pushButton_PRM_1->setIcon(R2_Green);
        }
        if(x10MGh.count()==6)
        {
            win_switchingSignalSMDM->pushButton_PRM_2->setIcon(R2_Green);
        }
        if(x10MGh.count()==7)
        {
            win_switchingSignalSMDM->pushButton_PRM_3->setIcon(R2_Green);
        }
        if(x10MGh.count()==8)
        {
            win_switchingSignalSMDM->pushButton_PRM_4->setIcon(R2_Green);
        }

        view->item(5,0)->setBackgroundColor(Qt::green);
        view->reset();
    }
}


void thread_SMDM::StartProverka7()
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
        emit addBDZapros("INSERT INTO Result (Date,IdLink,Ithernet,[Этап]) VALUES('"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(6,0), Qt::EditRole).toString()+"','Не соответствует','"+etap+"')");
    }
    else
    {
        view->item(6,0)->setBackgroundColor(Qt::green);
        view->reset();
        TP->Log2("Соответствует\n");
        emit addBDZapros("INSERT INTO Result (Date,IdLink,Ithernet,[Этап]) VALUES('"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(6,0), Qt::EditRole).toString()+"','Cоответствует','"+etap+"')");
    }

    TP->Log("Конец 7 проверки.\n");
    TP->Log2("Конец 7 проверки.\n");
}

void thread_SMDM::StartProveroka8()
{
    if(Link->rowCount()==8)
    {
        //TP->Log2("=================\n");
        TP->Log2("Старт 8 проверки.\n");

        view->item(7,0)->setBackgroundColor(Qt::yellow);
        view->item(7,0)->setBackgroundColor(Qt::green);
        view->reset();
        date = QDateTime::currentDateTime();
        emit addBDZapros("INSERT INTO Result (Date,IdLink,LongTime,[Этап]) VALUES('"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(7,0), Qt::EditRole).toString()+"','Cоответствует','"+etap+"')");
        TP->Log2("Конец 8 проверки.\n");
    }
}

void thread_SMDM::END()
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
    TP->udpsocket1->close(); // UDP soket Для чтения информации с сети от пульта по A выходу (Пульт -> коммутатор)
    TP->udpsocket2->close(); // UDP soket Для Переключения реле Пульта "A10"выхода по "A18"
    TP->udpsocket3->close();
    TP->udpsocket4->close(); // UDP soket Для чтения информации с сети от пульта по B выходу (Пульт <- коммутатор)
    TP->udpsocket5->close(); // UDP soket Для Переключения реле Пульта "В1"выход до B"9"
    TP->udpsocket6->close(); // UDP soket Для Переключения реле Пульта B"10" выхода до B"18"
    TP->udpsocket7->close();
    TP->udpsocket8->close(); // UDP soket Для Переключения реле Пульта A0 соединить с A выход
    TP->udpsocket9->close(); // UDP soket Для Переключения реле Пульта B0 соединить с B выход
    p_udpSocketOut->close(); // UDP soket для Переключения каналов реле Коммутатора СМ16-4
    */

    emit end();

}

void thread_SMDM::ConnectDevice()
{
    p_udpSocketOut = new QUdpSocket();
    p_udpSocketOut->bind(30020);
}


void thread_SMDM::StratRegyl()
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


    emit addBDZapros("INSERT INTO Result (Date,IdLink,[Этап]) VALUES('"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(3,0), Qt::EditRole).toString()+"','"+etap+"')");


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

void thread_SMDM::ProverkaRegyl()
{
    AchH();
}

void thread_SMDM::AchHRegyl()
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
        emit addBDZapros("INSERT INTO GraphPoint (Data,IdResult,X,Y,NumberGraph,Неравномерность,Минимум,Максимум,DataProverki) VALUES('"+date.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(3,0), Qt::EditRole).toString()+"','"+QString::number(dResultX/pow(10,6))+"','"+QString::number(dResultY)+"','K0','"+QString::number(NeravnACHX)+"','"+QString::number(min)+"','"+QString::number(max)+"','"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"')");

    }

    if(NeravnACHX >4)
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

