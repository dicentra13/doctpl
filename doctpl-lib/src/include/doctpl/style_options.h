#pragma once

#include <QBrush>
#include <QPen>

#include <memory>

namespace doctpl {

enum class LineStyleRole {
    PageFrame,
    SelectedPageFrame,

    FieldFrame,
    SelectedFieldFrame,

    TableInnerBorders,
    TableEmptyRowsCross,
    SelectedCellFrame
};

enum class BackgroundStyleRole {
    Page,
    SelectedPage,

    Field,
    SelectedField,

    SelectedCell
};

class StyleModifiers {
public:
    static const uint32_t NO_PAINT = 0x0u;

    static const uint32_t ENABLE_PAGE_FRAME = 0x1u;
    static const uint32_t ENABLE_PAGE_BACKGROUND = 0x2u;

    static const uint32_t ENABLE_FIELD_FRAME = 0x4u;
    static const uint32_t ENABLE_FIELD_BACKGROUND = 0x8u;

    static const uint32_t ENABLE_SELECTION_FRAME = 0x16u;
    static const uint32_t ENABLE_SELECTION_BACKGROUND = 0x32u;

    static const uint32_t ENABLE_TABLE_INNER_BORDERS = 0x64u;
    static const uint32_t ENABLE_TABLE_EMPTY_ROWS_CROSSING = 0x128u;
    static const uint32_t ENABLE_TABLE_SELECTED_CELL_FRAME = 0x256u;
    static const uint32_t ENABLE_TABLE_SELECTED_CELL_BACKGROUND = 0x512u;
};

typedef uint32_t StyleModifier;

class Style {
public:
    /// creates empty style - no painting
    Style();

    Style(const Style&);
    Style& operator = (const Style&);

    Style(Style&&);
    Style& operator = (Style&&);

    ~Style();

    StyleModifier modifier() const;
    void setModifier(StyleModifier modifier);

    void setPen(const QPen& pen, LineStyleRole role);
    const QPen& pen(LineStyleRole role) const;

    void setBrush(const QBrush& brush, BackgroundStyleRole role);
    const QBrush& brush(BackgroundStyleRole role) const;

private:
    class Impl;

    std::unique_ptr<Impl> impl_;
};

typedef std::shared_ptr<Style> StylePtr;

/// returns deafult (empty) Style instance
StylePtr defaultStyle();

} // namespace doctpl
