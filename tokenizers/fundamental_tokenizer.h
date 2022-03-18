#pragma once

#include "unicode/unistr.h"

#include <unordered_set>
#include <vector>

namespace tokenizers {
class FundamentalTokenizer {
 public:
  virtual ~FundamentalTokenizer() = 0;

  void Tokenize(const icu::UnicodeString& text);
  virtual std::vector<icu::UnicodeString> TokenizeImpl(
      const icu::UnicodeString& text);

 protected:
  std::unordered_set<icu::UnicodeString> special_tokens_;
};
}  // namespace tokenizers
