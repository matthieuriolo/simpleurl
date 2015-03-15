#include "../include/url.h"

char* substr_ptr(char* start, char* end) {
	size_t sublen = (end - start);
	char* tmp = (char*)malloc(sublen + sizeof(char));
	tmp[sublen] = '\0';
	strncpy(tmp, start, sublen);
	
	return tmp;
}


void urlstr(const char* str, URL* url) {
	char* ptr;
	char* endPos = (char*)str + strlen(str);
	char* startPos;
	
	if((ptr = strrchr(str, '#')) != NULL) {
		url->fragment = substr_ptr(ptr + sizeof(char), endPos);
		endPos = ptr;
	}
	
	if((ptr = strrchr(str, '?')) != NULL) {
		url->query = substr_ptr(ptr + sizeof(char), endPos);
		endPos = ptr;
	}
	
	
	if((startPos = strstr(str, "://")) == NULL)
		startPos = (char*)str;
	else {
		url->schema = substr_ptr((char*)str, startPos);
		startPos += 3*sizeof(char);
	}
	
	if((ptr = strchr(startPos, '/')) != NULL) {
		url->path = substr_ptr(ptr, endPos);
		endPos = ptr;
	}
	
	
	if((ptr = strchr(startPos, '@')) != NULL && ptr < endPos) {
		char* tok = strchr(startPos, ':');
		
		if(tok && tok < ptr) {
			url->password = substr_ptr(tok + sizeof(char), ptr);
		}else
			tok = ptr;
		
		url->username = substr_ptr(startPos, tok);
		startPos = ptr + sizeof(char);
	}
	
	if((ptr = strrchr(startPos, ':')) != NULL && ptr < endPos) {
		url->port = substr_ptr(ptr + sizeof(char), endPos);
		endPos = ptr;
	}
	
	//trim the brackets if it is an ipv6 host
	if(endPos[-1] == ']' && startPos[0] == '[') {
		endPos--;
		startPos++;
	}
	
	if(endPos != startPos) 
		url->address = substr_ptr(startPos, endPos);
}


void urldestroy(URL* url) {
	if(url) {
		if(url->schema)
			free(url->schema);
		
		if(url->username)
			free(url->username);
		
		if(url->password)
			free(url->password);
		
		if(url->address)
			free(url->address);
		
		if(url->port)
			free(url->port);
		
		if(url->path)
			free(url->path);
		
		if(url->query)
			free(url->query);
		
		if(url->fragment)
			free(url->fragment);
		
		//free(url);
	}
}

char* strconcat(char* str1, char* str2, char* separator) {
	if(separator)
		str1 = strconcat(str1, separator, NULL);
	
	str1 = realloc(str1, strlen(str1)+strlen(str2) + sizeof(char));
	str1 = strcat(str1, str2);
	
	return str1;
}

char* strurl(const URL* url) {
	char* str = strdup("");
	
	if(url) {
		if(url->schema) {
			str = strconcat(str, url->schema, NULL);
			str = strconcat(str, "://", NULL);
		}
		
		if(url->username)
			str = strconcat(str, url->username, NULL);
		
		if(url->password)
			str = strconcat(str, url->password, ":");
		
		
		if(url->address)
			str = strconcat(str, url->address, url->username || url->password ? "@" : NULL);
		
		if(url->port)
			str = strconcat(str, url->port, ":");
		
		if(url->path)
			str = strconcat(str, url->path, "/");
		
		if(url->query)
			str = strconcat(str, url->query, "?");
		
		if(url->fragment)
			str = strconcat(str, url->fragment, "#");
	}
	
	return str;
}

char* urlencode(const char* str, const size_t lenostr) {
	const int leninc = 32;
	size_t alcstr = leninc;
	size_t lenstr = 0;
	
	char* ptr = (char*)malloc(sizeof(char)*alcstr);
	
	//size_t lenostr = strlen(str);
	
	size_t i;
	for(i = 0; i < lenostr; i++) {
		if(
			str[i] == '.' || 
			str[i] == '-' || 
			str[i] == '_' || 
			str[i] == '~' ||
				(
					(str[i] >= 'a' && str[i] <= 'z') ||
					(str[i] >= 'A' && str[i] <= 'Z') ||
					(str[i] >= '0' && str[i] <= '9')
				)
			) {
			
			if(alcstr<lenstr+1) {
				alcstr += leninc;
				ptr = realloc(ptr, sizeof(char)*alcstr);
			}
			
			ptr[lenstr] = str[i];
			
			lenstr++;
			
			continue;
		}
		
		if(alcstr<lenstr+3) {
			alcstr += leninc;
			ptr = realloc(ptr, sizeof(char)*alcstr);
		}
				
		char buf[10];
		sprintf(buf, "%%%02x", (unsigned char)str[i]);
		
		memcpy(ptr+lenstr, buf, 3);
			
		lenstr+=3;
	}
	
	//cut down the string to his correct length
	ptr = realloc(ptr, sizeof(char)*lenstr+sizeof(char));
	ptr[lenstr] = '\0';
	
	return ptr;
}

