#pragma once

#include <doctpl/layout_item.h>
#include <doctpl/exception.h>

#include <QString>
#include <QSizeF>
#include <QPointF>

#include <memory>
#include <list>
#include <algorithm>

namespace doctpl {

class Page;

class Field : public LayoutItem {
public:
    typedef size_t ID;

    virtual ~Field();

    // from LayoutItem
    virtual void setWidth(double width);
    virtual void setHeight(double height);

    const ID& id() const;

    virtual const QString& fieldType() const = 0;
    virtual void clear() = 0;

    // it is guaranteed that page is one of ancestors of all its fields
    // this is how field`s page is accessed
    Page* page() const;

    void setName(const QString& name);
    const QString& name() const;

protected:
    Field(
        const QString& name,
        const QSizeF& size,
        const QPointF& pos,
        StylePtr style,
        Page* page);

    class Impl;

    std::unique_ptr<Impl> impl_;
};


template <class FieldT>
class FieldWatcher {
public:
    virtual ~FieldWatcher() {}

    virtual void add(FieldT* field) = 0;

    virtual void remove(Field::ID id) = 0;

    typedef std::shared_ptr<FieldWatcher<FieldT> > Ptr;
};


template <class DerivedField>
class WatchedField : public Field {
public:
    virtual ~WatchedField()
    {
        for (const auto& w : watchers_) {
            w->remove(id());
        }
    }

    typedef typename FieldWatcher<DerivedField>::Ptr WatcherPtr;

    void addWatcher(const WatcherPtr& watcher)
    {
        ASSERT(watcher);
        if (find(watcher) == watchers_.end()) {
            watchers_.push_back(watcher);
            watcher->add((DerivedField*)this);
        }
    }

    void removeWatcher(const WatcherPtr& watcher)
    {
        ASSERT(watcher);
        auto it = find(watcher);
        if (it != watchers_.end()) {
            (*it)->remove(id());
            watchers_.erase(it);
        }
    }

protected:
    WatchedField(
            const QString &name,
            const QSizeF& size,
            const QPointF& pos,
            StylePtr style,
            Page* page,
            const std::list<WatcherPtr>& watchers)
        : Field(name, size, pos, std::move(style), page)
    {
        for (const auto& w : watchers) {
            addWatcher(w);
        }
    }

    typename std::list<WatcherPtr>::iterator
    find(const WatcherPtr& watcher)
    {
        return std::find(watchers_.begin(), watchers_.end(), watcher);
    }

    std::list<WatcherPtr> watchers_;
};

} // namespace doctpl
