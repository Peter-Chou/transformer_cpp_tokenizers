#include "tokenizers/basic/basic_tokenizer.h"

#include "tokenizers/utils/tokenizer_utils.h"

#include <unicode/brkiter.h>
#include <unicode/ubrk.h>
#include <unicode/umachine.h>
#include <unicode/unistr.h>

#include <algorithm>

namespace tokenizers {

std::vector<icu::UnicodeString> BasicTokenizer::Tokenize(
    const icu::UnicodeString& text) {
  auto tmp_text = CleanText(text);
  if (tokenize_chinese_chars_) {
    tmp_text = TokenizeChineseChars(tmp_text);
  }

  icu::UnicodeString processed_text;
  bool is_start = true;
  for (auto& token : WhitespaceTokenize(tmp_text)) {
    if (auto it = special_tokens_ptr_->find(token);
        it == special_tokens_ptr_->end()) {
      if (do_lower_case_) {
        token = token.toLower();
      }
      if (strip_accents_) {
        token = StripAccents(token);
      }
    }
    auto sub_tokens = SplitByPunctuation(token, special_tokens_ptr_);
    for (auto& sub_token : sub_tokens) {
      if (is_start) {
        is_start = false;
      } else {
        processed_text += static_cast<UChar32>(U' ');
      }
      processed_text += sub_token;
    }
  }
  return WhitespaceTokenize(processed_text);
}

}  // namespace tokenizers
