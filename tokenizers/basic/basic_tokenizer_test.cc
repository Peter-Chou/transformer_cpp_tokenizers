#include "tokenizers/basic/basic_tokenizer.h"

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>
#include <unicode/unistr.h>

#include <memory>

namespace tokenizers {

class BasicTokenizerTest : public ::testing::Test {
 protected:
  static void SetUpTestSuite() {
    special_tokens_ = new std::unordered_set<icu::UnicodeString>();
    special_tokens_->insert("[CLS]");
    special_tokens_->insert("[SEP]");
    special_tokens_->insert("[UNK]");
    special_tokens_->insert("[MASK]");

    basic_tokenizer_ = std::make_unique<BasicTokenizer>(
        /*do_lower_case=*/true, /*tokenize_chinese_chars=*/true,
        /*strip_accents*/ false, special_tokens_);
  }
  static void TearDownTestSuite() { delete special_tokens_; }

  virtual void SetUp() {}
  virtual void TearDown() {}

  static std::unordered_set<icu::UnicodeString>* special_tokens_;
  static std::unique_ptr<tokenizers::BasicTokenizer> basic_tokenizer_;
};

std::unique_ptr<tokenizers::BasicTokenizer>
    BasicTokenizerTest::basic_tokenizer_ = nullptr;
std::unordered_set<icu::UnicodeString>* BasicTokenizerTest::special_tokens_ =
    nullptr;

TEST_F(BasicTokenizerTest, Tokenize) {
  icu::UnicodeString text("[CLS] you are good , [SEP] 你是最棒的。 [SEP] 。");
  std::vector<icu::UnicodeString> expected_output = {
      "[CLS]", "you", "are", "good", ",",  "[SEP]", "你",
      "是",    "最",  "棒",  "的",   "。", "[SEP]", "。"};
  auto tokens = basic_tokenizer_->Tokenize(text);
  EXPECT_THAT(expected_output, ::testing::ContainerEq(tokens));
}

}  // namespace tokenizers
