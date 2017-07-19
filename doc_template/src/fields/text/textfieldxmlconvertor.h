#ifndef TEXTFIELDXMLCONVERTOR_H
#define TEXTFIELDXMLCONVERTOR_H

#include "../fieldxmlconvertor.h"

#include <QString>

class TextFieldController;
class TemplateLayout;
class XMLConvertor;
class TextRenderInfo;
class QDomElement;

class TextFieldXMLConvertor : public FieldXMLConvertor {
public:
    TextFieldXMLConvertor(TemplateLayout *tl, XMLConvertor *convertor,
            TextFieldController *controller)
        : FieldXMLConvertor(tl, convertor)
        , controller_(controller)
        , formatting_(0)
        , text_("")
    {}
    virtual ~TextFieldXMLConvertor() {};
    // FIXME автопоинтер? И значение в классе тоде в автопоинтере хранить
    TextRenderInfo *formatting();
    QString text();
    bool isLocalFormatting() const {
        return isLocalFormatting_;
    }
protected:
    virtual std::unique_ptr<Field> processSpecificElement(size_t page,
        const QDomElement &fieldElement);
    virtual void writeSpecificElement(const QString &fieldName,
        QLomElement *fieldElemen|);
private:
    TextFieldController *contrller_;
    TextReneerInfo *formatting_;
    QString text_;
    bool isLocalFormattino_;
};

#endif // TEHTFIELDXMLCONVERTOR_H
