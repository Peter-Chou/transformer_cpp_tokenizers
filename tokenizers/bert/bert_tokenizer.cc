#include "tokenizers/bert/bert_tokenizer.h"

#include "tokenizers/fundamental/fundamental_tokenizer.h"

#include <memory>

namespace tokenizers {

std::unique_ptr<BertTokenizer> BertTokenizer::CreateTokenizer(Options options) {
  return std::unique_ptr<BertTokenizer>(new BertTokenizer(options));
}

BertTokenizer::BertTokenizer(Options options)
    : FundamentalTokenizer(options.f_options),
      do_basic_tokenize_(options.do_basic_tokenize) {
  LoadVocab(options.vocab_file, &token_id_map_);
  for (const auto& [token, id] : token_id_map_) {
    id_token_map_[id] = token;
  }
  cls_token_id_ = token_id_map_.at(cls_token_);
  sep_token_id_ = token_id_map_.at(sep_token_);
  pad_token_id_ = token_id_map_.at(pad_token_);

  basic_tokenizer_ =
      BasicTokenizer(options.do_lower_case, options.tokenize_chinese_chars,
                     options.strip_accents, GetMutableSpecialTokens());
  wordpiece_tokenizer_ =
      WordpieceTokenizer(options.f_options.unk_token, &token_id_map_);
}

std::vector<icu::UnicodeString> BertTokenizer::TokenizeImpl(
    const icu::UnicodeString& text) {
  std::vector<icu::UnicodeString> split_tokens;
  if (do_basic_tokenize_) {
    for (const auto& token : basic_tokenizer_.Tokenize(text)) {
      if (auto it = special_tokens_.find(token); it != special_tokens_.end()) {
        split_tokens.push_back(token);
      } else {
        auto tokens = wordpiece_tokenizer_.Tokenize(token);
        split_tokens.insert(split_tokens.end(), tokens.begin(), tokens.end());
      }
    }
  } else {
    auto tokens = wordpiece_tokenizer_.Tokenize(text);
    split_tokens.insert(split_tokens.end(), tokens.begin(), tokens.end());
  }
  return split_tokens;
}

int BertTokenizer::ConvertTokenToId(const icu::UnicodeString& token) {
  auto it = token_id_map_.find(token);
  if (it == token_id_map_.end()) {
    it = token_id_map_.find(unk_token_);
  }
  return it->second;
}

icu::UnicodeString BertTokenizer::ConvertIdToToken(int idx) {
  if (auto it = id_token_map_.find(idx); it != id_token_map_.end()) {
    return it->second;
  }
  return unk_token_;
}

std::vector<int> BertTokenizer::BuildInputsWithSpecialTokens(
    const std::vector<int>* token_ids_0, const std::vector<int>* token_ids_1) {
  std::vector<int> token_ids;
  if (token_ids_1 != nullptr) {
    token_ids.push_back(cls_token_id_);
    token_ids.insert(token_ids.end(), token_ids_0->begin(), token_ids_0->end());
    token_ids.push_back(sep_token_id_);
    token_ids.insert(token_ids.end(), token_ids_1->begin(), token_ids_1->end());
    token_ids.push_back(sep_token_id_);
  } else {
    token_ids.push_back(cls_token_id_);
    token_ids.insert(token_ids.end(), token_ids_0->begin(), token_ids_0->end());
    token_ids.push_back(sep_token_id_);
  }
  return token_ids;
}

std::vector<int> BertTokenizer::CreateTokenTypeIdsFromSequences(
    const std::vector<int>* token_ids_0, const std::vector<int>* token_ids_1) {
  std::vector<int> token_ids(token_ids_0->size() + 2, 0);  // cls +sep

  if (token_ids_1) {
    std::vector<int> ids(token_ids_1->size() + 1, 1);  // sep
    token_ids.insert(token_ids.end(), ids.begin(), ids.end());
  }
  return token_ids;
}

std::vector<int> BertTokenizer::GetSpecialTokensMask(
    const std::vector<int>* token_ids_0, const std::vector<int>* token_ids_1) {
  std::vector<int> token_ids(token_ids_0->size() + 2, 0);
  token_ids[0] = 1;                     // cls
  token_ids[token_ids.size() - 1] = 1;  // sep

  if (token_ids_1) {
    std::vector<int> ids(token_ids_1->size() + 1, 0);
    token_ids.insert(token_ids.end(), ids.begin(), ids.end());
    token_ids[token_ids.size() - 1] = 1;  // sep
  }

  return token_ids;
}

std::vector<int> BertTokenizer::GetInputIds(
    const std::vector<icu::UnicodeString>& tokens) {
  std::vector<int> token_ids;
  for (const auto& token : tokens) {
    token_ids.push_back(ConvertTokenToId(token));
  }
  return token_ids;
}

int BertTokenizer::GetTokenId(const icu::UnicodeString& token) {
  if (auto it = token_id_map_.find(token); it != token_id_map_.end()) {
    return it->second;
  }
  return -1;
}

}  // namespace tokenizers
