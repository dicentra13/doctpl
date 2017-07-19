#ifndef TEXTSOURCEFACTORY_H
#define TEXTSOURCEFACTORY_H

#include "../sourcefactory.h"

#include "../../template/sourcenamevalidator.h"
#include "../../xmlconvertor.h"

#include <QDomElement>

#include <memory>

class TextSource;
class TextSourceDialog;
class TextSourceXMLConvertor;

class TextSourceFactory : public SourceFactory {
public:
    std::unique_ptr<TextSource> createNewSource(
        SourceNameValidator validator) const;
    std::unique_ptr<TextSource> createNewSource(SourceNameValidator validator,
        const QDomElement &sourceElement, XMLConvertor *convertor) const;
protected:
    TextSourceFactory(const QString &subtype) : SourceFactory(subtype) {}

    virtual std::unique_ptr<TextSourceDialog> newSourceDialog(
        SourceNameValidator validator) const = 0;
    virtual std::unique_ptr<TextSourceXMLConvertor>
        newSourceXMLConvertor() const = 0;
};

#endif // TEXTSOURCEFACTORY_H
