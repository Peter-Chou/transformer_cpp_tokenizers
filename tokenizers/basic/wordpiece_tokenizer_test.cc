#include "tokenizers/basic/wordpiece_tokenizer.h"

#include "utils/tokenizer_utils.h"

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>
#include <unicode/unistr.h>
#include <unicode/ustream.h>

#include <data.h>
#include <iostream>
#include <memory>

namespace tokenizers {

class WordpieceTokenizerTest : public ::testing::Test {
 protected:
  static void SetUpTestSuite() {
    std::string vocab_file = std::string(data_dir) + "/vocabs/vocab.txt";
    token_id_map_ = new std::unordered_map<icu::UnicodeString, int>();
    LoadVocab(vocab_file, token_id_map_);
    wordpiece_tokenizer_ =
        std::make_unique<WordpieceTokenizer>("[UNK]", token_id_map_);
  }
  static void TearDownTestSuite() { delete token_id_map_; }

  virtual void SetUp() {}
  virtual void TearDown() {}

  static std::unordered_map<icu::UnicodeString, int>* token_id_map_;
  static std::unique_ptr<tokenizers::WordpieceTokenizer> wordpiece_tokenizer_;
};

std::unordered_map<icu::UnicodeString, int>*
    WordpieceTokenizerTest::token_id_map_ = nullptr;
std::unique_ptr<tokenizers::WordpieceTokenizer>
    WordpieceTokenizerTest::wordpiece_tokenizer_ = nullptr;

TEST_F(WordpieceTokenizerTest, Tokenize) {
  icu::UnicodeString token1("unaffable");
  icu::UnicodeString token2("example");
  std::vector<icu::UnicodeString> expected_tokens1 = {"u", "##na", "##ff",
                                                      "##able"};
  std::vector<icu::UnicodeString> expected_tokens2 = {"ex", "##am", "##ple"};
  auto tokens1 = wordpiece_tokenizer_->Tokenize(token1);
  auto tokens2 = wordpiece_tokenizer_->Tokenize(token2);

  EXPECT_THAT(expected_tokens1, ::testing::ContainerEq(tokens1));
  EXPECT_THAT(expected_tokens2, ::testing::ContainerEq(tokens2));
}

}  // namespace tokenizers
