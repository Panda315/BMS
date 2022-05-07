#ifndef CLEAR_DATA_H
#define CLEAR_DATA_H

#include <QDialog>
#include <QtSql>

namespace Ui {
class clear_data;
}

class clear_data : public QDialog
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

    bool collegedbOpen() {
        QString path = "/Users/panda/Documents/project-official/myrepo/bms/data/college.db";
        collegedb = QSqlDatabase::addDatabase("QSQLITE");
        collegedb.setDatabaseName(path);

        if(!collegedb.open()) {
            qDebug() << "Failed to connect to college.db";
            return false;
        } else {
            qDebug() << "Successfully connected to college.db";
            return true;
        }
    }

    void collegedbClose() {
        collegedb.close();
        collegedb.removeDatabase("connectionName");
    }

public:
    explicit clear_data(QWidget *parent = nullptr);
    ~clear_data();

private slots:


    void on_clear_clicked();

private:
    Ui::clear_data *ui;
};

#endif // CLEAR_DATA_H
