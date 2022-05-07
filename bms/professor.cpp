#include "professor.h"
#include "ui_professor.h"
#include <QDebug>
#include <QMessageBox>

Professor::Professor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Professor)
{
    ui->setupUi(this);
}

Professor::~Professor()
{
    delete ui;
}

void Professor::on_cp_create_clicked()                  //cp -> create professor ( add professor )
{
    QString name = ui->cp_name->text();
    QString username = ui->cp_username->text();
    QString pwd = ui->cp_pwd->text();
    QString dept = ui->cp_dept->currentText();

    collegedbOpen();
    QSqlQuery qry;

    if(name.isEmpty()||username.isEmpty()||pwd.isEmpty())
    {
        QMessageBox::warning(this,"Incomplete","Incomplete Information");
    }

    else
    {
        qry.prepare("insert into professor (name,dept,username,pwd) values ('"+name+"','"+dept+"','"+username+"','"+pwd+"') ");
        if(qry.exec())
        {
            QMessageBox::information(this,"Success","Succesfully created account");
        }
        else
        {
            QMessageBox::warning(this,"Failure","Can not store the information in the database");
        }
        collegedbClose();

        ui->cp_name->clear();
        ui->cp_username->clear();
        ui->cp_pwd->clear();
    }
}


void Professor::on_cp_exit_clicked()
{
   this->hide();
}


