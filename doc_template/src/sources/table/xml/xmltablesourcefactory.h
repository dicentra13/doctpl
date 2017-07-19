#ifndef XMLTABLESOURCEFACTORY_H
#define XMLTABLESOURCEFACTORY_H

#include "../tablesourcefactory.h"

class QWidget;

class XMLTableSourceFactory : public TableSourceFactory {

    Q_OBJECT

public:
    XMLTableSourceFactory(QWidget *dialogParent);
    virtual ~XMLTableSourceFactory() {}

protected:
    virtual std::unique_ptr<TableSourceDialog> newSourceDialog(
        SourceNameValidator validator) const;
    virtual std::unique_ptr<TableSourceXMLConvertor>
        newSourceXMLConvertor() const;
private:
    QWidget *dialogParent_;
};

#endif // XMLTABLESOURCEFACTORY_H
