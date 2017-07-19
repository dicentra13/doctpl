#ifndef TEMPLATE_EDITOR_H
#define TEMPLATE_EDITOR_H

#include "../documenttemplate.h"

#include <QMainWindow>
#include <QString>

#include <map>
#include <vector>
#include <memory>
#include<boost/ptr_container/ptr_vector.hpp>

class TemplateViewMode;
class PageTools;
class FieldTools;

class QAction;
class QDockWidget;
class QComboBox;
class QTabWidget;

class TemplateEditor : public QMainWindow {
     
     Q_OBJECT

public:
     TemplateEditor();
     TemplateViewMode *defaultViewMode();
     const QString getSetting(const QString &key);
     void updatePageInformation();
     void updateFieldInformation();
     void updateCurrentFieldName(const QString &);
     void setCurrentDocumentModified();
     QDockWidget *fieldSettingsWidget() {
         return fieldSettings_;
     }

protected:
     void closeEvent(QCloseEvent *event);
signals:
     void currentDocumentChanged(DocumentTemplate *current);
private slots:
     void newFile();
     void open();
     void save();
     void saveAs();
     void saveAll();
     void close();
     void closeAll();
     void onExit();

     void openSourceWidget();

     void about();
     void showSettings();
     void documentWasModified();
     void changeCurrentDocument(int);
     void documentAboutToClose(int);
private:
     void saveDocument(int document, const QString &filename);

     void createFileActions();
     void updateFileActions();

     void createSourceActions();
     void updateSourceActions();
     void updateAllActions();

     void createMenus();

     void createStatusBar();
     void loadFile(const QString &fileName);
     bool saveFile(const QString &fileName);
     void setCurrentFile(const QString &fileName);
     QString strippedName(const QString &fullFileName);

     std::map<QString, QString> settings_;

     int currentDocument_;
     boost::ptr_vector<DocumentTemplate> documents_;

     QMenu *fileMenu_;
     QMenu *editMenu_;

     QMenu *helpMenu_;

     // document actions
     QAction *newAct_;
     QAction *openAct_;
     QAction *saveAct_;
     QAction *saveAsAct_;
     QAction *saveAllAct_;
     QAction *exitAct_;
     QAction *closeAct_;
     QAction *closeAllAct_;

     // source actions
     QAction *sourceWidgetOpenAct_;
     QAction *aboutAct_;
     QAction *settingsAct_;

     QDockWidget *fieldSettings_;

     QComboBox *viewModeSelector_;
     QTabWidget *documentWidget_;

     PageTools *pageTools_;
     FieldTools *fieldTools_;
};

#endif
