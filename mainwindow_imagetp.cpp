#include <mainwindow.h>
#include "ui_mainwindow.h"
#include <windows.h>

/*Image*/
////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::SetImage(QByteArray a) //установка начального цвета лампочек в серый в ГРУПБОКСЕ к1
{

    QPixmap R1_Gray(":/images/circle_gray.png");
    QPixmap R2_Green(":/images/circle_zel.png");
    QPixmap R3_Red(":/images/circle_kras.png");

    auto ok = char(0x01);

    if(a[0]== ok)
    {
        ui->pbK11->setIcon(R2_Green);
       // ui->pbK11->setIconSize(ui->pbK11->size()/2);
    }
    else
    {
        ui->pbK11->setIcon(R3_Red);
       // ui->pbK11->setIconSize(ui->pbK11->size()/2);
    }

    if(a[1]==ok)
    {
        ui->pbK12->setIcon(R2_Green);
       // ui->pbK12->setIconSize(ui->pbK12->size()/2);
    }
    else
    {
        ui->pbK12->setIcon(R3_Red);
       // ui->pbK12->setIconSize(ui->pbK12->size()/2);
    }

    if(a[2]==ok)
    {
        ui->pbK13->setIcon(R2_Green);
       // ui->pbK13->setIconSize(ui->pbK13->size()/2);
    }
    else
    {
        ui->pbK13->setIcon(R3_Red);
       // ui->pbK13->setIconSize(ui->pbK13->size()/2);
    }

    if(a[3]==ok)
    {
        ui->pbK14->setIcon(R2_Green);
      //  ui->pbK14->setIconSize(ui->pbK14->size()/2);
    }
    else
    {
        ui->pbK14->setIcon(R3_Red);
       // ui->pbK14->setIconSize(ui->pbK14->size()/2);
    }

    if(a[4]==ok)
    {
        ui->pbK15->setIcon(R2_Green);
       // ui->pbK15->setIconSize(ui->pbK15->size()/2);
    }
    else
    {
        ui->pbK15->setIcon(R3_Red);
      //  ui->pbK15->setIconSize(ui->pbK15->size()/2);
    }

    if(a[5]==ok)
    {
        ui->pbK16->setIcon(R2_Green);
       // ui->pbK16->setIconSize(ui->pbK16->size()/2);
    }
    else
    {
        ui->pbK16->setIcon(R3_Red);
       // ui->pbK16->setIconSize(ui->pbK16->size()/2);
    }

    if(a[6]==ok)
    {
        ui->pbK17->setIcon(R2_Green);
      //  ui->pbK17->setIconSize(ui->pbK17->size()/2);
    }
    else
    {
        ui->pbK17->setIcon(R3_Red);
      //  ui->pbK17->setIconSize(ui->pbK17->size()/2);
    }

    if(a[7]==ok)
    {
        ui->pbK18->setIcon(R2_Green);
       // ui->pbK18->setIconSize(ui->pbK18->size()/2);
    }
    else
    {
        ui->pbK18->setIcon(R3_Red);
       // ui->pbK18->setIconSize(ui->pbK18->size()/2);
    }

    if(a[8]==ok)
    {
        ui->pbK19->setIcon(R2_Green);
       // ui->pbK19->setIconSize(ui->pbK19->size()/2);
    }
    else
    {
        ui->pbK19->setIcon(R3_Red);
      //  ui->pbK19->setIconSize(ui->pbK19->size()/2);
    }

    if(a[9]==ok)
    {
        ui->pbK110->setIcon(R2_Green);
       // ui->pbK110->setIconSize(ui->pbK110->size()/2);
    }
    else
    {
        ui->pbK110->setIcon(R3_Red);
       // ui->pbK110->setIconSize(ui->pbK110->size()/2);
    }

}


