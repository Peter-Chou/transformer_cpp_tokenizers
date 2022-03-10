#pragma once

#include <string>
#include <vector>

namespace tokenizers {
class BasicTokenizer {
 public:
  // BasicTokenizer(bool doLowerCase = True, )
  std::vector<std::string> tokenize(const std::string& text) const;
};
}  // namespace tokenizers
