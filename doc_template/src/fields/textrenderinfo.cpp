#include "textrenderinfo.h"

#include "textitem.h"
#include "../settings.h"

TextRenderInfo *TextRenderInfo::globals_ = 0;

TextRenderInfo::TextRenderInfo(const QFont &font, int flags, Margins margins)
    : font_(font)
    , flags_(flags)
    , margins_(margins) {
    //font_.setPointSize(font_.pointSize());
    font_.setPixelSize(font_.pointSize());
}
/**
  FIXME
  реализовать получение из Settings
  */

TextRenderInfo *TextRenderInfo::defaults() {

    TextRenderInfo *prototype = new TextRenderInfo;
    prototype->setFont(globals()->font());
    prototype->setFlags(globals()->flags());
    prototype->setMargins(globals()->margins());
    return prototype;
}

TextRenderInfo *TextRenderInfo::globals() {
    if (!globals_) {
        globals_ = new TextRenderInfo(QFont(), Qt::AlignLeft | Qt::AlignBottom,
            Margins(1.0, 1.0, 1.0, 1.0));
    }
    return globals_;
}

void TextRenderInfo::updateItems() const {

    std::list<TextItem *>::const_iterator i;
    for (i = associatedItems_.begin();
        i != associatedItems_.end(); i++)
    {
        (*i)->needsUpdate();
    }
}
