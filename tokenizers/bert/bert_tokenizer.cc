#include "bert/bert_tokenizer.h"

#include "fundamental/fundamental_tokenizer.h"

#include <unicode/ustream.h>

#include <iostream>
#include <memory>

namespace tokenizers {

std::unique_ptr<FundamentalTokenizer> BertTokenizer::CreateBertTokenizer(
    Options options) {
  return std::unique_ptr<FundamentalTokenizer>(new BertTokenizer(options));
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
        // for (auto& t : tokens) {
        //   std::cout << t << std::endl;
        // }
        split_tokens.insert(split_tokens.end(), tokens.begin(), tokens.end());
      }
    }
  } else {
    auto tokens = wordpiece_tokenizer_.Tokenize(text);
    split_tokens.insert(split_tokens.end(), tokens.begin(), tokens.end());
  }
  return split_tokens;
}
}  // namespace tokenizers
