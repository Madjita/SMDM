#include "thread_sm.h"

#include <mainwindow.h>
#include <QPalette>
#include <QMessageBox>
#include <QtGui>


thread_SM::thread_SM()
{

}

void thread_SM::process_start()
{

    qDebug () << "process_start";

}



void thread_SM::perecl(char* a,int index,int out)
{
    switch (out) {
        case 1:
        {
            a[index]=0x01;
            break;
        }
        case 2:
        {
            a[index]=0x02;
            break;
        }
        case 3:
        {
            a[index]=0x03;
            break;
        }
        case 4:
        {
            a[index]=0x04;
            break;
        }

    }


}

void thread_SM::Ypravlenie(int in, int out)
{
    char a[41];

    for(int i=0;i<41;i++)
    {
        a[i] = '\0';
    }



    switch (in)
    {

        case 1:
        {
            perecl(a,1,out);
            break;
        }
        case 2:
        {
            perecl(a,2,out);
            break;
        }
        case 3:
        {
            perecl(a,3,out);
            break;
        }
        case 4:
        {
            perecl(a,4,out);
            break;
        }
        case 5:
        {
            perecl(a,5,out);
            break;
        }
        case 6:
        {
            perecl(a,6,out);
            break;
        }
        case 7:
        {
            perecl(a,7,out);
            break;
        }
        case 8:
        {
            perecl(a,8,out);
            break;
        }
        case 9:
        {
            perecl(a,9,out);
            break;
        }
        case 10:
        {
            perecl(a,10,out);
            break;
        }
        case 11:
        {
            perecl(a,11,out);
            break;
        }
        case 12:
        {
            perecl(a,12,out);
            break;
        }
        case 13:
        {
            perecl(a,13,out);
            break;
        }
        case 14:
        {
            perecl(a,14,out);
            break;
        }
        case 15:
        {
            perecl(a,15,out);
            break;
        }
        case 16:
        {
            perecl(a,16,out);
            break;
        }


    }





    //p_udpSocketOut->writeDatagram(a,41,QHostAddress("192.168.1.100"),30020);
    emit Log("[СМ16-4] Переключение входа "+ QString::number(in) +" на выход "+QString::number(out) +".\n");
}


void thread_SM::setListRegyl(QVector<QVector<double>>* ListRegylGet)
{
    ListRegyl = ListRegylGet;
}

thread_SM::thread_SM(Micran_Gen*  Micran1Get,N9000A*   N9000Get,PowerSourse*  HMP2020Get,TP_SMDM* TP_SMDMGet,QTableWidget* viewGet):
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


    //SQL_OneProverknew QSqlQueryModel();


    p_udpSocketOut = new QUdpSocket();
    p_udpSocketOut->bind(30020);



    this->moveToThread(new QThread());

    connect(this->thread(),&QThread::started,this,&thread_SM::process_start);

    this->thread()->start();


}

thread_SM::~thread_SM()
{
    p_udpSocketOut->close();
}



void thread_SM::Set_log_text(QTextEdit* text)
{
    log_text = text;
}



void thread_SM::SetProv(int r, int c)
{

    if( view->item(r,c)->flags() != Qt::NoItemFlags)
    {
        if(view->item(r,c)->checkState()==Qt::Checked)
        {
            if(ListProv.value(r)!=-1)
            {
                ListProv[r] = true;
            }
        }
        else
        {
            if(ListProv.value(r)!=-1)
            {
                ListProv[r] = false;
            }
        }
    }

}

void thread_SM::SetUi(  Ui::Power* win_powerGet,
                        Ui::Frequency* win_frequencyGet,
                        Ui::TransferCoefficient* win_transferCoefficientGet,
                        Ui::SwitchingSignalSM* win_switchingSignalSMGet)
{

    win_power = win_powerGet;
    win_frequency =win_frequencyGet;
    win_transferCoefficient =win_transferCoefficientGet;
    win_switchingSignalSM = win_switchingSignalSMGet;

    QPixmap R1_Gray(":/images/circle_gray.png");

    //reset
    win_power->lcdNumber_10V_Bt->display(0);
    win_power->lcdNumber_10V_I->display(0);
    win_power->lcdNumber_10V_V->display(0);
    win_power->lcdNumber_27V_Bt->display(0);
    win_power->lcdNumber_27V_I->display(0);
    win_power->lcdNumber_27V_V->display(0);
    win_power->lcdNumber_32V_Bt->display(0);
    win_power->lcdNumber_32V_I->display(0);
    win_power->lcdNumber_32V_V->display(0);

    win_power->pushButton_10V->setIcon(R1_Gray);
    win_power->pushButton_27V->setIcon(R1_Gray);
    win_power->pushButton_32V->setIcon(R1_Gray);

    win_frequency->ProverkaGraph->clearGraphs();

    win_frequency->tableWidgetAChH->clear();

    win_transferCoefficient->Graph->clearGraphs();
    win_transferCoefficient->tableWidgetPerestrouka->clear();

    win_switchingSignalSM->lcdNumber->display(0);
    win_switchingSignalSM->lcdNumber_2->display(0);
    win_switchingSignalSM->lcdNumber_3->display(0);
    win_switchingSignalSM->lcdNumber_4->display(0);

    win_switchingSignalSM->pushButton_1->setIcon(R1_Gray);
    win_switchingSignalSM->pushButton_2->setIcon(R1_Gray);
    win_switchingSignalSM->pushButton_3->setIcon(R1_Gray);
    win_switchingSignalSM->pushButton_4->setIcon(R1_Gray);


    emit CreateGraphfrenq();

}


void thread_SM::SetIdLink(QSqlQueryModel *LinkGet)
{
    Link = LinkGet;
}

void thread_SM::SetEtap(QString etapGet)
{
    etap = etapGet;
}


void thread_SM::MyClear()
{


    TP->LogClear();
    Micran1->LogClear();
    HMP2020->LogClear();
    N9000->LogClear();



    emit LogClear();

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


}


