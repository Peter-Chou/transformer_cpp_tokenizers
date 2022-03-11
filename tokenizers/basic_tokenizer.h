#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace tokenizers {
class BasicTokenizer {
  using tokenMapList =
      std::vector<std::unordered_map<std::string, std::unordered_set<int>>>;

 public:
  void AddSpecialToken(const std::string& token);
  std::vector<std::string> SplitBySpecialToken() { return {}; }

 private:
  // std::vector<std::string> special_tokens_;
  tokenMapList token_map_list_;

  std::vector<std::string> model_input_names_ = {"input_ids", "token_type_ids",
                                                 "attention_mask"};
  std::string padding_side_ = "right";
  std::string truncation_side_ = "right";
};
}  // namespace tokenizers
