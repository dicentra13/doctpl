#include "textfieldwidget.h"

#include "textfieldcontroller.h"
#include "../formatfieldsettingswidget.h"
#include "textfield.h"
#include "../../page.h"

#include <QLineEdit>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QHBoxLayout>
#include <QLabel>

TextFieldWidget::TextFieldWidget(TextFieldController *controller,
    QWidget *parent)
        : FieldSettingsWidget(parent)
        , controller_(controller)
{
    formatSettings_ = new FormatFieldSettingsWidget(this);
    text_ = new QLineEdit(this);
    localSettingsSelector_ = new QCheckBox(this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);
    layout->addWidget(commonSettingsWidget());
    QHBoxLayout *l = new QHBoxLayout;
    l->addWidget(localSettingsSelector_);
    l->addWidget(new QLabel(tr("FIELD_WIDGET_USE_LOCAL_SETTINGS"), this));
    layout->addLayout(l);

    layout->addWidget(formatSettings_);
    layout->addWidget(text_);

    connect(text_, SIGNAL(textChanged(QString)),
        this, SLOT(textChanged_(QString)));
    connect(localSettingsSelector_, SIGNAL(clicked(bool)),
        this, SLOT(settingsSwitched(bool)));

    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
}

TextFieldWidget::~TextFieldWidget() {
    // удаляется в QWidget
}

void TextFieldWidget::setEnabled(bool isEnabled) {
    FieldSettingsWidget::setEnabled(isEnabled);
    formatSettings_->setEnabled(isEnabled);
    text_->setEnabled(isEnabled);
}

void TextFieldWidget::setField(TextField *field) {
    if (field) {
        setEnabled(true);
        setName(field->name());
        setPageNumber(field->page()->num());
        setX(field->x());
        setY(field->y());
        setWidth(field->width());
        setHeight(field->height());
        localSettingsSelector_->setChecked(field->isLocalFormatting());
        formatSettings_->setData(field->textRenderInfo());
        if (field->isLocalFormatting()) {
            formatSettings_->setEnabled(true);
        } else {
            formatSettings_->setEnabled(false);
        }
        text_->setText(field->text());
    } else {
        setEnabled(false);
    }
}

void TextFieldWidget::textChanged_(const QString &newText) {
    emit textChanged(newText);
}

void TextFieldWidget::settingsSwitched(bool isLocal) {
    if (isLocal) {
        formatSettings_->setEnabled(true);
    } else {
        formatSettings_->setEnabled(false);
    }
    emit this->settingsSetToLocal(isLocal);
}

void TextFieldWidget::setFormatting(TextRenderInfo *format) {
    formatSettings_->setData(format);
}
