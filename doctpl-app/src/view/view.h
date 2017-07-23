#pragma once

#include <QGraphicsView>
#include <QString>
#include <QPointF>

#include <memory>

namespace doctpl {

class Layout;
class Page;
class Field;

} // namespace doctpl

class QWheelEvent;
class QMouseEvent;
class QResizeEvent;

namespace view {

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

class ViewImpl;

class View : public QGraphicsView {

    Q_OBJECT

public:
    enum class Mode {FitPage, FitWidth};

    View(
        doctpl::Layout& layout,
        //Mode mode,
        QWidget* parent = nullptr);

    virtual ~View();

    //Mode mode() const { return mode_; }

    /// No signals emitted, adjust current object in view only
    void setCurrentField(doctpl::Field* f);
    void setCurrentPage(doctpl::Page* p);

    doctpl::Page* currentPage() const;
    doctpl::Field* currentField() const;

    doctpl::Page* pageAt(const QPoint& pos);
    doctpl::Field* fieldAt(const QPoint& pos);

    doctpl::Layout& layout();
    const doctpl::Layout& layout() const;

signals:
    void pageSelected(doctpl::Page*);
    void fieldSelected(doctpl::Field*);

protected:
    void adjustCurrentPage();
    void adjustCurrentField();

    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void wheelEvent(QWheelEvent *event);
    virtual void resizeEvent(QResizeEvent *event);
    virtual void scrollContentsBy(int dx, int dy);

    void selectedPageChanged(doctpl::Page* p);
    void selectedFieldChanged(doctpl::Field* f);

private:
    std::unique_ptr<ViewImpl> impl_;
    //Mode mode_;

    doctpl::Layout& layout_;

    doctpl::Page* currentPage_;
    doctpl::Field* currentField_;
};

} // namespace view
