#ifndef XMLTEXTSOURCE_H
#define XMLTEXTSOURCE_H

#include "../textsource.h"

#include <QString>

class SourceDialog;
class SourceXMLConvertor;

class XMLTextSource : public TextSource {
public:
    XMLTextSource(const QString &name, const QString &filename);
    virtual ~XMLTextSource();

    void setFileName(const QString &filename);
    QString fileName() const;
    void setQuery(const QString &query);
    QString query() const;

    virtual std::unique_ptr<SourceDialog> settingsDialog(
        SourceNameValidator validator, QWidget *parent);
    virtual std::unique_ptr<SourceXMLConvertor> getXMLConvertor() const;

    static void setDirectory(const QString &path) {
        directory_ = path;
    }

protected:
    virtual bool init(const QString &ID);
    virtual QString getText();
    virtual bool finalize();
private:
    QString filename_;
    QString query_;

    QString result_;

    static QString directory_;
};

#endif // XMLTEXTSOURCE_H
