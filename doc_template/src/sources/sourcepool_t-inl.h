#ifndef SOURCEPOOL_T_INL
#define SOURCEPOOL_T_INL

#include "../exception.h"
#include "../xmlconvertor.h"

#include <QWidget>
#include <QDomElement>

#include <algorithm>

template <class source, class source_factory,
    class field, class field_controller>
TSourcePool<source, source_factory, field, field_controller>::
        TSourcePool(const QString &type, field_controller *controller)
    : SourcePool(type)
    , controller_(controller)
{}

template <class source, class source_factory,
    class field, class field_controller>
void TSourcePool<source, source_factory, field, field_controller>::
        registerTSourceFactory(const QString &subtypeName,
    std::unique_ptr<source_factory> subtypeFactory)
{
    subtypeFactories_.insert(std::make_pair(subtypeName, std::move(subtypeFactory)));
}

template <class source, class source_factory,
    class field, class field_controller>
QStringList TSourcePool<source, source_factory, field, field_controller>::
    supportedSubtypes() const
{
    QStringList supportedTypes;
    for (const auto& factoryPair : subtypeFactories_)
    {
        supportedTypes.push_back(factoryPair.first);
    }
    return supportedTypes;
}

template <class source, class source_factory,
    class field, class field_controller>
QIcon TSourcePool<source, source_factory, field, field_controller>::
    subtypeIcon(const QString &subtype) const
{
    typename SubtypeFactoryMap::const_iterator i =
        subtypeFactories_.find(subtype);
    if (i == subtypeFactories_.end()) {
        THROW_EX(Exception, "Source subtype " << subtype.toStdString() <<
             " not registered in document\n")
    }
    return i->second->icon();
}

template <class source, class source_factory,
    class field, class field_controller>
QString TSourcePool<source, source_factory, field, field_controller>::
    subtypeIconHint(const QString &subtype) const
{
    typename SubtypeFactoryMap::const_iterator i =
        subtypeFactories_.find(subtype);
    if (i == subtypeFactories_.end()) {
        THROW_EX(Exception, "Source subtype " << subtype.toStdString() <<
             " not registered in document\n")
    }
    return i->second->iconHint();
}

template <class source, class source_factory,
    class field, class field_controller>
void TSourcePool<source, source_factory, field, field_controller>::
    addField(const QString &fieldName)
{
    field *f = controller_->field(fieldName);
    if (!f) {
        THROW_EX(Exception, "Field named " << fieldName.toStdString() <<
             " not registered in its controller\n")
    }
    unassignedFieldsPool_.push_back(f);
}

template <class source, class source_factory,
    class field, class field_controller>
