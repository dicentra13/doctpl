#ifndef TEMPLATESOURCESMANAGER_H
#define TEMPLATESOURCESMANAGER_H

#include "../sources/sourcepool.h"
#include "sourcenamevalidator.h"

#include <QObject>
#include <QString>
#include <QStringList>
#include <QIcon>

#include <memory>
#include <map>

class Source;
class SourceDialog;
class QWidget;
class XMLConvertor;
class QDomElement;

class TemplateSourcesManager : public QObject {

    Q_OBJECT

public:
    explicit TemplateSourcesManager(QObject *parent)
        : QObject(parent)
    {}

    void addSourcePool(const QString &sourceType,
        std::unique_ptr<SourcePool> pool);
    QStringList supportedTypes() const;
    QStringList supportedSubtypes(const QString &sourceType) const;
    QIcon subtypeIcon(const QString &type, const QString &subtype) const;
    QString subtypeIconTip(const QString &type, const QString &subtype) const;

    SourceNameValidator getNameValidator() const;

    QStringList sourcesNames() const;
    QString sourceType(const QString &sourceName) const;
    std::unique_ptr<SourceDialog> sourceDialog(const QString &sourceName,
        QWidget *parent);
    QString newSource(const QString &type, const QString &subtype);
    void renameSource(const QString &oldName, const QString &newName);
    void deleteSource(const QString &name);

    void processSourcesElement(const QDomElement &sourcesElement,
        XMLConvertor *convertor);
    void writeSourcesElement(QDomElement *sourcesElement,
        XMLConvertor *convertor) const;

    QStringList assignedFieldNames(const QString &sourceName) const;
    QStringList unassignedFieldNames(const QString &sourceType) const;
    void assignFieldsToSource(const QString &sourceName,
        QStringList fieldNames);

    void addField(const QString &type, const QString &name);
    void removeField(const QString &type, const QString &name);

    bool processSources(const QString &ID);
private:
    typedef std::map<QString, std::unique_ptr<SourcePool>> SourcePoolMap;

    void writeSingleSourceElement(const QString &sourceName,
        QDomElement *sourceElement, XMLConvertor *convertor) const;
    void processSingleSourceElement(const QDomElement &sourceElement,
        XMLConvertor *convertor);
    QStringList processAssignedFieldsElement(const QDomElement &sourceElement,
        XMLConvertor *convertor);
    QString processSingleAssignedFieldElement(
        const QDomElement &assignedFieldElement, XMLConvertor *convertor);

    SourcePoolMap sourcePools_;
};

#endif // TEMPLATESOURCESMANAGER_H