void thread_SM::Ysilenie(int ysilenie,int out)
{



    char a[50];

    for(int i=0; i < 50;i++)
    {
        a[i]=NULL;
    }

    a[0]=0x64;
    a[3]=0x80;
    a[4]=0x32;
    a[6]=0x31;
    a[7]=0x44;
    a[8]=0x05;
    a[9]=0x00; // 10МГц

    /* a[27] = 0x3f;
    a[28] = 0x3f;
    a[29] = 0x3f;
    a[30] = 0x3f;


   a[0]=0x64;
    a[2]=0x80;
    a[5]=0x12;
    a[6]=0x31;
    a[7]=0x44;
    a[8]=0x01;
    a[9]=0x02;
    */

    a[26]=ysilenie*2+20;
    a[27]=ysilenie*2+20;
    a[28]=ysilenie*2+20;
    a[29]=ysilenie*2+20;

   /* for(int i=0;i<18;i++)
    {
        if(i==(out+9))
        {
            a[i]= ysilenie*2+20;
        }
    }*/


    for(int i=10; i< 26;i++)
    {
      // if(i==(in+17))
      //  {
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
      // }

    }

    p_udpSocketOut->writeDatagram(a,50,QHostAddress("192.168.1.100"),30020);

    emit Log("Установлен коэфициент усиления "+ QString::number(ysilenie) +" на выход "+QString::number(out) +".\n");
}

void thread_SM::Rele_Kom(int in,int out)
{


  //  QString str = "\00d\80\002\001D\05\00\0"+QString::number(out)+"\0"+QString::number(out)+"\0"+QString::number(out)+"\0"+QString::number(out)+"\0"+QString::number(out)+"\0"+QString::number(out)+"\0"+QString::number(out)+"\0"+QString::number(out)+"\0"+QString::number(out)+"\0"+QString::number(out)+"\0"+QString::number(out)+"\0"+QString::number(out)+"\0"+QString::number(out)+"\0"+QString::number(out)+"\0"+QString::number(out)+"\0"+QString::number(out)+"????\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00\00";



    if(in > 10)
    {
        in -=2;
    }

    char a[50];

    for(int i=0; i < 50;i++)
    {
        a[i]=NULL;
    }

    a[0]=0x64;
    a[3]=0x80;
    a[4]=0x32;
    a[6]=0x31;
    a[7]=0x44;
    a[8]=0x05;
    a[9]=0x00; // 10МГц


    int k = 1*2+20;



    a[26]= 1*2+20; //QString("%1").arg(k,0,16).toUpper();
    a[27]=1*2+20;
    a[28]=1*2+20;
    a[29]=1*2+20;


    for(int i=10; i< 26;i++)
    {
        qDebug () << "i = " << i << "  in = " << (in+9);

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
            a[i]=NULL;
       }



    }


   p_udpSocketOut->writeDatagram(a,50,QHostAddress("192.168.1.100"),30020);


   //p_udpSocketOut->writeDatagram(str.toUtf8(),str.count(),QHostAddress("192.168.1.100"),30020);


    emit Log("Переключен реле коммутатора с  "+ QString::number(in) +" на выход "+QString::number(out) +".\n");

}

void thread_SM::Rele_Kom_10MGH(int out)
{

    char a[50];

    for(int i=0; i < 50;i++)
    {
        a[i]=NULL;
    }

    a[0]=0x64;
    a[3]=0x80;
    a[4]=0x32;
    a[6]=0x31;
    a[7]=0x44;
    a[8]=0x05;
    a[9]=0x00; // 10МГц


    switch (out)
    {
    case 1:
        a[9]=0x01; // 10МГц
        break;
    case 2:
        a[9]=0x02; // 10МГц
        break;
    case 3:
        a[9]=0x03; // 10МГц
        break;
    case 4:
        a[9]=0x04; // 10МГц
        break;
    default:
        break;
    }


    a[26]=0*2+20;
    a[27]=0*2+20;
    a[28]=0*2+20;
    a[29]=0*2+20;



   p_udpSocketOut->writeDatagram(a,50,QHostAddress("192.168.1.100"),30020);

   emit Log("Переключен реле коммутатора с  "+ QString::number(out) +" на 10МГЦ.\n");
}

void thread_SM::Work()
{




    flagRegylK0=false;
    flagRegyl_A =false;
    flagRegyl_B=false;

    flagEnd = true;
    flag_6_proverki = false;
    TP->flag_6_proverki=false;




    dateStart = QDateTime::currentDateTime();


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
    ListPerestrouka.clear();
    Listy1.clear();






    emit SQL_add("INSERT INTO Log (Data,DataStart,IdResult,BlockName) VALUES('"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"','"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(0,0), Qt::EditRole).toString()+"','Источник питания')");
    emit SQL_add("INSERT INTO Log (Data,DataStart,IdResult,BlockName) VALUES('"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"','"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(0,0), Qt::EditRole).toString()+"','Микран')");
    emit SQL_add("INSERT INTO Log (Data,DataStart,IdResult,BlockName) VALUES('"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"','"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(0,0), Qt::EditRole).toString()+"','Пульт ТП-СМДМ')");
    emit SQL_add("INSERT INTO Log (Data,DataStart,IdResult,BlockName) VALUES('"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"','"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(0,0), Qt::EditRole).toString()+"','Блок')");
    emit SQL_add("INSERT INTO Log (Data,DataStart,IdResult,BlockName) VALUES('"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"','"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(0,0), Qt::EditRole).toString()+"','Анализатор')");
    emit SQL_add("INSERT INTO Log (Data,DataStart,IdResult,BlockName) VALUES('"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"','"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(0,0), Qt::EditRole).toString()+"','Вывод')");

    emit SetDataStart(dateStart);

        qDebug() << "Work thread_SM 2 ";

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