void TSourcePool<source, source_factory, field, field_controller>::
    removeField(const QString &fieldName)
{
    field *f = controller_->field(fieldName);
    if (!f) {
        THROW_EX(Exception, "Field named " << fieldName.toStdString() <<
             " not registered in its controller\n")
    }
    typename FieldPool::iterator where = std::find(
        unassignedFieldsPool_.begin(), unassignedFieldsPool_.end(), f);
    if (where != unassignedFieldsPool_.end()) {
        unassignedFieldsPool_.erase(where);
    } else {
        typename SourceMap::iterator i = sources_.begin();
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

template <class source, class source_factory,
    class field, class field_controller>
bool TSourcePool<source, source_factory, field, field_controller>::
    sourceExists(const QString &sourceName) const
{
    return sources_.find(sourceName) != sources_.end();
}

template <class source, class source_factory,
    class field, class field_controller>
QString TSourcePool<source, source_factory, field, field_controller>::
    createNewSource(const QString &subtype, SourceNameValidator validator)
{
    typename SubtypeFactoryMap::const_iterator i =
        subtypeFactories_.find(subtype);
    if (i == subtypeFactories_.end()) {
        THROW_EX(Exception, "Source subtype " << subtype.toStdString() <<
             " not registered in document\n")
    }
    std::unique_ptr<source> newSource = i->second->createNewSource(validator);
    QString newName;
    if (newSource.get() != 0) {
        newName = newSource->name();
        sources_.insert(std::make_pair(newName, std::move(newSource)));
    }
    return newName;
}

template <class source, class source_factory,
    class field, class field_controller>
QString TSourcePool<source, source_factory, field, field_controller>::
    createNewSource(const QString &subtype, SourceNameValidator validator,
    const QDomElement &sourceElement, XMLConvertor *convertor)
{
    typename SubtypeFactoryMap::const_iterator i =
        subtypeFactories_.find(subtype);
    if (i == subtypeFactories_.end()) {
        THROW_EX(Exception, "Source subtype " << subtype.toStdString() <<
             " not registered in document\n")
    }
    std::unique_ptr<source> newSource = i->second->createNewSource(
        validator, sourceElement, convertor);
    QString newName = newSource->name();
    sources_.insert(std::make_pair(newName, std::move(newSource)));
    return newName;
}

template <class source, class source_factory,
    class field, class field_controller>
std::unique_ptr<SourceDialog>
    TSourcePool<source, source_factory, field, field_controller>::
        sourceEditDialog(const QString &sourceName,
        SourceNameValidator validator, QWidget *parent)
{
    typename SourceMap::iterator i = sources_.find(sourceName);
    if (i == sources_.end()) {
        THROW_EX(Exception, "Source with name " << sourceName.toStdString() <<
             " is not found\n")
    }
    return i->second->settingsDialog(validator, parent);
}

template <class source, class source_factory,
    class field, class field_controller>
std::unique_ptr<SourceXMLConvertor>
    TSourcePool<source, source_factory, field, field_controller>::
        sourceXMLConvertor(const QString &sourceName) const
{
    typename SourceMap::const_iterator i = sources_.find(sourceName);
    if (i == sources_.end()) {
        THROW_EX(Exception, "Source with name " << sourceName.toStdString() <<
             " is not found\n")
    }
    return i->second->getXMLConvertor();
}

template <class source, class source_factory,
    class field, class field_controller>
void TSourcePool<source, source_factory, field, field_controller>::
    renameSource(const QString &oldName, const QString &newName)
{
    typename SourceMap::iterator i = sources_.find(oldName);
    if (i == sources_.end()) {
        THROW_EX(Exception, "Source with name " << oldName.toStdString() <<
             " is not found\n")
    }
    auto sourcePtr = std::unique_ptr<source>(i->second.release());
    sources_.insert(std::make_pair(newName, std::move(sourcePtr)));
}

template <class source, class source_factory,
    class field, class field_controller>
void TSourcePool<source, source_factory, field, field_controller>::
    deleteSource(const QString &sourceName)
{
    typename SourceMap::iterator i = sources_.find(sourceName);
    if (i == sources_.end()) {
        THROW_EX(Exception, "Source with name " << sourceName.toStdString() <<
             " is not found\n")
    }
    field_list removedFields = i->second->takeFields();
    unassignedFieldsPool_.insert(unassignedFieldsPool_.end(),
        removedFields.begin(), removedFields.end());
    sources_.erase(i);
}

template <class source, class source_factory,
    class field, class field_controller>
bool TSourcePool<source, source_factory, field, field_controller>::
    assignFieldsToSource(const QString &sourceName,
        const QStringList &fieldNames)
{
    typename SourceMap::iterator i = sources_.find(sourceName);
    if (i == sources_.end()) {
        THROW_EX(Exception, "Source with name " << sourceName.toStdString() <<
             " is not found\n")
    }

    if (i->second->maxAssignedFieldsCount() != -1 &&
        fieldNames.size() > i->second->maxAssignedFieldsCount())
    {
        return false;
    }

    field_list removedFields = i->second->takeFields();
    unassignedFieldsPool_.insert(unassignedFieldsPool_.end(),
        removedFields.begin(), removedFields.end());
    field_list fieldsToAdd;
    QStringList::const_iterator fi = fieldNames.begin();
    for (; fi != fieldNames.end(); fi++) {
        typename field_list::iterator f = unassignedFieldsPool_.begin();
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

template <class source, class source_factory,
    class field, class field_controller>
QStringList TSourcePool<source, source_factory, field, field_controller>::
    sourceNames() const
{
    QStringList names;
    for (const auto& s : sources_) {
        names.append(s.first);
    }
    return names;
}

template <class source, class source_factory,
    class field, class field_controller>
QStringList TSourcePool<source, source_factory, field, field_controller>::
    assignedFieldNames(const QString &sourceName) const
{
    typename SourceMap::const_iterator i = sources_.find(sourceName);
    if (i == sources_.end()) {
        THROW_EX(Exception, "Source with name " << sourceName.toStdString() <<
             " is not found\n")
    }
    return i->second->assignedFieldsNames();
}

template <class source, class source_factory,
    class field, class field_controller>
QStringList TSourcePool<source, source_factory, field, field_controller>::
    unassignedFieldNames() const
{
    QStringList fields;
    for (const auto& f : unassignedFieldsPool_) {
        fields.push_back(f->name());
    }
    return fields;
}

template <class source, class source_factory,
    class field, class field_controller>
size_t TSourcePool<source, source_factory, field, field_controller>::
    unassignedFieldsCount() const
{
    return unassignedFieldsPool_.size();
}

template <class source, class source_factory,
    class field, class field_controller>
bool TSourcePool<source, source_factory, field, field_controller>::
    processSources(const QString &ID)
{
    bool res = true;
    typename SourceMap::iterator i = sources_.begin();
    for (; res && i != sources_.end(); i++) {
        res = i->second->process(ID);
    }
    return res;
}

#endif // SOURCEPOOL_T_INL
