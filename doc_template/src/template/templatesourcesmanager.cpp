#include "templatesourcesmanager.h"

#include "../sources/sourcepool.h"
#include "../sources/sourcedialog.h"
#include "../exception.h"
#include "../xmlconvertor.h"
#include "../sources/sourcexmlconvertor.h"

#include <QWidget>
#include <QDomElement>
#include <QDomNodeList>
#include <QDomDocument>

void TemplateSourcesManager::addSourcePool(const QString &sourceType,
    std::unique_ptr<SourcePool> pool)
{
    sourcePools_.insert(std::make_pair(sourceType, std::move(pool)));
}

QStringList TemplateSourcesManager::supportedTypes() const {
    QStringList types;
    for (const auto& sourcePool : sourcePools_) {
        types.push_back(sourcePool.first);
    }
    return types;
}

QStringList TemplateSourcesManager::supportedSubtypes(
    const QString &sourceType) const
{
    SourcePoolMap::const_iterator i = sourcePools_.find(sourceType);
    if (i == sourcePools_.end()) {
        /** FIXME throw */
    }
   return i->second->supportedSubtypes();
}

QIcon TemplateSourcesManager::subtypeIcon(const QString &type,
    const QString &subtype) const
{
    SourcePoolMap::const_iterator i = sourcePools_.find(type);
    if (i == sourcePools_.end()) {
        /** FIXME throw */
    }
   return i->second->subtypeIcon(subtype);
}

QString TemplateSourcesManager::subtypeIconTip(const QString &type,
    const QString &subtype) const
{
    SourcePoolMap::const_iterator i = sourcePools_.find(type);
    if (i == sourcePools_.end()) {
        /** FIXME throw */
    }
   return i->second->subtypeIconHint(subtype);
}

SourceNameValidator TemplateSourcesManager::getNameValidator() const
{
    return SourceNameValidator(*this);
}

QStringList TemplateSourcesManager::sourcesNames() const {
    QStringList names;
    for (const auto& poolPair : sourcePools_) {
        names.append(poolPair.second->sourceNames());
    }
    return names;
}

QString TemplateSourcesManager::sourceType(const QString &sourceName) const {
    SourcePoolMap::const_iterator i = sourcePools_.begin();
    for (; i != sourcePools_.end(); i++) {
        if (i->second->sourceExists(sourceName)) {
            break;
        }
    }
    if (i == sourcePools_.end()) {
        /** FIXME throw */
    }
    return i->first;
}

std::unique_ptr<SourceDialog> TemplateSourcesManager::sourceDialog(
    const QString &sourceName, QWidget *parent)
{
    QString type = sourceType(sourceName);
    SourcePoolMap::iterator i = sourcePools_.find(type);
    if (i == sourcePools_.end()) {
        /** FIXME throw */
    }
    return i->second->sourceEditDialog(sourceName,
        SourceNameValidator(*this), parent);
}

QString TemplateSourcesManager::newSource(const QString &type,
    const QString &subtype)
{
    SourcePoolMap::iterator i = sourcePools_.find(type);
    if (i == sourcePools_.end()) {
        /** FIXME throw */
    }
   return i->second->createNewSource(subtype, SourceNameValidator(*this));
}

void TemplateSourcesManager::renameSource(const QString &oldName,
    const QString &newName)
{
    QString type = sourceType(oldName);
    SourcePoolMap::iterator i = sourcePools_.find(type);
    if (i == sourcePools_.end()) {
        /** FIXME throw */
    }
    i->second->renameSource(oldName, newName);
}

void TemplateSourcesManager::deleteSource(const QString &name) {
    QString type = sourceType(name);
    SourcePoolMap::iterator i = sourcePools_.find(type);
    if (i == sourcePools_.end()) {
        /** FIXME throw */
    }
    i->second->deleteSource(name);
}

QStringList TemplateSourcesManager::assignedFieldNames(
    const QString &sourceName) const
{
    QString type = sourceType(sourceName);
    SourcePoolMap::const_iterator i = sourcePools_.find(type);
    if (i == sourcePools_.end()) {
        /** FIXME throw */
    }
    return i->second->assignedFieldNames(sourceName);
}

QStringList TemplateSourcesManager::unassignedFieldNames(
    const QString &sourceType) const
{
    SourcePoolMap::const_iterator i = sourcePools_.find(sourceType);
    if (i == sourcePools_.end()) {
        /** FIXME throw */
    }
    return i->second->unassignedFieldNames();
}

void TemplateSourcesManager::assignFieldsToSource(const QString &sourceName,
    QStringList fieldNames)
{
    QString type = sourceType(sourceName);
    SourcePoolMap::iterator i = sourcePools_.find(type);
    if (i == sourcePools_.end()) {
        /** FIXME throw */
    }
    i->second->assignFieldsToSource(sourceName, fieldNames);
}

void TemplateSourcesManager::addField(const QString &type,
    const QString &name)
{
    SourcePoolMap::iterator i = sourcePools_.find(type);
    if (i == sourcePools_.end()) {
        /** FIXME throw */
    }
    i->second->addField(name);
}

