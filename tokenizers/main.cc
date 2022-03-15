
#include "basic_tokenizer.h"

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
  // icu::UnicodeString test("you are good, 你是最棒的。");
  icu::UnicodeString test("you");
  icu::UnicodeString test2("[MASK]");
  icu::UnicodeString test3("[SEP]");
  icu::UnicodeString test4("[CLS]");
  auto tokenizer = BasicTokenizer{};
  tokenizer.AddSpecialToken(test);
  tokenizer.AddSpecialToken(test2);
  tokenizer.AddSpecialToken(test3);
  tokenizer.AddSpecialToken(test4);
  // auto setlist = tokenizer.GetTokenSetMapList();
  // icu::UnicodeString text("[CLS]you are good,[SEP] 你是最棒的。[SEP]。");
  icu::UnicodeString text("[CLS]you are good,[SEP] 你是最棒的[SEP]");

  std::cout << text.length() << std::endl;
  // auto span = tokenizer.GetSpecialTokenSpan(text, 8);
  // std::cout << span.length << std::endl;
  // std::cout << icu::UnicodeString(text, span.start, span.length) <<
  // std::endl;
  // auto spanlist = tokenizer.getSpecialTokenSpans(text);
  // for (auto& span : spanlist) {
  //   std::cout << span.start << '\t' << span.length << std::endl;
  // }

  auto sub_texts = tokenizer.SplitBySpecialToken(text);
  for (auto& text : sub_texts) {
    std::cout << text << std::endl;
  }

  // int j = 0;
  // for (auto& map : setlist) {
  //   std::cout << "list: " << j++ << std::endl;
  //   for (auto& item : map) {
  //     std::cout << item.first << std::endl;
  //     for (auto& k : item.second) {
  //       std::cout << k << std::endl;
  //     }
  //   }
  //   std::cout << std::endl;
  // }

  // std::unordered_map<icu::UnicodeString, std::set<int>> testMap{
  //     {icu::UnicodeString(""), {1, 2}}};
  // // auto& set = testMap["a"];
  // // set.insert(3);
  // // set.insert(2);
  // std::cout << icu::UnicodeString("") << std::endl;
  // for (auto i : testMap[""]) {
  //   std::cout << i << std::endl;
  // }

  // std::set<int> s1{1, 2, 5, 7, 8};
  // std::set<int> s2{9, 3, 5, 10, 7};
  // std::set<int> intersect;
  // set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(),
  //                  std::inserter(intersect, intersect.begin()));

  // std::set<int> s3{6, 2, 5, 22};
  // std::set<int> out = intersect;
  // intersect.clear();
  // set_intersection(s3.begin(), s3.end(), out.begin(), out.end(),
  //                  std::inserter(intersect, intersect.begin()));
  // for (auto i : intersect) {
  //   std::cout << i << std::endl;
  // }
}
