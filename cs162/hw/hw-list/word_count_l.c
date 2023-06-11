/*
 * Implementation of the word_count interface using Pintos lists.
 *
 * You may modify this file, and are expected to modify it.
 */

/*
 * Copyright © 2021 University of California, Berkeley
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */



#include "list.h"
#ifndef PINTOS_LIST
#error "PINTOS_LIST must be #define'd when compiling word_count_l.c"
#endif
#include "word_count.h"

char *new_string(char *str) {
  char *new_str = (char *) malloc(strlen(str) + 1);
  if (new_str == NULL) {
    return NULL;
  }
  return strcpy(new_str, str);
}

void init_words(word_count_list_t* wclist) { 
  list_init(wclist);
  return;
}

size_t len_words(word_count_list_t* wclist) {
  return list_size(wclist);
}

word_count_t* find_word(word_count_list_t* wclist, char* word) {
  struct list_elem *e;
  for (e = list_begin(wclist); e != list_end(wclist); e = list_next(e)) {
    word_count_t* wc = list_entry(e, word_count_t, elem);
    if (strcmp(wc->word, word) == 0) {
      return wc;
    }
  }
  return NULL;
}

word_count_t* add_word(word_count_list_t* wclist, char* word) {
  struct list_elem *e = list_begin(wclist);
  int flag = 0;
  word_count_t* wc;
  for (e = list_begin(wclist); e != list_end(wclist); e = list_next(e)) {
    wc = list_entry(e, word_count_t, elem);
    if (strcmp(wc->word, word) == 0) {
      // word in word_count update word_count
      wc->count += 1;
      flag = 1;
    }
  }
  if (flag == 0) {
    // word not in word_count, insert it
    wc = malloc(sizeof(word_count_t));
    wc->count = 1;
    wc->word = new_string(word);
    list_insert(list_end(wclist), &wc->elem);
  }
  return wc;
}

void fprint_words(word_count_list_t* wclist, FILE* outfile) { 
  struct list_elem* e;
  for(e = list_begin(wclist); e != list_end(wclist); e = e->next) {
    word_count_t* node = list_entry(e, word_count_t, elem);
    fprintf(outfile, "%8d\t%s\n", node->count, node->word);
  }
}

static bool less_list(const struct list_elem* ewc1, const struct list_elem* ewc2, void* aux) {
  word_count_t* word1 = list_entry(ewc1, word_count_t, elem);
  word_count_t* word2 = list_entry(ewc2, word_count_t, elem);
  if(word1->count < word2->count) {
    return true;
  } else if(word1->count == word2->count) {
    if(strcmp(word1->word, word2->word) <= 0)
      return true;
  }
  return false;
}

void wordcount_sort(word_count_list_t* wclist,
                    bool less(const word_count_t*, const word_count_t*)) {
  list_sort(wclist, less_list, less);
}
