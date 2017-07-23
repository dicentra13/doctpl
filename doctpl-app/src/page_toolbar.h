#pragma once

#include <QObject>

namespace view {

class View;

} // namespace view

namespace doctpl {

class Page;

} // namespace doctpl

class QWidget;
class QToolBar;
class QMenu;
class QLineEdit;
class QIntValidator;
class QLabel;
class QAction;

class PageToolbar : public QObject {

    Q_OBJECT

public:
    explicit PageToolbar(QWidget* parent);

    ~PageToolbar();

    QToolBar* toolBar();
    QMenu* menu();

signals:
    void pageConfigurationChanged();

public slots:
    void onCurrentDocumentChanged(view::View* view);

protected slots:
    void onCreate();
    void onEdit();
    void onDelete();

    void onSelectorValueChanged();
    void onNavigation();

private:
    void initNavigationActions();
    void initEditingActions();

    void updateSelectorValue();
    void updatePagesCount();
    void updateEditingActions();
    void updateNavigationActions();

    void onCurrentPageChanged();

    QToolBar* toolBar_;
    QMenu* menu_;
    QLineEdit* selector_;
    QIntValidator* validator_;

    QLabel* pagesCount_;

    QAction* createAct_;
    QAction* editAct_;
    QAction* deleteAct_;

    /// navigation actions
    QAction* prevAct_;
    QAction* nextAct_;

    view::View* currentView_;
};
