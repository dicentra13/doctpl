#include "sourcenamevalidator.h"

#include "templatesourcesmanager.h"

#include <algorithm>

SourceNameValidator::SourceNameValidator(
        const TemplateSourcesManager &manager)
    : sourcesManager_(manager)
{}

bool SourceNameValidator::checkNameIsAcceptable(
    const QString &sourceName) const
{
    QStringList existentNames = sourcesManager_.sourcesNames();
    QStringList::iterator where = std::find(existentNames.begin(),
        existentNames.end(), sourceName);
    return where == existentNames.end();
}
