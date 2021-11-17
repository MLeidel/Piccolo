#ifndef MYCLIB
#define MYCLIB
/* myclib.h

bool endswith(char* item, char* compr)
bool equals(char *str1, char *str2)
bool equalsIgnoreCase(char *str1, char *str2)
bool startswith(char* item, char* compr)
char *lowercase(char *str)
char *ltrim(char *)
char *removen(char *line)  // replaces ending new-line char with '\0'
char *replace (char *a, const char *b, const char *c, int start, int number)
char *rtrim(char *)
char *strrev(char *str)
char *substr(char *string, int position, int length)
char *today(char* buf)  // returns string of "YYYY/MM/DD" todays date
char *trim(char *s)
char *uppercase(char *str)
int fields(char *str, char *delim)  // char _fields[MAX_C][MAX_L];
int indexof (char* base, char* str)
int lastindexof (char* base, char* str)
FILE * open_for_append(char *fname)  // returns FILE handler or stops with error
FILE * open_for_read(char *fname)
FILE * open_for_write(char *fname)
void readfile(char *buffer, const char *filename)
*/


#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <sys/stat.h>   // stat


#define MAX_C 128   // used for array length or smaller string
#define MAX_L 4096  // used for string lengths

char *strrev(char *str) {
    int i=0, j=0;
    char temp;

    j=strlen(str)-1;
    while(i<j)
    {
        temp=str[j];
        str[j]=str[i];
        str[i]=temp;
        i++;
        j--;
    }
    return str;
}

char *ltrim(char *s) {
    while(isspace(*s)) s++;
    return s;
}

char *rtrim(char *s) {
    char* back = s + strlen(s);
    while(isspace(*--back));
    *(back+1) = '\0';
    return s;
}

char *trim(char *s) {
    return rtrim(ltrim(s));
}

// replace (Haystack, Needle, Replacement, Offset, How Many)
// start = 0 : start from beginning
// number = 0 : replace all targets
char *replace (char *a, const char *b, const char *c, int start, int number) {
    char buf[MAX_L] = {'\0'};
    char bfa[MAX_L] = {'\0'};
    char *p;
    char *s = 0;
    char *ap;
    long lenb = strlen(b);  // length of string to be replaced
    int count = 0;

    strcpy(bfa, a);
    ap = bfa;
    s = ap + start;

    while(1) {
        p = strstr(s, b);
        if (p == NULL) {
            break;
        }
        *p = '\0';
        strcat(buf, ap);  // building output buffer
        strcat(buf, c);  // with leading string and 1st replacement
        ap = p + lenb;  // increment pointer past the target string
        s = ap;  // s's work is done; it becomes ap
        count++;
        if (number != 0 && count >= number) {
            break;
        }
    }

    strcat(buf, ap);  // add on the final segment
    sprintf(a, "%s", buf);
    return a;
}

// Split a delimited string into fields
char _fields[MAX_C][MAX_L];

int fields(char *str, char *delim) {
    int _fields_count = 0;
    char* token = strtok(str, delim);
    //strcpy(_fields[_fields_count++], token);
    while (token != NULL) {
        strcpy(_fields[_fields_count++], token);
        token = strtok(NULL, delim);
    }
    return _fields_count;
}

bool file_exists (char *filename) {
  struct stat   buffer;
  return (stat (filename, &buffer) == 0);
}

FILE * open_for_read(char *fname) {
    FILE *f1;
    if ((f1 = fopen(fname,"rb")) == NULL) {
        printf("\nError trying to open %s\n", fname);
        exit(1);
    }
    return f1;
}

FILE * open_for_append(char *fname) {
    FILE *f1;
    if ((f1 = fopen(fname,"ab")) == NULL) {
        printf("\nError trying to open %s\n", fname);
        exit(1);
    }
    return f1;
}

FILE * open_for_write(char *fname) {
    FILE *f1;
    if ((f1 = fopen(fname,"wb")) == NULL) {
        printf("\nError trying to open %s\n", fname);
        exit(1);
    }
    return f1;
}