void thread_SM::StartProverka_1_And_2()
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
    QString Tok;
    QString Volt;
    bool flagGood = true;

    view->item(0,0)->setBackgroundColor(Qt::yellow);
    view->item(1,0)->setBackgroundColor(Qt::yellow);
    view->reset();

    HMP2020->write_VOLTage(10);

    HMP2020->Log("Установить Напряжение: 10 V.\n");

    HMP2020->write_OUTPut(true);

    HMP2020->Log("Включить подачу напряжения.\n");

    Sleep(1000);

    Tok = HMP2020->query_MEASure_CURRent();

    HMP2020->Log("Измеренный Ток: "+Tok);

    double P = Tok.toDouble();

    Volt = HMP2020->query_MEASure_VOLTage();

    HMP2020->Log("Измеренное Напряжение: "+Volt);

    P*=Volt.toDouble();

    HMP2020->Log("Посчитанная Мощнасть: "+QString::number(P)+"\n");

    win_power->lcdNumber_10V_I->display(Tok.toDouble());
    win_power->lcdNumber_10V_V->display(Volt.toDouble());
    win_power->lcdNumber_10V_Bt->display(P);

    if(P > 35)
    {
        emit SQL_add("INSERT INTO Power (Data,DataProverki,IdLink,PowerResult,Volt,Tok,Sootvetstvie) VALUES('"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"','"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(1,0), Qt::EditRole).toString()+"','"+QString::number(P)+"','"+QString::number(win_power->lcdNumber_10V_V->value())+"','"+QString::number(win_power->lcdNumber_10V_I->value())+"','Не соответствует')");

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

        emit SQL_add("INSERT INTO Power (Data,DataProverki,IdLink,PowerResult,Volt,Tok,Sootvetstvie) VALUES('"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"','"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(1,0), Qt::EditRole).toString()+"','"+QString::number(P)+"','"+QString::number(win_power->lcdNumber_10V_V->value())+"','"+QString::number(win_power->lcdNumber_10V_I->value())+"','Cоответствует')");

        view->item(0,0)->setBackgroundColor(Qt::green);
        view->item(1,0)->setBackgroundColor(Qt::green);
        HMP2020->Log("Напряжение 10 V: Соответствует\n");
        HMP2020->Log2("Напряжение 10 V: Соответствует\n");

        win_power->pushButton_10V->setIcon(R2_Green);
        view->reset();
    }

    HMP2020->write_OUTPut(false);
    HMP2020->Log("Выключить подачу напряжения.\n");

    HMP2020->Log("=================\n");

    HMP2020->write_VOLTage(32);
    HMP2020->Log("Установить Напряжение: 32 V.\n");
    HMP2020->write_OUTPut(true);
    HMP2020->Log("Включить подачу напряжения.\n");

    Sleep(1000);

    Tok = HMP2020->query_MEASure_CURRent();
    HMP2020->Log("Измеренный Ток: "+Tok);

    P = Tok.toDouble();

    Volt = HMP2020->query_MEASure_VOLTage();
    HMP2020->Log("Измеренное Напряжение: "+Volt);

    P*=Volt.toDouble();

    HMP2020->Log("Посчитанная Мощность: "+QString::number(P)+"\n");


    win_power->lcdNumber_32V_I->display(Tok.toDouble());
    win_power->lcdNumber_32V_V->display(Volt.toDouble());
    win_power->lcdNumber_32V_Bt->display(P);




    if(P > 35)
    {
        flagGood = false;

        emit SQL_add("INSERT INTO Power (Data,DataProverki,IdLink,PowerResult,Volt,Tok,Sootvetstvie) VALUES('"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"','"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(1,0), Qt::EditRole).toString()+"','"+QString::number(P)+"','"+QString::number(win_power->lcdNumber_32V_V->value())+"','"+QString::number(win_power->lcdNumber_32V_I->value())+"','Не соответствует')");

        view->item(0,0)->setBackgroundColor(Qt::red);
        view->item(1,0)->setBackgroundColor(Qt::red);
        HMP2020->Log("Напряжение 32 V: Несоответствует\n");
        HMP2020->Log2("Напряжение 32 V: Несоответствует\n");
        win_power->pushButton_32V->setIcon(R3_Red);
        view->reset();

    }
    else
    {
        emit SQL_add("INSERT INTO Power (Data,DataProverki,IdLink,PowerResult,Volt,Tok,Sootvetstvie) VALUES('"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"','"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(1,0), Qt::EditRole).toString()+"','"+QString::number(P)+"','"+QString::number(win_power->lcdNumber_32V_V->value())+"','"+QString::number(win_power->lcdNumber_32V_I->value())+"','Cоответствует')");

        view->item(0,0)->setBackgroundColor(Qt::green);
        view->item(1,0)->setBackgroundColor(Qt::green);
        HMP2020->Log("Напряжение 32 V: Соответствует\n");
        HMP2020->Log2("Напряжение 32 V: Соответствует\n");

        win_power->pushButton_32V->setIcon(R2_Green);

        view->reset();

    }

    HMP2020->write_OUTPut(false);
    HMP2020->Log("Включить подачу напряжения.\n");
    HMP2020->Log("=================\n");
    HMP2020->write_VOLTage(27);
    HMP2020->Log("Установить Напряжение: 27 V.\n");
    HMP2020->write_OUTPut(true);
    HMP2020->Log("Включить подачу напряжения.\n");

    Sleep(1000);

    Tok = HMP2020->query_MEASure_CURRent();
    HMP2020->Log("Измеренный Ток: "+Tok);

    P = Tok.toDouble();

    Volt = HMP2020->query_MEASure_VOLTage();
    HMP2020->Log("Измеренное Напряжение: "+Volt);

    P*=Volt.toDouble();

    HMP2020->Log("Посчитанная Мощнасть: "+QString::number(P)+"\n");


    win_power->lcdNumber_27V_I->display(Tok.toDouble());
    win_power->lcdNumber_27V_V->display(Volt.toDouble());
    win_power->lcdNumber_27V_Bt->display(P);


    if(P > 35)
    {
        flagGood = false;

        emit SQL_add("INSERT INTO Power (Data,DataProverki,IdLink,PowerResult,Volt,Tok,Sootvetstvie) VALUES('"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"','"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(1,0), Qt::EditRole).toString()+"','"+QString::number(P)+"','"+QString::number(win_power->lcdNumber_27V_V->value())+"','"+QString::number(win_power->lcdNumber_27V_I->value())+"','Не соответствует')");

        view->item(0,0)->setBackgroundColor(Qt::red);
        view->item(1,0)->setBackgroundColor(Qt::red);
        HMP2020->Log("Напряжение 27 V: Несоответствует\n");
        HMP2020->Log2("Напряжение 27 V: Несоответствует\n");

        win_power->pushButton_27V->setIcon(R3_Red);
        view->reset();

    }
    else
    {
        emit SQL_add("INSERT INTO Power (Data,DataProverki,IdLink,PowerResult,Volt,Tok,Sootvetstvie) VALUES('"+QDateTime::currentDateTime().toString("dd.MM.yyyy  hh:mm:ss")+"','"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(1,0), Qt::EditRole).toString()+"','"+QString::number(P)+"','"+QString::number(win_power->lcdNumber_27V_V->value())+"','"+QString::number(win_power->lcdNumber_27V_I->value())+"','Cоответствует')");

        view->item(0,0)->setBackgroundColor(Qt::green);
        view->item(1,0)->setBackgroundColor(Qt::green);
        HMP2020->Log("Напряжение 27 V: Соответствует\n");
        HMP2020->Log2("Напряжение 27 V: Соответствует\n");

        win_power->pushButton_27V->setIcon(R2_Green);
        view->reset();

    }

    if(flagGood==false)
    {
        emit SQL_add("INSERT INTO Result (Date,IdLink,ElectricalPower,[Этап]) VALUES('"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(0,0), Qt::EditRole).toString()+"','Не соответствует','"+etap+"')");
        emit SQL_add("INSERT INTO Result (Date,IdLink,Power,[Этап]) VALUES('"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(1,0), Qt::EditRole).toString()+"','Не соответствует','"+etap+"')");
    }
    else
    {
        emit SQL_add("INSERT INTO Result (Date,IdLink,ElectricalPower,[Этап]) VALUES('"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(0,0), Qt::EditRole).toString()+"','Cоответствует','"+etap+"')");
        emit SQL_add("INSERT INTO Result (Date,IdLink,Power,[Этап]) VALUES('"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(1,0), Qt::EditRole).toString()+"','Cоответствует','"+etap+"')");
    }

    HMP2020->Log("Конец 1 и 2 проверки\n");
    HMP2020->Log2("Конец 1 и 2 проверки\n");
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

