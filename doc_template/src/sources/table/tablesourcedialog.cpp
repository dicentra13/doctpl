#include "tablesourcedialog.h"

#include "tablesource.h"

#include <QWidget>
#include <QSpinBox>
#include <QLabel>
#include <QHBoxLayout>

TableSourceDialog::TableSourceDialog(SourceNameValidator validator,
         QWidget *parent)
    : SourceDialog(validator, parent)
{}

TableSourceDialog::TableSourceDialog(SourceNameValidator validator,
        TableSource *source, QWidget *parent)
    : SourceDialog(validator, source, parent)
{}

bool TableSourceDialog::checkNonEmptyInput() {
    return SourceDialog::checkNonEmptyInput();
}

bool TableSourceDialog::checkData() {
    return SourceDialog::checkData();
}
