#ifndef SOURCESWIDGET_H
#define SOURCESWIDGET_H

#include <QDialog>
#include <QString>
#include <QStringList>

#include <map>
#include <vector>

class TemplateSourcesManager;
class QComboBox;
class QListWidget;
class QAction;
class QPushButton;
class QSplitter;
class QToolBar;
class QCloseEvent;

class SourcesWidget : public QDialog {

    Q_OBJECT

public:
    SourcesWidget(TemplateSourcesManager *sourcesManager, QWidget *parent);
    ~SourcesWidget();
protected:
     void closeEvent(QCloseEvent *event);
private slots:
    void currentSourceIndexChanged(int);
    void newSource();
    void editCurrentSource();
    void deleteCurrentSource();
    void addFieldsToCurrentSource();
    void removeFieldsFromCurrentSource();
private:
    Q_DISABLE_COPY(SourcesWidget)

    static const int sourceSelectorWidth_;
    static const int sourceLabelWidth_;

    void generateLayout();
    void loadSourcesFromManager();
    void renameCurrentSourceInWidget();
    void reassignFieldsInCurrentSource();
    void updateAddRemoveActions();
    void loadDataToFieldWidget(QListWidget *fieldWidget, QStringList names);
    void clearFieldWidget(QListWidget *fieldWidget);

    QString currentSourceName_;
    QString currentSourceType_;
    TemplateSourcesManager *sourcesManager_;

    QComboBox *sourceSelector_;
    std::vector<QAction *> newSourceActions_;
    std::vector<std::pair<QString, QString> > newSourceTypesSubtypes_;
    QAction *editSourceAct_;
    QAction *deleteSourceAct_;
    QPushButton *addToSource_, *removeFromSource_;
    QSplitter *fieldsWidgetsSplitter_;
    QListWidget *assignedFields_, *unassignedFields_;
    QToolBar *sourceToolbar_;
};

#endif // SOURCESWIDGET_H
