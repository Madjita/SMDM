#ifndef REGYLIROVKA_H
#define REGYLIROVKA_H

#include <QDialog>
#include <QTextEdit>
#include <QTableWidgetItem>
#include <QTextBlock>

namespace Ui {
class Regylirovka;
}

class Regylirovka : public QDialog
{
    Q_OBJECT

public:
    explicit Regylirovka(QWidget *parent = 0);
    ~Regylirovka();

    QVector<QVector<QTableWidgetItem*>>*    tableRegyl = new QVector<QVector<QTableWidgetItem*>>;
    QVector<QVector<QTableWidgetItem*>>*    tableRegyl2 = new QVector<QVector<QTableWidgetItem*>>;


public slots:
    void Clear_textEdit_AB();
    void label_DataSetText(QString str);
    void insert_textEdit_AB(QString data);
    QTextEdit* getQTextEditObject();
    QPushButton* getQPushButtonObject();

    void PutRegyl();



private slots:
    void on_pushButton_StartRegylirovka_clicked();

private:
    Ui::Regylirovka *ui;
};

#endif // REGYLIROVKA_H
