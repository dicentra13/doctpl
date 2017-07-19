#include "commonfieldsettingswidget.h"

#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>

CommonFieldSettingsWidget::CommonFieldSettingsWidget(QWidget *parent)
    : QWidget(parent)
    , isSetProgrammaticaly_(false)
{
    setContentsMargins(0, 0, 0, 0);

    name_ = new QLineEdit(this);
    connect(name_, SIGNAL(returnPressed()), this, SLOT(nameChanged_()));

    pageNumber_ = new QComboBox(this);
    connect(pageNumber_, SIGNAL(currentIndexChanged(int)),
        this, SLOT(pageNumberChanged_(int)));

    // x, y
    xPos_ = new QDoubleSpinBox(this);
    xPos_->setDecimals(1);
    xPos_->setMinimum(0.0);
    xPos_->setMaximum(297.0);
    xPos_->setSingleStep(0.5);
    xPos_->setSuffix(tr("UNIT_MM"));
    yPos_ = new QDoubleSpinBox(this);
    yPos_->setDecimals(1);
    yPos_->setMinimum(0.0);
    yPos_->setMaximum(297.0);
    yPos_->setSingleStep(0.5);
    yPos_->setSuffix(tr("UNIT_MM"));

    connect(xPos_, SIGNAL(valueChanged(double)),
        this, SLOT(xPosChanged_(double)));
    connect(yPos_, SIGNAL(valueChanged(double)),
        this, SLOT(yPosChanged_(double)));
    // width info
    width_ = new QDoubleSpinBox(this);
    width_->setDecimals(1);
    width_->setMinimum(0.0);
    width_->setMaximum(297.0);
    width_->setSingleStep(0.5);
    width_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    width_->setSuffix(tr("UNIT_MM"));
    // height info
    height_ = new QDoubleSpinBox(this);
    height_->setDecimals(1);
    height_->setMinimum(0.0);
    height_->setMaximum(297.0);
    height_->setSingleStep(0.5);
    height_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    height_->setSuffix(tr("UNIT_MM"));
    connect(width_, SIGNAL(valueChanged(double)),
        this, SLOT(widthChanged_(double)));
    connect(height_, SIGNAL(valueChanged(double)),
        this, SLOT(heightChanged_(double)));

    generateLayout();
}

void CommonFieldSettingsWidget::setPages(size_t count) {
    isSetProgrammaticaly_ = true;
    pageNumber_->clear();
    QString s;
    for (size_t i = 1; i <= count; i++) {
        s.setNum(i);
        pageNumber_->addItem(s);
    }
    isSetProgrammaticaly_ = false;
}

void CommonFieldSettingsWidget::generateLayout() {

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(new QLabel(tr("FLD_NAME"), this), 0, 0);
    layout->addWidget(name_, 0, 1, 1, 2);
    layout->addWidget(new QLabel(tr("FLD_PAGE"), this), 1, 0);
    layout->addWidget(pageNumber_, 1, 1, 1, 2);
    layout->addWidget(new QLabel(tr("FLD_POSITION"), this), 2, 0, 1, 2);

    QHBoxLayout* positionLayout = new QHBoxLayout;
    positionLayout->addWidget(new QLabel(tr("FLD_X"), this));
    positionLayout->addWidget(xPos_);
    positionLayout->addItem(new QSpacerItem(5, 0));
    positionLayout->addWidget(new QLabel(tr("FLD_Y"), this));
    positionLayout->addWidget(yPos_);
    layout->addLayout(positionLayout, 3, 0, 1, 3);

    layout->addWidget(new QLabel(tr("FLD_WIDTH")), 4, 0);
    layout->addWidget(width_, 4, 1);
    layout->addWidget(new QLabel(tr("FLD_HEIGHT")), 5, 0);
    layout->addWidget(height_, 5, 1);

    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
}

QString CommonFieldSettingsWidget::name() const {

    return name_->text();
}

bool CommonFieldSettingsWidget::setName(const QString &name) {
// FIXME вот эти флаги - это неочевидное решение. Если очень надо, то передавай в качестве
// параметра в setText, но не так.
    isSetProgrammaticaly_ = true;
    name_->setText(name);
    isSetProgrammaticaly_ = false;
    return true;
}

