#include "formatfieldsettingswidget.h"
#include "textrenderinfo.h"
#include "../settings.h"

#include <QWidget>
#include <QFontComboBox>
#include <QComboBox>
#include <QList>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QRadioButton>
#include <QButtonGroup>
#include <QPushButton>
#include <QDoubleSpinBox>


FormatFieldSettingsWidget::FormatFieldSettingsWidget(QWidget *parent,
        TextRenderInfo *data)
    : QWidget(parent)
    , isSignalsBlocked_(false)
{
    fontFamilies_ = new QFontComboBox(this);
    fontFamilies_->setWritingSystem(Settings::script());

    mainLayout_ = new QVBoxLayout(this);

    initFontBlock();
    initAlignBlock();
    initIdentBlock();
    setEnabled(false);

    setData(data);

    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
}

void FormatFieldSettingsWidget::initFontBlock() {

    QFont f;
    QString family = f.family();
    QString style = Settings::fontDB()->styleString(f);
    QString size;
    size.setNum(f.pixelSize());

    const QStringList &styles = Settings::fontDB()->styles(family);
    fontStyles_ = new QComboBox(this);
    fontStyles_->addItems(styles);

    const QList<int> &sizes = Settings::fontDB()->pointSizes(family, style);
    fontSizes_ = new QComboBox(this);
    QList<int>::const_iterator i;
    QString temp;
    for (i = sizes.begin(); i != sizes.end(); i++) {
        temp.setNum(*i);
        fontSizes_->addItem(temp);
    }

    connect(fontFamilies_, SIGNAL(currentFontChanged(const QFont &)),
        this, SLOT(fontFamilyChanged(const QFont &)));
    connect(fontStyles_, SIGNAL(activated(const QString &)),
        this, SLOT(fontStyleChanged(const QString &)));
    connect(fontSizes_, SIGNAL(activated(const QString &)),
        this, SLOT(fontSizeChanged(const QString &)));

    QGridLayout *fontLayout = new QGridLayout;
    fontLayout->addWidget(new QLabel(tr("FLD_FMT_FONT"), this), 0, 0);
    fontLayout->addWidget(fontFamilies_, 0, 1, 1, 3);
    fontLayout->addWidget(new QLabel(tr("FLD_FMT_STYLE"), this), 1, 0);
    fontLayout->addWidget(fontStyles_, 1, 1);
    fontLayout->addWidget(new QLabel(tr("FLD_FMT_SIZE"), this), 1, 2);
    fontLayout->addWidget(fontSizes_, 1, 3);
    mainLayout_->addLayout(fontLayout);
}

