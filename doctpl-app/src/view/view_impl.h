#pragma once

#include <QPoint>

#include <functional>

namespace doctpl {

class Layout;
class Page;
class Field;

} // namespace doctpl

class QWheelEvent;
class QMouseEvent;
class QResizeEvent;

namespace view {

struct DefaultViewCallbacks {
    std::function<void(QWheelEvent*)> onWheelEvent;
    std::function<void(QResizeEvent*)> onResizeEvent;
    std::function<void(int, int)> onScrollByEvent;
    std::function<void(QMouseEvent*)> onDoubleClickEvent;
};

struct LayoutObjectCallbacks {
    std::function<void(doctpl::Field*)> onFieldSelection;
    std::function<void(doctpl::Page*)> onPageSelection;
};

class View;

using Layout = doctpl::Layout;

class ViewImpl {
public:
    ViewImpl(const ViewImpl&) = delete;
    ViewImpl& operator = (const ViewImpl&) = delete;

    ViewImpl(ViewImpl&&) = default;
    ViewImpl& operator = (ViewImpl&&) = default;

    virtual ~ViewImpl() = default;

    virtual void adjustCurrentField();
    virtual void adjustCurrentPage();

    QPointF topLeft() const { return topLeft_; }
    QPointF bottomRight() const { return bottomRight_; }

    virtual void processWheelEvent(QWheelEvent* event);
    virtual void processResizeEvent(QResizeEvent* event);
    virtual void processScrollByEvent(int dx, int dy);
    virtual void processDoubleClickEvent(QMouseEvent* event);

    virtual void adjust() = 0;

protected:
    ViewImpl(
        Layout& layout,
        View& view,
        DefaultViewCallbacks viewCallbacks,
        LayoutObjectCallbacks objectCallbacks);

    void changeCurrentPage(doctpl::Page* page);
    void changeCurrentField(doctpl::Field* field);

    Layout& layout_;
    View& view_;

    DefaultViewCallbacks viewCallbacks_;
    LayoutObjectCallbacks objectCallbacks_;

    QPointF topLeft_;
    QPointF bottomRight_;
};

} // namespace view
