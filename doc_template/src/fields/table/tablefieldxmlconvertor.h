#ifndef TABLEFIELDXMLCONVERTOR_H
#define TABLEFIELDXMLCONVERTOR_H

#include "../fieldxmlconvertor.h"
#include "../textrenderinfo.h"

#include <QString>
#include <QDomNodeList>

#include <vector>

class TableFieldController;
class TemplateLayout;
class XMLConvertor;
class QDomElement;

class TableFieldXMLConvertor : public FieldXMLConvertor {
public:
    TableFieldXMLConvertor(TemplateLayout *tl, XMLConvertor *convertor,
            TableFieldController *controller)
        : FieldXMLConvertor(tl, convertor)
        , controller_(controller)
    {}
    virtual ~TableFieldXMLConvertor() {}
    std::vector<double> rowHeights() const { return rowHeights_;}
    std::vector<double> columnWidths() const { return columnWidths_;}
    TextRenderInfoList columnFormatting() const {
        return columnFormatting_;
    }
    std::vector<bool> isLocalFormatting() const {
        return isLocalFormatting_;
    }
    QString text(int row, int col) const {
        return texts_[row - 1][col - 1];
    }

protected:
    virtual std::unique_ptr<Field> processSpecificElement(size_t page,
        const QDomElement &fieldElement);
    virtual void writeSpecificElement(const QString &fieldName,
        QDomElement *fieldElement);
private:
    void processRowElement(const QDomElement &rowElement);
    void processColumnElement(const QDomElement &columnElement);

    TableFieldController *controller_;
    std::vector<double> rowHeights_;
    std::vector<double> columnWidths_;
    TextRenderInfoList columnFormatting_;
    std::vector<bool> isLocalFormatting_;
    std::vector<std::vector<QString> > texts_;
};

#endif // TABLEFIELDXMLCONVERTOR_H