void thread_SM::StartProverka3()
{
    //Старт 3 првоерки
    N9000->Log("=================\n");
    N9000->Log("Старт 3 проверки.\n");
    N9000->Log2("Старт 3 проверки.\n");
    view->item(2,0)->setBackgroundColor(Qt::yellow);
    view->reset();

    flag=true;


    this->thread()->msleep(100);

    N9000->write_AVER_STAT(false);
    N9000->Log("Отключаем усреднение: AVER.\n");

    N9000->write_CALC_MARK_MODE("NORM");
    N9000->Log("Установить режим маркера: NORM.\n");

    N9000->write_DISP_WIND_TRAC_Y_RLEV(12);
    N9000->Log("Установить Y: 12 dBm.\n");

    //Задает полосу просмотра для отображения сигнала на анализаторе....
    N9000->write_FREQuency_STARt(950,"MHz");
    N9000->Log("Установить начальную частоту просмотра: 950 MHz.\n");

    N9000->write_FREQuency_STOP(2150,"MHz");
    N9000->Log("Установить конечную частоту просмотра: 2150 MHz.\n");


    if(!flag)
    {
        view->item(2,0)->setBackgroundColor(Qt::red);
        view->reset();

        N9000->Log2("Несоответствует\n");

        emit SQL_add("INSERT INTO Result (Date,IdLink,LimitDeviationOfGain,[Этап]) VALUES('"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(2,0), Qt::EditRole).toString()+"','Не Соответствует','"+etap+"')");
    }
    else
    {
        view->item(2,0)->setBackgroundColor(Qt::green);
        view->reset();

        N9000->Log2("Соответствует\n");

        emit SQL_add("INSERT INTO Result (Date,IdLink,LimitDeviationOfGain,[Этап]) VALUES('"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(2,0), Qt::EditRole).toString()+"','Cоответствует','"+etap+"')");

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

void thread_SM::StartProverka4()
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

            qDebug() << "win_frequency->tableWidgetAChH = " <<  point;

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



    Micran1->write_OUTPut_STATe(true);
    Micran1->Log("Включить выход ВЧ.\n");

    for(int i=1;i<=4;i++)
    {
        Ysilenie(0,i);
        //Дописать Вывод в консоль блока его команды
    }


    FlagGoodAchH = true;


    qDebug () << " Проверка 4 : Начало обхода реле А";

    for(int i=1;i <=18;i++) //Поменять i на 1
    {
        if(i==9 || i==10)
        {
            TP->ReleA(0);
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

        emit SQL_add("INSERT INTO Result (Date,IdLink,Frequency,[Этап]) VALUES('"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(3,0), Qt::EditRole).toString()+"','Соответствует','"+etap+"')");
    }
    else
    {
        view->item(3,0)->setBackgroundColor(Qt::red);
        view->reset();

        N9000->Log2("Несоответствует\n");

        emit SQL_add("INSERT INTO Result (Date,IdLink,Frequency,[Этап]) VALUES('"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(3,0), Qt::EditRole).toString()+"','Не соответствует','"+etap+"')");
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


int thread_SM::Rele(int B)
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


void thread_SM::ProverkaAchH(int A)
{
    qDebug () << "ProverkaAchH  " << A;

    int trakt=0;

    TP->ReleA(A);

    if(A>=0 && A<=10)
    {
        ReleA = A;
    }
    else
    {
        ReleA = A - 1;
    }


    int Vbix =0;

    //Настройка реле  пульта

    while(Vbix <= 4)
    {
        // Написать код для установки выхода на комутатор! (Вход1 -> Выход1)
        switch (Vbix)
        {
        case 0:
        {
            TP->ReleB(6); //Соединение реле на пульте B0-B6
            Rele_Kom(A,1); // Соединение реле на коммутаторе СМ16-4 "A" вход с 1 выходом
            ReleB = Rele(6);
            AchH(A,1);
            ListMin2.append(ListMin);
            ListMax2.append(ListMax);
            ListNEravn2.append(ListNEravn);
            if(A > 10)
                emit SQL_add("UPDATE GraphPoint SET [Вход]='"+QString::number(A-1)+"',[Выход]='6' WHERE IdResult ='"+Link->data(Link->index(3,0), Qt::EditRole).toString()+"' AND NumberGraph='"+QString::number(ListX.count())+"'");
            else
                emit SQL_add("UPDATE GraphPoint SET [Вход]='"+QString::number(A)+"',[Выход]='6' WHERE IdResult ='"+Link->data(Link->index(3,0), Qt::EditRole).toString()+"' AND NumberGraph='"+QString::number(ListX.count())+"'");
            break;
        }
        case 1:
        {
            TP->ReleB(7); //Соединение реле на пульте B0-B7
            Rele_Kom(A,2); // Соединение реле на коммутаторе СМ16-4 "A" вход с 2 выходом
            ReleB = Rele(7);
            AchH(A,2);
            ListMin2.append(ListMin);
            ListMax2.append(ListMax);
            ListNEravn2.append(ListNEravn);
            if(A > 10)
                emit SQL_add("UPDATE GraphPoint SET [Вход]='"+QString::number(A-1)+"',[Выход]='7' WHERE IdResult ='"+Link->data(Link->index(3,0), Qt::EditRole).toString()+"' AND NumberGraph='"+QString::number(ListX.count())+"'");
            else
                emit SQL_add("UPDATE GraphPoint SET [Вход]='"+QString::number(A)+"',[Выход]='7' WHERE IdResult ='"+Link->data(Link->index(3,0), Qt::EditRole).toString()+"' AND NumberGraph='"+QString::number(ListX.count())+"'");
            break;
        }
        case 2:
        {
            TP->ReleB(8); //Соединение реле на пульте B0-B8
            Rele_Kom(A,3); // Соединение реле на коммутаторе СМ16-4 "A" вход с 3 выходом
            ReleB = Rele(8);
            AchH(A,3);
            ListMin2.append(ListMin);
            ListMax2.append(ListMax);
            ListNEravn2.append(ListNEravn);
            if(A > 10)
                emit SQL_add("UPDATE GraphPoint SET [Вход]='"+QString::number(A-1)+"',[Выход]='8' WHERE IdResult ='"+Link->data(Link->index(3,0), Qt::EditRole).toString()+"' AND NumberGraph='"+QString::number(ListX.count())+"'");
            else
                emit SQL_add("UPDATE GraphPoint SET [Вход]='"+QString::number(A)+"',[Выход]='8' WHERE IdResult ='"+Link->data(Link->index(3,0), Qt::EditRole).toString()+"' AND NumberGraph='"+QString::number(ListX.count())+"'");
            break;
        }
        case 3:
        {
            TP->ReleB(9); //Соединение реле на пульте B0-B9
            Rele_Kom(A,4); // Соединение реле на коммутаторе СМ16-4 "A" вход с 4 выходом
            ReleB = Rele(9);
            AchH(A,4);
            ListMin2.append(ListMin);
            ListMax2.append(ListMax);
            ListNEravn2.append(ListNEravn);
            if(A > 10)
                emit SQL_add("UPDATE GraphPoint SET [Вход]='"+QString::number(A-1)+"',[Выход]='9' WHERE IdResult ='"+Link->data(Link->index(3,0), Qt::EditRole).toString()+"' AND NumberGraph='"+QString::number(ListX.count())+"'");
            else
                emit SQL_add("UPDATE GraphPoint SET [Вход]='"+QString::number(A)+"',[Выход]='9' WHERE IdResult ='"+Link->data(Link->index(3,0), Qt::EditRole).toString()+"' AND NumberGraph='"+QString::number(ListX.count())+"'");
            break;
        }
        }
        Vbix++;
    }


    //////////////////////////////////////////////////////////////////////////////

    //Написать код для переключения Трактов коммутатора с 1 до 4 !

    //Написать код для установки выхода на комутатор! (Вход1 -> Выход1)

    //////////////////////////////////////////////////////////////////////////
}


