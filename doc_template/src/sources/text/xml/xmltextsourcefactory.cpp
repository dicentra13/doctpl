#include "xmltextsourcefactory.h"

#include "xmltextsourcedialog.h"
#include "xmltextsourcexmlconvertor.h"

#include <QWidget>

XMLTextSourceFactory::XMLTextSourceFactory(QWidget *dialogParent)
    : TextSourceFactory("xml")
    , dialogParent_(dialogParent)
{
    icon_ = QIcon(":/icons/images/fields/xml_text.png");
    iconHint_ = QString(tr("XML_TEXT_SOURCE"));
}

std::unique_ptr<TextSourceDialog> XMLTextSourceFactory::newSourceDialog(
    SourceNameValidator validator) const
{
    return std::unique_ptr<TextSourceDialog>(new XMLTextSourceDialog(
        validator, dialogParent_));
}

std::unique_ptr<TextSourceXMLConvertor>
    XMLTextSourceFactory::newSourceXMLConvertor() const
{
    return std::unique_ptr<TextSourceXMLConvertor>(
        new XMLTextSourceXMLConvertor());
}

