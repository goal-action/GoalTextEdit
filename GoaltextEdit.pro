QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    GoalTextEdit.cpp \
    gtfile.cpp \
    gtmainmenu.cpp \
    gtproject.cpp \
    gtsidebar.cpp \
    gtstackedtextedits.cpp \
    gtstatusbar.cpp \
    gttextdarkhighlighter.cpp \
    gttextlighthighlighter.cpp \
    main.cpp

HEADERS += \
    GoalTextEdit.h \
    gtdialogs.h \
    gtfile.h \
    gtmainmenu.h \
    gtproject.h \
    gtsidebar.h \
    gtstackedtextedits.h \
    gtstatusbar.h \
    gttextdarkhighlighter.h \
    gttextlighthighlighter.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

DISTFILES +=

RC_FILE = logo.rc
