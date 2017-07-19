#include "tablesourcepool.h"

#include "../../fields/table/tablefieldcontroller.h"
#include "tablesource.h"
#include "tablesourcefactory.h"
#include "../../fields/table/tablefield.h"
#include "../../exception.h"
#include "../../xmlconvertor.h"

#include <QWidget>
#include <QDomElement>

#include <boost/foreach.hpp>
#include <algorithm>

TableSourcePool::TableSourcePool(TableFieldController *controller)
    : SourcePool("table")
    , controller_(controller)
{}

void TableSourcePool::registerTableSourceFactory(const QString &subtypeName,
    std::auto_ptr<TableSourceFactory> subtypeFactory)
{
    subtypeFactories_.insert(subtypeName, subtypeFactory);
}

QStringList TableSourcePool::supportedSubtypes() const {
    QStringList supportedTypes;
    BOOST_FOREACH(const SubtypeFactoryMap::const_reference &factory,
        subtypeFactories_)
    {
        supportedTypes.push_back(factory.first);
    }
    return supportedTypes;
}

QIcon TableSourcePool::subtypeIcon(const QString &subtype) const {
    SubtypeFactoryMap::const_iterator i = subtypeFactories_.find(subtype);
    if (i == subtypeFactories_.end()) {
        THROW_EX(Exception, "Source subtype " << subtype.toStdString() <<
             " not registered in document\n")
    }
    return i->second->icon();
}

QString TableSourcePool::subtypeIconHint(const QString &subtype) const {
    SubtypeFactoryMap::const_iterator i = subtypeFactories_.find(subtype);
    if (i == subtypeFactories_.end()) {
        THROW_EX(Exception, "Source subtype " << subtype.toStdString() <<
             " not registered in document\n")
    }
    return i->second->iconHint();
}

void TableSourcePool::addField(const QString &fieldName) {
    TableField *field = controller_->field(fieldName);
    if (!field) {
        THROW_EX(Exception, "Field named " << fieldName.toStdString() <<
             " not registered in its controller\n")
    }
    unassignedFieldsPool_.push_back(field);
}

void TableSourcePool::removeField(const QString &fieldName) {
    TableField *f = controller_->field(fieldName);
    if (!f) {
        THROW_EX(Exception, "Field named " << fieldName.toStdString() <<
             " not registered in its controller\n")
    }
    FieldPool::iterator where = std::find(
        unassignedFieldsPool_.begin(), unassignedFieldsPool_.end(), f);
    if (where != unassignedFieldsPool_.end()) {
        unassignedFieldsPool_.erase(where);
    } else {
        SourceMap::iterator i = sources_.begin();
        for (; i != sources_.end(); i++) {
            if (i->second->isFieldAssigned(fieldName))
                break;
        }
        if (i == sources_.end()) {
            THROW_EX(Exception, "Field named " << fieldName.toStdString() <<
                 " is not found neither in unassigned fields pool " <<
                 "nor in any source\n")
        }
        i->second->removeField(fieldName);
    }
}

bool TableSourcePool::sourceExists(const QString &sourceName) const {
    return sources_.find(sourceName) != sources_.end();
}

QString TableSourcePool::createNewSource(const QString &subtype,
    SourceNameValidator validator)
{
    SubtypeFactoryMap::const_iterator i = subtypeFactories_.find(subtype);
    if (i == subtypeFactories_.end()) {
        THROW_EX(Exception, "Source subtype " << subtype.toStdString() <<
             " not registered in document\n")
    }
    std::auto_ptr<TableSource> newSource = i->second->createNewSource(
        validator);
    QString newName;
    if (newSource.get() != 0) {
        newName = newSource->name();
        sources_.insert(newName, newSource);
    }
    return newName;
}

QString TableSourcePool::createNewSource(const QString &subtype,
    SourceNameValidator validator, const QDomElement &sourceElement,
    XMLConvertor *convertor)
{
    SubtypeFactoryMap::const_iterator i = subtypeFactories_.find(subtype);
    if (i == subtypeFactories_.end()) {
        THROW_EX(Exception, "Source subtype " << subtype.toStdString() <<
             " not registered in document\n")
    }
    std::auto_ptr<TableSource> newSource = i->second->createNewSource(
        validator, sourceElement, convertor);
    QString newName = newSource->name();
    sources_.insert(newName, newSource);
    return newName;
}

