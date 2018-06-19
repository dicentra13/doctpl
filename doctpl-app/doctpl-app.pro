QT += core gui \
    xml \
    xmlpatterns \
#    opengl \
    widgets \
    printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CXXFLAGS += -std=c++11

TARGET = doctpl-app
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += \
    src \
    ../doctpl-lib/src/include \
    $(BOOST_PATH)

SOURCES += \
    src/main.cpp \
    src/template_editor.cpp \
    src/view/view_impl.cpp \
    src/view/view.cpp \
    src/view/fit_width_view_impl.cpp \
    src/page_toolbar.cpp \
    src/view/fit_page_view_impl.cpp \
    src/page_dialog.cpp \
    src/edit_widgets.cpp \
    src/settings.cpp

HEADERS += \
    src/template_editor.h \
    src/view/view_impl.h \
    src/view/view.h \
    src/view/fit_width_view_impl.h \
    src/page_toolbar.h \
    src/view/fit_page_view_impl.h \
    src/page_dialog.h \
    src/edit_widgets.h \
    src/settings.h

TRANSLATIONS += \
    interface_ru.ts \
    interface_en.ts

RESOURCES += \
    doctpl-app.qrc

win32:LIBS += \
    C:/projects/doctpl/build-doctpl-lib/src/debug/doctpl.dll

DISTFILES += \
    todo.txt
