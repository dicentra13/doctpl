#pragma once

#include <doctpl/page.h>
#include <doctpl/field.h>

#include <QObject>
#include <QString>
#include <QSizeF>

#include <set>
#include <memory>

namespace doctpl {

class Template;

class Layout : public QObject {

    Q_OBJECT

public:
    explicit Layout(Template* document);

    virtual ~Layout();

    Template* document() const;

    typedef size_t Index;

    Page* insert(
        Index at,
        const QSizeF& size);

    Page* insert(
        Index at,
        const QSizeF& size, double dx, double dy);

    Page* append(
        const QSizeF& size);

    Page* append(
        const QSizeF& size, double dx, double dy);

    void move(Index from, Index to);
    void erase(Index at);

    // callback for Page on resize
    void onPageResize(const Page* page, const Page::SizeDiff& diff);

    Page* page(Index at);
    const Page* page(Index at) const;

    size_t pagesCount() const;
    Index end() const;

    // linear on pages count
    Index pageIndex(const Page* page) const;

    // page separator in scene
    double pageSeparator() const;
    void setPageSeparator(double sep);

    void print(const QString& filename) const;
    void print(const QString& filename, const std::set<Index>& pages) const;

private:
    class Impl;

    std::unique_ptr<Impl> impl_;
};

} // namespace doctpl
