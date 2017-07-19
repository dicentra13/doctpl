#ifndef TABLESOURCE_H
#define TABLESOURCE_H

#include "../../sources/source.h"

#include <QString>
#include <QStringList>

#include <vector>
#include <memory>

class SourceDialog;
class TableField;

class TableSource : public Source {
public:
    virtual ~TableSource() {};
    QString subtype() const {
        return subtype_;
    }
    virtual QStringList assignedFieldsNames() const;
    virtual size_t assignedFieldsCount() const {
        return assignedFields_.size();
    }
    virtual int maxAssignedFieldsCount() const { return -1; }
    virtual bool process(const QString &ID);

    virtual std::unique_ptr<SourceDialog> settingsDialog(
        SourceNameValidator validator, QWidget *parent) = 0;

    virtual bool isFieldAssigned(const QString &fieldName) const;
    bool assignFields(std::list<TableField *> fields);
    std::list<TableField *> takeFields();
    virtual bool removeField(const QString &fieldName);

    void setNumberOfColumns(size_t num) {
        numberOfColumns_ = num;
    }
    size_t numberOfColumns() const {
        return numberOfColumns_;
    }
protected:
    virtual bool init(const QString &ID) = 0;
    virtual QStringList getNext() = 0;
    virtual bool atEnd() = 0;
    /** FIXME
      init возвращает scoped cursor
      переделать на курсор */
    virtual bool finalize() = 0;

    TableSource(const QString &subtype, const QString &name)
        : Source("table", name)
        , subtype_(subtype)
        , numberOfColumns_(1)
    {}
    QString subtype_;

    std::vector<TableField *> assignedFields_;
    size_t numberOfColumns_;
};

#endif // TABLESOURCE_H
