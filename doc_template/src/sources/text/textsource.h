#ifndef TEXTSOURCE_H
#define TEXTSOURCE_H

#include "../../sources/source.h"

#include <QString>
#include <QStringList>

#include <vector>
#include <memory>

class SourceDialog;
class TextField;

class TextSource : public Source {
public:
    virtual ~TextSource() {};
    QString subtype() const {
        return subtype_;
    }
    virtual QStringList assignedFieldsNames() const;
    virtual size_t assignedFieldsCount() const {
        return assignedFields_.size();
    }
    /** если несколько, выводим во все одно и то же */
    virtual int maxAssignedFieldsCount() const { return -1; }
    virtual bool process(const QString &ID);

    virtual std::unique_ptr<SourceDialog> settingsDialog(
        SourceNameValidator validator, QWidget *parent) = 0;

    virtual bool isFieldAssigned(const QString &fieldName) const;
    bool assignFields(std::list<TextField *> fields);
    std::list<TextField *> takeFields();
    virtual bool removeField(const QString &fieldName);

protected:
    virtual bool init(const QString &ID) = 0;
    virtual QString getText() = 0;
    /** FIXME
      init возвращает scoped cursor
      переделать на курсор */
    virtual bool finalize() = 0;

    TextSource(const QString &subtype, const QString &name)
        : Source("text", name)
        , subtype_(subtype)
    {}

    QString subtype_;

    std::vector<TextField *> assignedFields_;
};

#endif // TEXTSOURCE_H
