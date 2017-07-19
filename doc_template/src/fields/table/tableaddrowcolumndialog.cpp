#include "tableaddrowcolumndialog.h"

#include <QDialogButtonBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QMessageBox>
#include <QDoubleValidator>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

TableAddRowColumnDialog::TableAddRowColumnDialog(Mode m, size_t maxPos,
        QWidget *parent)
    : QDialog(parent)
{
    posSelector_ = new QSpinBox(this);
    posSelector_->setMinimum(1);
    posSelector_->setMaximum(maxPos);
    posSelector_->setValue(1);
    size_ = new QLineEdit(this);
    QDoubleValidator *val = new QDoubleValidator(this);
    val->setBottom(0.0);
    size_->setValidator(val);
    buttons_ = new QDialogButtonBox(QDialogButtonBox::Ok |
        QDialogButtonBox::Cancel, Qt::Horizontal, this);
    connect(buttons_, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttons_, SIGNAL(rejected()), this, SLOT(reject()));

    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);
    QHBoxLayout *l = new QHBoxLayout;
    l->addWidget(new QLabel(tr("DLG_TABLE_ADD_ROW_COLUMN_NUMBER"), this));
    l->addWidget(posSelector_);
    layout->addLayout(l);
    l = new QHBoxLayout;
    if (m == Row) {
        l->addWidget(new QLabel(tr("DLG_TABLE_ADD_ROW_COLUMN_HEIGHT"), this));
        setWindowTitle(tr("DLG_TABLE_ADD_ROW_TITLE"));
    } else {
        l->addWidget(new QLabel(tr("DLG_TABLE_ADD_ROW_COLUMN_WIDTH"), this));
        setWindowTitle(tr("DLG_TABLE_ADD_COLUMN_TITLE"));
    }
    l->addWidget(size_);
    layout->addLayout(l);
    layout->addWidget(buttons_);
}

TableAddRowColumnDialog::~TableAddRowColumnDialog() {
    // автоматически
}

size_t TableAddRowColumnDialog::pos() const {
    return (size_t) posSelector_->value();
}

double TableAddRowColumnDialog::size() const {
    return size_->text().toDouble();
}

void TableAddRowColumnDialog::accept() {
    if (size_->text().isEmpty()) {
        size_->setFocus();
        QMessageBox::warning(this, tr("DLG_TABLE_ADD_ROW_COLUMN"),
            tr("DLG_TABLE_ADD_ROW_COLUMN_EMPTY_FIELDS_MESSAGE"),
            QMessageBox::Ok);
    } else
        QDialog::accept();
}
