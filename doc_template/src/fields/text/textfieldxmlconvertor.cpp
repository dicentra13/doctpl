#include "textfieldxmlconvertor.h"

#include "textfieldcontroller.h"
#include "../../template/templatelayout.h"
#include "../../xmlconvertor.h"
#include "textfield.h"

#include <QDomElement>
#include <QDomText>

std::unique_ptr<Field> TextFieldXMLConvertor::processSpecificElement(size_t page,
    const QDomElement &specificElement)
{
    QDomElement format = convertor_->getSingleChild(specificElement, "formatting");
    QString local = convertor_->getString(format, "local");
    isLocalFormatting_ = (local == "true");
    formatting_ = convertor_->processFormattingElement(
        format);

    QDomElement text = convertor_->getSingleChild(specificElement, "text");
    if (text.childNodes().size() == 0) {
        text_ = "";
    } else if (text.childNodes().size() == 1 &&
        text.childNodes().item(0).isText())
    {
        text_ = text.childNodes().item(0).toText().data();
    } else {
        THROW_XML_EX(XMLParseError, text.lineNumber(), text.columnNumber(),
            "Invalid child elements in text field text element");
    }
    return controller_->createField(this, page);
}

void TextFieldXMLConvertor::writeSpecificElement(const QString &fieldName,
    QDomElement *specificElement)
{
    TextField *field = controller_->field(fieldName);
    QDomElement format = specificElement->ownerDocument().createElement("formatting");
    if (field->isLocalFormatting()) {
        format.setAttribute("local", "true");
    } else {
        format.setAttribute("local", "false");
    }
    convertor_->convertToXML(*field->localTextRenderInfo(), &format);
    specificElement->appendChild(format);
    QDomElement textEl = specificElement->ownerDocument().createElement("text");
    QDomText text = specificElement->ownerDocument().createTextNode(
        field->text());
    textEl.appendChild(text);
    specificElement->appendChild(textEl);
}

TextRenderInfo *TextFieldXMLConvertor::formatting() {
    return formatting_;
}

QString TextFieldXMLConvertor::text() {
    return text_;
}
