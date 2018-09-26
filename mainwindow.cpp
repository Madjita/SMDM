#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    win_frequency(new Ui::Frequency),
    win_transferCoefficient(new Ui::TransferCoefficient),
    win_switchingSignalSM(new Ui::SwitchingSignalSM),
    win_switchingSignalSMDM(new Ui::SwitchingSignalSMDM),
    win_switchingSignalDM(new Ui::SwitchingSignalDM),
    win_power(new Ui::Power),
    win_power_2(new Ui::Power),
    win_frequency_2(new Ui::Frequency),
    win_transferCoefficient_2(new Ui::TransferCoefficient),
    win_switchingSignalSM_2(new Ui::SwitchingSignalSM),
    win_switchingSignalSMDM_2(new Ui::SwitchingSignalSMDM),
    win_switchingSignalDM_2(new Ui::SwitchingSignalDM)
{
    ui->setupUi(this);






    log_text_Micran = new  QTextEdit();
    log_text_TP = new  QTextEdit();
    log_text = new QTextEdit();
    log_text_Block = new QTextEdit();
    log_text_N9000 = new QTextEdit();
    log_text_Ist = new QTextEdit();

    CreateLog();
    CreateDevice();
    CreateTollBar();
    CreatePDF();
    LoadTable();
    CreateStartSeitingGraph();
    LoadFile();






    newLog = new Log();
    newLog->setBD(BD);

    threadSM = new thread_SM(Micran1,N9000,HMP2020,TP,ui->view);

    connect(threadSM,&thread_SM::SQL_add,this,&MainWindow::addBD);

    threadSM->SQL_OneProverka = new QSqlQueryModel();


    threadSM->setListRegyl(ListRegyl);
    threadSM->SetUi(win_power,win_frequency,win_transferCoefficient,win_switchingSignalSM);

    connect(this,&MainWindow::startSM,threadSM,&thread_SM::Work);
    connect(threadSM, &thread_SM::updateGraph, this, &MainWindow::UpdateGraph);
    connect(threadSM, &thread_SM::updateGraphPerestrouyka, this, &MainWindow::UpdateGraphPerestrouyka);
    connect(threadSM,SIGNAL(SetDataStart(QDateTime)), newLog, SLOT(SetDataStart(QDateTime)));
    connect(threadSM,&thread_SM::end, this, &MainWindow::END);
    connect(threadSM,&thread_SM::Log,this,&MainWindow::log_wr_message_Block);

    connect(threadSM,&thread_SM::Log2,this,&MainWindow::log_wr_message_Block);
    connect(threadSM,&thread_SM::StartFrequency,ui->tabWidget,&QTabWidget::setCurrentIndex);
    connect(threadSM,&thread_SM::StartPower,ui->tabWidget,&QTabWidget::setCurrentIndex);
    connect(threadSM,&thread_SM::StartPerestroyka,ui->tabWidget,&QTabWidget::setCurrentIndex);
    connect(TP,&TP_SMDM::SetImage,this,&MainWindow::SetImage);
    connect(TP,&TP_SMDM::SetImage2,this,&MainWindow::SetImage2);
    connect(TP,&TP_SMDM::SetImage3,this,&MainWindow::SetImage3);
    connect(TP,&TP_SMDM::SetImage4,this,&MainWindow::SetImage4);
    connect(threadSM,&thread_SM::LogClear,this,&MainWindow::log_wr_message_Block_claer);
    connect(threadSM,&thread_SM::CreateGraph,this,&MainWindow::CreateGraph2);
    connect(threadSM,&thread_SM::CreateGraphfrenq,this,&MainWindow::CreateGraphFrenq,Qt::QueuedConnection);
    connect(ui->view,SIGNAL(cellClicked(int,int)),threadSM,SLOT(SetProv(int,int)));




    threadDM = new thread_DM();
    threadSMDM = new thread_SMDM();


    connect(this,&MainWindow::Setlog_wr_message,newLog,&Log::log_wr_message);//,Qt::DirectConnection);
    connect(this,&MainWindow::Setlog_wr_message_Block,newLog,&Log::log_wr_message_Block);//,Qt::DirectConnection);
    connect(this,&MainWindow::Setlog_wr_message_Ist,newLog,&Log::log_wr_message_Ist);//,Qt::DirectConnection);
    connect(this,&MainWindow::Setlog_wr_message_Micran,newLog,&Log::log_wr_message_Micran);//,Qt::DirectConnection);
    connect(this,&MainWindow::Setlog_wr_message_TP,newLog,&Log::log_wr_message_TP);//,Qt::DirectConnection);
    connect(this,&MainWindow::Setlog_wr_message_N9000,newLog,&Log::log_wr_message_N9000);//,Qt::DirectConnection);


    connect(ui->view,&QTableWidget::cellClicked,this,&MainWindow::SetTabProverki);
    connect(Button_PDF,&QPushButton::clicked,this,&MainWindow::PDF);




}

MainWindow::~MainWindow()
{
    DisableDevice();

    threadSM->p_udpSocketOut->close();

    threadSM->thread()->terminate();

    delete ui;
}

void MainWindow::SetTabProverki(int r, int c)
{
    if( ui->view->item(r,0)->flags() != Qt::NoItemFlags)
    {

        if(ui->view->item(r,0)->checkState()==Qt::Checked)
        {

            if(r==0 || r == 1)
            {

                ui->tabWidget->setTabEnabled(0,true);

            }
            else
            {
                if(r!=2)
                {
                    ui->tabWidget->setTabEnabled(r-2,true);
                }
            }
        }
        else
        {
            if(r==0 || r == 1)
            {
                ui->tabWidget->setTabEnabled(0,false);
            }
            else
            {
                if(r!=2)
                {
                    ui->tabWidget->setTabEnabled(r-2,false);
                }
            }
        }
    }
}

