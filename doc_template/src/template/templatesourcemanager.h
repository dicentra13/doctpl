#ifndef TEMPLATESOURCEMANAGER_H
#define TEMPLATESOURCEMANAGER_H

//class Source;

/*
class NonExistentSourceName : public std::exception {
public:
    NonExistentSourceName() {};
    ~NonExistentSourceName() {};
};

class DuplicateSourceName : public std::exception {
public:
    DuplicateSourceName() {};
    ~DuplicateSourceName() {};
};
*/

/*
// работа с источниками
// источник идентифицируется именем, которое д.б. уникальным
// NonExistentSourceName, DuplicateSourceName
void addSource(const QString &name, Source *source);
void deleteSource(const QString &name, bool deleteFields = false);
Source *getSource(const QString &name) const;
bool sourceExists(const QString &name) const; // сущ. источник с таким именем
void renameSource(const QString &oldName, const QString &newName);
// назначение полей источникам
// проверка соответствия типов производится извне
void assignFieldToSource(const QString &fieldName,
    const QString &sourceName, const QString &fieldAfterName);
void assignFieldToSource(const QStringList &fieldNames,
    const QString &sourceName, const QString &fieldAfterName);
void removeFieldFromSource(const QString &fieldName,
    const QString &sourceName);
void removeFieldFromSource(const QStringList &fieldNames,
    const QString &sourceName);
QStringList getSources() const;
QStringList getAssignedFields(const QString &sourceName) const;
*/

/*
std::map<QString, Source *> sources_;
// список групп полей, ассоциированных с источниками
std::list<std::pair<QString, QStringList>> fieldGroups_;
*/

#endif // TEMPLATESOURCEMANAGER_H
