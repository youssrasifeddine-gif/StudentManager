QT += core gui widgets
greaterThan(QT_MAJOR_VERSION, 5): QT += charts

QMAKE_LFLAGS += -mwindows

CONFIG += c++17

TARGET   = StudentManager
TEMPLATE = app

SOURCES += main_gui.cpp src/models/Student.cpp src/models/UndergraduateStudent.cpp src/models/GraduateStudent.cpp src/models/PhDStudent.cpp src/services/StudentManager.cpp src/persistence/PersistenceManager.cpp src/gui/MainWindow.cpp src/gui/StudentDialog.cpp

HEADERS += src/models/Student.h src/models/UndergraduateStudent.h src/models/GraduateStudent.h src/models/PhDStudent.h src/services/StudentManager.h src/persistence/PersistenceManager.h src/exceptions/StudentExceptions.h src/gui/MainWindow.h src/gui/StudentDialog.h

DEFINES += DATA_PATH=\\\"data/students.txt\\\"

QMAKE_CXXFLAGS += -Wall -Wextra -pedantic
