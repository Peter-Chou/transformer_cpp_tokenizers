
#include "basic/basic_tokenizer.h"
#include "basic/wordpiece_tokenizer.h"
#include "bert/bert_tokenizer.h"
#include "unilib/uninorms.h"
#include "utils/tokenizer_utils.h"

#include <unicode/unistr.h>

#include <string>

using namespace tokenizers;

#include <unicode/chariter.h>
#include <unicode/ustream.h>

#include <algorithm>
#include <codecvt>
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

  icu::UnicodeString text(
      "\t[CLS]You  aReü gOod,[SEP] unaffable example ü "
      "你是\t最棒的！[SEP]\t");

  auto bert_tokenizer = BertTokenizer(
      "/home/peter/projects/cgec-initialized-with-plm/vocabs/vocab.txt");

  auto tokenizer = WordpieceTokenizer(bert_tokenizer.getMutableVocab());
  auto sub_texts = tokenizer.Tokenize(text);

  // auto tokenizer = BasicTokenizer();
  // auto sub_texts = tokenizer.Tokenize(text);
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
  // std::u32string std_str = U"hello, 你好呀。ü饭吃拉么？";
  // icu::UnicodeString text("hello, 你好呀。ü饭吃拉么？");

  // ufal::unilib::uninorms::nfd(std_str);
  // std::cout << std_str.size() << std::endl;

  // icu::UnicodeString text = icu::UnicodeString::fromUTF32(
  //     reinterpret_cast<const UChar32*>(std_str.c_str()), std_str.size());
  // UErrorCode status = U_ZERO_ERROR;
  // UChar32 temp[text.length()];
  // text.toUTF32(temp, text.length(), status);
  // std::u32string std_text =
  //     std::u32string(reinterpret_cast<const char32_t*>(temp), text.length());
  // std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
  // std::cout << converter.to_bytes(std_text) << std::endl;
  // std::cout << converter.to_bytes(std_str) << std::endl;
}
