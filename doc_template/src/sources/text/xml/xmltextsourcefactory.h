#ifndef XMLTEXTSOURCEFACTORY_H
#define XMLTEXTSOURCEFACTORY_H

#include "../textsourcefactory.h"

class QWidget;

class XMLTextSourceFactory : public TextSourceFactory {

    Q_OBJECT

public:
    XMLTextSourceFactory(QWidget *dialogParent);
    virtual ~XMLTextSourceFactory() {}

protected:
    virtual std::unique_ptr<TextSourceDialog> newSourceDialog(
        SourceNameValidator validator) const;
    virtual std::unique_ptr<TextSourceXMLConvertor>
        newSourceXMLConvertor() const;
private:
    QWidget *dialogParent_;
};

#endif // XMLTEXTSOURCEFACTORY_H
