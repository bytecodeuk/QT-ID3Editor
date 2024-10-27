#-------------------------------------------------
#
# Project created by QtCreator 2016-07-20T10:21:31
#
#-------------------------------------------------

#
# Add line below to 'Additional Arguments:' in 'qmake' step in 'Build Steps' in 'Projects'.
# "PATH_CONFIG=1" (without quotes)
#

#contains(PATH_CONFIG,1) {
#    DEFINES += PATH_CONFIG1
##    TAGLIB_DIR = "C:/Dev/3rdparty/GitHub/taglib/taglib-install-static-x64-Debug"
#    TAGLIB_DIR = "C:/Dev/3rdparty/GitHub/taglib/taglib-install-static-x64"
#    message("PATH_CONFIG1")
#} else {
#    DEFINES += PATH_CONFIG2
#    TAGLIB_DIR = "C:/Dev/libs/taglib/taglib-install-static-x64-Debug"
#    message("PATH_CONFIG2")
#}

contains(PATH_CONFIG,x64_Debug) {
    DEFINES += PATH_CONFIG_x64_Debug
    TAGLIB_DIR = "C:/Dev/3rdparty/GitHub/taglib/taglib-install-static-x64-Debug"
    message("PATH_CONFIG_x64_Debug")

} contains(PATH_CONFIG,x64_Release) {
    DEFINES += PATH_CONFIG_x64_Release
    TAGLIB_DIR = "C:/Dev/3rdparty/GitHub/taglib/taglib-install-static-x64"
    message("PATH_CONFIG_x64_Release")

} contains(PATH_CONFIG,x32_Debug) {
    DEFINES += PATH_CONFIG_x32_Debug
    TAGLIB_DIR = "C:/Dev/3rdparty/GitHub/taglib/taglib-install-static"
    message("PATH_CONFIG_x64_Debug")

} contains(PATH_CONFIG,x32_Release) {
    DEFINES += PATH_CONFIG_x32_Release
    TAGLIB_DIR = "C:/Dev/3rdparty/GitHub/taglib/taglib-install-static"
    message("PATH_CONFIG_x32_Release")
} else {
    DEFINES += PATH_CONFIG_ERROR
    TAGLIB_DIR = ""
    message("Error PATH_CONFIG_ERROR")
}

TAGLIB_DIR = "C:/Dev/3rdparty/GitHub/taglib/taglib-install-static-x64"

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ID3Editor
TEMPLATE = app


TAGLIB_INC = $$TAGLIB_DIR/include
TAGLIB_LIB = $$TAGLIB_DIR/lib

DEFINES += TAGLIB_STATIC

INCLUDEPATH += $$TAGLIB_INC
LIBS += $$TAGLIB_LIB/tag_c.lib
LIBS += $$TAGLIB_LIB/tag.lib

SOURCES += main.cpp\
        mainwindow.cpp \
    cfilelistmodel.cpp \
    ccelldelegate.cpp \
    cmp3fileitem.cpp \
    ctableview.cpp \
    cstringutils.cpp

HEADERS  += mainwindow.h \
    cfilelistmodel.h \
    ccelldelegate.h \
    cmp3fileitem.h \
    debughelpers.h \
    ctableview.h \
    tablecolumns.h \
    cstringutils.h

FORMS    += mainwindow.ui
