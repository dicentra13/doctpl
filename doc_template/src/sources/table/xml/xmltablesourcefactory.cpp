#include "xmltablesourcefactory.h"

#include "xmltablesourcedialog.h"
#include "xmltablesourcexmlconvertor.h"

#include <QWidget>

XMLTableSourceFactory::XMLTableSourceFactory(QWidget *dialogParent)
    : TableSourceFactory("xml")
    , dialogParent_(dialogParent)
{
    icon_ = QIcon(":/icons/images/fields/xml_table.png");
    iconHint_ = QString(tr("XML_TABLE_SOURCE"));
}

std::unique_ptr<TableSourceDialog> XMLTableSourceFactory::newSourceDialog(
    SourceNameValidator validator) const
{
    return std::unique_ptr<TableSourceDialog>(new XMLTableSourceDialog(
        validator, dialogParent_));
}

std::unique_ptr<TableSourceXMLConvertor>
    XMLTableSourceFactory::newSourceXMLConvertor() const
{
    return std::unique_ptr<TableSourceXMLConvertor>(
        new XMLTableSourceXMLConvertor());
}
