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

#ifndef BOT2_VIS_BOT_VIS_TOKENIZE_H_
#define BOT2_VIS_BOT_VIS_TOKENIZE_H_

#include <stdio.h>

typedef struct tokenize
{
    // current token
    char *token;

    // position where last token started
    int line, column;

    // position where next token starts
    int nextline, nextcolumn;

    // used to track accurate positions wrt ungetc
    int save_line, save_column;
    int unget_char; // the char that was unget'd, or -1

    // current position of parser (end of last token, usually)
    int in_line, in_column;

    // the current line
    char *line_buffer;
    int  line_pos, line_len;

    char *path;
    FILE *f;

    // do we have a token ready?
    int hasnext;
} tokenize_t;

tokenize_t *tokenize_create(const char *path);
void tokenize_destroy(tokenize_t *t);
int tokenize_next(tokenize_t *t);
int tokenize_peek(tokenize_t *t);

#endif  /* BOT2_VIS_BOT_VIS_TOKENIZE_H_ */
