#ifndef SOURCEXMLCONVERTOR_H
#define SOURCEXMLCONVERTOR_H

class QDomElement;
class XMLConvertor;

class SourceXMLConvertor {
public:
    virtual void writeSourceData(QDomElement *sourceElement,
        XMLConvertor *convertor) const = 0;
protected:
    SourceXMLConvertor() {}
};

#endif // SOURCEXMLCONVERTOR_H
