#include "tablefieldwidget.h"

#include "../formatfieldsettingswidget.h"
#include "tablefield.h"
#include "tablefieldcontroller.h"
#include "../textrenderinfo.h"
#include "../../page.h"
#include "tableaddrowcolumndialog.h"

#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDoubleValidator>
#include <QLabel>
#include <QCheckBox>

TableFieldWidget::TableFieldWidget(TableFieldController *controller,
        QWidget *parent)
    : FieldSettingsWidget(parent)
    , controller_(controller)
    , isSignalsBlocked_(false)
{
    numberOfRows_ = new QLabel(this);
    numberOfRows_->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    addRow_ = new QPushButton(tr("TABLE_WIDGET_ADD_ROW_BUTTON"), this);
    connect(addRow_, SIGNAL(clicked()), this, SLOT(addRow()));

    numberOfColumns_ = new QLabel(this);
    numberOfColumns_->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    addColumn_ = new QPushButton(tr("TABLE_WIDGET_ADD_COLUMN_BUTTON"), this);
    connect(addColumn_, SIGNAL(clicked()), this, SLOT(addColumn()));

    rowSelector_ = new QSpinBox(this);
    connect(rowSelector_, SIGNAL(valueChanged(int)),
        this, SLOT(currentRowChanged(int)));
    rowHeight_ = new QDoubleSpinBox(this);
    rowHeight_->setMinimum(0.0);
    rowHeight_->setSingleStep(0.5);
    rowHeight_->setSuffix(tr("UNIT_MM"));

    connect(rowHeight_, SIGNAL(valueChanged(double)), this,
        SLOT(currentRowHeightEdited(double)));
    columnSelector_ = new QSpinBox(this);
    connect(columnSelector_, SIGNAL(valueChanged(int)),
        this, SLOT(currentColumnChanged(int)));
    columnWidth_ = new QDoubleSpinBox(this);
    columnWidth_->setMinimum(0.0);
    columnWidth_->setSingleStep(0.5);
    columnWidth_->setSuffix(tr("UNIT_MM"));

    connect(columnWidth_, SIGNAL(valueChanged(double)), this,
        SLOT(currentColumnWidthEdited(double)));
    removeRow_ = new QPushButton(tr("TABLE_WIDGET_REMOVE_ROW_BUTTON"), this);
    connect(removeRow_, SIGNAL(clicked()), this, SLOT(removeRow()));
    removeColumn_ = new QPushButton(tr("TABLE_WIDGET_REMOVE_COLUMN_BUTTON"),
        this);
    connect(removeColumn_, SIGNAL(clicked()), this, SLOT(removeColumn()));

    formatSettings_ = new FormatFieldSettingsWidget(this);
    text_ = new QLineEdit(this);
    isLocalFormatting_ = new QCheckBox(this);
    connect(text_, SIGNAL(textChanged(QString)),
        this, SLOT(textChanged_(QString)));
    connect(isLocalFormatting_, SIGNAL(clicked(bool)),
        this, SLOT(settingsSwitchedToLocal(bool)));
    generateLayout();
}

TableFieldWidget::~TableFieldWidget() {
    // автоматически
}

void TableFieldWidget::setEnabled(bool isEnabled) {
    FieldSettingsWidget::setEnabled(isEnabled);
    numberOfRows_->setEnabled(isEnabled);
    numberOfColumns_->setEnabled(isEnabled);
    rowHeight_->setEnabled(isEnabled);
    columnWidth_->setEnabled(isEnabled);
    rowSelector_->setEnabled(isEnabled);
    columnSelector_->setEnabled(isEnabled);
    addRow_->setEnabled(isEnabled);
    removeRow_->setEnabled(isEnabled);
    addColumn_->setEnabled(isEnabled);
    removeColumn_->setEnabled(isEnabled);
    formatSettings_->setEnabled(isEnabled);
    text_->setEnabled(isEnabled);
}

