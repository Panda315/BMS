#include "mainwindow.h"
#include <QIODevice>
#include <QDir>
#include "ui_mainwindow.h"
#include "horizontaltab.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QPixmap>
#include <QDebug>
#include <string>
#include "student.h"
#include "professor.h"
#include "block.h"
#include "view_professor.h"
#include "view_student.h"
#include "update_professor.h"
#include "clear_data.h"
#include "delete_student.h"
#include "add_faculty.h"
#include "add_dept.h"

//check if room is available in given time : from reservations.db
bool room_available(int a_start, int a_end, int start, int end) {
    if(a_start == start || a_end == end) return false;
    if(start < a_start && end > a_start) return false;
    if(start > a_start && start < a_end) return false;
    return true;
}

QString filepath;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("BMS");
    ui->dashboardTabWidget->tabBar()->setStyle(new CustomTabStyle);
    QFile faculty(":/resources/data/faculty.txt");
    QTextStream faculty_stream(&faculty);
    QString line;
    if (faculty.open(QFile::ReadOnly | QFile::Text)) {
       while (!faculty_stream.atEnd())
       {
          line = faculty_stream.readLine();
          ui->rev_faculty->addItem(line);
//          ui->profile_register_dept->addItem(line);
          ui->routine_faculty->addItem(line);
       }
       faculty_stream.flush(); faculty.close();
    }

    collegedbOpen();
    QSqlQuery qry;
    qry.prepare("select * from professor");
    if(qry.exec()) {
        while(qry.next()) {
            ui->rev_professor->addItem(qry.value(0).toString());
            ui->ps_name->addItem(qry.value(0).toString());
            ui->ov_professor->addItem(qry.value(0).toString());

        }
    }

    qry.prepare("select * from block");
    if(qry.exec()) {
        while(qry.next()) {
            ui->ov_block->addItem(qry.value(0).toString());
            ui->rev_block_no->addItem(qry.value(0).toString());
            ui->sw_from_block->addItem(qry.value(0).toString());
            ui->sw_to_block->addItem(qry.value(0).toString());
            ui->cs_block->addItem(qry.value(0).toString());
            ui->del_block->addItem(qry.value(0).toString());
            ui->dur_block->addItem(qry.value(0).toString());
        }
    }
    collegedbClose();

    ui->mainTabWidget->setStyleSheet("QTabBar::tab { min-width: 100px; min-height: 60px;  }");

}

MainWindow::~MainWindow()
{
    delete ui;
}

//to reserve a room : reservations.db
void MainWindow::on_reserve_button_clicked()
{
    //read all the data from the reserve tab
    QString block = ui->rev_block_no->currentText();
    QString room = ui->rev_room_no->text();
    QString faculty = ui->rev_faculty->currentText();
    QString year = ui->rev_year->currentText();
    QString semester = ui->rev_semester->currentText();
    QString start = ui->rev_start_time->currentText();
    QString end = ui->rev_end_time->currentText();
    QString day = ui->rev_day->currentText();
    QString professor = ui->rev_professor->currentText();
    QString subject = ui->rev_subject->text();
    QString status = "Reserved";
    std::string s_day = day.toUtf8().constData();

    reservationsOpen();     //open and connect to reservations.db
    QSqlQuery qry;
    qry.prepare("select start,end from '"+day+"' where block='"+block+"' and room='"+room+"'");   //prepare a query to search the reserved room from the database
    if(qry.exec()) {
        int check_count = 1;
        while(qry.next()) {
            qDebug() << "(" << check_count++ << ")" << "Checking...";
            int a_start = qry.value(0).toInt();
            int a_end = qry.value(1).toInt();
            if(room_available(a_start, a_end, start.toInt(), end.toInt())) {
                continue;
            } else {
                qDebug() << "Cannot Reserve! Exiting!";
                return;
            }
        }
        //if room is available
        qry.prepare("insert into '"+day+"' (block,room,start,end,status,faculty,year,semester,professor,subject) values ('"+block+"','"+room+"','"+start+"','"+end+"','"+status+"','"+faculty+"','"+year+"','"+semester+"','"+professor+"','"+subject+"')");
        if(qry.exec()) {
            qDebug() << "Room available. Writing to the database.";
        } else qDebug() << "Couldn't execute the query to book the room!";
    } else {    //if no entry is found in the database
        qDebug() << "No entries for the data. Creating a new entry...";
        qry.prepare("insert into '"+day+"' (block,room,start,end,status,faculty,year,semester,professor,subject) values ('"+block+"','"+room+"','"+start+"','"+end+"','"+status+"','"+faculty+"','"+year+"','"+semester+"','"+professor+"','"+subject+"')");
        if(qry.exec()) {
            qDebug() << "Created a new entry to the database.";
        } else {
            qDebug() << "Couldn't create a new entry to the database!";
        }
    }
    reservationsClose();    //close the connection to reservations.db
    qDebug() << "Successfully reserved!";
}