void readfile(char *buffer, const char *filename) {
    FILE *f;
    if ((f = fopen(filename,"rb")) == NULL) {
        printf("\nError trying to open %s\n", filename);
        exit(1);
    }
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *string = malloc(fsize + 1);
    fread(string, 1, fsize, f);
    fclose(f);
    string[fsize] = 0;
    strcpy(buffer, string);
    free(string);
}

char* removen(char *line) {
    line[strlen(line) - 1] = '\0';
    return line;
}

char* today(char* buf) {
    time_t rawtime;
    struct tm *info;
    char buffer[80];

    time( &rawtime );

    info = localtime( &rawtime );

    strftime(buffer, 80, "%F", info);
    sprintf(buf, "%s", buffer );
    return buf;
}

bool startswith (char* base, char* str) {
    return (strstr(base, str) - base) == 0;
}

bool endswith (char* base, char* str) {
    int blen = strlen(base);
    int slen = strlen(str);
    return (blen >= slen) && (0 == strcmp(base + blen - slen, str));
}

int indexOf_shift (char* base, char* str, int startIndex) {
    int result;
    int baselen = strlen(base);
    // str should not longer than base
    if (strlen(str) > baselen || startIndex > baselen) {
        result = -1;
    } else {
        if (startIndex < 0) {
            startIndex = 0;
        }
        char* pos = strstr(base+startIndex, str);
        if (pos == NULL) {
            result = -1;
        } else {
            result = pos - base;
        }
    }
    return result;
}

int indexof (char* base, char* str) {
    return indexOf_shift(base, str, 0);
}

int lastindexof (char* base, char* str) {
    int result;
    // str should not longer than base
    if (strlen(str) > strlen(base)) {
        result = -1;
    } else {
        int start = 0;
        int endinit = strlen(base) - strlen(str);
        int end = endinit;
        int endtmp = endinit;
        while(start != end) {
            start = indexOf_shift(base, str, start);
            end = indexOf_shift(base, str, end);

            // not found from start
            if (start == -1) {
                end = -1; // then break;
            } else if (end == -1) {
                // found from start
                // but not found from end
                // move end to middle
                if (endtmp == (start+1)) {
                    end = start; // then break;
                } else {
                    end = endtmp - (endtmp - start) / 2;
                    if (end <= start) {
                        end = start+1;
                    }
                    endtmp = end;
                }
            } else {
                // found from both start and end
                // move start to end and
                // move end to base - strlen(str)
                start = end;
                end = endinit;
            }
        }
        result = start;
    }
    return result;
}


char *substr(char *string, int position, int length) {
   char *p;
   int c;

   p = malloc(length+2);

   if (p == NULL) {
      printf("Unable to allocate memory.\n");
      exit(1);
   }

   for (c = 0; c <= length; c++) {
      *(p+c) = *(string+position-1);
      string++;
   }

   *(p+c) = '\0';

   return p;
}


char *lowercase(char *str) {
    int i, length = strlen(str);

    for(i = 0; i < length; i++)
    {
        if(isalpha(str[i]))
        {
            if(64 < str[i] && str[i] < 91)
                str[i] += 32;
        }
    }

    return str;
}

char *uppercase(char *str) {
    int i, length = strlen(str);

    for(i = 0; i < length; i++)
    {
        if(isalpha(str[i]))
        {
            if(96 < str[i] && str[i] < 123)
                str[i] -= 32;
        }
    }

    return str;
}

bool equals(char *str1, char *str2) {
    return (strcmp(str1, str2) == 0);
}

bool equalsignorecase(char *str1, char *str2) {
    return (strcasecmp(str1, str2) == 0);
}

/*
    Is a specific named argument present
    and what is it's position.
    ac - argc from main function args
    argv - argv from main function args
    arg - the named arg looking for
*/
int is_arg(int ac, char **argv, char *arg) {
    if (ac < 2) {
        return 0;
    }
    for(int x=1; x < ac; x++) {
        if (0 == strcmp(argv[x], arg)) {
            return x;  // return position of arg
        }
    }
    return 0;  // arg not present
}

#endif
