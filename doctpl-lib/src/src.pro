include(../defaults.pri)

TARGET = doctpl
TEMPLATE = lib

DEFINES += DOCTPLLIB_LIBRARY

SOURCES += layout.cpp \
    layout_item.cpp \
    text_item.cpp \
    formatting_settings.cpp \
    field.cpp \
    page.cpp \
    text_field.cpp \
    table_field.cpp \
    table_field_impl.cpp \
    template.cpp \
    serialize/serialize.cpp \
    serialize/xml_helpers.cpp \
    serialize/xml_text_field_factory.cpp \
    serialize/xml_table_field_factory.cpp \
    settings.cpp \
    style_options.cpp

HEADERS += \
    include/doctpl/layout.h \
    include/doctpl/layout_item.h \
    include/doctpl/page.h \
    include/doctpl/field.h \
    include/doctpl/text_field.h \
    include/doctpl/formatting_settings.h \
    text_item.h \
    include/doctpl/table_field.h \
    table_field_impl.h \
    include/doctpl/fields_collection.h \
    include/doctpl/template.h \
    include/doctpl/exception.h \
    include/doctpl/serialize.h \
    serialize/xml_helpers.h \
    serialize/xml_magic_strings.h \
    serialize/xml_text_field_factory.h \
    serialize/xml_table_field_factory.h \
    include/doctpl/settings.h \
    include/doctpl/style_options.h

OTHER_FILES += \
    todo.txt
