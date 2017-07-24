#include "fit_page_view_impl.h"

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

FitPageViewImpl::FitPageViewImpl(
        Layout& layout,
        View& view,
        DefaultViewCallbacks viewCallbacks,
        LayoutObjectCallbacks objectCallbacks)
    : ViewImpl(
        layout,
        view,
        std::move(viewCallbacks),
        std::move(objectCallbacks))
    , isInducedScrolling_(false)
{}

void FitPageViewImpl::adjustCurrentField()
{
    auto currentField = view_.currentField();
    if (currentField) {
        if (currentField->page() != view_.currentPage()) {
            onPageSelected(currentField->page());
            adjustCurrentPage();
        }
    }
}

void FitPageViewImpl::adjustCurrentPage()
{
    auto p = view_.currentPage();

    if (!p) {
        return;
    }

    const double ps = layout_.pageSeparator();
    const double halfPS = ps / 2.0;

    isInducedScrolling_ = true;

    view_.fitInView(
        p->x() - halfPS,
        p->y() - halfPS,
        p->width() + ps,
        p->height() + ps,
        Qt::KeepAspectRatio);
    view_.ensureVisible(
        p->x(), p->y(), p->width(), p->height(),
        0, 0);

    isInducedScrolling_ = false;
}

void FitPageViewImpl::processResizeEvent(QResizeEvent* /*event*/)
{
    adjustCurrentPage();
}

void FitPageViewImpl::processScrollByEvent(int dx, int dy)
{
    if (!view_.currentPage()
        || view_.layout().pagesCount() == 0
        || isInducedScrolling_)
    {
        ViewImpl::processScrollByEvent(dx, dy);
        return;
    }

//    auto vScrollbarPtr = view_.verticalScrollBar();
//    const int pageStep = (vScrollbarPtr->maximum() - vScrollbarPtr->minimum()) /
//        view_.layout().pagesCount();

//    if (dy > 0 && dy > pageStep / 3) {

//    } else if ()

}

void FitPageViewImpl::processDoubleClickEvent(QMouseEvent* event)
{
    auto field = view_.fieldAt(event->pos());
    if (field && field != view_.currentField()) {
        onFieldSelected(field);
        adjustCurrentField();
    } else if (!field) {
        onFieldSelected(nullptr);
    }
}

void FitPageViewImpl::adjust()
{
    view_.setDragMode(QGraphicsView::NoDrag);
    view_.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view_.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    view_.horizontalScrollBar()->hide();

    view_.currentField()
        ? adjustCurrentField()
        : adjustCurrentPage();
}

} // namespace view
