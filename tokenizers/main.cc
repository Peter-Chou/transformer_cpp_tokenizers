#include "bert/bert_tokenizer.h"
#include "tokenizer_utils.h"

#include <unicode/brkiter.h>
#include <unicode/chariter.h>
#include <unicode/locid.h>
#include <unicode/schriter.h>
#include <unicode/unistr.h>
#include <unicode/ustream.h>
#include <unicode/utext.h>
#include <unicode/utypes.h>

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

using namespace tokenizers;

int main() {
  // std::string vocab_file =
  //     "/home/peter/projects/ernie_deploy/ernie_tokenizer/vocab.txt";
  // BertTokenizer tokenizer = BertTokenizer(vocab_file);
  // auto vocab_ptr = tokenizer.getMutableVocab();
  // printf("[MASK] id : %d\n", (*vocab_ptr)["[MASK]"]);
  icu_66::UnicodeString test("you are good, 你是最棒的。");
  std::cout << test << std::endl;
  // UErrorCode err = U_ZERO_ERROR;
  // std::unique_ptr<icu_66::BreakIterator> iter(
  //     icu_66::BreakIterator::createCharacterInstance(
  //         icu_66::Locale::getDefault(), err));
  // iter->setText(test);
  // while (iter->next() != icu_66::BreakIterator::DONE) {
  //   // auto = iter->
  //   UText text;
  //   iter->getUText(&text, err);
  //   std::cout << text.chunkContents << std::endl;

  // std::cout << iter-> << std::endl;
  // }

  icu_66::StringCharacterIterator iter(test);
  while (iter.hasNext()) {
    // icu_66::UnicodeString token;
    // iter.getText(token);
    // iter.getIndex()
    std::cout << iter.getIndex() << std::endl;
    iter.next();
  }
}
