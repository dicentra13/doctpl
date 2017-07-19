#ifndef SOURCEFACTORY_H
#define SOURCEFACTORY_H

#include <QObject>
#include <QIcon>
#include <QString>

class SourceFactory : public QObject {

    Q_OBJECT

public:
    QString subtypeName() const { return subtype_; }
    QString iconHint() const { return iconHint_; }
    QIcon icon() const { return icon_; }
protected:
    SourceFactory(const QString &subtype)
        : QObject(0)
        , subtype_(subtype)
    {}

    QString subtype_;
    QIcon icon_;
    QString iconHint_;
};

#endif // SOURCEFACTORY_H
