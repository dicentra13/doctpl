#ifndef FIELDTOOLS_H
#define FIELDTOOLS_H

#include <QObject>
#include <QString>
#include <QStringList>

#include <vector>

class DocumentTemplate;
class QWidget;
class QToolBar;
class QMenu;
class QComboBox;
class QAction;

class FieldTools : public QObject {

    Q_OBJECT

public:
    explicit FieldTools(QWidget *parent);
    ~FieldTools();
    QToolBar *toolBar();
    QMenu *menu();
public slots:
    void currentDocumentChanged(DocumentTemplate *currentDoc);
    void updatePageInformation();
    void updateFieldActions();
    void currentFieldNameChanged(QString);
protected slots:
    void deleteField();
    void fieldChanged(QString); // field selector changed
private:
    void currentFieldChanged();
    void setNewFieldActionsEnabled(bool isEnabled);

    std::vector<QAction *> newFieldActions_;

    QToolBar *fieldToolBar_;
    QMenu *fieldMenu_;
    QComboBox *fieldSelector_;
    QAction *deleteFieldAct_;
    DocumentTemplate *currentDocument_;
    QWidget *parent_;
    bool isCurrentFieldChanging_;
};

#endif // FIELDTOOLS_H
