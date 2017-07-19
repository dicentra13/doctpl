#include "tablefieldcontroller.h"

#include "tablefield.h"
#include "tablefieldwidget.h"
#include "tablefieldcreationdialog.h"
#include "../textrenderinfo.h"
#include "../../template/templatelayout.h"
#include "../../page.h"
#include "../../xmlconvertor.h"
#include "tablefieldxmlconvertor.h"
#include "../../exception.h"

#include <QWidget>
#include <QMessageBox>
#include <QDockWidget>
#include <QAction>
#include <QDomElement>

/** если эти виджеты 0, то контроллер создается без оконного режима
  слоты не связываются
  */
TableFieldController::TableFieldController(QWidget *dialogParent,
        QDockWidget *settingsWidget)
    : FieldController("TableField")
    , currentField_(0)
    , dialogParent_(dialogParent)
    , dockWidget_(settingsWidget)
{
    if (dialogParent_ && dockWidget_) {
        settingsWidget_ = new TableFieldWidget(this, settingsWidget);
        connect(settingsWidget_, SIGNAL(nameChanged(QString)),
            this, SLOT(nameChanged(QString)));
        connect(settingsWidget_, SIGNAL(pageChanged(size_t)),
            this, SLOT(pageChanged(size_t)));
        connect(settingsWidget_, SIGNAL(xPosChanged(double)),
            this, SLOT(xPosChanged(double)));
        connect(settingsWidget_, SIGNAL(yPosChanged(double)),
            this, SLOT(yPosChanged(double)));
        connect(settingsWidget_, SIGNAL(widthChanged(double)),
            this, SLOT(widthChanged(double)));
        connect(settingsWidget_, SIGNAL(heightChanged(double)),
            this, SLOT(heightChanged(double)));
        connect(settingsWidget_, SIGNAL(addColumn(size_t,double)),
            this, SLOT(addColumn(size_t,double)));
        connect(settingsWidget_, SIGNAL(addRow(size_t,double)),
            this, SLOT(addRow(size_t,double)));
        connect(settingsWidget_, SIGNAL(removeColumn(size_t)),
            this, SLOT(removeColumn(size_t)));
        connect(settingsWidget_, SIGNAL(removeRow(size_t)),
            this, SLOT(removeRow(size_t)));
        connect(settingsWidget_, SIGNAL(resizeColumn(size_t,double)),
            this, SLOT(resizeColumn(size_t,double)));
        connect(settingsWidget_, SIGNAL(resizeRow(size_t,double)),
            this, SLOT(resizeRow(size_t,double)));
        connect(settingsWidget_, SIGNAL(currentColumnChanged(size_t)),
            this, SLOT(currentColumnChanged(size_t)));
        connect(settingsWidget_, SIGNAL(currentRowChanged(size_t)),
            this, SLOT(currentRowChanged(size_t)));
        connect(settingsWidget_, SIGNAL(textChanged(size_t, size_t, QString)),
            this, SLOT(textChanged(size_t, size_t, QString)));
        connect(settingsWidget_, SIGNAL(switchSettingsToLocal(size_t,bool)),
            this, SLOT(switchSettingsToLocal(size_t,bool)));
        settingsWidget_->hide();
        newFieldAct_ = new QAction(QIcon(":/icons/images/fields/table.png"),
            tr(type_.toStdString().c_str()), this);
    } else {
        settingsWidget_ = 0;
    }
}

TableFieldController::~TableFieldController() {
    if (settingsWidget_) {
        settingsWidget_->setParent(0);
        delete settingsWidget_;
    }
}

FieldCreationDialog *TableFieldController::creationDialog() {
    return new TableFieldCreationDialog(this, dialogParent_);
}

FieldSettingsWidget *TableFieldController::settingsWidget() {
    return settingsWidget_;
}

Field *TableFieldController::currentField() const {
    return currentField_;
}

void TableFieldController::setCurrentField(TableField *f) {
    currentField_= f;
    if (settingsWidget_) {
        if (currentField_) {
            loadDataToWidget();
        } else {
            settingsWidget_->hide();
            dockWidget_->hide();
        }
    }
}

std::unique_ptr<Field> TableFieldController::createField(TableFieldCreationDialog *dialog)
{
    std::unique_ptr<Field> field;
    if (!checkName(dialog->name())) {
        QMessageBox::warning(dialog, tr("DLG_FIELD_CREATE"),
            tr("DLG_FIELD_CREATE_EXISTENT_NAME_MESSAGE"), QMessageBox::Ok);
    } else if (!checkGeometry(dialog->pageNumber(), dialog->x(), dialog->y(),
        dialog->width(), dialog->height()))
    {
        QMessageBox::warning(dialog, tr("DLG_FIELD_CREATE"),
            tr("DLG_FIELD_CREATE_INCORRECT_GEOMETRY_MESSAGE"), QMessageBox::Ok);
    } else {
        std::unique_ptr<TableField> newField(new TableField(this, dialog->width(),
            dialog->height(), dialog->numberOfRows(),
            dialog->numberOfColumns()));
        fields_.insert(std::pair<QString, TableField *>(dialog->name(),
            newField.get()));
        field.reset(newField.release());
    }
    return field;
}

