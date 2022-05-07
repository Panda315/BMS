#include "update_professor.h"
#include "ui_update_professor.h"
#include <QMessageBox>
update_professor::update_professor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::update_professor)
{
    ui->setupUi(this);
}

update_professor::~update_professor()
{
    delete ui;
}


void update_professor::on_up_delete_clicked()
{
    QString name = ui->up_name->text();
    QString username = ui->up_username->text();
    QString dept = ui->up_dept->currentText();

    collegedbOpen();
    QSqlQuery qry;

    QMessageBox::StandardButton reply = QMessageBox::question(this,"Confirmation","Do you want to delete the user information ?",QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
    if(reply==QMessageBox::Yes)
    {
        qry.prepare("select * from professor where name='"+name+"' and username='"+username+"' and dept='"+dept+"' ");
        if(qry.exec())
        {
            int count=0;
            while(qry.next())
            {
                count++;
            }
            if(count>=1)
            {
                qry.prepare("delete from professor where name='"+name+"' and username='"+username+"' and dept='"+dept+"' ");
                if(qry.exec())
                {
                    QMessageBox::information(this,"Success","User Information has been sucessfully deleted");
                }
            }

            if(count==0)
            {
                QMessageBox::warning(this,"Error","User not found");
            }
        }

         else
         {
             QMessageBox::warning(this,"Error","Database Error");
         }
    }
    else
    {
        QMessageBox::information(this,"Termination","Delete process terminated !!!");
    }
    collegedbClose();
    ui->up_name->clear();
    ui->up_username->clear();
}


void update_professor::on_up_exit_clicked()
{
    this->hide();
}

