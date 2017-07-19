#ifndef XMLCONVERTOR_H
#define XMLCONVERTOR_H

#include "fields/textrenderinfo.h"
#include "exception.h"

#include <QString>

#include <map>

class QDomElement;
class QFont;
class DocumentTemplate;
class Field;
class Page;
class TextRenderInfo;
class FieldXMLConvertor;

class XMLParseError : public Exception {
public:
    XMLParseError(const std::string& file, size_t file_line,
                  size_t xmlLine, size_t xmlSymbol,
                  const std::string& what)
        : Exception(file, file_line, what, "XMLParseError")
        , xmlLine_(xmlLine)
        , xmlSymbol_(xmlSymbol) {};

    size_t xmlLine() const {
        return xmlLine_;
    }

    size_t xmlSymbol() const {
        return xmlSymbol_;
    }

protected:
    size_t xmlLine_;
    size_t xmlSymbol_;
};

#define THROW_XML_EX(ExType, line, symbol, message) \
do { \
    std::stringstream what; \
    what << message; \
    throw ExType(__FILE__, __LINE__, line, symbol, what.str()); \
} while(true);

class XMLConvertor {
public:
    XMLConvertor() {};
    ~XMLConvertor() {};
    void addFieldConvertor(const QString &fieldType,
        FieldXMLConvertor *convertor);

    bool writeToFile(DocumentTemplate *doc, const QString &fileName);
    void convertToXML(const Page &p, QDomElement *pageElement);
    void convertToXML(const Field &f, QDomElement *fieldElement);
    void convertToXML(const TextRenderInfo &tri, QDomElement *formatElement);
// FIXME кажется тут много можно вынести в protected
    void readFromFile(DocumentTemplate *doc, const QString &fileName);
    void processLayoutElement(DocumentTemplate *doc, const QDomElement &layoutElement);
    void processPageElement(DocumentTemplate *doc, size_t num, const QDomElement &pageElement);
    void processFieldElement(DocumentTemplate *doc, size_t num, const QDomElement &fieldElement);
    TextRenderInfo *processFormattingElement(const QDomElement &formatElement);

    QDomElement getSingleChild(const QDomElement &parent, const QString &childName);
    QString getString(const QDomElement &element, const QString &attributeName);
    double getDouble(const QDomElement &element, const QString &attributeName);
    int getInt(const QDomElement &element, const QString &attributeName);
private:
    QFont processFontElement(const QDomElement &fontElement);
    int processAlignElement(const QDomElement &alignElement);
    Margins processMarginsElement(const QDomElement &marginsElement);
};

#endif // XMLCONVERTOR_H
