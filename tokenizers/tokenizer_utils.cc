#include "tokenizer_utils.h"

#include "unicode/regex.h"
#include "unilib/unicode.h"
#include "unilib/uninorms.h"

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
bool IsWhiteSpace(const char32_t& uchar) {
  if (uchar == U' ' || uchar == U'\t' || uchar == U'\n' || uchar == U'\r') {
    return true;
  }
  auto cat = unilib::unicode::category(uchar);
  if (cat == unilib::unicode::Zs) {
    return true;
  }

  return false;
}

bool IsControl(const char32_t& uchar) {
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

bool IsPunctuation(const char32_t& uchar) {
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

}  // namespace tokenizers
