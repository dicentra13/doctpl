#include <doctpl/style_options.h>

#include <map>

namespace doctpl {

struct Style::Impl {
    Impl()
        : modifier(StyleModifiers::NO_PAINT)
    {
        lineStyles.emplace(LineStyleRole::FieldFrame, QPen(Qt::NoPen));
        lineStyles.emplace(LineStyleRole::PageFrame, QPen(Qt::NoPen));
        lineStyles.emplace(LineStyleRole::SelectedCellFrame, QPen(Qt::NoPen));
        lineStyles.emplace(LineStyleRole::SelectedFieldFrame, QPen(Qt::NoPen));
        lineStyles.emplace(LineStyleRole::SelectedPageFrame, QPen(Qt::NoPen));
        lineStyles.emplace(LineStyleRole::TableEmptyRowsCross, QPen(Qt::NoPen));
        lineStyles.emplace(LineStyleRole::TableInnerBorders, QPen(Qt::NoPen));

        backgroundStyles.emplace(BackgroundStyleRole::Field, QBrush(Qt::NoBrush));
        backgroundStyles.emplace(BackgroundStyleRole::Page, QBrush(Qt::NoBrush));
        backgroundStyles.emplace(BackgroundStyleRole::SelectedCell, QBrush(Qt::NoBrush));
        backgroundStyles.emplace(BackgroundStyleRole::SelectedField, QBrush(Qt::NoBrush));
        backgroundStyles.emplace(BackgroundStyleRole::SelectedPage, QBrush(Qt::NoBrush));
    }

    StyleModifier modifier;
    std::map<LineStyleRole, QPen> lineStyles;
    std::map<BackgroundStyleRole, QBrush> backgroundStyles;
};

Style::Style()
    : impl_(new Impl)
{}

Style::Style(const Style& o)
    : impl_(new Impl(*o.impl_))
{}

Style& Style::operator = (const Style& o)
{
    impl_.reset(new Impl(*o.impl_));
    return *this;
}

Style::Style(Style&&) = default;
Style& Style::operator = (Style&&) = default;

Style::~Style() = default;

StyleModifier Style::modifier() const { return impl_->modifier; }
void Style::setModifier(StyleModifier modifier) { impl_->modifier = modifier; }

void Style::setPen(const QPen& pen, LineStyleRole role)
{
    impl_->lineStyles[role] = pen;
}

const QPen& Style::pen(LineStyleRole role) const
{
    return impl_->lineStyles.at(role);
}

void Style::setBrush(const QBrush& brush, BackgroundStyleRole role)
{
    impl_->backgroundStyles[role] = brush;
}

const QBrush& Style::brush(BackgroundStyleRole role) const
{
    return impl_->backgroundStyles.at(role);
}


StylePtr defaultStyle()
{
    static StylePtr s_style = std::make_shared<Style>();
    return s_style;
}

} // namespace doctpl
