
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
  icu::UnicodeString text(
      "\tYou  aReü gOod, unaffable example ü   "
      "你是\t最棒的！\t");
  icu::UnicodeString text_b("谢谢你哦！我知道拉，haha ü去吃饭咯");
  FundamentalTokenizer::Options f_options{};
  BertTokenizer::Options bert_options;
  bert_options.f_options = f_options;
  bert_options.vocab_file =
      "/home/peter/projects/cgec-initialized-with-plm/vocabs/vocab.txt";
  auto tokenizer = BertTokenizer::CreateBertTokenizer(bert_options);
  auto outputs = tokenizer->Encode(&text_b, nullptr, 20);
  std::cout << "input_ids size: " << outputs.input_ids.size() << std::endl;
  for (auto& t : outputs.input_ids) {
    std::cout << t << std::endl;
  }
}
