#ifndef ADD_DEPT_H
#define ADD_DEPT_H

#include <QDialog>
#include <QtSql>

namespace Ui {
class add_dept;
}

class add_dept : public QDialog
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
    explicit add_dept(QWidget *parent = nullptr);
    ~add_dept();

private slots:
    void on_add_clicked();

private:
    Ui::add_dept *ui;
};

#endif // ADD_DEPT_H
