#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QScrollBar>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QRadioButton>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTableWidget>
#include <QDockWidget>
#include <QTextEdit>
#include <QStackedWidget>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QAbstractSocket>
#include <QtNetwork>
#include <QStackedWidget>
#include <QFileDialog>
//#include <QPrinter>

//Библиотека Базы данных
#include "bdata.h"

//Библиотеки устройств
#include "micran_gen.h"             //Класс Генератора Micran
#include "powersourse.h"            //Класс Источника питания ROSHESWARDS
#include "n9000a.h"                 //Класс Анализатора N9000
#include "tp_smdm.h"                //Класс Пульт ТП-СМДМ

#include <QScrollBar>

//Библиотеки окон првоерок
#include <moydialog.h>
#include <regylirovka.h>
#include <ui_power.h>
#include <ui_frequency.h>
#include <ui_transfercoefficient.h>
#include <ui_switchingsignalsm.h>
#include <ui_switchingsignalsmdm.h>
#include <ui_switchingsignaldm.h>



//Библиотеки провермяемых блоков
#include <thread_sm.h>
#include <thread_dm.h>
#include <thread_smdm.h>

//Библиотеки для добавления логов в БД
#include <log.h>

#include <QAxObject>
#include <QAxWidget>
#include <QAxBase>

#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //Устройства
    Micran_Gen*     Micran1;
    PowerSourse*    HMP2020;
    N9000A*         N9000;
    TP_SMDM*        TP;
    //БД
    BData*          BD;

    //Проверяемые блоки
    thread_SM *threadSM;
    thread_DM *threadDM;
    thread_SMDM *threadSMDM;


    //Переменные ToolBar
    QLabel *NameDevice_Lable; // текст перед названием устройства
    QLabel *NDevice_Lable; // текст перед номером устройства
    QLabel *EtapLabel; // текст перед выбором этапа изготовления на котором проводится текущая проверка

    QComboBox *NDevice_ComboBox; // поле для ввода номера устройства
    QComboBox *EtapGroup; // менюшка с названиями этапов - НУ, климаты, ...
    QComboBox *EtapGroup_Device; // менюшка с выбаром устройств из Базы Данных

    QGroupBox  *GroupBox_RadioButton; // Група радио кнопок для выбора ПСИ или ПИ

    QRadioButton *EtapGroup_PSI_RadioButton; // Радио кнопка для выбора ПСИ
    QRadioButton *EtapGroup_PI_RadioButton; // Радио кнопка для выбора ПИ
    QRadioButton *EtapGroup_Regylirovka_RadioButton; // Регулировка


    QHBoxLayout *vbox; // Лаяут для радио батанов

    QPushButton *Button_StartOpros; // Кнопка подключения устройств (Опрашивает устройства в сети)
    QPushButton *Button_EndOpros; // Кнопка отключения устройств (Отключает подключенные устройства от сети)
    QPushButton *Button_StartProverka; // Кнопка запуска автоматической проверки
    QPushButton *Button_Result; // Кнопка запуска Выбора просмотра результатов
    QPushButton *Button_Regylirovka; // Кнопка для регулировки пульта

    QPushButton *Button_PDF; // Кнопка генерации отчетов PDF

    MoyDialog *win_ipadres;
    Regylirovka *win_Regylirovka;
    QDialog *FindResult;
    QDialog *CheckResult;

    //Переменные для регулировки
    QVector<QVector<QTableWidgetItem*>>* tableRegyl;
    QVector<QVector<QTableWidgetItem*>>* tableRegyl2;

     QVector<QVector<double>>* ListRegyl;

    QWidget *ProverkaList1; // окно проверки
    QWidget *ProverkaList2 ; // окно проверки
    QWidget *ProverkaList3; // окно проверки
    QWidget *ProverkaList4; // окно проверки
    QWidget *ProverkaList5; // окно проверки

    QTimer *timer;
    QTimer *timerKolibrovka;
    QTimer *timer_perestroyka; //таймер для графика перестройки

    //Частота обновления таймера для графиков
    int fps;

    //Для графика АЧХ результатов
    QCPItemText *textLabel_Min;
    QCPItemLine *arrow_Min;
    QCPItemText *textLabel;
    QCPItemLine *arrow;
    QCPItemText *textLabel_Neravnomernost;

    //Клаа добавления логово
    Log* newLog;

    //Переменные для результатов
    QString PutBlok;
    QString PutSerial;
    QString PutEtap;
    QString PutProverka;
    QString PutData;
    QString PutIDLink;
    QString IdLink;

    //Переменные для результатов для отображения графиков и нахождения точек в БД
    QList<QSqlQuery>* list;
    QVector<QVector<double>> ListMin,ListMax,ListNEravn;
    QVector<QVector<double>> ListX;
    QVector<QVector<double>> ListY;

    QVector<QVector<double>> ListPerestrouka;
    QVector<QVector<double>> ListX1;
    QVector<QVector<double>> ListY1;
    QVector<QVector<double>> PerestroykaX;


    //Функции Инициализации
    void CreateLog();
    void CreateDevice();
    void CreateTollBar();
    void PutRegyl();
    void CreatePDF();

    void END();

    void slot_CreateWord();


    //ОТЧЕТ

    QStringList list_etaps;

    QStringList list_id_Proverki_Find;

    QStringList list_Proverki_Find;

    QStringList list_data;


