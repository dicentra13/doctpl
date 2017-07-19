#include "documenttemplate.h"

#include "template/templateeditor.h"
#include "page.h"
#include "fields/field.h"
#include "template/templatelayout.h"
#include "template/templatesourcesmanager.h"
#include "template/templateview.h"
#include "fields/fieldcreationdialog.h"
#include "fields/fieldcontroller.h"


#include "fields/text/textfieldcontroller.h"
#include "fields/table/tablefieldcontroller.h"

#include "sources/sourcedialog.h"
#include "sources/sourcexmlconvertor.h"
#include "sources/sourcepool_t.h"

#include "sources/table/tablesource.h"
#include "fields/table/tablefield.h"
#include "sources/table/tablesourcefactory.h"

#include "sources/text/textsource.h"
#include "fields/text/textfield.h"
#include "sources/text/textsourcefactory.h"

#include "sources/table/xml/xmltablesourcefactory.h"
#include "sources/text/xml/xmltextsourcefactory.h"

#include <QSignalMapper>
#include <QAction>
#include <QDockWidget>

#include <memory>

#include <iostream>

/** Передача 0 в параметре app означает работу без оконного режима
    вид в TemplateLayout не создается, view() возвращает 0
 */
DocumentTemplate::DocumentTemplate(const QString &fileName, TemplateEditor *app)
    : fileName_(fileName)
    , app_(app)
    , sourcesManager_(0)
{
    if (app_) {
        layout_ = new TemplateLayout(fileName, app_->defaultViewMode());
        connect(layout_, SIGNAL(currentPageChanged(size_t)), this,
            SLOT(currentPageChanged(size_t)));
        connect(layout_, SIGNAL(currentFieldChanged(QString)), this,
            SLOT(currentFieldChanged(QString)));
        connect(layout_, SIGNAL(modified()), this, SLOT(modified_()));

        newFieldActionsMapper_ = new QSignalMapper(this);
        connect(newFieldActionsMapper_, SIGNAL(mapped(const QString &)),
            this, SLOT(newField(const QString &)));
    } else {
        layout_ = new TemplateLayout(fileName, 0);
    }
    init();
}

DocumentTemplate::~DocumentTemplate() {
    delete layout_;
    while (!fieldControllers_.empty()) {
        FieldController *fc = fieldControllers_.begin()->second;
        fieldControllers_.erase(fieldControllers_.begin());
        delete fc;
    }
}

bool DocumentTemplate::isModified() const {
    return layout_->isModified();
}

TemplateView *DocumentTemplate::view() {
    return layout_->view();
}

void DocumentTemplate::registerController(const QString &fieldType,
    FieldController *controller)
{
    fieldControllers_.insert(std::pair<QString, FieldController *>(fieldType,
        controller));
    controller->setLayout(layout_);
    if (app_) {
        connect(controller, SIGNAL(currentFieldNameChanged(QString)),
            this, SLOT(currentFieldNameChanged(QString)));
        // добавить action
        QAction *newFieldAct = controller->newFieldAction();
        connect(newFieldAct, SIGNAL(triggered()),
            newFieldActionsMapper_, SLOT(map()));
        newFieldActionsMapper_->setMapping(newFieldAct, fieldType);
    }
}

QStringList DocumentTemplate::supportedFieldTypes() const {
    QStringList fieldTypesList;
    std::map<QString, FieldController *>::const_iterator i;
    for (i = fieldControllers_.begin(); i != fieldControllers_.end(); i++) {
        fieldTypesList.push_back(i->first);
    }
    return fieldTypesList;
}

FieldController *DocumentTemplate::getController(const QString &fieldType) {
    std::map<QString, FieldController *>::iterator i = fieldControllers_.find(
        fieldType);
    return (i != fieldControllers_.end()) ? i->second : 0;
}

bool DocumentTemplate::addPage(size_t num, std::unique_ptr<Page> page) {
    bool result = layout_->addPage(num, std::move(page));
    if (result && app_) {
        layout_->setCurrentPage(num);
        std::map<QString, FieldController *>::iterator i;
        for (i = fieldControllers_.begin(); i != fieldControllers_.end(); i++)
            i->second->updatePages(numberOfPages());
    }
    return result;
}

bool DocumentTemplate::deletePage(size_t num) {
    bool result = false;
    if (numberOfPages() >= 1) {
        size_t nextCurrent;
        if (numberOfPages() == 1) {
            nextCurrent = pageOutOfRange();
        } else if (currentPage() == lastPage()) {
            nextCurrent = currentPage() - 1;
        } else {
            // текущая удалится, следующая получит номер текущей
            nextCurrent = currentPage();
        }
        result = layout_->deletePage(num);
        if (result && app_) {
            layout_->setCurrentPage(nextCurrent);
            std::map<QString, FieldController *>::iterator i;
            for (i = fieldControllers_.begin(); i != fieldControllers_.end(); i++)
                i->second->updatePages(numberOfPages());
        }
    }
    return result;
}

bool DocumentTemplate::pageExists(size_t num) const {
    return layout_ && layout_->pageExists(num);
}

bool DocumentTemplate::editPage(size_t num, size_t newNum, double width,
    double height, double dx, double dy)
{
    bool result = false;
    if (app_) {
        layout_->setCurrentPage(num);
    }
    if (num == newNum) {
        result = layout_->resizePage(num, width, height, dx, dy);
        if (result && app_)
            view()->adjustCurrentPage();
    } else if (layout_->pageExists(num) && layout_->pageExists(newNum)
        && layout_->resizePage(num, width, height, dx, dy))
    {
        layout_->movePage(num, newNum); // true, т.к. страницы существуют
        if (app_) {
            layout_->setCurrentPage(newNum);
            std::map<QString, FieldController *>::iterator i;
            for (i = fieldControllers_.begin(); i != fieldControllers_.end(); i++)
                i->second->updatePages(numberOfPages());
        }
        result = true;
    }
    return result;
}

