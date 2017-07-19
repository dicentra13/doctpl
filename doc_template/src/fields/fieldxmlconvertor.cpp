#include "fieldxmlconvertor.h"

#include "field.h"
#include "../template/templatelayout.h"
#include "../xmlconvertor.h"

#include <QDomElement>
#include <QDomDocument>
#include <QDomNodeList>
#include <QString>

FieldXMLConvertor::FieldXMLConvertor(TemplateLayout *layout,
         XMLConvertor *convertor)
    : x_(0.0)
    , y_(0.0)
    , width_(0.0)
    , height_(0.0)
    , layout_(layout)
    , convertor_(convertor)
{}

std::unique_ptr<Field> FieldXMLConvertor::processFieldElement(size_t page,
    const QDomElement &fieldElement)
{
    name_ = convertor_->getString(fieldElement, "name");
    type_ = convertor_->getString(fieldElement, "type");

    processCommonElement(fieldElement);
    return processSpecificElement(page, fieldElement);
}

void FieldXMLConvertor::writeFieldElement(const QString &fieldName,
    QDomElement *fieldElement)
{
    fieldElement->setTagName("field");
    fieldElement->setAttribute("name", fieldName);
    fieldElement->setAttribute("type", layout_->field(fieldName)->fieldType());

    writeCommonElement(layout_->field(fieldName), fieldElement);
    writeSpecificElement(fieldName, fieldElement);
}

void FieldXMLConvertor::processCommonElement(const QDomElement &commonElement)
{
    processPosElement(convertor_->getSingleChild(commonElement, "pos"));
    processSizeElement(convertor_->getSingleChild(commonElement, "size"));
}

void FieldXMLConvertor::processPosElement(const QDomElement &posElement)
{
    x_ = convertor_->getDouble(posElement, "x");
    y_ = convertor_->getDouble(posElement, "y");
}

void FieldXMLConvertor::processSizeElement(const QDomElement &sizeElement)
{
    width_ = convertor_->getDouble(sizeElement, "width");
    height_ = convertor_->getDouble(sizeElement, "height");
}

void FieldXMLConvertor::writeCommonElement(Field *field,
    QDomElement *commonElement)
{
    QDomElement pos = commonElement->ownerDocument().createElement("pos");
    pos.setAttribute("x", field->x());
    pos.setAttribute("y", field->y());
    commonElement->appendChild(pos);
    QDomElement size = commonElement->ownerDocument().createElement("size");
    size.setAttribute("width", field->width());
    size.setAttribute("height", field->height());
    commonElement->appendChild(size);
}