void CommonFieldSettingsWidget::resetName() {
// FIXME а поему тут ты программно меняешь значение, а флаги не выставила?
    name_->clear();
    name_->setFocus();
}

bool CommonFieldSettingsWidget::isNameClear() const {
    return name_->text().isEmpty();
}

int CommonFieldSettingsWidget::pageNumber() const {

    return pageNumber_->currentText().toInt();
}

bool CommonFieldSettingsWidget::setPageNumber(int pageNumber) {

    isSetProgrammaticaly_ = true;
    pageNumber_->setCurrentIndex(pageNumber - 1);
    isSetProgrammaticaly_ = false;
    return true;
}

double CommonFieldSettingsWidget::x() const {

    return xPos_->value();
}

bool CommonFieldSettingsWidget::setX(double x) {

    isSetProgrammaticaly_ = true;
    xPos_->setValue(x);
    isSetProgrammaticaly_ = false;
    return true;
}

void CommonFieldSettingsWidget::resetX() {

    xPos_->clear();
    xPos_->setFocus();
}

bool CommonFieldSettingsWidget::isXClear() const {
    return xPos_->text().isEmpty();
}

double CommonFieldSettingsWidget::y() const {

    return yPos_->value();
}

bool CommonFieldSettingsWidget::setY(double y) {

    isSetProgrammaticaly_ = true;
    yPos_->setValue(y);
    isSetProgrammaticaly_ = false;
    return true;
}

void CommonFieldSettingsWidget::resetY() {

    yPos_->clear();
    yPos_->setFocus();
}

bool CommonFieldSettingsWidget::isYClear() const {
    return yPos_->text().isEmpty();
}

double CommonFieldSettingsWidget::width() const {

    return width_->value();
}

bool CommonFieldSettingsWidget::setWidth(double width) {
    isSetProgrammaticaly_ = true;
    width_->setValue(width);
    isSetProgrammaticaly_ = false;
    return true;
}

void CommonFieldSettingsWidget::resetWidth() {

    width_->clear();
    width_->setFocus();
}

bool CommonFieldSettingsWidget::isWidthClear() const {
    return width_->text().isEmpty();
}

double CommonFieldSettingsWidget::height() const {

    return height_->value();
}

bool CommonFieldSettingsWidget::setHeight(double height) {
    isSetProgrammaticaly_ = true;
    height_->setValue(height);
    isSetProgrammaticaly_ = false;
    return true;
}

void CommonFieldSettingsWidget::resetHeight() {

    height_->clear();
    height_->setFocus();
}

bool CommonFieldSettingsWidget::isHeightClear() const {
    return height_->text().isEmpty();
}

void CommonFieldSettingsWidget::setEnabled(bool isEnabled) {

    name_->setEnabled(isEnabled);
    pageNumber_->setEnabled(isEnabled);
    xPos_->setEnabled(isEnabled);
    yPos_->setEnabled(isEnabled);
    width_->setEnabled(isEnabled);
    height_->setEnabled(isEnabled);
}

void CommonFieldSettingsWidget::nameChanged_() {

    if (!isSetProgrammaticaly_)
        emit nameChanged(name_->text());
}

// newNum - индекс в виджете
void CommonFieldSettingsWidget::pageNumberChanged_(int newNum) {
    if (!isSetProgrammaticaly_)
        emit pageNumberChanged((size_t) (newNum + 1));
}

void CommonFieldSettingsWidget::xPosChanged_(double x) {
    if (!isSetProgrammaticaly_)
        emit xPosChanged(x);
}

void CommonFieldSettingsWidget::yPosChanged_(double y) {
    if (!isSetProgrammaticaly_)
        emit yPosChanged(y);
}

void CommonFieldSettingsWidget::widthChanged_(double width) {
    if (!isSetProgrammaticaly_)
        emit widthChanged(width);
}

void CommonFieldSettingsWidget::heightChanged_(double height) {
    if (!isSetProgrammaticaly_)
        emit heightChanged(height);
}
