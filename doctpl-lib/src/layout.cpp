#include <doctpl/layout.h>

#include <doctpl/template.h>
#include <doctpl/exception.h>

#include <QGraphicsScene>
#include <QPrinter>
#include <QPainter>

#include <vector>
#include <algorithm>

namespace doctpl {

class Layout::Impl {
public:
    typedef Layout::Index I;

    Impl(Template* doc, Layout* layout)
        : document_(doc)
        , layout_(layout)
        , scene_(new QGraphicsScene)
        , pageSeparator_(0.0)
    {
        ASSERT(document_);
        ASSERT(layout_);
    }

    double pageSeparator() const { return pageSeparator_; }

    Template* document() const { return document_; }

    template <class... Args>
    Page* insert(I at, Args... args)
    {
        std::unique_ptr<Page> page(new Page(args..., layout_));

        double y = at == 0
            ? 0.0
            : pages_.at(at - 1)->y() + pages_.at(at - 1)->height() + pageSeparator_ ;

        Page* p = page.get();
        pages_.insert(pages_.begin() + at, p);

        updateSceneRect();

        if (at != pagesCount() - 1) {
            movePages(at + 1, pagesCount() - 1, p->height() + pageSeparator_);
        }

        scene_->addItem(page.release());
        p->setPos(-p->width() / 2.0, y);
        return p;
    }

    void move(I from, I to)
    {
        ASSERT(from != to);

        auto totalDelta = [&] (I s, I e)
        {
            double r = 0.0;
            for (I i = s; i <= e; ++i) {
                r += pages_.at(i)->height() + pageSeparator_;
            }
            return r;
        };

        if (from < to) {
            movePages(from + 1, to, -(pages_.at(from)->height() + pageSeparator_));
            movePages(from, from, totalDelta(from + 1, to));
        } else {
            movePages(to, from - 1, pages_.at(from)->height() + pageSeparator_);
            movePages(from, from, -totalDelta(to, from - 1));
        }

        int d = to > from ? 1 : -1;
        auto fromPtr = pages_.at(from);
        for (I i = from; i != to; i += d) {
            pages_.at(i) = pages_.at(i + d);
        }
        pages_.at(to) = fromPtr;
    }

    void erase(I at)
    {
        Page* ptr = pages_.at(at);
        pages_.erase(pages_.begin() + at);
        scene_->removeItem(ptr);
        if (at < pagesCount()) {
            movePages(at, pagesCount() - 1, -(ptr->height() + pageSeparator_));
        }
        updateSceneRect();
        delete ptr; // with all child fields
    }

    void onPageResize(I at, const Page::SizeDiff& diff)
    {
        Page* p = pages_.at(at);
        if (diff.height && at < pagesCount() - 1) {
            movePages(at + 1, pagesCount() - 1, p->height() - diff.height->first);
        }
        if (diff.width) {
            p->setX(- p->width() / 2.0);
        }
        updateSceneRect();
    }

    Page* page(I at) { return pages_.at(at); }
    const Page* page(I at) const { return pages_.at(at); }

    Layout::Index end() const { return pages_.size(); }
    size_t pagesCount() const { return pages_.size(); }

    Layout::Index pageIndex(const Page* page) const
    {
        auto it = std::find(pages_.begin(), pages_.end(), page);
        if (it != pages_.end()) {
            return it - pages_.begin();
        }
        return end();
    }

    void checkIndexIsValid(I at) const
    {
        REQUIRE(at < pagesCount(), "Page index " << at << " is out of range");
    }

    void setPageSeparator(double sep)
    {
        double old = pageSeparator_;
        pageSeparator_ = sep;
        for (size_t i = 1; i < pagesCount(); ++i) {
            movePages(i, i, i * (pageSeparator_ - old));
        }
        updateSceneRect();
    }

    void print(const QString& filename, const std::set<I>& pages) const
    {
        QPrinter printer(QPrinter::HighResolution);
        printer.setOutputFileName(filename);
        QPainter painter;
        QPen pen;
        setPageParams(printer, *(pages.begin()));
        painter.begin(&printer);
        painter.setRenderHint(QPainter::SmoothPixmapTransform);
        pen.setWidthF(0.1);
        painter.setPen(pen);
        auto i = pages.begin();
        while (i != pages.end()) {
            printPage(printer, painter, *i);
            ++i;
            if (i != pages.end()) {
                setPageParams(printer, *i);
                printer.newPage();
            }
        }
        painter.end();
    }

private:
    void movePages(I start, I end, double delta)
    {
        checkIndexIsValid(start);
        checkIndexIsValid(end);
        ASSERT(start <= end);
        for (I i = start; i <= end; ++i) {
            pages_.at(i)->setY(pages_.at(i)->y() + delta);
        }
    }