int error=0;

void thread_SM::AchH(int in,int out)
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


    qDebug () << "LOLOL " << win_frequency->tableWidgetAChH->columnCount();
    qDebug () << "LOLOL2 " << win_frequency->tableWidgetAChH->rowCount();

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
      //  viQueryf(Micran1->vi,"SYSTem:ERRor?\n","%T",buff);
      //  Micran1->Log("Ошибка: "+QString(buff)+"\n");


        qDebug () << GnFREQuencyEnd << " GnFREQuencyEnd > = end " << end;

        //Задает полосу просмотра для отображения сигнала на анализаторе....
        viPrintf(N9000->vi, "FREQuency:STARt %d MHz\r\n",(end-GnFREQuencyStep));
        N9000->Log("Установить начальную частоту просмотра "+QString::number(end-GnFREQuencyStep)+" MHz\n");

        viPrintf(N9000->vi, "FREQuency:STOP %d MHz\r\n",(end+GnFREQuencyStep));
        N9000->Log("Установить конечную частоту просмотра "+QString::number(end+GnFREQuencyStep)+" MHz\n");


        if(end == 950)
        {
            this->thread()->msleep(700);
        }
        else
        {
            this->thread()->msleep(100);
        }


        viPrintf(N9000->vi, "CALC:MARK:MAX\n");
        N9000->Log("Захватить максимум.\n");
        viQueryf(N9000->vi, "CALC:MARK:Y?\n", "%lf", &dResultY);
        N9000->Log("Считанный Y :"+QString::number(dResultY)+" дб\n");
        viQueryf(N9000->vi, "CALC:MARK:X?\n", "%lf", &dResultX);
        N9000->Log("Считанный X :"+QString::number(dResultX/pow(10,6))+" MHz\n");

        if(dResultY <= -50)
        {
             Rele_Kom(in,out); // Соединение реле на коммутаторе СМ16-4 "A" вход с 1 выходом

             error++;

             N9000->Log("ERRRRRRRRRRRRRRRRRRRRROR\n");

             if(error == 3)
             {
                    error = 0;
             }
             else
             {
                 continue;
             }
        }


        x.append(dResultX/pow(10,6));


        //разкоментировать когда добавлю калибровку по модулю
        double ot = fabs(ListRegyl->value(ReleB+18).value(y.count()));

        double ot2 = fabs(ListRegyl->value(ReleA).value(y.count()));

        qDebug () << "ReleB = " << ReleB;
        qDebug () << "ReleA = " << ReleA;



       double PosleRegyl  = dResultY + (ot+ot2);


       qDebug () << "PosleRegyl =  " << dResultY << " + ("<< ot << " + " << ot2<<") = " <<PosleRegyl;


        dResultY = PosleRegyl; // положили в переменную число после регулировки для не изменения в запросе переменной

        y.append(dResultY);


        auto min1 = std::min_element(y.begin(),y.end());
        auto max1 = std::max_element(y.begin(),y.end());

        min = (*min1);
        max = (*max1);

        NeravnACHX = (*max1)-(*min1);


        end+=GnFREQuencyStep;

        date = QDateTime::currentDateTime();


        emit SQL_add("INSERT INTO GraphPoint (Data,IdResult,X,Y,NumberGraph,Неравномерность,Минимум,Максимум,DataProverki) VALUES('"+date.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(3,0), Qt::EditRole).toString()+"','"+QString::number(dResultX/pow(10,6))+"','"+QString::number(dResultY)+"','"+QString::number(ListX.count()+1)+"','"+QString::number(NeravnACHX)+"','"+QString::number(min)+"','"+QString::number(max)+"','"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"')");

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


