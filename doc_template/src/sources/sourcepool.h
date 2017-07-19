#ifndef SOURCEPOOL_H
#define SOURCEPOOL_H

#include "../template/sourcenamevalidator.h"

#include <QString>
#include <QStringList>
#include <QIcon>

#include <list>
#include <memory>

class QAction;
class QWidget;
class SourceDialog;
class QDomElement;
class XMLConvertor;
class SourceXMLConvertor;

class SourcePool {
public:
    virtual ~SourcePool() {};
    virtual QStringList supportedSubtypes() const = 0;
    virtual QIcon subtypeIcon(const QString &subtype) const = 0;
    virtual QString subtypeIconHint(const QString &subtype) const = 0;

    virtual void addField(const QString &fieldName) = 0;
    virtual void removeField(const QString &fieldName) = 0;

    virtual bool sourceExists(const QString &sourceName) const = 0;

    virtual QString createNewSource(const QString &subType,
        SourceNameValidator validator) = 0;
    virtual QString createNewSource(const QString &subType,
        SourceNameValidator validator, const QDomElement &sourceElement,
        XMLConvertor *convertor) = 0;

    virtual std::unique_ptr<SourceDialog> sourceEditDialog(
        const QString &sourceName, SourceNameValidator validator,
        QWidget *parent) = 0;
    virtual std::unique_ptr<SourceXMLConvertor> sourceXMLConvertor(
        const QString &sourceName) const = 0;

    virtual void renameSource(const QString &oldName,
        const QString &newName) = 0;
    virtual void deleteSource(const QString &name) = 0;

    virtual bool assignFieldsToSource(const QString &sourceName,
        const QStringList &fieldNames) = 0;

    virtual QStringList sourceNames() const = 0;
    virtual QStringList assignedFieldNames(const QString &sourceName) const = 0;
    virtual QStringList unassignedFieldNames() const = 0;
    virtual size_t unassignedFieldsCount() const = 0;

    virtual bool processSources(const QString &ID) = 0;

    QString sourceType() const {
        return type_;
    }
protected:
    SourcePool(const QString &type)
        : type_(type)
    {}

    QString type_;
};

#endif // SOURCEPOOL_H
