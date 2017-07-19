#include "textfieldcreationdialog.h"

#include "textfieldcontroller.h"
#include "../field.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QMessageBox>

TextFieldCreationDialog::TextFieldCreationDialog(
    TextFieldController *controller, QWidget *parent)
        : FieldCreationDialog(parent)
        , controller_(controller)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);
    layout->addWidget(commonSettingsWidget());
    layout->addWidget(buttons());
    setWindowTitle(tr("DLG_TEXT_FIELD_TITLE"));
}

/**
  Проверяет наличие незаполненных полей во вводе
  Возвращает false, если есть незаполненные поля
  */
bool TextFieldCreationDialog::checkNonEmptyInput() {
    return FieldCreationDialog::checkNonEmptyInput();
}

/**
  Возвращает контроллер поля
  */
FieldController *TextFieldCreationDialog::controller() {
    return controller_;
}

std::unique_ptr<Field> TextFieldCreationDialog::createField() {
    return controller_->createField(this);
}