    void updateSceneRect() {
        double newWidth = 0.0, newHeight = 0.0;
        for (size_t i = 0; i < pagesCount(); ++i) {
            newWidth = std::max(pages_[i]->width(), newWidth);
            newHeight += pages_[i]->height() + (i == 0 ? 0.0 : pageSeparator_);
        }
        scene_.get()->setSceneRect(QRectF(
            QPointF(-(newWidth + pageSeparator_) / 2.0, -pageSeparator_ / 2.0),
            QSizeF(newWidth + pageSeparator_, newHeight)));
    }

    void setPageParams(QPrinter& printer, I at) const
    {
        printer.setPaperSize(
            QSizeF(pages_[at]->width(), pages_[at]->height()),
            QPrinter::Millimeter);
        printer.setOrientation(QPrinter::Orientation::Portrait/*pages_.at(at)->orientation()*/);
        printer.setPageMargins(0.0, 0.0, 0.0, 0.0, QPrinter::Millimeter);
    }

    void printPage(
        QPrinter& printer, QPainter& painter, I at) const
    {
        const Page* p = pages_.at(at);
        painter.setViewport(0, 0, printer.width(), printer.height());
        painter.setWindow(0, 0, printer.widthMM(), printer.heightMM());
        scene_->render(
            &painter,
            printer.paperRect(QPrinter::Millimeter),
            QRectF(p->x(), p->y(), p->width(), p->height()),
            Qt::KeepAspectRatio);
    }

    Template* document_;
    Layout* layout_;
    std::unique_ptr<QGraphicsScene> scene_;
    std::vector<Page*> pages_;
    double pageSeparator_;
};


// Layout

Layout::Layout(Template* document)
    : impl_(new Impl(document, this))
{}

Layout::~Layout() {}

Template* Layout::document() const { return impl_->document(); }

Page* Layout::insert(
    Index at,
    const QSizeF& size)
{
    REQUIRE(at <= pagesCount(), "Page index " << at << " is out of range");
    return impl_->insert(at, size);
}

Page* Layout::insert(
    Index at,
    const QSizeF& size, double dx, double dy)
{
    REQUIRE(at <= pagesCount(), "Page index " << at << " is out of range");
    return impl_->insert(at, size, dx, dy);
}

Page* Layout::append(const QSizeF& size)
{
    return impl_->insert(pagesCount(), size);
}

Page* Layout::append(const QSizeF& size, double dx, double dy)
{
    return impl_->insert(pagesCount(), size, dx, dy);
}

void Layout::move(Index from, Index to)
{
    impl_->checkIndexIsValid(from);
    impl_->checkIndexIsValid(to);
    if (from == to) {
        return;
    }
    impl_->move(from, to);
}

void Layout::erase(Index at)
{
    impl_->checkIndexIsValid(at);
    impl_->erase(at);
}
void Layout::onPageResize(const Page* page, const Page::SizeDiff& diff)
{
    ASSERT(page);
    REQUIRE(page->layout() == this, "Page not from this layout");
    Index at = pageIndex(page);
    impl_->checkIndexIsValid(at);
    impl_->onPageResize(at, diff);
}

Page* Layout::page(Index at)
{
    impl_->checkIndexIsValid(at);
    return impl_->page(at);
}

const Page* Layout::page(Index at) const
{
    impl_->checkIndexIsValid(at);
    return impl_->page(at);
}

size_t Layout::pagesCount() const { return impl_->pagesCount(); }

Layout::Index Layout::end() const { return impl_->end(); }

Layout::Index Layout::pageIndex(const Page* page) const
{
    ASSERT(page);
    return impl_->pageIndex(page);
}

double Layout::pageSeparator() const { return impl_->pageSeparator(); }

void Layout::setPageSeparator(double sep)
{
    REQUIRE(!(sep < 0.0), "Negative separators are not allowed");
    impl_->setPageSeparator(sep);
}

void Layout::print(const QString& filename) const
{
    REQUIRE(pagesCount() > 0, "Empty document");
    std::set<Index> pages;
    for (Index i = 0; i < pagesCount(); ++i) {
        pages.insert(i);
    }
    impl_->print(filename, pages);
}

void Layout::print(const QString& filename, const std::set<Index>& pages) const
{
    REQUIRE(!pages.empty(), "Empty page set to print");
    for (auto i : pages) {
        impl_->checkIndexIsValid(i);
    }
    impl_->print(filename, pages);
}

} // namespace doctpl
