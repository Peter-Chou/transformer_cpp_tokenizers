#include "basic_tokenizer.h"

#include <unicode/brkiter.h>
#include <unicode/ubrk.h>
#include <unicode/unistr.h>
#include <unicode/ustream.h>

#include <algorithm>
#include <iostream>

namespace tokenizers {

using TokenSpan = BasicTokenizer::TokenSpan;

void BasicTokenizer::AddSpecialToken(const icu::UnicodeString& token) {
  special_tokens_.push_back(token);

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

}  // namespace tokenizers
