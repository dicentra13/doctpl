#include "tablesource.h"

#include "../../fields/table/tablefield.h"

QStringList TableSource::assignedFieldsNames() const {
    std::vector<TableField *>::const_iterator i = assignedFields_.begin();
    QStringList names;
    for (; i != assignedFields_.end(); i++) {
        names.push_back((*i)->name());
    }
    return names;
}

bool TableSource::process(const QString &ID) {
    bool res = true;
    if (!assignedFields_.empty()) {
        res = init(ID);
        if (res) {
            std::vector<TableField *>::iterator i;
            for (i = assignedFields_.begin(); i != assignedFields_.end(); i++) {
                (*i)->clear();
            }
            i = assignedFields_.begin();
            while (res && !atEnd() && i != assignedFields_.end()) {
                while (res && !atEnd() && !((*i)->atEnd())) {
                    res = (*i)->appendData(getNext());
                }
                if (res && (*i)->atEnd() && !atEnd()) {
                    (*i)->update();
                    i++;
                }
            }
            if (res) {
                res = finalize();
            }
        }
    }
    return res;
}

bool TableSource::isFieldAssigned(const QString &fieldName) const {
    bool isFound = false;
    std::vector<TableField *>::const_iterator i = assignedFields_.begin();
    for (; !isFound && i != assignedFields_.end(); i++) {
        isFound = (*i)->name() == fieldName;
    }
    return isFound;
}

bool TableSource::assignFields(std::list<TableField *> fields) {
    assignedFields_.clear();
    assignedFields_.assign(fields.begin(), fields.end());
    return true;
}

std::list<TableField *> TableSource::takeFields() {
    std::list<TableField *> fields;
    fields.assign(assignedFields_.begin(), assignedFields_.end());
    assignedFields_.clear();
    return fields;
}

bool TableSource::removeField(const QString &fieldName) {
    bool isFound = false;
    std::vector<TableField *>::iterator i = assignedFields_.begin();
    while (!isFound && i != assignedFields_.end()) {
        if (!(isFound = (*i)->name() == fieldName)) {
            i++;
        }
    }
    if (isFound) {
        assignedFields_.erase(i);
    }
    return isFound;
}
