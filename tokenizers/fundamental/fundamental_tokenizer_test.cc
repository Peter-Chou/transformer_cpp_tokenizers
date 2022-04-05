#include "tokenizers/fundamental/fundamental_tokenizer_test.h"

#include "fundamental/fundamental_tokenizer.h"

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>
#include <unicode/unistr.h>
#include <unicode/ustream.h>

#include <iostream>
#include <memory>

namespace tokenizers {

class FundamentalTokenizerTest : public ::testing::Test {
 protected:
  static void SetUpTestSuite() {
    FundamentalTokenizer::Options options{};
    tokenizer_ = std::make_unique<DummyTokenizer>(options);
  }
  static void TearDownTestSuite() {}

  virtual void SetUp() {}
  virtual void TearDown() {}

  static std::unique_ptr<tokenizers::DummyTokenizer> tokenizer_;
};

std::unique_ptr<tokenizers::DummyTokenizer>
    FundamentalTokenizerTest::tokenizer_ = nullptr;

TEST_F(FundamentalTokenizerTest, SplitBySpecialToken) {
  icu::UnicodeString text("\t[CLS]You aRe gOod,[SEP] 你是\t最棒\r的！[SEP]\t");
  auto sub_texts = tokenizer_->SplitBySpecialToken(text);
  std::vector<icu::UnicodeString> expected_outputs = {
      "\t",    "[CLS]", "You aRe gOod,", "[SEP]", " 你是\t最棒\r的！",
      "[SEP]", "\t"};
  EXPECT_THAT(expected_outputs, ::testing::ContainerEq(sub_texts));
}

}  // namespace tokenizers
