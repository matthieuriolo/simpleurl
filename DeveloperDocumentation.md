#Overview of the functions

# Structures #

```
typedef struct {
	char* schema;
	
	char* username;
	char* password;
	
	char* address;
	char* port;
	
	char* path;
	char* query;
	char* fragment;
} URL;


typedef struct {
	char* key;
	char* value;
} QUERY_TOKEN;
```

All pointers have to be freed by the developer. For the object URL exists a function called _urldestroy_ that provides this functionality

# Functions #

## urlstr ##
**void urlstr(const char`*` str, URL`*` url)**

Copies all URL parts out of the string _str_. The parameter _url_ must be an alloced structure of URL that does not contain any information. The function _urlstr_ fills up the object URL with all found sequences.


## strurl ##
**char`*` strurl(const URL`*` url)**

The function _strurl_ concates all sequences of the URL to one string.


## urldestroy ##
**void urldestroy(URL`*` url)**

Gives all set memory in URL  free. The object _url_ must be destroyed be the user.

## urlencode ##
**char`*` urlencode(const char`*` str, const size\_t lenostr)**

Encodes all special chars to hexadecimal (no character encoding done. You have to use UTF-8 yourself). The parameter _lenostr_ describes how many characters should be read in from _str_

## urldecode ##
**char`*` urldecode(const char`*` str, const size\_t strolen)**

Decodes all hexadecimal to special chars (no character encoding done. You have to use UTF-8 yourself). The parameter _strolen_ describes how many characters should be read in from _str_

## qrytkn ##
**QUERY\_TOKEN`*` qrytkn(const char`*` str, size\_t`*` length)**

The function _qrytkn_ separates the string _str_ with the sign "&" and copies the sequence till the sign "=" to the _name_ property of the object _QUERY\_TOKEN_. The resulting array is returned. Mind, the array is unsorted and must be freed. The parameter _length_ will be reset to the number of elements in the array. The function _qrytkn_ uses urldecode() on each value before copying.

## tknqry ##
**char`*` tknqry(QUERY\_TOKEN`*` arr, const size\_t arrlen)**

All elements in _arr_ gets concated to one string. No sign ? set in front of the while text. The function _tknqry_ uses urlencode() on each value before copying.
