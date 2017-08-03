#include "page_toolbar.h"

//#include "pagesettingsdialog.h"

#include "view/view.h"

#include <doctpl/template.h>
#include <doctpl/layout.h>
#include <doctpl/page.h>

#include <QWidget>
#include <QString>
#include <QToolBar>
#include <QMenu>
#include <QLineEdit>
#include <QIntValidator>
#include <QLabel>
#include <QAction>
#include <QMessageBox>
#include <QDomElement>

#include <memory>

PageToolbar::PageToolbar(QWidget* parent)
    : QObject(parent)
    , currentView_(nullptr)
{
    toolBar_ = new QToolBar(tr("PAGE"), parent);
    toolBar_->addWidget(
        new QLabel("  " + tr("PAGE") + ":  ", toolBar_));

    validator_ = new QIntValidator(this);
    validator_->setBottom(1);

    selector_ = new QLineEdit(toolBar_);
    selector_->setValidator(validator_);
    connect(
        selector_, SIGNAL(editingFinished()),
        this, SLOT(onSelectorValueChanged()));
    toolBar_->addWidget(selector_);

    toolBar_->addWidget(
        new QLabel(" " + tr("PAGE_OF") + " ", toolBar_));

    pagesCount_ = new QLabel(tr("0"), toolBar_);
    toolBar_->addWidget(pagesCount_);

    initNavigationActions();

    toolBar_->addSeparator();

    initEditingActions();

    // menu
    menu_ = new QMenu(tr("PAGE"), parent);
    menu_->addAction(createAct_);
    menu_->addAction(editAct_);
    menu_->addAction(deleteAct_);
}

void PageToolbar::initNavigationActions()
{
    prevAct_ = toolBar_->addAction(tr("PAGE_PREVIOUS"));
    prevAct_->setIcon(QIcon(":/icons/images/go-previous.png"));
    prevAct_->setStatusTip(tr("PAGE_PREVIOUS_TIP"));
    connect(prevAct_, SIGNAL(triggered()), this, SLOT(onNavigation()));

    nextAct_ = toolBar_->addAction(tr("PAGE_NEXT"));
    nextAct_->setIcon(QIcon(":/icons/images/go-next.png"));
    nextAct_->setStatusTip(tr("PAGE_NEXT_TIP"));
    connect(nextAct_, SIGNAL(triggered()), this, SLOT(onNavigation()));
}

void PageToolbar::initEditingActions()
{
    createAct_ = toolBar_->addAction(tr("PAGE_NEW"));
    createAct_->setIcon(QIcon(":/icons/images/document-new.png"));
    createAct_->setStatusTip(tr("PAGE_NEW_TIP"));
    connect(createAct_, SIGNAL(triggered()), this, SLOT(onCreate()));

    editAct_ = toolBar_->addAction(tr("PAGE_EDIT"));
    editAct_->setIcon(QIcon(":/icons/images/document-properties.png"));
    editAct_->setStatusTip(tr("PAGE_EDIT_TIP"));
    connect(editAct_, SIGNAL(triggered()), this, SLOT(onEdit()));

    deleteAct_ = toolBar_->addAction(tr("PAGE_DELETE"));
    deleteAct_->setIcon(QIcon(":/icons/images/edit-delete.png"));
    deleteAct_->setStatusTip(tr("PAGE_DELETE_TIP"));
    connect(deleteAct_, SIGNAL(triggered()), this, SLOT(onDelete()));
}

PageToolbar::~PageToolbar() = default;

QToolBar *PageToolbar::toolBar() { return toolBar_; }

QMenu *PageToolbar::menu() { return menu_; }

void PageToolbar::onCurrentDocumentChanged(view::View* v)
{
    currentView_ = v;

    selector_->setEnabled(currentView_);
    if (currentView_) {
        const auto pagesCount = currentView_->layout().pagesCount();
        validator_->setTop(pagesCount);

        QString s;
        s.setNum(pagesCount);
        selector_->setText(s);
        selector_->setFixedWidth(selector_->width());
        selector_->clear();

        updateSelectorValue();
    }
    updatePagesCount();

    updateEditingActions();
    updateNavigationActions();

    menu_->menuAction()->setEnabled(currentView_);
}

void PageToolbar::updateSelectorValue()
{
    selector_->clear();
    QString s;
    if (currentView_ && currentView_->currentPage())
    {
        s.setNum(currentView_->layout().pageIndex(
            currentView_->currentPage()) + 1);
    }
    selector_->setText(s);
}

