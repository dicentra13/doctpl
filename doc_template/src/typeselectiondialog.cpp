#include "typeselectiondialog.h"

#include <QComboBox>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

TypeSelectionDialog::TypeSelectionDialog(const QString &title,
        QStringList types, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(title);
    typeSelector_ = new QComboBox(this);
    typeSelector_->addItems(types);
    QDialogButtonBox *buttons_ = new QDialogButtonBox(QDialogButtonBox::Ok |
        QDialogButtonBox::Cancel, Qt::Horizontal, this);
    connect(buttons_, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttons_, SIGNAL(rejected()), this, SLOT(reject()));
    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);
    QHBoxLayout *l = new QHBoxLayout;
    l->addWidget(new QLabel(tr("DLG_TYPE_TYPE") + ":", this));
    l->addWidget(typeSelector_);
    layout->addLayout(l);
    layout->addWidget(buttons_);
}

TypeSelectionDialog::~TypeSelectionDialog() {
    // автоматически
}

QString TypeSelectionDialog::selectedType() const {
    return typeSelector_->currentText();
}
