//gcc -o main main.c ../src/url.c 



#include <stdio.h>
#include "../include/url.h"


int main() {
	URL* myurl;
	
	char** urls;
	
	urls = malloc(15*sizeof(char*));
	
	urls[0] = "http://www.google.com";
	urls[1] = "http://www.google.com/index.php?get=value&and=co#mhwassolldas";
	urls[2] = "http://user:pass@www.google.com:8080";
	urls[3] = "http://user@www.google.com:8080#mhwassolldas";
	urls[4] = "http://user:pass@www.google.com:8080/some/path/index.php?get=value&and=co#mhwassolldas";
	urls[5] = "file:///usr/local";
	urls[6] = "ftp://usr:pass@provider:21/httpdocs/joomla/templates";
	urls[7] = "ftp://usr:pass@127.0.0.1:21/httpdocs/joomla/templates";
	urls[8] = "ftp://[2001:0db8:85a3:0000:0000:8a2e:0370:7334]:21";
	urls[9] = "http://[2001:0db8:85a3:08d3:1319:8a2e:0370:7344]:8080/";
	
	//lets try the parser on broken/invalid urls
	urls[10] = "[2001:0db8:85a3:08d3:1319:8a2e:0370:7344]:8080/";
	urls[11] = "http://:8080/some/path";
	urls[12] = ":8080#bizar";
	urls[13] = "ftp://user:password#bizar";
	urls[14] = "user:password@thehost#bizar";
	
	
	size_t i;
	for(i = 0; i < 15; i++) {
		myurl = calloc(1, sizeof(URL));
		urlstr(urls[i], myurl);
		
		printf("Parsing url: %s\n", urls[i]);
		printf("schema: %s\n", myurl->schema);
		printf("host: %s\n", myurl->address);
		printf("port: %s\n", myurl->port);
		printf("path: %s\n", myurl->path);
		printf("query: %s\n", myurl->query);
		printf("fragment: %s\n", myurl->fragment);
		
		printf("username: %s\n", myurl->username);
		printf("password: %s\n", myurl->password);
		
		printf("----------\n");
		
		fflush(NULL);
		
		urldestroy(myurl);
	}
	
	
	
	
	
	printf("\n\n----------\ntest strurl\n----------\n");
	
	URL uri;
	
	uri.schema = "smtp";
	uri.address = "www.ocsource.tk";
	uri.port = "465";
	
	
	uri.username = "user";
	uri.password = "plaintext";
	
	uri.path = "controllers/index.py";
	
	uri.query = "mail=jojo";
	uri.fragment = "new";
	
	printf("%s\n", strurl(&uri));
	
	
	printf("\n\n----------\ntest urlencode\n----------\n");
	
	
	char* testcase3 = "aä bc&def%gho@ijklmn\n";
	printf("%s\n%s\n", "aä bc&def%gho@ijklmn\\n", urlencode(testcase3, strlen(testcase3)));
	
	
	
	printf("\n\n----------\ntest urldecode\n----------\n");
	
	char* testcase1 = "a%20bc%26def%25gho%40ijklmn%0a";
	char* testcase2 = "some-thing completly@different!öäüp$¨-: +\"*ç%&/(";
	
	printf("%s\n", urldecode(testcase1, strlen(testcase1)));
	
	char* decoded = urlencode(testcase2, strlen(testcase2));
	printf("%s\n%s\n%s\n", testcase2, decoded, urldecode(decoded, strlen(decoded)));
	
	
	printf("\n\n----------\ntest qrytkn\n----------\n");
	
	
	char* testcase4 = "key1=value1%20blah&%40key&key2=value2";
	size_t qrylength;
	QUERY_TOKEN* arr = qrytkn(testcase4, &qrylength);
	
	printf("qrytkn(\"%s\") =>\nquery length: %lu\n", testcase4, qrylength);
	
	
	for(i = 0; i < qrylength; i++) {
		printf("name: %s value: %s\n", arr[i].key, arr[i].value);
	}
	
	
	
	printf("\n\n----------\ntest tknqry\n----------\n");
	
	char* result = tknqry(arr, qrylength);
	printf("%s\n", result);
	
	
	return 0;
}