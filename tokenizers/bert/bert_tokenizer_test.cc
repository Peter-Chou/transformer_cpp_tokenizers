#include "tokenizers/bert/bert_tokenizer.h"

#include "fundamental/fundamental_tokenizer.h"

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include <data.h>
#include <memory>

namespace tokenizers {

class BertTokenizerTest : public ::testing::Test {
 protected:
  static void SetUpTestSuite() {
    BertTokenizer::Options bert_options;
    bert_options.vocab_file = std::string(data_dir) + "/vocabs/vocab.txt";
    bert_tokenizer_ = BertTokenizer::CreateTokenizer(bert_options);
  }
  static void TearDownTestSuite() {}

  virtual void SetUp() {}
  virtual void TearDown() {}

  static std::unique_ptr<BertTokenizer> bert_tokenizer_;
};
std::unique_ptr<BertTokenizer> BertTokenizerTest::bert_tokenizer_ = nullptr;

TEST_F(BertTokenizerTest, test_Encode) {
  std::string text("谢谢你哦！我知道拉，haha ü去吃饭咯");
  std::vector<int> expected_output = {101,  6468, 6468, 872,  1521,  8013, 2769,
                                      4761, 6887, 2861, 8024, 11643, 8778, 100,
                                      1343, 1391, 7649, 1492, 102,   0};
  auto output = bert_tokenizer_->Encode(&text, nullptr, 20);

  EXPECT_THAT(expected_output, ::testing::ContainerEq(output.input_ids));
}

TEST_F(BertTokenizerTest, test_BatchEncode) {
  std::vector<std::string> texts = {"谢谢你哦！我知道拉，haha ü去吃饭咯",
                                    "谢谢你哦！我知道拉，haha ü去吃饭咯"};
  std::vector<int> expected_output = {101,  6468, 6468, 872,  1521,  8013, 2769,
                                      4761, 6887, 2861, 8024, 11643, 8778, 100,
                                      1343, 1391, 7649, 1492, 102,   0};
  auto outputs = bert_tokenizer_->BatchEncode(&texts, nullptr, 20);
  for (const auto& output : outputs) {
    EXPECT_THAT(expected_output, ::testing::ContainerEq(output.input_ids));
  }
}

}  // namespace tokenizers