void MainWindow::errorMessage(QString data)
{
    QMessageBox msgBox;
    msgBox.setText("Ошибка. Блок "+data+" не отвечает на запросы. Проверка остановленна.");
    msgBox.exec();

    threadSMDM->sem.release();
}


void MainWindow::CreateLog()
{
    log_text->setReadOnly(1); // установка только на чтение
    QScrollBar *scrol = new (QScrollBar);
    scrol->setPageStep(log_text->size().height());

    log_text->setVerticalScrollBar(scrol);
    log_text->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    QPalette log_palet = log_text->palette() ;
    log_palet.setColor(QPalette::Text, Qt::white);
    log_palet.setColor(QPalette::Base, Qt::darkGray);
    log_text->setPalette(log_palet);
    ui->dockWidget_Out->setWidget(log_text);


    //Лог Источника питания
    log_text_Ist->setReadOnly(1); // установка только на чтение
    QScrollBar *scrol2 = new (QScrollBar);
    scrol2->setPageStep(log_text_Ist->size().height());
    log_text_Ist->setVerticalScrollBar(scrol2);
    log_text_Ist->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    QPalette log_palet2 = log_text_Ist->palette() ;
    log_palet2.setColor(QPalette::Text, Qt::white);
    log_palet2.setColor(QPalette::Base, Qt::darkGray);
    log_text_Ist->setPalette(log_palet2);
    ui->dockWidget_Ist->setWidget(log_text_Ist);


    //Лог Микрана
    log_text_Micran->setReadOnly(1); // установка только на чтение
    QScrollBar *scrol3 = new (QScrollBar);
    scrol3->setPageStep(log_text_Micran->size().height());
    log_text_Micran->setVerticalScrollBar(scrol3);
    log_text_Micran->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    QPalette log_palet3 = log_text_Ist->palette() ;
    log_palet3.setColor(QPalette::Text, Qt::white);
    log_palet3.setColor(QPalette::Base, Qt::darkGray);
    log_text_Micran->setPalette(log_palet3);
    ui->dockWidget_Micran->setWidget(log_text_Micran);


    //Лог ТП
    log_text_TP->setReadOnly(1); // установка только на чтение
    QScrollBar *scrol4 = new (QScrollBar);
    scrol4->setPageStep(log_text_TP->size().height());
    log_text_TP->setVerticalScrollBar(scrol4);
    log_text_TP->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    QPalette log_palet4 = log_text_Ist->palette() ;
    log_palet4.setColor(QPalette::Text, Qt::white);
    log_palet4.setColor(QPalette::Base, Qt::darkGray);
    log_text_TP->setPalette(log_palet4);
    ui->dockWidget_TP->setWidget(log_text_TP);


    //Лог Блок
    log_text_Block->setReadOnly(1); // установка только на чтение
    QScrollBar *scrol5 = new (QScrollBar);
    scrol5->setPageStep(log_text_Block->size().height());
    log_text_Block->setVerticalScrollBar(scrol5);
    log_text_Block->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    QPalette log_palet5 = log_text_Block->palette() ;
    log_palet5.setColor(QPalette::Text, Qt::white);
    log_palet5.setColor(QPalette::Base, Qt::darkGray);
    log_text_Block->setPalette(log_palet5);
    ui->dockWidget_Block->setWidget(log_text_Block);


    //Лог Анализатора
    log_text_N9000->setReadOnly(1); // установка только на чтение
    QScrollBar *scrol6 = new (QScrollBar);
    scrol6->setPageStep(log_text_N9000->size().height());
    log_text_N9000->setVerticalScrollBar(scrol6);
    log_text_N9000->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    QPalette log_palet6 = log_text_N9000->palette() ;
    log_palet6.setColor(QPalette::Text, Qt::white);
    log_palet6.setColor(QPalette::Base, Qt::darkGray);
    log_text_N9000->setPalette(log_palet6);
    ui->dockWidget_N9000->setWidget(log_text_N9000);
}

