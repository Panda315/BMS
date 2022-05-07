#include "clear_data.h"
#include "ui_clear_data.h"
#include <QMessageBox>

clear_data::clear_data(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::clear_data)
{
    ui->setupUi(this);
}

clear_data::~clear_data()
{
    delete ui;
}




void clear_data::on_clear_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this,"Confirmation","Do you want to clear data of selected items ?",QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
       if(reply==QMessageBox::Yes)
       {
           if((ui->clear_all->isChecked())||(ui->clear_professors->isChecked())||(ui->clear_students->isChecked())||(ui->clear_classes->isChecked()))
           {
               collegedbOpen();
               QSqlQuery qry;
               if(ui->clear_all->isChecked())
               {
                   qry.prepare("delete from professor");
                   if(qry.exec())
                   {
                       qry.prepare("delete from student");
                       if(qry.exec())
                       {
                           collegedbClose();
                           reservationsOpen();
                           QSqlQuery qry;
                           qry.prepare("delete from Sunday");
                           if(qry.exec())
                           {
                               qry.prepare("delete from Monday");
                               if(qry.exec())
                               {
                                   qry.prepare("delete from Tuesday");
                                   if(qry.exec())
                                   {
                                       qry.prepare("delete from Wednesday");
                                       if(qry.exec())
                                       {
                                           qry.prepare("delete from Thursday");
                                           if(qry.exec())
                                           {
                                               qry.prepare("delete from Friday");
                                               if(qry.exec())
                                               {
                                                   reservationsClose();
                                                   QMessageBox::information(this,"Deleted","Data Deleted");
                                               }
                                           }
                                       }
                                   }
                               }
                           }
                       }
                     }
                   this->close();
                   return ;
                   }


               collegedbOpen();
               if(ui->clear_professors->isChecked())
               {
                   QSqlQuery qry;
                   qry.prepare("delete from professor");
                   if(qry.exec())
                   {
                       qDebug() << "Deleted datas from professor table";
                   }

               }

               if(ui->clear_students->isChecked())
               {
                   QSqlQuery qry;
                   qry.prepare("delete from student");
                   if(qry.exec())
                   {
                       qDebug() << "Deleted data from student table";
                   }
               }

               if(ui->clear_classes->isChecked())
               {
                   collegedbClose();
                   reservationsOpen();
                   QSqlQuery qry;
                   qry.prepare("delete from Sunday");
                   if(qry.exec())
                   {
                       qry.prepare("delete from Monday");
                       if(qry.exec())
                       {
                           qry.prepare("delete from Tuesday");
                           if(qry.exec())
                           {
                               qry.prepare("delete from Wednesday");
                               if(qry.exec())
                               {
                                   qry.prepare("delete from Thursday");
                                   if(qry.exec())
                                   {
                                       qry.prepare("delete from Friday");
                                       if(qry.exec())
                                       {
                                           reservationsClose();
                                           QMessageBox::information(this,"Deleted","Data Deleted");
                                           this->close();
                                           return;
                                       }
                                   }
                               }
                           }
                       }
                   }
               }

               collegedbClose();
               QMessageBox::information(this,"Complete","Data Deleted");
               this->close();
           }

           else
           {
               QMessageBox::warning(this,"select","Select at least one !!!");
           }
       }
       else
       {
           QMessageBox::information(this,"Cancelled","The process was terminated");
       }

}

