#pragma once

#include "unicode/unistr.h"
#include "utils/unistr_utils.h"

#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace tokenizers {
class FundamentalTokenizer {
 public:
  using CharIdsMap = std::unordered_map<icu::UnicodeString, std::set<int>>;
  using CharIdsMapList = std::vector<CharIdsMap>;

  struct TokenSpan {
    int start;
    int length;
  };

  struct Options {
    icu::UnicodeString unk_token = "[UNK]";
    icu::UnicodeString sep_token = "[SEP]";
    icu::UnicodeString pad_token = "[PAD]";
    icu::UnicodeString cls_token = "[CLS]";
    icu::UnicodeString mask_token = "[MASK]";
  };

  FundamentalTokenizer() = delete;

  FundamentalTokenizer(Options options);

  virtual ~FundamentalTokenizer() = default;

  std::vector<icu::UnicodeString> Tokenize(const icu::UnicodeString& text);
  virtual std::vector<icu::UnicodeString> TokenizeImpl(
      const icu::UnicodeString& text) = 0;

  void AddSpecialToken(const icu::UnicodeString& token);
  std::vector<icu::UnicodeString> SplitBySpecialToken(
      const icu::UnicodeString& text);
  TokenSpan GetSpecialTokenSpan(const icu::UnicodeString& text, int start);
  icu::UnicodeString ProcessSpecialTokens(const icu::UnicodeString& text);
  std::vector<TokenSpan> getSpecialTokenSpans(const icu::UnicodeString& text);
  CharIdsMapList* GetMutableTokenSetMapList() { return &char_ids_map_list_; }
  std::unordered_set<icu::UnicodeString>* GetMutableSpecialTokens() {
    return &special_tokens_;
  }

 protected:
  void addUcharToSet(const icu::UnicodeString& uchar, int list_pos,
                     int token_idx);

  CharIdsMapList char_ids_map_list_;
  std::unordered_set<icu::UnicodeString> special_tokens_;

  std::vector<std::string> model_input_names_ = {"input_ids", "token_type_ids",
                                                 "attention_mask"};
  bool do_lower_case_ = true;
  bool tokenize_chinese_chars_ = true;
  bool strip_accents_ = false;
  std::string padding_side_ = "right";
  std::string truncation_side_ = "right";
};

}  // namespace tokenizers
