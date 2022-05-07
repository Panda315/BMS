#include "student.h"
#include "ui_student.h"
#include <QMessageBox>

student::student(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::student)
{
    ui->setupUi(this);
}

student::~student()
{
    delete ui;
}


void student::on_cs_create_clicked()
{
    QString name = ui->as_name->text();
    QString username = ui->as_username->text();
    QString pwd = ui->as_pwd->text();
    QString sem = ui->as_semester->currentText();
    QString year = ui->as_year->currentText();
    QString faculty = ui->as_faculty->text();

    collegedbOpen();
    QSqlQuery qry;

    if(name.isEmpty()||username.isEmpty()||pwd.isEmpty()||faculty.isEmpty())
    {
        QMessageBox::warning(this,"incomplete","Incomplete Information");
    }
     else
    {
         qry.prepare("insert into student (name,username,pwd,year,semester,faculty) values ('"+name+"','"+username+"','"+pwd+"','"+year+"','"+sem+"','"+faculty+"')");
         if(qry.exec())
         {
             QMessageBox::information(this,"Sucess","Sucessfully registered");
         }
         else
         {
             QMessageBox::warning(this,"Error","Could not registered the user");
         }

         collegedbClose();

         ui->as_name->clear();
         ui->as_username->clear();
         ui->as_pwd->clear();
         ui->as_faculty->clear();
    }
}


void student::on_cs_exit_clicked()
{
    this->hide();
}

