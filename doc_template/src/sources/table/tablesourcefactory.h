#ifndef TABLESOURCEFACTORY_H
#define TABLESOURCEFACTORY_H

#include "../sourcefactory.h"

#include "../../template/sourcenamevalidator.h"
#include "../../xmlconvertor.h"

#include <QDomElement>

#include <memory>

class TableSource;
class TableSourceDialog;
class TableSourceXMLConvertor;

class TableSourceFactory : public SourceFactory {
public:
    std::unique_ptr<TableSource> createNewSource(
        SourceNameValidator validator) const;
    std::unique_ptr<TableSource> createNewSource(SourceNameValidator validator,
        const QDomElement &sourceElement, XMLConvertor *convertor) const;
protected:
    TableSourceFactory(const QString &subtype) : SourceFactory(subtype) {}

    virtual std::unique_ptr<TableSourceDialog> newSourceDialog(
        SourceNameValidator validator) const = 0;
    virtual std::unique_ptr<TableSourceXMLConvertor>
        newSourceXMLConvertor() const = 0;
};

#endif // TABLESOURCEFACTORY_H
