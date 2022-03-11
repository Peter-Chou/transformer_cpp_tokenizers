#pragma once

#include <regex>
#include <string>
#include <vector>

namespace tokenizers {
class WhitespaceTokenizer {
 public:
  std::vector<std::string> WhitespaceTokenize(const std::string& text) {
    std::vector<std::string> tokens;
    std::sregex_token_iterator iter(text.begin(), text.end(), rgx_, -1);

    while (iter != end_) {
      if (iter->length()) {
        tokens.push_back(*iter);
      }
      ++iter;
    }
    return tokens;
  }

 private:
  std::regex rgx_ = std::regex("\\s+");
  std::sregex_token_iterator end_;
};

}  // namespace tokenizers