void FormatFieldSettingsWidget::initAlignBlock() {

    QHBoxLayout *l = new QHBoxLayout;

    l->addStretch();

    halign_ = new QButtonGroup(this);
    QPushButton *hl = new QPushButton(this);
    hl->setIcon(QIcon(":/icons/images/format-justify-left.png"));
    hl->setIconSize(QSize(32, 32));
    hl->setCheckable(true);
    hl->setStatusTip(tr("FLD_FMT_HALIGN_L"));
    halign_->addButton(hl, Qt::AlignLeft);
    l->addWidget(hl);

    QPushButton *hc = new QPushButton(this);
    hc->setIcon(QIcon(":/icons/images/format-justify-center.png"));
    hc->setIconSize(QSize(32, 32));
    hc->setCheckable(true);
    hc->setStatusTip(tr("FLD_FMT_HALIGN_C"));
    halign_->addButton(hc, Qt::AlignHCenter);
    l->addWidget(hc);

    QPushButton *hr = new QPushButton(this);
    hr->setIcon(QIcon(":/icons/images/format-justify-right.png"));
    hr->setIconSize(QSize(32, 32));
    hr->setCheckable(true);
    hr->setStatusTip(tr("FLD_FMT_HALIGN_R"));
    halign_->addButton(hr, Qt::AlignRight);
    l->addWidget(hr);
    halign_->setExclusive(true);
    mainLayout_->addLayout(l);

    l->addStretch();

    l = new QHBoxLayout;
    l->addStretch();

    valign_ = new QButtonGroup(this);

    QPushButton *vt = new QPushButton(this);
    vt->setIcon(QIcon(":/icons/images/format-justify-vertical-top.png"));
    vt->setIconSize(QSize(32, 32));
    vt->setCheckable(true);
    vt->setStatusTip(tr("FLD_FMT_VALIGN_T"));
    valign_->addButton(vt, Qt::AlignTop);
    l->addWidget(vt);

    QPushButton *vc = new QPushButton(this);
    vc->setIcon(QIcon(":/icons/images/format-justify-vertical-center.png"));
    vc->setIconSize(QSize(32, 32));
    vc->setCheckable(true);
    vc->setStatusTip(tr("FLD_FMT_VALIGN_C"));
    valign_->addButton(vc, Qt::AlignVCenter);
    l->addWidget(vc);

    QPushButton *vb = new QPushButton(this);
    vb->setIcon(QIcon(":/icons/images/format-justify-vertical-bottom.png"));
    vb->setIconSize(QSize(32, 32));
    vb->setCheckable(true);
    vb->setStatusTip(tr("FLD_FMT_VALIGN_B"));
    valign_->addButton(vb, Qt::AlignBottom);
    l->addWidget(vb);
    l->addStretch();

    valign_->setExclusive(true);
    mainLayout_->addLayout(l);

    connect(halign_, SIGNAL(buttonClicked(int)),
        this, SLOT(alignChanged(int)));
    connect(valign_, SIGNAL(buttonClicked(int)),
        this, SLOT(alignChanged(int)));
}

void FormatFieldSettingsWidget::initIdentBlock() {

    mainLayout_->addWidget(new QLabel(tr("FLD_FMT_IDENT"), this));

    const static int FIELD_WIDTH = 90;

    QHBoxLayout *rowLayout = new QHBoxLayout;
    rowLayout->addStretch();
    topMargin_ = new QDoubleSpinBox(this);
    topMargin_->setDecimals(1);
    topMargin_->setMinimum(0.0);
    topMargin_->setSingleStep(0.5);
    topMargin_->setSuffix(tr("UNIT_MM"));
    topMargin_->setFixedWidth(FIELD_WIDTH);
    rowLayout->addWidget(topMargin_);
    rowLayout->addStretch();
    mainLayout_->addLayout(rowLayout);

    rowLayout = new QHBoxLayout;
    rowLayout->addStretch();
    leftMargin_ = new QDoubleSpinBox(this);
    leftMargin_->setDecimals(1);
    leftMargin_->setMinimum(0.0);
    leftMargin_->setSingleStep(0.5);
    leftMargin_->setSuffix(tr("UNIT_MM"));
    leftMargin_->setFixedWidth(FIELD_WIDTH);
    rowLayout->addWidget(leftMargin_);
    rightMargin_ = new QDoubleSpinBox(this);
    rightMargin_->setDecimals(1);
    rightMargin_->setMinimum(0.0);
    rightMargin_->setSingleStep(0.5);
    rightMargin_->setSuffix(tr("UNIT_MM"));
    rowLayout->addWidget(rightMargin_);
    rowLayout->addStretch();
    mainLayout_->addLayout(rowLayout);

    rowLayout = new QHBoxLayout;
    rowLayout->addStretch();
    bottomMargin_ = new QDoubleSpinBox(this);
    bottomMargin_->setDecimals(1);
    bottomMargin_->setMinimum(0.0);
    bottomMargin_->setSingleStep(0.5);
    bottomMargin_->setSuffix(tr("UNIT_MM"));
    rowLayout->addWidget(bottomMargin_);
    rowLayout->addStretch();
    mainLayout_->addLayout(rowLayout);

    connect(leftMargin_, SIGNAL(valueChanged(double)),
        this, SLOT(identChanged(double)));
    connect(rightMargin_, SIGNAL(valueChanged(double)),
        this, SLOT(identChanged(double)));
    connect(bottomMargin_, SIGNAL(valueChanged(double)),
        this, SLOT(identChanged(double)));
    connect(topMargin_, SIGNAL(valueChanged(double)),
        this, SLOT(identChanged(double)));
}

