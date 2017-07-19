#include "fieldcreationdialog.h"

#include "fieldcontroller.h"
#include "field.h"
#include "commonfieldsettingswidget.h"
#include "../exception.h"

#include <QWidget>
#include <QDialogButtonBox>
#include <QMessageBox>

FieldCreationDialog::FieldCreationDialog(QWidget *parent)
    : QDialog(parent)
{
    widget_ = new CommonFieldSettingsWidget(this);
    buttons_ = new QDialogButtonBox(this);
    buttons_ = new QDialogButtonBox(QDialogButtonBox::Ok |
        QDialogButtonBox::Cancel, Qt::Horizontal, this);
    connect(buttons_, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttons_, SIGNAL(rejected()), this, SLOT(reject()));
}

FieldCreationDialog::~FieldCreationDialog() {
    // дочерние виджеты удалятся автоматически
}

/**
  Возвращает виджет с общими настройками
  Дочерние классы могут разместить его в своей layout требуемым способом
  */
QWidget *FieldCreationDialog::commonSettingsWidget() {
    return widget_;
}

/**
  Возвращает набор кнопок, связанных с нужными слотами
  Дочерние классы могут разместить его в своей layout требуемым способом
  */
QDialogButtonBox *FieldCreationDialog::buttons() {
    return buttons_;
}

/**
  Проверяет наличие незаполненных полей во вводе
  Возвращает false, если есть незаполненные поля
  Устанавливает фокус ввода на первое незаполненное поле
  */
bool FieldCreationDialog::checkNonEmptyInput() {
    bool result = false;
    if (widget_->isNameClear()) {
        widget_->resetName();
    } else if (widget_->isXClear()) {
        widget_->resetX();
    } else if (widget_->isYClear()) {
        widget_->resetY();
    } else if (widget_->isWidthClear()) {
        widget_->resetWidth();
    } else if (widget_->isHeightClear()) {
        widget_->resetHeight();
    } else {
        // ни одного пустого поля
        result = true;
    }
    return result;
}

/**
  Возвращает созданное поле
  */
std::unique_ptr<Field> FieldCreationDialog::createdField() {
    return std::move(createdField_);
}

bool FieldCreationDialog::setName(const QString &name) {
    return widget_->setName(name);
}

void FieldCreationDialog::resetName() {
    widget_->resetName();
}

QString FieldCreationDialog::name() const {
    return widget_->name();
}

bool FieldCreationDialog::isNameClear() const {
    return widget_->isNameClear();
}

void FieldCreationDialog::setPages(size_t count) {
    widget_->setPages(count);
}

bool FieldCreationDialog::setPageNumber(int pageNumber) {
    return widget_->setPageNumber(pageNumber);
}

int FieldCreationDialog::pageNumber() const {
    return widget_->pageNumber();
}

bool FieldCreationDialog::setX(double x) {
    return widget_->setX(x);
}

void FieldCreationDialog::resetX() {
    widget_->resetX();
}

double FieldCreationDialog::x() const {
    return widget_->x();
}

bool FieldCreationDialog::isXClear() const {
    return widget_->isXClear();
}

bool FieldCreationDialog::setY(double y) {
    return widget_->setY(y);
}

void FieldCreationDialog::resetY() {
    widget_->resetY();
}

double FieldCreationDialog::y() const {
    return widget_->y();
}

bool FieldCreationDialog::isYClear() const {
    return widget_->isYClear();
}

bool FieldCreationDialog::setWidth(double width) {
    return widget_->setWidth(width);
}

void FieldCreationDialog::resetWidth() {
    widget_->resetWidth();
}

double FieldCreationDialog::width() const {
    return widget_->width();
}

bool FieldCreationDialog::isWidthClear() const {
    return widget_->isWidthClear();
}

bool FieldCreationDialog::setHeight(double height) {
    return widget_->setHeight(height);
}

void FieldCreationDialog::resetHeight() {
    widget_->resetHeight();
}

double FieldCreationDialog::height() const {
    return widget_->height();
}

bool FieldCreationDialog::isHeightClear() const {
    return widget_->isHeightClear();
}

/**
  Не требует замещения в производных классах
  */
void FieldCreationDialog::accept() {
    if (checkNonEmptyInput()) {
        try {
            createdField_ = createField();
            QDialog::accept();
        } catch (Exception &e) {
            QMessageBox::warning(this, tr("DLG_FIELD_CREATE"),
                e.what(), QMessageBox::Ok);
            QDialog::reject();
        }
    } else {
        QMessageBox::warning(this, tr("DLG_FIELD_CREATE"),
            tr("DLG_FIELD_CREATE_EMPTY_FIELDS_MESSAGE"), QMessageBox::Ok);
    }
}
