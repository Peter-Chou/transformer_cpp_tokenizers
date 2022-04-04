#include "tokenizers/utils/tokenizer_utils.h"

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>
#include <unicode/unistr.h>
#include <unicode/ustream.h>

#include <iostream>

namespace tokenizers {

TEST(TokenizerUtilsTest, IsWhiteSpace) {
  char32_t uchar = U'\U00000020';  // space ' '
  char32_t uchar1 = '\r';
  EXPECT_TRUE(IsWhiteSpace(uchar));
  EXPECT_TRUE(IsWhiteSpace(uchar1));
}

TEST(TokenizerUtilsTest, IsControl) {
  char32_t uchar = U'\U00000008';  // Backspace
  EXPECT_TRUE(IsControl(uchar));
  EXPECT_FALSE(IsControl(u'\r'));
}

TEST(TokenizerUtilsTest, IsPunctuation) {
  char32_t uchar = U'\U0000002D';
  EXPECT_TRUE(IsPunctuation(uchar));
}

TEST(TokenizerUtilsTest, IsChineseChar) {
  char32_t uchar = U'\U00004F60';   // "你"
  char32_t uchar1 = U'\U00000075';  // "u"
  EXPECT_TRUE(IsChineseChar(uchar));
  EXPECT_FALSE(IsChineseChar(uchar1));
}

TEST(TokenizerUtilsTest, Trim) {
  icu::UnicodeString text("\t[CLS]you are good,[SEP] 你是最棒的[SEP]\t");
  icu::UnicodeString ltrim_text("[CLS]you are good,[SEP] 你是最棒的[SEP]\t");
  icu::UnicodeString rtrim_text("\t[CLS]you are good,[SEP] 你是最棒的[SEP]");
  icu::UnicodeString strip_text("[CLS]you are good,[SEP] 你是最棒的[SEP]");
  EXPECT_EQ(ltrim_text, LTrim(text));
  EXPECT_EQ(rtrim_text, RTrim(text));
  EXPECT_EQ(strip_text, Strip(text));
}

TEST(TokenizerUtilsTest, CleanText) {
  icu::UnicodeString text("\t[CLS]You aRe gOod,[SEP] 你是\t最棒\r的！[SEP]\t");
  icu::UnicodeString expected_output(
      " [CLS]You aRe gOod,[SEP] 你是 最棒 的！[SEP] ");
  auto cleaned_text = CleanText(text);
  EXPECT_EQ(expected_output, cleaned_text);
}

TEST(TokenizerUtilsTest, WhitespaceTokenize) {
  icu::UnicodeString text("[CLS] You aRe gOod , [SEP] 你 是 最 棒 的 ！ [SEP]");
  std::vector<icu::UnicodeString> expected_outputs = {
      "[CLS]", "You", "aRe", "gOod", ",",  "[SEP]", "你",
      "是",    "最",  "棒",  "的",   "！", "[SEP]"};
  auto tokens = (WhitespaceTokenize(text));
  EXPECT_THAT(expected_outputs, ::testing::ContainerEq(tokens));
}

TEST(TokenizerUtilsTest, TokenizeChineseChars) {
  icu::UnicodeString text("\t[CLS]You aRe gOod,[SEP] 你是\t最棒\r的！[SEP]\t");
  icu::UnicodeString expected_output(
      "\t[CLS]You aRe gOod,[SEP]  你  是 \t 最  棒 \r 的 ！[SEP]\t");
  auto res = TokenizeChineseChars(text);

  EXPECT_EQ(expected_output, res);
}

TEST(TokenizerUtilsTest, StripAccents) {
  icu::UnicodeString text("ü");
  icu::UnicodeString expected_output("u");

  auto res = StripAccents(text);
  EXPECT_EQ(expected_output, res);
}

TEST(TokenizerUtilsTest, SplitByPunctuation) {
  icu::UnicodeString text("Are you gOod? 你是最棒的！");
  std::vector<icu::UnicodeString> expected_outputs = {"Are you gOod", "?",
                                                      " 你是最棒的", "！"};
  auto tokens = SplitByPunctuation(text);
  EXPECT_THAT(expected_outputs, ::testing::ContainerEq(tokens));
}

}  // namespace tokenizers
