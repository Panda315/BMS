#include "view_professor.h"
#include "ui_view_professor.h"
#include <QDebug>
#include <QtSql>

view_professor::view_professor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::view_professor)
{
    ui->setupUi(this);
}

view_professor::~view_professor()
{
    delete ui;
}




void view_professor::on_vp_load_clicked()
{
    collegedbOpen();
    QSqlQuery qry;
    QSqlQueryModel *modal = new QSqlQueryModel();

    qry.prepare("select name,dept,username from professor");
    if(qry.exec())
    {
        qDebug() << "Sucessfull";
        modal->setQuery(qry);
        ui->vp_table->setModel(modal);
    }

    collegedbClose();
}

