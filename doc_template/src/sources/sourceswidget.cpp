#include "sourceswidget.h"

#include "../template/templatesourcesmanager.h"
#include "sourcepool.h"
#include "source.h"
#include "sourcedialog.h"

#include <QListWidget>
#include <QSplitter>
#include <QAction>
#include <QComboBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QStringList>
#include <QToolBar>
#include <QList>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QPushButton>
#include <QCloseEvent>

const int SourcesWidget::sourceSelectorWidth_ = 200;
const int SourcesWidget::sourceLabelWidth_ = 80;

SourcesWidget::SourcesWidget(TemplateSourcesManager *sourcesManager,
        QWidget *parent)
    : QDialog(parent)
    , currentSourceName_("")
    , currentSourceType_("")
    , sourcesManager_(sourcesManager)
{
    sourceSelector_ = new QComboBox(this);
    connect(sourceSelector_, SIGNAL(currentIndexChanged(int)),
        this, SLOT(currentSourceIndexChanged(int)));

    QStringList supportedTypes = sourcesManager_->supportedTypes();
    for (QStringList::const_iterator t = supportedTypes.begin();
        t != supportedTypes.end(); t++)
    {
        QStringList subtypes = sourcesManager_->supportedSubtypes(*t);
        for (QStringList::const_iterator st = subtypes.begin();
            st != subtypes.end(); st++)
        {
            QIcon icon = sourcesManager_->subtypeIcon(*t, *st);
            QString text = sourcesManager_->subtypeIconTip(*t, *st);
            QAction *newSourceAct = new QAction(icon, text, this);
            newSourceAct->setEnabled(true);
            connect(newSourceAct, SIGNAL(triggered()),
                this, SLOT(newSource()));
            newSourceActions_.push_back(newSourceAct);
            newSourceTypesSubtypes_.push_back(std::pair<QString, QString>(
                *t, *st));
        }
    }

    editSourceAct_ = new QAction(tr("SOURCES_EDIT"), this);
    connect(editSourceAct_, SIGNAL(triggered()),
        this, SLOT(editCurrentSource()));
    deleteSourceAct_ = new QAction(tr("SOURCES_DELETE"), this);
    connect(deleteSourceAct_, SIGNAL(triggered()),
        this, SLOT(deleteCurrentSource()));

    addToSource_ = new QPushButton(QIcon(":/icons/images/go-previous.png"),
        tr("ADD_TO_SOURCE"), this);
    connect(addToSource_, SIGNAL(clicked()),
        this, SLOT(addFieldsToCurrentSource()));
    removeFromSource_ = new QPushButton(QIcon(":/icons/images/go-next.png"),
        tr("REMOVE_FROM_SOURCE"), this);
    connect(removeFromSource_, SIGNAL(clicked()),
        this, SLOT(removeFieldsFromCurrentSource()));

    fieldsWidgetsSplitter_ = new QSplitter(this);
    assignedFields_ = new QListWidget(this);
    assignedFields_->setDragDropMode(QAbstractItemView::InternalMove);
    assignedFields_->setSelectionMode(QAbstractItemView::MultiSelection);
    unassignedFields_ = new QListWidget(this);
    unassignedFields_->setDragDropMode(QAbstractItemView::NoDragDrop);
    unassignedFields_->setSelectionMode(QAbstractItemView::MultiSelection);

    sourceToolbar_ = new QToolBar(this);
    generateLayout();
    loadSourcesFromManager();
}

void SourcesWidget::generateLayout() {
    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);
    QLabel *sourceLabel = new QLabel(tr("SOURCE") + ":", this);
    sourceLabel->setMinimumWidth(SourcesWidget::sourceLabelWidth_);
    sourceToolbar_->addWidget(sourceLabel);
    sourceSelector_->setMinimumWidth(SourcesWidget::sourceSelectorWidth_);
    sourceToolbar_->addWidget(sourceSelector_);
    sourceToolbar_->addSeparator();
    std::vector<QAction *>::iterator i = newSourceActions_.begin();
    for (; i != newSourceActions_.end(); i++) {
        sourceToolbar_->addAction(*i);
    }
    sourceToolbar_->addSeparator();
    sourceToolbar_->addAction(editSourceAct_);
    sourceToolbar_->addAction(deleteSourceAct_);
    layout->addWidget(sourceToolbar_);

    QWidget *fieldActionsWidget = new QWidget(this);
    QVBoxLayout *fieldActionsLayout = new QVBoxLayout(fieldActionsWidget);
    fieldActionsWidget->setLayout(fieldActionsLayout);
    fieldActionsLayout->addStretch();
    fieldActionsLayout->addWidget(addToSource_);
    fieldActionsLayout->addWidget(removeFromSource_);
    fieldActionsLayout->addStretch();

    QWidget *assignedWidget = new QWidget(this);
    QVBoxLayout *assignedLayout = new QVBoxLayout(assignedWidget);
    assignedWidget->setLayout(assignedLayout);
    assignedLayout->addWidget(new QLabel(
        tr("SOURCES_WIDGET_ASSIGNED_FIELDS"), assignedWidget));
    assignedLayout->addWidget(assignedFields_);
    QWidget *unassignedWidget = new QWidget(this);
    QVBoxLayout *unassignedLayout = new QVBoxLayout(unassignedWidget);
    unassignedWidget->setLayout(unassignedLayout);
    unassignedLayout->addWidget(new QLabel(
        tr("SOURCES_WIDGET_UNASSIGNED_FIELDS"), unassignedWidget));
    unassignedLayout->addWidget(unassignedFields_);

    fieldsWidgetsSplitter_->addWidget(assignedWidget);
    fieldsWidgetsSplitter_->addWidget(fieldActionsWidget);
    fieldsWidgetsSplitter_->addWidget(unassignedWidget);
    layout->addWidget(fieldsWidgetsSplitter_);

    setWindowTitle(tr("SOURCES_WIDGET_TITLE"));
}

