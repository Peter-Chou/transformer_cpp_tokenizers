#pragma once

#include "unicode/uniset.h"

#include <unicode/unistr.h>

#include <unordered_map>
#include <vector>

namespace tokenizers {
class WordpieceTokenizer {
 public:
  WordpieceTokenizer(
      std::unordered_map<icu::UnicodeString, int>* vocab_ptr = nullptr)
      : vocab_ptr_(vocab_ptr) {}

  /// Tokenizes a piece of text into its word pieces. This uses a greedy
  /// longest-match-first algorithm to perform tokenization using the given
  /// vocabulary.
  /// example, `input = "unaffable"` wil return as output `["un", "##aff",
  /// "##able"]`.
  std::vector<icu::UnicodeString> Tokenize(icu::UnicodeString text);

 private:
  icu::UnicodeString unk_token_ = "[UNK]";
  int max_input_chars_per_word_ = 100;
  std::unordered_map<icu::UnicodeString, int>* vocab_ptr_;
};
}  // namespace tokenizers
