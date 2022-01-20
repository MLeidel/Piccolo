#ifndef MYC
#define MYC
/* myc.h

STRING FUNCTIONS
bool endswith (char* base, char* str)
bool equals(char *str1, char *str2)
bool equalsignorecase(char *str1, char *str2)
bool startswith (char* base, char* str)
char *cshiftleft(char *str)
char *replace (char *a, const char *b, const char *c, int start, int number)
char *lowercase(char *str)
char *ltrim(char *s)
char *rtrim(char *s)
char *strrev(char *str)
char *substr(char *string, int position, int length)
char *trim(char *s)
char *uppercase(char *str)
char* removen(char *line)   // see also rtrim()
char* urlencode(char* originalText)
long int charat(char* base, char c)
long int indexof (char* base, char* str)
long int lastcharat(char* base, char c)
long int lastindexof (char* base, char* str)

ARRAY FUNCTIONS
uint ialen( int[] ) THIS IS A MACRO
uint stralen( char** ) THIS IS A MACRO
typedef struct clist {
    int max_row;    // max lenght of a field
    int max_col;    // number of fields
    char ** get; // array of fields (char arrays or strings)
} clist;
clist clist_init(int col, int len)
int clist_parse(clist, char *str, char *delim)
void clist_display(clist)
void clist_cleanup(clist)

FILE & PATH FUNCTIONS
bool file_exists (char *filename)
FILE * open_for_append(char *fname)
FILE * open_for_read(char *fname)
FILE * open_for_write(char *fname)
void readfile(char *buffer, const char *filename)

DATE FUNCTIONS
char* today()

SORTING FUNCTIONS
void isort(int values[], int n)
void ssort(const char* arr[], int n)

OTHER FUNCTIONS
int is_arg(int ac, char **argv, char *arg)
-----------------------------------------
--------- Helpfull gcc functions --------
-----------------------------------------
char *realpath(const char *restrict path, char *restrict resolved_path);
        //char* path = realpath(filename, NULL)
char *getenv(const char *name)
        // sprintf(descq_path, "%s/.config/descq", getenv("HOME"));
/*\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/*/

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <sys/stat.h>

#define MAX_L 4096  // used a lot for default string lengths **

// get length of an array of strings
#define stralen(X) (uint)(sizeof(X) / sizeof(char*))
// get length of an array of integers
#define ialen(X) (uint)(sizeof(X) / sizeof(int))

int panic(char * msg) {
    fprintf(stderr, "myc-panic: \n%s\n", msg);
    exit(EXIT_FAILURE);
}


static int myisortcmp (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}
// sort an array of integers
void isort(int values[], int n) {
    qsort(values, n, sizeof(int), myisortcmp);
}


static int myssortcmp(const void* a, const void* b) {
      return strcmp(*(const char**)a, *(const char**)b);
}
// sort an array of strings
void ssort(const char* arr[], int n) {
    qsort(arr, n, sizeof(const char*), myssortcmp);
}


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


/***
* replace arguments:
* a: haystack string
* b: needle to find
* c: replacement string
* start: index to start looking for needle (0 means beginning)
* number: number of replacements to make (0 means replace all)
***/
char * replace (char *a, const char *b, const char *c, int start, int number) {
    static char buf[MAX_L];
    char bfa[MAX_L] = {'\0'};
    char *p;
    char *s = 0;
    char *ap;
    long lenb = strlen(b);  // length of string to be replaced
    int count = 0;

    if (!(lenb < MAX_L)) panic("replace inputs out of bounds");
    if (!(strlen(a) < MAX_L)) panic("replace inputs out of bounds");
    if (!(start < MAX_L)) panic("replace start out of bounds");

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
        s = ap;  // s becomes ap
        count++;
        if (number != 0 && count >= number) {
            break;
        }
    }

    strcat(buf, ap);  // concatenate final segment
    sprintf(a, "%s", buf);
    return buf;
}


char * cshiftleft(char *str) {
    /* shift all characters beginning at str+1 one left
    */
    char *p = str;

    do {
        p++;
        *(p-1) = *p;
    } while(*p != '\0');

    return str;
}


/* Parsing out values from a field delimited string
csv string may include double quotes for explicit text
',' inside double quotes are handled

example:

char * line; // some input csv string

clist list = clist_init(5, 64);

clist_parse(list, line, ",");  // returns nbr of cols found

// list.get[0] would be the first field

clist_cleanup(list);  // free dynamic memory

// NOTE: the supplied input csv string is destroyed in the parsing
// NOTE: to get a single field from a csv string see the getfield function
*/

typedef struct clist {
    int max_row;    // max lenght of a field
    int max_col;    // number of fields
    char ** get; // array of fields (array of strings)
} clist;

clist clist_init(int col, int len) {
     /* Initialize variables and allocate memory
        Return pointer to clist struct
     */
    clist csvf;
    csvf.max_row = len;
    csvf.max_col = col;
    csvf.get = calloc(csvf.max_col, sizeof(char*));  // pointers
    for(int x=0; x < csvf.max_col; x++) {
        csvf.get[x] = calloc(csvf.max_row, sizeof(char));
    }
    return csvf;
}

char * qmark(char * str, char delim) {
    /* Hides delimiters within dbl quotes
       for the input string to csv_get_fields function.
       Called from csv_get_fields.
    */
    char *p = str;
    bool marking = false;

    while(*p != '\0') {

        if (*p == '\"') { // quotes found
            if (marking) {
                marking = false;
                p++;
                continue;
            } else {
                marking = true;
                p++;
                continue;
            }
        }  // quotes found

        if ((*p == delim) && (marking)) {
            *p = 31;  // US unit separator
        }
        p++;
    }
    return str;
}

