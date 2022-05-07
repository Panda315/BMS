#include "add_dept.h"
#include "ui_add_dept.h"
#include <QMessageBox>

add_dept::add_dept(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::add_dept)
{
    ui->setupUi(this);
}

add_dept::~add_dept()
{
    delete ui;
}

void add_dept::on_add_clicked()
{
    QString name = "Department of " + ui->dept_name->text();


    if(ui->dept_name->text().isEmpty())
    {
        QMessageBox::warning(this,"incomplete","Enter name of the department");
    }

    else
    {
        QMessageBox::StandardButton reply = QMessageBox::question(this,"Confirmation","Do you want to delete the user information ?",QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
        if(reply==QMessageBox::Yes)
        {
            collegedbOpen();
            QSqlQuery qry;
            qry.prepare("insert into Department(dept) values ('"+name+"')");
            if(qry.exec())
            {
                QMessageBox::information(this,"sucessful","Successfully added Department");
                collegedbClose();
                ui->dept_name->clear();
            }
        }

        else
        {
            QMessageBox::information(this,"termination","Process Terminated");
        }
    }

}

