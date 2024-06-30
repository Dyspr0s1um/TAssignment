QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

win32: {
    QT += winextras
    LIBS += -lDwmapi
}

CONFIG += c++17

# 设置对象文件输出路径
OBJECTS_DIR = $$OUT_PWD/temp/obj

# 设置中间文件路径（如 moc 文件）
MOC_DIR = $$OUT_PWD/temp/moc

# 设置临时文件路径（如编译器临时文件）
TEMP_DIR = $$OUT_PWD/temp/tmp

SOURCES += \
    GlobalSetting.cpp \
    GlobalTimerTask.cpp \
    MuCursorPosCalculator.cpp \
    MuCustomWindow.cpp \
    MuFramelessHelper.cpp \
    MuShadowWidget.cpp \
    MuTitleBar.cpp \
    MuWidgetData.cpp \
    MuWinDWMAPI.cpp \
    MuWinTitlebar.cpp \
    MuWinWindow.cpp \
    SqlObject.cpp \
    abstracttablemodel.cpp \
    createroomdialog.cpp \
    curriculam.cpp \
    desktoppet.cpp \
    forussettingdialog.cpp \
    main.cpp \
    roomwindow.cpp \
    task_table.cpp \
    task_tableRepository.cpp \
    tasktablemodel.cpp \
    taskwnd.cpp \
    techloginwnd.cpp \
    user_table.cpp \
    user_tableRepository.cpp \
    userinfownd.cpp \
    userlistwnd.cpp \
    userregwnd.cpp \
    userscorelabel.cpp \
    usertablemodel.cpp \
    widget.cpp

HEADERS += \
    GlobalSetting.h \
    GlobalTimerTask.h \
    ModelManager.h \
    MuCursorPosCalculator.h \
    MuCustomWindow.h \
    MuFramelessHelper.h \
    MuFramelessHelperPrivate.h \
    MuShadowWidget.h \
    MuShadowWindow.h \
    MuTitleBar.h \
    MuWidgetData.h \
    MuWinDWMAPI.h \
    MuWinTitlebar.h \
    MuWinWindow.h \
    Singleholder.h \
    SqlObject.h \
    abstracttablemodel.h \
    createroomdialog.h \
    curriculam.h \
    desktoppet.h \
    forussettingdialog.h \
    roomwindow.h \
    task_table.h \
    task_tableRepository.h \
    tasktablemodel.h \
    taskwnd.h \
    techloginwnd.h \
    user_table.h \
    user_tableRepository.h \
    userinfownd.h \
    userlistwnd.h \
    userregwnd.h \
    userscorelabel.h \
    usertablemodel.h \
    widget.h

FORMS += \
    taskwnd.ui \
    userinfownd.ui \
    userlistwnd.ui \
    userregwnd.ui \
    userscorelabel.ui

RESOURCES += \
    background.qrc \
    images.qrc \
    pets.qrc \
    photos.qrc
