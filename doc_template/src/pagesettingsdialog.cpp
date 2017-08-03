#include "pagesettingsdialog.h"

#include "settings.h"

#include <QComboBox>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QIntValidator>
#include <QMessageBox>

PageDialog::PageDialog(QWidget *parent, Mode mode)
    : QDialog(parent)
{
    pageNumber_ = new QComboBox(this);
    // width   
    width_ = new QLineEdit(this);
    //width_->setFocus();
    QIntValidator *wval = new QIntValidator(
        Settings::get("minPageWidth").toInt(),
        Settings::get("maxPageWidth").toInt(), this);
    width_->setValidator(wval);
    // height
    height_ = new QLineEdit(this);
    QIntValidator *hval = new QIntValidator(
        Settings::get("minPageHeight").toInt(),
        Settings::get("maxPageHeight").toInt(), this);
    height_->setValidator(hval);
    // dx, dy
    dx_ = new QLineEdit(this);
    dy_ = new QLineEdit(this);
    QIntValidator *deltaval = new QIntValidator(0,
        Settings::get("maxPageOffset").toInt(), this);
    dx_->setValidator(deltaval);
    dy_->setValidator(deltaval);
    // buttons
    buttons_ = new QDialogButtonBox(QDialogButtonBox::Ok |
        QDialogButtonBox::Cancel, Qt::Horizontal, this);
    connect(buttons_, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttons_, SIGNAL(rejected()), this, SLOT(reject()));
    if (mode == Create)
        setWindowTitle(tr("DLG_PAGE_TITLE_CREATE"));
    else // Edit
        setWindowTitle(tr("DLG_PAGE_TITLE_EDIT"));
    generateLayout();
}

void PageDialog::generateLayout() {

    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);

    QGridLayout *gl = new QGridLayout;

    gl->addWidget(new QLabel(tr("PAGE_NUMBER"), this), 0, 0);
    pageNumber_->setFixedWidth(70);
    gl->addWidget(pageNumber_, 0, 1, 1, 2);

    gl->addWidget(new QLabel(tr("PAGE_WIDTH"), this), 1, 0);
    width_->setFixedWidth(70);
    gl->addWidget(width_, 1, 1);
    gl->addWidget(new QLabel(tr("UNIT_MM"), this), 1, 2);
    gl->addWidget(new QLabel(tr("PAGE_HEIGHT"), this), 2, 0);
    height_->setFixedWidth(70);
    gl->addWidget(height_, 2, 1);
    gl->addWidget(new QLabel(tr("UNIT_MM"), this), 2, 2);

    gl->addWidget(new QLabel(tr("PAGE_OFFSET_X"), this), 3, 0);
    dx_->setFixedWidth(70);
    dx_->setText("0");
    gl->addWidget(dx_, 3, 1);
    gl->addWidget(new QLabel(tr("UNIT_MM"), this), 3, 2);
    gl->addWidget(new QLabel(tr("PAGE_OFFSET_Y"), this), 4, 0);
    dy_->setFixedWidth(70);
    dy_->setText("0");
    gl->addWidget(dy_, 4, 1);
    gl->addWidget(new QLabel(tr("UNIT_MM"), this), 4, 2);
    layout->addLayout(gl);

    QHBoxLayout* hl = new QHBoxLayout;
    hl->addWidget(buttons_);
    layout->addLayout(hl);
    setFixedSize(minimumWidth(), minimumHeight());
}

void PageDialog::setPages(size_t count) {

    while (pageNumber_->count() > 0)
        pageNumber_->removeItem(0);
    QString s;
    for (size_t i = 1; i <= count; i++) {
        s.setNum(i);
        pageNumber_->addItem(s);
    }
}

size_t PageDialog::pageNum() const {

    return (size_t) pageNumber_->currentText().toInt();
}

void PageDialog::setPageNum(size_t num) {

    if (1 <= num && num <= (size_t) pageNumber_->count())
        pageNumber_->setCurrentIndex(num - 1);
}

double PageDialog::width() const {

    return (double) width_->text().toInt();
}

void PageDialog::setWidth(double width) {

    QString s;
    s.setNum((int) width);
    width_->setText(s);
}

double PageDialog::height() const {

    return (double) height_->text().toInt();
}

void PageDialog::setHeight(double height) {

    QString s;
    s.setNum((int) height);
    height_->setText(s);
}

double PageDialog::dx() const {

    return (double) dx_->text().toInt();
}

void PageDialog::setDx(double dx) {

    QString s;
    s.setNum((int) dx);
    dx_->setText(s);
}

double PageDialog::dy() const {

    return (double) dy_->text().toInt();
}

void PageDialog::setDy(double dy) {

    QString s;
    s.setNum((int) dy);
    dy_->setText(s);
}

void PageDialog::accept() {

    bool result = true;
    if (width_->text().isEmpty()) {
        width_->setFocus();
        result = false;
    }
    if (result && height_->text().isEmpty()) {
        height_->setFocus();
        result = false;
    }
    if (result && dx_->text().isEmpty()) {
        dx_->setFocus();
        result = false;
    }
    if (result && dy_->text().isEmpty()) {
        dy_->setFocus();
        result = false;
    }
    if (result)
        QDialog::accept();
    else
        QMessageBox::warning(this, tr("APP"),
            tr("DLG_PAGE_EMPTY_FIELDS_MESSAGE"), QMessageBox::Ok);
}
