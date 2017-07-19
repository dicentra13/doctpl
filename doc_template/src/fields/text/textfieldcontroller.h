#ifndef TEXTFIELDCONTROLLER_H
#define TEXTFIELDCONTROLLER_H

#include "../fieldcontroller.h"

#include <QString>
#include <map>
#include <memory>

class TextField;
class TextFieldWidget;
class TextFieldCreationDialog;
class TextFieldXMLConvertor;
class QWidget;
class QDockWidget;
class QAction;

class TextFieldController : public FieldController {

    Q_OBJECT

public:
    /** если эти виджеты 0, то контроллер создается без оконного режима
      слоты не связываются
      */
    TextFieldController(QWidget *dialogParent, QDockWidget *settingsWidget);
    virtual ~TextFieldController();

    virtual FieldCreationDialog *creationDialog();
    virtual FieldSettingsWidget *settingsWidget();
    virtual Field *currentField() const;

    void setCurrentField(TextField *f);
    std::unique_ptr<Field> createField(TextFieldCreationDialog *dialog);
    std::unique_ptr<Field> createField(TextFieldXMLConvertor *convertor,
        size_t page);
    // FIXME кажеется он должен быть private
    virtual void loadDataToWidget();
    virtual void setLayout(TemplateLayout *tl);
    virtual QAction *newFieldAction();
    TextField *field(const QString &name) const;

    void removeField(const QString &name);
    FieldXMLConvertor *getXMLConvertor(XMLConvertor *);
    // FIXME метод renameField ну просто обязан переименовывать поле. А он этогго не делает.
    // Пусть он меняет и им внутри поля и обяжи пользоваться им. Или сделай так, чтобы поле его вызывало,
    // но тогда называй fieldRenamed
    void renameField(const QString &oldName, const QString &newName);
public slots:
    /** связывается только при создании в оконном режиме */
    void textChanged(QString);
    void setSettingsToLocal(bool);
private:
    TextFieldWidget *settingsWidget_;
    TextField *currentField_;
    QWidget *dialogParent_;
    QDockWidget *dockWidget_;
    QAction *newFieldAct_;

    std::map<QString, TextField *> fields_;
};

#endif // TEXTFIELDCONTROLLER_H
