#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QFile>
#include <string>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
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

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_reserve_button_clicked();

    void on_switch_button_clicked();

    void on_ps_show_clicked();

    void on_cs_show_clicked();

    void on_ov_show_clicked();

    void on_delete_2_clicked();

    void on_routine_show_clicked();

    void on_add_professor_clicked();

    void on_add_student_clicked();

    void on_add_block_clicked();

    void on_view_professor_clicked();

    void on_view_student_clicked();

    void on_update_student_clicked();

    void on_update_professor_clicked();

    void on_load_request_clicked();

    void on_req_tableview_activated(const QModelIndex &index);

    void on_req_reserve_clicked();

    void on_req_delete_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_delete_student_clicked();

    void on_add_faculty_clicked();

    void on_add_dept_clicked();

private:
    Ui::MainWindow *ui;
};

bool check_reservation(int start, int end);

#endif // MAINWINDOW_H
