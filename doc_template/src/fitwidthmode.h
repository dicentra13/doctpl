#ifndef FITWIDTHMODE_H
#define FITWIDTHMODE_H

#include "template/templateviewmode.h"

class TemplateLayout;
class TemplateView;

class QWheelEvent;
class QMouseEvent;
class QResizeEvent;

class FitWidthMode : public TemplateViewMode {
public:
    static TemplateViewMode *instance();

    virtual void adjustCurrentField(TemplateLayout *tl, TemplateView *tv);
    virtual void adjustCurrentPage(TemplateLayout *tl, TemplateView *tv);
    virtual void processWheelEvent(TemplateLayout *tl, TemplateView *tv,
        QWheelEvent *event);
    virtual void processResizeEvent(TemplateLayout *tl, TemplateView *tv,
        QResizeEvent *event);
    virtual void processScrollByEvent(TemplateLayout *tl, TemplateView *tv,
        int dx, int dy);
    virtual void processDoubleClickEvent(TemplateLayout *tl, TemplateView *tv,
        QMouseEvent *event);
    virtual void adjust(TemplateLayout *tl, TemplateView *tv);
protected:
    FitWidthMode() {};
private:
    static FitWidthMode *instance_;
};

#endif // FITWIDTHMODE_H
