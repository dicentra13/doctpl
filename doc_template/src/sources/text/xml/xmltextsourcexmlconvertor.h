#ifndef XMLTEXTSOURCEXMLCONVERTOR_H
#define XMLTEXTSOURCEXMLCONVERTOR_H

#include "../textsourcexmlconvertor.h"

#include <QStringList>
#include <QString>

class XMLTextSource;
class QDomElement;
class XMLConvertor;
class SourceNameValidator;

class XMLTextSourceXMLConvertor : public TextSourceXMLConvertor {
public:
    XMLTextSourceXMLConvertor();
    XMLTextSourceXMLConvertor(const XMLTextSource *source);
    virtual void writeSourceData(QDomElement *sourceElement,
        XMLConvertor *convertor) const;
    virtual std::unique_ptr<TextSource> processSourceData(
        const QDomElement &sourceElement, XMLConvertor *convertor,
        SourceNameValidator validator);
private:

    const XMLTextSource *sourceToWrite_;
};

#endif // XMLTEXTSOURCEXMLCONVERTOR_H
