QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH += /media/kate/diskE/QT/LR8/task2/heap_list/include
LIBS += -L/media/kate/diskE/QT/LR8/task2/heap_list/ -lheap_list
# g++ -fPIC -shared -o ../libheap_list.so list_heap.cpp

INCLUDEPATH += /media/kate/diskE/QT/LR8/task2/heap_array/include
LIBS += -L/media/kate/diskE/QT/LR8/task2/heap_array/src/ -larray_heap
# в src g++ -c -o array_heap.o array_heap.cpp -I../include
# ar rcs libarray_heap.a array_heap.o
SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
