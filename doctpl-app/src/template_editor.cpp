#include "template_editor.h"

#include "view/view.h"
#include "page_toolbar.h"

#include <doctpl/serialize.h>
#include <doctpl/template.h>
#include <doctpl/layout.h>

TemplateEditor::TemplateEditor(QWidget* parent)
    : QMainWindow(parent)
{
    documentWidget_ = new QTabWidget(this);
    setCentralWidget(documentWidget_);

    documents_.push_back(doctpl::xml::read("C:/Users/dicentra/Documents/att_2017/9.xml"));
    // FIXME return Layout& from Template
    view::View* v = new view::View(documents_.front(), documentWidget_);
    documentWidget_->addTab(v, "9.xml");
    v->show();

    pageToolbar_ = new PageToolbar(this);
    connect(v, SIGNAL(currentPageChanged()), pageToolbar_, SLOT(onCurrentPageChanged()));
    addToolBar(pageToolbar_->toolBar());

    pageToolbar_->onCurrentDocumentChanged(v);
}

TemplateEditor::~TemplateEditor() = default;


