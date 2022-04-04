#include "tokenizers/fundamental/fundamental_tokenizer.h"

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>
#include <unicode/ustream.h>

#include <iostream>

namespace tokenizers {

class FundamentalTokenizerTest : public ::testing::Test {
 protected:
  static void SetUpTestSuite() {
    // std::string vocab_file = std::string(data_dir) + "/vocabs/vocab.txt";
    // token_id_map_ = new std::unordered_map<icu::UnicodeString, int>();
    // LoadVocab(vocab_file, token_id_map_);
    // wordpiece_tokenizer_ =
    //     std::make_unique<WordpieceTokenizer>("[UNK]", token_id_map_);
  }
  static void TearDownTestSuite() {}

  virtual void SetUp() {}
  virtual void TearDown() {}

  // static std::unordered_map<icu::UnicodeString, int>* token_id_map_;
  // static std::unique_ptr<tokenizers::WordpieceTokenizer> wordpiece_tokenizer_;
};

  // std::unordered_map<icu::UnicodeString, int>*
  //     FundamentalTokenizerTest::token_id_map_ = nullptr;
  // std::unique_ptr<tokenizers::WordpieceTokenizer>
  //     FundamentalTokenizerTest::wordpiece_tokenizer_ = nullptr;

  TEST_F(FundamentalTokenizerTest, Tokenize) {
  }

}  // namespace tokenizers
