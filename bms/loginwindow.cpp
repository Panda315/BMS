#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <QMessageBox>
#include "professorwindow.h"
#include"ui_professorwindow.h"
#include"studentwindow.h"
#include"ui_studentwindow.h"


LogInWindow::LogInWindow(QWidget *parent, QString username)
    : QMainWindow(parent)
    , ui(new Ui::LogInWindow)
{
    ui->setupUi(this);
    setWindowTitle("Login");
    setWindowIcon(QIcon(":/data/icon.png"));



}

LogInWindow::~LogInWindow()
{
    delete ui;
}

void LogInWindow::on_pushButton_clicked()

{
    QString Username = ui->lineEdit_username->text();
    QString Password = ui->lineEdit_password->text();
    collegedbOpen();
    QSqlQuery qry;
    qry.prepare("select * from professor where username='"+Username+"' and pwd='"+Password+"'");
    bool p_data=qry.exec("select * from professor where username='"+Username+"' and pwd='"+Password+"'");
    bool professor=p_data && qry.next();
    qry.prepare("select * from student where username='"+Username+"' and pwd='"+Password+"'");
    bool s_data=qry.exec("select *from student where username='"+Username+"' and pwd='"+Password+"'");
    bool student =s_data && qry.next();
    if (Username == "test" && Password == "test"){
        this->hide();
        mainwindow = new MainWindow(this);
        mainwindow->show();
    }
    else if(professor) {
             this->hide();
             professorwindow Professorwindow;
            Professorwindow.setModal(true);
            Professorwindow.exec();
    }

   else if(student) {
             this->hide();
             studentwindow Studentwindow;
            Studentwindow.setModal(true);
            Studentwindow.exec();
    }
    else {
        QMessageBox::information(this, "login", "Invalid Username or Password");
    }
    collegedbClose();
}


void LogInWindow::on_pushButton_2_clicked()
{
    QApplication::quit();
}

