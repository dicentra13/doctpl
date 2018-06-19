#include "xml_text_field_factory.h"

#include "xml_helpers.h"

namespace doctpl {
namespace xml {

Field* textFieldXMLFactory(const QDomElement& fieldElement, Page* page)
{
    XML_REQUIRE(
        getAttr<QString>(fieldElement, "type") == "text",
        fieldElement.lineNumber(),
        fieldElement.columnNumber(),
        "Field is not of text type");

    CommonFieldData common = parseCommonFieldData(fieldElement);

    auto fs = parseFormattingSettings(
        getSingleChild(fieldElement, "formatting"),
        page->layout()->document()->defaultFormatting());

    auto texts = getTexts(fieldElement);
    XML_REQUIRE(
        texts.size() <= 1,
        fieldElement.lineNumber(),
        fieldElement.columnNumber(),
        "Text field can have no more than one \"text\" child node");

    TextField* f = new TextField(
        common.name, common.size, common.pos, fs, defaultStyle(), page);
    if (!texts.empty()) {
        f->setText(texts.front());
    }

    return f;
}

} // namespace xml
} // namespace doctpl
