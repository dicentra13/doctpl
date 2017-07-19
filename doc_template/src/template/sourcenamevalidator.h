#ifndef SOURCENAMEVALIDATOR_H
#define SOURCENAMEVALIDATOR_H

#include <QString>

class TemplateSourcesManager;

class SourceNameValidator {
public:
    SourceNameValidator(const TemplateSourcesManager &);
    bool checkNameIsAcceptable(const QString &sourceName) const;
private:
    const TemplateSourcesManager &sourcesManager_;
};

#endif // SOURCENAMEVALIDATOR_H
