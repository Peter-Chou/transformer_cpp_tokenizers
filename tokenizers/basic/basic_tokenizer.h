#pragma once

#include "utils/unistr_utils.h"

#include <unicode/unistr.h>

#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace tokenizers {
///  Basic Tokenization of a piece of text. Split on "white spaces" only, for
///  sub-word tokenization, see WordPieceTokenizer.
class BasicTokenizer {
 public:
  BasicTokenizer() = default;

  BasicTokenizer(bool do_lower_case,
                 bool tokenize_chinese_chars, bool strip_accents,
                 std::unordered_set<icu::UnicodeString>* special_token_ptr)
      : do_lower_case_(do_lower_case),
        tokenize_chinese_chars_(tokenize_chinese_chars),
        strip_accents_(strip_accents),
        special_tokens_ptr_(special_token_ptr) {}

  std::vector<icu::UnicodeString> Tokenize(const icu::UnicodeString& text);

private:
  std::unordered_set<icu::UnicodeString>* special_tokens_ptr_;

  bool do_lower_case_ = true;
  bool tokenize_chinese_chars_ = true;
  bool strip_accents_ = false;
};
}  // namespace tokenizers
