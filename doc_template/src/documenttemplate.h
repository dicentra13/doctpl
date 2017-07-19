#ifndef DOCUMENTTEMPLATE_H
#define DOCUMENTTEMPLATE_H

#include <QObject>
#include <QString>
#include <QStringList>

#include <map>
#include <vector>
#include <memory>

class TemplateEditor;
class TemplateLayout;
class TemplateView;
class Page;
class Field;
class FieldController;
class QAction;
class QSignalMapper;

class TemplateSourcesManager;

/*
    Класс, полностью отвечающий за шаблон и хранящий все объекты шаблона:
    - страницы
    - поля
    - источники
*/
class DocumentTemplate : public QObject {

    Q_OBJECT

public:
    /** Передача 0 в параметре app означает работу без оконного режима
        вид в TemplateLayout не создается, view() возвращает 0
     */
    DocumentTemplate(const QString &fileName, TemplateEditor *app);
    ~DocumentTemplate();
    bool isModified() const;
    TemplateView *view();
    TemplateLayout *layout() {
        return layout_;
    }

    void setSourcesManager(TemplateSourcesManager *sourcesManager) {
        sourcesManager_ = sourcesManager;
    }

    TemplateSourcesManager *sourcesManager() {
        return sourcesManager_;
    }

    // диалоги показываются отсюда
    bool addPage(size_t num, std::unique_ptr<Page> page);
    bool deletePage(size_t num); // поля удаляются
    bool pageExists(size_t num) const;
    // возвращает true, если изменено
    bool editPage(size_t num, size_t newNum, double width, double height,
        double dx, double dy);
    size_t firstPage() const;
    size_t lastPage() const;
    size_t pageOutOfRange() const;
    size_t numberOfPages() const;
    size_t currentPage() const;
    bool setCurrentPage(size_t num);

    void registerController(const QString &fieldType,
        FieldController *controller);
    QStringList supportedFieldTypes() const;
    FieldController *getController(const QString &fieldType);
    std::vector<QAction *> newFieldActions();

    bool addField(const QString &name, std::unique_ptr<Field> field,
        size_t pageNum, double x, double y);
    bool deleteField(const QString &name);
    bool fieldExists(const QString &name) const;
    QString currentField() const;
    bool setCurrentField(const QString &name);
    QString fieldOutOfRange() const;
    size_t numberOfFields() const;
    QStringList fieldNames() const;

    // ГЛАВНЫЙ МЕТОД
    // все данные в источниках должны быть установлены
    // этот метод ничего не меняет
    void printToFile(const QString &ID, const QString &filename,
        std::vector<size_t> pages) const;
    void setFileName(const QString &fileName) {
        fileName_ = fileName;
    }
    const QString& fileName() const {
        return fileName_;
    }
protected slots:
    /** связываются с сигналами только при создании в оконном режиме */
    void newField(const QString &type);
    void modified_();
    void currentPageChanged(size_t);
    void currentFieldChanged(QString);
    void currentFieldNameChanged(QString);
private:
    Q_DISABLE_COPY(DocumentTemplate);
    void init();

    QString fileName_; // идентифицирует шаблон
    TemplateEditor *app_;
    TemplateLayout *layout_;
    std::map<QString, FieldController *> fieldControllers_;
    QSignalMapper *newFieldActionsMapper_;

    TemplateSourcesManager *sourcesManager_;
};

#endif // DOCUMENTTEMPLATE_H
