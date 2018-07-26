#include "regylirovka.h"
#include "ui_regylirovka.h"

Regylirovka::Regylirovka(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Regylirovka)
{
    ui->setupUi(this);
}

Regylirovka::~Regylirovka()
{
    delete ui;
}

void Regylirovka::Clear_textEdit_AB()
{
    ui->textEdit_AB->clear();
}

void Regylirovka::label_DataSetText(QString str)
{
    ui->label_Data_2->setText(str);
}

void Regylirovka::insert_textEdit_AB(QString data)
{
     ui->textEdit_AB->insertPlainText(data);
}

QTextEdit* Regylirovka::getQTextEditObject()
{
    return ui->textEdit_AB;
}

QPushButton* Regylirovka::getQPushButtonObject()
{
     return ui->pushButton_StartRegylirovka;
}

void Regylirovka::PutRegyl()
{
    int lineNumber=0;
    QTextEdit::ExtraSelection selection;
    QTextCursor cursor = QTextCursor(ui->textEdit_AB->document());
    lineNumber = ui->textEdit_AB->textCursor().blockNumber();
    cursor.movePosition(QTextCursor::Start);
    cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor,lineNumber);
    cursor.select(QTextCursor::LineUnderCursor);
    QTextCharFormat format;
    format.setForeground(Qt::white);
    format.setBackground(Qt::blue);
    selection.cursor = cursor;
    selection.format = format;
    ui->textEdit_AB->setExtraSelections(QList<QTextEdit::ExtraSelection>() << selection);

    QString PutKanal = ui->textEdit_AB->textCursor().block().text(); //Выбранный текст.


    ui->Table_AB->clear();

    if(PutKanal!="")
    {

        int d =0;

        if(PutKanal.split('A').first() == "")
        {
            d = PutKanal.split('A').last().toInt();
        }

        if(PutKanal.split('B').first() == "")
        {
            d = PutKanal.split('B').last().toInt()+18;
        }

        if(PutKanal.split('K').first() == "")
        {
            d = PutKanal.split('K').last().toInt();
        }

        ui->Table_AB->setColumnCount(2);
        ui->Table_AB->setRowCount(25);



        for(int i=0;i < 25;i++)
        {

            ui->Table_AB->setItem(i,0,tableRegyl->value(d).value(i)->clone());
            ui->Table_AB->setItem(i,1,tableRegyl2->value(d).value(i)->clone());
        }

    }
}

void Regylirovka::on_pushButton_StartRegylirovka_clicked()
{

}
