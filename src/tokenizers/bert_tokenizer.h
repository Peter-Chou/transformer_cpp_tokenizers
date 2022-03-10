#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>

namespace tokenizers {
class BertTokenizer {
 public:
 private:
  std::unordered_map<std::string, int> load(std::string_view vocab_file) {
    std::unordered_map<std::string, int> token2int;
    std::ifstream fin(vocab_file.data());
    if (fin.is_open()) {
      std::string token;
      while (std::getline(fin, token)) {
        std::cout << token << std::endl;
      }
    }
    return {};
  }
};

}  // namespace tokenizers
