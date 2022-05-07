#ifndef VIEW_PROFESSOR_H
#define VIEW_PROFESSOR_H
#include <QtSql>
#include <QDialog>

namespace Ui {
class view_professor;
}

class view_professor : public QDialog
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
    explicit view_professor(QWidget *parent = nullptr);
    ~view_professor();

private slots:


    void on_vp_load_clicked();

private:
    Ui::view_professor *ui;
};

#endif // VIEW_PROFESSOR_H