void MainWindow::SetImage2(QByteArray a) //установка начального цвета лампочек в серый в ГРУПБОКСЕ к1
{
    QPixmap R1_Gray(":/images/circle_gray.png");
    QPixmap R2_Green(":/images/circle_zel.png");
    QPixmap R3_Red(":/images/circle_kras.png");

    auto ok = char(0x01);

    if(a[0]==ok)
    {
        ui->pbK21->setIcon(R2_Green);
       // ui->pbK21->setIconSize(ui->pbK21->size()/2);
    }
    else
    {
        ui->pbK21->setIcon(R3_Red);
       // ui-> pbK21->setIconSize(ui->pbK21->size()/2);
    }

    if(a[1]== ok)
    {
        ui->pbK22->setIcon(R2_Green);
       // ui->pbK22->setIconSize(ui->pbK22->size()/2);
    }
    else
    {
        ui->pbK22->setIcon(R3_Red);
       // ui->pbK22->setIconSize(ui->pbK22->size()/2);
    }

    if(a[2]== ok)
    {
        ui->pbK23->setIcon(R2_Green);
       // ui->pbK23->setIconSize(ui->pbK23->size()/2);
    }
    else
    {
        ui->pbK23->setIcon(R3_Red);
       // ui->pbK23->setIconSize(ui->pbK23->size()/2);
    }

    if(a[3]== ok)
    {
        ui->pbK24->setIcon(R2_Green);
       // ui->pbK24->setIconSize(ui->pbK24->size()/2);
    }
    else
    {
        ui->pbK24->setIcon(R3_Red);
       // ui->pbK24->setIconSize(ui->pbK24->size()/2);
    }

    if(a[4]== ok)
    {
        ui->pbK25->setIcon(R2_Green);
       // ui->pbK25->setIconSize(ui->pbK25->size()/2);
    }
    else
    {
        ui->pbK25->setIcon(R3_Red);
      //  ui->pbK25->setIconSize(ui->pbK25->size()/2);
    }

    if(a[5]== ok)
    {
        ui->pbK26->setIcon(R2_Green);
       // ui->pbK26->setIconSize(ui->pbK26->size()/2);
    }
    else
    {
        ui->pbK26->setIcon(R3_Red);
       // ui->pbK26->setIconSize(ui->pbK26->size()/2);
    }

    if(a[6]== ok)
    {
        ui->pbK27->setIcon(R2_Green);
       // ui->pbK27->setIconSize(ui->pbK27->size()/2);
    }
    else
    {
        ui->pbK27->setIcon(R3_Red);
       // ui->pbK27->setIconSize(ui->pbK27->size()/2);
    }

    if(a[7]== ok)
    {
        ui->pbK28->setIcon(R2_Green);
       // ui->pbK28->setIconSize(ui->pbK28->size()/2);
    }
    else
    {
        ui->pbK28->setIcon(R3_Red);
       // ui->pbK28->setIconSize(ui->pbK28->size()/2);
    }

    if(a[8]== ok)
    {
        ui->pbK29->setIcon(R2_Green);
       // ui->pbK29->setIconSize(ui->pbK29->size()/2);
    }
    else
    {
        ui->pbK29->setIcon(R3_Red);
       // ui->pbK29->setIconSize(ui->pbK29->size()/2);
    }

    if(a[9]== ok)
    {
        ui->pbK210->setIcon(R2_Green);
       // ui->pbK210->setIconSize(ui->pbK210->size()/2);
    }
    else
    {
        ui->pbK210->setIcon(R3_Red);
       // ui->pbK210->setIconSize(ui->pbK210->size()/2);
    }

}


void MainWindow::SetImage3(QByteArray a) //установка начального цвета лампочек в серый в ГРУПБОКСЕ к1
{
    QPixmap R1_Gray(":/images/circle_gray.png");
    QPixmap R2_Green(":/images/circle_zel.png");
    QPixmap R3_Red(":/images/circle_kras.png");

    auto ok = char(0x01);

    if(a[0]== ok)
    {
        ui->K31->setIcon(R2_Green);
        // ui-> K31->setIconSize(ui->K31->size());
    }
    else
    {
        ui->K31->setIcon(R3_Red);
        // ui-> K31->setIconSize(ui->K31->size());
    }

    if(a[1]== ok)
    {
        ui->K32->setIcon(R2_Green);
        // ui->K32->setIconSize(ui->K32->size());
    }
    else
    {
        ui->K32->setIcon(R3_Red);
        //  ui->K32->setIconSize(ui->K32->size());
    }

    if(a[2]== ok)
    {
        ui->K33->setIcon(R2_Green);
        // ui-> K33->setIconSize(ui->K33->size());
    }
    else
    {
        ui->K33->setIcon(R3_Red);
        //  ui->K33->setIconSize(ui->K33->size());
    }

    if(a[3]== ok)
    {
        ui->K34->setIcon(R2_Green);
        // ui->K34->setIconSize(ui->K34->size());
    }
    else
    {
        ui->K34->setIcon(R3_Red);
        //  ui->K34->setIconSize(ui->K34->size());
    }

    if(a[4]== ok)
    {
        ui->K35->setIcon(R2_Green);
        // ui->K35->setIconSize(ui->K35->size());
    }
    else
    {
        ui->K35->setIcon(R3_Red);
        //  ui->K35->setIconSize(ui->K35->size());
    }

    if(a[5]== ok)
    {
        ui->K36->setIcon(R2_Green);
        // ui->K36->setIconSize(ui->K36->size());
    }
    else
    {
        ui->K36->setIcon(R3_Red);
        //  ui->K36->setIconSize(ui->K36->size());
    }

    if(a[6]== ok)
    {
        ui->K37->setIcon(R2_Green);
        // ui->K37->setIconSize(ui->K37->size());
    }
    else
    {
        ui->K37->setIcon(R3_Red);
        //  ui->K37->setIconSize(ui->K37->size());
    }

    if(a[7]== ok)
    {
        ui->K38->setIcon(R2_Green);
        // ui->K38->setIconSize(ui->K38->size());
    }
    else
    {
        ui->K38->setIcon(R3_Red);
        // ui->K38->setIconSize(ui->K38->size());
    }

    if(a[8]== ok)
    {
        ui->K39->setIcon(R2_Green);
        // ui->K39->setIconSize(ui->K39->size());
    }
    else
    {
        ui->K39->setIcon(R3_Red);
        // ui->K39->setIconSize(ui->K39->size());
    }

    if(a[9]== ok)
    {
        ui->K310->setIcon(R2_Green);
        // ui->K310->setIconSize(ui->K310->size());
    }
    else
    {
        ui->K310->setIcon(R3_Red);
        // ui->K310->setIconSize(ui->K310->size());
    }

}

