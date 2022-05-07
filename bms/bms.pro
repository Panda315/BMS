QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    add_dept.cpp \
    add_faculty.cpp \
    block.cpp \
    clear_data.cpp \
    delete_student.cpp \
    main.cpp \
    loginwindow.cpp \
    mainwindow.cpp \
    professor.cpp \
    professorwindow.cpp \
    student.cpp \
    studentwindow.cpp \
    update_professor.cpp \
    view_professor.cpp \
    view_student.cpp

HEADERS += \
    add_dept.h \
    add_faculty.h \
    block.h \
    clear_data.h \
    delete_student.h \
    horizontaltab.h \
    loginwindow.h \
    mainwindow.h \
    professor.h \
    professorwindow.h \
    student.h \
    studentwindow.h \
    update_professor.h \
    view_professor.h \
    view_student.h

FORMS += \
    add_dept.ui \
    add_faculty.ui \
    block.ui \
    clear_data.ui \
    delete_student.ui \
    loginwindow.ui \
    mainwindow.ui \
    professor.ui \
    professorwindow.ui \
    student.ui \
    studentwindow.ui \
    update_professor.ui \
    view_professor.ui \
    view_student.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

DISTFILES += \
    data/blocks.csv \
    data/faculty.txt \
    data/professor.txt \
    data/reservations/friday.csv \
    data/reservations/monday.csv \
    data/reservations/saturday.csv \
    data/reservations/sunday.csv \
    data/reservations/sunday.db \
    data/reservations/thursday.csv \
    data/reservations/tuesday.csv \
    data/reservations/wednesday.csv \
    data/sample.txt