void MainWindow::CreateDevice()
{
    //Таймер для графиков
    fps =200;

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(UpdateGraph()));

    timerKolibrovka = new QTimer();
    connect(timerKolibrovka, SIGNAL(timeout()), this, SLOT(UpdateGraphRegylirovka()));

    timer_perestroyka = new QTimer();

    connect(timer, SIGNAL(timeout()), this, SLOT(UpdateGraphPerestrouyka()));


    Micran1 = new Micran_Gen();

    connect(this, &MainWindow::Connect_Micran,Micran1, &Micran_Gen::Connect);
    connect(Micran1,&Micran_Gen::Log,this,&MainWindow::log_wr_message_Micran);
    connect(Micran1,&Micran_Gen::Log2,this,&MainWindow::log_wr_message);
    connect(Micran1,&Micran_Gen::LogClear,this,&MainWindow::log_wr_message_Micran_claer);

    HMP2020 = new PowerSourse();

    connect(this,&MainWindow::Connect_HMP2020,HMP2020,&PowerSourse::Connect);

    connect(HMP2020,&PowerSourse::Log,this,&MainWindow::log_wr_message_Ist); // Qt::DirectConnection
    connect(HMP2020,&PowerSourse::Log2,this,&MainWindow::log_wr_message); // Qt::DirectConnection
    connect(HMP2020,&PowerSourse::LogClear,this,&MainWindow::log_wr_message_Ist_claer);

    N9000 = new N9000A();

    connect(this,&MainWindow::Connect_N9000,N9000,&N9000A::Connect, Qt::DirectConnection);
    connect(N9000,&N9000A::Log,this,&MainWindow::log_wr_message_N9000); // Qt::DirectConnection
    connect(N9000,&N9000A::Log2,this,&MainWindow::log_wr_message); //
    connect(N9000,&N9000A::LogClear,this,&MainWindow::log_wr_message_N9000_claer);

    TP = new TP_SMDM();

    connect(this,&MainWindow::Connect_TP,TP,&TP_SMDM::zapros_readip);
    connect(TP,&TP_SMDM::LogClear,this,&MainWindow::log_wr_message_TP_claer);

    connect(TP,&TP_SMDM::StartPower,ui->tabWidget,&QTabWidget::setCurrentIndex);
    TP->SetUi(win_frequency,win_transferCoefficient,Micran1,N9000);
    connect(this,&MainWindow::KolibrovkaSignal,TP,&TP_SMDM::Kolibrovka);

    connect(TP,&TP_SMDM::SetImage,this,&MainWindow::SetImage);//);
    connect(TP,&TP_SMDM::SetImage2,this,&MainWindow::SetImage2);//);
    connect(TP,&TP_SMDM::SetImage3,this,&MainWindow::SetImage3);//);
    connect(TP,&TP_SMDM::SetImage4,this,&MainWindow::SetImage4);//);
    connect(TP,&TP_SMDM::Log,this,&MainWindow::log_wr_message_TP); // Qt::DirectConnection
    connect(TP,&TP_SMDM::Log2,this,&MainWindow::log_wr_message); // Qt::DirectConnection

    char a_start[10] = {0,0,0,0,0,0,0,0,0};
    SetImage(a_start);
    SetImage2(a_start);
    SetImage3(a_start);
    SetImage4(a_start);


    BD = new BData();
    BD->Connect_BD();



    connect(BD,&BData::Log,this,&MainWindow::log_wr_message); // Qt::DirectConnection


//    connect(this,&MainWindow::Connect_BD,BD,&BData::Connect_BD,Qt::DirectConnection);

//    emit Connect_BD();


    qRegisterMetaType<QVector<int>>("QVector<int>");

    //qRegisterMetaType<QTextBlock>("QTextBlock");
    // qRegisterMetaType<QTextCursor>("QTextCursor");
    // qRegisterMetaType<QVector<int>>("QVector<int>");

    //connect(this,&MainWindow::StartTimer,BD,&BData::StartTimer);
}

