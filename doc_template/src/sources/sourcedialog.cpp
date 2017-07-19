#include "sourcedialog.h"

#include "source.h"

#include <QWidget>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QMessageBox>
#include <QLabel>
#include <QHBoxLayout>

SourceDialog::SourceDialog(SourceNameValidator validator,
        QWidget *parent)
    : QDialog(parent)
    , validator_(validator)
{
    createWidgets();
}

SourceDialog::SourceDialog(SourceNameValidator validator,
       Source *source, QWidget *parent)
   : QDialog(parent)
   , validator_(validator)
{
    createWidgets();
    name_->setText(source->name());
}

void SourceDialog::createWidgets() {
    nameWidget_ = new QWidget(this);
    name_ = new QLineEdit(nameWidget_);
    QHBoxLayout *layout = new QHBoxLayout(nameWidget_);
    nameWidget_->setLayout(layout);
    layout->addWidget(new QLabel(tr("SOURCE_NAME"), nameWidget_));
    layout->addWidget(name_);
    buttons_ = new QDialogButtonBox(QDialogButtonBox::Ok |
        QDialogButtonBox::Cancel, Qt::Horizontal, this);
    connect(buttons_, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttons_, SIGNAL(rejected()), this, SLOT(reject()));
}

QString SourceDialog::name() const {
    return name_->text();
}

void SourceDialog::setName(const QString &name) {
    name_->setText(name);
}

QWidget *SourceDialog::nameWidget() {
    return nameWidget_;
}

QDialogButtonBox *SourceDialog::buttons() {
    return buttons_;
}

bool SourceDialog::checkData() {
    if ((!source() || (source() && source()->name() != name_->text()))
        && !(validator_.checkNameIsAcceptable(name_->text())))
    {
        QMessageBox::warning(this, tr("SOURCE_DATA_VALIDATION"),
            tr("DLG_SOURCE_EXISTENT_SOURCE_NAME"), QMessageBox::Ok);
        return false;
    }
    return true;
}

bool SourceDialog::checkNonEmptyInput() {
    if (name_->text().isEmpty()) {
        name_->setFocus();
        QMessageBox::warning(this, tr("SOURCE_DATA_VALIDATION"),
            tr("DLG_SOURCE_EMPTY_INPUT"), QMessageBox::Ok);
        return false;
    }
    return true;
}
