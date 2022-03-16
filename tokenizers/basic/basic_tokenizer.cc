#include "basic/basic_tokenizer.h"

#include "utils/tokenizer_utils.h"

#include <unicode/brkiter.h>
#include <unicode/ubrk.h>
#include <unicode/umachine.h>
#include <unicode/unistr.h>
#include <unicode/ustream.h>

#include <algorithm>
#include <iostream>

namespace tokenizers {

using TokenSpan = BasicTokenizer::TokenSpan;

BasicTokenizer::BasicTokenizer(
    bool do_lower_case,
    const std::unordered_set<icu::UnicodeString>& never_split)
    : do_lower_case_(do_lower_case) {
  for (auto& token : never_split) {
    AddSpecialToken(token);
  }
}

void BasicTokenizer::AddSpecialToken(const icu::UnicodeString& token) {
  special_tokens_.insert(token);

  icu::BreakIterator* boundary;
  UErrorCode status = U_ZERO_ERROR;
  boundary =
      icu::BreakIterator::createCharacterInstance(icu::Locale::getUS(), status);
  boundary->setText(token);
  auto start = boundary->first();
  int length = 0;
  int token_idx = special_tokens_.size() - 1;
  for (auto end = boundary->next(); end != icu::BreakIterator::DONE;
       start = end, end = boundary->next()) {
    icu::UnicodeString uchar(token, start, 1);
    addUcharToSet(uchar, length, token_idx);
    length++;
  }
  addUcharToSet(icu::UnicodeString(""), length, token_idx);
}

std::vector<icu::UnicodeString> BasicTokenizer::SplitBySpecialToken(
    const icu::UnicodeString& text) {
  std::vector<icu::UnicodeString> sub_texts;
  auto token_spans = getSpecialTokenSpans(text);
  int start = 0;
  for (const auto& span : token_spans) {
    if (start < span.start) {
      sub_texts.push_back(icu::UnicodeString(text, start, span.start - start));
    }
    sub_texts.push_back(icu::UnicodeString(text, span.start, span.length));
    start = span.start + span.length;
  }
  if (start < text.length()) {
    sub_texts.push_back(icu::UnicodeString(text, start, text.length() - start));
  }
  return sub_texts;
}

std::vector<icu::UnicodeString> BasicTokenizer::SplitByPunctuation(
    const icu::UnicodeString& text) {
  if (auto it = special_tokens_.find(text); it != special_tokens_.end()) {
    return {text};
  }
  std::vector<icu::UnicodeString> outputs;
  icu::UnicodeString temp_str;
  std::vector<std::vector<UChar32>> temp_chars;
  bool start_new_word = true;

  auto length = text.length();
  UErrorCode status = U_ZERO_ERROR;
  UChar32 uchars[length];
  text.toUTF32(uchars, length, status);
  for (auto& uchar : uchars) {
    if (IsPunctuation(uchar)) {
      temp_chars.push_back(std::vector<UChar32>{uchar});
      start_new_word = true;
    } else {
      if (start_new_word) {
        temp_chars.push_back(std::vector<UChar32>{});
      }
      start_new_word = false;
      temp_chars.back().push_back(uchar);
    }
  }
  for (const auto& uchar_list : temp_chars) {
    temp_str.remove();
    for (const auto& uchar : uchar_list) {
      temp_str += uchar;
    }
    outputs.push_back(temp_str);
  }

  return outputs;
}

std::vector<TokenSpan> BasicTokenizer::getSpecialTokenSpans(
    const icu::UnicodeString& text) {
  std::vector<TokenSpan> token_spans;

  icu::BreakIterator* boundary;
  UErrorCode status = U_ZERO_ERROR;
  boundary =
      icu::BreakIterator::createCharacterInstance(icu::Locale::getUS(), status);
  boundary->setText(text);
  auto start = boundary->first();

  const auto& start_char_map = char_ids_map_list_.at(0);

  for (auto end = boundary->next(); end != icu::BreakIterator::DONE;
       start = end, end = boundary->next()) {
    icu::UnicodeString uchar(text, start, 1);
    if (auto it = start_char_map.find(uchar); it != start_char_map.end()) {
      TokenSpan span = GetSpecialTokenSpan(text, start);
      if (span.length > 0) {
        token_spans.push_back(span);
      }
    }
  }
  return token_spans;
}

TokenSpan BasicTokenizer::GetSpecialTokenSpan(const icu::UnicodeString& text,
                                              int start) {
  TokenSpan span;
  span.start = start;
  span.length = -1;
  icu::BreakIterator* boundary;
  UErrorCode status = U_ZERO_ERROR;
  boundary =
      icu::BreakIterator::createCharacterInstance(icu::Locale::getUS(), status);
  boundary->setText(text);
  boundary->following(start);
  start = boundary->previous();

  size_t length = 0;
  CharIdsMap* charMap;
  std::set<int> old_set;
  std::set<int> intersect;
  int token_size = special_tokens_.size();
  for (int i = 0; i < token_size; i++) {
    old_set.insert(i);
  }

  for (auto end = boundary->next(); end != icu::BreakIterator::DONE;
       start = end, end = boundary->next()) {
    charMap = &char_ids_map_list_.at(length);
    icu::UnicodeString uchar(text, start, 1);
    if (auto it = charMap->find(uchar); it == charMap->end()) {
      if (auto it = charMap->find(icu::UnicodeString(""));
          it != charMap->end()) {
        const auto& current_set = it->second;
        intersect.clear();
        std::set_intersection(current_set.begin(), current_set.end(),
                              old_set.begin(), old_set.end(),
                              std::inserter(intersect, intersect.begin()));
        if (!intersect.empty()) {
          span.length = length;
        }
        return span;
      }
      return span;
    } else {
      const auto& current_set = it->second;
      intersect.clear();
      std::set_intersection(current_set.begin(), current_set.end(),
                            old_set.begin(), old_set.end(),
                            std::inserter(intersect, intersect.begin()));
      old_set = intersect;
      if (intersect.empty()) {
        return span;
      }
    }
    length++;
  }
  charMap = &char_ids_map_list_.at(length);
  if (auto it = charMap->find(icu::UnicodeString("")); it != charMap->end()) {
    span.length = length;
  }

  return span;
}

void BasicTokenizer::addUcharToSet(const icu::UnicodeString& uchar,
                                   int list_pos, int token_idx) {
  if (list_pos < (int)char_ids_map_list_.size()) {
    auto& map = char_ids_map_list_[list_pos];
    auto it = map.find(uchar);
    if (it != map.end()) {
      it->second.insert(token_idx);
    } else {
      map[uchar] = {token_idx};
    }
  } else {
    CharIdsMap map;
    map[uchar] = std::set<int>{token_idx};
    char_ids_map_list_.push_back(map);
  }
}

std::vector<icu::UnicodeString> BasicTokenizer::Tokenize(
    icu::UnicodeString& text) {
  text = CleanText(text);
  if (tokenize_chinese_chars_) {
    text = TokenizeChineseChars(text);
  }
  auto sub_texts = SplitBySpecialToken(text);

  icu::UnicodeString processed_text;
  for (auto& sub_text : sub_texts) {
    for (auto& token : WhitespaceTokenize(sub_text)) {
      // orig_tokens.push_back(token);
      if (auto it = special_tokens_.find(token); it == special_tokens_.end()) {
        if (do_lower_case_) {
          token = token.toLower();
        }
      }
      auto sub_tokens = SplitByPunctuation(token);
      for (auto& sub_token : sub_tokens) {
        processed_text += static_cast<UChar32>(U' ');
        processed_text += sub_token;
      }
    }
  }
  processed_text = LTrim(processed_text);
  return WhitespaceTokenize(processed_text);
}

}  // namespace tokenizers
