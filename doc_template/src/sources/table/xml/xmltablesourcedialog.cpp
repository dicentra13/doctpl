#include "xmltablesourcedialog.h"

#include "xmltablesource.h"

#include <QListWidget>
#include <QListWidgetItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QPushButton>

XMLTableSourceDialog::XMLTableSourceDialog(SourceNameValidator validator,
        QWidget *parent)
    : TableSourceDialog(validator, parent)
    , edited_(0)
{
    createWidgets();
    addQuery();
    removeQueryButton_->setEnabled(false);
}

XMLTableSourceDialog::XMLTableSourceDialog(SourceNameValidator validator,
        XMLTableSource *source, QWidget *parent)
    : TableSourceDialog(validator, source, parent)
    , edited_(source)
{
    createWidgets();
    fileName_->setText(source->fileName());
    QStringList queries = source->queryList();
    QStringList::const_iterator i = queries.begin();
    for (; i != queries.end(); i++) {
        QListWidgetItem *item = new QListWidgetItem(queryWidget_,
            QListWidgetItem::Type);
        item->setFlags(item->flags() | Qt::ItemIsEditable);
        item->setText(*i);
        queryWidget_->addItem(item);
    }
    removeQueryButton_->setEnabled(queries.size() > 1);
}

void XMLTableSourceDialog::createWidgets() {
    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);
    layout->addWidget(nameWidget());

    QHBoxLayout *filenameLayout = new QHBoxLayout;
    filenameLayout->addWidget(new QLabel(tr("DLG_XML_FILE"), this));
    fileName_ = new QLineEdit(this);
    filenameLayout->addWidget(fileName_);
    layout->addLayout(filenameLayout);

    addQueryButton_ = new QPushButton(tr("DLG_XML_TABLE_ADD_QUERY"), this);
    removeQueryButton_ = new QPushButton(
        tr("DLG_XML_TABLE_REMOVE_QUERY"), this);
    connect(addQueryButton_, SIGNAL(clicked()), this, SLOT(addQuery()));
    connect(removeQueryButton_, SIGNAL(clicked()), this, SLOT(removeQueries()));
    QHBoxLayout *queryLayout = new QHBoxLayout;
    queryLayout->addWidget(new QLabel(tr("DLG_XML_TABLE_QUERIES"), this));
    queryLayout->addWidget(addQueryButton_);
    queryLayout->addWidget(removeQueryButton_);
    layout->addLayout(queryLayout);

    queryWidget_ = new QListWidget(this);
    queryWidget_->setDragDropMode(QAbstractItemView::InternalMove);
    connect(queryWidget_, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
        this, SLOT(editItem(QListWidgetItem*)));
    layout->addWidget(queryWidget_);

    layout->addWidget(buttons());
}

std::unique_ptr<TableSource> XMLTableSourceDialog::createdSource() const {
    std::unique_ptr<TableSource> source;
    if (result() == QDialog::Accepted) {
        XMLTableSource *newXMLTable = new XMLTableSource(name(),
            fileName_->text());
        QStringList queries;
        for (int i = 0; i < queryWidget_->count(); i++) {
            QListWidgetItem *item = queryWidget_->item(i);
            queries.push_back(item->text());
        }
        newXMLTable->setQueryList(queries);
        source.reset(newXMLTable);
    }
    return source;
}

void XMLTableSourceDialog::accept() {
    if (checkNonEmptyInput() && checkData()) {
        if (edited_) {
            edited_->setName(name_->text());
            edited_->setFileName(fileName_->text());
            QStringList queries;
            for (int i = 0; i < queryWidget_->count(); i++) {
                QListWidgetItem *item = queryWidget_->item(i);
                queries.push_back(item->text());
            }
            edited_->setQueryList(queries);
        }
        QDialog::accept();
    }
}

Source *XMLTableSourceDialog::source() {
    return edited_;
}

bool XMLTableSourceDialog::checkNonEmptyInput() {
    bool res = TableSourceDialog::checkNonEmptyInput();
    if (res) {
        if (fileName_->text().isEmpty()) {
            fileName_->setFocus();
            QMessageBox::warning(this, tr("SOURCE_DATA_VALIDATION"),
                tr("DLG_XML_SOURCE_EMPTY_FILE_NAME"), QMessageBox::Ok);
            res = false;
        } else {
            int row = 0;
            while (res && row < queryWidget_->count()) {
                res = !queryWidget_->item(row)->text().isEmpty();
                if (res)
                    row++;
            }
            if (!res) {
                QString s;
                s.setNum(row + 1);
                QMessageBox::warning(this, tr("SOURCE_DATA_VALIDATION"),
                    tr("DLG_XML_SOURCE_EMPTY_XPASS") + " " + s,
                    QMessageBox::Ok);
                queryWidget_->setCurrentRow(row);
                queryWidget_->editItem(queryWidget_->item(row));
            }
        }
    }
    return res;
}

bool XMLTableSourceDialog::checkData() {
    return TableSourceDialog::checkData();
}

void XMLTableSourceDialog::addQuery() {
    QListWidgetItem *newItem = new QListWidgetItem;
    int row = queryWidget_->currentRow();
    if (0 <= row && row < queryWidget_->count() - 1) {
        queryWidget_->insertItem(row + 1, newItem);
    } else {
        queryWidget_->addItem(newItem);
    }
    row++;
    newItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable |
                      Qt::ItemIsDragEnabled | Qt::ItemIsSelectable);
    queryWidget_->setCurrentRow(row);
    queryWidget_->editItem(newItem);
    removeQueryButton_->setEnabled(true);
}

void XMLTableSourceDialog::removeQueries() {

}

void XMLTableSourceDialog::editItem(QListWidgetItem *activatedItem) {
    queryWidget_->editItem(activatedItem);
}
