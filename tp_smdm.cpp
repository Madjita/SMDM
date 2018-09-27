#include <tp_smdm.h>



QString LocalIP();

TP_SMDM::TP_SMDM()
{

    this->moveToThread(new QThread());
    connect(this->thread(),&QThread::started,this,&TP_SMDM::process_start);
    this->thread()->start();

}

void TP_SMDM::process_start()
{

    ipTP = "10.12.0.210";

    ipComputer=LocalIP();

    log = "";

    noup[0] = (char)noupint[0];
    noup[1] = (char)noupint[1];
    noup[2] = (char)noupint[2];

    for(int i=0; i<10; i++)
    {
        list_k1.append("");
        list_k2.append("");
        list_k3.append("");
        list_k4.append("");
    }

    IPfromFILE();


    //new
    udpsocket65533=new QUdpSocket(); //᮪��� �� ����祭�� ���⮢ ५� �1

    udpsocket65510=new QUdpSocket();//á®ªª¥â ¤«ï çâ¥­¨ï IP
    udpsocket65510->bind(QHostAddress(ipComputer),65510);

    connect(udpsocket65510,&QUdpSocket::readyRead,this, &TP_SMDM::readIP);


    udpsocket65401=new QUdpSocket(); //᮪��� �� ����祭�� ���⮢ ५� �1
    udpsocket65401->bind(QHostAddress(IPaddrPLATA),65401);

    udpsocket65511=new QUdpSocket(this); //᮪��� �� ����祭�� ���⮢ ५� �1
    udpsocket65511->bind(QHostAddress(IPaddrPLATA),65511); //65511


    udpsocket1=new QUdpSocket(); //соккет для получения информации о состоянии ножек контроллера реле К1
    udpsocket1->bind(QHostAddress(ipComputer),65534);

    udpsocket3=new QUdpSocket(); //соккет для получения информации о состоянии ножек контроллера реле К2
    udpsocket3->bind(QHostAddress(ipComputer),65528);

    udpsocket4=new QUdpSocket(); //соккет для получения информации о состоянии ножек контроллера реле К3
    udpsocket4->bind(QHostAddress(ipComputer),65525);

    udpsocket7=new QUdpSocket(); //соккет для получения информации о состоянии ножек контроллера реле К4
    udpsocket7->bind(QHostAddress(ipComputer),65522);


    udpsocket65531=new QUdpSocket(); //á®ªª¥â ­  ¢ª«îç¥­¨¥ ¯®àâ®¢ à¥«¥ 2
    udpsocket65531->bind(QHostAddress(IPaddrPLATA),65531);

    udpsocket65533->bind(65533);
    udpsocket65533->writeDatagram(a,13,QHostAddress(IPaddrPLATA),65533); //¢ª«îç¥­¨¥ ¯®àâ®¢ 1

    udpsocket65526=new QUdpSocket(); //á®ªª¥â ­  ¢ª«îç¥­¨¥ ¯®àâ®¢ à¥«¥ 3
    udpsocket65526->bind(QHostAddress(IPaddrPLATA),65526);

    udpsocket65523=new QUdpSocket(); //á®ªª¥â ­  ¢ª«îç¥­¨¥ ¯®àâ®¢ à¥«¥ 4
    udpsocket65523->bind(QHostAddress(IPaddrPLATA),65523);

    udpsocket65521=new QUdpSocket();//á®ªª¥â ¤«ï ¯®¤ª«îç¥­¨ï £¥­¥à â®à  (5) ª 1 ¨«¨ 2
    udpsocket65521->bind(QHostAddress(IPaddrPLATA),65521);

    myIP=LocalIP();


    udpsocket65400=new QUdpSocket(this);
    udpsocket65400->bind(QHostAddress(myIP),65400);
    connect(udpsocket65400,SIGNAL(readyRead()),this, SLOT(selection_pult_ok()));

    udpsocket65500=new QUdpSocket(this);
    udpsocket65500->bind(QHostAddress(IPaddrPLATA),65500);

    udpsocket65521=new QUdpSocket(this);
    udpsocket65521->bind(QHostAddress(IPaddrPLATA),65521);

    udpsocket65520=new QUdpSocket(this);
    udpsocket65520->bind(QHostAddress(IPaddrPLATA),65520);



    // udpsocket8=new QUdpSocket();//соккет для подключения генератора к К1(A1-A10) или К2 (A10-A20)

}

TP_SMDM::~TP_SMDM()
{

}

void TP_SMDM::SetTp()
{

   process_start();
}

