#ifndef TEXT_RENDER_INFO_H
#define TEXT_RENDER_INFO_H

#include <QFont>
#include <list>

#include <boost/ptr_container/ptr_vector.hpp>

class TextItem;

/// Margins in millimeters
struct Margins {
    Margins()
        : top(0.0)
        , right(0.0)
        , bottom(0.0)
        , left(0.0) {}
    Margins(double top, double right, double bottom, double left)
        : top(top)
        , right(right)
        , bottom(bottom)
        , left(left) {}

    double top;
    double right;
    double bottom;
    double left;
};

class TextRenderInfo {
public:

    TextRenderInfo(const QFont &font = QFont(),
        int flags = Qt::AlignLeft | Qt::AlignBottom,
        Margins margins = Margins());

    static TextRenderInfo *defaults();
    static TextRenderInfo *globals();

    void setFont(const QFont &font) {
        font_ = font;
        updateItems();
    }

    void setFlags(int flags) {
        flags_ = flags;
        updateItems();
    }

    void setMargins(const Margins &margins)
    {
        margins_ = margins;
        updateItems();
    }

    const QFont &font() const {
        return font_;
    }

    int flags() const {
        return flags_;
    }
    Margins margins() const
    {
        return margins_;
    }
    void addItem(TextItem *i) {
        associatedItems_.push_back(i);
    }
    void removeItem(TextItem *i) {
        std::list<TextItem *>::iterator it = associatedItems_.begin();
        while (it != associatedItems_.end() && *it != i)
            it++;
        if (it != associatedItems_.end())
            associatedItems_.erase(it);
    }
private:
    static TextRenderInfo *globals_;
    QFont font_;
    int flags_;
    Margins margins_;
    /// List of weak pointers
    std::list<TextItem *> associatedItems_;
    void updateItems() const;
};

typedef boost::ptr_vector<TextRenderInfo> TextRenderInfoList;

#endif // TEXTRENDERINFO_H
