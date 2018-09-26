#include <mainwindow.h>
#include "ui_mainwindow.h"



//Функция отрисовки графика ачх
void MainWindow::UpdateGraph()
{
   // qDebug () << "UpdateGraph () " << win_frequency->ProverkaGraph->graphCount();

    if(threadSM != NULL )
    {
        if(threadSM->flagEnd == true)
        {

            if(threadSM->x.count() > 0 && threadSM->y.count() > 0 && threadSM->flag == true)
            {
                if(threadSM->x.count() == 1)
                {
                    win_frequency->ProverkaGraph->xAxis->setRange(900,2200);
                    win_frequency->ProverkaGraph->yAxis->setRange(-10,2);
                }


                if( win_frequency->ProverkaGraph->graphCount() <= 0)
                {
                    for(int i =0; i < 65;i++)
                    {
                        //ДОбавил проверить
                        win_frequency->ProverkaGraph->addGraph();
                        win_frequency->ProverkaGraph->graph(win_frequency->ProverkaGraph->graphCount()-1)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
                        win_frequency->ProverkaGraph->graph(win_frequency->ProverkaGraph->graphCount()-1)->setPen(QPen(qrand())); // line color blue for first graph
                        win_frequency->ProverkaGraph->graph(win_frequency->ProverkaGraph->graphCount()-1)->setBrush(QBrush(QColor(0, 0, 255, 20))); // first graph will be filled with translucent blue
                    }

                }

                qDebug () << "win_frequency->ProverkaGraph->graphCount() =" << win_frequency->ProverkaGraph->graphCount();

                win_frequency->ProverkaGraph->graph(0)->setData(threadSM->x,threadSM->y);
                win_frequency->ProverkaGraph->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));

                win_frequency->ProverkaGraph->replot();


                SetMinMaxGraph(threadSM->min,threadSM->max,threadSM->NeravnACHX,threadSM->x,threadSM->y);

                win_frequency->tableWidgetAChH->reset();

            }

            if(threadSM->y1.count()>0 && threadSM->Start5Proverka==true && timer_perestroyka->isActive() == false)
            {
                timer_perestroyka->start(fps);
            }

            threadSM->sem.release();
        }
    }


    //qDebug () << "UpdateGraph () ";

    if(threadDM->flagEnd == true)
    {

        if(threadDM->x.count() > 0 && threadDM->y.count() > 0 && threadDM->flag == true)
        {
            if(threadDM->x.count() == 1)
            {
                win_frequency->ProverkaGraph->xAxis->setRange(900,2200);
                win_frequency->ProverkaGraph->yAxis->setRange(-10,2);
            }

            if( win_frequency->ProverkaGraph->graphCount() <= 0)
            {
                win_frequency->ProverkaGraph->addGraph();
            }

            win_frequency->ProverkaGraph->graph(0)->setData(threadDM->x,threadDM->y);
            win_frequency->ProverkaGraph->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));

            win_frequency->ProverkaGraph->replot();


            SetMinMaxGraph(threadDM->min,threadDM->max,threadDM->NeravnACHX,threadDM->x,threadDM->y);

            win_frequency->tableWidgetAChH->reset();

        }

        if(threadDM->y1.count()>0 && threadDM->Start5Proverka==true && timer_perestroyka->isActive() == false)
        {
            timer_perestroyka->start(fps);
        }

        threadDM->sem.release();

    }


    if(threadSMDM->flagEnd == true)
    {

        if(threadSMDM->x.count() > 0 && threadSMDM->y.count() > 0 && threadSMDM->flag == true)
        {
            if(threadSMDM->x.count() == 1)
            {
                win_frequency->ProverkaGraph->xAxis->setRange(900,2200);
                win_frequency->ProverkaGraph->yAxis->setRange(-10,2);
            }

            if( win_frequency->ProverkaGraph->graphCount() <= 0)
            {
                win_frequency->ProverkaGraph->addGraph();
            }

            win_frequency->ProverkaGraph->graph(0)->setData(threadSMDM->x,threadSMDM->y);
            win_frequency->ProverkaGraph->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));

            win_frequency->ProverkaGraph->replot();


            SetMinMaxGraph(threadSMDM->min,threadSMDM->max,threadSMDM->NeravnACHX,threadSMDM->x,threadSMDM->y);

            win_frequency->tableWidgetAChH->reset();

        }

        if(threadSMDM->y1.count()>0 && threadSMDM->Start5Proverka==true && timer_perestroyka->isActive() == false)
        {
            timer_perestroyka->start(fps);
        }

        threadSMDM->sem.release();

    }




    if(threadSM->flagEnd == false && threadDM->flagEnd == false && threadSMDM->flagEnd == false)
    {

        qDebug() << "THE END ";
        timer->stop();
        timer_perestroyka->stop();

        if(EtapGroup_PSI_RadioButton->isChecked() == true)
        {
            log_wr_message("Проверка для блока : "+ EtapGroup_Device->currentText()+" на этапе: ПСИ ->"+EtapGroup->currentText()+" заверешена!\n");
        }
        if(EtapGroup_PI_RadioButton->isChecked() == true)
        {
            log_wr_message("Проверка для блока : "+ EtapGroup_Device->currentText()+" на этапе: ПИ ->"+EtapGroup->currentText()+" заверешена!\n");
        }

        if(EtapGroup_Regylirovka_RadioButton->isChecked() == true)
        {
            log_wr_message("Проверка для блока : "+ EtapGroup_Device->currentText()+" на этапе: Регулировка ->"+EtapGroup->currentText()+" заверешена!\n");
        }



        /*
        if(!potok1->wait(0))
        {
             qDebug () << "dsjfhjdaishfjuda !potok1->wait(0) sfpasdufdasofiudoasf";
            potok1->quit();
            potok1->wait();
        }
        if(!potok2->wait(0))
        {
            potok2->quit();
            potok2->wait();

        }
        if(!potok3->wait(0))
        {
            potok3->quit();
            potok3->wait();

        }
        if(!potok4->wait(0))
        {
            potok4->quit();
            potok4->wait();
        }
        */

        //Вернуть когда 4 проверку запустим
        win_frequency->ProverkaGraph->graph(0)->clearData();
        win_frequency->ProverkaGraph->clearItems();
        win_frequency->ProverkaGraph->replot();

        qDebug() << "THE END 2";

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


        // this->thread()->wait();

    }

}


