
;; I couldn't make any tokenization. Here is a simple code to just read a keyword from terminal:(

(defun KeywordToken(keyword)

	(cond
			((string-equal keyword "and") (write "KW_AND"))
			((string-equal keyword "or") (write "KW_OR"))
			((string-equal keyword "not") (write "KW_NOT"))
			((string-equal keyword "equal") (write "KW_EQUAL"))
			((string-equal keyword "less") (write "KW_LESS"))
			((string-equal keyword "nil") (write "KW_NIL"))
			((string-equal keyword "list") (write "KW_LIST"))
			((string-equal keyword "append") (write "KW_APPEND"))
			((string-equal keyword "concat") (write "KW_CONCAT"))
			((string-equal keyword "set") (write "KW_SET"))
			((string-equal keyword "deffun") (write "KW_DEFFUN"))
			((string-equal keyword "for") (write "KW_FOR"))
			((string-equal keyword "if") (write "KW_IF"))
			((string-equal keyword "exit") (write "KW_EXIT"))
			((string-equal keyword "load") (write "KW_LOAD"))
			((string-equal keyword "disp") (write "KW_DISP"))
			((string-equal keyword "true") (write "KW_TRUE"))
			((string-equal keyword "false") (write "KW_FALSE"))
			(t (write "INVALID"))
		)
   
)

(setq token_read (read))
(write (KeywordToken token_read))



















//> check-keyword
static TokenType checkKeyword(int start, int length,
    const char* rest, TokenType type) {
  if (scanner.current - scanner.start == start + length &&
      memcmp(scanner.start + start, rest, length) == 0) {
    return type;
  }

  return TOKEN_IDENTIFIER;
}
//< check-keyword
//> identifier-type
static TokenType identifierType() {
//> keywords
  switch (scanner.start[0]) {
    case 'a': return checkKeyword(1, 2, "nd", TOKEN_AND);
    case 'c': return checkKeyword(1, 4, "lass", TOKEN_CLASS);
    case 'e': return checkKeyword(1, 3, "lse", TOKEN_ELSE);
//> keyword-f
    case 'f':
      if (scanner.current - scanner.start > 1) {
        switch (scanner.start[1]) {
          case 'a': return checkKeyword(2, 3, "lse", TOKEN_FALSE);
          case 'o': return checkKeyword(2, 1, "r", TOKEN_FOR);
          case 'u': return checkKeyword(2, 1, "n", TOKEN_FUN);
        }
      }
      break;
//< keyword-f
    case 'i': return checkKeyword(1, 1, "f", TOKEN_IF);
    case 'n': return checkKeyword(1, 2, "il", TOKEN_NIL);
    case 'o': return checkKeyword(1, 1, "r", TOKEN_OR);
    case 'p': return checkKeyword(1, 4, "rint", TOKEN_PRINT);
    case 'r': return checkKeyword(1, 5, "eturn", TOKEN_RETURN);
    case 's': return checkKeyword(1, 4, "uper", TOKEN_SUPER);
//> keyword-t
    case 't':
      if (scanner.current - scanner.start > 1) {
        switch (scanner.start[1]) {
          case 'h': return checkKeyword(2, 2, "is", TOKEN_THIS);
          case 'r': return checkKeyword(2, 2, "ue", TOKEN_TRUE);
        }
      }
      break;
//< keyword-t
    case 'v': return checkKeyword(1, 2, "ar", TOKEN_VAR);
    case 'w': return checkKeyword(1, 4, "hile", TOKEN_WHILE);

//< keywords
  return TOKEN_IDENTIFIER;
  }