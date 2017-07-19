#include "xmltextsource.h"

#include "xmltextsourcedialog.h"
#include "xmltextsourcexmlconvertor.h"
#include "../../../exception.h"

#include <QFile>
#include <QXmlQuery>
#include <QVariant>

QString XMLTextSource::directory_ = "";

XMLTextSource::XMLTextSource(const QString &name, const QString &filename)
    : TextSource("xml", name)
    , filename_(filename)
{}

XMLTextSource::~XMLTextSource() {}

void XMLTextSource::setFileName(const QString &filename) {
    filename_ = filename;
}

QString XMLTextSource::fileName() const {
    return filename_;
}

void XMLTextSource::setQuery(const QString &query) {
    query_ = query;
}

QString XMLTextSource::query() const {
    return query_;
}

std::unique_ptr<SourceDialog> XMLTextSource::settingsDialog(
    SourceNameValidator validator, QWidget *parent)
{
    return std::unique_ptr<SourceDialog>(new XMLTextSourceDialog(validator, this,
        parent));
}

std::unique_ptr<SourceXMLConvertor> XMLTextSource::getXMLConvertor() const {
    return std::unique_ptr<SourceXMLConvertor>(
        new XMLTextSourceXMLConvertor(this));
}

bool XMLTextSource::init(const QString &ID) {
    QFile file(directory_ + filename_);
    bool res = file.open(QIODevice::ReadOnly);
    if (res) {
        QXmlQuery query;
        query.bindVariable("ID", QVariant(ID));
        query.setFocus(&file);
        query.setQuery(query_);
        res = query.evaluateTo(&result_);
        if (!res) {
            THROW_EX(Exception, "Query " <<
                 query_.toStdString() << " could not be evaluated" <<
                 " or returned too much results")
        }
    }
    file.close();
    return res;
}

QString XMLTextSource::getText() {
    return result_;
}

bool XMLTextSource::finalize() {
    return true;
}

