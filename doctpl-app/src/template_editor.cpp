#include "template_editor.h"

#include "view/view.h"

#include <doctpl/serialize.h>
#include <doctpl/template.h>
#include <doctpl/layout.h>

TemplateEditor::TemplateEditor(QWidget* parent)
    : QMainWindow(parent)
{
    documentWidget_ = new QTabWidget(this);
    setCentralWidget(documentWidget_);

    documents_.push_back(doctpl::xml::read("C:/Users/dicentra/Documents/att_2017/9.xml"));
    view::View* v = new view::View(documents_.front()->layout(), documentWidget_);
    documentWidget_->addTab(v, "9.xml");
    v->show();
}

TemplateEditor::~TemplateEditor() = default;


