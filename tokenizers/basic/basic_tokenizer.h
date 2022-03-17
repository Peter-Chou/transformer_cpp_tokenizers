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
  using CharIdsMap = std::unordered_map<icu::UnicodeString, std::set<int>>;
  using CharIdsMapList = std::vector<CharIdsMap>;

  struct TokenSpan {
    int start;
    int length;
  };
  BasicTokenizer(bool do_lower_case = true,
                 const std::unordered_set<icu::UnicodeString>& never_split = {
                     "[UNK]", "[SEP]", "[PAD]", "[CLS]", "[MASK]"});

  std::vector<icu::UnicodeString> Tokenize(icu::UnicodeString& text);

  void AddSpecialToken(const icu::UnicodeString& token);
  std::vector<icu::UnicodeString> SplitBySpecialToken(
      const icu::UnicodeString& text);
  TokenSpan GetSpecialTokenSpan(const icu::UnicodeString& text, int start);
  std::vector<TokenSpan> getSpecialTokenSpans(const icu::UnicodeString& text);

  CharIdsMapList& GetTokenSetMapList() { return char_ids_map_list_; }
  std::vector<icu::UnicodeString> SplitByPunctuation(
      const icu::UnicodeString& text);

 private:
  void addUcharToSet(const icu::UnicodeString& uchar, int list_pos,
                     int token_idx);

  CharIdsMapList char_ids_map_list_;
  // std::vector<icu::UnicodeString> special_tokens_;
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