std::unique_ptr<Field> TableFieldController::createField(
    TableFieldXMLConvertor *convertor,
    size_t page)
{
    if (!checkName(convertor->name())) {
        THROW_EX(Exception,
            tr("DLG_FIELD_CREATE_EXISTENT_NAME_MESSAGE").toStdString());
    } else if (!checkGeometry(page, convertor->x(), convertor->y(),
        convertor->width(), convertor->height()))
    {
        THROW_EX(Exception,
            tr("DLG_FIELD_CREATE_INCORRECT_GEOMETRY_MESSAGE").toStdString());
    } else {
        std::unique_ptr<TableField> newField(
            new TableField(this, convertor->width(), convertor->height(),
                convertor->rowHeights(), convertor->columnWidths(),
                convertor->columnFormatting(), convertor->isLocalFormatting()));
        fields_.insert(std::pair<QString, TableField *>(convertor->name(),
            newField.get()));
        for (size_t i = 1; i <= convertor->rowHeights().size(); i++) {
            for (size_t j = 1; j <= convertor->columnWidths().size(); j++) {
                newField->setText(i, j, convertor->text(i, j));
            }
        }
        return std::unique_ptr<Field>(newField.release());
    }
}

void TableFieldController::loadDataToWidget() {
    settingsWidget_->setField(currentField_);
    dockWidget_->setWidget(settingsWidget_);
    dockWidget_->show();
    settingsWidget_->show();
}

void TableFieldController::setLayout(TemplateLayout *tl) {
    FieldController::setLayout(tl);
    setCurrentField(0);
}

QAction *TableFieldController::newFieldAction() {
    return newFieldAct_;
}

void TableFieldController::addRow(size_t pos, double height) {
    if (1 <= pos && pos <= currentField_->numberOfRows() + 1 &&
        checkGeometry(currentField_->page()->num(),
        currentField_->x(), currentField_->y(),
        currentField_->width(), currentField_->height() + height))
    {
        currentField_->addRow(pos, height);
        currentField_->setCurrentRow(pos);
        currentField_->update();
        settingsWidget_->setHeight(currentField_->height());
        settingsWidget_->setNumberOfRows(currentField_->numberOfRows());
        settingsWidget_->setCurrentRow(pos);
        settingsWidget_->setCurrentRowHeight(currentField_->rowHeight(pos));
        settingsWidget_->setText(currentField_->text(
            currentField_->currentRow(), currentField_->currentColumn()));
        templateLayout_->setModified(true);
    }
}

void TableFieldController::removeRow(size_t num) {
    if (1 <= num && num <= currentField_->numberOfRows()) {
        currentField_->setCurrentRow(1);
        currentField_->deleteRow(num);
        currentField_->update();
        settingsWidget_->setNumberOfRows(currentField_->numberOfRows());
        settingsWidget_->setCurrentRow(1);
        settingsWidget_->setCurrentRowHeight(currentField_->rowHeight(1));
        settingsWidget_->setHeight(currentField_->height());
        settingsWidget_->setText(currentField_->text(
            currentField_->currentRow(), currentField_->currentColumn()));
        templateLayout_->setModified(true);
    }
}

void TableFieldController::resizeRow(size_t num, double height) {
    if (1 <= num && num <= currentField_->numberOfRows()) {
        if (height <= currentField_->rowHeight(num) ||
            (height > currentField_->rowHeight(num) &&
            checkGeometry(currentField_->page()->num(),
                currentField_->x(), currentField_->y(),
                currentField_->width(), currentField_->height() + height -
                currentField_->rowHeight(num))))
        {
            currentField_->resizeRow(num, height);
            settingsWidget_->setHeight(currentField_->height());
            settingsWidget_->setCurrentRowHeight(height);
            currentField_->update();
            templateLayout_->setModified(true);
        } else {
            settingsWidget_->setCurrentRowHeight(currentField_->rowHeight(num));
        }
    }
}

void TableFieldController::addColumn(size_t pos, double width) {
    if (1 <= pos && pos <= currentField_->numberOfColumns() + 1 &&
        checkGeometry(currentField_->page()->num(),
        currentField_->x(), currentField_->y(),
        currentField_->width() + width, currentField_->height()))
    {
        currentField_->addColumn(pos, width);
        currentField_->setCurrentColumn(pos);
        currentField_->update();
        settingsWidget_->setWidth(currentField_->width());
        settingsWidget_->setNumberOfColumns(currentField_->numberOfColumns());
        settingsWidget_->setCurrentColumn(pos);
        settingsWidget_->setCurrentColumnWidth(currentField_->columnWidth(pos));
        settingsWidget_->setText(currentField_->text(
            currentField_->currentRow(), currentField_->currentColumn()));
        settingsWidget_->setCurrentColumnFormatSettings(
            currentField_->columnFormatting(pos), currentField_->isLocalFormatting(pos));
        templateLayout_->setModified(true);
    }
}

