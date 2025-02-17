// -*- mode: c -*-
// vim: set filetype=c :

/*
 * This file is part of bot2-vis.
 *
 * bot2-vis is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * bot2-vis is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with bot2-vis. If not, see <https://www.gnu.org/licenses/>.
 */

#include "tokenize.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

static const char single_char_toks[] = "();\",:\'[].";
static const char op_chars[] = "!~<>=&|^%*+=/";

#define MAX_TOKEN_LEN 1024
#define MAX_LINE_LEN 1024

static char unescape(char c) {
  switch (c) {
    case 'n':
      return 10;
    case 'r':
      return 13;
    case 't':
      return 9;
    default:
      return c;
  }
}

tokenize_t* tokenize_create(const char* path) {
  tokenize_t* t = (tokenize_t*)calloc(1, sizeof(tokenize_t));
  t->path = strdup(path);
  t->f = fopen(path, "r");

  if (t->f == NULL) {
    free(t->path);
    free(t);
    return NULL;
  }

  t->token = (char*)calloc(1, MAX_TOKEN_LEN);
  t->line_buffer = (char*)calloc(1, MAX_LINE_LEN);

  t->in_line = 1;
  t->in_column = 0;
  t->line = -1;
  t->column = -1;
  t->hasnext = 0;

  t->line_pos = 0;
  t->line_len = 0;

  t->save_line = 0;
  t->save_column = 0;
  t->unget_char = -1;  // no currently saved char.

  return t;
}

void tokenize_destroy(tokenize_t* t) {
  free(t->line_buffer);
  free(t->token);

  fclose(t->f);
  free(t->path);
  free(t);
}

/** get the next character, counting line numbers, and columns. **/
static int tokenize_next_char(tokenize_t* t) {
  t->save_line = t->in_line;
  t->save_column = t->in_column;

  int c;

  // return an unget() char if one is stored
  if (t->unget_char >= 0) {
    c = t->unget_char;
    t->unget_char = -1;
  } else {
    // otherwise, get a character from the input stream

    // read the next line if we're out of data
    if (t->line_pos == t->line_len) {
      // reminder: fgets will store the newline in the buffer.
      if (fgets(t->line_buffer, MAX_LINE_LEN, t->f) == NULL) {
        return EOF;
      }
      t->line_len = strlen(t->line_buffer);
      t->line_pos = 0;
    }

    c = t->line_buffer[t->line_pos++];
  }

  // count lines
  if (c == '\n') {
    t->in_line++;
    t->in_column = 0;
  } else {
    t->in_column++;
  }

  return c;
}

static int tokenize_ungetc(tokenize_t* t, int c) {
  t->in_line = t->save_line;
  t->in_column = t->save_column;
  t->unget_char = c;

  return 0;
}

static void tokenize_flush_line(tokenize_t* t) {
  int c;
  do {
    c = tokenize_next_char(t);
  } while (c != EOF && c != '\n');
}

/** chunkify tokens. **/
static int tokenize_next_internal(tokenize_t* t) {
  int c;
  int pos = 0;  // output char pos

skip_white:
  c = tokenize_next_char(t);
  if (c == EOF) {
    return EOF;
  }
  if (isspace(c)) {
    goto skip_white;
  }

  // a token is starting. mark its position.
  t->line = t->in_line;
  t->column = t->in_column;

  // is a character literal?
  if (c == '\'') {
    t->token[pos++] = c;
    c = tokenize_next_char(t);
    if (c == '\\') {
      c = unescape(tokenize_next_char(t));
    }
    if (c == EOF) {
      return -4;
    }
    t->token[pos++] = c;
    c = tokenize_next_char(t);
    if (c != '\'') {
      return -5;
    }
    t->token[pos++] = c;
    goto end_tok;
  }

  // is a string literal?
  if (c == '\"') {
    int escape = 0;

    // add the initial quote
    t->token[pos++] = c;

    // keep reading until close quote
    while (1) {
      if (pos >= MAX_TOKEN_LEN) {
        return -2;
      }

      c = tokenize_next_char(t);

      if (escape) {
        escape = 0;
        c = unescape(c);

        continue;
      }

      if (c == '\"') {
        t->token[pos++] = c;
        goto end_tok;
      }
      if (c == '\\') {
        escape = 1;
        continue;
      }

      t->token[pos++] = c;
    }
    goto end_tok;
  }

  // is an operator?
  if (strchr(op_chars, c) != NULL) {
    while (strchr(op_chars, c) != NULL) {
      if (pos >= MAX_TOKEN_LEN) {
        return -2;
      }
      t->token[pos++] = c;
      c = tokenize_next_char(t);
    }
    tokenize_ungetc(t, c);
    goto end_tok;
  }

  if ('-' == c) {
    t->token[pos++] = c;
    c = tokenize_next_char(t);
  }

  // is it a numeric constant?
  if (isdigit(c)) {
    while (isdigit(c)) {
      if (pos >= MAX_TOKEN_LEN) {
        return -2;
      }
      t->token[pos++] = c;
      c = tokenize_next_char(t);
    }
    if ('.' == c) {
      if (pos >= MAX_TOKEN_LEN) {
        return -2;
      }
      t->token[pos++] = c;
      c = tokenize_next_char(t);
      while (isdigit(c)) {
        if (pos >= MAX_TOKEN_LEN) {
          return -2;
        }
        t->token[pos++] = c;
        c = tokenize_next_char(t);
      }
    }
    goto end_tok;
  }

  // otherwise, all tokens are alpha-numeric blobs
in_tok:
  if (pos >= MAX_TOKEN_LEN) {
    return -2;
  }

  t->token[pos++] = c;

  if (strchr(single_char_toks, c) != NULL) {
    goto end_tok;
  }

  c = tokenize_next_char(t);
  if (strchr(single_char_toks, c) != NULL || strchr(op_chars, c) != NULL) {
    tokenize_ungetc(t, c);
    goto end_tok;
  }

  if (!isspace(c)) {
    goto in_tok;
  }

end_tok:
  t->token[pos] = 0;

  return pos;
}

/** remove comments **/
int tokenize_next(tokenize_t* t) {
  int res;

  if (t->hasnext) {
    t->hasnext = 0;
    return 0;
  }

restart:
  res = tokenize_next_internal(t);

  if (res == EOF) {
    return EOF;
  }

  // block comment?
  if (!strncmp(t->token, "/*", 2)) {
    while (1) {
      res = tokenize_next_internal(t);
      if (res == EOF) {
        return -10;
      }
      int len = strlen(t->token);
      if (len >= 2 && !strncmp(&t->token[len - 2], "*/", 2)) {
        break;
      }
    }
    goto restart;
  }

  // end of line comment? If so, instantly consume the rest of this line.
  if (!strncmp(t->token, "//", 2)) {
    tokenize_flush_line(t);
    goto restart;
  }

  return res;
}

int tokenize_peek(tokenize_t* t) {
  int res = 0;

  if (t->hasnext) {
    return res;
  }

  res = tokenize_next(t);
  t->hasnext = 1;

  return res;
}
