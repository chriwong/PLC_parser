# SUBC Parser
## A lexical analyzer, and a recursive descent parser for a (sort of) subset of C language called SUBC

### Tricky Tokens
* Quotes
  * Must be single quote, followed by anything but a single quote, followed by a single quote
  * regex '.'
* Strings
  * Must be double quotes, followed by any number of characters other than double quotes, followed by double quotes
  * regex "[^"]*"
* Colon
  * Can be just colon, colon-equals, or colon-equals-colon
  * : := :=:
* Inequality Signs
  * For a token beginning with less-than, can be less-than, less-than-equals, or less-than-greater-than
  * < <= <>
  * For a token beginning with greater-than, can be greater-than or greater-than-equals
  * > >=
* Dot
  * Can be dot or dot-dot
  * . ..
* Other symbols
  * Plus, minus, star, forward-slash, semicolon, comma, open/close parentheses, and open/close braces are all valid
  * + - * / ; , ( { ) }
* Identifiers
  * A valid identifier begins with a letter underscore and is followed by any number of letters, numbers, or underscores
  * regex ([A-Z]|[a-z]|_)([a-z]|[A-Z]|[0-9]|_)*