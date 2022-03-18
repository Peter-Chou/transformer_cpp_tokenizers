#pragma once

#include <unicode/umachine.h>
#include <unicode/unistr.h>

#include <regex>
#include <string>
#include <unordered_set>
#include <vector>

namespace tokenizers {
bool IsWhiteSpace(const UChar32& uchar);
bool IsControl(const UChar32& uchar);
bool IsPunctuation(const UChar32& uchar);
bool IsChineseChar(const UChar32& uchar);
icu::UnicodeString LTrim(const icu::UnicodeString& text);
icu::UnicodeString RTrim(const icu::UnicodeString& text);
icu::UnicodeString Strip(const icu::UnicodeString& text);
icu::UnicodeString CleanText(const icu::UnicodeString& text);
icu::UnicodeString StripAccents(const icu::UnicodeString& text);
icu::UnicodeString TokenizeChineseChars(const icu::UnicodeString& text);
std::vector<icu::UnicodeString> WhitespaceTokenize(
    const icu::UnicodeString& text);

std::vector<icu::UnicodeString> SplitByPunctuation(
    const icu::UnicodeString& text,
    std::unordered_set<icu::UnicodeString>* special_tokens = nullptr);

}  // namespace tokenizers
