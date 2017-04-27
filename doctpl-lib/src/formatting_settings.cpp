#include <doctpl/formatting_settings.h>

#include "text_item.h"

#include <doctpl/exception.h>

#include <list>
#include <map>

namespace doctpl {

// Watcher

class TextItemFormatter::Impl {
public:
    void add(TextItem *i)
    {
        ASSERT(i);
        items_.emplace(i->id(), i);
    }

    void remove(TextItem *i)
    {
        ASSERT(i);
        items_.erase(i->id());
    }

    void forAll(const std::function<void(TextItem*)>& f)
    {
        for (auto& p : items_) {
            f(p.second);
        }
    }

private:
    std::map<TextItem::ID, TextItem*> items_;
};

TextItemFormatter::TextItemFormatter()
    : impl_(new Impl)
{}

TextItemFormatter::TextItemFormatter(TextItemFormatter&&) = default;
TextItemFormatter& TextItemFormatter::operator = (TextItemFormatter&&) = default;

TextItemFormatter::~TextItemFormatter() {}

void TextItemFormatter::add(TextItem* item) { impl_->add(item); }

void TextItemFormatter::remove(TextItem* item) { impl_->remove(item); }

void TextItemFormatter::forAll(std::function<void(TextItem*)> f) { impl_->forAll(f); }


class Formatting::Impl {
public:
    Impl(const QFont& font, int flags, const Margins& margins)
        : font(font)
        , flags(flags)
        , margins(margins)
    {}

    std::function<void(TextItem*)> itemsUpdater() const
    {
        return [] (TextItem* i) { i->forceUpdate(); };
    }

    QFont font;
    int flags;
    Margins margins;
};


Formatting::Formatting(
        const QFont& font, int flags, const Margins& margins)
    : impl_(new Impl(font, flags, margins))
{}

Formatting::Formatting(Formatting&&) = default;
Formatting& Formatting::operator = (Formatting&& o) = default;

Formatting::~Formatting() {}

FormattingPtr Formatting::defaults()
{
    static FormattingPtr s_defaults =
        std::make_shared<Formatting>();
    return s_defaults;
}

FormattingPtr Formatting::copy() const
{
    return std::make_shared<Formatting>(font(), flags(), margins());
}

void Formatting::setFont(const QFont& font)
{
    impl_->font = font;
    forAll(impl_->itemsUpdater());
}

const QFont& Formatting::font() const { return impl_->font; }

void Formatting::setFlags(int flags)
{
    impl_->flags = flags;
    forAll(impl_->itemsUpdater());
}

int Formatting::flags() const { return impl_->flags; }

void Formatting::setMargins(const Margins& margins)
{
    impl_->margins = margins;
    forAll(impl_->itemsUpdater());
}

const Margins& Formatting::margins() const { return impl_->margins; }


// FormattingSettings

class FormattingSettings::Impl {
public:
    Impl(
            const FormattingPtr& global,
            const FormattingPtr& local,
            FormattingSettings::Mode mode)
        : mode(mode)
        , global(global)
        , local(local)
    {
        ASSERT(global);
        ASSERT(local);
        current = mode == FormattingSettings::Mode::Global
            ? global
            : local;
    }

    void setGlobal(const FormattingPtr& global)
    {
        ASSERT(global);
        this->global = global;
        if (mode == FormattingSettings::Mode::Global) {
            auto old = current;
            current = this->global;
            parent->forAll(itemsUpdater(old, current));
        }
    }

    void setLocal(const FormattingPtr& local)
    {
        ASSERT(local);
        this->local = local;
        if (mode == FormattingSettings::Mode::Local) {
            auto old = current;
            current = this->local;
            parent->forAll(itemsUpdater(old, current));
        }
    }

    void setMode(FormattingSettings::Mode mode)
    {
        if (mode != this->mode) {
            this->mode = mode;
            auto old = current;
            current = this->mode == FormattingSettings::Mode::Global
                ? global
                : local;
            parent->forAll(itemsUpdater(old, current));
        }
    }

    std::function<void(TextItem*)> itemsUpdater(
        const FormattingPtr& oldFs,
        const FormattingPtr& newFs) const
    {
        return [oldFs, newFs] (TextItem* i) {
            oldFs->remove(i);
            newFs->add(i);
            i->forceUpdate();
        };
    }

    FormattingSettings::Mode mode;
    FormattingPtr global;
    FormattingPtr local;
    FormattingPtr current;

    FormattingSettings* parent;
};


FormattingSettings::FormattingSettings(
        const FormattingPtr& global,
        const FormattingPtr& local,
        Mode mode)
    : impl_(new Impl(global, local, mode))
{
    impl_->parent = this;
}

FormattingSettings::FormattingSettings(FormattingSettings&&) = default;
FormattingSettings& FormattingSettings::operator = (FormattingSettings&&) = default;

FormattingSettings::~FormattingSettings() {}

const FormattingPtr& FormattingSettings::current() const { return impl_->current; }
const FormattingPtr& FormattingSettings::global() const { return impl_->global; }
const FormattingPtr& FormattingSettings::local() const { return impl_->local; }

void FormattingSettings::setGlobal(const FormattingPtr& global) { impl_->setGlobal(global); }
void FormattingSettings::setLocal(const FormattingPtr& local) { impl_->setLocal(local); }

FormattingSettings::Mode FormattingSettings::mode() const { return impl_->mode; }
void FormattingSettings::setMode(Mode mode) { impl_->setMode(mode); }

void FormattingSettings::add(TextItem *i)
{
    TextItemFormatter::add(i);
    current()->add(i);
}
void FormattingSettings::remove(TextItem *i)
{
    current()->remove(i);
    TextItemFormatter::remove(i);
}

} // namespace doctpl
