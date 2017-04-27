#include "helpers.h"

#include "../src/serialize/xml_helpers.h"

#include <doctpl/exception.h>

#include <QDomDocument>
#include <QByteArray>

using namespace doctpl;

FormattingPtr formattingFromString(const std::string& str)
{
    QDomDocument xmldoc;
    QByteArray data(str.c_str());
    QString DOCTPL_ERRORMessage;
    REQUIRE(
        xmldoc.setContent(data, &DOCTPL_ERRORMessage),
        "Cannot parse str: " << DOCTPL_ERRORMessage.toStdString());

    QDomElement rootElement = xmldoc.documentElement();
    return xml::parseFormatting(rootElement);
}

FormattingSettingsPtr createFormatting(const std::string& str)
{
    auto fmt = formattingFromString(str);
    return std::make_shared<FormattingSettings>(
        fmt,
        fmt->copy(),
        FormattingSettings::Mode::Global);
}

