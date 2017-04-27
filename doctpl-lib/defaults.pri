QT += \
    gui \
    widgets \
    xml xmlpatterns \
    printsupport

CONFIG += c++11

INCLUDEPATH += ../src/include \
    $(BOOST_PATH)

QMAKE_CXXFLAGS += -std=c++11


