#include "textsourcefactory.h"

#include "textsourcedialog.h"
#include "textsourcexmlconvertor.h"
#include "textsource.h"
#include "../../xmlconvertor.h"

#include <QDomElement>

std::unique_ptr<TextSource> TextSourceFactory::createNewSource(
    SourceNameValidator validator) const
{
    std::unique_ptr<TextSource> newSource;
    std::unique_ptr<TextSourceDialog> dialog = newSourceDialog(validator);
    dialog->exec();
    if (dialog->result() == QDialog::Accepted) {
        newSource = dialog->createdSource();
    }
    return newSource;
}

std::unique_ptr<TextSource> TextSourceFactory::createNewSource(
    SourceNameValidator validator, const QDomElement &sourceElement,
    XMLConvertor *convertor) const
{
    return newSourceXMLConvertor()->processSourceData(
        sourceElement, convertor, validator);
}
