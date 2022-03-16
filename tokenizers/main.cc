
#include "basic/basic_tokenizer.h"
#include "bert/bert_tokenizer.h"
#include "utils/tokenizer_utils.h"

#include <unicode/unistr.h>

using namespace tokenizers;

#include <unicode/chariter.h>
#include <unicode/ustream.h>

#include <algorithm>
#include <cstdio>
#include <functional>
#include <iostream>
#include <set>
#include <unordered_map>

int main() {
  // auto tokenizer = BertTokenizer(
  //     "/home/peter/projects/cgec-initialized-with-plm/vocabs/vocab.txt");

  // auto map_ptr = tokenizer.getMutableVocab();
  // for (const auto& [token, id] : *map_ptr) {
  //   std::cout << token << "\t" << id << std::endl;
  // }

  // UnicodeString testString(testChars, "utf-8");

  // icu::UnicodeString text("[CLS]you are good,[SEP] 你是最棒的[SEP]");
  // icu::UnicodeString test("you");
  // icu::UnicodeString test2("[MASK]");
  // icu::UnicodeString test3("[SEP]");
  // icu::UnicodeString test4("[CLS]");
  // auto tokenizer = BasicTokenizer{};
  // tokenizer.AddSpecialToken(test);
  // tokenizer.AddSpecialToken(test2);
  // tokenizer.AddSpecialToken(test3);
  // tokenizer.AddSpecialToken(test4);

  // auto sub_texts = tokenizer.SplitBySpecialToken(text);
  // for (auto& text : sub_texts) {
  //   std::cout << text << std::endl;
  // }

  // char32_t uchar = '\r';
  // char32_t uchar = U'\U0010FFFF';
  // char32_t uchar = U'\U00000020';  //  space ' '
  // bool res = IsWhiteSpace(uchar);
  // char32_t uchar = U'\U0000002D';
  // bool res = IsPunctuation(uchar);
  // std::cout << std::boolalpha;
  // std::cout << res << std::endl;

  icu::UnicodeString text("\t[CLS]You aRe gOod,[SEP] 你是\t最棒的！[SEP]\t");
  // icu::UnicodeString text("[CLS] you");

  auto tokenizer = BasicTokenizer();
  auto sub_texts = tokenizer.Tokenize(text);
  // auto sub_texts = WhitespaceTokenize(text);
  for (auto& output : sub_texts) {
    std::cout << output << std::endl;
  }

  // text = TokenizeChineseChars(text);
  // text = text.toLower();
  // std::cout << text << std::endl;
  // auto tokenizer = BasicTokenizer();
  // tokenizer.Tokenize(text);

  // auto new_text =CleanText(text);
  // std::cout << new_text << std::endl;

  // auto length = text.length();
  // UChar32 delimiter(U' ');
  // auto idx = text.indexOf(delimiter, 10 );
  // std::cout << idx << std::endl;

  // text = CleanText(text);
  // auto outputs = WhitespaceTokenize(text);
  // std::cout << text << std::endl;
  // for (auto& output : outputs) {
  //   std::cout << output << std::endl;
  // }
}