void MainWindow::CreateTollBar()
{
    NameDevice_Lable = new QLabel();
    NameDevice_Lable->setText("Устройство:");
    EtapGroup_Device = new QComboBox(); // Объявления меню для выбора устройств из БД
    NDevice_Lable = new QLabel();
    NDevice_Lable->setText(" №:");
    NDevice_ComboBox = new QComboBox();
    EtapLabel = new QLabel();
    EtapLabel->setText(" Этап: ");

    //RadioButton ПСИ и ПИ
    EtapGroup_PSI_RadioButton = new QRadioButton();
    GroupBox_RadioButton = new QGroupBox();
    EtapGroup_PSI_RadioButton = new QRadioButton("ПСИ");
    EtapGroup_PSI_RadioButton->setChecked(true);
    EtapGroup_PI_RadioButton = new QRadioButton("ПИ");
    EtapGroup_Regylirovka_RadioButton = new QRadioButton("Регулировка");

    //Горизонтальный Лаяут для отображения радио кнопок в GroupBox_RadioButton
    vbox = new QHBoxLayout();
    vbox->addWidget(EtapGroup_PSI_RadioButton);
    vbox->addWidget(EtapGroup_PI_RadioButton);
    vbox->addWidget(EtapGroup_Regylirovka_RadioButton);
    vbox->addStretch(1);

    //GroupBox_RadioButton и фиксация в 200 пикселей по длине
    GroupBox_RadioButton->setLayout(vbox);
    GroupBox_RadioButton->setFixedWidth(200);
    EtapGroup_PI_RadioButton->setToolTip("Периодические испытания");
    EtapGroup_PSI_RadioButton->setToolTip("Приемо сдатачные испытания");
    EtapGroup_Regylirovka_RadioButton->setToolTip("Регулировка");

    //Настройка IP адресса
    win_ipadres = new MoyDialog();

    //Меню регулировка
    win_Regylirovka = new Regylirovka();

    connect(win_Regylirovka->getQPushButtonObject(),&QPushButton::clicked, this, &MainWindow::Kolibrovka);





    //    //Настройка Поиска результатов
    //    FindResult = new QDialog();
    //    //  win_findResult.setupUi(FindResult);

    //    //Настройка Отобржанения найденного результата
    //    CheckResult = new QDialog();
    //    //  win_chekResult.setupUi(CheckResult);

    EtapGroup = new QComboBox();
    EtapGroup->setFixedWidth(400); //Максимальная Длина  QComboBox

    //Кнопка Опроса устройств для готовности к работе
    Button_StartOpros = new QPushButton("Подключить");
    Button_StartOpros->setToolTip(tr("Начать опрос состоятиния устройств")); // задание всплывающей подсказки
    Button_EndOpros = new QPushButton("Отключить");
    Button_EndOpros->setDisabled(1);
    Button_EndOpros->setToolTip(tr("Оборвать соединение с  устройствами")); // задание всплывающей подсказки


    Button_StartProverka = new QPushButton("Старт");
    Button_StartProverka->setDisabled(1);
    Button_StartProverka->setToolTip(tr("Начать автоматическую проверку")); // задание всплывающей подсказки


    Button_Result = new QPushButton("Результаты");
    Button_Result->setToolTip(tr("Просмотр результатов")); // задание всплывающей подсказки
    connect(Button_Result,&QPushButton::clicked, this,&MainWindow::Result);


    Button_Regylirovka = new QPushButton("Колибровка ТП-СМДМ");
    Button_Regylirovka->setToolTip(tr("Колибровка пульта ТП-СМДМ")); // задание всплывающей подсказки
    connect(Button_Regylirovka,&QPushButton::clicked, this, &MainWindow::Kolibrovka);

    //Настройка Отображения NDevice_ComboBox and EtapGroup_Device
    NDevice_ComboBox->setFixedHeight(ui->mainToolBar->geometry().height());
    NDevice_ComboBox->setFixedWidth(80);
    EtapGroup_Device->setFixedHeight(ui->mainToolBar->geometry().height());
    EtapGroup_Device->setFixedWidth(80);

    // Добавление в mainToolBar (меню сверху)
    ui->mainToolBar->addWidget(NameDevice_Lable); // Lable Имя устройства
    ui->mainToolBar->addWidget(EtapGroup_Device); // Меню для выбора устройств из БД
    ui->mainToolBar->addWidget(NDevice_Lable); // Lable Номера устройства
    ui->mainToolBar->addWidget(NDevice_ComboBox); // Поле для номера устройства
    ui->mainToolBar->addWidget(GroupBox_RadioButton);
    ui->mainToolBar->addWidget(EtapLabel);
    ui->mainToolBar->addWidget(EtapGroup);
    ui->mainToolBar->addWidget(Button_EndOpros);
    ui->mainToolBar->addWidget(Button_StartOpros);
    ui->mainToolBar->addWidget(Button_StartProverka);
    ui->mainToolBar->addWidget(Button_Result);
    ui->mainToolBar->addWidget(Button_Regylirovka);

    //Загрузка Устрйоств в комбобокс
    QSqlQuery query;

    query = BD->zapros("SELECT * FROM Device");

    do
    {
        EtapGroup_Device->addItem(query.value(1).toString());

    }while (query.next());


    //Сигналы с ПИ , ПСИ , Регулировка
    connect(EtapGroup_PI_RadioButton, &QRadioButton::clicked,this,&MainWindow::GroupBox_RadioButtonClick);
    connect(EtapGroup_PSI_RadioButton, &QRadioButton::clicked,this,&MainWindow::GroupBox_RadioButtonClick);
    connect(EtapGroup_Regylirovka_RadioButton, &QRadioButton::clicked,this,&MainWindow::GroupBox_RadioButtonClick);

    //Сигналы с Нахождения Серийного номера и загрузки таблицы
    connect(EtapGroup_Device,&QComboBox::currentTextChanged, this,&MainWindow::DeviceSirealNumber);
    connect(NDevice_ComboBox,&QComboBox::currentTextChanged, this,&MainWindow::LoadTable);
    connect(EtapGroup,SIGNAL(activated(int)), this,SLOT(LoadTable()));

    connect(EtapGroup_PI_RadioButton,SIGNAL(clicked(bool)), this, SLOT(LoadTable()));
    connect(EtapGroup_PSI_RadioButton,SIGNAL(clicked(bool)), this, SLOT(LoadTable()));
    connect(EtapGroup_Regylirovka_RadioButton,SIGNAL(clicked(bool)), this, SLOT(LoadTable()));

    //Вывод меню настройки IP адресов
    connect(ui->action_IP,&QAction::triggered, this,&MainWindow::Adresa_nastroika);

    // Меню регулировки
    connect(ui->action_Reg, &QAction::triggered, this, &MainWindow::look_Regyl);

    //Connect Button_StartOpros
    connect(Button_StartOpros, SIGNAL(clicked()), this, SLOT(Proverka_StartOpros()));
    connect(Button_EndOpros,&QPushButton::clicked, this,&MainWindow::DisableDevice);
    //Connect Button_StartOpros
    connect(Button_StartProverka, &QPushButton::clicked, this, &MainWindow::Proverka_Start);


    //Загрузка Этапов
    QSqlQueryModel* SQL_Etap = nullptr;

    SQL_Etap = BD->zaprosQueryModel("SELECT Etaps FROM  Etap Where Etap.ПСИ = 1");


    for(int i=0;i< SQL_Etap->rowCount();i++)
    {
        EtapGroup->addItem(SQL_Etap->data(SQL_Etap->index(i,0), Qt::EditRole).toString());
    }

    EtapGroup->setCurrentIndex(0);
    DeviceSirealNumber(EtapGroup_Device->currentText());

    delete SQL_Etap;

    Button_StartProverka->setShortcut(tr("F5"));
    Button_StartOpros->setShortcut(tr("F5"));
    Button_EndOpros->setShortcut(tr("F4"));


}



void MainWindow::Kolibrovka()
{
    timerKolibrovka->start(200);

    emit Connect_TP();

    emit Connect_N9000(win_ipadres->getIP_N9000());


    emit Connect_HMP2020(win_ipadres->getIP_HMP2020());


    emit Connect_Micran(win_ipadres->getIP_Micran());


    emit KolibrovkaSignal();
}


