#include "fieldtools.h"

#include "documenttemplate.h"
#include "fields/fieldcontroller.h"
#include "fields/fieldcreationdialog.h"
#include "fields/fieldsettingswidget.h"
#include "template/templatelayout.h"
#include "fields/field.h"

#include <QWidget>
#include <QToolBar>
#include <QMenu>
#include <QComboBox>
#include <QAction>
#include <QSignalMapper>
#include <QLabel>

FieldTools::FieldTools(QWidget *parent)
    : QObject(parent)
    , parent_(parent)
    , isCurrentFieldChanging_(false)
{
    fieldToolBar_ = new QToolBar(tr("FIELD"), parent_);
    fieldToolBar_->addWidget(new QLabel("  " + tr("FIELD") + ":  ",
        fieldToolBar_));

    fieldSelector_ = new QComboBox(fieldToolBar_);
    connect(fieldSelector_, SIGNAL(currentIndexChanged(QString)),
        this, SLOT(fieldChanged(QString)));
    fieldToolBar_->addWidget(fieldSelector_);
    fieldToolBar_->addSeparator();
    deleteFieldAct_ = fieldToolBar_->addAction(tr("FIELD_DELETE"));
    deleteFieldAct_->setStatusTip(tr("FIELD_DELETE_TIP"));
    connect(deleteFieldAct_, SIGNAL(triggered()), this, SLOT(deleteField()));
    // menu
    fieldMenu_ = new QMenu(tr("FIELD"), parent_);
    fieldMenu_->addAction(deleteFieldAct_);
}

FieldTools::~FieldTools() {
    if (!parent_) {
        delete fieldToolBar_;
        delete fieldMenu_;
    }
    // иначе удаляется родительским объектом
}

QToolBar *FieldTools::toolBar() {
    return fieldToolBar_;
}

QMenu *FieldTools::menu() {
    return fieldMenu_;
}

/**
  FIXME поправить с проверкой дубликатов
 */



void FieldTools::currentDocumentChanged(DocumentTemplate *currentDoc) {
    currentDocument_ = currentDoc;
    if (currentDocument_) {
        Field *curField = currentDocument_->layout()->field(
            currentDocument_->currentField());
        if (curField)
            curField->setCurrent(true);
        while (!newFieldActions_.empty()) {
            fieldToolBar_->removeAction(newFieldActions_.back());
            fieldMenu_->removeAction(newFieldActions_.back());
            newFieldActions_.pop_back();
        }
        newFieldActions_ = currentDocument_->newFieldActions();
        std::vector<QAction *>::iterator ai = newFieldActions_.begin();
        QAction *nextAct = deleteFieldAct_;
        for (; ai != newFieldActions_.end(); ai++) {
            fieldToolBar_->insertAction(nextAct, *ai);
            fieldMenu_->insertAction(nextAct, *ai);
            nextAct = *ai;
        }
    }
    updateFieldActions();
}

void FieldTools::currentFieldChanged() {  
    isCurrentFieldChanging_ = true;
    int index = fieldSelector_->findText(currentDocument_->currentField(),
        Qt::MatchExactly);
    if (index >= 0) {
        fieldSelector_->setCurrentIndex(index);
    } else {
        currentDocument_->setCurrentField(currentDocument_->fieldOutOfRange());
        fieldSelector_->setCurrentIndex(0);
    }
    deleteFieldAct_->setEnabled(currentDocument_->currentField() !=
        currentDocument_->fieldOutOfRange());
    isCurrentFieldChanging_ = false;
}

void FieldTools::deleteField() {

}

void FieldTools::fieldChanged(QString newName) {
    if (!isCurrentFieldChanging_) {
        currentDocument_->setCurrentField(newName);
        deleteFieldAct_->setEnabled(currentDocument_->currentField() !=
            currentDocument_->fieldOutOfRange());
    }
}

void FieldTools::updateFieldActions() {
    isCurrentFieldChanging_ = true;
    if (currentDocument_) {
        setNewFieldActionsEnabled(currentDocument_->numberOfPages() > 0);
        fieldSelector_->clear();
        QStringList names = currentDocument_->fieldNames();
        fieldSelector_->addItems(names);
        if (currentDocument_->numberOfFields() == 0) {
            fieldSelector_->setCurrentIndex(0);
            fieldSelector_->setEnabled(false);
            deleteFieldAct_->setEnabled(false);
        } else {
            fieldSelector_->setEnabled(true);
            currentFieldChanged();
        }
    } else {
        fieldSelector_->setEnabled(false);
        setNewFieldActionsEnabled(false);
        deleteFieldAct_->setEnabled(false);
        fieldMenu_->menuAction()->setEnabled(false);
    }

    isCurrentFieldChanging_ = false;
}

void FieldTools::updatePageInformation() {
    updateFieldActions();
}

void FieldTools::currentFieldNameChanged(QString name) {
    isCurrentFieldChanging_ = true;
    fieldSelector_->setItemText(fieldSelector_->currentIndex(), name);
    isCurrentFieldChanging_ = false;
}

void FieldTools::setNewFieldActionsEnabled(bool isEnabled) {
    std::vector<QAction *>::iterator i = newFieldActions_.begin();
    for (; i != newFieldActions_.end(); i++)
        (*i)->setEnabled(isEnabled);
}
