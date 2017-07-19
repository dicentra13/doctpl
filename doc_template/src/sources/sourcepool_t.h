#ifndef SOURCEPOOL_T_H
#define SOURCEPOOL_T_H

#include "source.h"
#include "sourcepool.h"

#include <QString>
#include <QStringList>
#include <QIcon>

#include <list>
#include <map>
#include <memory>

class QAction;
class XMLConvertor;

template <class source, class source_factory,
    class field, class field_controller>
class TSourcePool : public SourcePool {
public:
    TSourcePool(const QString &type, field_controller *controller);
    void registerTSourceFactory(const QString &subtypeName,
        std::unique_ptr<source_factory> subtypeFactory);

    virtual ~TSourcePool() {}
    virtual QStringList supportedSubtypes() const;
    virtual QIcon subtypeIcon(const QString &subtype) const;
    virtual QString subtypeIconHint(const QString &subtype) const;

    virtual void addField(const QString &fieldName);
    virtual void removeField(const QString &fieldName);

    virtual bool sourceExists(const QString &sourceName) const;

    virtual QString createNewSource(const QString &subType,
        SourceNameValidator validator);
    virtual QString createNewSource(const QString &subType,
        SourceNameValidator validator, const QDomElement &sourceElement,
        XMLConvertor *convertor);

    virtual std::unique_ptr<SourceDialog> sourceEditDialog(
        const QString &sourceName, SourceNameValidator validator,
        QWidget *parent);
    virtual std::unique_ptr<SourceXMLConvertor> sourceXMLConvertor(
        const QString &sourceName) const;

    virtual void renameSource(const QString &oldName,
        const QString &newName);
    virtual void deleteSource(const QString &name);

    virtual bool assignFieldsToSource(const QString &sourceName,
        const QStringList &fieldNames);

    virtual QStringList sourceNames() const;
    virtual QStringList assignedFieldNames(const QString &sourceName) const;
    virtual QStringList unassignedFieldNames() const;
    virtual size_t unassignedFieldsCount() const;

    virtual bool processSources(const QString &ID);
private:
    typedef typename std::map<QString, std::unique_ptr<source_factory>> SubtypeFactoryMap;
    typedef typename std::map<QString, std::unique_ptr<source>> SourceMap;
    typedef typename std::list<field *> FieldPool;
    typedef typename std::list<field *> field_list;

    SourceMap sources_;
    FieldPool unassignedFieldsPool_;
    SubtypeFactoryMap subtypeFactories_;

    field_controller *controller_;
};

#include "sourcepool_t-inl.h"

#endif // SOURCEPOOL_T_H
