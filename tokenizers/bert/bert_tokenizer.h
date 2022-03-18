#pragma once

#include "basic/basic_tokenizer.h"
#include "basic/wordpiece_tokenizer.h"
#include "fundamental/fundamental_tokenizer.h"

#include <unicode/unistr.h>

#include <fstream>
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

  static std::unique_ptr<FundamentalTokenizer> CreateBertTokenizer(
      Options options);

  BertTokenizer(Options options)
      : FundamentalTokenizer(options.f_options),
        do_basic_tokenize_(options.do_basic_tokenize) {
    loadVocab(options.vocab_file);
    for (const auto& [token, id] : token_id_map_) {
      id_token_map_[id] = token;
    }
    basic_tokenizer_ =
        BasicTokenizer(options.do_lower_case, options.tokenize_chinese_chars,
                       options.strip_accents, GetMutableSpecialTokens());
    wordpiece_tokenizer_ =
        WordpieceTokenizer(options.f_options.unk_token, &token_id_map_);
  }
  std::vector<icu::UnicodeString> TokenizeImpl(const icu::UnicodeString& text);

  std::unordered_map<icu::UnicodeString, int>* getMutableVocab() {
    return &token_id_map_;
  }

 private:
  void loadVocab(const std::string& vocab_file) {
    std::ifstream fin(vocab_file);
    int idx = 0;
    if (fin.is_open()) {
      std::string token;
      while (std::getline(fin, token)) {
        token_id_map_[icu::UnicodeString::fromUTF8(token)] = idx;
        ++idx;
      }
    }
  }

  bool do_basic_tokenize_ = true;

  BasicTokenizer basic_tokenizer_;
  WordpieceTokenizer wordpiece_tokenizer_;

  std::unordered_map<icu::UnicodeString, int> token_id_map_;
  std::unordered_map<int, icu::UnicodeString> id_token_map_;
};

}  // namespace tokenizers
