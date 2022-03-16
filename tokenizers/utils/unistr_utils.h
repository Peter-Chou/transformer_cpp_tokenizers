#pragma once

#include <unicode/unistr.h>

template <>
struct std::hash<icu::UnicodeString> {
  size_t operator()(const icu::UnicodeString& x) const { return x.hashCode(); }
};
