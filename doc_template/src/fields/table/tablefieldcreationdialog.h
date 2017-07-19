#ifndef TABLEFIELDCREATIONDIALOG_H
#define TABLEFIELDCREATIONDIALOG_H

#include "../fieldcreationdialog.h"

#include <QString>

class TableFieldController;
class TableField;
class QWidget;
class QSpinBox;

class TableFieldCreationDialog : public FieldCreationDialog {

    Q_OBJECT

public:
    TableFieldCreationDialog(TableFieldController *controller, QWidget *parent);
    virtual ~TableFieldCreationDialog() {};
    /**
      Проверяет наличие незаполненных полей во вводе
      Возвращает false, если есть незаполненные поля
      */
    virtual bool checkNonEmptyInput();
    /**
      Возвращает контроллер поля
      */
    virtual FieldController *controller();
    size_t numberOfRows() const;
    size_t numberOfColumns() const;
protected:
    // возвращает созданное поле
    // данный запрос перенаправляется в контроллер
    virtual std::unique_ptr<Field> createField();
private:
    TableFieldController *controller_;
    QSpinBox *rowCount_, *columnCount_;
};

#endif // TABLEFIELDCREATIONDIALOG_H
