#include "block.h"
#include "ui_block.h"
#include <QMessageBox>
#include <QtWidgets>
block::block(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::block)
{
    ui->setupUi(this);
}

block::~block()
{
    delete ui;
}

void block::on_block_add_clicked()
{
    QString new_block = ui->block_new->text();

    collegedbOpen();
    QSqlQuery qry;
    qry.prepare("select * from block where block_no='"+new_block+"'");
    if(qry.exec())
    {
        int count=0;
        while(qry.next())
        {
            count++;
        }

        if(count==0)
        {
            qry.prepare("insert into block (block_no) values ('"+new_block+"')");
            if(qry.exec())
            {
                QMessageBox::information(this,"Success","Block has been Sucessfully Added");
            }
        }

        else if(count>0)
        {
            QMessageBox::warning(this,"warning","Block already exist");
        }
    }

    collegedbClose();

    ui->block_new->clear();
}


void block::on_block_exit_clicked()
{

    this->hide();
}





