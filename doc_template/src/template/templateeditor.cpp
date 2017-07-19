#include "templateeditor.h"
#include "templateview.h"
#include "templatelayout.h"
#include "../documenttemplate.h"
#include "../pagetools.h"
#include "../fieldtools.h"
#include "../fitwidthmode.h"
#include "../settings.h"

#include "templatesourcesmanager.h"
#include "../sources/sourceswidget.h"
#include "templateformatsettingsdialog.h"

#include "../xmlconvertor.h"
#include "../fields/fieldxmlconvertor.h"

#include <QtGui>
#include <QtXml>
#include <QDockWidget>
#include <QTabWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QAction>
#include <QToolBar>
#include <QMenuBar>
#include <QMenu>
#include <QStatusBar>

#include <vector>

const double mm2pt = 72.0 / 25.4;

TemplateEditor::TemplateEditor() {

    Settings::readFromFile("");
    documentWidget_ = new QTabWidget(this);
    setCentralWidget(documentWidget_);
    connect(documentWidget_, SIGNAL(currentChanged(int)),
        this, SLOT(changeCurrentDocument(int)));
    connect(documentWidget_, SIGNAL(tabCloseRequested(int)),
        this, SLOT(documentAboutToClose(int)));
    currentDocument_ = -1;

    pageTools_ = new PageTools(this);
    connect(this, SIGNAL(currentDocumentChanged(DocumentTemplate*)),
        pageTools_, SLOT(currentDocumentChanged(DocumentTemplate*)));
    addToolBar(pageTools_->toolBar());
    pageTools_->currentDocumentChanged(0);
    fieldTools_ = new FieldTools(this);
    connect(this, SIGNAL(currentDocumentChanged(DocumentTemplate*)),
        fieldTools_, SLOT(currentDocumentChanged(DocumentTemplate*)));
    connect(pageTools_, SIGNAL(pageConfigurationChanged()),
        fieldTools_, SLOT(updatePageInformation()));
    addToolBar(fieldTools_->toolBar());
    fieldTools_->currentDocumentChanged(0);
    fieldSettings_ = new QDockWidget(this);
    fieldSettings_->setFeatures(QDockWidget::NoDockWidgetFeatures);
    fieldSettings_->hide();
    this->addDockWidget(Qt::RightDockWidgetArea, fieldSettings_);
    createFileActions();
    createSourceActions();

    createMenus();
    createStatusBar();


    setCurrentFile("");
    setUnifiedTitleAndToolBarOnMac(true);

    updateAllActions();
}

TemplateViewMode *TemplateEditor::defaultViewMode() {

    return FitWidthMode::instance();
}

const QString TemplateEditor::getSetting(const QString &key) {

    std::map<QString, QString>::iterator i = settings_.find(key);
    if (i != settings_.end())
        return i->second;
    else
        return "";
}

void TemplateEditor::closeEvent(QCloseEvent *event) {

     event->accept();
}


void TemplateEditor::newFile() {

    std::unique_ptr<DocumentTemplate> newDoc(new DocumentTemplate("", this));

    documentWidget_->addTab(newDoc->view(), "Untitled");
    documents_.push_back(newDoc.release());
    currentDocument_ = documents_.size() - 1;
    documentWidget_->setCurrentIndex(documents_.size() - 1);
    changeCurrentDocument(documents_.size() - 1);
    updateAllActions();
}

