#include "template_editor.h"

#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTranslator qtTranslator, ifTranslator;
    if (qtTranslator.load("qt_ru", "/usr/share/qt4/translations")) {
        app.installTranslator(&qtTranslator);
    }
    if (ifTranslator.load("interface_ru", "c:/projects/doctpl/doctpl-app")) {
        app.installTranslator(&ifTranslator);
    }

    TemplateEditor w;
    w.show();

    return app.exec();
}