int qunmark(char **str, int sz, char delim) {
    /* Un-hides the delimiters found within dbl quotes
       of fields now residing in the fields array/list.
       Called from csv_get_fields.
    */
    char **p = str;
    char *t;
    int count = 0;

    for (int x=0; x < sz; x++) {
        t = p[x];
        while(*t != '\0') {
            if (*t == 31) {
                *t = delim;
                count++;
            }
            if (*t == '\"') cshiftleft(t);
            t++;
        }
    }
    return count;
}

int clist_parse(clist csvf, char *str, char *delim) {
    /*  parse the csv fields into the array elements
    */
    int finx = 0;
    char * found;

    if (strlen(delim) != 1) {
        panic("clist_parse delimiter must be length of 1");
    }

    qmark(str, delim[0]);  // hide quoted delimiters

    while( (found = strsep(&str, delim)) != NULL )
        strcpy(csvf.get[finx++], trim(found));

    qunmark(csvf.get, finx, delim[0]);  // put back hidden delimiters

    return finx;
}

void clist_display(clist csvf) {
    int x;
    for(x=0; x < csvf.max_col; x++) {
        printf("%03d - [%s]\n", x, csvf.get[x]);
    }
}

void clist_cleanup(clist csvf) {
    /* free each column's data then free the column pointer's
    */
    for(int col=0; col < csvf.max_col; col++) {
        free(csvf.get[col]);
        csvf.get[col] = NULL;
    }
    free(csvf.get);
    csvf.get = NULL;
}
/*================= END csv_fields ====================*/


/***
* getfield args:
* s:      pointer to string literal
* deli:   character used for delimiting fields
* coln:   the 'column' of the field to retrieve
* strip:  strip leading/trailing whitespace before returning field
***/
char * getfield(char * s, char deli, int coln, bool strip) {
   int i;   // current parsed delimiter (',' or '\0') count
   int j;  // current parsed column count
   int k; // current column length
   char *p;  // pointer to current char in haystack
   char *t; // pointer start of current field
   static char line[MAX_L]; // return string

   i = j = k = 0;
   p = t = s;

   memset(line, 0, MAX_L);

   while (true) {
      if (*p == deli || *p == '\0') {
         i++;
         j = i-1;

         if (j == coln) {  // is this the field wanted?
            if (j == 0) {  // time to return field
               strncpy(line, t, k);
            } else {
               if ((t - s) > strlen(s)) {
                  // this column request is out of bounds
                  return NULL;
               } else {
                  strncpy(line, t, k-1);
               }
            }
            if (strip) {
               return trim(line);
            } else {
               return line;
            }


         } else {  // reset the marker variables
            k = 0;  // reset "length" counter
            t = p+1; // set next field start pointer
         }
      }  // end if delimiter
      p++;
      k++;
   }  // end while
}    // end getfield


bool file_exists (char *filename) {
  struct stat   buffer;
  return (stat (filename, &buffer) == 0);
}


FILE * open_for_read(char *fname) {
    FILE *f1;
    if ((f1 = fopen(fname,"rb")) == NULL) {
        printf("\nError trying to open %s\n\n", fname);
        return NULL;
    }
    return f1;
}


FILE * open_for_append(char *fname) {
    FILE *f1;
    if ((f1 = fopen(fname,"ab")) == NULL) {
        printf("\nError trying to open %s\n\n", fname);
        return NULL;
    }
    return f1;
}


FILE * open_for_write(char *fname) {
    FILE *f1;
    if ((f1 = fopen(fname,"wb")) == NULL) {
        printf("\nError trying to open %s\n\n", fname);
        return NULL;
    }
    return f1;
}


int readfile(char *buffer, const char *filename) {
    FILE *f;
    if ((f = fopen(filename,"rb")) == NULL) {
        printf("\nError trying to open %s\n\n", filename);
        return -1;
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
    return 0;
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


char * strrstr(char *s, char *t) {
    // does not come with gcc as far as I know
    char *p, *r;
    char *i = NULL;
    p = s;
    while (1) {
        r = strstr(s, t);
        if (r == NULL) {
            return i;
        } else {
            i = r;
            s++;
        }
    }
}


long int indexof(char* base, char* str) {
    // find the 'index' of a string in a string
    char *p;
    p = strstr (base, str);
    if (p == NULL) {
        return -1;
    } else {
        return p-base;
    }
}


long int lastindexof (char* base, char* str) {
    // find the last 'index' of a string in a string
    char *p;
    p = strrstr(base, str);
    if (p == NULL) {
        return -1;
    } else {
        return p-base;
    }
}


long int charat(char* base, char c) {
    // find index of a character in a string
    char *p;
    p = strchr(base, c);
    if (p == NULL) {
        return -1;
    } else {
        return p-base;
    }
}


long int lastcharat(char* base, char c) {
    char *p;
    p = strrchr(base, c);
     if (p == NULL) {
        return -1;
    } else {
        return p-base;
    }
}


char *substr(char *string, int position, int length) {
   static char p[MAX_L] = {"\0"};
   int c;
   int len = strlen(string);

   if (!(len < MAX_L)) panic("substr inputs out of bounds");
   if (!(position + length < len)) panic("substr inputs out of bounds");

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