void TemplateEditor::open() {
    // FIXME если произошла ошибка, документ все равно создается
    QString filename = QFileDialog::getOpenFileName(
            this, tr("Open Document"), QDir::currentPath(),
            tr("Template files (*.xml)") );
    if (filename.isEmpty()) {
        return;
    }

    std::unique_ptr<DocumentTemplate> newDoc(
        new DocumentTemplate(filename, this));
    XMLConvertor conv;

    try {
        conv.readFromFile(newDoc.get(), filename);

        documentWidget_->addTab(newDoc->view(), filename);
        if (newDoc->numberOfPages() != 0)
            newDoc->setCurrentPage(1);
        documents_.push_back(newDoc.release());
        currentDocument_ = documents_.size() - 1;
        documentWidget_->setCurrentIndex(documents_.size() - 1);
        updateAllActions();
    } catch (XMLParseError &err) {
        std::stringstream message;
        message << tr("LINE").toStdString() << " " << err.xmlLine() << " "
                << tr("COLUMN").toStdString() << " " << err.xmlSymbol()
                << ": " << err.cause();
        QMessageBox::critical(this, tr("XML_PARSING_ERROR"),
                              QString::fromLocal8Bit(message.str().c_str()));
        fieldSettings_->hide();
    } catch (Exception &err) {
        QMessageBox::critical(this, tr("XML_PARSING_ERROR"),
                              QString::fromLocal8Bit(err.cause().c_str()));
        fieldSettings_->hide();
    }
    changeCurrentDocument(documents_.size() - 1);
}

void TemplateEditor::save() {
    QString filename = documents_[currentDocument_].fileName();
    if (filename.isEmpty()) {
        filename = QFileDialog::getSaveFileName(
            this, tr("Save Document"), QDir::currentPath(),
            tr("Template files (*.xml)") );
        if (filename.isEmpty()) {
            return;
        }
        documents_[currentDocument_].setFileName(filename);
    }
    saveDocument(currentDocument_, filename);

    std::vector<size_t> v;
    v.push_back(1);
    documents_[currentDocument_].printToFile("1", "attestate_test.pdf", v);
}

void TemplateEditor::saveDocument(int document, const QString &filename) {
    XMLConvertor conv;
    conv.writeToFile(&documents_[document], filename);

}

void TemplateEditor::saveAs() {
    QString filename = QFileDialog::getSaveFileName(
        this, tr("Save Document as"), QDir::currentPath(),
        tr("Template files (*.xml)") );
    if (!filename.isEmpty()) {
        documents_[currentDocument_].setFileName(filename);
        saveDocument(currentDocument_, filename);
    }
}

void TemplateEditor::about() {

    //QMessageBox::about(this, tr(""),
    //         tr(""));
}

void TemplateEditor::documentWasModified() {

     //setWindowModified(textEdit->document()->isModified());
}