void PageToolbar::updatePagesCount()
{
    if (!currentView_ || currentView_->layout().pagesCount() == 0) {
        pagesCount_->setText("0");
    } else {
        QString s;
        s.setNum(currentView_->layout().pagesCount());
        pagesCount_->setText(s);
    }
}

void PageToolbar::updateEditingActions()
{
    createAct_->setEnabled(currentView_);

    bool hasCurrentPage = currentView_ && currentView_->currentPage();

    editAct_->setEnabled(hasCurrentPage);
    deleteAct_->setEnabled(hasCurrentPage);
}

void PageToolbar::updateNavigationActions()
{
    if (!currentView_ || currentView_->layout().pagesCount() == 0) {
        prevAct_->setEnabled(false);
        nextAct_->setEnabled(false);
    } else if (!currentView_->currentPage()) {
        prevAct_->setEnabled(true);
        nextAct_->setEnabled(true);
    } else {
        const auto& layout = currentView_->layout();
        const auto currentPageIdx =
            layout.pageIndex(currentView_->currentPage());

        prevAct_->setEnabled(currentPageIdx > 0);
        nextAct_->setEnabled(currentPageIdx + 1 < layout.pagesCount());
    }
}

void PageToolbar::onCurrentPageChanged()
{
    updateSelectorValue();
    updateEditingActions();
    updateNavigationActions();
}

void PageToolbar::onCreate()
{
/*    PageSettingsDialog *pd = new PageSettingsDialog(this,
        PageSettingsDialog::Create);
    pd->setPages(currentLayout_->numberOfPages() + 1);
    bool result = (pd->exec() == QDialog::Accepted);
    if (result) {
        std::unique_ptr<Page> p(new Page);
        p->setWidth(pd->width());
        p->setHeight(pd->height());
        p->setOrientation(QPrinter::Portrait);
        if (currentLayout_->addPage(pd->pageNum(), std::move(p))) {
            updatePageActions();
            emit pageConfigurationChanged();
        }
    }
*/}

/*
 * page creation/deletion may affect available field actions
 */


void PageToolbar::onEdit()
{
/*    PageSettingsDialog *pd = new PageSettingsDialog(this,
        PageSettingsDialog::Edit);
    pd->setPages(currentLayout_->numberOfPages());
    Page *p = currentLayout_->layout()->page(currentLayout_->currentPage());
    pd->setPageNum(p->num());
    pd->setWidth(p->width());
    pd->setHeight(p->height());
    pd->setDx(p->dx());
    pd->setDy(p->dy());
    bool result = (pd->exec() == QDialog::Accepted);
    if (result) {
        if (currentLayout_->editPage(p->num(), pd->pageNum(), pd->width(),
            pd->height(), pd->dx(), pd->dy()))
        {
            updatePageActions();
            emit pageConfigurationChanged();
        }
    }
*/}

void PageToolbar::onDelete()
{
/*    QMessageBox::StandardButton result = QMessageBox::question(
        this,
        tr("DLG_PAGE_DELETE_PAGE_CONFIRM"),
        tr("DLG_PAGE_DELETE_PAGE_MESSAGE"),
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No);

    if (result == QMessageBox::Yes) {
        if (currentLayout_->deletePage(currentLayout_->currentPage())) {
            updatePageActions();
            emit pageConfigurationChanged();
        }
    }
*/}

void PageToolbar::onSelectorValueChanged()
{
    int val = selector_->text().toInt();
    ASSERT(val >= 1);
    currentView_->setCurrentPage(
        currentView_->layout().page((size_t)(val - 1)));

    updateNavigationActions();
}

void PageToolbar::onNavigation()
{
    ASSERT(currentView_);

    auto& layout = currentView_->layout();

    if (layout.pagesCount() == 0) {
        return;
    }

    size_t idx;
    if (currentView_->currentPage()) {
        idx = layout.pageIndex(currentView_->currentPage());
        if (sender() == prevAct_) {
            if (idx > 0) {
                --idx;
            }
        } else if (idx + 1 < layout.pagesCount()) {
            ++idx;
        }
    } else {
        idx = (sender() == prevAct_) ? 0 : layout.pagesCount() - 1;
    }

    currentView_->setCurrentPage(layout.page(idx));

    updateSelectorValue();
    updateNavigationActions();
}
