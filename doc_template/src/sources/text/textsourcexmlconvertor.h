#ifndef TEXTSOURCEXMLCONVERTOR_H
#define TEXTSOURCEXMLCONVERTOR_H

#include "../sourcexmlconvertor.h"

#include <memory>

class QDomElement;
class XMLConvertor;
class SourceNameValidator;
class TextSource;

class TextSourceXMLConvertor : public SourceXMLConvertor {
public:
    virtual void writeSourceData(QDomElement *sourceElement,
        XMLConvertor *convertor) const = 0;
    virtual std::unique_ptr<TextSource> processSourceData(
        const QDomElement &sourceElement, XMLConvertor *convertor,
        SourceNameValidator validator) = 0;
protected:
    TextSourceXMLConvertor() {}
};

#endif // TEXTSOURCEXMLCONVERTOR_H
