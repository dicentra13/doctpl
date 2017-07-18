#include "template_editor.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TemplateEditor w;
    w.show();

    return a.exec();
}