void MainWindow::SetImage4(QByteArray a) //установка начального цвета лампочек в серый в ГРУПБОКСЕ к1
{
    QPixmap R1_Gray(":/images/circle_gray.png");
    QPixmap R2_Green(":/images/circle_zel.png");
    QPixmap R3_Red(":/images/circle_kras.png");

    auto ok = char(0x01);

    if(a[0]== ok)
    {
        ui->K41->setIcon(R2_Green);
        //  ui->K41->setIconSize(ui->K41->size());
    }
    else
    {
        ui->K41->setIcon(R3_Red);
        //   ui->K41->setIconSize(ui->K41->size());
    }

    if(a[1]== ok)
    {
        ui->K42->setIcon(R2_Green);
        //  ui->K42->setIconSize(ui->K42->size());
    }
    else
    {
        ui->K42->setIcon(R3_Red);
        //  ui->K42->setIconSize(ui->K42->size());
    }

    if(a[2]== ok)
    {
        ui->K43->setIcon(R2_Green);
        // ui-> K43->setIconSize(ui->K43->size());
    }
    else
    {
        ui->K43->setIcon(R3_Red);
        //   ui->K43->setIconSize(ui->K43->size());
    }

    if(a[3]== ok)
    {
        ui->K44->setIcon(R2_Green);
        // ui->K44->setIconSize(ui->K44->size());
    }
    else
    {
        ui->K44->setIcon(R3_Red);
        //  ui->K44->setIconSize(ui->K44->size());
    }

    if(a[4]== ok)
    {
        ui->K45->setIcon(R2_Green);
        // ui->K45->setIconSize(ui->K45->size());
    }
    else
    {
        ui->K45->setIcon(R3_Red);
        //  ui->K45->setIconSize(ui->K45->size());
    }

    if(a[5]== ok)
    {
        ui->K46->setIcon(R2_Green);
        //  ui->K46->setIconSize(ui->K46->size());
    }
    else
    {
        ui->K46->setIcon(R3_Red);
        //  ui->K46->setIconSize(ui->K46->size());
    }

    if(a[6]== ok)
    {
        ui-> K47->setIcon(R2_Green);
        // ui->K47->setIconSize(ui->K47->size());
    }
    else
    {
        ui->K47->setIcon(R3_Red);
        //  ui->K47->setIconSize(ui->K47->size());
    }

    if(a[7]== ok)
    {
        ui->K48->setIcon(R2_Green);
        // ui->K48->setIconSize(ui->K48->size());
    }
    else
    {
        ui->K48->setIcon(R3_Red);
        //  ui->K48->setIconSize(ui->K48->size());
    }

    if(a[8]== ok)
    {
        ui-> K49->setIcon(R2_Green);
        // ui->K49->setIconSize(ui->K49->size());
    }
    else
    {
        ui->K49->setIcon(R3_Red);
        // ui-> K49->setIconSize(ui->K49->size());
    }

    if(a[9]== ok)
    {
        ui->K410->setIcon(R2_Green);
        //ui->K410->setIconSize(ui->K410->size());
    }
    else
    {
        ui-> K410->setIcon(R3_Red);
        //  ui->K410->setIconSize(ui->K410->size());
    }

}
