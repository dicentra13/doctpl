#ifndef PAGETOOLS_H
#define PAGETOOLS_H

#include <QObject>

class DocumentTemplate;
class Page;
class QWidget;
class QToolBar;
class QMenu;
class QLineEdit;
class QIntValidator;
class QLabel;
class QAction;

class PageTools : public QObject {

    Q_OBJECT

public:
    explicit PageTools(QWidget *parent);
    ~PageTools();
    QToolBar *toolBar();
    QMenu *menu();
signals:
    void pageConfigurationChanged();
public slots:
    void currentDocumentChanged(DocumentTemplate *currentDoc);
    void updatePageActions();
protected slots:
    void newPage();
    void editPage();
    void deletePage();
    void pageChanged(); // pageSelector_ edited
    void onPageSwitching();
private:
    void updatePageSwitchers();
    void currentPageChanged();

    QToolBar *pageToolBar_;
    QMenu *pageMenu_;
    QLineEdit *pageSelector_;
    QIntValidator *pageValidator_;
    QLabel *numberOfPages_;
    QAction *newPageAct_;
    QAction *editPageAct_;
    QAction *deletePageAct_;
    QAction *prevPageAct_;
    QAction *nextPageAct_;
    DocumentTemplate *currentDocument_;
    QWidget *parent_;
};

#endif // PAGETOOLS_H