void MainWindow::UpdateGraphPerestrouyka()
{
    qDebug () << "UpdateGraphPerestrouyka ";

    if(threadSM->Start5Proverka == true)
    {
        qDebug () << " threadSM UpdateGraphPerestrouyka ";

        if(threadSM->PerestroykaX.count() > 0 && threadSM->y1.count() > 0)
        {
            if(threadSM->y1.count() == 1)
            {
                //win_transferCoefficient->Graph->xAxis->setRange(-14,14);
                //win_transferCoefficient->Graph->yAxis->setRange(-14,14);
                win_transferCoefficient->Graph->xAxis->setRange(-12,12);
                win_transferCoefficient->Graph->yAxis->setRange(-30,14);
            }


            if( win_transferCoefficient->Graph->graphCount() <= 0)
            {
                win_transferCoefficient->Graph->addGraph();
            }

            qDebug () << " threadSM win_transferCoefficient->Graph =" << win_transferCoefficient->Graph->graphCount();


            win_transferCoefficient->Graph->graph(1)->setData(threadSM->PerestroykaX,threadSM->y1); //Наоборот x и y

            // qDebug() << "threadSM threadSM->PerestroykaX = "<< threadSM->PerestroykaX.count();

            win_transferCoefficient->Graph->replot();

            win_transferCoefficient->tableWidgetPerestrouka->reset();

            threadSM->sem3.release();
        }
    }

    if(threadDM->Start5Proverka == true)
    {
        qDebug () << " threadDM UpdateGraphPerestrouyka ";

        if(threadDM->PerestroykaX.count() > 0 && threadDM->y1.count() > 0)
        {
            if(threadDM->y1.count() == 1)
            {
//                win_transferCoefficient->Graph->xAxis->setRange(-14,14);
//                win_transferCoefficient->Graph->yAxis->setRange(-14,14);
                win_transferCoefficient->Graph->xAxis->setRange(-12,12);
                win_transferCoefficient->Graph->yAxis->setRange(-30,14);
            }

            if( win_transferCoefficient->Graph->graphCount() <= 0)
            {
                win_transferCoefficient->Graph->addGraph();
            }

            qDebug () << " threadDM win_transferCoefficient->Graph =" << win_transferCoefficient->Graph->graphCount();

            win_transferCoefficient->Graph->graph(1)->setData(threadDM->PerestroykaX,threadDM->y1); //Наоборот x и y
            win_transferCoefficient->Graph->replot();

            win_transferCoefficient->tableWidgetPerestrouka->reset();

            threadDM->sem3.release();
        }
    }

    if(threadSMDM->Start5Proverka == true)
    {
        if(threadSMDM->PerestroykaX.count() > 0 && threadSMDM->y1.count() > 0)
        {
            if(threadSMDM->y1.count() == 1)
            {
//                win_transferCoefficient->Graph->xAxis->setRange(-14,14);
//                win_transferCoefficient->Graph->yAxis->setRange(-14,14);
                win_transferCoefficient->Graph->xAxis->setRange(-12,12);
                win_transferCoefficient->Graph->yAxis->setRange(-30,14);
            }

            if( win_transferCoefficient->Graph->graphCount() <= 0)
            {
                win_transferCoefficient->Graph->addGraph();
            }


            win_transferCoefficient->Graph->graph(1)->setData(threadSMDM->PerestroykaX,threadSMDM->y1); //Наоборот x и y
            win_transferCoefficient->Graph->replot();

            win_transferCoefficient->tableWidgetPerestrouka->reset();

            threadSMDM->sem3.release();
        }
    }
}

