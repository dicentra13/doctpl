#include <QApplication>
#include <QCoreApplication>
#include <QTranslator>

#include <QString>
#include <QStringList>
#include <QFile>
#include <QTextStream>

#include <vector>
#include <memory>
#include <iostream>

#include "template/templateeditor.h"
#include "exception.h"
#include "documenttemplate.h"
#include "xmlconvertor.h"

#include "sources/table/xml/xmltablesource.h"
#include "sources/text/xml/xmltextsource.h"

#include <qgl.h>

struct DocumentData {
public:
    QString ID;
    QString filename;
};

int runPrintingMode(int argc, char *argv[]);
int runEditingMode(int argc, char *argv[]);
std::vector<DocumentData> readConfig(const char *filename);

int main(int argc, char *argv[]) {

    if (argc >= 2) {
        return runPrintingMode(argc, argv);
    } else {
        return runEditingMode(argc, argv);
    }
}

int runPrintingMode(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QTranslator qtTranslator, ifTranslator;
    if (qtTranslator.load("qt_ru", "/usr/share/qt4/translations")) {
        app.installTranslator(&qtTranslator);
    }
    if (ifTranslator.load("interface_ru", ":/attestate/ru")) {
        app.installTranslator(&ifTranslator);
    }
    app.setOrganizationName("");
    app.setApplicationName("DocumentTemplator");

    std::vector<DocumentData> data = readConfig(argv[1]);
    XMLTableSource::setDirectory(argv[2]);
    XMLTextSource::setDirectory(argv[2]);

    std::cerr << argv[3] << std::endl;
    QString filename(argv[3]);
    std::unique_ptr<DocumentTemplate> doc(new DocumentTemplate(filename, 0));

    XMLConvertor conv;

    try {
        conv.readFromFile(doc.get(), argv[3]);

    } catch (XMLParseError &err) {
        std::stringstream message;
        message << "LINE" << " " << err.xmlLine() << " "
                << "COLUMN" << " " << err.xmlSymbol()
                << ": " << err.cause();
        THROW_EX(Exception, "XML_PARSING_ERROR" <<
            message.str().c_str());
    } catch (Exception &err) {
        THROW_EX(Exception, "XML_PARSING_ERROR" <<
            err.cause().c_str());
    }

    for (size_t i = 0; i < data.size(); i++) {
        std::vector<size_t> pages;
        for (size_t j = 1; j <= doc.get()->numberOfPages(); j++)
            pages.push_back(j);
        doc.get()->printToFile(data[i].ID, data[i].filename, pages);
    }

    return 0;
}

int runEditingMode(int argc, char *argv[]) {

    //QGL::setPreferredPaintEngine(QPaintEngine::OpenGL);
    QApplication app(argc, argv);

    QTranslator qtTranslator, ifTranslator;
    if (qtTranslator.load("qt_ru", "/usr/share/qt4/translations")) {
        app.installTranslator(&qtTranslator);
    }
    if (ifTranslator.load("interface_ru", ":/attestate/ru")) {
        app.installTranslator(&ifTranslator);
    }
    app.setOrganizationName("");
    app.setApplicationName("DocumentTemplator");

    TemplateEditor mainWin;
    mainWin.show();

    return app.exec();
}

std::vector<DocumentData> readConfig(const char *filename) {
    QFile config(filename);
    if (!config.open(QIODevice::ReadOnly)) {
        THROW_EX(Exception, "Could not open configuration file " <<
             filename << "\n")
    }
    std::vector<DocumentData> result;
    QTextStream stream(&config);
    QString line = stream.readLine();
    int i = 1;
    while (!line.isNull()) {
        QStringList separated = line.split(';');
        if (separated.size() != 2) {
            config.close();
            THROW_EX(Exception, "Incorrect configuration file line number "
                << i << ": must be " << 2 << " comma separated values\n")
        }
        DocumentData documentData;
        documentData.ID = separated.at(0);
        documentData.filename = separated.at(1);
        result.push_back(documentData);
        line = stream.readLine();
        i++;
    }
    config.close();
    return result;
}
