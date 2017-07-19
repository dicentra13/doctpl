#ifndef SOURCE_H
#define SOURCE_H

#include "../template/sourcenamevalidator.h"

#include <QString>
#include <QStringList>

#include <memory>

class SourceDialog;
class SourceXMLConvertor;
class QWidget;

class Source {
public:
    virtual ~Source() {};
    QString type() const { return type_;}
    QString name() const { return name_;}
    void setName(const QString &name) {
        name_ = name;
    }

    virtual QStringList assignedFieldsNames() const = 0;
    virtual size_t assignedFieldsCount() const = 0;
    virtual int maxAssignedFieldsCount() const = 0; // -1, если нет ограничений
    virtual bool process(const QString &ID) = 0;
    virtual std::unique_ptr<SourceDialog> settingsDialog(
        SourceNameValidator validator, QWidget *parent) = 0;
    virtual std::unique_ptr<SourceXMLConvertor> getXMLConvertor() const = 0;

    virtual bool isFieldAssigned(const QString &fieldName) const = 0;
    virtual bool removeField(const QString &fieldName) = 0;

protected:
    Source(const QString &type, const QString &name)
        : name_(name)
        , type_(type)
    {}
    QString name_;
    QString type_;
};

#endif // SOURCE_H
