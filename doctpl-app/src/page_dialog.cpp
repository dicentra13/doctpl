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

PageDialog::PageDialog(
        doctpl::Layout& layout,
        PageIndexSetter setter,
        QWidget* parent)
    : QDialog(parent)
    , layout_(layout)
    , pageIdx_(boost::none)
    , pageIndexSetter_(setter)
    , mode_(Mode::Create)
{
    setWindowTitle(tr("DLG_PAGE_TITLE_CREATE"));
    initControls();
    initLayout();
}


PageDialog::PageDialog(
        doctpl::Layout& layout,
        size_t pageIdx,
        PageIndexSetter setter,
        QWidget* parent)
    : QDialog(parent)
    , layout_(layout)
    , pageIdx_(pageIdx)
    , pageIndexSetter_(setter)
    , mode_(Mode::Edit)
{
    ASSERT(pageIdx < layout_.pagesCount());

    setWindowTitle(tr("DLG_PAGE_TITLE_EDIT"));
    initControls();
    initLayout();
}

void PageDialog::initControls()
{
    pageIndex_ = new QSpinBox(this);
    pageIndex_->setMinimum(1);
    pageIndex_->setMaximum(pageIdx_
        ? layout_.pagesCount()
        : layout_.pagesCount()+ 1);
    pageIndex_->setValue(
        pageIdx_ ? *pageIdx_ + 1 : layout_.pagesCount() + 1);

    doctpl::Page* p = pageIdx_
        ? layout_.page(*pageIdx_)
        : nullptr;

    using DefT = boost::optional<QString>;

    DefT defaultWidth = p ? QString::number(p->width()) : DefT();
    width_ = new util::DoubleLineEdit(
        settings().get<double>("minPageWidth"),
        settings().get<double>("maxPageWidth"),
        DECIMALS,
        defaultWidth,
        this);

    DefT defaultHeight = p ? QString::number(p->height()) : DefT();
    height_ = new util::DoubleLineEdit(
        settings().get<double>("minPageHeight"),
        settings().get<double>("maxPageHeight"),
        DECIMALS,
        defaultHeight,
        this);

    const double maxOffset = settings().get<double>("maxPageOffset");

    DefT defaultDx = p ? QString::number(p->dx()) : DefT();
    dx_ = new util::DoubleLineEdit(
        0.0, maxOffset, DECIMALS, defaultDx, this);
    DefT defaultDy = p ? QString::number(p->dy()) : DefT();
    dy_ = new util::DoubleLineEdit(
        0.0, maxOffset, DECIMALS, defaultDy, this);

    buttons_ = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
        Qt::Horizontal,
        this);
    connect(buttons_, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttons_, SIGNAL(rejected()), this, SLOT(reject()));

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

QWidget* PageDialog::findInvalidDataWidget() const
{
    if (width_->text().isEmpty()) {
        return width_;
    }
    if (height_->text().isEmpty()) {
        return height_;
    }
    if (dx_->text().isEmpty()) {
        return dx_;
    }
    if (dy_->text().isEmpty()) {
        return dy_;
    }
    return nullptr;
}

void PageDialog::accept() {

    if (mode_ == Mode::Create) {
        QWidget* w = findInvalidDataWidget();
        if (w) {
            w->setFocus();
            QMessageBox::warning(
                this,
                tr("APP"),
                tr("DLG_PAGE_EMPTY_FIELDS_MESSAGE"),
                QMessageBox::Ok);
        } else {
            createPage();
            QDialog::accept();
        }
    } else {
        editPage();
        QDialog::accept();
    }
}

void PageDialog::createPage()
{
    layout_.insert(
        pageIndex_->value(),
        doctpl::Page::InitData {

        })
}

void PageDialog::editPage()
{

}
