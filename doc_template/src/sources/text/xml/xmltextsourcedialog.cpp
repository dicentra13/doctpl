#include "xmltextsourcedialog.h"

#include "xmltextsource.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QMessageBox>

XMLTextSourceDialog::XMLTextSourceDialog(SourceNameValidator validator,
        QWidget *parent)
    : TextSourceDialog(validator, parent)
    , edited_(0)
{
    createWidgets();
}

XMLTextSourceDialog::XMLTextSourceDialog(SourceNameValidator validator,
        XMLTextSource *source, QWidget *parent)
    : TextSourceDialog(validator, source, parent)
    , edited_(source)
{
    createWidgets();
    fileName_->setText(source->fileName());
    query_->setText(source->query());
}

void XMLTextSourceDialog::createWidgets() {
    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);
    layout->addWidget(nameWidget());

    QHBoxLayout *filenameLayout = new QHBoxLayout;
    filenameLayout->addWidget(new QLabel(tr("DLG_XML_FILE"), this));
    fileName_ = new QLineEdit(this);
    filenameLayout->addWidget(fileName_);
    layout->addLayout(filenameLayout);

    QHBoxLayout *queryLayout = new QHBoxLayout;
    queryLayout->addWidget(new QLabel(tr("DLG_XML_TEXT_QUERY"), this));
    query_ = new QLineEdit(this);
    queryLayout->addWidget(query_);
    layout->addLayout(queryLayout);

    layout->addWidget(buttons());
}

std::unique_ptr<TextSource> XMLTextSourceDialog::createdSource() const {
    std::unique_ptr<TextSource> source;
    if (result() == QDialog::Accepted) {
        XMLTextSource *newXMLText = new XMLTextSource(name(),
            fileName_->text());
        newXMLText->setQuery(query_->text());
        source.reset(newXMLText);
    }
    return source;
}

void XMLTextSourceDialog::accept() {
    if (checkNonEmptyInput() && checkData()) {
        if (edited_) {
            edited_->setName(name_->text());
            edited_->setFileName(fileName_->text());
            edited_->setQuery(query_->text());
        }
        QDialog::accept();
    }
}

Source *XMLTextSourceDialog::source() {
    return edited_;
}

bool XMLTextSourceDialog::checkNonEmptyInput() {
    bool res = TextSourceDialog::checkNonEmptyInput();
    if (res) {
        if (fileName_->text().isEmpty()) {
            fileName_->setFocus();
            QMessageBox::warning(this, tr("SOURCE_DATA_VALIDATION"),
                tr("DLG_XML_SOURCE_EMPTY_FILE_NAME"), QMessageBox::Ok);
            res = false;
        } else if (query_->text().isEmpty()) {
            query_->setFocus();
            QMessageBox::warning(this, tr("SOURCE_DATA_VALIDATION"),
                tr("DLG_XML_TEXT_SOURCE_EMPTY_QUERY"), QMessageBox::Ok);
            res = false;
        }
    }
    return res;
}

bool XMLTextSourceDialog::checkData() {
    return TextSourceDialog::checkData();
}