void MainWindow::Proverka_StartOpros()
{
    Button_StartOpros->setDisabled(1);

    TP->LogClear();
    Micran1->LogClear();
    HMP2020->LogClear();
    N9000->LogClear();

    emit Connect_TP();

    emit Connect_N9000(win_ipadres->getIP_N9000());

    emit Connect_HMP2020(win_ipadres->getIP_HMP2020());


    emit Connect_Micran(win_ipadres->getIP_Micran());


    Button_StartProverka->setDisabled(0); // Разрешить нажатие на кнопку старт
    Button_EndOpros->setDisabled(0); // Разрешить нажатие на кнопку Отключить



    /*if(HMP2020->viStatus < VI_SUCCESS || N9000->viStatus < VI_SUCCESS || Micran1->viStatus < VI_SUCCESS)
    {
        DisableDevice();
        Button_StartOpros->setDisabled(0);
    }
    else
    {
        Button_StartProverka->setDisabled(0); // Разрешить нажатие на кнопку старт
        Button_EndOpros->setDisabled(0); // Разрешить нажатие на кнопку Отключить
    }*/

}

void MainWindow::DisableDevice()
{
    HMP2020->DisConnect();
    Micran1->DisConnect();
    N9000->DisConnect();

    TP->LogClear();
    Micran1->LogClear();
    HMP2020->LogClear();
    N9000->LogClear();


    TP->Log("Соединение с устройством разорванно!");
    TP->Log("===================\n");



    HMP2020->Log("Соединение с устройством разорванно!");
    HMP2020->Log("===================\n");
    N9000->Log("Соединение с устройством разорванно!");
    N9000->Log("===================\n");
    Micran1->Log("Соединение с устройством разорванно!");
    Micran1->Log("===================\n");

    HMP2020->connected = false;
    N9000->connected = false;
    Micran1->connected = false;

    N9000->Log2("Соединение с устройствами разорванно!\n");

    Button_EndOpros->setDisabled(1);
    Button_StartOpros->setDisabled(0);
    Button_StartProverka->setDisabled(1);

}


void MainWindow::DeviceSirealNumber(QString device)
{

    NDevice_ComboBox->blockSignals(1);
    NDevice_ComboBox->clear();
    NDevice_ComboBox->blockSignals(0);

    QSqlQuery query;
    query = BD->zapros("SELECT * FROM DeviceSerial JOIN Device ON Device.IdDevice=DeviceSerial.IdDevice WHERE Device.[Название устройства]='"+device+"'");

    do
    {
        NDevice_ComboBox->addItem(query.value(2).toString());

    }while (query.next());
}



// Насатройка IP
void MainWindow::Adresa_nastroika()
{
    if(win_ipadres->exec())
    {
        qDebug() << "вышли ";
        char adress[40];

        sprintf(adress, "TCPIP::%s::8888::SOCKET", qPrintable(win_ipadres->getIP_Micran()));
        Micran1->viAddres = adress;

        char adressN900[40];
        char adressCNT53230[40];
        char *adr_ptr = adressN900;
        char *adr_ptr2 = adressCNT53230;
        sprintf(adressN900, "TCPIP0::%s::inst0::INSTR", qPrintable(win_ipadres->getIP_N9000()));
        N9000->viAddres = adr_ptr;

        sprintf(adressCNT53230, "TCPIP0::%s::5025::SOCKET", qPrintable(win_ipadres->getIP_HMP2020()));
        HMP2020->viAddres = adr_ptr2;


        qDebug()<< Micran1->viAddres;
        qDebug()<< N9000->viAddres;
        qDebug()<< HMP2020->viAddres;
    }

}

void MainWindow::look_Regyl()
{
    win_Regylirovka->Clear_textEdit_AB();
    QString str = "Регулировка не проводилась.";

    win_Regylirovka->label_DataSetText(str);

    win_Regylirovka->show();

    LoadFile();

}

void MainWindow::LoadFile()
{
    connect(win_Regylirovka->getQTextEditObject(),&QTextEdit::cursorPositionChanged, this,&MainWindow::PutRegyl,Qt::DirectConnection);

    QFile* file = new QFile("Regylirovka.txt");

    ListRegyl = new QVector<QVector<double>>;

    if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "No find file";
    }
    else
    {

        tableRegyl = new QVector<QVector<QTableWidgetItem*>>;

        tableRegyl2 = new QVector<QVector<QTableWidgetItem*>>;

        win_Regylirovka->label_DataSetText(file->readLine().split('\n').first());

        for(int i=0; i <=36;i++)
        {

            if(i == 0)
            {
                win_Regylirovka->insert_textEdit_AB("K0\n");
                file->readLine();
            }
            else
            {
                if(i <= 18)
                {
                    win_Regylirovka->insert_textEdit_AB("A"+QString::number(i)+"\n");
                }
                else
                {
                    win_Regylirovka->insert_textEdit_AB("B"+QString::number(i-18)+"\n");
                }
            }

            QVector<QTableWidgetItem*> list;
            QVector<QTableWidgetItem*> list2;
            QVector<double> list3;

            QString str;
            do
            {
                QTableWidgetItem* item = new QTableWidgetItem();

                str = file->readLine();

                if(str.split(':').first() == "X")
                {
                    QString y = str.split("Y:").last();

                    QString x = QString(str.split("Y:").first()).split("X:").last();


                    item->setText(QString::number(x.toDouble()));
                    list.append(item);

                    item =new QTableWidgetItem();
                    item->setText(QString::number(y.toDouble()));
                    list2.append(item);
                    list3.append(y.toDouble());
                }

            }
            while(str.split(':').first() == "X");

            tableRegyl->append(list);
            tableRegyl2->append(list2);
            ListRegyl->append(list3);

        }
        file->close();
    }
}

//Функция нажатия на выбора входа выхода регулировки
void MainWindow::PutRegyl()
{


    win_Regylirovka->tableRegyl = tableRegyl;
    win_Regylirovka->tableRegyl2 = tableRegyl2;

    win_Regylirovka->PutRegyl();



}

