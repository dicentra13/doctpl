#include "xml_helpers.h"
#include "xml_magic_strings.h"

#include <doctpl/serialize.h>
#include <doctpl/settings.h>

namespace doctpl {
namespace xml {

QDomElement getSingleChild(const QDomElement& parent, const QString& childName)
{
    QDomNodeList childNodes = parent.elementsByTagName(childName);
    XML_REQUIRE(
        childNodes.size() == 1,
        parent.lineNumber(),
        parent.columnNumber(),
        "Exactly one \"" << childName.toStdString() << "\" element is expected");
    XML_REQUIRE(
        childNodes.item(0).isElement(),
        parent.lineNumber(),
        parent.columnNumber(),
        "\"" << childName.toStdString() << "\" is not an element");

    return childNodes.item(0).toElement();
}

template <>
QString getAttr<QString>(const QDomElement& el, const QString& attrName)
{
    XML_REQUIRE(
        el.hasAttribute(attrName) && !el.attribute(attrName).isEmpty(),
        el.lineNumber(),
        el.columnNumber(),
        el.tagName().toStdString()
            << " attribute \"" << attrName.toStdString() << "\" not set or is empty");
    return el.attribute(attrName);
}

namespace {

template <class NumT>
NumT getNumeric(
    const QDomElement& el,
    const QString& attrName,
    const char* type,
    std::function<std::pair<NumT, bool>(const QString&)> conv)
{
    auto res = conv(getAttr<QString>(el, attrName));
    XML_REQUIRE(
        res.second,
        el.lineNumber(),
        el.columnNumber(),
        el.tagName().toStdString()
            << " attribute \"" << attrName.toStdString()
            << "\" is not a correct " << type << " value");
    return res.first;
}

} // namespace

template <>
double getAttr<double>(const QDomElement& el, const QString& attrName)
{
    return getNumeric<double>(
        el, attrName, "double",
        [] (const QString& s) -> std::pair<double, bool>
            { bool ok; double res = s.toDouble(&ok); return {res, ok}; });
}

template <>
int getAttr<int>(const QDomElement& el, const QString& attrName)
{
    return getNumeric<int>(
        el, attrName, "int",
        [] (const QString& s) -> std::pair<int, bool>
            { bool ok; int res = s.toInt(&ok); return {res, ok}; });
}

template <>
bool getAttr<bool>(const QDomElement& el, const QString& attrName)
{
    QString res = getAttr<QString>(el, attrName).toLower();
    XML_REQUIRE(
        res == "true" || res == "false",
        el.lineNumber(), el.columnNumber(),
        el.tagName().toStdString()
            << " attribute \"" << attrName.toStdString()
            << "\" is not a correct bool value");
    return res == "true";
}

QFont parseFont(const QDomElement& fontElement)
{
    QString family = getAttr<QString>(fontElement, "family");
    QString style = getAttr<QString>(fontElement, "style");
    int pixelSize = getAttr<int>(fontElement, "size");

    QFont font = settings().fontDB().font(family, style, pixelSize);
    font.setPixelSize(pixelSize);
    //font.setPointSize(int(14 * 25.4 / 72.0));
    return font;
}

int parseAlign(const QDomElement& alignElement)
{
    int halign, valign;
    QString halignStr = getAttr<QString>(alignElement, "halign");

    std::map<QString, Qt::AlignmentFlag> hValues = {
        {"left", Qt::AlignLeft},
        {"center", Qt::AlignHCenter},
        {"right", Qt::AlignRight}
    };

    auto it = hValues.find(halignStr);
    REQUIRE(it != hValues.end(), "halign must be one of \"left\", \"right\" or \"center\"");
    halign = it->second;

    QString valignStr = getAttr<QString>(alignElement, "valign");

    std::map<QString, Qt::AlignmentFlag> vValues = {
        {"bottom", Qt::AlignBottom},
        {"center", Qt::AlignVCenter},
        {"top", Qt::AlignTop}
    };

    it = vValues.find(valignStr);
    REQUIRE(it != vValues.end(), "valign must be one of \"bottom\", \"center\" or \"top\"");
    valign = it->second;

    return halign | valign;
}

Margins parseMargins(const QDomElement& marginsElement)
{
    return {
        getAttr<double>(marginsElement, "left"),
        getAttr<double>(marginsElement, "right"),
        getAttr<double>(marginsElement, "top"),
        getAttr<double>(marginsElement, "bottom")
    };
}

FormattingSettingsPtr parseFormattingSettings(
    const QDomElement& fmtElement,
    const FormattingPtr& defaults)
{
    FormattingSettings::Mode mode = getAttr<bool>(fmtElement, "local")
        ? FormattingSettings::Mode::Local
        : FormattingSettings::Mode::Global;
    FormattingPtr locals = mode == FormattingSettings::Mode::Local
        ? parseFormatting(fmtElement)
        : defaults->copy();
    return std::make_shared<FormattingSettings>(defaults, locals, mode);
}

FormattingPtr parseFormatting(const QDomElement& fmtElement)
{
    return std::make_shared<Formatting>(
        parseFont(getSingleChild(fmtElement, "font")),
        parseAlign(getSingleChild(fmtElement, "alignment")),
        parseMargins(getSingleChild(fmtElement, "margins")));
}

CommonFieldData parseCommonFieldData(const QDomElement& fieldElement)
{
    const auto& posEl = getSingleChild(fieldElement, "pos");
    const auto& sizeEl = getSingleChild(fieldElement, "size");
    return {
        getAttr<QString>(fieldElement, "name"),
        {getAttr<double>(posEl, "x"), getAttr<double>(posEl, "y")},
        {getAttr<double>(sizeEl, "width"), getAttr<double>(sizeEl, "height")}
    };
}

std::vector<QString> getTexts(const QDomElement& parentElement)
{
    QDomNodeList texts = parentElement.elementsByTagName("text");
    std::vector<QString> res;
    res.reserve(texts.size());
    for (int i = 0; i < texts.size(); ++i) {
        const auto& children = texts.at(i).childNodes();
        if (children.size() == 0) {
            res.push_back("");
            continue;
        }
        XML_REQUIRE(
            children.size() == 1 && children.item(0).isText(),
            parentElement.lineNumber(),
            parentElement.columnNumber(),
            "\"text\" element can have only one child text node");
        res.push_back(children.item(0).toText().data());
    }
    return res;
}

} // namespace xml
} // namespace doctpl
