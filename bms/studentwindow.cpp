#include "studentwindow.h"
#include "ui_studentwindow.h"

studentwindow::studentwindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::studentwindow)
{
    ui->setupUi(this);
}

studentwindow::~studentwindow()
{
    delete ui;
}

void studentwindow::on_pushButton_clicked()
{
    QString day = ui->routine_day->currentText();
    QString faculty = ui->routine_faculty->currentText();
    QString year = ui->routine_year->currentText();
    QString semester = ui->routine_semester->currentText();

    reservationsOpen();

    QSqlQueryModel * routine_modal = new QSqlQueryModel();
    QSqlQuery * qry = new QSqlQuery(reservations);

    qry->prepare("select start,end,subject from '"+day+"' where faculty = '"+faculty+"' and year = '"+year+"' and semester ='"+semester+"'");
    qry->exec();
    routine_modal->setQuery(*qry);
    routine_modal->setHeaderData(0, Qt::Horizontal, QObject::tr("Start"));
    routine_modal->setHeaderData(1, Qt::Horizontal, QObject::tr("End"));
    routine_modal->setHeaderData(2, Qt::Horizontal, QObject::tr("Subject"));

    ui->routine_table->setModel(routine_modal);
    ui->routine_table->horizontalHeader()->setStretchLastSection(true);
    ui->routine_table->setColumnWidth(5, 240);
    ui->routine_table->setColumnWidth(8, 240);
    ui->routine_table->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->routine_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    reservationsClose();
}

