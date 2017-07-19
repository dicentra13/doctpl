#ifndef FIELDCONTROLLER_H
#define FIELDCONTROLLER_H

#include <QString>
#include <QObject>
#include <QStringList>

class QWidget;
class QAction;
class TemplateLayout;
class Field;
class FieldCreationDialog;
class FieldSettingsWidget;
class FieldXMLConvertor;
class XMLConvertor;

class FieldController : public QObject {

    Q_OBJECT

public:
    virtual ~FieldController() {};    
    virtual FieldCreationDialog *creationDialog() = 0;
    virtual FieldSettingsWidget *settingsWidget() = 0;
    // FIXME application знает, какое поле сейчас выделено. А контроллер не владее всеми полями,
    // зачем ему знать о том, какое сейчас текущее? Может быть от этого метода можно избавиться
    virtual Field *currentField() const = 0;
    virtual void loadDataToWidget();
    virtual void setLayout(TemplateLayout *tl) {
        templateLayout_ = tl;
    }
    virtual QAction *newFieldAction() = 0;
    // FIXME Казалось бы, что этот метода должен обновлять страницы. А он обновляет диалоги редактирования.
    // А это не может быть слотом у самх далогов, чтобы кидать сигнал и он не ходил через контроллер? Ему вроде бы все равно на количестов страниц.
    void updatePages(size_t count);
    virtual FieldXMLConvertor *getXMLConvertor(XMLConvertor *) = 0;
    virtual void renameField(const QString &oldName, const QString &newName)
        = 0;
signals:
    void currentFieldNameChanged(QString);
public slots:
    // FIXME было бы лучше, если бы в параметрах сигнала было бы и имя поля, которое поменялось,
    // может быть тогда и не надо будет много где иметь переменную currentField и следить, чтобы нигде не зыбать ее обновить.
    void nameChanged(const QString &name);
    void pageChanged(size_t number);
    void xPosChanged(double x);
    void yPosChanged(double y);
    virtual void widthChanged(double w);
    virtual void heightChanged(double h);
protected:
    explicit FieldController(const QString &type);
    bool checkGeometry(size_t pageNum, double x, double y,
        double width, double height);
    bool checkName(const QString &name);

    QString type_;
    TemplateLayout *templateLayout_;
};

#endif // FIELDCONTROLLER_H
