#include <mainwindow.h>
#include "ui_mainwindow.h"


// Функция Лог приложения
void MainWindow::log_wr_message(QString msg)
{

   log_text->append(msg);

   log_text->moveCursor(QTextCursor::End); // перемещаем курсор в начало документа, чтобы всегда было видно новую строчку

}

// Функция Лог Источника питания
void MainWindow::log_wr_message_Ist(QString msg)
{
    log_text_Ist->append(msg);
    log_text_Ist->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor); // перемещаем курсор в начало документа, чтобы всегда было видно новую строчку

}

// Функция Лог Генератора
void MainWindow::log_wr_message_Micran(QString msg)
{
    log_text_Micran->append(msg);

    log_text_Micran->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor); // перемещаем курсор в начало документа, чтобы всегда было видно новую строчку

  //  qDebug () << "log_text->moveCursor msg = " << msg;


}


// Функция Лог Пульт
void MainWindow::log_wr_message_TP(QString msg)
{
    log_text_TP->append(msg);
    log_text_TP->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor); // перемещаем курсор в начало документа, чтобы всегда было видно новую строчку

}

// Функция Лог Блока
void MainWindow::log_wr_message_Block(QString msg)
{
    log_text_Block->append(msg);
    log_text_Block->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor); // перемещаем курсор в начало документа, чтобы всегда было видно новую строчку

}

// Функция Лог Анализатора
void MainWindow::log_wr_message_N9000(QString msg)
{
    log_text_N9000->append(msg);// append(msg);
    log_text_N9000->moveCursor(QTextCursor::End,QTextCursor::MoveAnchor); // перемещаем курсор в начало документа, чтобы всегда было видно новую строчку

}



// Функция Лог приложения отчистки
void MainWindow::log_wr_message_claer()
{
    log_text->document()->clear();
    log_text->moveCursor(QTextCursor::Start,QTextCursor::MoveAnchor); // перемещаем курсор в начало документа, чтобы всегда было видно новую строчку
}

// Функция Лог Источника питания отчистки
void MainWindow::log_wr_message_Ist_claer()
{

    log_text_Ist->document()->clear();
    log_text_Ist->moveCursor(QTextCursor::Start,QTextCursor::MoveAnchor); // перемещаем курсор в начало документа, чтобы всегда было видно новую строчку

}

// Функция Лог Генератора отчистки
void MainWindow::log_wr_message_Micran_claer()
{
    log_text_Micran->document()->clear();
    log_text_Micran->moveCursor(QTextCursor::Start,QTextCursor::MoveAnchor); // перемещаем курсор в начало документа, чтобы всегда было видно новую строчку
}


// Функция Лог Пульт отчистки
void MainWindow::log_wr_message_TP_claer()
{
    log_text_TP->document()->clear();
    log_text_TP->moveCursor(QTextCursor::Start,QTextCursor::MoveAnchor); // перемещаем курсор в начало документа, чтобы всегда было видно новую строчку

}

// Функция Лог Блока отчистки
void MainWindow::log_wr_message_Block_claer()
{

    log_text_Block->document()->clear();
    log_text_Block->moveCursor(QTextCursor::Start,QTextCursor::MoveAnchor); // перемещаем курсор в начало документа, чтобы всегда было видно новую строчку
}

// Функция Лог Анализатора отчистки
void MainWindow::log_wr_message_N9000_claer()
{
    log_text_N9000->document()->clear();
    log_text_N9000->moveCursor(QTextCursor::Start,QTextCursor::MoveAnchor); // перемещаем курсор в начало документа, чтобы всегда было видно новую строчку
}
