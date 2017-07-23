#include "view/view.h"

#include "view/fit_width_view_impl.h"

#include <doctpl/field.h>
#include <doctpl/page.h>
#include <doctpl/layout.h>
#include <doctpl/style_options.h>

#include <QWheelEvent>
#include <QMouseEvent>
#include <QScrollBar>
#include <QResizeEvent>

namespace view {

View::View(
        Layout& layout,
        //Mode mode,
        QWidget* parent)
    : QGraphicsView(parent)
    //, mode_(mode)
    , layout_(layout)
    , currentPage_(nullptr)
    , currentField_(nullptr)
{
    impl_.reset(new FitWidthViewImpl(
        layout_,
        *this,
        DefaultViewCallbacks {
            [this] (QWheelEvent* e) { QGraphicsView::wheelEvent(e); },
            [this] (QResizeEvent* e) { QGraphicsView::resizeEvent(e); },
            [this] (int dx, int dy) { QGraphicsView::scrollContentsBy(dx, dy); },
            [this] (QMouseEvent* e) { QGraphicsView::mouseDoubleClickEvent(e); }
        },
        LayoutObjectCallbacks{
            [this] (doctpl::Field* f)
                { if (f != currentField_) selectedFieldChanged(f); },
            [this] (doctpl::Page* p)
                { if (p != currentPage_) selectedPageChanged(p); }
        }));

    setBackgroundBrush(QBrush(QColor(Qt::gray)));

    auto defaultStylePtr = doctpl::defaultStyle();
    defaultStylePtr->setModifier(
        doctpl::StyleModifiers::ENABLE_PAGE_FRAME |
        doctpl::StyleModifiers::ENABLE_PAGE_BACKGROUND |
        doctpl::StyleModifiers::ENABLE_FIELD_FRAME |
        doctpl::StyleModifiers::ENABLE_FIELD_BACKGROUND);

    QPen pen(Qt::black);
    pen.setWidth(0);
    defaultStylePtr->setPen(pen, doctpl::LineStyleRole::PageFrame);
    defaultStylePtr->setPen(pen, doctpl::LineStyleRole::FieldFrame);

    defaultStylePtr->setBrush(QBrush(Qt::white), doctpl::BackgroundStyleRole::Page);

    layout_.addView(this);
    if (layout_.pagesCount() > 0) {
        currentPage_ = layout_.page(0);
    }
    layout_.setPageSeparator(4.0);

    impl_->adjust();
}

View::~View() = default;

//void View::setMode(View::Mode mode) {
//    mode_ = mode;
//    // TODO mode_->adjust(templateLayout_, this);
//}

const Layout& View::layout() const { return layout_; }
Layout& View::layout() { return layout_; }

void View::setCurrentField(doctpl::Field* f)
{
    if (currentField_) {
        currentField_->setSelected(false);
    }
    currentField_ = f;
    if (currentField_) {
        currentField_->setSelected(true);
        adjustCurrentField();
    }
}

void View::setCurrentPage(doctpl::Page* p)
{
    if (currentPage_) {
        currentPage_->setSelected(false);
    }
    currentPage_ = p;
    if (currentPage_) {
        currentPage_->setSelected(true);
        adjustCurrentPage();
    }
}

void View::adjustCurrentPage()
{
    impl_->adjustCurrentPage();
}

doctpl::Page* View::currentPage() const { return currentPage_; }

void View::adjustCurrentField()
{
    impl_->adjustCurrentField();
}

doctpl::Field* View::currentField() const { return currentField_; }

void View::mouseDoubleClickEvent(QMouseEvent* event)
{
    impl_->processDoubleClickEvent(event);
}

void View::wheelEvent(QWheelEvent* event)
{
    impl_->processWheelEvent(event);
}

void View::resizeEvent(QResizeEvent* event)
{
    impl_->processResizeEvent(event);
}

void View::scrollContentsBy(int dx, int dy)
{
    impl_->processScrollByEvent(dx, dy);
}

void View::selectedPageChanged(doctpl::Page* p)
{
    emit pageSelected(p);
}

void View::selectedFieldChanged(doctpl::Field* f)
{
    emit fieldSelected(f);
}

doctpl::Page* View::pageAt(const QPoint& pos)
{
    doctpl::Page* pAtPos = nullptr;
    for (auto iptr : items(pos)) {
        if (pAtPos = dynamic_cast<doctpl::Page*>(iptr)) {
            break;
        }
    }
    return pAtPos;
}

doctpl::Field* View::fieldAt(const QPoint& pos)
{
    doctpl::Field* fAtPos = nullptr;
    for (auto iptr : items(pos)) {
        if (fAtPos = dynamic_cast<doctpl::Field*>(iptr)) {
            break;
        }
    }
    return fAtPos;
}

} // namespace view
