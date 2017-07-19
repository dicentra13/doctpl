#include "fieldsettingswidget.h"

#include "commonfieldsettingswidget.h"

FieldSettingsWidget::FieldSettingsWidget(QWidget *parent)
    : QWidget(parent)
{
    widget_ = new CommonFieldSettingsWidget(this);
    connect(widget_, SIGNAL(nameChanged(QString)),
        this, SLOT(nameChanged_(QString)));
    connect(widget_, SIGNAL(pageNumberChanged(int)),
        this, SLOT(pageChanged_(int)));
    connect(widget_, SIGNAL(xPosChanged(double)),
        this, SLOT(xPosChanged_(double)));
    connect(widget_, SIGNAL(yPosChanged(double)),
        this, SLOT(yPosChanged_(double)));
    connect(widget_, SIGNAL(widthChanged(double)),
        this, SLOT(widthChanged_(double)));
    connect(widget_, SIGNAL(heightChanged(double)),
        this, SLOT(heightChanged_(double)));
}

FieldSettingsWidget::~FieldSettingsWidget() {
    // дочерние объекты удалятся автоматически
}

QWidget *FieldSettingsWidget::commonSettingsWidget() {
    return widget_;
}

void FieldSettingsWidget::setEnabled(bool isEnabled) {
    widget_->setEnabled(isEnabled);
}

bool FieldSettingsWidget::setName(const QString &name) {
    return widget_->setName(name);
}

void FieldSettingsWidget::resetName() {
    widget_->resetName();
}

QString FieldSettingsWidget::name() const {
    return widget_->name();
}

bool FieldSettingsWidget::isNameClear() const {
    return widget_->isNameClear();
}

void FieldSettingsWidget::setPages(size_t count) {
    widget_->setPages(count);
}

bool FieldSettingsWidget::setPageNumber(int pageNumber) {
    return widget_->setPageNumber(pageNumber);
}

int FieldSettingsWidget::pageNumber() const {
    return widget_->pageNumber();
}

bool FieldSettingsWidget::setX(double x) {
    return widget_->setX(x);
}

void FieldSettingsWidget::resetX() {
    widget_->resetX();
}

double FieldSettingsWidget::x() const {
    return widget_->x();
}

bool FieldSettingsWidget::isXClear() const {
    return widget_->isXClear();
}

bool FieldSettingsWidget::setY(double y) {
    return widget_->setY(y);
}

void FieldSettingsWidget::resetY() {
    widget_->resetY();
}

double FieldSettingsWidget::y() const {
    return widget_->y();
}

bool FieldSettingsWidget::isYClear() const {
    return widget_->isYClear();
}

bool FieldSettingsWidget::setWidth(double width) {
    return widget_->setWidth(width);
}

void FieldSettingsWidget::resetWidth() {
    widget_->resetWidth();
}

double FieldSettingsWidget::width() const {
    return widget_->width();
}

bool FieldSettingsWidget::isWidthClear() const {
    return widget_->isWidthClear();
}

bool FieldSettingsWidget::setHeight(double height) {
    return widget_->setHeight(height);
}

void FieldSettingsWidget::resetHeight() {
    widget_->resetHeight();
}

double FieldSettingsWidget::height() const {
    return widget_->height();
}

bool FieldSettingsWidget::isHeightClear() const {
    return widget_->isHeightClear();
}

void FieldSettingsWidget::nameChanged_(QString name) {
    emit nameChanged(name);
}

void FieldSettingsWidget::pageChanged_(int number) {
    emit pageChanged((size_t) number);
}

void FieldSettingsWidget::xPosChanged_(double x) {
    emit xPosChanged(x);
}

void FieldSettingsWidget::yPosChanged_(double y) {
    emit yPosChanged(y);
}

void FieldSettingsWidget::widthChanged_(double w) {
    emit widthChanged(w);
}

void FieldSettingsWidget::heightChanged_(double h) {
    emit heightChanged(h);
}
