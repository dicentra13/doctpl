#include "xmltextsourcexmlconvertor.h"

#include "xmltextsource.h"
#include "../../../xmlconvertor.h"
#include "../../../exception.h"

#include <QDomElement>
#include <QDomNodeList>

XMLTextSourceXMLConvertor::XMLTextSourceXMLConvertor()
    : sourceToWrite_(0)
{}

XMLTextSourceXMLConvertor::XMLTextSourceXMLConvertor(
        const XMLTextSource *source)
    : sourceToWrite_(source)
{}

void XMLTextSourceXMLConvertor::writeSourceData(QDomElement *sourceElement,
    XMLConvertor */*convertor*/) const
{
    if (!sourceToWrite_) {
        THROW_EX(Exception, "XMLTextSourceConvertor was not constructed "
             << "for writing source data")
    }
    sourceElement->setAttribute("name", sourceToWrite_->name());
    sourceElement->setAttribute("type", "text");
    sourceElement->setAttribute("subtype", "xml");
    QDomElement xmlElement =
        sourceElement->ownerDocument().createElement("xml_text");
    xmlElement.setAttribute("filename", sourceToWrite_->fileName());
    QDomElement queryElement =
        xmlElement.ownerDocument().createElement("xpass");
    QDomText text = queryElement.ownerDocument().createTextNode(
        sourceToWrite_->query());
    queryElement.appendChild(text);
    xmlElement.appendChild(queryElement);
    sourceElement->appendChild(xmlElement);
}

std::unique_ptr<TextSource> XMLTextSourceXMLConvertor::processSourceData(
    const QDomElement &sourceElement, XMLConvertor *convertor,
    SourceNameValidator validator)
{
    QString name = convertor->getString(sourceElement, "name");
    if (!validator.checkNameIsAcceptable(name)) {
        THROW_XML_EX(XMLParseError, sourceElement.lineNumber(),
            sourceElement.columnNumber(),
            "Source with name " << name.toStdString() << " already exists")
    }
    QDomElement xmlElement = convertor->getSingleChild(sourceElement,
        "xml_text");
    QString fileName = convertor->getString(xmlElement, "filename");
    QDomElement xpassElement = convertor->getSingleChild(xmlElement, "xpass");
    if (xpassElement.childNodes().size() != 1 ||
        !xpassElement.childNodes().item(0).isText())
    {
        THROW_XML_EX(XMLParseError, xpassElement.lineNumber(),
            xpassElement.columnNumber(),
            "Only XPass query text must be specified")
    }
    if (xpassElement.childNodes().item(0).toText().data().isEmpty())
    {
        THROW_XML_EX(XMLParseError, xpassElement.lineNumber(),
            xpassElement.columnNumber(),
            "Empty query specified")
    }
    QString query = xpassElement.childNodes().item(0).toText().data();

    std::unique_ptr<XMLTextSource> newSource(new XMLTextSource(name, fileName));
    newSource->setQuery(query);

    return std::unique_ptr<TextSource>(newSource.release());
}
