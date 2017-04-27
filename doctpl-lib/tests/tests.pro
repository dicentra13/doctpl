include(../defaults.pri)

TEMPLATE = app

SOURCES += \
    tests.cpp \
    serialize_test.cpp \
    text_field_test.cpp \
    helpers.cpp \
    table_field_test.cpp \
    layout_test.cpp

#LIBS += \
#    -L../src -ldoctpl -llibboost_unit_test_framework-mgw53-mt-d-1_62.lib

win32:LIBS += \
    "C:/boost/boost_1_62_0/lib/lib/libboost_unit_test_framework-mgw53-mt-d-1_62.dll" \
    "C:/projects/doctpl/build-lib-debug/src/debug/doctpl.dll"

HEADERS += \
    app_fixture.h \
    helpers.h

OTHER_FILES += \
    data/11kl_2016.xml