int PoiskX(double y,QVector<double> list_y,QVector<double> list_x)
{
    for(int i=0; i < list_y.count();i++)
    {
        if(y == list_y[i])
        {
            return list_x[i];
        }
    }
}


//q - Это либо Min либо Max , а flag_max -флаг для максимума
void MainWindow::SetMinMaxGraph(double min,double max,double Neravnomernost, QVector<double> x,QVector<double> y)
{
    //Min
    win_frequency->ProverkaGraph->clearItems();
    textLabel_Min = new QCPItemText(win_frequency->ProverkaGraph);
    win_frequency->ProverkaGraph->addItem(textLabel_Min);
    arrow_Min = new QCPItemLine(win_frequency->ProverkaGraph);
    win_frequency->ProverkaGraph->addItem(arrow_Min);
    textLabel_Min->setPositionAlignment(Qt::AlignTop|Qt::AlignCenter);
    textLabel_Min->position->setCoords(1800,1);
    arrow_Min->start->setParentAnchor(textLabel_Min->bottom);
    textLabel_Min->position->setType(QCPItemPosition::ptAxisRectRatio);
    textLabel_Min->setText("Min: "+QString::number(min)+" дБ");
    textLabel_Min->setFont(QFont(font().family(),10));
    textLabel_Min->setPen(QPen(Qt::black));
    arrow_Min->end->setCoords(PoiskX(min,y,x),min);
    arrow_Min->setHead(QCPLineEnding::esSpikeArrow);

    //Мах
    textLabel = new QCPItemText(win_frequency->ProverkaGraph);
    win_frequency->ProverkaGraph->addItem(textLabel);
    arrow = new QCPItemLine(win_frequency->ProverkaGraph);
    win_frequency->ProverkaGraph->addItem(arrow);
    textLabel->setPositionAlignment(Qt::AlignTop|Qt::AlignCenter);
    textLabel->position->setCoords(1300,1);
    arrow->start->setParentAnchor(textLabel->bottom);
    textLabel->position->setType(QCPItemPosition::ptAxisRectRatio);
    textLabel->setText("Max: "+QString::number(max)+" дБ");
    textLabel->setFont(QFont(font().family(),10));
    textLabel->setPen(QPen(Qt::black));
    arrow->end->setCoords(PoiskX(max,y,x),max);
    arrow->setHead(QCPLineEnding::esSpikeArrow);

    //Неравномерность
    textLabel_Neravnomernost = new QCPItemText(win_frequency->ProverkaGraph);
    win_frequency->ProverkaGraph->addItem(textLabel_Neravnomernost);
    textLabel_Neravnomernost->setPositionAlignment(Qt::AlignTop|Qt::AlignCenter);
    textLabel_Neravnomernost->position->setCoords(1500,1.5);
    textLabel_Neravnomernost->position->setType(QCPItemPosition::ptAxisRectRatio);
    textLabel_Neravnomernost->setText("Неравномерность: "+QString::number(Neravnomernost)+" дБ");


    textLabel_Neravnomernost->setFont(QFont(font().family(),10));
    textLabel_Neravnomernost->setPen(QPen(Qt::black));

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Функции для поиска графиков в только что сделанной проверке

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void MainWindow::graphSM(int r,int c)
{
    int point =0;

    if( win_frequency->tableWidgetAChH->item(r,c)->flags() != Qt::NoItemFlags)
    {

        // if(win_frequency->ProverkaGraph->graphCount() >= point)
        // {

        if(win_frequency->tableWidgetAChH->item(r,c)->checkState()==Qt::Checked)
        {

            point =0;
            for(int i=0;i < win_frequency->tableWidgetAChH->columnCount();i++)
            {
                for(int j=0;j < win_frequency->tableWidgetAChH->rowCount();j++)
                {


                    win_frequency->ProverkaGraph->graph(point)->clearData(); // в этой строчке ошибка так как графиков 0


                    if(point <64)
                    {
                        if(i == c && j == r)
                        {

                            win_frequency->tableWidgetAChH->item(j,i)->setCheckState(Qt::Checked);
                        }
                        else
                        {
                            win_frequency->tableWidgetAChH->item(j,i)->setCheckState(Qt::Unchecked);
                        }
                        point++;
                    }
                }
            }

            win_frequency->tableWidgetAChH->reset();
            point =  PoiskGraph(r,c);

            win_frequency->ProverkaGraph->xAxis->setRange(threadSM->ListX[point-1].first()-50,threadSM->ListX[point-1].last()+50);
            win_frequency->ProverkaGraph->yAxis->setRange(-10,2);


            win_frequency->ProverkaGraph->graph(point)->setData(threadSM->ListX[point-1],threadSM->ListY[point-1]);

            win_frequency->ProverkaGraph->clearItems();

            SetMinMaxGraph(threadSM->ListMin2[point-1].last(),threadSM->ListMax2[point-1].last(),threadSM->ListNEravn2[point-1].last(),threadSM->ListX[point-1],threadSM->ListY[point-1]);
            win_frequency->ProverkaGraph->replot();

            //  }

        }

        if(win_frequency->tableWidgetAChH->item(r,c)->checkState()==Qt::Unchecked)
        {
            win_frequency->ProverkaGraph->clearItems();

            for(int i=0;i <  win_frequency->ProverkaGraph->graphCount();i++)
            {
                win_frequency->ProverkaGraph->graph(i)->clearData();
            }

        }
        win_frequency->ProverkaGraph->replot();
    }
}

void MainWindow::graphDM(int r,int c)
{
    int point =0;

    if( win_frequency->tableWidgetAChH->item(r,c)->flags() != Qt::NoItemFlags)
    {

        if(win_frequency->tableWidgetAChH->item(r,c)->checkState()==Qt::Checked)
        {

            point =0;
            for(int i=0;i < win_frequency->tableWidgetAChH->columnCount();i++)
            {
                for(int j=0;j < win_frequency->tableWidgetAChH->rowCount();j++)
                {
                    win_frequency->ProverkaGraph->graph(point)->clearData();
                    if(point <64)
                    {
                        if(i == c && j == r)
                        {
                            win_frequency->tableWidgetAChH->item(j,i)->setCheckState(Qt::Checked);
                        }
                        else
                        {
                            win_frequency->tableWidgetAChH->item(j,i)->setCheckState(Qt::Unchecked);
                        }
                        point++;
                    }
                }
            }
            win_frequency->tableWidgetAChH->reset();
            point =  PoiskGraph(r,c);

            win_frequency->ProverkaGraph->xAxis->setRange(threadDM->ListX[point-1].first()-50,threadDM->ListX[point-1].last()+50);
            win_frequency->ProverkaGraph->yAxis->setRange(-10,2);


            win_frequency->ProverkaGraph->graph(point)->setData(threadDM->ListX[point-1],threadDM->ListY[point-1]);

            win_frequency->ProverkaGraph->clearItems();

            SetMinMaxGraph(threadDM->ListMin2[point-1].last(),threadDM->ListMax2[point-1].last(),threadDM->ListNEravn2[point-1].last(),threadDM->ListX[point-1],threadDM->ListY[point-1]);
            win_frequency->ProverkaGraph->replot();

        }

        if(win_frequency->tableWidgetAChH->item(r,c)->checkState()==Qt::Unchecked)
        {
            win_frequency->ProverkaGraph->clearItems();

            for(int i=0;i <  win_frequency->ProverkaGraph->graphCount();i++)
            {
                win_frequency->ProverkaGraph->graph(i)->clearData();
            }

        }
        win_frequency->ProverkaGraph->replot();
    }
}

void MainWindow::graphSMDM(int r,int c)
{
    int point =0;

    if( win_frequency->tableWidgetAChH->item(r,c)->flags() != Qt::NoItemFlags)
    {

        if(win_frequency->tableWidgetAChH->item(r,c)->checkState()==Qt::Checked)
        {

            point =0;
            for(int i=0;i < win_frequency->tableWidgetAChH->columnCount();i++)
            {
                for(int j=0;j < win_frequency->tableWidgetAChH->rowCount();j++)
                {
                    win_frequency->ProverkaGraph->graph(point)->clearData();

                    if(point < 64)
                    {
                        if(i == c && j == r)
                        {
                            win_frequency->tableWidgetAChH->item(j,i)->setCheckState(Qt::Checked);
                        }
                        else
                        {
                            win_frequency->tableWidgetAChH->item(j,i)->setCheckState(Qt::Unchecked);
                        }
                        point++;
                    }
                }
            }
            win_frequency->tableWidgetAChH->reset();
            point =  PoiskGraph(r,c);

            win_frequency->ProverkaGraph->xAxis->setRange(threadSMDM->ListX[point-1].first()-50,threadSMDM->ListX[point-1].last()+50);
            win_frequency->ProverkaGraph->yAxis->setRange(-10,2);


            win_frequency->ProverkaGraph->graph(point)->setData(threadSMDM->ListX[point-1],threadSMDM->ListY[point-1]);

            win_frequency->ProverkaGraph->clearItems();

            SetMinMaxGraph(threadSMDM->ListMin2[point-1].last(),threadSMDM->ListMax2[point-1].last(),threadSMDM->ListNEravn2[point-1].last(),threadSMDM->ListX[point-1],threadSMDM->ListY[point-1]);
            win_frequency->ProverkaGraph->replot();

        }

        if(win_frequency->tableWidgetAChH->item(r,c)->checkState()==Qt::Unchecked)
        {
            win_frequency->ProverkaGraph->clearItems();

            for(int i=0;i <  win_frequency->ProverkaGraph->graphCount();i++)
            {
                win_frequency->ProverkaGraph->graph(i)->clearData();
            }

        }
        win_frequency->ProverkaGraph->replot();
    }
}


//Функция обработки нажатия на чекбокс  в таблице результатов для отображения графика
void MainWindow::ClickCheckBoxGrafAChH(int r,int c)
{

    if( win_frequency->tableWidgetAChH->item(r,c)->flags() != Qt::NoItemFlags)
    {
        if(EtapGroup_Device->currentText() == "СМ16-4")
        {
            graphSM(r,c);
        }
        if(EtapGroup_Device->currentText() == "ДМ4-16")
        {
            graphDM(r,c);
        }
        if(EtapGroup_Device->currentText() == "СМДМ8-4")
        {
            graphSMDM(r,c);
        }
    }

}



//Функция обработки нажатия на чекбокс  в таблице результатов для отображения графика
void MainWindow::ClickCheckBoxGrafPerestrouka(int r,int c)
{
    int point =0;


    if( win_transferCoefficient->tableWidgetPerestrouka->item(r,c)->flags() != Qt::NoItemFlags)
    {

        if(win_transferCoefficient->tableWidgetPerestrouka->item(r,c)->checkState()==Qt::Checked)
        {

            point =0;
            for(int i=0;i < win_transferCoefficient->tableWidgetPerestrouka->columnCount();i++)
            {
                for(int j=0;j < win_transferCoefficient->tableWidgetPerestrouka->rowCount();j++)
                {
                    win_transferCoefficient->Graph->graph(point)->clearData();
                    if(i == c && j == r)
                    {
                        win_transferCoefficient->tableWidgetPerestrouka->item(j,i)->setCheckState(Qt::Checked);
                    }
                    else
                    {
                        win_transferCoefficient->tableWidgetPerestrouka->item(j,i)->setCheckState(Qt::Unchecked);
                    }
                    point++;
                }
            }

            win_transferCoefficient->tableWidgetPerestrouka->reset();
            point =  PoiskGraph2(r,c);


            win_transferCoefficient->Graph->xAxis->setRange(-14,14);
            win_transferCoefficient->Graph->yAxis->setRange(-14,14);

            if(EtapGroup_Device->currentText() == "СМ16-4")
            {
                win_transferCoefficient->Graph->graph(point)->setData(threadSM->Listy1[point],threadSM->ListPerestrouka[point]);
            }
            if(EtapGroup_Device->currentText() == "ДМ4-16")
            {
                win_transferCoefficient->Graph->graph(point)->setData(threadDM->Listy1[point],threadDM->ListPerestrouka[point]);
            }
            if(EtapGroup_Device->currentText() == "СМДМ8-4")
            {
                win_transferCoefficient->Graph->graph(point)->setData(threadSMDM->Listy1[point],threadSMDM->ListPerestrouka[point]);
            }

            win_transferCoefficient->Graph->replot();

        }

        if(win_transferCoefficient->tableWidgetPerestrouka->item(r,c)->checkState()==Qt::Unchecked)
        {

            for(int i=0;i <  win_transferCoefficient->Graph->graphCount();i++)
            {
                win_transferCoefficient->Graph->graph(i)->clearData();
            }
        }
        win_transferCoefficient->Graph->replot();
    }
}


//Функция поиска графика по выбранному чекбоксу Перестройка
int MainWindow::PoiskGraph2(int r,int c)
{
    int point =0;

    for(int i=0;i<win_transferCoefficient->tableWidgetPerestrouka->columnCount();i++)
    {

        for(int j=0;j<win_transferCoefficient->tableWidgetPerestrouka->rowCount();j++)
        {
            if(j==r && i==c)
            {
                return point;
                break;
            }
            point++;
        }
    }
}





void MainWindow::CreateGraph()
{
    if(win_transferCoefficient->Graph->itemCount() > 0 )
    {
        win_transferCoefficient->Graph->clearGraphs();
    }


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

    threadDM->sem3.release();
}

void MainWindow::CreateGraph2()
{
    if(win_transferCoefficient->Graph->itemCount() > 0 )
    {
        win_transferCoefficient->Graph->clearGraphs();
    }


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

    threadSM->sem3.release();
}
void MainWindow::CreateGraph3()
{
    if(win_transferCoefficient->Graph->itemCount() > 0 )
    {
        win_transferCoefficient->Graph->clearGraphs();
    }

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


    threadSMDM->sem3.release();
}

