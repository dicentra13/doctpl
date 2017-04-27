#pragma once

#include <doctpl/formatting_settings.h>

#include <string>

doctpl::FormattingPtr formattingFromString(const std::string& str);

doctpl::FormattingSettingsPtr createFormatting(const std::string& str);
