#pragma once

#include "basic/basic_tokenizer.h"

#include <unicode/unistr.h>

#include <fstream>
#include <string>
#include <unordered_map>

namespace tokenizers {
class BertTokenizer {
 public:
  BertTokenizer(const std::string& vocab_file) {
    loadVocab(vocab_file);
    for (const auto& [token, id] : token_id_map_) {
      id_token_map_[id] = token;
    }
  }
  void Tokenize(const icu::UnicodeString& text);

  std::unordered_map<icu::UnicodeString, int>* getMutableVocab() {
    return &token_id_map_;
  }

 private:
  std::unordered_map<std::string, int> loadVocab(
      const std::string& vocab_file) {
    std::ifstream fin(vocab_file);
    int idx = 0;
    if (fin.is_open()) {
      std::string token;
      while (std::getline(fin, token)) {
        token_id_map_[icu::UnicodeString::fromUTF8(token)] = idx;
        ++idx;
      }
    }
    return {};
  }

  std::unordered_map<icu::UnicodeString, int> token_id_map_;
  std::unordered_map<int, icu::UnicodeString> id_token_map_;
};

}  // namespace tokenizers
