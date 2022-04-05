#pragma once

#include "tokenizers/utils/unistr_utils.h"

#include <unicode/unistr.h>

#include <optional>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace tokenizers {

struct EncodeOutput {
  std::vector<int> input_ids;
  std::optional<std::vector<int>> attention_mask;
  std::optional<std::vector<int>> token_type_ids;
};

class FundamentalTokenizer {
 public:
  using CharIdsMap = std::unordered_map<icu::UnicodeString, std::set<int>>;
  using CharIdsMapList = std::vector<CharIdsMap>;

  struct TokenSpan {
    int start;
    int length;
  };

  enum class TruncateStrategy : int { ONLY_FIRST, ONLY_SECOND, LONGEST_FIRST };
  enum class TruncateSide : int { LEFT, RIGHT };
  enum class PaddingStrategy : int { DO_NOT_PAD, MAX_LENGTH, LONGEST };
  enum class PaddingSide : int { LEFT, RIGHT };

  struct Options {
    icu::UnicodeString unk_token = "[UNK]";
    icu::UnicodeString sep_token = "[SEP]";
    icu::UnicodeString pad_token = "[PAD]";
    icu::UnicodeString cls_token = "[CLS]";
    icu::UnicodeString mask_token = "[MASK]";
  };

  FundamentalTokenizer(Options options);

  virtual ~FundamentalTokenizer() = default;

  std::vector<EncodeOutput> BatchEncode(
      const std::vector<std::string>* a_texts,
      const std::vector<std::string>* b_texts = nullptr, int max_length = 512,
      bool add_special_tokens = true,
      TruncateStrategy truncate_stratety = TruncateStrategy::LONGEST_FIRST,
      PaddingStrategy padding_strategy = PaddingStrategy::MAX_LENGTH,
      bool return_token_type_ids = true, bool return_attention_mask = true);

  EncodeOutput Encode(
      const std::string* a_text, const std::string* b_text = nullptr,
      int max_length = 512, bool add_special_tokens = true,
      TruncateStrategy truncate_stratety = TruncateStrategy::LONGEST_FIRST,
      PaddingStrategy padding_strategy = PaddingStrategy::MAX_LENGTH,
      bool return_token_type_ids = true, bool return_attention_mask = true);

  std::vector<icu::UnicodeString> Tokenize(const icu::UnicodeString& text);
  virtual std::vector<icu::UnicodeString> TokenizeImpl(
      const icu::UnicodeString& text) = 0;
  virtual int GetTokenId(const icu::UnicodeString& token) = 0;
  virtual std::vector<int> GetInputIds(
      const std::vector<icu::UnicodeString>& tokens) = 0;
  virtual int ConvertTokenToId(const icu::UnicodeString& token) = 0;
  virtual icu::UnicodeString ConvertIdToToken(int idx) = 0;
  virtual std::vector<int> GetSpecialTokensMask(
      const std::vector<int>* token_ids_0,
      const std::vector<int>* token_ids_1 = nullptr) = 0;
  virtual std::vector<int> BuildInputsWithSpecialTokens(
      const std::vector<int>* token_ids_0,
      const std::vector<int>* token_ids_1 = nullptr);
  virtual std::vector<int> CreateTokenTypeIdsFromSequences(
      const std::vector<int>* token_ids_0,
      const std::vector<int>* token_ids_1 = nullptr);

  icu::UnicodeString ProcessSpecialTokens(const icu::UnicodeString& text);
  std::vector<icu::UnicodeString> SplitBySpecialToken(
      const icu::UnicodeString& text);
  std::vector<TokenSpan> getSpecialTokenSpans(const icu::UnicodeString& text);
  TokenSpan GetSpecialTokenSpan(const icu::UnicodeString& text, int start);

  CharIdsMapList* GetMutableTokenSetMapList() { return &char_ids_map_list_; }
  std::unordered_set<icu::UnicodeString>* GetMutableSpecialTokens() {
    return &special_tokens_;
  }

 protected:
  void addSpecialToken(const icu::UnicodeString& token);
  bool pad(EncodeOutput* output, int max_length = 512,
           PaddingStrategy padding_strategy = PaddingStrategy::MAX_LENGTH,
           bool return_attention_mask = true);

  void addUcharToSet(const icu::UnicodeString& uchar, int list_pos,
                     int token_idx);
  int numSpecialTokensToAdd(bool pair = false);
  bool truncateSequence(
      std::vector<int>* ids, std::vector<int>* pair_ids = nullptr,
      int num_tokens_to_move = 0,
      TruncateStrategy truncate_strategy = TruncateStrategy::LONGEST_FIRST);

  CharIdsMapList char_ids_map_list_;
  std::unordered_set<icu::UnicodeString> special_tokens_;

  std::vector<std::string> model_input_names_ = {"input_ids", "token_type_ids",
                                                 "attention_mask"};
  bool do_lower_case_ = true;
  bool tokenize_chinese_chars_ = true;
  bool strip_accents_ = false;

  icu::UnicodeString unk_token_ = "[UNK]";
  icu::UnicodeString sep_token_ = "[SEP]";
  icu::UnicodeString pad_token_ = "[PAD]";
  icu::UnicodeString cls_token_ = "[CLS]";
  icu::UnicodeString mask_token_ = "[MASK]";

  int pad_token_type_id_ = 0;

  TruncateSide truncate_side_ = TruncateSide::RIGHT;
  PaddingSide padding_side_ = PaddingSide::RIGHT;
};

}  // namespace tokenizers