std::auto_ptr<SourceDialog> TableSourcePool::sourceEditDialog(
    const QString &sourceName, SourceNameValidator validator, QWidget *parent)
{
    SourceMap::iterator i = sources_.find(sourceName);
    if (i == sources_.end()) {
        THROW_EX(Exception, "Source with name " << sourceName.toStdString() <<
             " is not found\n")
    }
    return i->second->settingsDialog(validator, parent);
}

std::auto_ptr<SourceXMLConvertor> TableSourcePool::sourceXMLConvertor(
    const QString &sourceName) const
{
    SourceMap::const_iterator i = sources_.find(sourceName);
    if (i == sources_.end()) {
        THROW_EX(Exception, "Source with name " << sourceName.toStdString() <<
             " is not found\n")
    }
    return i->second->getXMLConvertor();
}

void TableSourcePool::renameSource(const QString &oldName,
    const QString &newName)
{
    SourceMap::iterator i = sources_.find(oldName);
    if (i == sources_.end()) {
        THROW_EX(Exception, "Source with name " << oldName.toStdString() <<
             " is not found\n")
    }
    SourceMap::auto_type ptr = sources_.release(i);
    sources_.insert(newName, std::auto_ptr<TableSource>(ptr.release()));
}

void TableSourcePool::deleteSource(const QString &sourceName) {
    SourceMap::iterator i = sources_.find(sourceName);
    if (i == sources_.end()) {
        THROW_EX(Exception, "Source with name " << sourceName.toStdString() <<
             " is not found\n")
    }
    std::list<TableField *> removedFields = i->second->takeFields();
    unassignedFieldsPool_.insert(unassignedFieldsPool_.end(),
        removedFields.begin(), removedFields.end());
    sources_.erase(i);
}

bool TableSourcePool::assignFieldsToSource(const QString &sourceName,
    const QStringList &fieldNames)
{
    SourceMap::iterator i = sources_.find(sourceName);
    if (i == sources_.end()) {
        THROW_EX(Exception, "Source with name " << sourceName.toStdString() <<
             " is not found\n")
    }

    if (fieldNames.size() > i->second->maxAssignedFieldsCount())
        return false;

    std::list<TableField *> removedFields = i->second->takeFields();
    unassignedFieldsPool_.insert(unassignedFieldsPool_.end(),
        removedFields.begin(), removedFields.end());
    std::list<TableField *> fieldsToAdd;
    QStringList::const_iterator fi = fieldNames.begin();
    for (; fi != fieldNames.end(); fi++) {
        std::list<TableField *>::iterator f = unassignedFieldsPool_.begin();
        for (; f != unassignedFieldsPool_.end(); f++) {
            if ((*f)->name() == *fi)
                break;
        }
        if (f == unassignedFieldsPool_.end()) {
            THROW_EX(Exception, "Error assigning fields to source: " <<
                 "field with name " << fi->toStdString() <<
                 " is not found in unassigned sources pool\n")
        }
        fieldsToAdd.push_back(*f);
        unassignedFieldsPool_.erase(f);
    }
    i->second->assignFields(fieldsToAdd);
    return true;
}

QStringList TableSourcePool::sourceNames() const {
    QStringList names;
    BOOST_FOREACH(const SourceMap::const_reference &source, sources_) {
        names.append(source.first);
    }
    return names;
}

QStringList TableSourcePool::assignedFieldNames(const QString &sourceName)
    const
{
    SourceMap::const_iterator i = sources_.find(sourceName);
    if (i == sources_.end()) {
        THROW_EX(Exception, "Source with name " << sourceName.toStdString() <<
             " is not found\n")
    }
    return i->second->assignedFieldsNames();
}

QStringList TableSourcePool::unassignedFieldNames() const {
    QStringList fields;
    BOOST_FOREACH(const FieldPool::const_reference field,
        unassignedFieldsPool_)
    {
        fields.push_back(field->name());
    }
    return fields;
}

size_t TableSourcePool::unassignedFieldsCount() const {
    return unassignedFieldsPool_.size();
}

bool TableSourcePool::processSources(const QString &ID) {
    bool res = true;
    SourceMap::iterator i = sources_.begin();
    for (; res && i != sources_.end(); i++) {
        res = i->second->process(ID);
    }
    return res;
}
