#include "tablefieldxmlconvertor.h"

#include "tablefieldcontroller.h"
#include "../../template/templatelayout.h"
#include "../../xmlconvertor.h"
#include "tablefield.h"
#include "../textrenderinfo.h"

#include <QDomElement>
#include <QDomText>

std::unique_ptr<Field> TableFieldXMLConvertor::processSpecificElement(size_t page,
    const QDomElement &fieldElement)
{
    QDomElement header = convertor_->getSingleChild(fieldElement, "header");
    QDomNodeList columns = header.elementsByTagName("column");
    for (int i = 0; i < columns.size(); i++) {
        QDomElement column = columns.item(i).toElement();
        processColumnElement(column);
    }

    QDomElement body = convertor_->getSingleChild(fieldElement, "body");
    QDomNodeList rows = body.elementsByTagName("row");
    for (int i = 0; i < rows.size(); i++) {
        QDomElement row = rows.item(i).toElement();
        processRowElement(row);
    }

    return controller_->createField(this, page);
}

void TableFieldXMLConvertor::processRowElement(const QDomElement &rowElement)
{
    rowHeights_.push_back(convertor_->getDouble(rowElement, "height"));

    QDomNodeList texts = rowElement.elementsByTagName("text");
    std::vector<QString> textRow;
    for (int i = 0; i < texts.size(); i++) {
        if (texts.at(i).childNodes().size() != 1 ||
            !texts.at(i).childNodes().item(0).isText())
        {
            textRow.push_back("");
        } else {
            textRow.push_back(texts.at(i).childNodes().item(0).toText().data());
        }
    }
    if (textRow.size() != columnWidths_.size()) {
        THROW_XML_EX(XMLParseError, rowElement.lineNumber(),
            rowElement.columnNumber(),
            "Number of texts in row differes from number of columns");
    }
    texts_.push_back(textRow);
}

void TableFieldXMLConvertor::processColumnElement(
    const QDomElement &columnElement)
{
    columnWidths_.push_back(convertor_->getDouble(columnElement, "width"));
    QDomElement format = convertor_->getSingleChild(columnElement, "formatting");
    QString local = convertor_->getString(format, "local");
    isLocalFormatting_.push_back(local == "true");
    columnFormatting_.push_back(convertor_->processFormattingElement(
        format));
}

void TableFieldXMLConvertor::writeSpecificElement(const QString &fieldName,
    QDomElement *fieldElement)
{
    TableField *field = controller_->field(fieldName);

    QDomDocument doc(fieldElement->ownerDocument());

    QDomElement header = doc.createElement("header");
    for (size_t j = 1; j <= field->numberOfColumns(); j++) {
        QDomElement colEl = doc.createElement("column");
        colEl.setAttribute("width", field->columnWidth(j));
        QDomElement colForm = doc.createElement("formatting");
        if (field->isLocalFormatting(j)) {
            colForm.setAttribute("local", "true");
        } else {
            colForm.setAttribute("local", "false");
        }
        convertor_->convertToXML(*field->localColumnFormatting(j), &colForm);
        colEl.appendChild(colForm);
        header.appendChild(colEl);
    }
    fieldElement->appendChild(header);

    QDomElement body = doc.createElement("body");
    for (size_t i = 1; i <= field->numberOfRows(); i++) {
        QDomElement rowEl = doc.createElement("row");
        rowEl.setAttribute("height", field->rowHeight(i));
        for (size_t j = 1; j <= field->numberOfColumns(); j++) {
            QDomElement textEl = doc.createElement("text");
            QDomText text = doc.createTextNode(field->text(i, j));
            textEl.appendChild(text);
            rowEl.appendChild(textEl);
        }
        body.appendChild(rowEl);
    }
    fieldElement->appendChild(body);
}