void SetCheck(thread_SM* threadSM)
{
    int point=0;
    for(int i=0;i<threadSM->win_transferCoefficient->tableWidgetPerestrouka->columnCount();i++)
    {

        for(int j=0;j<threadSM->win_transferCoefficient->tableWidgetPerestrouka->rowCount();j++)
        {

            if(point < 8)
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

void thread_SM::StartProverka5()
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // qDebug()<< "Старт 5 првоерки!";

    emit StartPerestroyka(2);

    //Обнуление реле
    TP->ReleA(0);
    TP->ReleA(21);

    //Start5Proverktrue;


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

    FlagGood5 = true;
    FlagGood5Eshe = true;

    TP->ReleA(1);
    //ИЗМЕНИЛ

    win_transferCoefficient->tableWidgetPerestrouka->setRowCount(4);
    win_transferCoefficient->tableWidgetPerestrouka->setColumnCount(2);

   // emit CreateGraph();
    //sem3.acquire();


    int point=0;
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


    HMP2020->Log("Включить подачу напряжения.\n");
    HMP2020->Log("=================\n");
    HMP2020->write_VOLTage(27);
    HMP2020->Log("Установить Напряжение: 27 V.\n");
    HMP2020->write_OUTPut(true);
    HMP2020->Log("Включить подачу напряжения.\n");


    for(int j=1;j<=4;j++) //по 4 выходам
    {

        SetCheck(this);
        TP->ReleB(j+5);



        for(int i=-10;i<=10;i++) // -10 до 10
        {

            Ysilenie(i,j);

            this->thread()->msleep(100);

            //Sleep(1000);

            Proverka_5(i,j+5,1);
            emit SQL_add("UPDATE GraphPoint SET [Вход]='1',[Выход]='"+QString::number(j+5)+"' WHERE IdResult ='"+Link->data(Link->index(4,0), Qt::EditRole).toString()+"' AND NumberGraph='"+QString::number(ListPerestrouka.count()+1)+"'");
        }
        ListPerestrouka.append(y1); //Для отображения на графике подсчет количества графиков по выходам
        Listy1.append(PerestroykaX);


        if(FlagGood5)
        {
            if(ListPerestrouka.count() <=4)
            {
                win_transferCoefficient->tableWidgetPerestrouka->item(ListPerestrouka.count()-1,0)->setBackground(Qt::green);
            }
            else
            {
                win_transferCoefficient->tableWidgetPerestrouka->item(ListPerestrouka.count()-5,1)->setBackground(Qt::green);
            }
        }
        else
        {
            if(ListPerestrouka.count() <=4)
            {
                win_transferCoefficient->tableWidgetPerestrouka->item(ListPerestrouka.count()-1,0)->setBackground(Qt::red);
            }
            else
            {
                win_transferCoefficient->tableWidgetPerestrouka->item(ListPerestrouka.count()-5,1)->setBackground(Qt::red);

            }
        }

        y1.clear();
        PerestroykaX.clear();

    }


    TP->ReleA(0); //Обнулениереле 1 реле на А входе

    TP->ReleA(12); // Можно поменять на 11 но диодик на экране не рабоатет а выход 10 а так 11
    //ИЗМЕНИЛ
    for(int j=1;j<=4;j++)
    {
        SetCheck(this);

        TP->ReleB(j+5);

       // Rele_Kom(12,j); // Соединение реле на коммутаторе СМ16-4 "A" вход с 1 выходом

        for(int i=-10;i<=10;i++) // -10 до 10
        {
            Ysilenie(i,j);

            this->thread()->msleep(100);

            //Sleep(1000);
            Proverka_5(i,j+5,12);
            emit SQL_add("UPDATE GraphPoint SET [Вход]='11',[Выход]='"+QString::number(j+5)+"' WHERE IdResult ='"+Link->data(Link->index(4,0), Qt::EditRole).toString()+"' AND NumberGraph='"+QString::number(ListPerestrouka.count()+1)+"'");
        }
        ListPerestrouka.append(y1);//Для отображения на графике подсчет количества графиков по выходам
        Listy1.append(PerestroykaX);

        if(FlagGood5)
        {
            if(ListPerestrouka.count() <=4)
            {
                win_transferCoefficient->tableWidgetPerestrouka->item(ListPerestrouka.count()-1,0)->setBackground(Qt::green);
            }
            else
            {
                win_transferCoefficient->tableWidgetPerestrouka->item(ListPerestrouka.count()-5,1)->setBackground(Qt::green);
            }
        }
        else
        {
            if(ListPerestrouka.count() <=4)
            {
                win_transferCoefficient->tableWidgetPerestrouka->item(ListPerestrouka.count()-1,0)->setBackground(Qt::red);
            }
            else
            {
                win_transferCoefficient->tableWidgetPerestrouka->item(ListPerestrouka.count()-5,1)->setBackground(Qt::red);

            }
        }
        y1.clear();
        PerestroykaX.clear();
    }

    //Start5Proverkfalse;

    if(FlagGood5Eshe == true)
    {
        emit SQL_add("INSERT INTO Result (Date,IdLink,TransferCoefficient,[Этап]) VALUES('"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(4,0), Qt::EditRole).toString()+"','Cоотвутствует','"+etap+"')");
        TP->Log2("Соответствует\n");
    }
    else
    {
        emit SQL_add("INSERT INTO Result (Date,IdLink,TransferCoefficient,[Этап]) VALUES('"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(4,0), Qt::EditRole).toString()+"','Не соответствует','"+etap+"')");
        TP->Log2("Несоответствует\n");
    }




    TP->ReleA(0);
    TP->ReleB(0);
    TP->ReleA(22);
    TP->ReleB(22);

    Micran1->Log("Конец 5 проверки.\n");
    TP->Log("Конец 5 проверки.\n");
    N9000->Log("Конец 5 проверки.\n");
    N9000->Log2("Конец 5 проверки.\n");


    FlagGood5 = false;
}