void TP_SMDM::Connect()
{


    Log2("Соединение с устройством ТП-СМДМ: ");

    //  udpsocket20=new QUdpSocket();//соккет для чтения IP

    //   udpsocket20->bind(QHostAddress(ipComputer),65510);

    //  udpsocket21=new QUdpSocket();

    char b[2];

    udpsocket65511->writeDatagram(b,2,QHostAddress(ipTP),65533);


    QString result = "";
    while(udpsocket65510->hasPendingDatagrams())
    {

        QByteArray datagram;
        datagram.resize(udpsocket65510->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        udpsocket65510->readDatagram(datagram.data(),datagram.size(),&sender,&senderPort);

        int tmp=0;

        for ( int i = 0; i<4; i++)
        {
            tmp = (unsigned char) datagram[i];
            result.append(QString::number(tmp));
            if (i!=3)
            {
                result.append(".");
            }
        }

    }

    if(result!="")
    {
        qDebug() <<"[Соединение установленно(ОК)]";
        emit Log("===================\n");
        emit Log("[Соединение установленно(ОК)] \n");
        emit Log("===================\n");
        emit Log2("[Соединение установленно(ОК)] \n");
    }
    else
    {
        qDebug() <<"[Соединение не установленно(ERROR)]";
        emit Log("===================\n");
        emit Log("[Соединение не установленно(ERROR)] \n");
        emit Log("===================\n");
        emit Log2("[Соединение не установленно(ERROR)] \n");
    }

    // udpsocket20->close();
    // udpsocket21->close();



}


//Функция Отключения сокетов
void TP_SMDM::DisConnectToSoket()
{

    udpsocket1->close();
    udpsocket65400->close();
    udpsocket65401->close();
    udpsocket65500->close();
    udpsocket65510->close();
    udpsocket65511->close();
    udpsocket65521->close();
    udpsocket65523->close();
    udpsocket65526->close();
    udpsocket65531->close();
    udpsocket65533->close();

    p_udpSocketOut->close(); // UDP soket для Переключения каналов реле Коммутатора СМ16-4
}

//Проверка принял  ли Пульт нашу команду
bool TP_SMDM::readDatagramNaZapros(QUdpSocket* udp)
{


    bool lol_flag = false;
    Sleep(10);
    while(udp->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udp->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        udp->readDatagram(datagram.data(),datagram.size(),&sender,&senderPort);

        lol_flag = true;

    }


    qDebug () << "ФЛаг = " << lol_flag;

    return lol_flag;
}

bool TP_SMDM::ReleA(int A)
{
    char A0[2];
    char b[10];

    bool flag = false;



    if(A>=0 && A<=10)
    {
        generatorON(1); //включение  реле генератора К1
        A0[0]=1;
        A0[1]=0;
    }
    else
    {
        generatorON(2); //включение реле генератора К2

        A0[0]=0;
        A0[1]=1;
    }

    list_k1.clear();

    if(A0[0]==1)
    {
        emit Log("A: Установили реле К1\n");
    }
    else
    {
        emit Log("A: Установили реле К2\n");
    }

    if(flag_6_proverki==false)
    {

        for(int i=0;i<10;i++)
        {

            if(i==(A-1) || i==(A-11))
            {
                a[i]=1;
                list_k1.append("1");
            }
            else
            {
                a[i]=0;
                list_k1.append("0");
            }
        }
    }
    else
    {
        for(int i=0;i<10;i++)
        {
            if(i==(A-1) || i==(A-11))
            {

                a[i]=1;
                list_k1.append("1");
            }
            else
            {
                a[i]=0;
                list_k1.append("0");
            }

            if(i==9)
            {
                a[i]=1;
                list_k1.replace(i,"1");
            }
        }
    }


    qDebug () << "A = " << A << " list_k1 = " << list_k1 ;
    //}
    //  else
    //  {
    //       if(A0[0]==1)
    //       {
    //            emit Log("A: Ошибка в установке реле К1\n");
    //      }
    //      else
    //      {
    //         emit Log("A: Ошибка в установке реле К2\n");
    //     }
    //  }


    // udpsocket=new QUdpSocket(); //соккет на включение портов реле К1 в пульте (точне A1-A9 выход)
    //  udpsocket2=new QUdpSocket(); //соккет на включение портов реле К2 в пульте (точне A10-A18 выход)

    if(A>=0&&A<=10)
    {

        rele_portON(list_k1,1); //включение портов К1

        zaprosSOST_rele(1);

        flag = readDatagramK1();


        if(A==0 && flag == true)
        {
            emit Log("A: Порты реле К1 cброшенны.\n");
        }
        else
        {
            if(A==0)
            {
                emit Log("A: Ошибка! -> Порты реле К1 не cброшенны.\n");
            }
            if(flag)
            {
                emit Log("A: Порт "+QString::number(A)+" включен\n");
            }
            else
            {
                emit Log("A: Ошибка! -> Порт "+QString::number(A)+" не включен\n");
            }
        }
    }
    else
    {

        rele_portON(list_k1,2); //включение портов К2
        zaprosSOST_rele(2);

        flag = readDatagramK2();

        if(A>20 && flag == true)
        {
            emit Log("A: Порты реле К2 cброшенны.\n");
        }
        else
        {
            if(A>20)
            {
                emit Log("A: Ошибка! -> Порты реле К2 не cброшенны.\n");
            }
            if(flag)
            {
                emit Log("A: Порт "+QString::number(A-1)+" включен\n");
            }
            else
            {
                emit Log("A: Ошибка! -> Порт "+QString::number(A-1)+" не включен\n");
            }
        }

    }


    if(flag == false)
    {
        ReleA(A);
         qDebug() << "A don't open";
    }
    else
    {
       return flag;
    }

}

bool TP_SMDM::ReleB(int B)
{
    bool flag = false;

    char B0[2];

    char a[10];
    char b[10];


    if(B>=0 && B<=10)
    {
        analizatorON(1); //включение  реле генератора К3

        B0[0]=1;
        B0[1]=0;
    }
    else
    {
        analizatorON(2); //включение  реле генератора К4

        B0[0]=0;
        B0[1]=1;
    }

    list_k3.clear();

    if(B0[0]==1)
    {
        emit Log("B: Установили реле К3\n");
    }
    else
    {
        emit Log("B: Установили реле К4\n");
    }

    if(flag_6_proverki == false)
    {
        for(int i=0;i<10;i++)
        {
            if(i==(B-1) || i==(B-11))
            {
                a[i]=1;
                list_k3.append("1");
            }
            else
            {
                a[i]=0;
                list_k3.append("0");
            }
        }
    }
    else
    {
        for(int i=0;i<10;i++)
        {
            if(i==(B-1) || i==(B-11))
            {
                a[i]=1;
                list_k3.append("1");
            }
            else
            {
                a[i]=0;
                list_k3.append("0");
            }
            if(i==9)
            {
                a[i]=1;
                list_k3.replace(i,"1");
            }
        }
    }

    qDebug () << "B = " << B << " list_k3 = " << list_k3 ;


    if(B>=0&&B<=10)
    {
        rele_portON(list_k3,3); //включение портов К3

        zaprosSOST_rele(3);


        flag = readDatagramK3();
        if(B==0 && flag == true)
        {
            emit Log("B: Порты реле К3 cброшенны.\n");
        }
        else
        {
            if(B==0)
            {
                emit Log("B: Ошибка! -> Порты реле К3 не cброшенны.\n");
            }
            if(flag)
            {
                emit Log("B: Порт "+QString::number(B)+" включен\n");
            }
            else
            {
                emit Log("B: Ошибка! -> Порт "+QString::number(B)+" не включен\n");
            }
        }

    }
    else
    {
        rele_portON(list_k3,4); //включение портов К4

        zaprosSOST_rele(4);


        flag = readDatagramK4();

        if(B > 20 && flag == true)
        {
            emit Log("B: Порты реле К4 cброшенны.\n");
        }
        else
        {
            if(B > 20)
            {
                emit Log("B: Ошибка! -> Порты реле К4 не cброшенны.\n");
            }
            if(flag)
            {
                emit Log("B: Порт "+QString::number(B-1)+" включен\n");
            }
            else
            {
                emit Log("B: Ошибка! -> Порт "+QString::number(B-1)+" не включен\n");
            }
        }


    }

    if(flag == false)
    {
        ReleB(B);
        qDebug() << "B don't open";
    }
    else
    {
       return false;
    }
    //////////////////////////////////////////////////////////////////////////
}

bool TP_SMDM::ReleB(char a[10], int rele)
{
    list_k3.clear();

    QString str = "B: Порт ";

    for(int  i=0; i < 10;i++)
    {
        switch (a[i]) {
        case 0x01:{
            list_k3.append("1");
            break;
        }
        case 0x00:{
            list_k3.append("0");
            break;
        }


        }

        if(list_k3.value(i) == "1")
        {
            str +=list_k3.value(i);
            str +=" ";
        }
    }

    qDebug () << list_k3;

    switch (rele) {

    case 1:{
        analizatorON(1); //включение  реле генератора К3

        rele_portON(list_k3,3); //включение портов К3

        zaprosSOST_rele(3);

        flag = readDatagramK3();

        if(flag)
        {
            str +="включен\n";

            emit Log(str);
        }
        else
        {
            str +="не включен\n";
            emit Log(str);
        }


        break;
    }
    case 2: {
        analizatorON(2); //включение  реле генератора К3

        rele_portON(list_k3,4); //включение портов К3

        zaprosSOST_rele(4);

        if(flag)
        {
            str +="включен\n";

            emit Log(str);
        }
        else
        {
            str +="не включен\n";
            emit Log(str);
        }


        break;
    }

    }

    return  flag;

}


bool TP_SMDM::readDatagramK1()
{
    bool flag = false;
    QByteArray datagram;
    this->thread()->msleep(250);


    while(udpsocket1->hasPendingDatagrams())
    {

        datagram.resize(static_cast<int>(udpsocket1->pendingDatagramSize()));
        QHostAddress sender;
        quint16 senderPort;
        udpsocket1->readDatagram(datagram.data(),datagram.size(),&sender,&senderPort);

        emit SetImage(datagram);

        flag = true;
    }

    qDebug() << flag<< " : readDatagramK1 = "<< datagram;

    return flag;
}

bool TP_SMDM::readDatagramK2()
{
    bool flag = false;
    QByteArray datagram;
    this->thread()->msleep(250);


    while(udpsocket3->hasPendingDatagrams())
    {

        datagram.resize(static_cast<int>(udpsocket3->pendingDatagramSize()));
        QHostAddress sender;
        quint16 senderPort;
        udpsocket3->readDatagram(datagram.data(),datagram.size(),&sender,&senderPort);


        emit SetImage2(datagram);



        flag = true;
    }

    qDebug() << flag<< " : readDatagramK2 = "<< datagram;

    return flag;
}

bool TP_SMDM::readDatagramK3()
{
    bool flag = false;
    QByteArray datagram;
    this->thread()->msleep(250);

    while(udpsocket4->hasPendingDatagrams())
    {

        datagram.resize(static_cast<int>(udpsocket4->pendingDatagramSize()));
        QHostAddress sender;
        quint16 senderPort;
        udpsocket4->readDatagram(datagram.data(),datagram.size(),&sender,&senderPort);


        emit  SetImage3(datagram);



        flag = true;
    }

    qDebug() << flag<< " : readDatagramK3 = "<< datagram;

    return flag;
}

bool TP_SMDM::readDatagramK4()
{
    bool flag = false;
    QByteArray datagram;
    this->thread()->msleep(250);

    while(udpsocket7->hasPendingDatagrams())
    {

        datagram.resize(static_cast<int>(udpsocket7->pendingDatagramSize()));
        QHostAddress sender;
        quint16 senderPort;
        udpsocket7->readDatagram(datagram.data(),datagram.size(),&sender,&senderPort);


        emit  SetImage4(datagram);

        flag = true;
    }

    qDebug() << flag<< " : readDatagramK4 = "<< datagram;

    return flag;
}



void TP_SMDM::Kolibrovka()
{
    p_udpSocketOut = new QUdpSocket();
    p_udpSocketOut->bind(30021);



    file = new QFile("Regylirovka.txt");
    if (!file->open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "No find file";
    }

    dateStart = QDateTime::currentDateTime();
    ListRegyl = new QVector<QVector<double>>();

    StratRegyl();
    // file->close();
    //END();
    flagEnd = false;
}

char IPfromQStringtochar(QString str)//äã­ªæ¨ï ¯à¥®¡à §®¢ ­¨ï áâà®ª¨ ¢ ¬ áá¨¢ 4 char'®¢ (¯à¥¤­ §­ ç¥­  ¤«ï IP- ¤à¥á )
{
    char c[3] = {'.','.','.'};
    char result;
    int b[3]={0};
    int j=0;

    strncpy(c, qPrintable(str), sizeof(c));

    for (int i = 0; i<3; i++)
    {
        if (c[i]!=0)
        {
            j++;
            c[i] = c[i]-48;
        }
    }
    if (j==3)
    {
        b[0]=c[0]*100;
        b[1]=c[1]*10;
        b[2]=c[2];
    }
    if (j==2)
    {
        b[0]=c[0]*10;
        b[1]=c[1];
    }
    if (j==1)
    {
        b[0]=c[0];
    }

    result=(char)(b[0]+b[1]+b[2]);

    return result;
}


void TP_SMDM::zapros_readip()
{
    //emit myIP_QML_ChangeQML(ipComputer);

    emit Log2("Соединение с устройством ТП-СМДМ: ");

    QStringList listIPfromFunction;
    listIPfromFunction = ipComputer.split('.');

    myIPchar[0]=IPfromQStringtochar(listIPfromFunction[0]);
    myIPchar[1]=IPfromQStringtochar(listIPfromFunction[1]);
    myIPchar[2]=IPfromQStringtochar(listIPfromFunction[2]);
    myIPchar[3]=IPfromQStringtochar(listIPfromFunction[3]);

    char data[10] = {0} ;

    data[0] = noup[0];
    data[1] = noup[1];
    data[2] = noup[2];

    data[6] = myIPchar[0];
    data[7] = myIPchar[1];
    data[8] = myIPchar[2];
    data[9] = myIPchar[3];


    udpsocket65511->writeDatagram(data,10,QHostAddress("255.255.255.255"),65511); //21
}


void TP_SMDM::readIP()
{
    while(udpsocket65510->hasPendingDatagrams())
    {

        QByteArray datagram;
        datagram.resize(udpsocket65510->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        udpsocket65510->readDatagram(datagram.data(),datagram.size(),&sender,&senderPort);



        int tmp=0;
        QString result = "";
        for ( int i = 0; i<4; i++)
        {
            tmp = (unsigned char) datagram[i];
            result.append(QString::number(tmp));
            if (i!=3)
            {
                result.append(".");
            }
        }

        if (list.contains(result)==false)          //¥á«¨ â ª®£® IP ¥éñ ­¥ ¡ë«®
        {
            int m = list.indexOf(lastIP);

            if (m==-1)
            {
                list.append(result);               //¤®¡ ¢«ï¥â â¥ªãé¥¥ §­ ç¥­¨¥ ¢ QVector
            }
            else
            {
                list.replace(m,result);            //§ ¬¥­ï¥â áâ à®¥ §­ ç¥­¨¥ ­®¢ë¬
            }
        }


        qDebug () << "list = " << list;

        IPaddrPLATA = list.last();

        qDebug() <<"[Соединение установленно(ОК)]";
        emit Log("===================\n");
        emit Log("[Соединение установленно(ОК)] \n");
        emit Log("===================\n");
        emit Log2("[Соединение установленно(ОК)] \n");

        /*
          flag_combobox=list.indexOf(result);
          if (flag_combobox==-1)
          {
              flag_combobox=0;
          }
          emit flag_combobox_QML_ChangeQML(flag_combobox);


          emit listIP_PULT_QML_ChangeQML(list);

          */

    }
}

void TP_SMDM::setIP(QString ip_from_textbox)
{
    lastIP=currentIPinComboBox;
    QString str = ip_from_textbox;

    char ipresult[10] = {0};
    char GW[4] = {0};
    char check[4] = {0};


    QRegExp rx("((1{0,1}[0-9]{0,2}|2[0-4]{1,1}[0-9]{1,1}|25[0-5]{1,1})\\.){3,3}(1{0,1}[0-9]{0,2}|2[0-4]{1,1}[0-9]{1,1}|25[0-5]{1,1})");
    QRegExpValidator v(rx,0);
    QString validresult = "";
    int pos=0;
    validresult = v.validate(str,pos);

    QStringList listIPfromFunction;
    listIPfromFunction = lastIP.split('.');

    GW[0]=IPfromQStringtochar(listIPfromFunction[0]);
    GW[1]=IPfromQStringtochar(listIPfromFunction[1]);
    GW[2]=IPfromQStringtochar(listIPfromFunction[2]);
    GW[3]=IPfromQStringtochar(listIPfromFunction[3]);

    listIPfromFunction = str.split('.');

    if ((listIPfromFunction.length()==4))
    {
        check[0]=IPfromQStringtochar(listIPfromFunction[0]);
        check[1]=IPfromQStringtochar(listIPfromFunction[1]);
        check[2]=IPfromQStringtochar(listIPfromFunction[2]);
        check[3]=IPfromQStringtochar(listIPfromFunction[3]);
    }

    if((GW[0]==check[0])&(GW[1]==check[1])&("\002"==validresult)) //¯à®¢¥àª  ­  ¯ãáâ®¥ ¯®«¥ ¨ ­  ª®àà¥ªâ­ë© ä®à¬ â  ¤à¥á
    {

        ipresult[0]=noup[0];
        ipresult[1]=noup[1];
        ipresult[2]=noup[2];
        ipresult[3]=check[0];
        ipresult[4]=check[1];
        ipresult[5]=check[2];
        ipresult[6]=check[3];
        ipresult[7]=0;
        ipresult[8]=0;
        ipresult[9]=0;
        //udpsocket65500->bind(65500);
        udpsocket65500->writeDatagram(ipresult,10,QHostAddress(IPaddrPLATA),65500);
        udpsocket65500->writeDatagram(ipresult,10,QHostAddress(IPaddrPLATA),65500);
        udpsocket65500->writeDatagram(ipresult,10,QHostAddress(IPaddrPLATA),65500);


        if (list.contains(str)==false)     //¥á«¨ â ª®£® IP ¥éñ ­¥ ¡ë«®
        {
            int i = list.indexOf(lastIP);
            if (i==-1)
            {
                list.append(str);              //¤®¡ ¢«ï¥â â¥ªãé¥¥ §­ ç¥­¨¥ ¢ QVector
            }
            else
            {
                list.replace(i,str);           //§ ¬¥­ï¥â áâ à®¥ §­ ç¥­¨¥ ­®¢ë¬
            }
        }




        SetIPdone=true;

        /*
    flag_combobox=list.indexOf(str);
    if (flag_combobox==-1)
    {
        flag_combobox=0;
    }
    emit flag_combobox_QML_ChangeQML(flag_combobox);


    emit listIP_PULT_QML_ChangeQML(list);


    flag=0;
    emit signal_obratnogo_perekl(flag);
        */
    }

}

QString LocalIP()                    //äã­ªæ¨ï çâ¥­¨ï á¢®¥£® IP (á ä¨«ìâà æ¨¥© IPv6)
{
    QString LocIP;
    QList<QHostAddress> addr = QNetworkInterface::allAddresses();

    if(addr.first().toString().count() < 16)
    {
        LocIP = addr.first().toString();
    }
    else
    {
        LocIP = addr.value(1).toString();
    }


    qDebug () << LocIP;

    return LocIP;
}

void TP_SMDM::selection_pult()
{
    if(IPaddrPLATA == "")
    {
        ipComputer = LocalIP();

        //emit myIP_QML_ChangeQML(ipComputer);

        QStringList listIPfromFunction;
        listIPfromFunction = ipComputer.split('.');

        selection[0]=IPfromQStringtochar(listIPfromFunction[0]);
        selection[1]=IPfromQStringtochar(listIPfromFunction[1]);
        selection[2]=IPfromQStringtochar(listIPfromFunction[2]);
        selection[3]=IPfromQStringtochar(listIPfromFunction[3]);

        //vibran_pult = 1;
        udpsocket65401->writeDatagram(selection,4,QHostAddress(IPaddrPLATA),65401);
    }
}

void TP_SMDM::selection_pult_ok()
{
    while(udpsocket65400->hasPendingDatagrams())
    {

        QByteArray datagram;
        datagram.resize(udpsocket65400->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        udpsocket65400->readDatagram(datagram.data(),datagram.size(),&sender,&senderPort);

        char a[4];

        for(int i=0;i<=3;i++)
        {
            a[i]=datagram[i];
        }

        if((a[0]==selection[0]) & (a[1]==selection[1]) & (a[2]==selection[2]) & (a[3]==selection[3]))
        {
            flag=1;

            // IPaddrPLATA = currentIPinComboBox;

            //emit signal_perekl(flag);

            qDebug () << " selection_pult_ok";

            IPvFILE();
        }
    }
}

void TP_SMDM::miganie()
{
    udpsocket65533->bind(65455);
    udpsocket65533->writeDatagram(noup,3,QHostAddress(currentIPinComboBox),65455);
}

void TP_SMDM::dHCP()
{
    lastIP=currentIPinComboBox;
    udpsocket65533->bind(60000);
    udpsocket65533->writeDatagram(noup,3,QHostAddress(currentIPinComboBox),60000);
}

void TP_SMDM::current_IP(int id)
{
    currentIPinComboBox=list[id];
    // emit currentIPinComboBox_QML_ChangeQML(currentIPinComboBox);
}

void TP_SMDM::IPvFILE()
{
    QFile file("lastIPfile.txt");
    QTextStream stream(&file);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        stream << IPaddrPLATA;
    }
    file.close();
}

void TP_SMDM::IPfromFILE()
{
    QString IPfromFILE = "";
    QFile file("lastIPfile.txt");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        IPfromFILE = file.readLine();
        if (list.contains(IPfromFILE)==false)          //¥á«¨ â ª®£® IP ¥éñ ­¥ ¡ë«®
        {

            int m = list.indexOf(lastIP);

            if (m==-1)
            {
                list.append(IPfromFILE);               //¤®¡ ¢«ï¥â â¥ªãé¥¥ §­ ç¥­¨¥ ¢ QStringList
            }
            else
            {
                list.replace(m,IPfromFILE);            //§ ¬¥­ï¥â áâ à®¥ §­ ç¥­¨¥ ­®¢ë¬
            }

            // emit listIP_PULT_QML_ChangeQML(list);
        }
    }
}

void TP_SMDM::osvobodit_pult()
{
    udpsocket65533->bind(65533);
    udpsocket65533->writeDatagram(noup,3,QHostAddress(IPaddrPLATA),55666);
}

//Номер положение, номер реле
void TP_SMDM::rele_portON(QStringList polojenie, int rele)
{
    QString temp;
    char a[13];
    char c[10];

    for(int i=0; i<10; i++)
    {
        temp.append(polojenie[i]);
    }
    strncpy(c, qPrintable(temp), sizeof(c));

    a[0]=noup[0];
    a[1]=noup[1];
    a[2]=noup[2];
    for(int i=0; i<10; i++)
    {
        a[i+3]=c[i];
        a[i+3]=a[i+3]-48;
    }

    if(rele==1)
    {
       // udpsocket65533->bind(65533);
        udpsocket65533->writeDatagram(a,13,QHostAddress(IPaddrPLATA),65533); //¢ª«îç¥­¨¥ ¯®àâ®¢ 1
    }
    if(rele==2)
    {
        udpsocket65531->writeDatagram(a,13,QHostAddress(IPaddrPLATA),65531); //¢ª«îç¥­¨¥ ¯®àâ®¢ 2
    }
    if(rele==3)
    {

        udpsocket65526->writeDatagram(a,13,QHostAddress(IPaddrPLATA),65526); //¢ª«îç¥­¨¥ ¯®àâ®¢ 3
    }
    if(rele==4)
    {
        udpsocket65523->writeDatagram(a,13,QHostAddress(IPaddrPLATA),65523); //¢ª«îç¥­¨¥ ¯®àâ®¢ 4
    }
}

void TP_SMDM::zaprosSOST_rele(int rele)
{
    if(rele==1)
    {
       // udpsocket65533->bind(65532);
        udpsocket65533->writeDatagram(noup,3,QHostAddress(IPaddrPLATA),65532);
        rele_seichas=1;
    }
    if(rele==2)
    {
       // udpsocket65533->bind(65529);
        udpsocket65533->writeDatagram(noup,3,QHostAddress(IPaddrPLATA),65529);
        rele_seichas=2;
    }
    if(rele==3)
    {
       // udpsocket65533->bind(65527);
        udpsocket65533->writeDatagram(noup,3,QHostAddress(IPaddrPLATA),65527);
        rele_seichas=3;
    }
    if(rele==4)
    {
       // udpsocket65533->bind(65524);
        udpsocket65533->writeDatagram(noup,3,QHostAddress(IPaddrPLATA),65524);
        rele_seichas=4;
    }
}

void TP_SMDM::readDatagram_rele(int rele)
{
    if (rele==1)
    {
        //this->thread()->msleep(10);
        while(udpsocket1->hasPendingDatagrams())
        {
            QByteArray datagram;
            datagram.resize(udpsocket1->pendingDatagramSize());
            QHostAddress sender;
            quint16 senderPort;
            udpsocket1->readDatagram(datagram.data(),datagram.size(),&sender,&senderPort);

            for(int i=0;i<10;i++)
            {
                list_k1[i]=QString::number(datagram[i],16);
            }
        }
        // emit list_k1_QML_ChangeQML(list_k1);
    }
    if (rele==2)
    {
        while(udpsocket3->hasPendingDatagrams())
        {
            QByteArray datagram;
            datagram.resize(udpsocket3->pendingDatagramSize());
            QHostAddress sender;
            quint16 senderPort;
            udpsocket3->readDatagram(datagram.data(),datagram.size(),&sender,&senderPort);

            for(int i=0;i<10;i++)
            {
                list_k2[i]=QString::number(datagram[i],16);
            }
        }
        //  emit list_k2_QML_ChangeQML(list_k2);
    }
    if (rele==3)
    {
        while(udpsocket4->hasPendingDatagrams())
        {
            QByteArray datagram;
            datagram.resize(udpsocket4->pendingDatagramSize());
            QHostAddress sender;
            quint16 senderPort;
            udpsocket4->readDatagram(datagram.data(),datagram.size(),&sender,&senderPort);

            for(int i=0;i<10;i++)
            {
                list_k3[i]=QString::number(datagram[i],16);
            }
        }
        //emit list_k3_QML_ChangeQML(list_k3);
    }
    if (rele==4)
    {
        while(udpsocket7->hasPendingDatagrams())
        {
            QByteArray datagram;
            datagram.resize(udpsocket7->pendingDatagramSize());
            QHostAddress sender;
            quint16 senderPort;
            udpsocket7->readDatagram(datagram.data(),datagram.size(),&sender,&senderPort);

            for(int i=0;i<10;i++)
            {
                list_k4[i]=QString::number(datagram[i],16);
            }
        }
        // emit list_k4_QML_ChangeQML(list_k4);
    }
}

void TP_SMDM::generatorON(int rele)
{
    char a[5];
    if(rele==1)
    {
        a[3]=0;
        a[4]=1;
    }

    else if(rele==2)
    {
        a[3]=1;
        a[4]=0;
    }

    else
    {
        a[3]=0;
        a[4]=0;
    }

    a[0]=noup[0];
    a[1]=noup[1];
    a[2]=noup[2];


    qDebug () << " Отправленна установка пульта # " << rele;

    udpsocket65521->writeDatagram(a,5,QHostAddress(IPaddrPLATA),65521);

    //udpsocket8->writeDatagram(a,5,QHostAddress(IPaddrPLATA),65521);
}

void TP_SMDM::analizatorON(int rele)
{
    char a[5];
    if(rele==1)
    {
        a[3]=0;
        a[4]=1;
    }

    else if(rele==2)
    {
        a[3]=1;
        a[4]=0;
    }

    else
    {
        a[3]=0;
        a[4]=0;
    }

    a[0]=noup[0];
    a[1]=noup[1];
    a[2]=noup[2];

    udpsocket65520->writeDatagram(a,5,QHostAddress(IPaddrPLATA),65520);
}

void TP_SMDM::StratRegyl()
{
    emit StartPower(1);

    Log("=================\n");
    Log("Старт Колибровки пульта.\n");
    Log2("Старт Колибровки пульта.\n");
    Micran1->Log("=================\n");
    Micran1->Log("Старт Колибровки пульта.\n");
    N9000->Log("=================\n");
    N9000->Log("Старт Колибровки пульта.\n");

    //  emit Log("Старт Колибровки пульта.\n");

    flagEnd = true;
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

    win_frequency->ProverkaGraph->clearGraphs();


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

                win_frequency->ProverkaGraph->addGraph();
                win_frequency->ProverkaGraph->graph(point-1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
                win_frequency->ProverkaGraph->graph(point-1)->setPen(QPen(qrand())); // line color blue for first graph
                win_frequency->ProverkaGraph->graph(point-1)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue
            }
        }
    }

    SQL_OneProverka = new QSqlQueryModel();

    FlagGoodAchH = true;

    flagRegylK0=false;

    flagRegylK0=true;
    m_mutex.lock();
    sem2.acquire();


    QTextStream  writeStream(file);
    //  SQL_OneProverka->setQuery("INSERT INTO Result (Date,IdLink,[Этап]) VALUES('"+dateStart.toString("dd.MM.yyyy  hh:mm:ss")+"','"+Link->data(Link->index(3,0), Qt::EditRole).toString()+"','"+etap+"')");
    writeStream << dateStart.toString("dd.MM.yyyy  hh:mm:ss")<<endl;
    writeStream << "K0: "<<endl;


    releA =0;
    flagRegyl_A=false;
    flagRegyl_B=false;


    AchHRegyl();// Промерить   К0 кабель


    // Set power out on
    viPrintf(Micran1->vi, "OUTPut:STATe OFF\r\n"); // Включить/Выключить выход СВЧ
    Micran1->Log("Включить выход ВЧ.\n");

    ListX.clear();
    ListY.clear();

    ListMin.clear();
    ListMax.clear();
    ListNEravn.clear();



    //Реле А
    for(int i=1;i <=19;i++) //Поменять i на 1
    {
        if( i==10)
        {
            ReleA(0);
        }
        else
        {

            if(i <=9)
            {
                releA = i;
                qDebug() <<" i = "<< releA;
                writeStream <<"A"<<releA<<":"<<endl;
            }
            else
            {
                releA =i-1;
                writeStream <<"A"<<releA<<":"<<endl;
                qDebug() <<" i-1 = "<<  releA;

            }
            flagRegyl_A = true;
            m_mutex.lock();
            sem.acquire();

            ReleA(i);

            qDebug() <<"После i = " <<  i;

            AchH(); // i Вход у пульта

            // Set power out on
            viPrintf(Micran1->vi, "OUTPut:STATe OFF\r\n"); // Включить/Выключить выход СВЧ
            Micran1->Log("Включить выход ВЧ.\n");


        }
    }


    ReleA(22);
    ReleB(22);

    //Реле Б
    for(int i=1;i <=19;i++) //Поменять i на 1
    {
        if( i==10)
        {
            ReleB(0);
        }
        else
        {

            if(i <=9)
            {
                releA = i;
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
            ReleB(i);

            qDebug() <<"После i = " <<  i;
            AchH(); // i Вход у пульта

            // Set power out on
            viPrintf(Micran1->vi, "OUTPut:STATe OFF\r\n"); // Включить/Выключить выход СВЧ
            Micran1->Log("Включить выход ВЧ.\n");

        }
    }

    ReleB(22);


    QMessageBox msgBox;
    msgBox.setText("Конец регулировки пульта.");
    msgBox.exec();


    Log("Конец Колибровки пульта.\n");
    Log2("Конец Колибровки пульта.\n");
    Micran1->Log("Конец Колибровки пульта.\n");
    N9000->Log("Конец Колибровки пульта.\n");
    emit Log("Конец Колибровки пульта.\n");

}

