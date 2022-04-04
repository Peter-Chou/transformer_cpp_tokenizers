#pragma once

#include "tokenizers/basic/basic_tokenizer.h"
#include "tokenizers/basic/wordpiece_tokenizer.h"
#include "tokenizers/fundamental/fundamental_tokenizer.h"
#include "tokenizers/utils/tokenizer_utils.h"

#include <unicode/unistr.h>

// #include <fstream>
#include <memory>
#include <string>
#include <unordered_map>

namespace tokenizers {
class BertTokenizer : public FundamentalTokenizer {
 public:
  struct Options {
    std::string vocab_file;
    FundamentalTokenizer::Options f_options;
    bool strip_accents = false;
    bool do_lower_case = true;
    bool do_basic_tokenize = true;
    bool tokenize_chinese_chars = true;
  };

  static std::unique_ptr<BertTokenizer> CreateBertTokenizer(Options options);

  BertTokenizer(Options options);
  std::vector<icu::UnicodeString> TokenizeImpl(
      const icu::UnicodeString& text) override;

  int ConvertTokenToId(const icu::UnicodeString& token) override;
  icu::UnicodeString ConvertIdToToken(int idx) override;

  int GetTokenId(const icu::UnicodeString& token) override;

  std::vector<int> GetInputIds(
      const std::vector<icu::UnicodeString>& tokens) override;

  std::vector<int> BuildInputsWithSpecialTokens(
      const std::vector<int>* token_ids_0,
      const std::vector<int>* token_ids_1 = nullptr) override;

  std::vector<int> CreateTokenTypeIdsFromSequences(
      const std::vector<int>* token_ids_0,
      const std::vector<int>* token_ids_1 = nullptr) override;

  std::vector<int> GetSpecialTokensMask(
      const std::vector<int>* token_ids_0,
      const std::vector<int>* token_ids_1 = nullptr) override;

  std::unordered_map<icu::UnicodeString, int>* getMutableVocab() {
    return &token_id_map_;
  }

 private:
  bool do_basic_tokenize_ = true;

  int cls_token_id_;
  int sep_token_id_;
  int pad_token_id_;

  BasicTokenizer basic_tokenizer_;
  WordpieceTokenizer wordpiece_tokenizer_;

  std::unordered_map<icu::UnicodeString, int> token_id_map_;
  std::unordered_map<int, icu::UnicodeString> id_token_map_;
};

}  // namespace tokenizers
