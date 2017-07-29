#include "view/view.h"

#include "view/fit_width_view_impl.h"
#include "view/fit_page_view_impl.h"

#include <doctpl/field.h>
#include <doctpl/page.h>
#include <doctpl/template.h>
#include <doctpl/layout.h>
#include <doctpl/style_options.h>

#include <QWheelEvent>
#include <QMouseEvent>
#include <QScrollBar>
#include <QResizeEvent>

namespace view {

View::View(
        TemplatePtr document,
        //Mode mode,
        QWidget* parent)
    : QGraphicsView(parent)
    //, mode_(mode)
    , document_(document)
    , layout_(document_->layout())
    , currentPage_(nullptr)
    , currentField_(nullptr)
{
    impl_.reset(new FitPageViewImpl(
        *layout_,
        *this,
        DefaultViewCallbacks {
            [this] (QWheelEvent* e) { QGraphicsView::wheelEvent(e); },
            [this] (QResizeEvent* e) { QGraphicsView::resizeEvent(e); },
            [this] (int dx, int dy) { QGraphicsView::scrollContentsBy(dx, dy); },
            [this] (QMouseEvent* e) { QGraphicsView::mouseDoubleClickEvent(e); }
        },
        LayoutObjectCallbacks{
            [this] (doctpl::Field* f)
                { if (f != currentField_) onFieldSelected(f); },
            [this] (doctpl::Page* p)
                { if (p != currentPage_) onPageSelected(p); }
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

    layout_->addView(this);
    if (layout_->pagesCount() > 0) {
        currentPage_ = layout_->page(0);
    }
    layout_->setPageSeparator(4.0);

    impl_->adjust();
}

View::~View()
{
    layout_ = nullptr;
}

//void View::setMode(View::Mode mode) {
//    mode_ = mode;
//    // TODO mode_->adjust(templateLayout_, this);
//}

const Layout& View::layout() const { ASSERT(layout_); return *layout_; }
Layout& View::layout() { ASSERT(layout_); return *layout_; }

#define DOCTPL_APP_VIEW_LAYOUT_GUARD \
    while (!layout_) { return; }

#define DOCTPL_APP_VIEW_LAYOUT_GUARD_NULLPTR \
    while (!layout_) { return nullptr; }


void View::setCurrentField(doctpl::Field* f)
{
    DOCTPL_APP_VIEW_LAYOUT_GUARD

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
    DOCTPL_APP_VIEW_LAYOUT_GUARD

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
    DOCTPL_APP_VIEW_LAYOUT_GUARD
    impl_->adjustCurrentPage();
}

doctpl::Page* View::currentPage() const
{
    DOCTPL_APP_VIEW_LAYOUT_GUARD_NULLPTR
    return currentPage_;
}

void View::adjustCurrentField()
{
    DOCTPL_APP_VIEW_LAYOUT_GUARD
    impl_->adjustCurrentField();
}

doctpl::Field* View::currentField() const
{
    DOCTPL_APP_VIEW_LAYOUT_GUARD_NULLPTR
    return currentField_;
}

void View::mouseDoubleClickEvent(QMouseEvent* event)
{
    DOCTPL_APP_VIEW_LAYOUT_GUARD
    impl_->processDoubleClickEvent(event);
}

void View::wheelEvent(QWheelEvent* event)
{
    DOCTPL_APP_VIEW_LAYOUT_GUARD
    impl_->processWheelEvent(event);
}

void View::resizeEvent(QResizeEvent* event)
{
    DOCTPL_APP_VIEW_LAYOUT_GUARD
    impl_->processResizeEvent(event);
}

void View::scrollContentsBy(int dx, int dy)
{
    DOCTPL_APP_VIEW_LAYOUT_GUARD
    impl_->processScrollByEvent(dx, dy);
}

void View::onPageSelected(doctpl::Page* p)
{
    DOCTPL_APP_VIEW_LAYOUT_GUARD
    if (p != currentPage_) {
        currentPage_ = p;
        emit currentPageChanged();
    }
}

void View::onFieldSelected(doctpl::Field* f)
{
    DOCTPL_APP_VIEW_LAYOUT_GUARD
    if (f != currentField_) {
        currentField_ = f;
        emit currentFieldChanged();
    }
}

doctpl::Page* View::pageAt(const QPoint& pos)
{
    DOCTPL_APP_VIEW_LAYOUT_GUARD_NULLPTR
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
    DOCTPL_APP_VIEW_LAYOUT_GUARD_NULLPTR
    doctpl::Field* fAtPos = nullptr;
    for (auto iptr : items(pos)) {
        if (fAtPos = dynamic_cast<doctpl::Field*>(iptr)) {
            break;
        }
    }
    return fAtPos;
}

#undef DOCTPL_APP_VIEW_LAYOUT_GUARD
#undef DOCTPL_APP_VIEW_LAYOUT_GUARD_NULLPTR

} // namespace view
