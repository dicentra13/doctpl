#pragma once

#include <QDialog>

class QSpinBox;
class QDialogButtonBox;

namespace util {

class DoubleLineEdit;

} // namespace util

namespace doctpl {

class Layout;

} // namespace doctpl

class PageDialog : public QDialog {

    Q_OBJECT

public:

    enum class Mode {Create, Edit};

    /// open in Create mode
    PageDialog(doctpl::Layout& layout, QWidget* parent);

    /// open in Edit mode
    PageDialog(doctpl::Layout& layout, size_t pageIdx, QWidget* parent);

    virtual ~PageDialog() = default;

protected slots:
    virtual void accept();

private:
    void initLayout();

    QSpinBox* pageIndex_;
    util::DoubleLineEdit* width_;
    util::DoubleLineEdit* height_;
    util::DoubleLineEdit* dx_;
    util::DoubleLineEdit* dy_;

    QDialogButtonBox* buttons_;
};
