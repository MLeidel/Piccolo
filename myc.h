#ifndef MYC
#define MYC
/* myc.h

STRING FUNCTIONS
bool endswith (char* base, char* str)
bool equals(char *str1, char *str2)
bool equalsignorecase(char *str1, char *str2)
bool startswith (char* base, char* str)
char * replace (char *a, const char *b, const char *c, int start, int number)
char *lowercase(char *str)
char *ltrim(char *s)
char *rtrim(char *s)
char *strrev(char *str)
char *substr(char *string, int position, int length)
char *trim(char *s)
char *uppercase(char *str)
char* removen(char *line)   // see also rtrim()
char* urlencode(char* originalText)
int charinx(char* base, char c)
int indexof (char* base, char* str)
int lastindexof (char* base, char* str)

ARRAY FUNCTIONS
array_of_strings aos_allocate(int col, int len)
int aos_fields(array_of_strings aos, char *str, char *delim)
void aos_cleanup(array_of_strings aos)
void aos_display(array_of_strings aos)

FILE & PATH FUNCTIONS
bool file_exists (char *filename)
FILE * open_for_append(char *fname)
FILE * open_for_read(char *fname)
FILE * open_for_write(char *fname)
void readfile(char *buffer, const char *filename)

DATE FUNCTIONS
char* today()

OTHER FUNCTIONS
int is_arg(int ac, char **argv, char *arg)
-----------------------------------------
--------- Helpfull gcc functions --------
char *realpath(const char *restrict path, char *restrict resolved_path);
*/

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <sys/stat.h>

#define MAX_L 4096  // used for default string lengths **
#define ARRSIZE(x)  (sizeof(x) / sizeof((x)[0]))  // find len of an array


char *strrev(char *str) {
    int i=0, j=0;
    char temp;

    j=strlen(str)-1;
    while(i<j) {
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

/*  replace (Haystack, Needle, Replacement, Offset, How Many)
    start = 0 : start from beginning
    number = 0 : replace all targets */
char * replace (char *a, const char *b, const char *c, int start, int number) {
    static char buf[MAX_L];
    char bfa[MAX_L] = {'\0'};
    char *p;
    char *s = 0;
    char *ap;
    long lenb = strlen(b);  // length of string to be replaced
    int count = 0;

    assert(lenb < MAX_L);
    assert(strlen(a) < MAX_L);
    assert(start < MAX_L);

    buf[0] = '\0';  // reset static string

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
    return buf;
}

/* For a 2d array_of_strings ================================
  Example:
    char line[100] = "Michael, David, leidel, CEO,cool";

    array_of_strings in = aos_allocate(5, 64);
    aos_fields(in, line, ",");  // parses delimited fields into array
    #aos_display(in); // in.fields[n]
    aos_cleanup(in);
*/

typedef struct array_of_strings {
    int max_row;    // max lenght of a field
    int max_col;    // number of fields
    char ** fields; // array of fields (char arrays or strings)
} array_of_strings;

array_of_strings aos_allocate(int col, int len) {
    /*  initialize variables and allocate memory to an
        array_of_strings struct */
    array_of_strings aos;
    aos.max_row = len;
    aos.max_col = col;
    aos.fields = malloc(aos.max_col * sizeof(char*));
    for(int x=0; x < aos.max_col; x++) {
        aos.fields[x] = malloc(aos.max_row * sizeof(char));
    }
    return aos;
}
int aos_fields(array_of_strings aos, char *str, char *delim) {
    int finx = 0;
    char * found;

    while( (found = strsep(&str, delim)) != NULL )
        strcpy(aos.fields[finx++], trim(found));
    return finx;
}
void aos_display(array_of_strings aos) {
    int x;
    for(x=0; x < aos.max_col; x++) {
        printf("%s\n", aos.fields[x]);
    }
}
void aos_cleanup(array_of_strings aos) {
    /* free each column's data then free the column pointer's */
    for(int col=0; col < aos.max_col; col++) {
        free(aos.fields[col]);
    }
    free(aos.fields);
}

/*================= END array_of_strings ====================*/

bool file_exists (char *filename) {
  struct stat   buffer;
  return (stat (filename, &buffer) == 0);
}

FILE * open_for_read(char *fname) {
    FILE *f1;
    if ((f1 = fopen(fname,"rb")) == NULL) {
        printf("\nError trying to open %s\n\n", fname);
        exit(1);
    }
    return f1;
}

FILE * open_for_append(char *fname) {
    FILE *f1;
    if ((f1 = fopen(fname,"ab")) == NULL) {
        printf("\nError trying to open %s\n\n", fname);
        exit(1);
    }
    return f1;
}

FILE * open_for_write(char *fname) {
    FILE *f1;
    if ((f1 = fopen(fname,"wb")) == NULL) {
        printf("\nError trying to open %s\n\n", fname);
        exit(1);
    }
    return f1;
}

void readfile(char *buffer, const char *filename) {
    FILE *f;
    if ((f = fopen(filename,"rb")) == NULL) {
        printf("\nError trying to open %s\n\n", filename);
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

char* removen(char *line) {  // see also rtrim()
    // Just want to remove very last character of a string
    line[strlen(line) - 1] = '\0';
    return line;
}

char* today() {
    time_t rawtime;
    struct tm *info;
    static char buffer[80];

    time( &rawtime );
    info = localtime( &rawtime );
    strftime(buffer, 80, "%F", info);
    return buffer;
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
    // indexof(haystack, needle);
    // find index of substring in a string
    return indexOf_shift(base, str, 0);
}

int charinx(char* base, char c) {
    // indexof(haystack, needle);
    // find index of a character in a string
    char ctos[2] = {'\0'};
    ctos[0] = c;
    return indexof(base, ctos);
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
   static char p[MAX_L] = {"\0"};
   int c;
   int len = strlen(string);

   assert(len < MAX_L);
   assert(position + length < len);

   if (length == 0) {  // from position to end of string

      return string + position;

   }

   for (c = 0; c < length; c++) {  // return position for length
      *(p+c) = *(string+position);
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
    and what is it's arg index.
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

char* urlencode(char* originalText) {
    static char encodedText[MAX_L] = {"\0"};

    char *hex = "0123456789abcdef";

    int pos = 0;
    for (int i = 0; i < strlen(originalText); i++) {
        if (('a' <= originalText[i] && originalText[i] <= 'z')
            || ('A' <= originalText[i] && originalText[i] <= 'Z')
            || ('0' <= originalText[i] && originalText[i] <= '9')) {
                encodedText[pos++] = originalText[i];
            } else {
                encodedText[pos++] = '%';
                encodedText[pos++] = hex[originalText[i] >> 4];
                encodedText[pos++] = hex[originalText[i] & 15];
            }
    }
    encodedText[pos] = '\0';
    return encodedText;
}

#endif
