#include "tablefieldcreationdialog.h"

#include "tablefieldcontroller.h"
#include "../field.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QSpinBox>
#include <QLabel>

TableFieldCreationDialog::TableFieldCreationDialog(
    TableFieldController *controller, QWidget *parent)
        : FieldCreationDialog(parent)
        , controller_(controller)
{
    QGridLayout *layout = new QGridLayout(this);
    setLayout(layout);
    layout->addWidget(commonSettingsWidget(), 0, 0, 1, 2);

    layout->addWidget(new QLabel(tr("DLG_TABLE_FIELD_NUM_OF_ROWS"), this), 1, 0);
    rowCount_ = new QSpinBox(this);
    rowCount_->setMinimum(1);
    layout->addWidget(rowCount_, 1, 1);

    layout->addWidget(new QLabel(tr("DLG_TABLE_FIELD_NUM_OF_COLUMNS"), this), 2, 0);
    columnCount_ = new QSpinBox(this);
    columnCount_->setMinimum(1);
    layout->addWidget(columnCount_, 2, 1);

    layout->addWidget(buttons(), 3, 0, 1, 2);
    setWindowTitle(tr("DLG_TABLE_FIELD_TITLE"));
}

/**
  Проверяет наличие незаполненных полей во вводе
  Возвращает false, если есть незаполненные поля
  */
bool TableFieldCreationDialog::checkNonEmptyInput() {
    bool res = FieldCreationDialog::checkNonEmptyInput();
    if (res) {
        if (rowCount_->text().isEmpty()) {
            rowCount_->setFocus();
            res = false;
        } else if (columnCount_->text().isEmpty()) {
            columnCount_->setFocus();
            res = false;
        }
    }
    return res;
}

/**
  Возвращает контроллер поля
  */
FieldController *TableFieldCreationDialog::controller() {
    return controller_;
}

size_t TableFieldCreationDialog::numberOfRows() const {
    return (size_t) rowCount_->value();
}

size_t TableFieldCreationDialog::numberOfColumns() const {
    return (size_t) columnCount_->value();
}

std::unique_ptr<Field> TableFieldCreationDialog::createField() {
    return controller_->createField(this);
}
