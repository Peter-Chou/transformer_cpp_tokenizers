#include "tokenizers/basic/wordpiece_tokenizer.h"

#include "tokenizers/basic/basic_tokenizer.h"
#include "tokenizers/utils/tokenizer_utils.h"

#include <unicode/unistr.h>

namespace tokenizers {

std::vector<icu::UnicodeString> WordpieceTokenizer::Tokenize(
    icu::UnicodeString text) {
  std::vector<icu::UnicodeString> outputs;
  bool is_bad = false;
  int start = 0;
  int end;
  icu::UnicodeString cur_substr;
  icu::UnicodeString temp_str;
  std::vector<icu::UnicodeString> sub_tokens;
  for (const auto& token : WhitespaceTokenize(text)) {
    int token_length = token.length();
    if (token_length > max_input_chars_per_word_) {
      outputs.push_back(unk_token_);
      continue;
    }
    if (token_length == 1) {
      if (auto it = vocab_ptr_->find(token); it != vocab_ptr_->end()) {
        outputs.push_back(token);
      } else {
        outputs.push_back(unk_token_);
      }
      continue;
    }

    while (start < token_length) {
      end = token_length;
      cur_substr.remove();
      while (start < end) {
        icu::UnicodeString substr;
        if (start > 0) {
          substr += "##";
        }
        temp_str.remove();
        token.extract(start, end - start, temp_str);
        substr += temp_str;
        if (auto it = vocab_ptr_->find(substr); it != vocab_ptr_->end()) {
          cur_substr = substr;
          break;
        }
        end--;
      }
      if (cur_substr.isEmpty()) {
        is_bad = true;
        break;
      }
      sub_tokens.push_back(cur_substr);
      start = end;
      cur_substr.remove();
    }
    if (is_bad) {
      outputs.push_back(unk_token_);
    } else {
      outputs.insert(outputs.end(), sub_tokens.begin(), sub_tokens.end());
    }
    is_bad = false;
    start = 0;
    sub_tokens.clear();
  }

  return outputs;
}
}  // namespace tokenizers
