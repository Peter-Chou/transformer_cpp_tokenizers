#pragma once

#include <unicode/unistr.h>

#include <set>
#include <string>
#include <unordered_map>
#include <vector>

template <>
struct std::hash<icu::UnicodeString> {
  size_t operator()(const icu::UnicodeString& x) const { return x.hashCode(); }
};

namespace tokenizers {
class BasicTokenizer {
 public:
  using CharIdsMap = std::unordered_map<icu::UnicodeString, std::set<int>>;
  using CharIdsMapList = std::vector<CharIdsMap>;

  struct TokenSpan {
    int start;
    int length;
  };
  void AddSpecialToken(const icu::UnicodeString& token);
  std::vector<icu::UnicodeString> SplitBySpecialToken(
      const icu::UnicodeString& text);
  TokenSpan GetSpecialTokenSpan(const icu::UnicodeString& text, int start);
  std::vector<TokenSpan> getSpecialTokenSpans(const icu::UnicodeString& text);

  CharIdsMapList& GetTokenSetMapList() { return char_ids_map_list_; }

 private:
  void addUcharToSet(const icu::UnicodeString& uchar, int list_pos,
                     int token_idx);

  // std::vector<std::string> special_tokens_;
  CharIdsMapList char_ids_map_list_;
  std::vector<icu::UnicodeString> special_tokens_;

  std::vector<std::string> model_input_names_ = {"input_ids", "token_type_ids",
                                                 "attention_mask"};
  std::string padding_side_ = "right";
  std::string truncation_side_ = "right";
};
}  // namespace tokenizers