void TemplateSourcesManager::removeField(const QString &type,
    const QString &name)
{
    SourcePoolMap::iterator i = sourcePools_.find(type);
    if (i == sourcePools_.end()) {
        /** FIXME throw */
    }
    i->second->removeField(name);
}

bool TemplateSourcesManager::processSources(const QString &ID) {
    bool result = true;
    SourcePoolMap::iterator i = sourcePools_.begin();
    for (; result && i != sourcePools_.end(); i++) {
        result = i->second->processSources(ID);
    }
    return result;
}

void TemplateSourcesManager::processSourcesElement(
    const QDomElement &sourcesElement, XMLConvertor *convertor)
{
    QDomNodeList sources = sourcesElement.elementsByTagName("source");
    if (sources.size() != sourcesElement.childNodes().size()) {
        THROW_XML_EX(XMLParseError, sourcesElement.lineNumber(),
            sourcesElement.columnNumber(),
            "Only child elements with tag \"source\" allowed in sources");
    } else {
        for (size_t i = 0; i < (size_t)sources.size(); i++) {
            QDomElement sourceElement = sources.item(i).toElement();
            processSingleSourceElement(sourceElement, convertor);
        }
    }
}

void TemplateSourcesManager::processSingleSourceElement(
    const QDomElement &sourceElement, XMLConvertor *convertor)
{
    QString type = convertor->getString(sourceElement, "type");
    SourcePoolMap::iterator i = sourcePools_.find(type);
    if (i == sourcePools_.end()) {
        THROW_EX(Exception, "Source type " << type.toStdString() <<
             " is not registered\n")
    }
    QString subtype = convertor->getString(sourceElement, "subtype");
    QString sourceName = i->second->createNewSource(subtype,
        SourceNameValidator(*this), sourceElement, convertor);
    QStringList assignedFields = processAssignedFieldsElement(sourceElement,
        convertor);
    i->second->assignFieldsToSource(sourceName, assignedFields);
}

QStringList TemplateSourcesManager::processAssignedFieldsElement(
    const QDomElement &sourceElement, XMLConvertor *convertor)
{
    QDomElement assignedFieldsElement = convertor->getSingleChild(
        sourceElement, "assigned_fields");
    QDomNodeList fields = assignedFieldsElement.elementsByTagName(
        "assigned_field");
    if (fields.size() != assignedFieldsElement.childNodes().size()) {
        THROW_XML_EX(XMLParseError, assignedFieldsElement.lineNumber(),
            assignedFieldsElement.columnNumber(),
            "Only child elements with tag \"assigned_field\" allowed in assigned fields")
    }
    QStringList fieldNames;
    for (size_t i = 0; i < (size_t)fields.size(); i++) {
        QDomElement fieldElement = fields.item(i).toElement();
        fieldNames.push_back(
            processSingleAssignedFieldElement(fieldElement, convertor));
    }
    return fieldNames;
}

QString TemplateSourcesManager::processSingleAssignedFieldElement(
    const QDomElement &assignedFieldElement, XMLConvertor */*convertor*/)
{
    if (assignedFieldElement.childNodes().size() != 1 ||
        !assignedFieldElement.childNodes().item(0).isText())
    {
        THROW_XML_EX(XMLParseError, assignedFieldElement.lineNumber(),
            assignedFieldElement.columnNumber(),
            "Only assigned field name must be specified")
    }
    return assignedFieldElement.childNodes().item(0).toText().data();
}

void TemplateSourcesManager::writeSourcesElement(QDomElement *sourcesElement,
    XMLConvertor *convertor) const
{
    QStringList sourcesNames = this->sourcesNames();
    for (const auto& sourceName : sourcesNames) {
        QDomElement sourceElement = sourcesElement->ownerDocument().
            createElement("source");
        writeSingleSourceElement(sourceName, &sourceElement, convertor);
        sourcesElement->appendChild(sourceElement);
    }
}

void TemplateSourcesManager::writeSingleSourceElement(
    const QString &sourceName,QDomElement *sourceElement,
    XMLConvertor *convertor) const
{
    QString type = sourceType(sourceName);
    SourcePoolMap::const_iterator i = sourcePools_.find(type);
    if (i == sourcePools_.end()) {
        THROW_EX(Exception, "Source type " << type.toStdString() <<
             " is not registered\n")
    }
    std::unique_ptr<SourceXMLConvertor> sourceConvertor =
        i->second->sourceXMLConvertor(sourceName);
    sourceConvertor->writeSourceData(sourceElement, convertor);
    QDomElement assignedFieldsElement = sourceElement->ownerDocument().
        createElement("assigned_fields");
    QStringList assignedFields = i->second->assignedFieldNames(sourceName);
    for (const auto& fieldName : assignedFields) {
        QDomElement fieldElement = sourceElement->ownerDocument().
            createElement("assigned_field");
        QDomText fieldNameElement = sourceElement->ownerDocument().
            createTextNode(fieldName);
        fieldElement.appendChild(fieldNameElement);
        assignedFieldsElement.appendChild(fieldElement);
    }
    sourceElement->appendChild(assignedFieldsElement);
}