void FormatFieldSettingsWidget::setData(TextRenderInfo *data) {

    if (data) {
        if (!data_)
            setEnabled(true);
        isSignalsBlocked_ = true;
        const QFont &f = data->font();
        QString family = f.family();
        QString style = Settings::fontDB()->styleString(f);
        QString size;
        size.setNum(f.pixelSize());
        fontFamilies_->setCurrentIndex(fontFamilies_->findText(family));
        fontStyles_->setCurrentIndex(fontStyles_->findText(style));
        fontSizes_->setCurrentIndex(fontSizes_->findText(size));
        setAlignButtonsValues(data->flags());
        Margins margins = data->margins();
        leftMargin_->setValue(margins.left);
        rightMargin_->setValue(margins.right);
        topMargin_->setValue(margins.top);
        bottomMargin_->setValue(margins.bottom);
        isSignalsBlocked_ = false;
    } else
        setEnabled(false);
    data_ = data;
}

void FormatFieldSettingsWidget::fontFamilyChanged(const QFont &curFont) {

    if (!isSignalsBlocked_ && data_) {
        QFont f = Settings::fontDB()->font(curFont.family(),
            fontStyles_->currentText(), fontSizes_->currentText().toInt());
        f.setPixelSize(fontSizes_->currentText().toInt());
        //f.setPointSize(int(14 * 25.4 / 72.0));
        data_->setFont(f);
    }
}

void FormatFieldSettingsWidget::fontStyleChanged(const QString &curStyle) {

    if (!isSignalsBlocked_ && data_) {
        QFont f = Settings::fontDB()->font(fontFamilies_->currentText(),
            curStyle, fontSizes_->currentText().toInt());
        f.setPixelSize(fontSizes_->currentText().toInt());
        //f.setPointSize(int(14 * 25.4 / 72.0));
        data_->setFont(f);
    }
}

void FormatFieldSettingsWidget::fontSizeChanged(const QString &curSize) {

    if (!isSignalsBlocked_ && data_) {
        QFont f = Settings::fontDB()->font(fontFamilies_->currentText(),
            fontStyles_->currentText(), curSize.toInt());
        f.setPixelSize(curSize.toInt());
        //f.setPointSize(int(14 * 25.4 / 72.0));
        data_->setFont(f);
    }
}

void FormatFieldSettingsWidget::alignChanged(int /*id*/) {

    if (!isSignalsBlocked_ && data_) {
        data_->setFlags(halign_->checkedId() | valign_->checkedId());
    }
}

void FormatFieldSettingsWidget::identChanged(double /*m*/) {

    if (!isSignalsBlocked_ && data_) {
        data_->setMargins(Margins(topMargin_->value(), rightMargin_->value(),
            bottomMargin_->value(), leftMargin_->value()));
    }
}

void FormatFieldSettingsWidget::setEnabled(bool isEditable) {

    fontFamilies_->setEnabled(isEditable);
    fontStyles_->setEnabled(isEditable);
    fontSizes_->setEnabled(isEditable);
    setAlignButtonsEnabled(isEditable);
    leftMargin_->setEnabled(isEditable);
    rightMargin_->setEnabled(isEditable);
    bottomMargin_->setEnabled(isEditable);
    topMargin_->setEnabled(isEditable);
}

void FormatFieldSettingsWidget::setAlignButtonsEnabled(bool isEnabled) {

    halign_->button(Qt::AlignLeft)->setEnabled(isEnabled);
    halign_->button(Qt::AlignHCenter)->setEnabled(isEnabled);
    halign_->button(Qt::AlignRight)->setEnabled(isEnabled);

    valign_->button(Qt::AlignBottom)->setEnabled(isEnabled);
    valign_->button(Qt::AlignVCenter)->setEnabled(isEnabled);
    valign_->button(Qt::AlignTop)->setEnabled(isEnabled);
}

void FormatFieldSettingsWidget::setAlignButtonsValues(int flags) {

    halign_->button(flags & Qt::AlignHorizontal_Mask)->setChecked(true);
    valign_->button(flags & Qt::AlignVertical_Mask)->setChecked(true);
}
