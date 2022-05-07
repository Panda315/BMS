#ifndef STUDENTWINDOW_H
#define STUDENTWINDOW_H

#include <QDialog>
#include <QMainWindow>
#include <QtSql>
#include <QFile>
#include <string>
namespace Ui {
class studentwindow;
}

class studentwindow : public QDialog
{
    Q_OBJECT

    QSqlDatabase reservations;
    QSqlDatabase collegedb;

    //to open the database : reservations.db
    bool reservationsOpen() {
        QString path = "/Users/panda/Documents/project-official/myrepo/bms/data/reservations.db";
        reservations = QSqlDatabase::addDatabase("QSQLITE");
        reservations.setDatabaseName(path);

        if(!reservations.open()) {
            qDebug() << "Failed to connect to reservations.db!";
            return false;
        } else {
            qDebug() << "Successfully connected to reservations.db!";
            return true;
        }
    };

    //to close the database : reservations.db
    void reservationsClose() {
        reservations.close();
        reservations.removeDatabase("connectionName");
    };
public:
    explicit studentwindow(QWidget *parent = nullptr);
    ~studentwindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::studentwindow *ui;
};

#endif // STUDENTWINDOW_H