//Функция обработки нажатия выбора ПСИ или ПИ
void MainWindow::GroupBox_RadioButtonClick()
{
    EtapGroup->clear();

    QSqlQueryModel* SQL_Etap = nullptr;

    if(EtapGroup_PSI_RadioButton->isChecked() == true)
    {
        SQL_Etap  = BD->zaprosQueryModel("SELECT Etaps FROM  Etap Where Etap.ПСИ = 1");

        for(int i=0;i< SQL_Etap->rowCount();i++)
        {
            EtapGroup->addItem(SQL_Etap->data(SQL_Etap->index(i,0), Qt::EditRole).toString());
        }
    }

    if(EtapGroup_PI_RadioButton->isChecked() == true)
    {

        SQL_Etap  = BD->zaprosQueryModel("SELECT Etaps FROM  Etap Where Etap.ПИ = 1");

        for(int i=0;i< SQL_Etap->rowCount();i++)
        {
            EtapGroup->addItem(SQL_Etap->data(SQL_Etap->index(i,0), Qt::EditRole).toString());
        }
    }
    if(EtapGroup_Regylirovka_RadioButton->isChecked() == true)
    {

        SQL_Etap  = BD->zaprosQueryModel("SELECT Etaps FROM  Etap Where Etap.Регулировка = 1");

        for(int i=0;i< SQL_Etap->rowCount();i++)
        {
            EtapGroup->addItem(SQL_Etap->data(SQL_Etap->index(i,0), Qt::EditRole).toString());
        }
    }

    EtapGroup->setCurrentIndex(0);

    ui->mainToolBar->addWidget(EtapGroup);
    ui->mainToolBar->addWidget(Button_EndOpros);
    ui->mainToolBar->addWidget(Button_StartOpros);
    ui->mainToolBar->addWidget(Button_StartProverka);
    ui->mainToolBar->addWidget(Button_Result);
    ui->mainToolBar->addWidget(Button_PDF);
    ui->mainToolBar->addWidget(Button_Regylirovka);

}

