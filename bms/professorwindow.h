#ifndef PROFESSORWINDOW_H
#define PROFESSORWINDOW_H
#include <QtSql>
#include <QFile>
#include <QDialog>

namespace Ui {
class professorwindow;
}

class professorwindow : public QDialog
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
    explicit professorwindow(QWidget *parent = nullptr, QString prof_username = nullptr);
    ~professorwindow();

private slots:
    void on_pw_show_2_clicked();

    void on_pw_show_clicked();

    void on_request_button_clicked();

private:
    Ui::professorwindow *ui;
};

#endif // PROFESSORWINDOW_H
