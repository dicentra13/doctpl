#ifndef FIELDXMLCONVERTOR_H
#define FIELDXMLCONVERTOR_H

#include <QString>
#include <memory>

class Field;
class TemplateLayout;
class XMLConvertor;

class QDomElement;

class FieldXMLConvertor {
public:
    std::unique_ptr<Field> processFieldElement(size_t page, const QDomElement &fieldElement);
    void writeFieldElement(const QString &fieldName, QDomElement *fieldElement);
    virtual ~FieldXMLConvertor() {};
    QString name() const { return name_;}
    QString type() const { return type_;}
    double x() const { return x_;}
    double y() const { return y_;}
    double width() const { return width_;}
    double height() const { return height_;}

protected:
    FieldXMLConvertor(TemplateLayout *layout, XMLConvertor *convertor);

    virtual std::unique_ptr<Field> processSpecificElement(size_t page,
        const QDomElement &specificElement) = 0;
    virtual void writeSpecificElement(const QString &fieldName,
        QDomElement *fieldElement) = 0;

    void processCommonElement(const QDomElement &commonElement);
    void processPosElement(const QDomElement &posElement);
    void processSizeElement(const QDomElement &sizeElement);

    void writeCommonElement(Field *field, QDomElement *commonElement);

    QString name_;
    QString type_;
    double x_, y_, width_, height_;
    TemplateLayout *layout_;
    XMLConvertor *convertor_;
};

#endif // FIELDXMLCONVERTOR_H