void MainWindow::LoadTable()
{

    if(EtapGroup_PSI_RadioButton->isChecked() == true)
    {
        log_wr_message("Загрузка проверок для блока: "+ EtapGroup_Device->currentText()+" №"+NDevice_ComboBox->currentText()+" на этапе: ПСИ ->"+EtapGroup->currentText()+"\n");
    }
    if(EtapGroup_PI_RadioButton->isChecked() == true)
    {
        log_wr_message("Загрузка проверок для блока: "+ EtapGroup_Device->currentText()+" №"+NDevice_ComboBox->currentText()+" на этапе: ПИ ->"+EtapGroup->currentText()+"\n");
    }
    if(EtapGroup_Regylirovka_RadioButton->isChecked() == true)
    {
        log_wr_message("Загрузка проверок для блока: "+ EtapGroup_Device->currentText()+" №"+NDevice_ComboBox->currentText()+" на этапе: Регулировка ->"+EtapGroup->currentText()+"\n");
    }

    QSqlQueryModel *SQL_chekProverka = new QSqlQueryModel();

    QSqlQueryModel* model = nullptr;

    if(EtapGroup_PSI_RadioButton->isChecked() == true)
    {
        model = BD->zaprosQueryModel("SELECT [Имя проверки],[Размерность]"
                                     "FROM Proverka JOIN Device ON Device.IdDevice=DeviceSerial.IdDevice JOIN DeviceSerial ON Link.IdDevice == DeviceSerial.IdSerial JOIN LInk ON Link.IdEtap == Etap.IdEtap JOIN Etap ON Link.IdProverka == Proverka.IdProverka Where Device.[Название устройства] = '"+EtapGroup_Device->currentText()+"' AND DeviceSerial.[Серийный номер] = '"+NDevice_ComboBox->currentText()+"' AND Etap.Etaps = '" +EtapGroup->currentText()+"' AND Etap.ПСИ = 1");

        SQL_chekProverka->setQuery("SELECT [Описание]"
                                   "FROM Proverka JOIN Device ON Device.IdDevice=DeviceSerial.IdDevice JOIN DeviceSerial ON Link.IdDevice == DeviceSerial.IdSerial JOIN LInk ON Link.IdEtap == Etap.IdEtap JOIN Etap ON Link.IdProverka == Proverka.IdProverka Where Device.[Название устройства] = '"+EtapGroup_Device->currentText()+"' AND DeviceSerial.[Серийный номер] = '"+NDevice_ComboBox->currentText()+"' AND Etap.Etaps = '" +EtapGroup->currentText()+"' AND Etap.ПСИ = 1");

    }

    if(EtapGroup_PI_RadioButton->isChecked() == true)
    {
        model = BD->zaprosQueryModel("SELECT [Имя проверки],[Размерность]"
                                     "FROM Proverka JOIN Device ON Device.IdDevice=DeviceSerial.IdDevice JOIN DeviceSerial ON Link.IdDevice == DeviceSerial.IdSerial JOIN LInk ON Link.IdEtap == Etap.IdEtap JOIN Etap ON Link.IdProverka == Proverka.IdProverka Where Device.[Название устройства] = '"+EtapGroup_Device->currentText()+"' AND DeviceSerial.[Серийный номер] = '"+NDevice_ComboBox->currentText()+"' AND Etap.Etaps = '" +EtapGroup->currentText()+"' AND Etap.ПИ = 1");

        SQL_chekProverka->setQuery("SELECT [Описание]"
                                   "FROM Proverka JOIN Device ON Device.IdDevice=DeviceSerial.IdDevice JOIN DeviceSerial ON Link.IdDevice == DeviceSerial.IdSerial JOIN LInk ON Link.IdEtap == Etap.IdEtap JOIN Etap ON Link.IdProverka == Proverka.IdProverka Where Device.[Название устройства] = '"+EtapGroup_Device->currentText()+"' AND DeviceSerial.[Серийный номер] = '"+NDevice_ComboBox->currentText()+"' AND Etap.Etaps = '" +EtapGroup->currentText()+"' AND Etap.ПИ = 1");
    }

    if(EtapGroup_Regylirovka_RadioButton->isChecked() == true)
    {
        model = BD->zaprosQueryModel("SELECT [Имя проверки],[Размерность]"
                                     "FROM Proverka JOIN Device ON Device.IdDevice=DeviceSerial.IdDevice JOIN DeviceSerial ON Link.IdDevice == DeviceSerial.IdSerial JOIN LInk ON Link.IdEtap == Etap.IdEtap JOIN Etap ON Link.IdProverka == Proverka.IdProverka Where Device.[Название устройства] = '"+EtapGroup_Device->currentText()+"' AND DeviceSerial.[Серийный номер] = '"+NDevice_ComboBox->currentText()+"' AND Etap.Etaps = '" +EtapGroup->currentText()+"' AND Etap.[Регулировка] = 1");

        SQL_chekProverka->setQuery("SELECT [Описание]"
                                   "FROM Proverka JOIN Device ON Device.IdDevice=DeviceSerial.IdDevice JOIN DeviceSerial ON Link.IdDevice == DeviceSerial.IdSerial JOIN LInk ON Link.IdEtap == Etap.IdEtap JOIN Etap ON Link.IdProverka == Proverka.IdProverka Where Device.[Название устройства] = '"+EtapGroup_Device->currentText()+"' AND DeviceSerial.[Серийный номер] = '"+NDevice_ComboBox->currentText()+"' AND Etap.Etaps = '" +EtapGroup->currentText()+"' AND Etap.[Регулировка] = 1");
    }


    if (model->lastError().isValid())
    {
        qDebug() << model->lastError();
    }


    ui->view->setObjectName(QStringLiteral("Table"));
    ui->view->setRowCount(model->rowCount());
    ui->view->setColumnCount(model->columnCount());


    ui->tabWidget->clear();

    QPixmap R1_Gray(":/images/circle_gray.png");

    //  QWidget *ProverkaList1 = new QWidget(); // окно проверки
    QWidget *ProverkaList2 = new QWidget(); // окно проверки
    QWidget *ProverkaList3 = new QWidget(); // окно проверки
    QWidget *ProverkaList4 = new QWidget(); // окно проверки
    QWidget *ProverkaList5 = new QWidget(); // окно проверки



    int point =0;

    for(int i=0;i< model->columnCount();i++)
    {
        QTableWidgetItem * itemHeader = new QTableWidgetItem();

        itemHeader->setText( model->headerData(i,Qt::Horizontal,Qt::DisplayRole).toString());

        ui->view->setHorizontalHeaderItem(i,itemHeader);

        for(int j=0;j < model->rowCount();j++)
        {


            QModelIndex newIndex = model->index(j,i);

            QTableWidgetItem * item = new QTableWidgetItem();

            item->setText(model->data(newIndex, Qt::EditRole).toString());



            if(i==0)
            {
                item->setToolTip(SQL_chekProverka->data(newIndex,Qt::EditRole).toString());
            }

            ui->view->setItem(j,i,item);

            if(i==0)
            {
                ui->view->item(j,0)->setCheckState(Qt::Checked);


                if(ui->view->item(j,0)->text() == "Power")
                {

                    win_power->setupUi(ProverkaList2);
                    win_power->pushButton_10V->setIcon(R1_Gray);
                    win_power->pushButton_10V->setIconSize(win_power->pushButton_10V->size());

                    win_power->pushButton_32V->setIcon(R1_Gray);
                    win_power->pushButton_32V->setIconSize(win_power->pushButton_32V->size());

                    win_power->pushButton_27V->setIcon(R1_Gray);
                    win_power->pushButton_27V->setIconSize(win_power->pushButton_27V->size());

                    ui->tabWidget->addTab(ProverkaList2,ui->view->item(j,0)->text());

                }

                if(ui->view->item(j,0)->text() == "Frequency")
                {

                    point++;
                    win_frequency->setupUi(ProverkaList3);

                    ui->tabWidget->addTab(ProverkaList3,ui->view->item(j,0)->text());

                }

                if(ui->view->item(j,0)->text() == "TransferCoefficient")
                {

                    win_transferCoefficient->setupUi(ProverkaList4);

                    ui->tabWidget->addTab(ProverkaList4,ui->view->item(j,0)->text());

                }

                if(ui->view->item(j,0)->text() == "SwitchingSignalSM")
                {

                    win_switchingSignalSM->setupUi(ProverkaList5);

                    win_switchingSignalSM->pushButton_1->setIcon(R1_Gray);
                    win_switchingSignalSM->pushButton_1->setIconSize(win_switchingSignalSM->pushButton_1->size());

                    win_switchingSignalSM->pushButton_2->setIcon(R1_Gray);
                    win_switchingSignalSM->pushButton_2->setIconSize(win_switchingSignalSM->pushButton_2->size());
                    win_switchingSignalSM->pushButton_3->setIcon(R1_Gray);
                    win_switchingSignalSM->pushButton_3->setIconSize(win_switchingSignalSM->pushButton_3->size());
                    win_switchingSignalSM->pushButton_4->setIcon(R1_Gray);
                    win_switchingSignalSM->pushButton_4->setIconSize(win_switchingSignalSM->pushButton_4->size());

                    ui->tabWidget->addTab(ProverkaList5,ui->view->item(j,0)->text());

                }

                if(ui->view->item(j,0)->text() == "SwitchingSignalDM") //Другое нужно
                {

                    win_switchingSignalDM->setupUi(ProverkaList5);

                    win_switchingSignalDM->pushButton_1->setIcon(R1_Gray);
                    win_switchingSignalDM->pushButton_1->setIconSize(win_switchingSignalDM->pushButton_1->size());

                    win_switchingSignalDM->pushButton_2->setIcon(R1_Gray);
                    win_switchingSignalDM->pushButton_2->setIconSize(win_switchingSignalDM->pushButton_2->size());
                    win_switchingSignalDM->pushButton_3->setIcon(R1_Gray);
                    win_switchingSignalDM->pushButton_3->setIconSize(win_switchingSignalDM->pushButton_3->size());
                    win_switchingSignalDM->pushButton_4->setIcon(R1_Gray);
                    win_switchingSignalDM->pushButton_4->setIconSize(win_switchingSignalDM->pushButton_4->size());

                    ui->tabWidget->addTab(ProverkaList5,ui->view->item(j,0)->text());

                }

                if(ui->view->item(j,0)->text() == "SwitchingSignalSMDM") //Другое нужно
                {

                    win_switchingSignalSMDM->setupUi(ProverkaList5);

                    win_switchingSignalSMDM->pushButton_PRM_1->setIcon(R1_Gray);
                    win_switchingSignalSMDM->pushButton_PRM_1->setIconSize(win_switchingSignalSMDM->pushButton_PRM_1->size());

                    win_switchingSignalSMDM->pushButton_PRM_2->setIcon(R1_Gray);
                    win_switchingSignalSMDM->pushButton_PRM_2->setIconSize(win_switchingSignalSMDM->pushButton_PRM_2->size());
                    win_switchingSignalSMDM->pushButton_PRM_3->setIcon(R1_Gray);
                    win_switchingSignalSMDM->pushButton_PRM_3->setIconSize(win_switchingSignalSMDM->pushButton_PRM_3->size());
                    win_switchingSignalSMDM->pushButton_PRM_4->setIcon(R1_Gray);
                    win_switchingSignalSMDM->pushButton_PRM_4->setIconSize(win_switchingSignalSMDM->pushButton_PRM_4->size());

                    win_switchingSignalSMDM->pushButton_PRD_1->setIcon(R1_Gray);
                    win_switchingSignalSMDM->pushButton_PRD_1->setIconSize(win_switchingSignalSMDM->pushButton_PRD_1->size());

                    win_switchingSignalSMDM->pushButton_PRD_2->setIcon(R1_Gray);
                    win_switchingSignalSMDM->pushButton_PRD_2->setIconSize(win_switchingSignalSMDM->pushButton_PRD_2->size());
                    win_switchingSignalSMDM->pushButton_PRD_3->setIcon(R1_Gray);
                    win_switchingSignalSMDM->pushButton_PRD_3->setIconSize(win_switchingSignalSMDM->pushButton_PRD_3->size());
                    win_switchingSignalSMDM->pushButton_PRD_4->setIcon(R1_Gray);
                    win_switchingSignalSMDM->pushButton_PRD_4->setIconSize(win_switchingSignalSMDM->pushButton_PRD_4->size());

                    ui->tabWidget->addTab(ProverkaList5,ui->view->item(j,0)->text());

                }

                ui->view->item(j,0)->setBackground(QBrush(QColor(158, 207, 207, 20)));
            }


        }
    }

    ui->view->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->view->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

}

