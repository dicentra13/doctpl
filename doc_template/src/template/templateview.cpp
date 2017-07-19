#include "templateview.h"

#include "templatelayout.h"
#include "templateviewmode.h"
#include "../page.h"
#include "../fields/field.h"

#include "../fields/textitem.h"

#include <QWheelEvent>
#include <QMouseEvent>
#include <QScrollBar>
#include <QResizeEvent>

TemplateView::TemplateView(TemplateLayout *layout, TemplateViewMode *mode,
        QWidget *parent)
    : QGraphicsView(parent)
    , mode_(mode)
    , templateLayout_(layout)
    , isResizing_(false)
{
    setBackgroundBrush(QBrush(QColor(Qt::gray)));
    mode_->adjust(templateLayout_, this);
}

TemplateView::~TemplateView()
{}

void TemplateView::setMode(TemplateViewMode *mode) {
    mode_ = mode;
    mode_->adjust(templateLayout_, this);
}

void TemplateView::adjustCurrentPage() {
    mode_->adjustCurrentPage(templateLayout_, this);
}

Page *TemplateView::currentPage() const {
    return templateLayout_->page(templateLayout_->currentPage());
}

void TemplateView::adjustCurrentField() {
    mode_->adjustCurrentField(templateLayout_, this);
}

Field *TemplateView::currentField() const {
    return templateLayout_->field(templateLayout_->currentField());
}

void TemplateView::mouseDoubleClickEvent(QMouseEvent *event) {
    mode_->processDoubleClickEvent(templateLayout_, this, event);
}

void TemplateView::wheelEvent(QWheelEvent *event) {
    mode_->processWheelEvent(templateLayout_, this, event);
}

void TemplateView::resizeEvent(QResizeEvent *event) {

    isResizing_ = true;
    if (currentPage()) {
        mode_->processResizeEvent(templateLayout_, this, event);
    }
    isResizing_ = false;
}

void TemplateView::scrollContentsBy(int dx, int dy) {
    if (!isResizing_)
        topLeft_ = mapToScene(0, 0);
    mode_->processScrollByEvent(templateLayout_, this, dx, dy);
}

void TemplateView::selectedPageChanged(Page *p) {
    emit pageSelected(p);
}

void TemplateView::selectedFieldChanged(Field *f) {
    emit fieldSelected(f);
}