char* urldecode(const char* str, const size_t strolen) {
	//size_t strolen = strlen(str);
	size_t strlen = 0;
	size_t i;
	
	const int leninc = 32;
	size_t alcstr = leninc;
	
	char* ptr = malloc(sizeof(char)*alcstr);
	
	for(i = 0; i < strolen; i++) {
		//resize string
		if(alcstr<strlen+1) {
			alcstr += leninc;
			ptr = realloc(ptr, sizeof(char)*alcstr);
		}
			
		if(str[i] == '%') {
			unsigned int t;
			sscanf(str+i, "%%%02x", &t);
			ptr[strlen]=(unsigned char)t;
			i=i+2; //jump over the hex values
			strlen++;
			continue;
		}
		
		ptr[strlen] = str[i];
		strlen++;
	}
	
	//cut down the string to his correct length
	ptr = realloc(ptr, sizeof(char)*strlen+sizeof(char));
	ptr[strlen] = '\0';
	
	return ptr;
}

QUERY_TOKEN* qrytkn(const char* str, size_t* length) {
	/*
	we create a simple unsorted array
	we use incrementation steps for allocating memory
	the array contains objects of the type QUERY_TOKEN
	*/
	const int leninc = 16;
	size_t lenalc = leninc;
	QUERY_TOKEN* arr = malloc(sizeof(QUERY_TOKEN)*lenalc);
	size_t arrlen = 0;
	
	
	
	char* selectorKeyDelimiter = NULL;
	char* selectorLastSeparator = (char*)str;
	
	
	size_t pos, lenstr = strlen(str);
	for(pos = 0; pos < lenstr; pos++) {
		if(str[pos] == '=') {
			selectorKeyDelimiter = (char*)str + pos;
			continue;
		}
		
		if(str[pos] == '&') {
			if(lenalc<arrlen+1) {
				lenalc+=leninc;
				arr = realloc(arr, sizeof(QUERY_TOKEN)*lenalc);
			}
			
			
			if(selectorLastSeparator < selectorKeyDelimiter) {
				arr[arrlen].key = urldecode(selectorLastSeparator, selectorKeyDelimiter - selectorLastSeparator);
				arr[arrlen].value = urldecode(selectorKeyDelimiter + sizeof(char), pos - (selectorKeyDelimiter - str + sizeof(char)));
			}else {
				arr[arrlen].key = urldecode(selectorLastSeparator, pos - (selectorLastSeparator - str));
			}
			
			selectorLastSeparator = (char*)str + pos  + sizeof(char);
			arrlen++;
		}
	}
	
	if(selectorLastSeparator < str + pos) {
		if(lenalc<arrlen+1) {
			lenalc+=leninc;
			arr = realloc(arr, sizeof(QUERY_TOKEN)*lenalc);
		}
		
		
		if(selectorLastSeparator < selectorKeyDelimiter) {
			arr[arrlen].key = urldecode(selectorLastSeparator, selectorKeyDelimiter - selectorLastSeparator);
			arr[arrlen].value = urldecode(selectorKeyDelimiter + sizeof(char), pos - (selectorKeyDelimiter - str + sizeof(char)));
		}else {
			arr[arrlen].key = urldecode(selectorLastSeparator, pos - (selectorLastSeparator - str));
		}
		
		selectorLastSeparator = (char*)str + pos  + sizeof(char);
		arrlen++;
	}
	
	//cut down array
	arr = realloc(arr, sizeof(QUERY_TOKEN)*arrlen);
	
	
	*length = arrlen;
	return arr;
}

char* tknqry(QUERY_TOKEN* arr, const size_t arrlen) {
	const int leninc = 32;
	size_t lenalc = leninc;
	char* str = malloc(sizeof(char)*lenalc);
	size_t lenstr = 0;
	
	if(arrlen) {
		
		char* ptr = urlencode(arr[0].key, strlen(arr[0].key));
		size_t len = strlen(ptr);
		if(lenalc < lenstr+len) {
			lenalc += leninc*ceil(len / leninc);
			str = realloc(str, lenalc);
		}
		
		memcpy(str+lenstr, ptr, len);
		lenstr+=len;
		free(ptr);
		
		
		if(arr[0].value && strlen(arr[0].value)) {
			ptr = urlencode(arr[0].value, strlen(arr[0].value));
			len = strlen(ptr);
			
			if(lenalc < lenstr+len+sizeof(char)) {
				lenalc += leninc*ceil((lenstr+len+sizeof(char)) / leninc);
				str = realloc(str, lenalc);
			}
			
			memcpy(str+lenstr+sizeof(char), ptr, len);
			str[lenstr] = '=';
			
			lenstr+=len + sizeof(char);
			free(ptr);
		}
		
		size_t pos;
		for(pos = 1; pos < arrlen; pos++) {
			ptr = urlencode(arr[pos].key, strlen(arr[pos].key));
			len = strlen(ptr);
			
			if(lenalc < lenstr+len+sizeof(char)) {
				lenalc += leninc*ceil((lenstr+len+sizeof(char)) / leninc);
				str = realloc(str, lenalc);
			}
			
			memcpy(str+lenstr+sizeof(char), ptr, len);
			str[lenstr] = '&';
			
			lenstr+=len + sizeof(char);
			free(ptr);
			
			if(arr[pos].value && strlen(arr[pos].value)) {
				ptr = urlencode(arr[pos].value, strlen(arr[pos].value));
				len = strlen(ptr);
				
				if(lenalc < lenstr+len+sizeof(char)) {
					lenalc += leninc*ceil((lenstr+len+sizeof(char)) / leninc);
					str = realloc(str, lenalc);
				}
				
				
				memcpy(str+lenstr+sizeof(char), ptr, len);
				str[lenstr] = '=';
				
				lenstr += len + sizeof(char);
				free(ptr);
			}
			
		}
	}
	
	//cut down string
	str = realloc(str, lenstr+sizeof(char));
	str[lenstr] = '\0';
	
	return str;
}