void TableFieldController::removeColumn(size_t num) {
    if (1 <= num && num <= currentField_->numberOfColumns()) {
        currentField_->setCurrentColumn(1);
        currentField_->deleteColumn(num);
        currentField_->update();
        settingsWidget_->setCurrentColumn(1);
        settingsWidget_->setNumberOfColumns(currentField_->numberOfColumns());
        settingsWidget_->setCurrentColumnWidth(currentField_->columnWidth(1));
        settingsWidget_->setWidth(currentField_->width());
        settingsWidget_->setText(currentField_->text(
            currentField_->currentRow(), currentField_->currentColumn()));
        settingsWidget_->setCurrentColumnFormatSettings(
            currentField_->columnFormatting(1), currentField_->isLocalFormatting(1));
        templateLayout_->setModified(true);
    }
}

void TableFieldController::resizeColumn(size_t num, double width) {
    if (1 <= num && num <= currentField_->numberOfColumns()) {
        if (width <= currentField_->columnWidth(num) ||
            (width > currentField_->columnWidth(num) &&
            checkGeometry(currentField_->page()->num(),
                currentField_->x(), currentField_->y(),
                currentField_->width() + width -
                currentField_->columnWidth(num),
                currentField_->height())))
        {
            currentField_->resizeColumn(num, width);
            settingsWidget_->setWidth(currentField_->width());
            settingsWidget_->setCurrentColumnWidth(width);
            currentField_->update();
            templateLayout_->setModified(true);
        } else {
            settingsWidget_->setCurrentColumnWidth(
                currentField_->columnWidth(num));
        }
    }
}

void TableFieldController::currentRowChanged(size_t num) {
    if (1 <= num && num <= currentField_->numberOfRows()) {
        currentField_->setCurrentRow(num);
        currentField_->update();
        settingsWidget_->setCurrentRowHeight(currentField_->rowHeight(num));
        settingsWidget_->setText(currentField_->text(
            currentField_->currentRow(), currentField_->currentColumn()));
    }
}

void TableFieldController::currentColumnChanged(size_t num) {
    if (1 <= num && num <= currentField_->numberOfColumns()) {
        currentField_->setCurrentColumn(num);
        currentField_->update();
        settingsWidget_->setCurrentColumnWidth(currentField_->columnWidth(num));
        settingsWidget_->setText(currentField_->text(
            currentField_->currentRow(), currentField_->currentColumn()));
        settingsWidget_->setCurrentColumnFormatSettings(
            currentField_->columnFormatting(num), currentField_->isLocalFormatting(num));
    }
}

void TableFieldController::textChanged(size_t row, size_t col, QString text) {
    if (1 <= row && row <= currentField_->numberOfRows()
        && 1 <= col && col <= currentField_->numberOfColumns())
    {
        currentField_->setText(row, col, text);
        templateLayout_->setModified(true);
    }
}

void TableFieldController::widthChanged(double w) {
    if (currentField_->width() -
        currentField_->columnWidth(currentField_->numberOfColumns()) <= w)
    {
        FieldController::widthChanged(w);
        settingsWidget_->setCurrentColumnWidth(currentField_->columnWidth(
            currentField_->currentColumn()));
    } else {
        settingsWidget_->setWidth(w);
    }
}

void TableFieldController::heightChanged(double h) {
    if (currentField_->height() -
        currentField_->rowHeight(currentField_->numberOfRows()) <= h)
    {
        FieldController::heightChanged(h);
        settingsWidget_->setCurrentRowHeight(currentField_->rowHeight(
            currentField_->currentRow()));
    } else {
        settingsWidget_->setHeight(h);
    }
}

TableField *TableFieldController::field(const QString &name) const {
    std::map<QString, TableField *>::const_iterator i =
        fields_.find(name);
    return (i != fields_.end()) ? i->second : 0;
}

void TableFieldController::removeField(const QString &name) {
    fields_.erase(name);
}

FieldXMLConvertor *TableFieldController::getXMLConvertor(XMLConvertor *conv) {
    return new TableFieldXMLConvertor(templateLayout_, conv, this);
}

void TableFieldController::renameField(const QString &oldName,
    const QString &newName)
{
    std::map<QString, TableField *>::iterator i = fields_.find(oldName);
    if (i != fields_.end()){
        TableField *f = i->second;
        fields_.erase(i);
        fields_.insert(std::pair<QString, TableField *>(newName, f));
    } else {
        THROW_EX(Exception, "TableField required with unknown name");
    }
}

void TableFieldController::switchSettingsToLocal(size_t col, bool isLocal) {
    if (currentField_) {
        currentField_->switchLocalFormatting(col, isLocal);
        if (isLocal) {
            settingsWidget_->setFormatting(currentField_->columnFormatting(col));
        }
    }
}
