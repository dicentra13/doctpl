#pragma once

#include "view/view_impl.h"

namespace view {

class FitPageViewImpl : public ViewImpl {
public:
    FitPageViewImpl(
        Layout& layout,
        View& view,
        DefaultViewCallbacks viewCallbacks,
        LayoutObjectCallbacks objectCallbacks);

    virtual void adjustCurrentField();
    virtual void adjustCurrentPage();

    virtual void processResizeEvent(QResizeEvent* event);
    virtual void processScrollByEvent(int dx, int dy);
    virtual void processDoubleClickEvent(QMouseEvent* event);

    virtual void adjust();

private:
    bool isInducedScrolling_;
};

} // namespace view

