#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include "mainwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class LogInWindow; }
QT_END_NAMESPACE

class LogInWindow : public QMainWindow
{
    Q_OBJECT
    QSqlDatabase collegedb;
    bool collegedbOpen() {
        QString path = "/Users/panda/Documents/project-official/myrepo/bms/data/reservations.db";
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
    LogInWindow(QWidget *parent = nullptr, QString username = nullptr);
    ~LogInWindow();

private slots:
void on_pushButton_clicked();

void on_pushButton_2_clicked();

private:
    Ui::LogInWindow *ui;
    MainWindow *mainwindow;
};
#endif // LOGINWINDOW_H
