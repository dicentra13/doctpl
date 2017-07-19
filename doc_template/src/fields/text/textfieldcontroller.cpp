#include "textfieldcontroller.h"

#include "textfield.h"
#include "textfieldwidget.h"
#include "textfieldcreationdialog.h"
#include "../textrenderinfo.h"
#include "../../template/templatelayout.h"
#include "textfieldxmlconvertor.h"
#include "../../xmlconvertor.h"

#include <QWidget>
#include <QMessageBox>
#include <QDockWidget>
#include <QAction>

/** если эти виджеты 0, то контроллер создается без оконного режима
  слоты не связываются
  */
TextFieldController::TextFieldController(QWidget *dialogParent,
         QDockWidget *settingsParent)
    : FieldController("TextField")
    , currentField_(0)
    , dialogParent_(dialogParent)
    , dockWidget_(settingsParent)
{
    if (dialogParent_ && settingsWidget_) {
        settingsWidget_ = new TextFieldWidget(this, settingsParent);
        connect(settingsWidget_, SIGNAL(nameChanged(QString)),
            this, SLOT(nameChanged(QString)));
        connect(settingsWidget_, SIGNAL(pageChanged(size_t)),
            this, SLOT(pageChanged(size_t)));
        connect(settingsWidget_, SIGNAL(xPosChanged(double)),
            this, SLOT(xPosChanged(double)));
        connect(settingsWidget_, SIGNAL(yPosChanged(double)),
            this, SLOT(yPosChanged(double)));
        connect(settingsWidget_, SIGNAL(widthChanged(double)),
            this, SLOT(widthChanged(double)));
        connect(settingsWidget_, SIGNAL(heightChanged(double)),
            this, SLOT(heightChanged(double)));
        connect(settingsWidget_, SIGNAL(textChanged(QString)),
            this, SLOT(textChanged(QString)));
        connect(settingsWidget_, SIGNAL(settingsSetToLocal(bool)),
            this, SLOT(setSettingsToLocal(bool)));
        settingsWidget_->hide();
        newFieldAct_ = new QAction(QIcon(":/icons/images/fields/text.png"),
                tr(type_.toStdString().c_str()), this);
    } else {
        settingsWidget_ = 0;
    }
}

TextFieldController::~TextFieldController() {
    if (settingsWidget_) {
        settingsWidget_->setParent(0);
        delete settingsWidget_;
    }
}

void TextFieldController::setCurrentField(TextField *f) {
    currentField_= f;
    if (settingsWidget_) {
        if (currentField_) {
            loadDataToWidget();
        } else {
            settingsWidget_->hide();
            dockWidget_->hide();
        }
    }
}

std::unique_ptr<Field> TextFieldController::createField(TextFieldCreationDialog *dialog)
{
    std::unique_ptr<Field> field;
    if (!checkName(dialog->name())) {
        QMessageBox::warning(dialog, tr("DLG_FIELD_CREATE"),
            tr("DLG_FIELD_CREATE_EXISTENT_NAME_MESSAGE"), QMessageBox::Ok);
    } else if (!checkGeometry(dialog->pageNumber(), dialog->x(), dialog->y(),
        dialog->width(), dialog->height()))
    {
        QMessageBox::warning(dialog, tr("DLG_FIELD_CREATE"),
            tr("DLG_FIELD_CREATE_INCORRECT_GEOMETRY_MESSAGE"), QMessageBox::Ok);
    } else {
        std::unique_ptr<TextField> newField(new TextField(this, dialog->width(),
            dialog->height(), TextRenderInfo::defaults(), false));
        fields_.insert(std::pair<QString, TextField *>(dialog->name(),
            newField.get()));
        field.reset(newField.release());
    }
    return field;
}

std::unique_ptr<Field> TextFieldController::createField(TextFieldXMLConvertor *convertor,
    size_t page)
{
    if (!checkName(convertor->name())) {
        THROW_EX(Exception,
            tr("DLG_FIELD_CREATE_EXISTENT_NAME_MESSAGE").toStdString());
    } else if (!checkGeometry(page, convertor->x(), convertor->y(),
        convertor->width(), convertor->height()))
    {
        THROW_EX(Exception,
            tr("DLG_FIELD_CREATE_INCORRECT_GEOMETRY_MESSAGE").toStdString());
    } else {
        std::unique_ptr<TextField> newField(new TextField(
            this, convertor->width(), convertor->height(),
            convertor->formatting(), convertor->isLocalFormatting()));
        newField->setText(convertor->text());
        fields_.insert(std::pair<QString, TextField *>(convertor->name(),
            newField.get()));
        return std::unique_ptr<Field>(newField.release());
    }
}

/** связывается только при создании в оконном режиме */
void TextFieldController::textChanged(QString text) {
    if (currentField_ && currentField_->text() != text) {
        currentField_->setText(text);
        templateLayout_->setModified(true);
    }
}

void TextFieldController::setSettingsToLocal(bool isLocal) {
    if (currentField_) {
        currentField_->switchFormattingSettings(isLocal);
        if (isLocal) {
            settingsWidget_->setFormatting(currentField_->textRenderInfo());
        }
    }
}

FieldCreationDialog *TextFieldController::creationDialog() {
    return new TextFieldCreationDialog(this, dialogParent_);
}

/**
  Загружает данные текущего поля в виджет настроек
  */
void TextFieldController::loadDataToWidget() {
    // loads common settings
    //FieldController::loadDataToWidget();
    settingsWidget_->setField(currentField_);
    dockWidget_->setWidget(settingsWidget_);
    dockWidget_->show();
    settingsWidget_->show();
}

QAction *TextFieldController::newFieldAction() {
    return newFieldAct_;
}

void TextFieldController::setLayout(TemplateLayout *tl) {
    FieldController::setLayout(tl);
    setCurrentField(0);
}

FieldSettingsWidget *TextFieldController::settingsWidget() {
    return settingsWidget_;
}

Field *TextFieldController::currentField() const {
    return currentField_;
}

TextField *TextFieldController::field(const QString &name) const {
    std::map<QString, TextField *>::const_iterator i =
        fields_.find(name);
    return (i != fields_.end()) ? i->second : 0;
}

void TextFieldController::removeField(const QString &name) {
    fields_.erase(name);
}

FieldXMLConvertor *TextFieldController::getXMLConvertor(XMLConvertor *conv) {
    return new TextFieldXMLConvertor(templateLayout_, conv, this);
}

void TextFieldController::renameField(const QString &oldName,
    const QString &newName)
{
    std::map<QString, TextField *>::iterator i = fields_.find(oldName);
    if (i != fields_.end()){
        TextField *f = i->second;
        fields_.erase(i);
        fields_.insert(std::pair<QString, TextField *>(newName, f));
    } else {
        THROW_EX(Exception, "TextField required with unknown name");
    }
}
