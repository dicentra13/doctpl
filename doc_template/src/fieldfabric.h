#ifndef FIELDFABRIC_H
#define FIELDFABRIC_H

#include <QString>
#include <QObject>
#include <QStringList>

class QWidget;
class QAction;
class Field;
class FieldController;
class FieldCreationDialog;
class FieldXMLConvertor;
class XMLConvertor;
class TemplateLayout;

class FieldFabric : public QObject {

    Q_OBJECT

public:
    virtual ~FieldFabric() {};
    virtual FieldCreationDialog *createCreationDialog(QWidget *parent) = 0;
    virtual FieldController *createFieldController(TemplateLayout *doc) = 0;
    virtual QAction *newFieldAction() = 0;
    virtual FieldXMLConvertor *getXMLConvertor(XMLConvertor *) = 0;
protected:
    FieldFabric(const QString &fieldType);

    QString fieldType_;
};

#endif // FIELDFABRIC_H
