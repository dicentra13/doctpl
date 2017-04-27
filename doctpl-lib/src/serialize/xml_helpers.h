#pragma once

#include <doctpl/formatting_settings.h>

#include <QDomElement>
#include <QString>
#include <QFont>
#include <QPointF>
#include <QSizeF>

#include <vector>

namespace doctpl {
namespace xml {

QDomElement getSingleChild(const QDomElement& parent, const QString& childName);

/**
 * QString
 * int
 * double
 * bool
 */
template <class T>
T getAttr(const QDomElement& el, const QString& attrName);

QFont parseFont(const QDomElement& fontElement);

int parseAlign(const QDomElement& alignElement);

Margins parseMargins(const QDomElement& marginsElement);

FormattingPtr parseFormatting(const QDomElement& fmtElement);

FormattingSettingsPtr parseFormattingSettings(
    const QDomElement& fmtElement, const FormattingPtr& defaults);

struct CommonFieldData {
    QString name;
    QPointF pos;
    QSizeF size;
};

CommonFieldData parseCommonFieldData(const QDomElement& fieldElement);

std::vector<QString> getTexts(const QDomElement& parentElement);

} // namespace xml
} // namespace doctpl
