#ifndef TABLEFIELDCONTROLLER_H
#define TABLEFIELDCONTROLLER_H

//FIXME имена файлов сделать с _. А то без пробелов они плохо читаются
#include "../fieldcontroller.h"

#include <QString>
#include <map>
#include <memory>

class TableField;
class TableFieldWidget;
class TableFieldCreationDialog;
class TableFieldXMLConvertor;
class QWidget;
class QDockWidget;
class QAction;

class TableFieldController : public FieldController {

    Q_OBJECT

public:
    /** если эти виджеты 0, то контроллер создается без оконного режима
      слоты не связываются
      */
    TableFieldController(QWidget *dialogParent, QDockWidget *settingsWidget);
    virtual ~TableFieldController();

    virtual FieldCreationDialog *creationDialog();
    virtual FieldSettingsWidget *settingsWidget();
    virtual Field *currentField() const;

    void setCurrentField(TableField *f);
    std::unique_ptr<Field> createField(TableFieldCreationDialog *dialog);
    std::unique_ptr<Field> createField(TableFieldXMLConvertor *convertor, size_t page);
    // FIXME меня пугает void метод без параметров, который грузит какие-то данные в виджет
    // нельзя ли сюда передавать виджет или данные?
    virtual void loadDataToWidget();
    virtual void setLayout(TemplateLayout *tl);
    // FIXME не самое удачное название. Его можно понять двумя или тримя способами. fieldCreationButton
    virtual QAction *newFieldAction();
    TableField *field(const QString &name) const;
    // FIXME У тебя поле само дергает этот метод. Если я не посмотрю на реализацию поля
    // то точно решу, что этим методом надо удалять поля. Если от этого метода нельзяизбавиться,
    // обязательно напиши тут комментарий о том, что самостоятельно ввзывать этот метод не надо.
    void removeField(const QString &name);
    FieldXMLConvertor *getXMLConvertor(XMLConvertor *);
    void renameField(const QString &oldName, const QString &newName);
public slots:
    /** эти слоты связываются только при создании в оконном режиме */
    void addRow(size_t, double);
    void removeRow(size_t);
    void resizeRow(size_t, double);
    void addColumn(size_t, double);
    void removeColumn(size_t);
    void resizeColumn(size_t, double);
    void currentRowChanged(size_t);
    void currentColumnChanged(size_t);
    void textChanged(size_t, size_t, QString);
    void switchSettingsToLocal(size_t, bool);
    virtual void widthChanged(double);
    virtual void heightChanged(double);
private:
    TableFieldWidget *settingsWidget_;
    TableField *currentField_;
    QWidget *dialogParent_;
    QDockWidget *dockWidget_;

    /** Владельцем полей является QGraphicsScene из TemplateLayout
      (ограничение Qt)). Поэтому поле сообщает контроллеру о своем уничтожении
      из своего деструктора.
      */
    std::map<QString, TableField *> fields_;
    QAction *newFieldAct_;
};

#endif // TABLEFIELDCONTROLLER_H
