#include "tokenizers/fundamental/fundamental_tokenizer.h"

#include "tokenizers/utils/tokenizer_utils.h"

#include <unicode/brkiter.h>
#include <unicode/unistr.h>

#include <algorithm>
#include <cstddef>
#include <iterator>

namespace tokenizers {
using TokenSpan = FundamentalTokenizer::TokenSpan;

FundamentalTokenizer::FundamentalTokenizer(Options options)
    : unk_token_(options.unk_token),
      sep_token_(options.sep_token),
      pad_token_(options.pad_token),
      cls_token_(options.cls_token),
      mask_token_(options.mask_token) {
  addSpecialToken(options.unk_token);
  addSpecialToken(options.sep_token);
  addSpecialToken(options.pad_token);
  addSpecialToken(options.cls_token);
  addSpecialToken(options.mask_token);
}

std::vector<EncodeOutput> FundamentalTokenizer::BatchEncode(
    const std::vector<std::string>* a_texts,
    const std::vector<std::string>* b_texts, int max_length,
    bool add_special_tokens, TruncateStrategy truncate_stratety,
    PaddingStrategy padding_strategy, bool return_token_type_ids,
    bool return_attention_mask) {
  std::vector<EncodeOutput> outputs;
  const std::string* b_text = nullptr;
  for (size_t i = 0; i < a_texts->size(); ++i) {
    const std::string* a_text = &a_texts->at(i);
    if (b_texts != nullptr) {
      b_text = &b_texts->at(i);
    } else {
      b_text = nullptr;
    }
    outputs.push_back(Encode(a_text, b_text, max_length, add_special_tokens,
                             truncate_stratety, padding_strategy,
                             return_token_type_ids, return_attention_mask));
  }
  return outputs;
}
EncodeOutput FundamentalTokenizer::Encode(
    const std::string* a_text, const std::string* b_text, int max_length,
    bool add_special_tokens, TruncateStrategy truncate_stratety,
    PaddingStrategy padding_strategy, bool return_token_type_ids,
    bool return_attention_mask) {
  EncodeOutput output;
  icu::UnicodeString a_utext = icu::UnicodeString::fromUTF8(*a_text);
  std::vector<int> a_utext_token_ids = GetInputIds(Tokenize(a_utext));
  std::vector<int> b_utext_token_ids;
  if (b_text) {
    icu::UnicodeString b_utext = icu::UnicodeString::fromUTF8(*b_text);
    b_utext_token_ids = GetInputIds(Tokenize(b_utext));
  }

  int total_length = a_utext_token_ids.size() + b_utext_token_ids.size();
  if (add_special_tokens) {
    total_length +=
        numSpecialTokensToAdd(/* pair= */ !b_utext_token_ids.empty());
  }

  truncateSequence(&a_utext_token_ids, &b_utext_token_ids,
                   total_length - max_length, truncate_stratety);
  std::vector<int> sequence;
  std::vector<int> token_type_ids;
  if (add_special_tokens) {
    if (b_utext_token_ids.empty()) {
      sequence = BuildInputsWithSpecialTokens(&a_utext_token_ids, nullptr);
      token_type_ids =
          CreateTokenTypeIdsFromSequences(&a_utext_token_ids, nullptr);
    } else {
      sequence =
          BuildInputsWithSpecialTokens(&a_utext_token_ids, &b_utext_token_ids);
      token_type_ids = CreateTokenTypeIdsFromSequences(&a_utext_token_ids,
                                                       &b_utext_token_ids);
    }
  } else {
    sequence = a_utext_token_ids;
    if (!b_utext_token_ids.empty()) {
      sequence.insert(sequence.end(), b_utext_token_ids.begin(),
                      b_utext_token_ids.end());
      token_type_ids = std::vector<int>(
          a_utext_token_ids.size() + b_utext_token_ids.size(), 0);
    }
  }
  output.input_ids = sequence;
  if (return_token_type_ids) {
    output.token_type_ids = token_type_ids;
  }
  if (padding_strategy != PaddingStrategy::DO_NOT_PAD ||
      return_attention_mask) {
    pad(&output, max_length, padding_strategy, return_attention_mask);
  }
  return output;
}

std::vector<icu::UnicodeString> FundamentalTokenizer::Tokenize(
    const icu::UnicodeString& text) {
  std::vector<icu::UnicodeString> outputs;

  auto sub_texts = SplitBySpecialToken(Strip(text));
  for (const auto& sub_text : sub_texts) {
    if (!sub_text.isEmpty()) {
      if (auto it = special_tokens_.find(sub_text);
          it != special_tokens_.end()) {
        outputs.push_back(sub_text);
      } else {
        auto tokens = TokenizeImpl(sub_text);
        outputs.insert(outputs.end(), tokens.begin(), tokens.end());
      }
    }
  }

  return outputs;
}

icu::UnicodeString FundamentalTokenizer::ProcessSpecialTokens(
    const icu::UnicodeString& text) {
  icu::UnicodeString processed_text;
  bool is_start = true;
  for (const auto& sub_text : SplitBySpecialToken(text)) {
    if (is_start) {
      is_start = false;
    } else {
      processed_text += static_cast<UChar32>(U' ');
    }
    processed_text += sub_text;
  }
  return processed_text;
}

void FundamentalTokenizer::addSpecialToken(const icu::UnicodeString& token) {
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

std::vector<icu::UnicodeString> FundamentalTokenizer::SplitBySpecialToken(
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

std::vector<TokenSpan> FundamentalTokenizer::getSpecialTokenSpans(
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

TokenSpan FundamentalTokenizer::GetSpecialTokenSpan(
    const icu::UnicodeString& text, int start) {
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

void FundamentalTokenizer::addUcharToSet(const icu::UnicodeString& uchar,
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

std::vector<int> FundamentalTokenizer::BuildInputsWithSpecialTokens(
    const std::vector<int>* token_ids_0, const std::vector<int>* token_ids_1) {
  if (token_ids_1 != nullptr) {
    std::vector<int> token_ids = *token_ids_0;
    token_ids.insert(token_ids.end(), token_ids_1->begin(), token_ids_1->end());
    return token_ids;
  }
  return *token_ids_0;
}

std::vector<int> FundamentalTokenizer::CreateTokenTypeIdsFromSequences(
    const std::vector<int>* token_ids_0, const std::vector<int>* token_ids_1) {
  std::vector<int> token_ids(token_ids_0->size(), 0);

  if (token_ids_1) {
    std::vector<int> ids(token_ids_1->size(), 1);
    token_ids.insert(token_ids.end(), ids.begin(), ids.end());
  }
  return token_ids;
}

int FundamentalTokenizer::numSpecialTokensToAdd(bool pair) {
  std::vector<int> a_text;
  if (pair) {
    // std::vector<int> b_text;
    return BuildInputsWithSpecialTokens(&a_text, &a_text).size();
  }
  return BuildInputsWithSpecialTokens(&a_text, nullptr).size();
}

bool FundamentalTokenizer::truncateSequence(
    std::vector<int>* ids, std::vector<int>* pair_ids, int num_tokens_to_move,
    TruncateStrategy truncate_strategy) {
  if (num_tokens_to_move <= 0) {
    return true;
  }
  if (pair_ids->empty()) {
    if (truncate_strategy == TruncateStrategy::ONLY_FIRST ||
        truncate_strategy == TruncateStrategy::LONGEST_FIRST) {
      if (ids->size() > num_tokens_to_move) {
        int num_to_keep = ids->size() - num_tokens_to_move;
        if (truncate_side_ == TruncateSide::RIGHT) {
          ids->resize(num_to_keep);
        } else {
          std::vector<int>(ids->begin() + num_tokens_to_move, ids->end())
              .swap(*ids);
        }
      } else {
        return false;
      }
    } else {
      return false;
    }
  } else {
    std::vector<int>* target_ids;
    if (truncate_strategy == TruncateStrategy::LONGEST_FIRST) {
      for (int i = 0; i < num_tokens_to_move; ++i) {
        target_ids = ids;
        if (ids->size() < pair_ids->size()) {
          target_ids = pair_ids;
        }
        if (truncate_side_ == TruncateSide::RIGHT) {
          target_ids->pop_back();
        } else {
          target_ids->erase(target_ids->begin());
        }
      }
    } else {
      target_ids = ids;
      if (truncate_strategy == TruncateStrategy::ONLY_SECOND) {
        target_ids = pair_ids;
      }
      if (target_ids->size() > num_tokens_to_move) {
        int num_to_keep = target_ids->size() - num_tokens_to_move;
        if (truncate_side_ == TruncateSide::RIGHT) {
          target_ids->resize(num_to_keep);
        } else {
          std::vector<int>(target_ids->begin() + num_tokens_to_move,
                           target_ids->end())
              .swap(*target_ids);
        }
      } else {
        return false;
      }
    }
  }
  return false;
}

bool FundamentalTokenizer::pad(EncodeOutput* output, int max_length,
                               PaddingStrategy padding_strategy,
                               bool return_attention_mask) {
  auto& required_ids = output->input_ids;
  int required_ids_size = required_ids.size();
  if (padding_strategy == PaddingStrategy::LONGEST) {
    max_length = required_ids_size;
  }
  if (padding_strategy != PaddingStrategy::DO_NOT_PAD &&
      required_ids_size != max_length) {
    int pad_token_id = GetTokenId(pad_token_);
    int diff = max_length - required_ids_size;
    if (padding_side_ == PaddingSide::RIGHT) {
      required_ids.reserve(max_length);
      std::generate_n(std::back_inserter(required_ids), diff,
                      [pad_token_id] { return pad_token_id; });
      if (return_attention_mask) {
        auto attention_mask = std::vector<int>(required_ids_size, 1);
        attention_mask.reserve(max_length);
        std::generate_n(std::back_inserter(attention_mask), diff,
                        [] { return 0; });
        output->attention_mask = std::move(attention_mask);
      }
      auto& optional = output->token_type_ids;
      if (optional) {
        auto& token_type_ids = (*optional);
        token_type_ids.reserve(max_length);
        std::generate_n(std::back_inserter(token_type_ids), diff,
                        [this] { return pad_token_type_id_; });
      }
    } else {
      std::vector<int> tmp_input_ids(diff, pad_token_id);
      tmp_input_ids.reserve(max_length);
      tmp_input_ids.insert(tmp_input_ids.end(), required_ids.begin(),
                           required_ids.end());
      required_ids = std::move(tmp_input_ids);

      if (return_attention_mask) {
        auto attention_mask = std::vector<int>(diff, 0);
        attention_mask.reserve(max_length);
        std::generate_n(std::back_inserter(attention_mask), required_ids_size,
                        [] { return 1; });
        output->attention_mask = std::move(attention_mask);
      }

      auto& optional = output->token_type_ids;
      if (optional) {
        auto& token_type_ids = (*optional);

        std::vector<int> tmp_token_type_ids(diff, pad_token_type_id_);
        tmp_token_type_ids.reserve(max_length);
        tmp_token_type_ids.insert(tmp_token_type_ids.end(),
                                  token_type_ids.begin(), token_type_ids.end());
        token_type_ids = std::move(tmp_token_type_ids);
      }
    }
  } else {
    if (return_attention_mask) {
      output->attention_mask = std::vector<int>(required_ids_size, 1);
    }
  }

  return true;
}

}  // namespace tokenizers
