/*

Copyright © 2019 University of California, Berkeley

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

word_count provides lists of words and associated count

Functional methods take the head of a list as first arg.
Mutators take a reference to a list as first arg.
*/

#include "word_count.h"

/* Basic utilities */

char *new_string(char *str) {
  char *new_str = (char *) malloc(strlen(str) + 1);
  if (new_str == NULL) {
    return NULL;
  }
  return strcpy(new_str, str);
}

int init_words(WordCount **wclist) {
  /* Initialize word count.
     Returns 0 if no errors are encountered
     in the body of this function; 1 otherwise.
  */
  WordCount *list = malloc(sizeof(WordCount));
  if (list == NULL)
  {
    return 1;
  }
  list->count = 0;
  list->word = NULL;
  list->next = NULL;
  *wclist = list;
  return 0;
}

ssize_t len_words(WordCount *wchead) {
  /* Return -1 if any errors are
     encountered in the body of
     this function.
  */
    size_t len = 0;
    WordCount* ptr = wchead;
    while (ptr != NULL)
    {
      len += 1;
      ptr = ptr->next;
    }
    return len;
}

WordCount *find_word(WordCount *wchead, char *word) {
  /* Return count for word, if it exists */
  WordCount *wc = NULL;
  WordCount* ptr = wchead;
  while (ptr != NULL)
  {
    if (strcmp(ptr->word, word) == 0)
    {
      wc = ptr;
      break;
    }
    ptr = ptr->next;
  }
  return wc;
}

int add_word(WordCount **wclist, char *word) {
  /* If word is present in word_counts list, increment the count.
     Otherwise insert with count 1.
     Returns 0 if no errors are encountered in the body of this function; 1 otherwise.
  */
  WordCount *ptr = *wclist;
  int flag = 0;
  if (ptr->word == NULL)
  {
    // empty list
    ptr->word = new_string(word);
    ptr->count = 1;
    ptr->next = NULL;
    return 0;
  }
  while (ptr->next != NULL)
  {
    if (strcmp(ptr->word, word) == 0)
    {
      // word exists, update count
      ptr->count += 1;
      flag = 1;
      break;
    } 
    ptr = ptr->next;
  }
  if (ptr->next == NULL && flag == 0)
  {
    // word not exists, insert into wclist
    ptr->next = malloc(sizeof(WordCount));
    if (ptr->next == NULL)
    {
      return 1;
    }
    ptr->next->word = new_string(word);
    ptr->next->count = 1;
    ptr->next->next = NULL;
  }
  return 0;
}


void fprint_words(WordCount *wchead, FILE *ofile) {
  /* print word counts to a file */
  WordCount *wc;
  for (wc = wchead; wc; wc = wc->next) {
    fprintf(ofile, "%i\t%s\n", wc->count, wc->word);
  }
}