signals:
    void work_threadSMDM();

    void startSM();
    void startDM();
    void startSMDM();

    void Connect_Micran(QString);
    void Connect_N9000(QString);
    void Connect_HMP2020(QString);
    void Connect_TP();

    void Connect_BD();
    void KolibrovkaSignal();


    void Setlog_wr_message(QString);
    void Setlog_wr_message_Ist(QString msg);   // Функция Лог Источника питания
    void Setlog_wr_message_Micran(QString msg);   // Функция Лог Микрана (генератора)
    void Setlog_wr_message_TP(QString msg);        // Функция Лог Пульта
    void Setlog_wr_message_Block(QString msg);    // Функция Лог Блока
    void Setlog_wr_message_N9000(QString msg);    // Функция Лог Анализатора



public slots:

   void addBD(QString data);

    void LoadResult();                      // Фукнция загрузки результатов
    void LoadTableResultat();               // Создание таблицы для результатов


    void ClickCheckBoxGrafAChH(int,int);   //Функция Нажатиня на выбор графика в Проверке АЧХ
    void ClickCheckBoxGrafPerestrouka(int,int); // Функция Нажатия на выбор графика в Проверке Перестройка

    void ClickCheckBoxGraf_winReuslt(int r,int c );   //Функция Нажатиня на выбор графика в Результатах АЧХ
    void ClickCheckBoxGraf_winReuslt2(int r,int c );   //Функция Нажатиня на выбор графика в Результатах Неравномерность

    int PoiskGraph(int r,int c);            //Функция поиска графика АЧХ
    int PoiskGraph_2(int r,int c);            //Функция поиска графика АЧХ

    void graphSM(int r,int c);
    void graphDM(int r,int c);
    void graphSMDM(int r,int c);


    int PoiskGraph2(int r,int c);            //Функция поиска графика Перестройке
    int PoiskGraph2_2(int r,int c);            //Функция поиска графика Перестройке
    void SetMinMaxGraphResult(double min,double max, double Neravnomernost,QVector<double> x,QVector<double> y);


    void CreateGraphFrenq();

    void CreateGraph();
    void CreateGraph2();
    void CreateGraph3();


    void UpdateGraph();                     //Функция отрисовки графика ачх
    void UpdateGraphPerestrouyka();          // Функция отрисовки графика перестройки

    //Функции Лога
    void log_wr_message(QString msg);       // Функция Лог приложения
    void log_wr_message_Ist(QString msg);   // Функция Лог Источника питания
    void log_wr_message_Micran(QString msg);   // Функция Лог Микрана (генератора)
    void log_wr_message_TP(QString msg);        // Функция Лог Пульта
    void log_wr_message_Block(QString msg);    // Функция Лог Блока
    void log_wr_message_N9000(QString msg);    // Функция Лог Анализатора

    void log_wr_message_claer();       // Функция Лог приложения
    void log_wr_message_Ist_claer();   // Функция Лог Источника питания
    void log_wr_message_Micran_claer();   // Функция Лог Микрана (генератора)
    void log_wr_message_TP_claer();        // Функция Лог Пульта
    void log_wr_message_Block_claer();    // Функция Лог Блока
    void log_wr_message_N9000_claer();    // Функция Лог Анализатора

    //Слоты(функции) Для установки Картинки какие каналы на Пульте ТП-СМДМ включены
    void SetImage(QByteArray);
    void SetImage2(QByteArray);
    void SetImage3(QByteArray);
    void SetImage4(QByteArray);

    void Proverka_StartOpros();
    void DisableDevice();                   // Функция Отключение устройств
    void Proverka_Start();                  // Функция Старта проверки устройств. Нажатие на кнопку Button_Start
    //Функция поиска серийного номера
    void DeviceSirealNumber(QString);       // Функция Нахождения серийного номера устройства в БД
    void Result();                          //Функция обновления окна для результатов
    void DisebleElements();
    void DisebleElementsRetult();
    void Kolibrovka();
    void Adresa_nastroika();

    //Функции регулировки
    void look_Regyl();
    void LoadFile();
    void UpdateGraphRegylirovka();

    void SetMinMaxGraph(double ,double ,double , QVector<double> ,QVector<double> );

    //GroupBox ПСИ функции выбора
    void GroupBox_RadioButtonClick();       // Функция обработки нажатия выбора ПСИ или ПИ

    QVector<QSqlQuery> CreateVectorResultPDF();
    QVector<QSqlQuery> CreateVectorResultPDF_power();
    QVector<QSqlQuery> CreateVectorResultPDF_Regylirovka();
    QVector<QSqlQuery> CreateVectorResultPDF_Regylirovka_power();
    QVector<double> CreateVectorResultPDF_Regylirovka_AchH();
    QVector<double> CreateVectorResultPDF_PSI_AchH();


    QList<QStringList> sql_listEtap(QString,QString);




    void LoadingStartGraphResult();
    void AddGraphResult();

    //Функции для отображения результатов
    QSqlQuery Find(int point);
    QSqlQuery Find2(int point);


    void PDF();
    void LoadTable();
    void CreateStartSeitingGraph();

    void MyAddGraph(Ui::Frequency*, Ui::TransferCoefficient*);

    void PutBlock();                        // Функция выбора блока;
    void PutSer();                          // Функция выбора блока;
    void putEtap();                         // Функция выбор этапа;
    void putProverka();                     // Функция выбор проверки результатов;
    void GoBack();                          // Функция возврата к проверкам
    void GoBack2();
    void GoNext();                          // Функция просмотра выбранного результата

     void SetTabProverki(int r,int c);

     void errorMessage(QString data);
