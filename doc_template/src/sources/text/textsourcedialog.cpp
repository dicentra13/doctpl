#include "textsourcedialog.h"

#include "textsource.h"

#include <QWidget>
#include <QSpinBox>
#include <QLabel>
#include <QHBoxLayout>

TextSourceDialog::TextSourceDialog(SourceNameValidator validator,
         QWidget *parent)
    : SourceDialog(validator, parent)
{}

TextSourceDialog::TextSourceDialog(SourceNameValidator validator,
        TextSource *source, QWidget *parent)
    : SourceDialog(validator, source, parent)
{}

bool TextSourceDialog::checkNonEmptyInput() {
    return SourceDialog::checkNonEmptyInput();
}

bool TextSourceDialog::checkData() {
    return SourceDialog::checkData();
}

