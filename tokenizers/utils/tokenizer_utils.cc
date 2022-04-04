#include "tokenizers/utils/tokenizer_utils.h"

#include "tokenizers/lib/unilib/unicode.h"
#include "tokenizers/lib/unilib/uninorms.h"
#include "tokenizers/utils/unistr_utils.h"

#include <unicode/regex.h>
#include <unicode/umachine.h>

#include <cstdint>
#include <fstream>
#include <unordered_map>

namespace unilib = ufal::unilib;

static std::unordered_map<unilib::unicode::category_t, const char*> categories =
    {
        {unilib::unicode::Lu, "Lu"}, {unilib::unicode::Ll, "Ll"},
        {unilib::unicode::Lt, "Lt"}, {unilib::unicode::Lm, "Lm"},
        {unilib::unicode::Lo, "Lo"}, {unilib::unicode::Mn, "Mn"},
        {unilib::unicode::Mc, "Mc"}, {unilib::unicode::Me, "Me"},
        {unilib::unicode::Nd, "Nd"}, {unilib::unicode::Nl, "Nl"},
        {unilib::unicode::No, "No"}, {unilib::unicode::Pc, "Pc"},
        {unilib::unicode::Pd, "Pd"}, {unilib::unicode::Ps, "Ps"},
        {unilib::unicode::Pe, "Pe"}, {unilib::unicode::Pi, "Pi"},
        {unilib::unicode::Pf, "Pf"}, {unilib::unicode::Po, "Po"},
        {unilib::unicode::Sm, "Sm"}, {unilib::unicode::Sc, "Sc"},
        {unilib::unicode::Sk, "Sk"}, {unilib::unicode::So, "So"},
        {unilib::unicode::Zs, "Zs"}, {unilib::unicode::Zl, "Zl"},
        {unilib::unicode::Zp, "Zp"}, {unilib::unicode::Cc, "Cc"},
        {unilib::unicode::Cf, "Cf"}, {unilib::unicode::Cs, "Cs"},
        {unilib::unicode::Co, "Co"}, {unilib::unicode::Cn, "Cn"},
};