size_t DocumentTemplate::firstPage() const {
    return layout_->firstPage();
}

size_t DocumentTemplate::lastPage() const {
    return layout_->lastPage();
}

/**
  Возвращает значение, которое не является допустимым номером страницы
  при любом количестве страниц (не зависит от количества страниц)
  */
size_t DocumentTemplate::pageOutOfRange() const {
    return layout_->pageOutOfRange();
}

size_t DocumentTemplate::numberOfPages() const {
    return layout_->numberOfPages();
}

size_t DocumentTemplate::currentPage() const {
    return layout_->currentPage();
}

bool DocumentTemplate::setCurrentPage(size_t num) {
    layout_->setCurrentPage(num);
    return layout_->currentPage() != layout_->pageOutOfRange();
}

bool DocumentTemplate::addField(const QString &name,
    std::unique_ptr<Field> field, size_t pageNum, double x, double y)
{
    QString fieldType = field.get()->fieldType();
    bool result = layout_->addField(name, std::move(field), pageNum, x, y);
    if (result) {
        layout_->setCurrentField(name);
        sourcesManager_->addField(fieldType, name);
    }
    return result;
}

/** TODO
    реализовать удаление
    удалять из sourcesManager_ тоже
  */
bool DocumentTemplate::deleteField(const QString &name) {
    return layout_->deleteField(name);
}

bool DocumentTemplate::fieldExists(const QString &name) const {
    return layout_->fieldExists(name);
}

QString DocumentTemplate::currentField() const {
    return layout_->currentField();
}

bool DocumentTemplate::setCurrentField(const QString &name) {
    layout_->setCurrentField(name);
    return layout_->currentField() != layout_->fieldOutOfRange();
}

QString DocumentTemplate::fieldOutOfRange() const {
    return layout_->fieldOutOfRange();
}

size_t DocumentTemplate::numberOfFields() const {
    return layout_->numberOfFields();
}
/**
  Первый элемент списка всегда fieldOutOfRange()
  */
QStringList DocumentTemplate::fieldNames() const {
    QStringList names;
    names.push_back(fieldOutOfRange());
    names.append(layout_->fieldNames());
    return names;
}


void DocumentTemplate::printToFile(const QString &ID, const QString &filename,
    std::vector<size_t> pages) const
{
    sourcesManager_->processSources(ID);
    layout_->printToFile(filename, pages);
}

void DocumentTemplate::currentPageChanged(size_t num) {
    if (num == pageOutOfRange() && layout_->numberOfPages() > 0) {
        layout_->setCurrentPage(1);
    }
    app_->updatePageInformation();
}

void DocumentTemplate::currentFieldChanged(QString) {
     app_->updateFieldInformation();
}

void DocumentTemplate::modified_() {
    app_->setCurrentDocumentModified();
}

void DocumentTemplate::newField(const QString &fieldType) {
    FieldController *controller =
        fieldControllers_.find(fieldType)->second;
    FieldCreationDialog *fd = controller->creationDialog();
    fd->setPages(numberOfPages());
    fd->setPageNumber(currentPage());
    bool result = (fd->exec() == QDialog::Accepted);
    if (result) {
        if (addField(fd->name(), fd->createdField(),
            fd->pageNumber(), fd->x(), fd->y()))
        {
            app_->updateFieldInformation();
        }
    }
}

void DocumentTemplate::currentFieldNameChanged(QString newName) {
    app_->updateCurrentFieldName(newName);
}

std::vector<QAction *> DocumentTemplate::newFieldActions() {
    std::vector<QAction *> actions;
    std::map<QString, FieldController *>::iterator i =
        fieldControllers_.begin();
    for (; i != fieldControllers_.end(); i++)
        actions.push_back(i->second->newFieldAction());
    return actions;
}

void DocumentTemplate::init() {
    QWidget *dialogParent = (app_) ? app_ : 0;
    QDockWidget *settingsDock = (app_) ? app_->fieldSettingsWidget() : 0;
    TableFieldController *tableController = new TableFieldController(
            dialogParent, settingsDock);
    registerController("table", tableController);
    TextFieldController *textController = new TextFieldController(
            dialogParent, settingsDock);
    registerController("text", textController);

    sourcesManager_ = new TemplateSourcesManager(this);

    std::unique_ptr<TSourcePool<TableSource, TableSourceFactory,
        TableField, TableFieldController> > tablePool(
        new TSourcePool<TableSource, TableSourceFactory,
        TableField, TableFieldController>("table", tableController));
    std::unique_ptr<TableSourceFactory> xmlTableFactory(
        new XMLTableSourceFactory(dialogParent));
    tablePool->registerTSourceFactory("xml", std::move(xmlTableFactory));
    sourcesManager_->addSourcePool("table", std::move(tablePool));

    std::unique_ptr<TSourcePool<TextSource, TextSourceFactory,
        TextField, TextFieldController> > textPool(
        new TSourcePool<TextSource, TextSourceFactory,
        TextField, TextFieldController>("text", textController));
    std::unique_ptr<TextSourceFactory> xmlTextFactory(
        new XMLTextSourceFactory(dialogParent));
    textPool->registerTSourceFactory("xml", std::move(xmlTextFactory));
    sourcesManager_->addSourcePool("text", std::move(textPool));
}
