#include "pagetools.h"

#include "pagesettingsdialog.h"
#include "documenttemplate.h"
#include "template/templatelayout.h"
#include "template/templateview.h"
#include "page.h"

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

PageTools::PageTools(QWidget *parent)
    : QObject(parent)
    , parent_(parent)
{
    pageToolBar_ = new QToolBar(tr("PAGE"), parent_);
    pageToolBar_->addWidget(new QLabel("  " + tr("PAGE") + ":  ",
        pageToolBar_));

    pageSelector_ = new QLineEdit(pageToolBar_);
    pageValidator_ = new QIntValidator(this);
    pageSelector_->setValidator(pageValidator_);
    connect(pageSelector_, SIGNAL(editingFinished()),
        this, SLOT(pageChanged()));
    pageToolBar_->addWidget(pageSelector_);
    pageToolBar_->addWidget(new QLabel(" " + tr("PAGE_OF") + " ",
        pageToolBar_));
    numberOfPages_ = new QLabel(tr("0"), pageToolBar_);
    pageToolBar_->addWidget(numberOfPages_);
    prevPageAct_ = pageToolBar_->addAction(tr("PAGE_PREVIOUS"));
    prevPageAct_->setIcon(QIcon(":/icons/images/go-previous.png"));
    prevPageAct_->setStatusTip(tr("PAGE_PREVIOUS_TIP"));
    connect(prevPageAct_, SIGNAL(triggered()), this, SLOT(onPageSwitching()));
    nextPageAct_ = pageToolBar_->addAction(tr("PAGE_NEXT"));
    nextPageAct_->setIcon(QIcon(":/icons/images/go-next.png"));
    nextPageAct_->setStatusTip(tr("PAGE_NEXT_TIP"));
    connect(nextPageAct_, SIGNAL(triggered()), this, SLOT(onPageSwitching()));
    pageToolBar_->addSeparator();
    newPageAct_ = pageToolBar_->addAction(tr("PAGE_NEW"));
    newPageAct_->setIcon(QIcon(":/icons/images/document-new.png"));
    newPageAct_->setStatusTip(tr("PAGE_NEW_TIP"));
    connect(newPageAct_, SIGNAL(triggered()), this, SLOT(newPage()));
    editPageAct_ = pageToolBar_->addAction(tr("PAGE_EDIT"));
    editPageAct_->setIcon(QIcon(":/icons/images/document-properties.png"));
    editPageAct_->setStatusTip(tr("PAGE_EDIT_TIP"));
    connect(editPageAct_, SIGNAL(triggered()), this, SLOT(editPage()));
    deletePageAct_ = pageToolBar_->addAction(tr("PAGE_DELETE"));
    deletePageAct_->setIcon(QIcon(":/icons/images/edit-delete.png"));
    deletePageAct_->setStatusTip(tr("PAGE_DELETE_TIP"));
    connect(deletePageAct_, SIGNAL(triggered()), this, SLOT(deletePage()));
    // menu
    pageMenu_ = new QMenu(tr("PAGE"), parent_);
    pageMenu_->addAction(newPageAct_);
    pageMenu_->addAction(editPageAct_);
    pageMenu_->addAction(deletePageAct_);
}

PageTools::~PageTools() {
    if (!parent_) {
        delete pageToolBar_;
        delete pageMenu_;
    }
    // иначе удаляется родительским объектом
}

QToolBar *PageTools::toolBar() {
    return pageToolBar_;
}

QMenu *PageTools::menu() {
    return pageMenu_;
}

/**
  Полное обновление всех инструментов
  */
void PageTools::currentDocumentChanged(DocumentTemplate *currentDoc) {
    currentDocument_ = currentDoc;
    updatePageActions();
}

