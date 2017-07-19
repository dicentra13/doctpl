#include "xmlconvertor.h"

#include "fields/fieldxmlconvertor.h"
#include "fields/fieldcontroller.h"
#include "documenttemplate.h"
#include "template/templatelayout.h"
#include "template/templatesourcesmanager.h"
#include "page.h"
#include "fields/field.h"
#include "fields/textrenderinfo.h"
#include "settings.h"

#include <QDomElement>
#include <QDomDocument>
#include <QDomNodeList>
#include <QTextStream>
#include <QFile>
#include <QFont>

#include <boost/foreach.hpp>

#include <memory>

bool XMLConvertor::writeToFile(DocumentTemplate *doc, const QString &fileName) {
    QFile data(fileName);
    if (data.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream out(&data);
        QDomDocument xmlDoc;
        QDomElement root = xmlDoc.createElement("template");
        xmlDoc.appendChild(root);
        QDomElement settings = xmlDoc.createElement("settings");
        QDomElement formatting = xmlDoc.createElement("formatting");
        convertToXML(*TextRenderInfo::globals(), &formatting);
        settings.appendChild(formatting);
        root.appendChild(settings);
        QDomElement layout = xmlDoc.createElement("layout");
        layout.setTagName("layout");
        for (size_t i = doc->firstPage(); i <= doc->lastPage(); i++) {
            Page *page = doc->layout()->page(i);
            QDomElement pageElement =
                layout.ownerDocument().createElement("page");
            convertToXML(*page, &pageElement);
            layout.appendChild(pageElement);
            const std::list<Field *> &fields = page->fields();
            BOOST_FOREACH(Field const * field, fields) {
                QDomElement fieldElement =
                    pageElement.ownerDocument().createElement("field");
                convertToXML(*field, &fieldElement);
                pageElement.appendChild(fieldElement);
            }
        }
        root.appendChild(layout);
        QDomElement sources = xmlDoc.createElement("sources");
        sources.setTagName("sources");
        doc->sourcesManager()->writeSourcesElement(&sources, this);
        root.appendChild(sources);
        static const int INDENT = 2;
        out << xmlDoc.toString(INDENT);
        data.close();
        return true;
    } else {
        return false;
    }
}

void XMLConvertor::readFromFile(DocumentTemplate *newDoc,
    const QString &fileName)
{
    QDomDocument doc(fileName);
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        THROW_EX(Exception, "Cannot open file: " << fileName.toStdString());
    }
    QString errorMessage;
    if (!doc.setContent(&file, &errorMessage)) {
        file.close();
        THROW_EX(Exception, "Cannot read file: " << errorMessage.toStdString());
    }
    QDomElement rootElement = doc.documentElement();
    QDomElement settingsElement = getSingleChild(rootElement, "settings");
    QDomElement format = getSingleChild(settingsElement, "formatting");
    TextRenderInfo *global = processFormattingElement(format);
    TextRenderInfo::globals()->setFont(global->font());
    TextRenderInfo::globals()->setFlags(global->flags());
    TextRenderInfo::globals()->setMargins(global->margins());
    QDomElement layoutElement = getSingleChild(rootElement, "layout");
    processLayoutElement(newDoc, layoutElement);
   // if (rootElement.elementsByTagName("sources").size() > 0) {
        QDomElement sourcesElement = getSingleChild(rootElement, "sources");
        newDoc->sourcesManager()->processSourcesElement(sourcesElement, this);
    //}
    file.close();
}

void XMLConvertor::processLayoutElement(DocumentTemplate *doc,
    const QDomElement &layoutElement)
{
    QDomNodeList pages = layoutElement.elementsByTagName("page");
    if (pages.size() != layoutElement.childNodes().size()) {
        THROW_XML_EX(XMLParseError, layoutElement.lineNumber(),
            layoutElement.columnNumber(),
            "Only child elements with tag \"page\" allowed in layout");
    } else {
        for (size_t i = 0; i < (size_t)pages.size(); i++) {
            QDomElement pageElement = pages.item(i).toElement();
            processPageElement(doc, i + 1, pageElement);
        }
    }
}

