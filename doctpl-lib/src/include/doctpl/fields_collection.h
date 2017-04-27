#pragma once

#include <doctpl/field.h>
#include <doctpl/exception.h>

#include <list>
#include <functional>

namespace doctpl {

template <class FieldT>
class FieldsCollectionBase : public FieldWatcher<FieldT> {
public:
    void add(FieldT* field)
    {
        ASSERT(field);
        fieldsById_.emplace(field->id(), field);

        for (auto w : watchers_) {
            w->add(field);
        }
    }

    void remove(Field::ID id)
    {
        auto it = fieldsById_.find(id);
        if (it != fieldsById_.end()) {
            fieldsById_.erase(it);
            for (auto w : watchers_) {
                w->remove(id);
            }
        }
    }

    bool contains(Field::ID id) const
    {
        return fieldsById_.find(id) != fieldsById_.end();
    }

    // throw if not found

    FieldT* find(Field::ID id) const
    {
        auto it = fieldsById_.find(id);
        REQUIRE(it != fieldsById_.end(), "Field " << id << " not found");
        return it->second;
    }

    FieldT* find(const QString& name) const
    {
        auto it = std::find_if(
            fieldsById_.begin(), fieldsById_.end(),
            [&name] (const std::pair<Field::ID, FieldT*>& f)
                { return f.second->name() == name; });
        REQUIRE(it != fieldsById_.end(), "Field " << name.toStdString() << " not found");
        return it->second;
    }

    typedef std::list<FieldT*> List;

    List all() const
    {
        List res;
        for (const auto& p : fieldsById_) {
            res.push_back(p->second);
        }
        return res;
    }

    // global field watchers

    typedef typename FieldWatcher<FieldT>::Ptr WatcherPtr;
    typedef std::list<WatcherPtr> WatcherPtrList;

    void addWatcher(const WatcherPtr& watcher)
    {
        ASSERT(watcher);
        if (std::find(
            watchers_.begin(), watchers_.end(), watcher) == watchers_.end())
        {
            watchers_.push_back(watcher);
            for (auto f : fieldsById_) {
                watcher->add(f.second);
            }
        }
    }

    const WatcherPtrList& watchers() const { return watchers_; }

private:
    std::map<Field::ID, FieldT*> fieldsById_;

    WatcherPtrList watchers_;
};


template <class... FieldTs> class FieldsCollectionImpl;

template <> class FieldsCollectionImpl<>
{
    QString type(const Field::ID& id) const
    {
        DOCTPL_ERROR("Field not found: " << id);
        return "";
    }
};

template <class FieldT, class... FieldTs>
class FieldsCollectionImpl<FieldT, FieldTs...>
    : public FieldsCollectionImpl<FieldTs...>
{
public:
    FieldsCollectionImpl()
        : f_(std::make_shared<FieldsCollectionBase<FieldT> >())
    {}

    typedef std::shared_ptr<FieldsCollectionBase<FieldT> > BasePtr;

    template <class OtherFieldT>
    const BasePtr&
    as(const typename
        std::enable_if<
            std::is_same<FieldT, OtherFieldT>::value,
            FieldT
        >::type* = nullptr)
    {
        return f_;
    }

    template <class OtherFieldT>
    const std::shared_ptr<FieldsCollectionBase<OtherFieldT> >&
    as(const typename
        std::enable_if<
            !std::is_same<FieldT, OtherFieldT>::value,
            OtherFieldT
        >::type* = nullptr)
    {
        return FieldsCollectionImpl<FieldTs...>::template as<OtherFieldT>();
    }

    QString type(const Field::ID& id) const
    {
        if (f_->contains(id)) {
            return f_->find(id)->fieldType();
        }
        return FieldsCollectionImpl<FieldTs...>::type(id);
    }

protected:
    BasePtr f_;
};

template <class... FieldTs>
class FieldsCollection {
public:
    template <class FieldT>
    const std::shared_ptr<FieldsCollectionBase<FieldT> >& as()
    {
        return impl_.as<FieldT>();
    }

    QString type(const Field::ID& id) const { return impl_.type(id); }

private:
    FieldsCollectionImpl<FieldTs...> impl_;
};

} // namespace doctpl
