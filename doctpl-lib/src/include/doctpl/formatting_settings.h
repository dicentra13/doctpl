#pragma once

#include <QFont>

#include <memory>

namespace doctpl {

/// in millimeters
struct Margins {
    Margins()
        : top(0.0)
        , right(0.0)
        , bottom(0.0)
        , left(0.0)
    {}

    Margins(double top, double right, double bottom, double left)
        : top(top)
        , right(right)
        , bottom(bottom)
        , left(left)
    {}

    double top;
    double right;
    double bottom;
    double left;
};


class TextItem;

class TextItemFormatter {
public:
    TextItemFormatter();

    TextItemFormatter(TextItemFormatter&&);
    TextItemFormatter& operator = (TextItemFormatter&&);

    virtual ~TextItemFormatter();

    virtual void add(TextItem* item);
    virtual void remove(TextItem* item);

    void forAll(std::function<void(TextItem*)>);

private:
    class Impl;

    std::unique_ptr<Impl> impl_;
};


class Formatting : public TextItemFormatter {
public:
    Formatting(
        const QFont& font = QFont(),
        int flags = Qt::AlignLeft | Qt::AlignBottom,
        const Margins& margins = Margins());

    Formatting(Formatting&&);
    Formatting& operator = (Formatting&&);

    ~Formatting();

    static std::shared_ptr<Formatting> defaults();

    std::shared_ptr<Formatting> copy() const;

    void setFont(const QFont& font);
    const QFont& font() const;

    void setFlags(int flags);
    int flags() const;

    void setMargins(const Margins& margins);
    const Margins& margins() const;

private:
    class Impl;

    std::unique_ptr<Impl> impl_;
};

typedef std::shared_ptr<Formatting> FormattingPtr;


class FormattingSettings : public TextItemFormatter {
public:
    enum class Mode {Local, Global};

    FormattingSettings(
        const FormattingPtr& global,
        const FormattingPtr& local,
        Mode mode);

    FormattingSettings(FormattingSettings&&);
    FormattingSettings& operator = (FormattingSettings&&);

    ~FormattingSettings();

    const FormattingPtr& current() const;
    const FormattingPtr& global() const;
    const FormattingPtr& local() const;

    void setGlobal(const FormattingPtr& global);
    void setLocal(const FormattingPtr& local);

    Mode mode() const;
    void setMode(Mode mode);

    virtual void add(TextItem* item);
    virtual void remove(TextItem* item);

private:
    class Impl;

    std::unique_ptr<Impl> impl_;
};

typedef std::shared_ptr<FormattingSettings> FormattingSettingsPtr;

} // namespace doctpl
