#include "xmltablesource.h"

#include "xmltablesourcedialog.h"
#include "xmltablesourcexmlconvertor.h"
#include "../../../exception.h"

#include <QFile>
#include <QXmlQuery>

QString XMLTableSource::directory_ = "";

XMLTableSource::XMLTableSource(const QString &name, const QString &filename)
    : TableSource("xml", name)
    , filename_(filename)
{}

XMLTableSource::~XMLTableSource() {}

void XMLTableSource::setFileName(const QString &filename) {
    filename_ = filename;
}

QString XMLTableSource::fileName() const {
    return filename_;
}

void XMLTableSource::setQueryList(QStringList queryStrings) {
    queries_ = queryStrings;
    numberOfColumns_ = (size_t)queries_.size();
}

QStringList XMLTableSource::queryList() const {
    return queries_;
}

std::unique_ptr<SourceDialog> XMLTableSource::settingsDialog(
    SourceNameValidator validator, QWidget *parent)
{
    return std::unique_ptr<SourceDialog>(new XMLTableSourceDialog(validator, this,
        parent));
}

std::unique_ptr<SourceXMLConvertor> XMLTableSource::getXMLConvertor() const {
    return std::unique_ptr<SourceXMLConvertor>(
        new XMLTableSourceXMLConvertor(this));
}

bool XMLTableSource::init(const QString &ID) {
    cursor_ = 0;
    result_.clear();
    QFile file(directory_ + filename_);
    bool res = file.open(QIODevice::ReadOnly);
    if (res) {
        QXmlQuery query;
        query.setFocus(&file);
        QStringList::iterator i = queries_.begin();
        for (; i != queries_.end(); i++) {
            query.bindVariable("ID", QVariant(ID));
            query.setQuery(*i);
            QStringList result;
            res = query.evaluateTo(&result);
            if (!res) {
                THROW_EX(Exception, "Query " <<
                     i->toStdString() << " could not be evaluated")
            }

            result_.push_back(result);
        }
    }
    file.close();
    return res;
}

QStringList XMLTableSource::getNext() {
    QStringList row;
    std::vector<QStringList>::const_iterator i = result_.begin();
    for (; i != result_.end(); i++) {
        row.push_back(i->at(cursor_));
    }
    cursor_++;
    return row;
}

bool XMLTableSource::atEnd() {
    return cursor_ == result_.front().size();
}

bool XMLTableSource::finalize() {
    return true;
}
