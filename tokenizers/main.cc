
#include "basic_tokenizer.h"
#include "tokenizer_utils.h"

#include <unicode/unistr.h>

using namespace tokenizers;

#include <unicode/ustream.h>

#include <algorithm>
#include <functional>
#include <iostream>
#include <set>
#include <unordered_map>

// template <>
// struct std::hash<icu::UnicodeString> {
//   size_t operator()(const icu::UnicodeString& x) const { return x.hashCode();
//   }
// };

int main() {
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

  icu::UnicodeString text("\t[CLS]you are good,[SEP] 你是最棒的[SEP]\t");
  // text = LTrim(text);
  // text = RTrim(text);
  // text = Strip(text);
  std::cout << text << "aaaaaaaa" << std::endl;
}
