#include "professorwindow.h"
#include "ui_professorwindow.h"
#include <QIODevice>
#include<QMessageBox>
#include <QFileDialog>

QString professor;
professorwindow::professorwindow(QWidget *parent, QString prof_username) :
    QDialog(parent),
    ui(new Ui::professorwindow)
{
    ui->setupUi(this);
    QFile faculty(":/resources/data/faculty.txt");
    QTextStream faculty_stream(&faculty);
    QString line;
    if (faculty.open(QFile::ReadOnly | QFile::Text)) {
       while (!faculty_stream.atEnd())
       {
          line = faculty_stream.readLine();
          ui->req_faculty->addItem(line);
          ui->pw_faculty->addItem(line);
       }
       faculty_stream.flush(); faculty.close();
    }

    collegedbOpen();

    QSqlQuery qry;
    qry.prepare("select name from professor where username='"+prof_username+"'");
    qry.exec();
    while(qry.next()) {
        professor = qry.value(0).toString();
    }

    qry.prepare("select * from block");
    if(qry.exec()) {
        while(qry.next()) {
            ui->pw_block->addItem(qry.value(0).toString());
            ui->req_block->addItem(qry.value(0).toString());
        }
    }

    collegedbClose();
}

professorwindow::~professorwindow()
{
    delete ui;
}
bool room_available2(int a_start, int a_end, int start, int end) {
    if(a_start == start || a_end == end) return false;
    if(start < a_start && end > a_start) return false;
    if(start > a_start && start < a_end) return false;
    return true;
}

void professorwindow::on_pw_show_2_clicked()
{
    QString day = ui->pw_day->currentText();
    QString faculty = ui->pw_faculty->currentText();
    QString year = ui->pw_year->currentText();
    QString semester = ui->pw_semester->currentText();

    reservationsOpen();

    QSqlQueryModel * routine_modal = new QSqlQueryModel();
    QSqlQuery * qry = new QSqlQuery(reservations);

    qry->prepare("select start,end,subject from '"+day+"' where faculty = '"+faculty+"' and year = '"+year+"' and semester ='"+semester+"'");
    qry->exec();
    routine_modal->setQuery(*qry);
    routine_modal->setHeaderData(0, Qt::Horizontal, QObject::tr("Start"));
    routine_modal->setHeaderData(1, Qt::Horizontal, QObject::tr("End"));
    routine_modal->setHeaderData(2, Qt::Horizontal, QObject::tr("Subject"));

    ui->pw_table->setModel(routine_modal);
    ui->pw_table->horizontalHeader()->setStretchLastSection(true);
    ui->pw_table->setColumnWidth(5, 240);
    ui->pw_table->setColumnWidth(8, 240);
    ui->pw_table->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->pw_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    reservationsClose();
}


void professorwindow::on_pw_show_clicked()
{
    QString block = ui->pw_block->currentText();
    QString room = ui->pw_room->text();
    QString day = ui->pw_day->currentText();
    QString start = ui->pw_start_time->currentText();
    QString end = ui->pw_end_time->currentText();

    bool available  = true;
    reservationsOpen();
    QSqlQuery qry;

    qry.prepare("select start,end from '"+day+"' where block='"+block+"' and room= '"+room+"'" );
                if(qry.exec()){
                       int check_count =1;
                       while (qry.next()){
                           qDebug()<<"(" << check_count++ << ")" <<"Checking...";
                           int a_start= qry.value(0).toInt();
                           int a_end= qry.value(1).toInt();
                           if (room_available2(a_start,a_end,start.toInt(), end.toInt())) {
                               continue;
                           } else{
                             available = false;
                               break;
                           }
                       }
                      if (available){
                          QMessageBox::information(this, "class","Class is available");
                      }else{
                          QMessageBox::information(this, "class","Class is not available");
                      } }else {

                          qDebug()<<"Failed to execute the query!";
                      }
                      reservationsClose();

}


void professorwindow::on_request_button_clicked()
{
    QString block = ui->req_block->currentText();
    QString room = ui->req_room->text();
    QString day = ui->req_day->currentText();
    QString start = ui->req_start_time->currentText();
    QString end = ui->req_end_time->currentText();
    QString faculty = ui->req_faculty->currentText();
    QString year = ui->req_year->currentText();
    QString semester = ui->req_semester->currentText();
    QString subject = ui->req_subject->text();
    reservationsOpen();
    QSqlQuery qry;
    qry.prepare("insert into request (faculty,year,semester,day,start,end,block,room,subject,professor) values ('"+faculty+"','"+year+"','"+semester+"','"+day+"','"+start+"','"+end+"','"+block+"','"+room+"','"+subject+"','"+professor+"')");
    if(qry.exec()) {
        QMessageBox::information(this, "Request", "Successfully requested a classroom");
    }
    reservationsClose();
}

