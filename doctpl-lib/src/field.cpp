#include <doctpl/field.h>

#include <doctpl/page.h>
#include <doctpl/exception.h>

namespace doctpl {

namespace {

Field::ID gen()
{
    static Field::ID s_id = 0;
    return ++s_id;
}

} // namespace

class Field::Impl {
public:
    explicit Impl(const QString &name)
        : id(gen())
        , name(name)
    {}

    ID id;
    QString name;
};


Field::Field(
        const QString &name,
        const QSizeF& size,
        const QPointF& pos,
        StylePtr style,
        Page* page)
    : LayoutItem(
        page ? page->layout() : nullptr,
        size,
        std::move(style))
    , impl_(new Impl(name))
{
    ASSERT(page);
    setParentItem(page->fieldsParent());
    setPos(pos);
}

Field::~Field() {}

void Field::setWidth(double width)
{
    LayoutItem::setWidth(width);
}

void Field::setHeight(double height)
{
    LayoutItem::setHeight(height);
}

const Field::ID& Field::id() const { return impl_->id; }

Page* Field::page() const
{
    Page* p;
    QGraphicsItem* parent = const_cast<Field*>(this);
    do {
        parent = parent->parentItem();
        p = dynamic_cast<Page*>(parent);
    } while (p == nullptr && parent != nullptr);
    return p;
}

void Field::setName(const QString& name) { impl_->name = name; }
const QString& Field::name() const { return impl_->name; }

} // namespace doctpl
