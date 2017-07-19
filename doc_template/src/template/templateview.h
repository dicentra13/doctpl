#ifndef TEMPLATEVIEW_H
#define TEMPLATEVIEW_H

#include <QGraphicsView>
#include <QString>
#include <QPointF>

class TemplateLayout;
class TemplateViewMode;
class Page;
class Field;

class QWheelEvent;
class QMouseEvent;
class QResizeEvent;

/**
    Класс, отвечающий за просмотр шаблона
    Имеет следующие режимы просмотра:
    - fitPage (по умолчанию)
        Просмотр постранично. Текущая страница полностью умещается в viewport.
        Горизонтальная прокрутка выключена.
        Вертикальная прокрутка перелистывает страницы. Когда слайдер
        вертикальной полосы прокрутки переходит в зону следующей страницы,
        она становится текущей (сигнал currentPageChanged)
        Смена текущей страницы также может осуществляться извне вызовом
        setCurrentPage
        DoubleClick ничего не меняет (страница и так текущая).
        Drag режим не работает.
        TODO нажатие Ctrl с wheel переводит в режим dynamicZoom
    - fitWidth
        viewport отмасштабирован по ширине самой широкой страницы.
        Горизонтальная прокуртка выключена.
        Вертикальная прокрутка работает в обычном режиме (унасл. QGraphicsView)
        Как только следующая по направлению прокрутки страница показывается в
        viewport более чем наполовину, она становится текущей.
        При установке текущей страницы извне, ее верхний край выравнивается по
        верхнему краю viewport (то же происходит при изменении извне размеров
        текущей страницы).
        DoubleClick меняет текущую страницу (если она действительно сменилась),
        но не трансформирует viewport.
        Drag режим работает в унаследованной реализации (перетаскивание в
        границах sceneRect).
        TODO нажатие Ctrl с wheel переводит в режим dynamicZoom
    - dynamicZoom
        Полосы прокрутки, resizeEvent и drag работают в унаследованной
        реализации.
        wheelEvent трактуется как масштабирование (minScaleFactor и
        MaxScaleFactor устанавливаюся извне).
        При переходе в этот режим из других текущая страница не меняется.
        TODO Текущая страница
            1) если текущая страница уходит
                более чем наполовину из viewport, то оказавшаяся во viewport
                становится текущей
                в противном случае
            2) та, по которой кликнули
        Важно!!! Выбор поля на любой странице делает ее текущей
  */
class TemplateView : public QGraphicsView {

    Q_OBJECT

public:
    TemplateView(TemplateLayout *layout, TemplateViewMode *mode,
        QWidget *parent = 0);
    virtual ~TemplateView();

    void adjustCurrentPage();
    void adjustCurrentField();
    QPointF topLeft() const {
        return topLeft_;
    }
    QPointF bottomRight() const {
        return bottomRight_;
    }
    TemplateViewMode *mode() const {
        return mode_;
    }
    Page *currentPage() const;
    Field *currentField() const;
signals:
    void pageSelected(Page *);
    void fieldSelected(Field *);
protected:
    //virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void wheelEvent(QWheelEvent *event);
    virtual void resizeEvent(QResizeEvent *event);
    virtual void scrollContentsBy(int dx, int dy);
    void selectedPageChanged(Page *p);
    void selectedFieldChanged(Field *f);
private:
    friend class TemplateViewMode;

    void setMode(TemplateViewMode *mode);

    TemplateViewMode *mode_;
    TemplateLayout *templateLayout_;
    bool isResizing_;

    QPointF topLeft_, bottomRight_;
};

#endif // TEMPLATEVIEW_H