void XMLConvertor::convertToXML(const Page &page, QDomElement *pageElement) {
    pageElement->setTagName("page");
    pageElement->setAttribute("width", page.width());
    pageElement->setAttribute("height", page.height());
    pageElement->setAttribute("dx", page.dx());
    pageElement->setAttribute("dy", page.dy());
}

void XMLConvertor::convertToXML(const Field &field, QDomElement *fieldElement) {
    FieldXMLConvertor *fc = field.controller()->getXMLConvertor(this);
    fc->writeFieldElement(field.name(), fieldElement);
}

void XMLConvertor::convertToXML(const TextRenderInfo &data,
    QDomElement *formatElement)
{
    formatElement->setTagName("formatting");
    QDomElement font = formatElement->ownerDocument().createElement("font");
    font.setAttribute("family", data.font().family());
    font.setAttribute("style",
        Settings::fontDB()->styleString(data.font()));
    font.setAttribute("size", data.font().pixelSize());
    formatElement->appendChild(font);
    QDomElement align = formatElement->ownerDocument().createElement("alignment");
    int halign = data.flags() & Qt::AlignHorizontal_Mask;
    if (halign == Qt::AlignLeft) {
        align.setAttribute("halign", "left");
    } else if (halign == Qt::AlignHCenter) {
        align.setAttribute("halign", "center");
    } else {
        align.setAttribute("halign", "right");
    }
    int valign = data.flags() & Qt::AlignVertical_Mask;
    if (valign == Qt::AlignBottom) {
        align.setAttribute("valign", "bottom");
    } else if (valign == Qt::AlignVCenter) {
        align.setAttribute("valign", "center");
    } else {
        align.setAttribute("valign", "top");
    }
    formatElement->appendChild(align);
    QDomElement marginsXml = formatElement->ownerDocument().createElement("margins");
    Margins margins = data.margins();
    marginsXml.setAttribute("left", margins.left);
    marginsXml.setAttribute("right", margins.right);
    marginsXml.setAttribute("top", margins.top);
    marginsXml.setAttribute("bottom", margins.bottom);
    formatElement->appendChild(marginsXml);
}

void XMLConvertor::processPageElement(DocumentTemplate *doc, size_t num,
    const QDomElement &pageElement)
{
    double width = getDouble(pageElement, "width");
    double height = getDouble(pageElement, "height");
    double dx = getDouble(pageElement, "dx");
    double dy = getDouble(pageElement, "dy");

    std::unique_ptr<Page> p(new Page);
    p->setWidth(width);
    p->setHeight(height);
    p->setDx(dx);
    p->setDy(dy);
    p->setOrientation(QPrinter::Portrait);
    if (!doc->addPage(num, std::move(p))) {
        THROW_XML_EX(XMLParseError, pageElement.lineNumber(),
            pageElement.columnNumber(),
            "Error adding current page to layout");
    }

    QDomNodeList fields = pageElement.elementsByTagName("field");
    if (fields.size() != pageElement.childNodes().size()) {
        THROW_XML_EX(XMLParseError, pageElement.lineNumber(),
            pageElement.columnNumber(),
            "Only child elements with tag \"field\" allowed in page");
    } else {
        for (size_t i = 0; i < (size_t)fields.size(); i++) {
            QDomElement fieldElement = fields.item(i).toElement();
            processFieldElement(doc, num, fieldElement);
        }
    }
}

void XMLConvertor::processFieldElement(DocumentTemplate *doc, size_t num,
    const QDomElement &fieldElement)
{
    QString type = getString(fieldElement, "type");

    FieldController *controller;
    FieldXMLConvertor *fc;
    controller = doc->getController(type);

    if (controller == 0) {
        THROW_XML_EX(XMLParseError, fieldElement.lineNumber(),
            fieldElement.columnNumber(), "Field type unknown");
    }

    fc = doc->getController(type)->getXMLConvertor(this);
    // проверки контроллером осуществляются внутри
    std::unique_ptr<Field> newField(
        fc->processFieldElement(num, fieldElement));
    if (newField.get() != 0) {
        if (!doc->addField(fc->name(), std::move(newField), num,
            fc->x(), fc->y()))
        {
            THROW_XML_EX(XMLParseError, fieldElement.lineNumber(),
                fieldElement.columnNumber(),
                "Error adding field to layout");
        }
    }
}

