#include "fit_width_view_impl.h"

#include "view/view.h"

#include <doctpl/layout.h>
#include <doctpl/page.h>
#include <doctpl/field.h>

#include <QGraphicsScene>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QScrollBar>

namespace view {

FitWidthViewImpl::FitWidthViewImpl(
        Layout* layout,
        View* view,
        DefaultViewCallbacks viewCallbacks,
        LayoutObjectCallbacks objectCallbacks)
    : ViewImpl(layout, view, std::move(viewCallbacks), std::move(objectCallbacks))
{}

void FitWidthViewImpl::adjustCurrentField()
{
    auto currentField = view_->currentField();
    if (currentField) {
        if (currentField->page() != view_->currentPage()) {
            changeCurrentPage(currentField->page());
        }
        view_->ensureVisible(currentField, 0, 0);
    }
}

/**
  Вызывается при установлении новой текущей страницы извне
  viewport устанавливается на верхнюю границу страницы
  */
void FitWidthViewImpl::adjustCurrentPage()
{
    if (!view_->currentPage()) {
        return;
    }
    QRectF sr = view_->scene()->sceneRect();
    double halfPS = layout_->pageSeparator() / 2.0;
    double ratio = view_->viewport()->size().width() /
        (double) view_->viewport()->size().height();
    view_->fitInView(
        sr.x(),
        view_->currentPage()->y() - halfPS,
        sr.width(),
        sr.width() / ratio,
        Qt::KeepAspectRatio);
    view_->ensureVisible(
        0.0,
        view_->currentPage()->y() - halfPS,
        sr.x(),
        halfPS * 2.0,
        0,
        0);
}

void FitWidthViewImpl::processResizeEvent(QResizeEvent* event)
{
    if (!view_->currentPage()) {
        return;
    }

    isResizing_ = true;

    double halfPS = layout_->pageSeparator() / 2.0;
    QRectF sr = view_->scene()->sceneRect();
    double ratio = event->size().width() / (double) event->size().height();
    view_->fitInView(
        sr.x(),
        topLeft_.y(),
        sr.width(),
        sr.width() / ratio,
        Qt::KeepAspectRatio);
    view_->ensureVisible(
        sr.x(),
        topLeft_.y(),
        sr.width(),
        halfPS * 2.0,
        0,
        0);

    isResizing_ = false;
}

void FitWidthViewImpl::processScrollByEvent(int dx, int dy)
{
    if (!isResizing_)
        topLeft_ = view_->mapToScene(0, 0);

    ViewImpl::processScrollByEvent(dx, dy);

    auto p = view_->currentPage();
    if (!p) {
        return;
    }

    auto pAtCenter = view_->pageAt(QPoint(
        view_->viewport()->width() / 2,
        view_->viewport()->height() / 2));
    if (pAtCenter && pAtCenter != p) {
        changeCurrentPage(pAtCenter);
    }
}

void FitWidthViewImpl::processDoubleClickEvent(QMouseEvent* event)
{
    auto field = view_->fieldAt(event->pos());
    if (field && field != view_->currentField()) {
        changeCurrentField(field);
        adjustCurrentField();
    } else if (!field) {
        changeCurrentField(nullptr);
    }
}

void FitWidthViewImpl::adjust()
{
    view_->setDragMode(QGraphicsView::NoDrag);
    view_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    view_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    view_->horizontalScrollBar()->hide();

    view_->currentField()
        ? adjustCurrentField()
        : adjustCurrentPage();
}

} // namespace view