void SourcesWidget::loadSourcesFromManager() {
    sourceSelector_->addItems(sourcesManager_->sourcesNames());
    if (sourceSelector_->count() == 0) {
        sourceSelector_->setEnabled(false);
        editSourceAct_->setEnabled(false);
        deleteSourceAct_->setEnabled(false);
        addToSource_->setEnabled(false);
        removeFromSource_->setEnabled(false);
    } else {
        sourceSelector_->setCurrentIndex(0);
    }
}

SourcesWidget::~SourcesWidget() {
    // все удаляется автоматически
}

void SourcesWidget::closeEvent(QCloseEvent *event) {
    if (!currentSourceName_.isEmpty() && !currentSourceType_.isEmpty()) {
        reassignFieldsInCurrentSource();
    }
    event->accept();
}

void SourcesWidget::currentSourceIndexChanged(int newIndex) {
    if (newIndex != -1) {
        if (!currentSourceName_.isEmpty() && !currentSourceType_.isEmpty()) {
            reassignFieldsInCurrentSource();
        }
        currentSourceName_ = sourceSelector_->currentText();
        currentSourceType_ = sourcesManager_->sourceType(
            currentSourceName_);
        loadDataToFieldWidget(assignedFields_,
            sourcesManager_->assignedFieldNames(currentSourceName_));
        loadDataToFieldWidget(unassignedFields_,
            sourcesManager_->unassignedFieldNames(currentSourceType_));
        updateAddRemoveActions();
    }
}

void SourcesWidget::newSource() {
    size_t i = 0;
    for (; i < newSourceActions_.size(); i++) {
        if (sender() == newSourceActions_[i])
            break;
    }
    if (i < newSourceActions_.size()) {
        QString newSourceName = sourcesManager_->newSource(
            newSourceTypesSubtypes_[i].first,
            newSourceTypesSubtypes_[i].second);
        if (!newSourceName.isEmpty()) {
            if (sourceSelector_->count() == 0) {
                sourceSelector_->setEnabled(true);
                editSourceAct_->setEnabled(true);
                deleteSourceAct_->setEnabled(true);
            }
            sourceSelector_->addItem(newSourceName);
            sourceSelector_->setCurrentIndex(sourceSelector_->count() - 1);
        }
    } else {
        /** FIXME кинуть exception */
    }
}

void SourcesWidget::editCurrentSource() {
    std::unique_ptr<SourceDialog> dialog =
        sourcesManager_->sourceDialog(currentSourceName_, this);
    dialog->exec();
    if (dialog->result() == QDialog::Accepted && dialog->name() !=
        currentSourceName_)
    {
        // имя поменялось
        sourcesManager_->renameSource(currentSourceName_, dialog->name());
        currentSourceName_ = dialog->name();
        sourceSelector_->setItemText(sourceSelector_->currentIndex(),
            currentSourceName_);
    }
}

void SourcesWidget::deleteCurrentSource() {
    sourcesManager_->deleteSource(currentSourceName_);
    currentSourceName_.clear();
    currentSourceType_.clear();
    sourceSelector_->removeItem(sourceSelector_->currentIndex());
    if (sourceSelector_->count() == 0) {
        sourceSelector_->setEnabled(false);
        editSourceAct_->setEnabled(false);
        deleteSourceAct_->setEnabled(false);
        addToSource_->setEnabled(false);
        removeFromSource_->setEnabled(false);
        clearFieldWidget(assignedFields_);
        clearFieldWidget(unassignedFields_);
    }
}

void SourcesWidget::addFieldsToCurrentSource() {
    int i = 0;
    while (i < unassignedFields_->count()) {
        if (unassignedFields_->item(i)->isSelected()) {
            assignedFields_->addItem(unassignedFields_->takeItem(i));
        } else {
            i++;
        }
    }
    updateAddRemoveActions();
}

void SourcesWidget::removeFieldsFromCurrentSource() {
    int i = 0;
    while (i < assignedFields_->count()) {
        if (assignedFields_->item(i)->isSelected()) {
            unassignedFields_->addItem(assignedFields_->takeItem(i));
        } else {
            i++;
        }
    }
    updateAddRemoveActions();
}

void SourcesWidget::reassignFieldsInCurrentSource() {
    QStringList fieldNames;
    for (int i = 0; i < assignedFields_->count(); i++) {
        fieldNames.push_back(assignedFields_->item(i)->text());
    }
    sourcesManager_->assignFieldsToSource(currentSourceName_,fieldNames);
}

void SourcesWidget::renameCurrentSourceInWidget() {
    sourceSelector_->setItemText(sourceSelector_->currentIndex(),
        currentSourceName_);
}

void SourcesWidget::updateAddRemoveActions() {
    addToSource_->setEnabled(unassignedFields_->count() > 0);
    removeFromSource_->setEnabled(assignedFields_->count() > 0);
}

void SourcesWidget::loadDataToFieldWidget(QListWidget *fieldWidget,
    QStringList names)
{
    clearFieldWidget(fieldWidget);
    for (QStringList::iterator i = names.begin(); i != names.end(); i++) {
        QListWidgetItem *item = new QListWidgetItem(*i);
        fieldWidget->addItem(item);
    }
}

void SourcesWidget::clearFieldWidget(QListWidget *fieldWidget) {
    while (fieldWidget->count() > 0) {
        QListWidgetItem *item = fieldWidget->takeItem(0);
        delete item;
    }
}
