#include "sourcexmlconvertor.h"

#include "../template/sourcenamevalidator.h"
#include "source.h"

SourceXMLConvertor::SourceXMLConvertor(SourceNameValidator validator)
    : validator_(validator)
{}

SourceXMLConvertor::SourceXMLConvertor(SourceNameValidator validator,
        Source *source)
    : validator_(validator)
    , name_(source->name())
{}
