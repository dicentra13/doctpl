#include "templateformatsettingsdialog.h"

#include "../fields/formatfieldsettingswidget.h"
#include "../fields/textrenderinfo.h"

#include <QDialogButtonBox>
#include <QVBoxLayout>

TemplateFormatSettingsDialog::TemplateFormatSettingsDialog(QWidget *parent)
    : QDialog(parent)
{
    widget_ = new FormatFieldSettingsWidget(this, TextRenderInfo::globals());
    widget_->setEnabled(true);
    buttons_ = new QDialogButtonBox(QDialogButtonBox::Ok |
        QDialogButtonBox::Cancel, Qt::Horizontal, this);
    connect(buttons_, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttons_, SIGNAL(rejected()), this, SLOT(reject()));

    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);
    layout->addWidget(widget_);
    layout->addWidget(buttons_);
}
