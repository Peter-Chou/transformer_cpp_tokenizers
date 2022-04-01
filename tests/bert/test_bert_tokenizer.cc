// #include "tokenizers/"
#include "fundamental/fundamental_tokenizer.h"
#include "tokenizers/bert/bert_tokenizer.h"

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include <data.h>
#include <memory>

using namespace tokenizers;

class BertTokenizerTest : public ::testing::Test {
 protected:
  static void SetUpTestSuite() {
    FundamentalTokenizer::Options f_options{};
    BertTokenizer::Options bert_options;
    bert_options.f_options = f_options;
    bert_options.vocab_file = std::string(data_dir) + "/vocabs/vocab.txt";
    fundamental_tokenizer_ =
        std::move(BertTokenizer::CreateBertTokenizer(bert_options));
    bert_tokenizer_ = static_cast<BertTokenizer*>(fundamental_tokenizer_.get());
  }
  static void TearDownTestSuite() {}

  virtual void SetUp() {}
  virtual void TearDown() {}

  static std::unique_ptr<tokenizers::FundamentalTokenizer>
      fundamental_tokenizer_;
  static tokenizers::BertTokenizer* bert_tokenizer_;
};

std::unique_ptr<tokenizers::FundamentalTokenizer>
    BertTokenizerTest::fundamental_tokenizer_ = nullptr;
tokenizers::BertTokenizer* BertTokenizerTest::bert_tokenizer_ = nullptr;

TEST_F(BertTokenizerTest, test_encode) {
  std::string text("谢谢你哦！我知道拉，haha ü去吃饭咯");
  std::vector<int> expected_output = {101,  6468, 6468, 872,  1521,  8013, 2769,
                                      4761, 6887, 2861, 8024, 11643, 8778, 100,
                                      1343, 1391, 7649, 1492, 102,   0};
  auto output = bert_tokenizer_->Encode(&text, nullptr, 20);

  EXPECT_THAT(expected_output, ::testing::ContainerEq(output.input_ids));
}