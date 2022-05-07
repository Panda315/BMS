#include "add_faculty.h"
#include "ui_add_faculty.h"
#include <QMessageBox>

add_faculty::add_faculty(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::add_faculty)
{
    ui->setupUi(this);
}

add_faculty::~add_faculty()
{
    delete ui;
}

void add_faculty::on_add_clicked()
{
    QString name = ui->faculty_name->text();
    collegedbOpen();
    QSqlQuery qry;

    if(name.isEmpty())
    {
        QMessageBox::warning(this,"incomplete","Provide name of the faculty");
    }
    else
    {
        QMessageBox::StandardButton reply = QMessageBox::question(this,"Confirmation","Do you want to delete the user information ?",QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
        if(reply==QMessageBox::Yes)
        {
            qry.prepare("insert into faculty (Faculty) values ('"+name+"') ");
            if(qry.exec())
            {
                QMessageBox::information(this,"done","Sucessfully Added");
                collegedbClose();
                ui->faculty_name->clear();
            }
        }

        else
        {
            QMessageBox::information(this,"Termination","Process Terminated");
        }
    }
}

