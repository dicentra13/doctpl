#include "tablesourcexmlconvertor.h"

#include "tablesource.h"
#include "../../template/sourcenamevalidator.h"

TableSourceXMLConvertor::TableSourceXMLConvertor(SourceNameValidator validator)
    : SourceXMLConvertor(validator)
{}

TableSourceXMLConvertor::TableSourceXMLConvertor(SourceNameValidator validator,
        TableSource *source)
    : SourceXMLConvertor(validator, source)
{}
