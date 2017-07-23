#include "view/view_impl.h"

#include "view/view.h"

#include <doctpl/page.h>
#include <doctpl/field.h>
#include <doctpl/exception.h>

#include <QWheelEvent>
#include <QResizeEvent>
#include <QMouseEvent>

namespace view {

ViewImpl::ViewImpl(
        Layout& layout,
        View& view,
        DefaultViewCallbacks viewCallbacks,
        LayoutObjectCallbacks objectCallbacks)
    : layout_(layout)
    , view_(view)
    , viewCallbacks_(std::move(viewCallbacks))
    , objectCallbacks_(std::move(objectCallbacks))
{}

void ViewImpl::adjustCurrentField() {}

void ViewImpl::adjustCurrentPage() {}

void ViewImpl::processWheelEvent(QWheelEvent* event)
{
    viewCallbacks_.onWheelEvent(event);
}

void ViewImpl::processResizeEvent(QResizeEvent* event)
{
    viewCallbacks_.onResizeEvent(event);
}

void ViewImpl::processScrollByEvent(int dx, int dy)
{
    viewCallbacks_.onScrollByEvent(dx, dy);
}

void ViewImpl::processDoubleClickEvent(QMouseEvent* event)
{
    viewCallbacks_.onDoubleClickEvent(event);
}

void ViewImpl::changeCurrentPage(doctpl::Page* page)
{
    objectCallbacks_.onPageSelection(page);
}

void ViewImpl::changeCurrentField(doctpl::Field* field)
{
    objectCallbacks_.onFieldSelection(field);
}

} // namespace view
