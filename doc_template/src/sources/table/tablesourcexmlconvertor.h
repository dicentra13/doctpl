#ifndef TABLESOURCEXMLCONVERTOR_H
#define TABLESOURCEXMLCONVERTOR_H

#include "../sourcexmlconvertor.h"

#include <memory>

class QDomElement;
class XMLConvertor;
class SourceNameValidator;
class TableSource;

class TableSourceXMLConvertor : public SourceXMLConvertor {
public:
    virtual void writeSourceData(QDomElement *sourceElement,
        XMLConvertor *convertor) const = 0;
    virtual std::unique_ptr<TableSource> processSourceData(
        const QDomElement &sourceElement, XMLConvertor *convertor,
        SourceNameValidator validator) = 0;
protected:
    TableSourceXMLConvertor() {}
};

#endif // TABLESOURCEXMLCONVERTOR_H
