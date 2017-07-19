#ifndef TEXTFIELDCREATIONDIALOG_H
#define TEXTFIELDCREATIONDIALOG_H

#include "../fieldcreationdialog.h"
//#include "textfield.h"

#include <QString>

class TextFieldController;
class TextField;
class QWidget;

class TextFieldCreationDialog : public FieldCreationDialog {
    // FIXME метод renameField ну просто обязан переименовывать поле. А он этогго не делает.
    // Пусть он меняет и им внутри поля и обяжи пользоваться им. Или сделай так, чтобы поле его вызывало,
    // но тогда называй fieldRenamed
    Q_OBJECT

public:
    TextFieldCreationDialog(TextFieldController *controller, QWidget *parent);
    virtual ~TextFieldCreationDialog() {};
    /**
      Проверяет наличие незаполненных полей во вводе
      Возвращает false, если есть незаполненные поля
      */
    virtual bool checkNonEmptyInput();
    /**
      Возвращает контроллер поля
      */
    virtual FieldController *controller();
protected:
    // возвращает созданное поле
    // данный запрос перенаправляется в контроллер
    virtual std::unique_ptr<Field> createField();
private:
    TextFieldController *controller_;
};

#endif // TEXTFIELDCREATEDIALOG_H