void TP_SMDM::AchH()
{
    N9000->Log("=================\n");
    Micran1->Log("=================\n");


    //view->item(3,0)->setBackgroundColor(Qt::yellow);
    // view->reset();


    double dResultY = 0;
    double dResultX = 0;
    char buff[9024];

    x.clear();
    y.clear();
    int end=GnFREQuencyStart;
    int point=0;

    GnPower = 0;



    for(int i=0;i<win_frequency->tableWidgetAChH->columnCount();i++)
    {

        for(int j=0;j<win_frequency->tableWidgetAChH->rowCount();j++)
        {


            if(point < 36)
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


        if(end == 950)
        {
            Sleep(1000);
        }
        else
        {
            this->thread()->msleep(200);
        }

        viPrintf(N9000->vi, "CALC:MARK:MAX\n");
        N9000->Log("Захватить максимум.\n");
        viQueryf(N9000->vi, "CALC:MARK:Y?\n", "%lf", &dResultY);
        N9000->Log("Считанный Y :"+QString::number(dResultY)+" дб\n");
        viQueryf(N9000->vi, "CALC:MARK:X?\n", "%lf", &dResultX);
        N9000->Log("Считанный X :"+QString::number(dResultX/pow(10,6))+" MHz\n");

        x.append(dResultX/pow(10,6));
        // dResultY -=GnPower;



        double ot = fabs(dResultY) - fabs(ListRegyl->value(0).value(y.count())); // Берем два числа по модулю отнимаем K0

        //Для графика умнажаем на -1
        ot *=(-1);

        y.append(ot);


        auto min1 = std::min_element(y.begin(),y.end());
        auto max1 = std::max_element(y.begin(),y.end());

        min = (*min1);
        max = (*max1);

        NeravnACHX = (*max1)-(*min1);


        end+=GnFREQuencyStep;
        date = QDateTime::currentDateTime();


        QTextStream  writeStream(file);

        writeStream << "X: " << QString::number(dResultX/pow(10,6))<<"\t Y: "<<QString::number(y.last())<<endl;

    }

    if(NeravnACHX >2)
    {
        qDebug()<< "Ошибка! Неравномерность АЧХ "<<NeravnACHX<<" > 2";


        //view->item(3,0)->setBackgroundColor(Qt::red);
        //view->reset();

        FlagGoodAchH = false;
    }
    else
    {
        qDebug()<< "Ок! Неравномерность АЧХ "<<NeravnACHX<<" < 2";
        // view->item(3,0)->setBackgroundColor(Qt::green);
        // view->reset();

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

void TP_SMDM::AchHRegyl()
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

    GnPower = 0;

    flag=true;
    //4 Проверка АЧХ

    QVector<double> list3;

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
        //  viQueryf(Micran1->vi,"SYSTem:ERRor?\n","%T",buff);
        //   Micran1->Log("Ошибка: "+QString(buff)+"\n");

        if(end == 950)
        {
            Sleep(1000);
        }
        else
        {
            this->thread()->msleep(200);
        }

        viPrintf(N9000->vi, "CALC:MARK:MAX\n");
        N9000->Log("Захватить максимум.\n");
        viQueryf(N9000->vi, "CALC:MARK:Y?\n", "%lf", &dResultY);
        N9000->Log("Считанный Y :"+QString::number(dResultY)+" дб\n");
        viQueryf(N9000->vi, "CALC:MARK:X?\n", "%lf", &dResultX);
        N9000->Log("Считанный X :"+QString::number(dResultX/pow(10,6))+" MHz\n");


        x.append(dResultX/pow(10,6));

        //  dResultY -=GnPower;

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

        list3.append(dResultY);

    }

    ListRegyl->append(list3);

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


void TP_SMDM::SetUi(Ui::Frequency *win_frequencyGet, Ui::TransferCoefficient *win_transferCoefficientGet,Micran_Gen*  Micran1Get,N9000A*   N9000Get)
{
    win_frequency = win_frequencyGet;

    win_transferCoefficient = win_transferCoefficientGet;

    Micran1 =  Micran1Get;

    N9000 =   N9000Get;

    GnFREQuencyStart = 950;
    GnFREQuencyEnd = 2150;
    GnFREQuencyStep = 50;
    GnPower    = 0;

}




void TP_SMDM::END()
{
    viPrintf(Micran1->vi, "OUTPut:STATe OFF\r\n");
    Micran1->Log("Включить выход ВЧ.\n");

    // Send "Clear status" command
    viPrintf(Micran1->vi, "*CLS\r\n");
    Micran1->Log("Cброс статуса.\n");


    ReleA(0);
    ReleB(0);
    ReleA(21);
    ReleB(21);

    udpsocket1->close();

    udpsocket65400->close();
    udpsocket65401->close();
    udpsocket65500->close();
    udpsocket65510->close();
    udpsocket65511->close();
    udpsocket65521->close();
    udpsocket65523->close();
    udpsocket65526->close();
    udpsocket65531->close();
    udpsocket65533->close();

    p_udpSocketOut->close(); // UDP soket для Переключения каналов реле Коммутатора СМ16-4

    //    udpsocket->close(); // UDP soket Для Переключения реле Пульта "A1"выход по "A9"
    //    udpsocket2->close(); // UDP soket Для Переключения реле Пульта "A10"выхода по "A18"
    //    udpsocket5->close(); // UDP soket Для Переключения реле Пульта "В1"выход до B"9"
    //    udpsocket6->close(); // UDP soket Для Переключения реле Пульта B"10" выхода до B"18"
    //    p_udpSocketOut->close(); // UDP soket для Переключения каналов реле Коммутатора СМ16-4

}
