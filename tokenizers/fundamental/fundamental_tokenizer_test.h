#pragma once

#include "tokenizers/fundamental/fundamental_tokenizer.h"

namespace tokenizers {

class DummyTokenizer : public FundamentalTokenizer {
 public:
  DummyTokenizer(FundamentalTokenizer::Options options)
      : FundamentalTokenizer(options) {}

  std::vector<icu::UnicodeString> TokenizeImpl(
      const icu::UnicodeString& text) override {
    return {};
  }
  int GetTokenId(const icu::UnicodeString& token) override { return 0; }
  std::vector<int> GetInputIds(
      const std::vector<icu::UnicodeString>& tokens) override {
    return {};
  }
  int ConvertTokenToId(const icu::UnicodeString& token) override { return 0; }
  icu::UnicodeString ConvertIdToToken(int idx) override { return {}; }
  std::vector<int> GetSpecialTokensMask(
      const std::vector<int>* token_ids_0,
      const std::vector<int>* token_ids_1 = nullptr) override {
    return {};
  }
};
}  // namespace tokenizers
