#include "textsource.h"

#include "../../fields/text/textfield.h"

QStringList TextSource::assignedFieldsNames() const {
    std::vector<TextField *>::const_iterator i = assignedFields_.begin();
    QStringList names;
    for (; i != assignedFields_.end(); i++) {
        names.push_back((*i)->name());
    }
    return names;
}

bool TextSource::process(const QString &ID) {
    bool res = true;
    if (!assignedFields_.empty()) {
        res = init(ID);
        if (res) {
            std::vector<TextField *>::iterator i;
            for (i = assignedFields_.begin(); i != assignedFields_.end(); i++) {
                (*i)->clear();
            }
            i = assignedFields_.begin();
            while (res && i != assignedFields_.end()) {
                res = (*i)->setText(getText());
                (*i)->update();
                i++;
            }
            if (res) {
                res = finalize();
            }
        }
    }
    return res;
}

bool TextSource::isFieldAssigned(const QString &fieldName) const {
    bool isFound = false;
    std::vector<TextField *>::const_iterator i = assignedFields_.begin();
    for (; !isFound && i != assignedFields_.end(); i++) {
        isFound = (*i)->name() == fieldName;
    }
    return isFound;
}

bool TextSource::assignFields(std::list<TextField *> fields) {
    assignedFields_.clear();
    assignedFields_.assign(fields.begin(), fields.end());
    return true;
}

std::list<TextField *> TextSource::takeFields() {
    std::list<TextField *> fields;
    fields.assign(assignedFields_.begin(), assignedFields_.end());
    assignedFields_.clear();
    return fields;
}

bool TextSource::removeField(const QString &fieldName) {
    bool isFound = false;
    std::vector<TextField *>::iterator i = assignedFields_.begin();
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