void TemplateEditor::createFileActions() {

    newAct_ = new QAction(tr("FILE_NEW"), this);
    newAct_->setShortcuts(QKeySequence::New);
    newAct_->setStatusTip(tr("FILE_NEW_TIP"));
    connect(newAct_, SIGNAL(triggered()), this, SLOT(newFile()));

    openAct_ = new QAction(tr("FILE_OPEN") + "...", this);
    openAct_->setShortcuts(QKeySequence::Open);
    openAct_->setStatusTip(tr("FILE_OPEN_TIP"));
    connect(openAct_, SIGNAL(triggered()), this, SLOT(open()));

    saveAct_ = new QAction(tr("FILE_SAVE"), this);
    saveAct_->setShortcuts(QKeySequence::Save);
    saveAct_->setStatusTip(tr("FILE_SAVE_TIP"));
    connect(saveAct_, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAct_ = new QAction(tr("FILE_SAVE_AS") + "...", this);
    saveAsAct_->setShortcuts(QKeySequence::SaveAs);
    saveAsAct_->setStatusTip(tr("FILE_SAVE_AS_TIP"));
    connect(saveAsAct_, SIGNAL(triggered()), this, SLOT(saveAs()));

    saveAllAct_ = new QAction(tr("FILE_SAVE_ALL") + "...", this);
    saveAllAct_->setStatusTip(tr("FILE_SAVE_ALL_TIP"));
    connect(saveAllAct_, SIGNAL(triggered()), this, SLOT(saveAll()));

    closeAct_ = new QAction(tr("FILE_CLOSE"), this);
    closeAct_->setShortcuts(QKeySequence::Close);
    closeAct_->setStatusTip(tr("FILE_CLOSE_TIP"));
    connect(closeAct_, SIGNAL(triggered()), this, SLOT(close()));

    closeAllAct_ = new QAction(tr("FILE_CLOSE_ALL"), this);
    closeAllAct_->setStatusTip(tr("FILE_CLOSE_ALL_TIP"));
    connect(closeAllAct_, SIGNAL(triggered()), this, SLOT(closeAll()));

    exitAct_ = new QAction(tr("FILE_EXIT"), this);
    exitAct_->setShortcuts(QKeySequence::Quit);
    exitAct_->setStatusTip(tr("FILE_EXIT_TIP"));
    connect(exitAct_, SIGNAL(triggered()), this, SLOT(onExit()));

    aboutAct_ = new QAction(tr("FILE_ABOUT"), this);
    aboutAct_->setStatusTip(tr("FILE_ABOUT_TIP"));
    connect(aboutAct_, SIGNAL(triggered()), this, SLOT(about()));

    settingsAct_ = new QAction(tr("FILE_SETTINGS"), this);
    settingsAct_->setStatusTip(tr("FILE_SETTINGS_TIP"));
    connect(settingsAct_, SIGNAL(triggered()), this, SLOT(showSettings()));
}

void TemplateEditor::updateFileActions() {

    if (currentDocument_ == -1) {
        // текущий не установлен
        saveAct_->setEnabled(false);
        saveAsAct_->setEnabled(false);
        closeAct_->setEnabled(false);
        if (documents_.empty()) {
            // нет документов
            saveAllAct_->setEnabled(false);
            closeAllAct_->setEnabled(false);
        }
    } else {
        saveAct_->setEnabled(true);
        saveAsAct_->setEnabled(true);
        closeAct_->setEnabled(true);
        saveAllAct_->setEnabled(true);
        closeAllAct_->setEnabled(true);
    }
}

void TemplateEditor::createSourceActions() {
    sourceWidgetOpenAct_ = new QAction(tr("SOURCE_WIDGET_OPEN"), this);
    sourceWidgetOpenAct_->setStatusTip(tr("SOURCE_WIDGET_OPEN_TIP"));
    connect(sourceWidgetOpenAct_, SIGNAL(triggered()),
        this, SLOT(openSourceWidget()));
    QToolBar *source = addToolBar("SOURCE");
    source->addAction(sourceWidgetOpenAct_);
}

void TemplateEditor::updateSourceActions() {


}

void TemplateEditor::updatePageInformation() {
    pageTools_->updatePageActions();
//    QString currentTitle =
//        documentWidget_->tabText(documentWidget_->currentIndex());
//    if (currentTitle == "untitled*" &&
//        documents_[currentDocument_].second->numberOfPages() == 0)
//    {
//        documentWidget_->setTabText(documentWidget_->currentIndex(),
//            "untitled");
//    } else {
//        documentWidget_->setTabText(documentWidget_->currentIndex(),
//            currentTitle + "*");
//    }
}

void TemplateEditor::updateFieldInformation() {

    fieldTools_->updateFieldActions();
}

void TemplateEditor::updateAllActions() {

    updateFileActions();
    pageTools_->updatePageActions();
    fieldTools_->updateFieldActions();
    updateSourceActions();
}

void TemplateEditor::createMenus() {

    fileMenu_ = menuBar()->addMenu(tr("&File"));
    fileMenu_->addAction(newAct_);
    fileMenu_->addAction(openAct_);
    fileMenu_->addAction(saveAct_);
    fileMenu_->addAction(saveAsAct_);
    fileMenu_->addAction(saveAllAct_);
    fileMenu_->addAction(closeAct_);
    fileMenu_->addAction(closeAllAct_);
    fileMenu_->addSeparator();
    fileMenu_->addAction(exitAct_);

    editMenu_ = menuBar()->addMenu(tr("&Edit"));
    editMenu_->addMenu(pageTools_->menu());
    editMenu_->addMenu(fieldTools_->menu());
    editMenu_->addAction(settingsAct_);

    menuBar()->addSeparator();

    helpMenu_ = menuBar()->addMenu(tr("&Help"));
    helpMenu_->addAction(aboutAct_);
}

void TemplateEditor::createStatusBar() {

     statusBar()->showMessage(tr("Ready"));
}

void TemplateEditor::loadFile(const QString &/*fileName*/) {
/*
     QFile file(fileName);
     if (!file.open(QFile::ReadOnly | QFile::Text)) {
         QMessageBox::warning(this, tr("Application"),
                              tr("Cannot read file %1:\n%2.")
                              .arg(fileName)
                              .arg(file.errorString()));
         return;
     }

     QTextStream in(&file);
 #ifndef QT_NO_CURSOR
     QApplication::setOverrideCursor(Qt::WaitCursor);
 #endif
     textEdit->setPlainText(in.readAll());
 #ifndef QT_NO_CURSOR
     QApplication::restoreOverrideCursor();
 #endif

     setCurrentFile(fileName);
     statusBar()->showMessage(tr("File loaded"), 2000);
*/
}

bool TemplateEditor::saveFile(const QString &/*fileName*/) {
/*
     QFile file(fileName);
     if (!file.open(QFile::WriteOnly | QFile::Text)) {
         QMessageBox::warning(this, tr("Application"),
                              tr("Cannot write file %1:\n%2.")
                              .arg(fileName)
                              .arg(file.errorString()));
         return false;
     }

     QTextStream out(&file);
 #ifndef QT_NO_CURSOR
     QApplication::setOverrideCursor(Qt::WaitCursor);
 #endif
     out << textEdit->toPlainText();
 #ifndef QT_NO_CURSOR
     QApplication::restoreOverrideCursor();
 #endif

     setCurrentFile(fileName);
     statusBar()->showMessage(tr("File saved"), 2000);
*/
    return true;
}

void TemplateEditor::setCurrentFile(const QString &/*fileName*/) {


}

QString TemplateEditor::strippedName(const QString &fullFileName) {

     return QFileInfo(fullFileName).fileName();
}

void TemplateEditor::saveAll() {
    for (size_t document = 0; document < documents_.size(); ++document) {
        QString filename = documents_[document].fileName();
        if (filename.isEmpty()) {
            filename = QFileDialog::getSaveFileName(
                this, tr("Save Document"), QDir::currentPath(),
                tr("Template files (*.xml)") );
            if (filename.isEmpty()) {
                continue;
            }
            documents_[document].setFileName(filename);
        }
        saveDocument(document, filename);
    }
}

void TemplateEditor::close() {

}

void TemplateEditor::closeAll() {

}

void TemplateEditor::onExit() {

}

void TemplateEditor::openSourceWidget() {
    SourcesWidget *widget = new SourcesWidget(
        documents_[currentDocument_].sourcesManager(), this);
    widget->exec();
}

void TemplateEditor::changeCurrentDocument(int index) {
    DocumentTemplate *newCurrent;
    if (0 <= index && index < (int) documents_.size()) {
        currentDocument_ = index;
        newCurrent = &documents_[currentDocument_];
    } else {
        currentDocument_ = -1;
        newCurrent = 0;
    }
    emit currentDocumentChanged(newCurrent);
}

void TemplateEditor::setCurrentDocumentModified() {
    documentWidget_->setTabText(documentWidget_->currentIndex(),
        documentWidget_->tabText(documentWidget_->currentIndex()) + "*");
}

void TemplateEditor::documentAboutToClose(int /*index*/) {

}

void TemplateEditor::updateCurrentFieldName(const QString &name) {
    fieldTools_->currentFieldNameChanged(name);
}

void TemplateEditor::showSettings() {
    TemplateFormatSettingsDialog *d = new TemplateFormatSettingsDialog(this);
    d->exec();
}