TextRenderInfo *XMLConvertor::processFormattingElement(
    const QDomElement &formatElement)
{
    QFont font = processFontElement(getSingleChild(formatElement, "font"));
    int alignFlags =
        processAlignElement(getSingleChild(formatElement, "alignment"));
    Margins margins = processMarginsElement(
        getSingleChild(formatElement, "margins"));

    return new TextRenderInfo(font, alignFlags, margins);
}

QFont XMLConvertor::processFontElement(const QDomElement &fontElement)
{
    QString family = getString(fontElement, "family");
    QString style = getString(fontElement, "style");
    int pixelSize = getInt(fontElement, "size");

    QFont font = Settings::fontDB()->font(family, style, pixelSize);
    font.setPixelSize(pixelSize);
    //font.setPointSize(int(14 * 25.4 / 72.0));
    return font;
}

int XMLConvertor::processAlignElement(const QDomElement &alignElement)
{
    int halign, valign;
    QString halignStr = getString(alignElement, "halign");

    if (halignStr == "left") {
        halign = Qt::AlignLeft;
    } else if (halignStr == "center") {
        halign = Qt::AlignHCenter;
    } else if (halignStr == "right") {
        halign = Qt::AlignRight;
    } else {
        THROW_EX(Exception, "halign must be one of \"left\", \"right\" or \"center\"");
    }
    QString valignStr = getString(alignElement, "valign");

    if (valignStr == "bottom") {
        valign = Qt::AlignBottom;
    } else if (valignStr == "center") {
        valign = Qt::AlignVCenter;
    } else if (valignStr == "top") {
        valign = Qt::AlignTop;
    } else {
        THROW_EX(Exception, "valign must be one of \"bottom\", \"center\" or \"top\"");
    }
    return halign | valign;
}

Margins XMLConvertor::processMarginsElement(const QDomElement &marginsElement)
{
    Margins margins;
    margins.left = getDouble(marginsElement, "left");
    margins.right = getDouble(marginsElement, "right");
    margins.top = getDouble(marginsElement, "top");
    margins.bottom = getDouble(marginsElement, "bottom");
    return margins;
}

QDomElement XMLConvertor::getSingleChild(const QDomElement &parent,
    const QString &childName)
{
    QDomNodeList childNodes = parent.elementsByTagName(childName);
    if (childNodes.size() != 1) {
        THROW_XML_EX(XMLParseError, parent.lineNumber(), parent.columnNumber(),
            "Exactly one \"" << childName.toStdString() << "\" element is expected");
    } else if (!childNodes.item(0).isElement()) {
        THROW_XML_EX(XMLParseError, parent.lineNumber(), parent.columnNumber(),
            "\"" << childName.toStdString() << "\" is not an element");
    } else {
        return childNodes.item(0).toElement();
    }
}

QString XMLConvertor::getString(const QDomElement &element, const QString &attributeName)
{
    if (!element.hasAttribute(attributeName) ||
        element.attribute(attributeName).isEmpty()) {
        THROW_XML_EX(XMLParseError, element.lineNumber(),
            element.columnNumber(), element.tagName().toStdString() <<
            " attribute \"" << attributeName.toStdString() << "\" not set or is empty");
    } else {
        return element.attribute(attributeName);
    }
}

double XMLConvertor::getDouble(const QDomElement &element,
    const QString &attributeName)
{
    QString str = getString(element, attributeName);

    bool error;
    double res = str.toDouble(&error);
    if (!error) {
        THROW_XML_EX(XMLParseError, element.lineNumber(),
            element.columnNumber(),
            element.tagName().toStdString() <<
            " attribute \"" << attributeName.toStdString() <<
            "\" is not a correct double value");
    }
    return res;
}

int XMLConvertor::getInt(const QDomElement &element,
    const QString &attributeName)
{
    QString str = getString(element, attributeName);
    bool error;
    int res = str.toInt(&error);
    if (!error) {
        THROW_XML_EX(XMLParseError, element.lineNumber(),
            element.columnNumber(),
            element.tagName().toStdString() <<
            " attribute \"" << attributeName.toStdString() <<
        "\" is not a correct integer value");
    }
    return res;
}
