#ifndef __NRELAIS__COMMON__WALK__URL__
#define __NRELAIS__COMMON__WALK__URL__ 1

/*

a real simple library for parsing urls

*/

#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <math.h>

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

/* loads everything into the URL structure */
void urlstr(const char* str, URL* url);

/* concates all pieces to a string */
char* strurl(const URL* url);

/* destroys an URL object */
void urldestroy(URL* url);



/* some helpers */
char* urlencode(const char* str, const size_t lenostr);
char* urldecode(const char* str, const size_t strolen);


typedef struct {
	char* key;
	char* value;
} QUERY_TOKEN;

/* split up a text into his pieces (the pieces are already decoded!)
can be reused for post request
str format: name&name=value
*/
QUERY_TOKEN* qrytkn(const char* str, size_t* length);

/* concates a list of tokens to a query (url encode is involved) */
char* tknqry(QUERY_TOKEN* arr, const size_t arrlen);


//some helpers makros

//an url should contain his own copy
#define URLSET(URI, NAME, VALUE) \
free(URI->NAME); \
URI->NAME = strdup(VALUE);



#endif