void PageTools::updatePageActions() {
    if (!currentDocument_) {
        pageSelector_->clear();
        pageSelector_->setEnabled(false);
        newPageAct_->setEnabled(false);
        editPageAct_->setEnabled(false);
        deletePageAct_->setEnabled(false);
        pageMenu_->menuAction()->setEnabled(false);
        numberOfPages_->setText("0");
    } else if (currentDocument_->numberOfPages() > 0 &&
       currentDocument_->numberOfPages() ==
       (size_t)pageSelector_->text().toInt())
    {   // изменить только текущую страницу
        currentPageChanged();
    } else {
        pageSelector_->clear();
        newPageAct_->setEnabled(true);
        pageMenu_->menuAction()->setEnabled(true);
        QString s;
        s.setNum(currentDocument_->numberOfPages());
        numberOfPages_->setText(s);
        // update page selector
        if (currentDocument_->numberOfPages() > 0) {
            if (currentDocument_->currentPage() !=
                currentDocument_->pageOutOfRange())
            {
                s.setNum(currentDocument_->currentPage());
            } else {
                currentDocument_->setCurrentPage(1);
                s.setNum(1);
            }
            pageSelector_->setEnabled(true);
            pageValidator_->setBottom(currentDocument_->firstPage());
            pageValidator_->setTop(currentDocument_->lastPage());
            pageSelector_->setText(s);
            editPageAct_->setEnabled(true);
            deletePageAct_->setEnabled(true);
        } else {
            // нет страниц
            pageSelector_->setEnabled(false);
            editPageAct_->setEnabled(false);
            deletePageAct_->setEnabled(false);
        }
    }
    updatePageSwitchers(); // обновить switching actions
}

/**
  Данные текущего документа не изменились
  Изменилась только текущая страница
  */
void PageTools::currentPageChanged() {
    pageSelector_->clear();
    QString s;
    if (currentDocument_->currentPage() !=
        currentDocument_->pageOutOfRange())
    {
        s.setNum(currentDocument_->currentPage());
    } else {
        currentDocument_->setCurrentPage(1);
        s.setNum(1);
    }
    pageSelector_->setText(s);
    updatePageSwitchers();
}

void PageTools::newPage() {
    PageDialog *pd = new PageDialog(parent_,
        PageDialog::Create);
    pd->setPages(currentDocument_->numberOfPages() + 1);
    bool result = (pd->exec() == QDialog::Accepted);
    if (result) {
        std::unique_ptr<Page> p(new Page);
        p->setWidth(pd->width());
        p->setHeight(pd->height());
        p->setOrientation(QPrinter::Portrait);
        if (currentDocument_->addPage(pd->pageNum(), std::move(p))) {
            updatePageActions();
            emit pageConfigurationChanged();
        }
    }
}

void PageTools::editPage() {
    PageDialog *pd = new PageDialog(parent_,
        PageDialog::Edit);
    pd->setPages(currentDocument_->numberOfPages());
    Page *p = currentDocument_->layout()->page(currentDocument_->currentPage());
    pd->setPageNum(p->num());
    pd->setWidth(p->width());
    pd->setHeight(p->height());
    pd->setDx(p->dx());
    pd->setDy(p->dy());
    bool result = (pd->exec() == QDialog::Accepted);
    if (result) {
        if (currentDocument_->editPage(p->num(), pd->pageNum(), pd->width(),
            pd->height(), pd->dx(), pd->dy()))
        {
            updatePageActions();
            emit pageConfigurationChanged();
        }
    }
}

void PageTools::deletePage() {
    QMessageBox::StandardButton result = QMessageBox::question(parent_,
        tr("DLG_PAGE_DELETE_PAGE_CONFIRM"),
        tr("DLG_PAGE_DELETE_PAGE_MESSAGE"), QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No);
    if (result == QMessageBox::Yes) {
        if (currentDocument_->deletePage(currentDocument_->
            currentPage()))
        {
            updatePageActions();
            emit pageConfigurationChanged();
        }
    }
}

void PageTools::pageChanged() {
    currentDocument_->setCurrentPage((size_t) pageSelector_->text().toInt());
    currentPageChanged();
}

void PageTools::onPageSwitching() {
    pageSelector_->clear();
    size_t newNum = currentDocument_->currentPage();
    if (sender() == prevPageAct_)
        newNum--;
    else
        newNum++;
    currentDocument_->setCurrentPage(newNum);
    QString s;
    s.setNum(newNum);
    pageSelector_->setText(s);
    updatePageSwitchers();
}

void PageTools::updatePageSwitchers() {
    if (!currentDocument_
        || currentDocument_->numberOfPages() == 0
        || currentDocument_->currentPage() ==
            currentDocument_->pageOutOfRange())
    {
        prevPageAct_->setEnabled(false);
        nextPageAct_->setEnabled(false);
    } else {
        if (currentDocument_->currentPage() ==
            currentDocument_->firstPage())
        {
            prevPageAct_->setEnabled(false);
        } else {
            prevPageAct_->setEnabled(true);
        }
        if (currentDocument_->currentPage() ==
            currentDocument_->lastPage())
        {
            nextPageAct_->setEnabled(false);
        } else {
            nextPageAct_->setEnabled(true);
        }
    }
}