void TableFieldWidget::generateLayout() {
    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);
    layout->addWidget(commonSettingsWidget());
    QHBoxLayout *l = new QHBoxLayout;
    l->addWidget(new QLabel(tr("TABLE_WIDGET_NUM_OF_ROWS"), this));
    l->addWidget(numberOfRows_);
    l->addWidget(addRow_);
    l->addWidget(new QLabel(tr("TABLE_WIDGET_NUM_OF_COLUMNS"), this));
    l->addWidget(numberOfColumns_);
    l->addWidget(addColumn_);
    layout->addLayout(l);

    QGridLayout *cellSelectorLayout = new QGridLayout;
    cellSelectorLayout->addWidget(new QLabel(tr("TABLE_WIDGET_ROW"), this), 0, 0);
    cellSelectorLayout->addWidget(rowSelector_, 0, 1);
    cellSelectorLayout->addWidget(new QLabel(tr("TABLE_WIDGET_ROW_HEIGHT"), this), 0, 2);
    cellSelectorLayout->addWidget(rowHeight_, 0, 3);
    cellSelectorLayout->addWidget(removeRow_, 0, 4);
    cellSelectorLayout->addWidget(new QLabel(tr("TABLE_WIDGET_COLUMN"), this), 1, 0);
    cellSelectorLayout->addWidget(columnSelector_, 1, 1);
    cellSelectorLayout->addWidget(new QLabel(tr("TABLE_WIDGET_COLUMN_WIDTH"), this), 1, 2);
    cellSelectorLayout->addWidget(columnWidth_, 1, 3);
    cellSelectorLayout->addWidget(removeColumn_, 1, 4);
    layout->addLayout(cellSelectorLayout);

    l = new QHBoxLayout;
    l->addWidget(isLocalFormatting_);
    l->addWidget(new QLabel(tr("FIELD_WIDGET_USE_LOCAL_SETTINGS"), this));
    layout->addLayout(l);
    layout->addWidget(formatSettings_);
    layout->addWidget(text_);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
}

void TableFieldWidget::setField(TableField *field) {
    if (field) {
        // TODO вынести куда-нибудь в базовые классы
        setEnabled(true);
        setName(field->name());
        setPageNumber(field->page()->num());
        setX(field->x());
        setY(field->y());
        setWidth(field->width());
        setHeight(field->height());
        isSignalsBlocked_ = true;
        QString s;
        s.setNum(field->numberOfRows());
        numberOfRows_->setText(s);
        s.setNum(field->numberOfColumns());
        numberOfColumns_->setText(s);
        rowHeight_->setValue(field->rowHeight(field->currentRow()));
        columnWidth_->setValue(field->columnWidth(field->currentColumn()));
        rowSelector_->setMinimum(1);
        rowSelector_->setMaximum(field->numberOfRows());
        rowSelector_->setValue(field->currentRow());
        columnSelector_->setMinimum(1);
        columnSelector_->setMaximum(field->numberOfColumns());
        columnSelector_->setValue(field->currentColumn());
        removeRow_->setEnabled(field->numberOfRows() > 1);
        removeColumn_->setEnabled(field->numberOfColumns() > 1);
        formatSettings_->setData(field->columnFormatting(
            field->currentColumn()));
        isLocalFormatting_->setChecked(field->isLocalFormatting(field->currentColumn()));
        if (!field->isLocalFormatting(field->currentColumn())) {
            formatSettings_->setEnabled(false);
        } else {
            formatSettings_->setEnabled(true);
        }
        text_->setText(field->text(field->currentRow(),
            field->currentColumn()));
        isSignalsBlocked_ = false;
    } else {
        setEnabled(false);
    }
}

void TableFieldWidget::setNumberOfRows(size_t num) {
    isSignalsBlocked_ = true;
    QString s;
    s.setNum(num);
    numberOfRows_->setText(s);
    if ((size_t) rowSelector_->value() > num) {
        rowSelector_->setValue(1);
    }
    rowSelector_->setMinimum(1);
    rowSelector_->setMaximum(num);
    isSignalsBlocked_ = false;
}