void MainWindow::CreateStartSeitingGraph()
{

    ProverkaList1 = new QWidget(); // окно проверки
    ProverkaList2 = new QWidget(); // окно проверки
    ProverkaList3 = new QWidget(); // окно проверки
    ProverkaList4 = new QWidget(); // окно проверки
    ProverkaList5 = new QWidget(); // окно проверки


    // win_frequency->setupUi(ProverkaList3);
    //win_transferCoefficient->setupUi(ProverkaList4);

    win_frequency->ProverkaGraph->xAxis->setLabel("Частота(МГц)");
    win_frequency->ProverkaGraph->yAxis->setLabel("дБ");
    win_frequency->ProverkaGraph->xAxis2->setVisible(true);
    win_frequency->ProverkaGraph->xAxis2->setTickLabels(false);


    win_frequency->ProverkaGraph->xAxis->setRange(900,2200);
    win_frequency->ProverkaGraph->yAxis->setRange(-10,2);

    win_transferCoefficient->Graph->xAxis2->setVisible(true);
    win_transferCoefficient->Graph->xAxis2->setTickLabels(false);
    win_transferCoefficient->Graph->xAxis->setLabel("Установленные(дБ))");
    win_transferCoefficient->Graph->yAxis->setLabel("Измеренные(дБ)");


    win_transferCoefficient->Graph->xAxis->setRange(-14,14);
    win_transferCoefficient->Graph->yAxis->setRange(-14,14);


    win_frequency->ProverkaGraph->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    win_transferCoefficient->Graph->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);


    connect(win_frequency->tableWidgetAChH,SIGNAL(cellClicked(int,int)),this,SLOT(ClickCheckBoxGrafAChH(int,int)));
    connect(win_transferCoefficient->tableWidgetPerestrouka,SIGNAL(cellClicked(int,int)),this,SLOT(ClickCheckBoxGrafPerestrouka(int,int)));
}