namespace tokenizers {

bool LoadVocab(const std::string& vocab_file,
               std::unordered_map<icu::UnicodeString, int>* token_id_map) {
  std::ifstream fin(vocab_file);
  int idx = 0;
  if (fin.is_open()) {
    std::string token;
    while (std::getline(fin, token)) {
      (*token_id_map)[RTrim(icu::UnicodeString::fromUTF8(token))] = idx;
      ++idx;
    }
    return true;
  }
  return false;
}

bool IsWhiteSpace(const UChar32& uchar) {
  if (uchar == U' ' || uchar == U'\t' || uchar == U'\n' || uchar == U'\r') {
    return true;
  }
  auto cat = unilib::unicode::category(uchar);
  if (cat == unilib::unicode::Zs) {
    return true;
  }

  return false;
}

bool IsControl(const UChar32& uchar) {
  // strcmp(sName,Student.name) == 0
  if (uchar == U'\t' || uchar == U'\n' || uchar == U'\r') {
    return false;
  }
  auto cat = unilib::unicode::category(uchar);
  const char* cat_ = categories[cat];
  if (cat_[0] == 'C') {
    return true;
  }
  return false;
}

bool IsPunctuation(const UChar32& uchar) {
  if ((uchar >= 33 and uchar <= 47) or (uchar >= 58 and uchar <= 64) or
      (uchar >= 91 and uchar <= 96) or (uchar >= 123 and uchar <= 126)) {
    return true;
  }
  auto cat = unilib::unicode::category(uchar);
  const char* cat_ = categories[cat];
  if (cat_[0] == 'P') {
    return true;
  }

  return false;
}

icu::UnicodeString LTrim(const icu::UnicodeString& text) {
  UErrorCode status = U_ZERO_ERROR;
  icu::RegexMatcher m(icu::UnicodeString("^\\s+"), 0, status);
  m.reset(text);
  icu::UnicodeString replacement("");

  return m.replaceAll(replacement, status);
}

icu::UnicodeString RTrim(const icu::UnicodeString& text) {
  UErrorCode status = U_ZERO_ERROR;
  icu::RegexMatcher m(icu::UnicodeString("\\s+$"), 0, status);
  m.reset(text);
  icu::UnicodeString replacement("");

  return m.replaceAll(replacement, status);
}

icu::UnicodeString Strip(const icu::UnicodeString& text) {
  return LTrim(RTrim(text));
}

bool IsChineseChar(const UChar32& uchar) {
  if ((uchar >= U'\U00004E00' && uchar <= U'\U00009FFF') ||
      (uchar >= U'\U00003400' && uchar <= U'\U00004DBF') ||
      (uchar >= U'\U00020000' && uchar <= U'\U0002A6DF') ||
      (uchar >= U'\U0002A700' && uchar <= U'\U0002B73F') ||
      (uchar >= U'\U0002B740' && uchar <= U'\U0002B81F') ||
      (uchar >= U'\U0002B820' && uchar <= U'\U0002CEAF') ||
      (uchar >= U'\U0000F900' && uchar <= U'\U0000FAFF') ||
      (uchar >= U'\U0002F800' && uchar <= U'\U0002FA1F')) {
    return true;
  }
  return false;
}

icu::UnicodeString CleanText(const icu::UnicodeString& text) {
  icu::UnicodeString output;

  auto length = text.length();
  UErrorCode status = U_ZERO_ERROR;
  UChar32 uchars[length];
  text.toUTF32(uchars, length, status);

  for (auto& uchar : uchars) {
    if (uchar == U'\U00000000' || uchar == U'\U0000FFFD' || IsControl(uchar)) {
      continue;
    }
    if (IsWhiteSpace(uchar)) {
      output += static_cast<UChar32>(' ');
    } else {
      output += uchar;
    }
  }
  return output;
}

std::vector<icu::UnicodeString> WhitespaceTokenize(
    const icu::UnicodeString& text) {
  if (text.length() == 1) {
    return {text};
  }

  std::vector<icu::UnicodeString> outputs;
  icu::UnicodeString output;
  auto length = text.length();
  UChar32 delimiter(U' ');
  int32_t start = 0;
  auto idx = text.indexOf(delimiter, start);
  while (idx != -1) {
    text.extract(start, idx - start, output);
    outputs.push_back(output);
    output.remove();
    start = idx + 1;
    idx = text.indexOf(delimiter, start);
  }
  if (start < length) {
    text.extract(start, length - start, output);
    outputs.push_back(output);
  }
  return outputs;
}

icu::UnicodeString TokenizeChineseChars(const icu::UnicodeString& text) {
  icu::UnicodeString output;

  auto length = text.length();
  UErrorCode status = U_ZERO_ERROR;
  UChar32 uchars[length];
  text.toUTF32(uchars, length, status);

  for (auto& uchar : uchars) {
    if (IsChineseChar(uchar)) {
      output += static_cast<UChar32>(' ');
      output += uchar;
      output += static_cast<UChar32>(' ');
    } else {
      output += uchar;
    }
  }

  return output;
}

icu::UnicodeString StripAccents(const icu::UnicodeString& text) {
  icu::UnicodeString result;

  auto length = text.length();
  UErrorCode status = U_ZERO_ERROR;
  UChar32 temp[length];
  text.toUTF32(temp, length, status);
  std::u32string std_text =
      std::u32string(reinterpret_cast<const char32_t*>(temp), length);
  unilib::uninorms::nfd(std_text);
  for (auto& u32_char : std_text) {
    auto cat = unilib::unicode::category(u32_char);
    if (cat != unilib::unicode::Mn) {
      result += static_cast<UChar32>(u32_char);
    }
  }
  return result;
}

std::vector<icu::UnicodeString> SplitByPunctuation(
    const icu::UnicodeString& text,
    std::unordered_set<icu::UnicodeString>* special_tokens) {
  if (special_tokens) {
    if (auto it = special_tokens->find(text); it != special_tokens->end()) {
      return {text};
    }
  }
  std::vector<icu::UnicodeString> outputs;
  icu::UnicodeString temp_str;
  std::vector<std::vector<UChar32>> temp_chars;
  bool start_new_word = true;

  auto length = text.length();
  UErrorCode status = U_ZERO_ERROR;
  UChar32 uchars[length];
  text.toUTF32(uchars, length, status);
  for (auto& uchar : uchars) {
    if (IsPunctuation(uchar)) {
      temp_chars.push_back(std::vector<UChar32>{uchar});
      start_new_word = true;
    } else {
      if (start_new_word) {
        temp_chars.push_back(std::vector<UChar32>{});
      }
      start_new_word = false;
      temp_chars.back().push_back(uchar);
    }
  }
  for (const auto& uchar_list : temp_chars) {
    temp_str.remove();
    for (const auto& uchar : uchar_list) {
      temp_str += uchar;
    }
    outputs.push_back(temp_str);
  }

  return outputs;
}

}  // namespace tokenizers
