#include "view_student.h"
#include "ui_view_student.h"
#include <QtSql>

view_student::view_student(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::view_student)
{
    ui->setupUi(this);
}

view_student::~view_student()
{
    delete ui;
}

void view_student::on_vs_load_clicked()
{
    collegedbOpen();
    QSqlQuery qry;
    QSqlQueryModel *modal = new QSqlQueryModel();

    qry.prepare("select name,username,year,semester,faculty from student");
    if(qry.exec())
    {
        modal->setQuery(qry);
        ui->vs_table->setModel(modal);
    }

    collegedbClose();

}



void view_student::on_filter_textChanged(const QString &arg1)
{
    QString name = ui->filter->text();
        collegedbOpen();
        QSqlQuery qry;
        QSqlQueryModel *modal = new QSqlQueryModel();

        qry.prepare("select name,username,year,semester,faculty from student where name like '"+name+"%' ");  //searches letter by letter in the database
        if(qry.exec())
        {
            modal->setQuery(qry);
            ui->vs_table->setModel(modal);
        }

        collegedbClose();
}

