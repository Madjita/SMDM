#include <mainwindow.h>
#include "ui_mainwindow.h"
#include <windows.h>

//Функция отрисовки графика ачх
void MainWindow::UpdateGraphRegylirovka()
{

    if(TP != NULL )
    {
        if(TP->flagEnd == true )
        {

            if(TP->x.count() > 0 && TP->y.count() > 0 && TP->flag == true)
            {
                if(TP->x.count() == 1)
                {
                    win_frequency->ProverkaGraph->xAxis->setRange(900,2200);
                    win_frequency->ProverkaGraph->yAxis->setRange(-10,2);
                }


                win_frequency->ProverkaGraph->graph(0)->setData(TP->x,TP->y);
                win_frequency->ProverkaGraph->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));

                win_frequency->ProverkaGraph->replot();


                SetMinMaxGraph(TP->min,TP->max,TP->NeravnACHX,TP->x,TP->y);

                win_frequency->tableWidgetAChH->reset();

            }

            //            if(TP->y1.count()>0 && TP->Start5Proverka==true && timer_perestroyka->isActive() == false)
            //            {
            //                timer_perestroyka->start(fps);
            //            }

            if(TP->flagRegylK0)
            {
                QMessageBox* pmbx2 = new QMessageBox("MessageBox","<b>Подлючите<b> <i>кабель К0 от генератора до анализатора.<i>",
                                                     QMessageBox::Information,
                                                     QMessageBox::Yes,
                                                     QMessageBox::No,
                                                     QMessageBox::Cancel | QMessageBox::Escape);
                pmbx2->show();

                int n2 = pmbx2->exec();

                if(n2 == QMessageBox::Yes)
                {
                    TP->sem2.release();
                    TP->m_mutex.unlock();
                    TP->flagRegylK0= false;
                    delete pmbx2;
                }
                else
                {
                    delete pmbx2;
                }
            }

            if(TP->flagRegyl_A)
            {
                QMessageBox* pmbx = new QMessageBox("MessageBox","<b>Подлючите<b> <i>кабель к выходу <i> <u>A: № "+QString::number(TP->releA)+"</u>",
                                                    QMessageBox::Information,
                                                    QMessageBox::Yes,
                                                    QMessageBox::No,
                                                    QMessageBox::Cancel | QMessageBox::Escape);
                pmbx->show();

                int n = pmbx->exec();

                if(n == QMessageBox::Yes)
                {
                    TP->sem.release();
                    TP->m_mutex.unlock();
                    TP->flagRegyl_A= false;
                    delete pmbx;
                }
                else
                {
                    delete pmbx;
                }
            }

            if(TP->flagRegyl_B)
            {
                QMessageBox* pmbx = new QMessageBox("MessageBox","<b>Подлючите<b> <i>кабель к выходу <i> <u>B: № "+QString::number(TP->releA)+"</u>",
                                                    QMessageBox::Information,
                                                    QMessageBox::Yes,
                                                    QMessageBox::No,
                                                    QMessageBox::Cancel | QMessageBox::Escape);
                pmbx->show();

                int n = pmbx->exec();

                if(n == QMessageBox::Yes)
                {
                    TP->sem.release();
                    TP->m_mutex.unlock();
                    TP->flagRegyl_B= false;
                    delete pmbx;
                }
                else
                {
                    delete pmbx;
                }
            }

        }
    }

    if(TP->flagEnd == false)
    {

        log_wr_message("Калибровка для пульта ТП_СМДМ на этапе заверешена!\n");

        timerKolibrovka->stop();




        ui->view->setDisabled(0);
        Button_StartProverka->setDisabled(0);
        GroupBox_RadioButton->setDisabled(0);
        EtapGroup_Device->setDisabled(0);
        EtapGroup->setDisabled(0);
        NDevice_ComboBox->setDisabled(0);
        Button_EndOpros->setDisabled(0);
        win_frequency->tableWidgetAChH->setDisabled(0);
        win_frequency->tableWidgetAChH->setDisabled(0);
        win_transferCoefficient->tableWidgetPerestrouka->setDisabled(0);


    }

}
