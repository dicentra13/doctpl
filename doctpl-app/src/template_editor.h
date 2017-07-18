#pragma once

#include <doctpl/template.h>

#include <QMainWindow>

#include <vector>

class QTabWidget;
class QMenu;
class QAction;

class TemplateEditor : public QMainWindow
{
    Q_OBJECT

public:
    explicit TemplateEditor(QWidget* parent = nullptr);

    ~TemplateEditor();

private slots:
    // TODO void onCurrentDocumentChanged(int docIdx);

private:
    // menus
    QMenu* fileMenu_;
    QMenu* editMenu_;
    QMenu* helpMenu_;

    // document actions
    QAction* newAct_;
    QAction* openAct_;
    QAction* saveAct_;
    QAction* saveAsAct_;
    QAction* saveAllAct_;
    QAction* exitAct_;
    QAction* closeAct_;
    QAction* closeAllAct_;
    
    QTabWidget* documentWidget_;

    using DoctplTemplatePtr = std::unique_ptr<doctpl::Template>;

    std::vector<DoctplTemplatePtr> documents_;
    size_t currentDocumentIdx_;
};