void TableFieldWidget::setCurrentRow(size_t row) {
    isSignalsBlocked_ = true;
    if (row > (size_t) rowSelector_->maximum()) {
        rowSelector_->setValue(1);
    } else {
        rowSelector_->setValue(row);
    }
    isSignalsBlocked_ = false;
}

void TableFieldWidget::setCurrentRowHeight(double height) {
    isSignalsBlocked_ = true;
    rowHeight_->setValue(height);
    isSignalsBlocked_ = false;
}

void TableFieldWidget::setNumberOfColumns(size_t num) {
    isSignalsBlocked_ = true;
    QString s;
    s.setNum(num);
    numberOfColumns_->setText(s);
    if ((size_t)columnSelector_->value() > num)
        columnSelector_->setValue(1);
    columnSelector_->setMinimum(1);
    columnSelector_->setMaximum(num);
    isSignalsBlocked_ = false;
}

void TableFieldWidget::setCurrentColumn(size_t col) {
    isSignalsBlocked_ = true;
    if (col > (size_t) columnSelector_->maximum()) {
        columnSelector_->setValue(1);
    } else {
        columnSelector_->setValue(col);
    }
    isSignalsBlocked_ = false;
}

void TableFieldWidget::setCurrentColumnWidth(double width) {
    isSignalsBlocked_ = true;
    columnWidth_->setValue(width);
    isSignalsBlocked_ = false;
}

void TableFieldWidget::setCurrentColumnFormatSettings(
    TextRenderInfo *formatSettings, bool isLocal)
{
    formatSettings_->setData(formatSettings);
    if (isLocal) {
        formatSettings_->setEnabled(true);
        isLocalFormatting_->setChecked(true);
    } else {
        formatSettings_->setEnabled(false);
        isLocalFormatting_->setChecked(false);
    }
}

void TableFieldWidget::textChanged_(QString text) {
    if (!isSignalsBlocked_)
        emit textChanged(rowSelector_->value(),
            columnSelector_->value(), text);
}

void TableFieldWidget::setText(const QString &text) {
    isSignalsBlocked_ = true;
    text_->setText(text);
    isSignalsBlocked_ = false;
}

void TableFieldWidget::addRow() {
    TableAddRowColumnDialog *d = new TableAddRowColumnDialog(
        TableAddRowColumnDialog::Row, rowSelector_->maximum() + 1, this);
    if (d->exec() == QDialog::Accepted) {
        controller_->addRow(d->pos(), d->size());
    }
}

void TableFieldWidget::removeRow() {
    emit removeRow(rowSelector_->value());
}

void TableFieldWidget::addColumn() {
    TableAddRowColumnDialog *d = new TableAddRowColumnDialog(
        TableAddRowColumnDialog::Column, columnSelector_->maximum() + 1, this);
    if (d->exec() == QDialog::Accepted) {
        controller_->addColumn(d->pos(), d->size());
    }
}

void TableFieldWidget::removeColumn() {
    emit removeColumn(columnSelector_->value());
}

void TableFieldWidget::currentRowChanged(int val) {
    if (!isSignalsBlocked_)
        emit currentRowChanged((size_t) val);
}

void TableFieldWidget::currentColumnChanged(int val) {
    if (!isSignalsBlocked_)
        emit currentColumnChanged((size_t) val);
}

void TableFieldWidget::currentRowHeightEdited(double val) {
    if (!isSignalsBlocked_) {
        emit resizeRow(rowSelector_->value(), val);
    }
}

void TableFieldWidget::currentColumnWidthEdited(double val) {
    if (!isSignalsBlocked_) {
        emit resizeColumn(columnSelector_->value(), val);
    }
}

void TableFieldWidget::settingsSwitchedToLocal(bool isLocal) {
    if (isLocal) {
        formatSettings_->setEnabled(true);
    } else {
        formatSettings_->setEnabled(false);
    }
    emit switchSettingsToLocal(columnSelector_->value(), isLocal);
}

void TableFieldWidget::setFormatting(TextRenderInfo *format) {
    formatSettings_->setData(format);
}