void thread_SM::Proverka_5(int KoefPeredachi,int ReleB,int ReleA)
{
    FlagGood5 = true;

    view->item(4,0)->setBackgroundColor(Qt::yellow);
    view->reset();

    double dResultY = 0;
    double dResultX = 0;
    char buff[9024];


    viPrintf(Micran1->vi, "SOURce:FREQuency:MODE FIXed\r\n");
    Micran1->Log("Установить MODE: FIXed\n");


    //KoefPeredachi = 0; /// Задаем 10 ДБ



   // Sleep(5000);

    GnPower = 0;
    // Set power level value
    viPrintf(Micran1->vi, "SOURce:POWer:LEVel %d DBM\r\n",GnPower); // Установка Мощности для 1550 МГЦ.
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

    //Sleep(1000);

    this->thread()->msleep(100);

    viPrintf(N9000->vi, "CALC:MARK:MAX\n");
    N9000->Log("Захватить максимум.\n");
    viQueryf(N9000->vi, "CALC:MARK:Y?\n", "%lf", &dResultY);
    N9000->Log("Считанный Y :"+QString::number(dResultY)+" дб\n");
    viQueryf(N9000->vi, "CALC:MARK:X?\n", "%lf", &dResultX);
    N9000->Log("Считанный X :"+QString::number(dResultX/pow(10,6))+" MHz\n");



    //разкоментировать когда добавлю калибровку по модулю
    double ot = fabs(ListRegyl->value(ReleB+18).value(y.count()));

    double ot2 = fabs(ListRegyl->value(ReleA).value(y.count()));


   double PosleRegyl  = dResultY + (ot+ot2);


   qDebug () << "PosleRegyl =  " << dResultY << " + ("<< ot << " + " << ot2<<") = " <<PosleRegyl;


    dResultY = PosleRegyl; // положили в переменную число после регулировки для не изменения в запросе переменной






    x1.append(dResultX/pow(10,6));

    y1.append(dResultY);

    PerestroykaX.append(GnPower);

    date = QDateTime::currentDateTime();

    emit SQL_add("INSERT INTO GraphPoint (Data,IdResult,X,Y,[Коэфициент передачи],NumberGraph,DataProverki) VALUES('"+date.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(4,0), Qt::EditRole).toString()+"','"+QString::number(dResultX/pow(10,6))+"','"+QString::number(dResultY)+"','"+QString::number(KoefPeredachi)+"','"+QString::number(ListPerestrouka.count()+1)+"','"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"')");


   Start5Proverka = true;

    emit updateGraphPerestrouyka();
    sem3.acquire();


    //Примерная мощность для ошибки
    if(dResultY <= KoefPeredachi + 1 && dResultY >= KoefPeredachi - 1)
    {
        view->item(4,0)->setBackgroundColor(Qt::green);
        view->reset();

        FlagGood5 = true;
        FlagGood5Eshe = true;

    }
    else
    {
        view->item(4,0)->setBackgroundColor(Qt::red);
        view->reset();

        FlagGood5 = false;
        FlagGood5Eshe = false;

    }

}

void thread_SM:: StartProverka6()
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

    TP->ReleA(20);


    char a[10];

    for(int i=0; i < 10;i++)
    {
        a[i] = NULL;
    }

    a[8] = 0x01;
    a[9] = 0x01;

    TP->ReleB(a,2);


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
        TP->ReleB(i);
        Rele_Kom_10MGH(i-5); // Переключение реле на коммутаторе СМ16-4 с ВЫХ на ПРД к 10 МГЦ
        Proverka10MGU();
        emit SQL_add("UPDATE GraphPoint SET [Вход]='"+QString::number(i)+"',[Выход]='10 МГЦ' WHERE IdResult ='"+Link->data(Link->index(5,0), Qt::EditRole).toString()+"'AND NumberGraph='"+QString::number(x10MGh.count())+"'");
    }

    if(FlagGood10Mhz == true)
    {
        N9000->Log2("Cоответствует\n");
        emit SQL_add("INSERT INTO Result (Date,IdLink,SwitchingSignalSM,[Этап]) VALUES('"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(5,0), Qt::EditRole).toString()+"','соответствует','"+etap+"')");
    }
    else
    {
        N9000->Log2("Несоответствует\n");
        emit SQL_add("INSERT INTO Result (Date,IdLink,SwitchingSignalSM,[Этап]) VALUES('"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(5,0), Qt::EditRole).toString()+"','Не соответствует','"+etap+"')");
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


void thread_SM::Proverka10MGU()
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

     Sleep(1000);

    viPrintf(N9000->vi, "CALC:MARK:MAX\n");
    N9000->Log("Захватить максимум.\n");

    viQueryf(N9000->vi, "CALC:MARK:Y?\n", "%lf", &dResultY);
    N9000->Log("Считанный Y :"+QString::number(dResultY)+" дб\n");

    viQueryf(N9000->vi, "CALC:MARK:X?\n", "%lf", &dResultX);
    N9000->Log("Считанный X :"+QString::number(dResultX/pow(10,6))+" MHz\n");

    x10MGh.append(dResultX/pow(10,6));

    y10MGh.append(dResultY);

    date = QDateTime::currentDateTime();
    emit SQL_add("INSERT INTO GraphPoint (Data,IdResult,X,Y,NumberGraph,DataProverki) VALUES('"+date.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(5,0), Qt::EditRole).toString()+"','"+QString::number(dResultX/pow(10,6))+"','"+QString::number(dResultY)+"','"+QString::number(x10MGh.count())+"','"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"')");


    if(x10MGh.count()==1)
    {
        win_switchingSignalSM->lcdNumber->display(dResultY);
    }
    if(x10MGh.count()==2)
    {
        win_switchingSignalSM->lcdNumber_2->display(dResultY);
    }
    if(x10MGh.count()==3)
    {
        win_switchingSignalSM->lcdNumber_3->display(dResultY);
    }
    if(x10MGh.count()==4)
    {
        win_switchingSignalSM->lcdNumber_4->display(dResultY);
    }

    //Примерная мощность для ошибки
    if(dResultY < -5)
    {
        if(x10MGh.count()==1)
        {
            win_switchingSignalSM->pushButton_1->setIcon(R3_Red);
        }
        if(x10MGh.count()==2)
        {
            win_switchingSignalSM->pushButton_2->setIcon(R3_Red);
        }
        if(x10MGh.count()==3)
        {
            win_switchingSignalSM->pushButton_3->setIcon(R3_Red);
        }
        if(x10MGh.count()==4)
        {
            win_switchingSignalSM->pushButton_4->setIcon(R3_Red);
        }

        view->item(5,0)->setBackgroundColor(Qt::red);
        view->reset();

        FlagGood10Mhz = false;
    }
    else
    {
        if(x10MGh.count()==1)
        {
            win_switchingSignalSM->pushButton_1->setIcon(R2_Green);
        }
        if(x10MGh.count()==2)
        {
            win_switchingSignalSM->pushButton_2->setIcon(R2_Green);
        }
        if(x10MGh.count()==3)
        {
            win_switchingSignalSM->pushButton_3->setIcon(R2_Green);
        }
        if(x10MGh.count()==4)
        {
            win_switchingSignalSM->pushButton_4->setIcon(R2_Green);
        }

        view->item(5,0)->setBackgroundColor(Qt::green);
        view->reset();
    }
}


void thread_SM::StartProverka7()
{
    // qDebug()<< "Старт 7 првоерки!";

    qDebug() << "StartProverka7";
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
        emit SQL_add("INSERT INTO Result (Date,IdLink,Ithernet,[Этап]) VALUES('"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(6,0), Qt::EditRole).toString()+"','Не соответствует','"+etap+"')");
    }
    else
    {
        view->item(6,0)->setBackgroundColor(Qt::green);
        view->reset();
        TP->Log2("Соответствует\n");
        date = QDateTime::currentDateTime();
        emit SQL_add("INSERT INTO Result (Date,IdLink,Ithernet,[Этап]) VALUES('"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(6,0), Qt::EditRole).toString()+"','Cоответствует','"+etap+"')");
    }

    TP->Log("Конец 7 проверки.\n");
    TP->Log2("Конец 7 проверки.\n");

    qDebug() << "StartProverka7 END";
}

