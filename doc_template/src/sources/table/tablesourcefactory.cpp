#include "tablesourcefactory.h"

#include "tablesourcedialog.h"
#include "tablesourcexmlconvertor.h"
#include "tablesource.h"
#include "../../xmlconvertor.h"

#include <QDomElement>

std::unique_ptr<TableSource> TableSourceFactory::createNewSource(
    SourceNameValidator validator) const
{
    std::unique_ptr<TableSource> newSource;
    std::unique_ptr<TableSourceDialog> dialog = newSourceDialog(validator);
    dialog->exec();
    if (dialog->result() == QDialog::Accepted) {
        newSource = dialog->createdSource();
    }
    return newSource;
}

std::unique_ptr<TableSource> TableSourceFactory::createNewSource(
    SourceNameValidator validator, const QDomElement &sourceElement,
    XMLConvertor *convertor) const
{
    return newSourceXMLConvertor()->processSourceData(
        sourceElement, convertor, validator);
}
