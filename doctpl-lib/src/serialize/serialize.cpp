#include <doctpl/serialize.h>

#include "xml_helpers.h"
#include "xml_text_field_factory.h"
#include "xml_table_field_factory.h"

#include <doctpl/formatting_settings.h>

#include <functional>
#include <map>

namespace doctpl {
namespace xml {

// factory

class Factory::Impl {
public:
    Impl()
    {
        fieldFactories.emplace("text", &textFieldXMLFactory);
        fieldFactories.emplace("table", &tableFieldXMLFactory);
    }

    std::map<QString, FieldFactory> fieldFactories;
};

Factory::Factory()
    : impl_(new Impl)
{}

Factory::~Factory() {}

void Factory::add(const QString& type, const FieldFactory& factory)
{
    auto res = impl_->fieldFactories.emplace(type, factory);
    REQUIRE(
        res.second,
        "Factory for field type " << type.toStdString() << " already set");
}

const FieldFactory& Factory::fieldFactory(const QString& type) const
{
    auto it = impl_->fieldFactories.find(type);
    REQUIRE(
        it != impl_->fieldFactories.end(),
        "Factory for field type " << type.toStdString() << " not set");
    return it->second;
}

FactoryPtr factory()
{
    FactoryPtr s_factory = std::make_shared<Factory>();

    return s_factory;
}


// reader impl

namespace {

void parseField(Page* p, const QDomElement& fieldElement)
{
    auto factory = xml::factory()->fieldFactory(
        getAttr<QString>(fieldElement, "type"));
    factory(fieldElement, p);
}

void parsePage(Layout* layout, const QDomElement& pageElement)
{
    Page* p = layout->append(
        Page::InitData {
            {
                getAttr<double>(pageElement, "width"),
                getAttr<double>(pageElement, "height")
            },
            defaultStyle(),
            Page::Offset {
                getAttr<double>(pageElement, "dx"),
                getAttr<double>(pageElement, "dy")
            }
        });

    QDomNodeList fields = pageElement.elementsByTagName("field");
    XML_REQUIRE(
        fields.size() == pageElement.childNodes().size(),
        pageElement.lineNumber(),
        pageElement.columnNumber(),
        "Only child elements with tag \"field\" allowed in page");
    for (int i = 0; i < fields.size(); ++i) {
        parseField(p, fields.item(i).toElement());
    }
}

void parseLayout(Template* doc, const QDomElement& layoutElement)
{
    QDomNodeList pages = layoutElement.elementsByTagName("page");
    XML_REQUIRE(
        pages.size() == layoutElement.childNodes().size(),
        layoutElement.lineNumber(),
        layoutElement.columnNumber(),
        "Only child elements with tag \"page\" allowed in layout");
    for (int i = 0; i < pages.size(); ++i) {
        parsePage(doc->layout(), pages.item(i).toElement());
    }
}

std::unique_ptr<Template> readImpl(const QString& filename)
{
    QDomDocument xmldoc(filename);
    QFile file(filename);
    REQUIRE(
        file.open(QIODevice::ReadOnly),
        "Cannot open file: " << filename.toStdString());
    QString DOCTPL_ERRORMessage;
    REQUIRE(
        xmldoc.setContent(&file, &DOCTPL_ERRORMessage),
        "Cannot read file: " << DOCTPL_ERRORMessage.toStdString());

    QDomElement rootElement = xmldoc.documentElement();

    std::unique_ptr<Template> doc(new Template(filename));

    QDomElement settingsElement = getSingleChild(rootElement, "settings");
    QDomElement layoutElement = getSingleChild(rootElement, "layout");

    doc->setDefaultFormatting(
        parseFormatting(getSingleChild(settingsElement, "formatting")));

    parseLayout(doc.get(), layoutElement);

    return doc;
}

} // namespace

std::unique_ptr<Template> read(const QString& filename)
{
    return readImpl(filename);
}

} // namespace xml
} // namespace doctpl
