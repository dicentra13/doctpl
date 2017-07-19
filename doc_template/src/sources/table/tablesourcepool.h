#ifndef TABLESOURCEPOOL_H
#define TABLESOURCEPOOL_H

#include "../sourcepool.h"
#include "../table/tablesource.h"

#include <QString>
#include <QStringList>
#include <QIcon>

#include <boost/ptr_container/ptr_map.hpp>

#include <list>
#include <map>
#include <memory>

class TableFieldController;
class TableField;
class TableSource;
class TableSourceFactory;
class QAction;

class TableSourcePool : public SourcePool {
public:
    TableSourcePool(TableFieldController *controller);
    void registerTableSourceFactory(const QString &subtypeName,
        std::auto_ptr<TableSourceFactory> subtypeFactory);

    virtual ~TableSourcePool() {}
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

    virtual std::auto_ptr<SourceDialog> sourceEditDialog(
        const QString &sourceName, SourceNameValidator validator,
        QWidget *parent);
    virtual std::auto_ptr<SourceXMLConvertor> sourceXMLConvertor(
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
    typedef boost::ptr_map<QString, TableSourceFactory> SubtypeFactoryMap;
    typedef boost::ptr_map<QString, TableSource> SourceMap;
    typedef std::list<TableField *> FieldPool;

    SourceMap sources_;
    FieldPool unassignedFieldsPool_;
    SubtypeFactoryMap subtypeFactories_;

    TableFieldController *controller_;
};

#endif // TABLESOURCEPOOL_H
