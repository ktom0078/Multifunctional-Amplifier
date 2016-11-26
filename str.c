#include "str.h"


unsigned int StrCmp(const char *a,const char *b) {
	unsigned int eq = 1;

	while(*a && *b) {
		if(*a != *b) {
			eq = 0;
			break;
		}
		a++;
		b++;
	}
	return eq;
}

unsigned int StrLen(const char *str) {
	unsigned int lon = 0;

	while(*str) {
		lon++;
		str++;
	}
	return lon;
}

void StrCopy(const char* from,char* to) {
	while(*from) {
		*to = *from;
		to++;
		from++;
	}
	*to='\0';
}



void StrFillSpace(unsigned int count, char * buff) {
	unsigned int i;

	for(i=0;i<count;i++)
	{
		buff[i]=' ';
	}
	buff[i]='\0';
}
