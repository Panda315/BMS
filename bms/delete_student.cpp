#include "delete_student.h"
#include "ui_delete_student.h"
#include <QMessageBox>

delete_student::delete_student(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::delete_student)
{
    ui->setupUi(this);
}

delete_student::~delete_student()
{
    delete ui;
}

void delete_student::on_delete_2_clicked()
{
    QString username = ui->username->text();
        collegedbOpen();
        QSqlQuery qry;

        QMessageBox::StandardButton reply = QMessageBox::question(this,"Confirmation","Do you want to delete the user " + username +" ?",QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
        if(reply==QMessageBox::Yes)
        {
            qry.prepare("delete from student where username='"+username+"'");
            if(qry.exec())
            {
                QMessageBox::information(this,"success","Account sucessfully deleted");
            }
        }
}

