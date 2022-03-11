#pragma once

#include <fstream>
#include <string>
#include <string_view>
#include <unordered_map>

namespace tokenizers {
class BertTokenizer {
 public:
  BertTokenizer(std::string_view vocab_file) { load(vocab_file); }

  std::unordered_map<std::string, int>* getMutableVocab() {
    return &token2int_;
  }

 private:
  std::unordered_map<std::string, int> load(std::string_view vocab_file) {
    std::ifstream fin(vocab_file.data());
    int idx = 0;
    if (fin.is_open()) {
      std::string token;
      while (std::getline(fin, token)) {
        token2int_[token] = idx;
        ++idx;
      }
    }
    return {};
  }

  std::unordered_map<std::string, int> token2int_;
};

}  // namespace tokenizers
