#include "page_dialog.h"

#include "edit_widgets.h"
#include "settings.h"

#include <doctpl/layout.h>

#include <QSpinBox>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDoubleValidator>
#include <QMessageBox>

namespace {

const int DECIMALS = 1;

} // namespace

PageDialog::PageDialog(doctpl::Layout& layout, QWidget* parent)
    : QDialog(parent)
{
    pageIndex_ = new QSpinBox(this);
    pageIndex_->setMinimum(1);
    pageIndex_->setMaximum(layout.pagesCount() + 1);
    pageIndex_->setValue(1);

    width_ = new util::DoubleLineEdit(
        settings().get<double>("minPageWidth"),
        settings().get<double>("maxPageWidth"),
        DECIMALS,
        boost::none,
        this);

    height_ = new util::DoubleLineEdit(
        settings().get<double>("minPageHeight"),
        settings().get<double>("maxPageHeight"),
        DECIMALS,
        boost::none,
        this);

    const double maxOffset = settings().get<double>("maxPageOffset");
    dx_ = new util::DoubleLineEdit(
        0.0, maxOffset, DECIMALS, boost::none, this);
    dy_ = new util::DoubleLineEdit(
        0.0, maxOffset, DECIMALS, boost::none, this);

    buttons_ = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal,
        this);
    connect(buttons_, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttons_, SIGNAL(rejected()), this, SLOT(reject()));

/*    if (mode_ == Mode::Create)
        setWindowTitle(tr("DLG_PAGE_TITLE_CREATE"));
    else // Edit
        setWindowTitle(tr("DLG_PAGE_TITLE_EDIT"));
*/
    initLayout();
}

void PageDialog::initLayout()
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    setLayout(layout);

    QGridLayout* gl = new QGridLayout;

    gl->addWidget(new QLabel(tr("PAGE_NUMBER"), this), 0, 0);
    pageIndex_->setFixedWidth(70);
    gl->addWidget(pageIndex_, 0, 1, 1, 2);

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

void PageDialog::accept() {
/*
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
*/}