void MainWindow::on_switch_button_clicked()
{
    //read all the data from the switch tab
    QString from_block = ui->sw_from_block->currentText();
    QString from_room = ui->sw_from_room->text();
    QString from_day = ui->sw_from_day->currentText();
    QString from_stime = ui->sw_from_stime->currentText();
    int stime_from = from_stime.toInt();
    QString from_etime = ui->sw_from_etime->currentText();
    int etime_from = from_etime.toInt();
    QString to_block = ui->sw_to_block->currentText();
    QString to_room = ui->sw_to_room->text();
    QString to_day = ui->sw_to_day->currentText();
    QString to_stime = ui->sw_to_stime->currentText();
    int stime_to = to_stime.toInt();
    QString to_etime = ui->sw_to_etime->currentText();
    int etime_to= to_etime.toInt();



     reservationsOpen();
     QSqlQuery qry;
     QMessageBox::StandardButton reply = QMessageBox::question(this,"Confirmation","Do you want to switch?",QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
     if(reply == QMessageBox::Yes)
     {
         if((stime_from - etime_from) == (stime_to - etime_to)) //checking if classes are of same time duration or not
         {
               qDebug() << "reached inside yes";

               //checking if given class exist or not at the given time
               qry.prepare("select * from '"+from_day+"' where block ='"+from_block+"' and room = '"+from_room+"' and start = '"+from_stime+"' and end = '"+from_etime+"'");
               if(qry.exec())
               {
                    qDebug() << "reached inside 2";
                    int _count = 0;
                    while(qry.next())
                    {
                         _count++;
                    }

                    if(_count<1) //class does not exist
                    {
                         QMessageBox::warning(this,"Warning","Class to be switched does not exist");
                         reservationsClose();
                         return;
                    }

                    if(_count>1)
                    {
                        QMessageBox::warning(this,"Warning","Error in the database !!!"); //error in the database is to show that more than one classses exist for the smae time duration
                        reservationsClose();
                        return;
                    }
                 }

               int count = 0;
               //checking if another class exist or not in the targeted location at the same time
               qry.prepare("select * from '"+to_day +"' where block='"+to_block+"' and room = '"+to_room+"' and start = '"+to_stime+"' and end = '"+to_etime+"'");
               if(qry.exec())
               {
                    qDebug() << "reached inside 1";
                    while(qry.next())
                    {
                         count++;
                    }

               }

               qry.prepare("select * from '"+to_day+"' where block='"+to_block+"' and room = '"+to_room+"' and (start between :to_stime_to and :to_etime_to) and (end between :to_stime_to and :to_etime_to) ");
               qry.bindValue(":to_stime_to",stime_to);
               qry.bindValue(":to_etime_to",etime_to);
               int check_class = 0;
               if(qry.exec())
               {
                  while(qry.next())
                  {
                      check_class++;
                  }
               }
               qDebug() << "value of check_class is " << check_class;
              //if another class exist which runs or end between the given time duration
              qry.prepare("select * from '"+to_day+"' where block='"+to_block+"' and room = '"+to_room+"' and (((start between :to_stime_to and :to_etime_to) and (end not between :to_stime_to and :to_etime_to)) or ((start not between :to_stime_to and :to_etime_to) and (end between :to_stime_to and :to_etime_to)))");
              qry.bindValue(":to_stime_to",stime_to);
              qry.bindValue(":to_etime_to",etime_to);
              int _check_class = 0;
              if(qry.exec())
              {
                  while(qry.next())
                  {
                        _check_class++;
                  }
              }
              qDebug() <<"value of _check_class is " << _check_class;


               if(count == 0 && (check_class>=1 || _check_class>=1))
               {
                   QMessageBox::information(this,"Termination","Class time not avaiable.");    //because in the "to room" class of same time duration does not exist and other classes run
                   reservationsClose();
                   return;
               }

               else if(count == 0)
               {
                   if(from_day == to_day)
                   {
                       qry.prepare("update '"+to_day+"' set block = '"+to_block+"',room = '"+to_room+"',start='"+to_stime+"',end='"+to_etime+"' where block='"+from_block+"' and room = '"+from_room+"' and start = '"+from_stime+"' and end = '"+from_etime+"' ");
                       if(qry.exec())
                       {
                           QMessageBox::information(this,"Successful","Class has been sucessfully switched");
                           reservationsClose();
                           return;
                       }
                   }
                   else //if in different day
                   {
                       qry.prepare("insert into '"+to_day+"' (block,room,start,end,status,faculty,year,semester,professor,subject) select * from '"+from_day+"' where block='"+from_block+"' and room = '"+from_room+"' and start = '"+from_stime+"' and end = '"+from_etime+"'");
                       if(qry.exec())
                       {
                            qry.prepare("delete from '"+from_day+"' where block='"+from_block+"' and room = '"+from_room+"' and start = '"+from_stime+"' and end = '"+from_etime+"'");
                            if(qry.exec())
                            {
                                qry.prepare("update '"+to_day+"' set block = '"+to_block+"',room = '"+to_room+"',start='"+to_stime+"',end='"+to_etime+"' where block='"+from_block+"' and room = '"+from_room+"' and start = '"+from_stime+"' and end = '"+from_etime+"'");
                                if(qry.exec())
                                {
                                    QMessageBox::information(this,"Successful","Class has been sucessfully switched");
                                    reservationsClose();
                                    return;
                                }
                            }
                        }
                   }
               }

               else if(count == 1)   //if class exist in the same time duration
               {
                   qDebug() << "reached inside 3";
                   if(from_day == to_day)  //if both the classes are in the same day
                   {
                       qDebug() << "reached D-1";
                       qry.prepare("insert into temp (block,room,start,end,status,faculty,year,semester,professor,subject) select * from '"+from_day+"' where block='"+from_block+"' and room = '"+from_room+"' and start = '"+from_stime+"' and end = '"+from_etime+"'");
                       if(qry.exec())
                       {
                            qDebug() << "reached D-2";
                            qry.prepare("update temp set start ='"+to_stime+"',end='"+to_etime+"',block='"+to_block+"',room='"+to_room+"' where block='"+from_block+"' and room = '"+from_room+"' and start='"+from_stime+"'and end='"+from_etime+"'");
                            if(qry.exec())
                            {
                                 qDebug() << "reached D-3";
                                 qry.prepare("delete from '"+from_day+"' where block='"+from_block+"' and room = '"+from_room+"' and start = '"+from_stime+"' and end = '"+from_etime+"'");
                                 if(qry.exec())
                                 {
                                      qDebug() << "reached D-4";
                                      qry.prepare("update '"+from_day+"' set start ='"+from_stime+"',end='"+from_etime+"',block='"+from_block+"',room='"+from_room+"' where block='"+to_block+"' and room = '"+to_room+"' and start='"+to_stime+"'and end='"+to_etime+"'");
                                      if(qry.exec())
                                      {
                                           qDebug() << "reached D-5";
                                           qry.prepare("insert into '"+from_day+"' (block,room,start,end,status,faculty,year,semester,professor,subject) select * from temp where block='"+to_block+"' and room = '"+to_room+"' and start = '"+to_stime+"' and end = '"+to_etime+"'");
                                           if(qry.exec())
                                           {
                                                qDebug() << "reached D-6";
                                                qry.prepare("delete from temp");
                                                if(qry.exec())
                                                {
                                                     QMessageBox::information(this,"Successful","Class has been sucessfully switched");
                                                     reservationsClose();
                                                     return;
                                                 }
                                           }
                                      }
                                 }
                             }
                         }
                     }

                   else   //if both classes are not in the same day
                   {
                   if(qry.exec("insert into temp (block,room,start,end,status,faculty,year,semester,professor,subject) select * from '"+from_day+"' where block='"+from_block+"' and room = '"+from_room+"' and start = '"+from_stime+"' and end = '"+from_etime+"'"))
                    {
                         if(qry.exec("delete from '"+from_day+"' where block='"+from_block+"' and room = '"+from_room+"' and start = '"+from_stime+"' and end = '"+from_etime+"'"))
                         {
                              if(qry.exec("insert into '"+from_day+"' (block,room,start,end,status,faculty,year,semester,professor,subject) select * from '"+to_day +"' where block='"+to_block+"' and room = '"+to_room+"' and start = '"+to_stime+"' and end = '"+to_etime+"'"))
                              {
                                   if(qry.exec("update '"+from_day+"' set block = '"+from_block+"',room = '"+from_room+"',start='"+from_stime+"',end='"+from_etime+"' where block='"+to_block+"' and room = '"+to_room+"' and start = '"+to_stime+"' and end = '"+to_etime+"'"))
                                   {
                                       if(qry.exec("delete from '"+to_day+"' where block='"+to_block+"' and room = '"+to_room+"' and start = '"+to_stime+"' and end = '"+to_etime+"'"))
                                       {
                                            if(qry.exec("insert into '"+to_day+"' (block,room,start,end,status,faculty,year,semester,professor,subject) select * from temp where block='"+from_block+"' and room = '"+from_room+"' and start = '"+from_stime+"' and end = '"+from_etime+"'"))
                                            {
                                                 if(qry.exec("delete from temp"))
                                                 {
                                                      if(qry.exec("update '"+to_day+"' set block = '"+to_block+"',room = '"+to_room+"',start='"+to_stime+"',end='"+to_etime+"' where block='"+from_block+"' and room = '"+from_room+"' and start = '"+from_stime+"' and end = '"+from_etime+"'"))
                                                      {
                                                            QMessageBox::information(this,"Successful","Class has been sucessfully switched");
                                                            reservationsClose();
                                                            return;
                                                       }
                                                 }
                                            }
                                       }
                                   }
                              }
                         }
                    }
                 }

               }

               else
               {
                   QMessageBox::warning(this,"Warning","Error in the database !!!");
                   reservationsClose();
                   return;
               }

         }

         else  //if classes are not of same time duration
         {
             QMessageBox::information(this,"Termination","Only classes with same time duration can be switched.");
             reservationsClose();
             return;
         }

     }

     else
     {
         QMessageBox::information(this,"Termination","Switch process was terminated.");
         reservationsClose();
         return;
     }

}

void MainWindow::on_ps_show_clicked()
{
    QString professor = ui->ps_name->currentText();
    QString day = ui->ps_day->currentText();
    QString start = ui->ps_start->currentText();
    QString end = ui->ps_end->currentText();
    bool available = true;

    reservationsOpen();
    QSqlQuery qry;
    qry.prepare("Select start,end from '"+day+"' where professor='"+professor+"'");
    if(qry.exec()) {
        while(qry.next()) {
            int a_start = qry.value(0).toInt();
            int a_end = qry.value(1).toInt();
            if(room_available(a_start, a_end, start.toInt(), end.toInt())) {
                continue;
            } else {
                available = false;
                break;
            }
        }
        if(available) {
            QMessageBox::information(this, "Status", professor+" is available!");
        } else {
            QMessageBox::information(this, "Status", professor+" is not available!");
        }
    } else {
        qDebug() << "Failed to execute the query!";
    }
    reservationsClose();
}

void MainWindow::on_cs_show_clicked()
{

    QString block = ui->cs_block->currentText();
    QString room = ui->cs_room->text();
    QString day = ui->cs_day->currentText();
    QString start = ui->cs_start_time->currentText();
    QString end = ui->cs_end_time->currentText();

    bool available  = true;
    reservationsOpen();
    QSqlQuery qry;

    qry.prepare("select start,end from '"+day+"' where block='"+block+"' and room= '"+room+"'" );
                if(qry.exec()){
                       int check_count =1;
                       while (qry.next()){
                           qDebug()<<"(" << check_count++ << ")" <<"Checking...";
                           int a_start= qry.value(0).toInt();
                           int a_end= qry.value(1).toInt();
                           if (room_available(a_start,a_end,start.toInt(), end.toInt())) {
                               continue;
                           } else{
                             available = false;
                               break;
                           }
                       }
                      if (available){
                          QMessageBox::information(this, "class","Class is available");
                      }else{
                          QMessageBox::information(this, "class","Class is not available");
                      } }else {

                          qDebug()<<"Failed to execute the query!";
                      }
                      reservationsClose();

                   }


void MainWindow::on_ov_show_clicked()
{
    reservationsOpen();
    QString day = ui->ov_day->currentText();
    QString block = ui->ov_block->currentText();
    QString professor = ui->ov_professor->currentText();
    QSqlQueryModel * modal = new QSqlQueryModel();
    QSqlQuery * qry = new QSqlQuery(reservations);

    if(block.toUtf8() != "All" && professor.toUtf8() != "All") {
        qDebug() << block << " " << professor;
        qry->prepare("select * from '"+day+"' where block = '"+block+"' and professor = '"+professor+"'");
       }
    else if(block.toUtf8() != "All" && professor.toUtf8() == "All"){
        qry->prepare("select * from '"+day+"' where block = '"+block+"'");
        qDebug() << "2";
    }
    else if(professor.toUtf8() != "All" && block.toUtf8() == "All")
        qry->prepare("select * from '"+day+"' where professor = '"+professor+"'");
    else
        qry->prepare("select * from '"+day+"'");

    qry->exec();
    modal->setQuery(*qry);
    modal->setHeaderData(0, Qt::Horizontal, QObject::tr("Block"));
    modal->setHeaderData(1, Qt::Horizontal, QObject::tr("Room"));
    modal->setHeaderData(2, Qt::Horizontal, QObject::tr("Start"));
    modal->setHeaderData(3, Qt::Horizontal, QObject::tr("End"));
    modal->setHeaderData(4, Qt::Horizontal, QObject::tr("Status"));
    modal->setHeaderData(5, Qt::Horizontal, QObject::tr("Faculty"));
    modal->setHeaderData(6, Qt::Horizontal, QObject::tr("Year"));
    modal->setHeaderData(7, Qt::Horizontal, QObject::tr("Semester"));
    modal->setHeaderData(8, Qt::Horizontal, QObject::tr("Professor"));
    modal->setHeaderData(9, Qt::Horizontal, QObject::tr("Subject"));


    ui->ov_tableView->setModel(modal);
    ui->ov_tableView->horizontalHeader()->setStretchLastSection(true);
    ui->ov_tableView->setColumnWidth(5, 240);
    ui->ov_tableView->setColumnWidth(8, 240);
    ui->ov_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->ov_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    reservationsClose();
    qDebug() << modal->rowCount();
}

void MainWindow::on_delete_2_clicked()
{

        QString block= ui ->del_block->currentText();
        QString room = ui ->del_room->text();
        QString day = ui->del_day-> currentText();
        QString start = ui ->del_start_time->currentText();
        QString end = ui ->del_end_time->currentText();
        reservationsOpen();
        QSqlQuery qry;

    bool ok;


    ok = qry.exec("select * from '"+day+"' WHERE block='"+block+"' and room= '"+room+"'and start = '"+start+"' and end = '"+end+"' ");
    if (ok && qry.next()) {

                                 qry.prepare("Delete from '"+day+"' where block='"+block+"' and room= '"+room+"'and start = '"+start+"' and end = '"+end+"'" );
                                 qry.exec();
        QMessageBox::information(this, "class","Class is deleted");

    }else{
                                  QMessageBox::information(this, "class","Unable to delete class");

                                  }
                               reservationsClose();
    }



void MainWindow::on_routine_show_clicked()
{
    QString day = ui->routine_day->currentText();
    QString faculty = ui->routine_faculty->currentText();
    QString year = ui->routine_year->currentText();
    QString semester = ui->routine_semester->currentText();

    reservationsOpen();

    QSqlQueryModel * routine_modal = new QSqlQueryModel();
    QSqlQuery * qry = new QSqlQuery(reservations);

    qry->prepare("select start,end,subject from '"+day+"' where faculty = '"+faculty+"' and year = '"+year+"' and semester ='"+semester+"'");
    if(qry->exec()) {
        qDebug() << "Routine shown successfully!";
    }
    routine_modal->setQuery(*qry);
    routine_modal->setHeaderData(0, Qt::Horizontal, QObject::tr("Start"));
    routine_modal->setHeaderData(1, Qt::Horizontal, QObject::tr("End"));
    routine_modal->setHeaderData(2, Qt::Horizontal, QObject::tr("Subject"));

    ui->routine_table->setModel(routine_modal);
    ui->routine_table->horizontalHeader()->setStretchLastSection(true);
    ui->routine_table->setColumnWidth(5, 240);
    ui->routine_table->setColumnWidth(8, 240);
    ui->routine_table->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->routine_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    reservationsClose();
}


void MainWindow::on_add_professor_clicked()
{
            Professor professor;
            professor.setModal(true);
            professor.exec();
}




void MainWindow::on_add_student_clicked()
{
           student student;
           student.setModal(true);
           student.exec();
}




void MainWindow::on_add_block_clicked()
{
    block block;
    block.setModal(true);
    block.exec();
}


void MainWindow::on_view_professor_clicked()
{
    view_professor vp;
    vp.setModal(true);
    vp.exec();
}


void MainWindow::on_view_student_clicked()
{
    view_student vs;
    vs.setModal(true);
    vs.exec();
}


void MainWindow::on_update_student_clicked()
{


     QMessageBox::StandardButton reply = QMessageBox::question(this,"Confirmation","Semester of all the students will be updated by one. Do you want to confirm it ?",QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
     if(reply==QMessageBox::Yes)
     {
         collegedbOpen();
         QSqlQuery qry;
         qry.prepare("update student set semester=semester+1 ");
         if(qry.exec())
         {
                qDebug() << "thik cha";
                qry.prepare("update student set year=year+1,semester=1 where semester=3 ");
                if(qry.exec())
                {
                   QMessageBox::information(this,"Sucess","Sucessfully updated");
                }
        }
     }
     else
     {
         QMessageBox::information(this,"Terminated","Student information update process terminated");
     }
}


void MainWindow::on_update_professor_clicked()
{
    update_professor up;
    up.setModal(true);
    up.exec();
}


void MainWindow::on_load_request_clicked()
{
    reservationsOpen();
    QSqlQueryModel * requests = new QSqlQueryModel();
    QSqlQuery * qry = new QSqlQuery(reservations);

    qry->prepare("select * from request");

    qry->exec();
    requests->setQuery(*qry);
    requests->setHeaderData(0, Qt::Horizontal, QObject::tr("Faculty"));
    requests->setHeaderData(1, Qt::Horizontal, QObject::tr("Year"));
    requests->setHeaderData(2, Qt::Horizontal, QObject::tr("Semester"));
    requests->setHeaderData(3, Qt::Horizontal, QObject::tr("Day"));
    requests->setHeaderData(4, Qt::Horizontal, QObject::tr("Start"));
    requests->setHeaderData(5, Qt::Horizontal, QObject::tr("End"));
    requests->setHeaderData(6, Qt::Horizontal, QObject::tr("Block"));
    requests->setHeaderData(7, Qt::Horizontal, QObject::tr("Room"));
    requests->setHeaderData(8, Qt::Horizontal, QObject::tr("Subject"));
    requests->setHeaderData(9, Qt::Horizontal, QObject::tr("Professor"));


    ui->req_tableview->setModel(requests);
    ui->req_tableview->horizontalHeader()->setStretchLastSection(true);
    ui->req_tableview->setColumnWidth(5, 240);
    ui->req_tableview->setColumnWidth(8, 240);
    ui->req_tableview->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->req_tableview->setSelectionBehavior(QAbstractItemView::SelectRows);
    reservationsClose();
    qDebug() << requests->rowCount();
}


void MainWindow::on_req_tableview_activated(const QModelIndex &index)
{
    QString val = ui->req_tableview->model()->data(index).toString();

    reservationsOpen();

    QSqlQuery qry;
    qry.prepare("select * from request where faculty='"+val+"' or year='"+val+"' or semester='"+val+"' or day='"+val+"' or start='"+val+"' or end='"+val+"' or block='"+val+"' or room='"+val+"' or subject='"+val+"' or professor='"+val+"'");

    if(qry.exec()) {
        while(qry.next()) {
            ui->req_faculty->setText(qry.value(0).toString());
            ui->req_year->setText(qry.value(1).toString());
            ui->req_semester->setText(qry.value(2).toString());
            ui->req_day->setText(qry.value(3).toString());
            ui->req_start->setText(qry.value(4).toString());
            ui->req_end->setText(qry.value(5).toString());
            ui->req_block->setText(qry.value(6).toString());
            ui->req_room->setText(qry.value(7).toString());
            ui->req_subject->setText(qry.value(8).toString());
            ui->req_professor->setText(qry.value(9).toString());
        }
    }

    reservationsClose();
}

void MainWindow::on_req_reserve_clicked()
{
    QString block = ui->req_block->text();
    QString room = ui->req_room->text();
    QString faculty = ui->req_faculty->text();
    QString year = ui->req_year->text();
    QString semester = ui->req_semester->text();
    QString start = ui->req_start->text();
    QString end = ui->req_end->text();
    QString day = ui->req_day->text();
    QString professor = ui->req_professor->text();
    QString subject = ui->req_subject->text();
    QString status = "Reserved";
    QString req_day = ui->req_day->text();

    reservationsOpen();     //open and connect to reservations.db
    QSqlQuery qry;
    qry.prepare("select start,end from '"+req_day+"' where block='"+block+"' and room='"+room+"'");   //prepare a query to search the reserved room from the database
    if(qry.exec()) {
        int check_count = 1;
        while(qry.next()) {
            qDebug() << "(" << check_count++ << ")" << "Checking...";
            int a_start = qry.value(0).toInt();
            int a_end = qry.value(1).toInt();
            if(room_available(a_start, a_end, start.toInt(), end.toInt())) {
                continue;
            } else {
                qDebug() << "Cannot Reserve! Exiting!";
                QMessageBox::information(this, "Request", "Cannot complete the request");
                return;
            }
        }
        //if room is available
        qry.prepare("delete from '"+req_day+"' where block = '"+block+"' and room = '"+room+"' and faculty = '"+faculty+"' and year = '"+year+"' and semester = '"+semester+"' and day = '"+day+"' and professor = '"+professor+"' and subject = '"+subject+"'");
        qry.exec();
        qry.prepare("insert into '"+req_day+"' (block,room,start,end,status,faculty,year,semester,professor,subject) values ('"+block+"','"+room+"','"+start+"','"+end+"','"+status+"','"+faculty+"','"+year+"','"+semester+"','"+professor+"','"+subject+"')");
        if(qry.exec()) {
            qDebug() << "Room available. Writing to the database.";
            QMessageBox::information(this, "Request", "Successfully reserved the requested class");
            QSqlQuery qry;
            qry.prepare("delete from request where block = '"+block+"' and room = '"+room+"' and faculty = '"+faculty+"' and year = '"+year+"' and semester = '"+semester+"' and start = '"+start+"' and end = '"+end+"' and day = '"+day+"' and professor = '"+professor+"' and subject = '"+subject+"'");
            qry.exec();
        } else qDebug() << "Couldn't execute the query to book the room!";
    } else {    //if no entry is found in the database
        qDebug() << "No entries for the data. Creating a new entry...";
        qry.prepare("delete from '"+req_day+"' where block = '"+block+"' and room = '"+room+"' and faculty = '"+faculty+"' and year = '"+year+"' and semester = '"+semester+"' and day = '"+day+"' and professor = '"+professor+"' and subject = '"+subject+"'");
        qry.exec();
        qry.prepare("insert into '"+req_day+"' (block,room,start,end,status,faculty,year,semester,professor,subject) values ('"+block+"','"+room+"','"+start+"','"+end+"','"+status+"','"+faculty+"','"+year+"','"+semester+"','"+professor+"','"+subject+"')");
        if(qry.exec()) {
            qDebug() << "Created a new entry to the database.";
            QMessageBox::information(this, "Request", "Successfully reserved the requested class");
            QSqlQuery qry;
            qry.prepare("delete from request where block = '"+block+"' and room = '"+room+"' and faculty = '"+faculty+"' and year = '"+year+"' and semester = '"+semester+"' and start = '"+start+"' and end = '"+end+"' and day = '"+day+"' and professor = '"+professor+"' and subject = '"+subject+"'");
            qry.exec();
            qry.prepare("delete from '"+req_day+"' where block = '"+block+"' and room = '"+room+"' and faculty = '"+faculty+"' and year = '"+year+"' and semester = '"+semester+"' and start = '"+start+"' and end = '"+end+"' and day = '"+day+"' and professor = '"+professor+"' and subject = '"+subject+"'");
            qry.exec();
        } else {
            qDebug() << "Couldn't create a new entry to the database!";
        }
    }
    reservationsClose();    //close the connection to reservations.db
    MainWindow::on_load_request_clicked();
}


void MainWindow::on_req_delete_clicked()
{
    QString block = ui->req_block->text();
    QString room = ui->req_room->text();
    QString faculty = ui->req_faculty->text();
    QString year = ui->req_year->text();
    QString semester = ui->req_semester->text();
    QString start = ui->req_start->text();
    QString end = ui->req_end->text();
    QString professor = ui->req_professor->text();
    QString subject = ui->req_subject->text();
    QString req_day = ui->req_day->text();

    reservationsOpen();

    QSqlQuery qry;
    qry.prepare("select * from request where day='"+req_day+"' and block = '"+block+"' and room = '"+room+"' and faculty = '"+faculty+"' and year = '"+year+"' and semester = '"+semester+"' and start = '"+start+"' and end = '"+end+"' and professor = '"+professor+"' and subject = '"+subject+"'");
    bool del_req = qry.exec();
    if(del_req && qry.next()) {
        qry.exec("delete from request where day='"+req_day+"' and block = '"+block+"' and room = '"+room+"' and faculty = '"+faculty+"' and year = '"+year+"' and semester = '"+semester+"' and start = '"+start+"' and end = '"+end+"' and professor = '"+professor+"' and subject = '"+subject+"'");
        QMessageBox::information(this, "Delete", "Successfully deleted request!");
    } else {
        QMessageBox::information(this, "Delete", "Failed to delete request");
    }
    reservationsClose();

    MainWindow::on_load_request_clicked();
}

void MainWindow::on_pushButton_clicked()
{
    QString day = ui->dur_day->currentText();
    QString block = ui->dur_block->currentText();
    QString room = ui->dur_room->text();
    QString prev_start = ui->dur_prev_start->currentText();
    QString prev_end = ui->dur_prev_end->currentText();
    QString new_start = ui->dur_new_start->currentText();
    QString new_end = ui->dur_new_end->currentText();
    QString status = "Reserved";

    reservationsOpen();
    QSqlQuery qry;
    qry.prepare("select * from '"+day+"' where block='"+block+"' and room = '"+room+"' and start='"+prev_start+"' and end='"+prev_end+"'");
    bool class_exists = qry.exec();
    if(class_exists && qry.next()) {
        QString faculty = qry.value(5).toString();
        QString year = qry.value(6).toString();
        QString semester = qry.value(7).toString();
        QString professor = qry.value(8).toString();
        QString subject = qry.value(9).toString();
        qry.prepare("select start,end from '"+day+"' where block='"+block+"' and room='"+room+"'");   //prepare a query to search the reserved room from the database
        if(qry.exec()) {
            int check_count = 1;
            while(qry.next()) {
                qDebug() << "(" << check_count++ << ")" << "Checking...";
                int a_start = qry.value(0).toInt();
                int a_end = qry.value(1).toInt();
                if(a_start == prev_start.toInt() && a_end == prev_end.toInt())
                    continue;
                if(room_available(a_start, a_end, new_start.toInt(), new_end.toInt())) {
                    continue;
                } else {
                    QMessageBox::information(this, "Duration", "Failed to change duration the of class.");
                    return;
                }
            }
            //if room is available
            qry.exec("delete from '"+day+"' where block='"+block+"' and room='"+room+"' and start='"+prev_start+"' and end='"+prev_end+"'");
            qry.prepare("insert into '"+day+"' (block,room,start,end,status,faculty,year,semester,professor,subject) values ('"+block+"','"+room+"','"+new_start+"','"+new_end+"','"+status+"','"+faculty+"','"+year+"','"+semester+"','"+professor+"','"+subject+"')");
            if(qry.exec()) {
                qDebug() << "Room available. Writing to the database.";
            } else qDebug() << "Couldn't execute the query to book the room!";
        }
    } else {
        QMessageBox::information(this, "Duration", "Failed to change duration the of class.");
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    clear_data cd;
    cd.setModal(true);
    cd.exec();
}


void MainWindow::on_delete_student_clicked()
{
    delete_student ds;
      ds.setModal(true);
      ds.exec();
}


void MainWindow::on_add_faculty_clicked()
{
    add_faculty af;
    af.setModal(true);
    af.exec();
}


void MainWindow::on_add_dept_clicked()
{
    add_dept ad;
    ad.setModal(true);
    ad.exec();
}

