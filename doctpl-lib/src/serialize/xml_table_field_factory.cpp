#include "xml_table_field_factory.h"

#include "xml_helpers.h"

namespace doctpl {
namespace xml {

namespace {

std::pair<double, std::vector<QString>> parseRow(const QDomElement& rowElement)
{
    return {
        getAttr<double>(rowElement, "height"),
        getTexts(rowElement)
    };
}

std::pair<double, FormattingSettingsPtr> parseColumn(
    const QDomElement& columnElement,
    const FormattingPtr& defaults)
{
    return {
        getAttr<double>(columnElement, "width"),
        parseFormattingSettings(
            getSingleChild(columnElement, "formatting"),
            defaults)
    };
}

} // namespace

Field* tableFieldXMLFactory(const QDomElement& fieldElement, Page* page)
{
    XML_REQUIRE(
        getAttr<QString>(fieldElement, "type") == "table",
        fieldElement.lineNumber(),
        fieldElement.columnNumber(),
        "Field is not of table type");

    CommonFieldData common = parseCommonFieldData(fieldElement);

    std::vector<double> widths, heights;
    std::vector<FormattingSettingsPtr> fs;
    std::vector<std::vector<QString>> texts;

    QDomElement header = getSingleChild(fieldElement, "header");
    QDomNodeList columns = header.elementsByTagName("column");
    widths.reserve(columns.size());
    fs.reserve(columns.size());
    for (int i = 0; i < columns.size(); ++i) {
        auto res = parseColumn(
            columns.item(i).toElement(),
            page->layout()->document()->defaultFormatting());
        widths.push_back(res.first);
        fs.push_back(res.second);
    }

    QDomElement body = getSingleChild(fieldElement, "body");
    QDomNodeList rows = body.elementsByTagName("row");
    heights.reserve(rows.size());
    texts.reserve(rows.size());
    for (int i = 0; i < rows.size(); ++i) {
        const auto& rowEl = rows.item(i).toElement();
        auto res = parseRow(rowEl);
        heights.push_back(res.first);
        const auto& textRow = res.second;
        XML_REQUIRE(
            textRow.size() == widths.size(),
            rowEl.lineNumber(),
            rowEl.columnNumber(),
            "\"row\" element with " <<i << " index text nodes size mismatch with header size");
        texts.push_back(textRow);
    }

    TableField* f = new TableField(
        common.name, widths, heights, common.pos, std::move(fs), page);

    for (size_t i = 0; i < heights.size(); ++i) {
        for (size_t j = 0; j < widths.size(); ++j) {
            f->setText(i, j, texts[i][j]);
        }
    }

    return f;
}

} // namespace xml
} // namespace doctpl
