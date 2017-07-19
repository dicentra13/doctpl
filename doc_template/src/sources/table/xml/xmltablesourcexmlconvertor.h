#ifndef XMLTABLESOURCEXMLCONVERTOR_H
#define XMLTABLESOURCEXMLCONVERTOR_H

#include "../tablesourcexmlconvertor.h"

#include <QStringList>
#include <QString>

class XMLTableSource;
class QDomElement;
class XMLConvertor;
class SourceNameValidator;

class XMLTableSourceXMLConvertor : public TableSourceXMLConvertor {
public:
    XMLTableSourceXMLConvertor();
    XMLTableSourceXMLConvertor(const XMLTableSource *source);
    virtual void writeSourceData(QDomElement *sourceElement,
        XMLConvertor *convertor) const;
    virtual std::unique_ptr<TableSource> processSourceData(
        const QDomElement &sourceElement, XMLConvertor *convertor,
        SourceNameValidator validator);
private:
    QStringList processQueriesElement(const QDomElement &queriesElement,
        XMLConvertor *convertor);
    QString processXPassElement(const QDomElement &xpassElement,
        XMLConvertor *convertor);

    const XMLTableSource *sourceToWrite_;
};

#endif // XMLTABLESOURCEXMLCONVERTOR_H