private:
    Ui::MainWindow *ui;
    Ui::Power* win_power;   // в форме electricalpower имя electricalpower, поэтому и тип описан как electricalpower. Но обязательно нужен #include "ui_electricalpower.h"
    Ui::TransferCoefficient* win_transferCoefficient;   // в форме electricalpower имя electricalpower, поэтому и тип описан как electricalpower. Но обязательно нужен #include "ui_electricalpower.h"
    Ui::SwitchingSignalSM* win_switchingSignalSM;
    Ui::SwitchingSignalSMDM* win_switchingSignalSMDM;
    Ui::SwitchingSignalDM* win_switchingSignalDM;
    Ui::Frequency* win_frequency;   // в форме electricalpower имя electricalpower, поэтому и тип описан как electricalpower. Но обязательно нужен #include "ui_electricalpower.h"


    Ui::Power* win_power_2;   // в форме electricalpower имя electricalpower, поэтому и тип описан как electricalpower. Но обязательно нужен #include "ui_electricalpower.h"
    Ui::Frequency* win_frequency_2;   // в форме electricalpower имя electricalpower, поэтому и тип описан как electricalpower. Но обязательно нужен #include "ui_electricalpower.h"
    Ui::TransferCoefficient* win_transferCoefficient_2;   // в форме electricalpower имя electricalpower, поэтому и тип описан как electricalpower. Но обязательно нужен #include "ui_electricalpower.h"
    Ui::SwitchingSignalSM* win_switchingSignalSM_2;
    Ui::SwitchingSignalSMDM* win_switchingSignalSMDM_2;
    Ui::SwitchingSignalDM* win_switchingSignalDM_2;

    //Переменные с текстом логов
    QTextEdit* log_text;
    QTextEdit* log_text_Micran;
    QTextEdit* log_text_TP;
    QTextEdit* log_text_Block;
    QTextEdit* log_text_N9000;
    QTextEdit* log_text_Ist;
};

#endif // MAINWINDOW_H
