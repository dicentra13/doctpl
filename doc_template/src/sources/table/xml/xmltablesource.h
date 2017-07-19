#ifndef XMLTABLESOURCE_H
#define XMLTABLESOURCE_H

#include "../tablesource.h"

#include <QString>
#include <QStringList>

#include <vector>

class SourceDialog;
class SourceXMLConvertor;

class XMLTableSource : public TableSource {
public:
    XMLTableSource(const QString &name, const QString &filename);
    virtual ~XMLTableSource();

    void setFileName(const QString &filename);
    QString fileName() const;
    void setQueryList(QStringList queryStrings);
    QStringList queryList() const;

    virtual std::unique_ptr<SourceDialog> settingsDialog(
        SourceNameValidator validator, QWidget *parent);
    virtual std::unique_ptr<SourceXMLConvertor> getXMLConvertor() const;

    static void setDirectory(const QString &path) {
        directory_ = path;
    }

protected:
    virtual bool init(const QString &ID);
    virtual QStringList getNext();
    virtual bool atEnd();
    virtual bool finalize();
private:
    QString filename_;
    QStringList queries_;

    static QString directory_;

    std::vector<QStringList> result_;
    size_t cursor_;
};

#endif // XMLTABLESOURCE_H
