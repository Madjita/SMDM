#ifndef MOYDIALOG_H
#define MOYDIALOG_H

#include <QDialog>

namespace Ui {
class MoyDialog;
}

class MoyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MoyDialog(QWidget *parent = 0);
    ~MoyDialog();


public slots:
    QString getIP_N9000();
    QString getIP_HMP2020();
    QString getIP_Micran();

private:
    Ui::MoyDialog *ui;
};

#endif // MOYDIALOG_H
