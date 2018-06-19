#pragma once

#include <doctpl/layout.h>

#include <QDialog>

#include <boost/optional.hpp>

#include <functional>

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

    typedef std::function<void(doctpl::Layout::Index)> PageIndexSetter;

    /// open in Create mode
    PageDialog(
        doctpl::Layout& layout,
        PageIndexSetter pageIdxSetter,
        QWidget* parent);

    /// open in Edit mode
    PageDialog(
        doctpl::Layout& layout,
        size_t pageIdx,
        PageIndexSetter pageIdxSetter,
        QWidget* parent);

    virtual ~PageDialog() = default;

protected slots:
    virtual void accept();

private:
    void initControls();
    void initLayout();

    QWidget* findInvalidDataWidget() const;

    void createPage();
    void editPage();

    doctpl::Layout& layout_;
    boost::optional<size_t> pageIdx_;
    PageIndexSetter pageIndexSetter_;
    Mode mode_;

    QSpinBox* pageIndex_;
    util::DoubleLineEdit* width_;
    util::DoubleLineEdit* height_;
    util::DoubleLineEdit* dx_;
    util::DoubleLineEdit* dy_;

    QDialogButtonBox* buttons_;
};
