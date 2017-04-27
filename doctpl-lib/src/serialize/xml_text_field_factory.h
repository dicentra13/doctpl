#pragma once

#include <doctpl/serialize.h>
#include <doctpl/text_field.h>

namespace doctpl {
namespace xml {

Field* textFieldXMLFactory(const QDomElement& fieldElement, Page* page);

} // namespace xml
} // namespace doctpl
