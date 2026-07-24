# Указываем, что это библиотека
TEMPLATE = lib
CONFIG += shared

# Имя целевой библиотеки
TARGET = StringLib

# Исходные файлы
SOURCES += src/String.cpp

# Заголовочные файлы
HEADERS += include/String.h

# Указываем дополнительные директории, если необходимо
INCLUDEPATH += include
