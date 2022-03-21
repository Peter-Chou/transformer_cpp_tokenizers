
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
  // icu::UnicodeString text(
  //     "\tYou  aReü gOod, unaffable example ü   "
  //     "你是\t最棒的！\t");
  icu::UnicodeString text(
      "3月19日024时，31个省（自治区、直辖市）和新疆生产建设兵团报告新增确诊病例"
      "1737例。其中境外输入病例81例（广东22例，北京14例，浙江11例，广西8例，山"
      "东6例，上海5例，天津4例，四川4例，湖北3例，福建2例，黑龙江1例，河南1例）"
      "，含15例由无症状感染者转为确诊病例（浙江8例，四川4例，河南1例，广东1例，"
      "广西1例）；本土病例1656例（吉林1191例，其中长春市833例、吉林市327例、四"
      "平市24例、延边朝鲜族自治州5例、松原市2例；福建158例，其中泉州市138例、莆"
      "田市13例、厦门市4例、漳州市3例；山东51例，其中滨州市35例、青岛市6例、临"
      "沂市4例、威海市3例、烟台市2例、德州市1例；广东51例，其中深圳市49例、珠海"
      "市1例、东莞市1例；辽宁39例，其中沈阳市17例、大连市11例、营口市11例；甘肃"
      "29例，其中兰州市25例、白银市2例、兰州新区2例；天津21例，其中西青区7例、"
      "武清区6例、滨海新区5例、南开区1例、红桥区1例、东丽区1例；河北21例，其中"
      "廊坊市12例、唐山市7例、邯郸市1例、沧州市1例；浙江19例，其中衢州市15例、"
      "嘉兴市2例、杭州市1例、宁波市1例；黑龙江18例，均在哈尔滨市；上海17例，其"
      "中浦东新区8例、闵行区4例、嘉定区2例、虹口区1例、宝山区1例、崇明区1例；陕"
      "西15例，其中宝鸡市11例、铜川市3例、汉中市1例；江西6例，其中南昌市5例、赣"
      "州市1例；湖南6例，其中邵阳市2例、长沙市1例、岳阳市1例、永州市1例、怀化市"
      "1例；云南4例，均在德宏傣族景颇族自治州；安徽3例，其中铜陵市2例、马鞍山市"
      "1例；重庆3例，均在沙坪坝区；河南2例，其中郑州市1例、信阳市1例；北京1例，"
      "在朝阳区；江苏1例，在盐城市），含76例由无症状感染者转为确诊病例（吉林19"
      "例，黑龙江14例，辽宁11例，上海6例，浙江6例，天津4例，福建4例，江西3例，"
      "山东3例，安徽2例，甘肃2例，江苏1例，湖南1例）。无新增死亡病例。新增疑似"
      "病例3例，均为境外输入病例（均在上海）。");

  icu::UnicodeString text_b("谢谢你哦！我知道拉，haha ü去吃饭咯");
  FundamentalTokenizer::Options f_options{};
  BertTokenizer::Options bert_options;
  bert_options.f_options = f_options;
  bert_options.vocab_file =
      "/home/peter/projects/cgec-initialized-with-plm/vocabs/vocab.txt";
  // auto tokenizer = BertTokenizer(options);
  auto tokenizer = BertTokenizer::CreateBertTokenizer(bert_options);
  // tokenizer->Encode(&text, &text_b);
  auto outputs = tokenizer->Encode(&text_b, nullptr, 20);
  // std::cout << "attention mask size: " << (*outputs.attention_mask).size()
  //           << std::endl;
  // for (auto& t : (*outputs.attention_mask)) {
  //   std::cout << t << std::endl;
  // }
  std::cout << "input_ids size: " << outputs.input_ids.size() << std::endl;
  for (auto& t : outputs.input_ids) {
    std::cout << t << std::endl;
  }

  // icu::UnicodeString text("你");
  // auto results = WhitespaceTokenize(text);
  // for (auto& text : results) {
  //   std::cout << text << std::endl;
  // }

  // auto sub_texts = tokenizer.SplitBySpecialToken(text);
  // for (auto& text : sub_texts) {
  //   std::cout << text << std::endl;
  //   // }

  // char32_t uchar = '\r';
  // char32_t uchar = U'\U0010FFFF';
  // char32_t uchar = U'\U00000020';  //  space ' '
  // bool res = IsWhiteSpace(uchar);
  // char32_t uchar = U'\U0000002D';
  // bool res = IsPunctuation(uchar);
  // std::cout << std::boolalpha;
  // std::cout << res << std::endl;

  // icu::UnicodeString text(
  //                         "\t[CLS]You  aReü gOod,[SEP] unaffable example ü "
  //                         "你是\t最棒的！[SEP]\t");

  // auto bert_tokenizer = BertTokenizer(
  //                                     "/home/peter/projects/cgec-initialized-with-plm/vocabs/vocab.txt");

  // auto tokenizer = WordpieceTokenizer(bert_tokenizer.getMutableVocab());
  // auto sub_texts = tokenizer.Tokenize(text);

  // auto tokenizer = BasicTokenizer();
  // auto sub_texts = tokenizer.Tokenize(text);
  // for (auto& output : sub_texts) {
  //   std::cout << output << std::endl;
  // }

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
  //     std::u32string(reinterpret_cast<const char32_t*>(temp),
  //     text.length());
  // std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
  // std::cout << converter.to_bytes(std_text) << std::endl;
  // std::cout << converter.to_bytes(std_str) << std::endl;
}