void thread_SM::StartProveroka8()
{
    qDebug() << "StartProveroka8";
    if(Link->rowCount()==8)
    {
        //TP->Log2("=================\n");
        TP->Log2("Старт 8 проверки.\n");

        view->item(7,0)->setBackgroundColor(Qt::yellow);
        view->item(7,0)->setBackgroundColor(Qt::green);
        view->reset();
        date = QDateTime::currentDateTime();
        emit SQL_add("INSERT INTO Result (Date,IdLink,LongTime,[Этап]) VALUES('"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(7,0), Qt::EditRole).toString()+"','Cоответствует','"+etap+"')");
        TP->Log2("Конец 8 проверки.\n");
    }

}

void thread_SM::END()
{
    qDebug() << "END";

    viPrintf(Micran1->vi, "OUTPut:STATe OFF\r\n");
    Micran1->Log("Включить выход ВЧ.\n");


    // Send "Clear status" command
    viPrintf(Micran1->vi, "*CLS\r\n");
    Micran1->Log("Cброс статуса.\n");


    viPrintf(HMP2020->vi, "OUTPut:GENeral 0\r\n");


    N9000->write_AVER_STAT(false);
    N9000->Log("Отключаем усреднение: AVER.\n");

    N9000->write_CALC_MARK_MODE("NORM");
    N9000->Log("Установить режим маркера: NORM.\n");

    N9000->write_DISP_WIND_TRAC_Y_RLEV(12);
    N9000->Log("Установить Y: 12 dBm.\n");

    //Задает полосу просмотра для отображения сигнала на анализаторе....
    N9000->write_FREQuency_STARt(950,"MHz");
    N9000->Log("Установить начальную частоту просмотра: 950 MHz.\n");

    N9000->write_FREQuency_STOP(2150,"MHz");
    N9000->Log("Установить конечную частоту просмотра: 2150 MHz.\n");



    TP->ReleA(0);
    TP->ReleB(0);
    TP->ReleA(21);
    TP->ReleB(21);




//    TP->udpsocket->close(); // UDP soket Для Переключения реле Пульта "A1"выход по "A9"
//    TP->udpsocket1->close(); // UDP soket Для чтения информации с сети от пульта по A выходу (Пульт -> коммутатор)
//    TP->udpsocket2->close(); // UDP soket Для Переключения реле Пульта "A10"выхода по "A18"
//    TP->udpsocket3->close();
//    TP->udpsocket4->close(); // UDP soket Для чтения информации с сети от пульта по B выходу (Пульт <- коммутатор)
//    TP->udpsocket5->close(); // UDP soket Для Переключения реле Пульта "В1"выход до B"9"
//    TP->udpsocket6->close(); // UDP soket Для Переключения реле Пульта B"10" выхода до B"18"
//    TP->udpsocket7->close();
//    TP->udpsocket8->close(); // UDP soket Для Переключения реле Пульта A0 соединить с A выход
//    TP->udpsocket9->close(); // UDP soket Для Переключения реле Пульта B0 соединить с B выход
//    p_udpSocketOut->close(); // UDP soket для Переключения каналов реле Коммутатора СМ16-4



    emit end();


}

void thread_SM::StratRegyl()
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

    //SQL_OneProverknew QSqlQueryModel();

    FlagGoodAchH = true;

    flagRegylK0=false;

    flagRegylK0=true;
    m_mutex.lock();
    sem2.acquire();

    QTextStream  writeStream(file);
    //  SQL_OneProverka->setQuery("INSERT INTO Result (Date,IdLink,[Этап]) VALUES('"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(3,0), Qt::EditRole).toString()+"','"+etap+"')");
    writeStream << dateStart.toString("dd.MM.yyyy  hh:mm:ss")<<endl;
    writeStream << "K0: "<<endl;

    AchHRegyl();// Промерить   К0 кабель

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
                //relei;
                qDebug() <<" i = "<< releA;
                writeStream <<"A"<<releA<<":"<<endl;
            }
            else
            {
                releA =i-1;
                writeStream <<"A"<<releA<<":"<<endl;
                qDebug() <<" i-1 = "<<  releA;
            }
            //flagRegyl_true;
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
                //relei;
                writeStream <<"B"<<releA<<":"<<endl;
            }
            else
            {
                releA =i-1;
                writeStream <<"B"<<releA<<":"<<endl;
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

void thread_SM::ProverkaRegyl()
{
    AchH(1,1);
}

void thread_SM::AchHRegyl()
{
    N9000->Log("=================\n");
    Micran1->Log("=================\n");


    double dResultY = 0;
    double dResultX = 0;
    char buff[9024];

    x.clear();
    y.clear();
    int end=GnFREQuencyStart;

    QTextStream  writeStream(file);

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


        if(end == 950)
        {
            this->thread()->msleep(500);
        }
        else
        {
            this->thread()->msleep(100);
        }

        viPrintf(N9000->vi, "CALC:MARK:MAX\n");
        N9000->Log("Захватить максимум.\n");
        viQueryf(N9000->vi, "CALC:MARK:Y?\n", "%lf", &dResultY);
        N9000->Log("Считанный Y :"+QString::number(dResultY)+" дб\n");
        viQueryf(N9000->vi, "CALC:MARK:X?\n", "%lf", &dResultX);
        N9000->Log("Считанный X :"+QString::number(dResultX/pow(10,6))+" MHz\n");


        x.append(dResultX/pow(10,6));
       // dResultY -=GnPower;
        y.append(dResultY);

        auto min1 = std::min_element(y.begin(),y.end());
        auto max1 = std::max_element(y.begin(),y.end());

        min = (*min1);
        max = (*max1);

        NeravnACHX = (*max1)-(*min1);


        end+=GnFREQuencyStep;
        date = QDateTime::currentDateTime();
        //   SQL_OneProverka->setQuery("INSERT INTO GraphPoint (Data,IdResult,X,Y,NumberGraph,Неравномерность,Минимум,Максимум,DataProverki) VALUES('"+date.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(3,0), Qt::EditRole).toString()+"','"+QString::number(dResultX/pow(10,6))+"','"+QString::number(dResultY)+"','K0','"+QString::number(NeravnACHX)+"','"+QString::number(min)+"','"+QString::number(max)+"','"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"')");
        writeStream << "X: " << QString::number(dResultX/pow(10,6))<<"\t Y: "<<QString::number(dResultY)<<endl;


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
