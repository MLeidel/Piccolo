#ifndef MYC
#define MYC
/* myc.h */

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <stddef.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>

  // DECLARATIONS

// STRING FUNCTIONS
bool endswith (char*, char*);
bool equals(char*, char*);
bool equalsignorecase(char*, char*);
bool startswith (char*, char*);
char* chomp(char*);   // see also rtrim()
char* deletechar(char*, char*, char, int);
char* field(char*, char*, char, int, bool);
char* replace (char*, char*, char*, char*, int);
char* lowercase(char*);
char* leftof(char*, char*, int);
char* ltrim(char*);
char* rightof(char*, char*, int);
char* rtrim(char*);
char* strrev(char*);
char* strrstr(char*, char*);
char* substr(char*, char*, int, int);
char* trim(char*);
char* uppercase(char*);
char* urlencode(char*);
int charat(char*, char);
int countchar(char *buf, char *targ);
int indexof (char*, char*);
int lastcharat(char*, char);
int lastindexof (char*, char*);
int replacechar(char*, char, char, int);
int strtype(char *, int);

// ARRAY FUNCTIONS

// ARRSIZE(x) THIS IS A MACRO

typedef struct clist {
    int max_row;   // max lenght of a field
    int max_col;  // number of fields
    char**  get; // array of fields (char arrays or strings)
} clist;

clist clist_init(int, int);
int clist_parse(clist, char*, char*);
void clist_display(clist);
void clist_cleanup(clist);

// FILE & PATH FUNCTIONS
bool file_exists (char*);
FILE * open_for_append(char*);
FILE * open_for_read(char*);
FILE * open_for_write(char*);
int readfile(char*, const char*);
int writefile(char*, const char*, bool append);

// DATE/TIME FUNCTIONS
char* today();
void timeout(int, void f(int));

// SORTING FUNCTIONS
void isort(int[], int);
void ssort(char*[], int, bool);

// EXTENDED STRING FUNCTIONS

typedef struct cstr {
    size_t length;  // allocated length
    char *str;
} cstr;

cstr cstr_init(size_t, char);
void cstr_cleanup(cstr);
void cstr_fill(cstr, char);
bool cstr_append(cstr, char*);
bool cstr_copy(cstr, char*);
bool cstr_prepend(cstr, char*);
bool cstr_insert(cstr, char*, size_t);
void cstr_replace(cstr, char*, char*, int);

/* END DECLARATIONS

-----------------------------------------
--------- Helpfull gcc functions --------
-----------------------------------------
char *realpath(const char *restrict path, char *restrict resolved_path);
        //char* path = realpath(filename, NULL)
char *getenv(const char *name)
        // sprintf(descq_path, "%s/.config/descq", getenv("HOME"));
/*\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/*/

#define MAX_L 4096  // used a lot for default string lengths

#define ARRSIZE(x)  (sizeof(x) / sizeof((x)[0]))

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

static int mydsortcmp (const void * a, const void * b) {
   return ( *(double*)a - *(double*)b );
}
// sort an array of doubles
void dsort(double values[], int n) {
    qsort(values, n, sizeof(double), mydsortcmp);
}

static int myssortcmp(const void* a, const void* b) {
      return strcmp(*(const char**)a, *(const char**)b);
}
static int myssortcasecmp(const void* a, const void* b) {
      return strcasecmp(*(const char**)a, *(const char**)b);
}
// sort an array of strings
void ssort(char* arr[], int n, bool ignorecase) {
    if (ignorecase)
        qsort(arr, n, sizeof(const char*), myssortcasecmp);
    else
        qsort(arr, n, sizeof(const char*), myssortcmp);
}


int countchar(char *buf, char *targ) {
    int count = 0;
    char *tmp = buf;
    while(tmp = strstr(tmp, targ))
    {
       count++;
       tmp++;
    }
    return count;
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
    char *forward = s;
    while(isspace(*forward++));
    return forward-1;
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

char *rightof(char *in, char *targ, int start) {
    char *p;
    char *s;

    s = in + start;
    p = strstr(s, targ);
    if (p == NULL)
        return NULL;
    p += strlen(targ);
    return p;  // *in remains unchanged
}

char *leftof(char *in, char *targ, int start) {
    static char s[MAX_L];  // work buffer
    char *p;

    strcpy(s, in + start);
    p = strstr(s, targ);
    if (p == NULL)
        return NULL;
    *p = '\0';
    return s;  // *in remains unchanged
}


/***
* replace
* params:
*   buf: return string with replacements
*   a: haystack string
*   b: needle string
*   c: replacement string
*   number: number of replacements to make (0 means replace all)
* NOTE: replace will fail if the resulting string length after replacements
* is greater than MAX_L. 'cstr_replace' does not have this limitation.
***/
char * replace (char *buf, char *a, char *b, char *c, int number) {
    char *bfa = calloc(MAX_L, sizeof(char));
    long lenb = strlen(b);  // length of string to be replaced
    int count = 0;
    char *s = 0;
    char *ap;
    char *p;

    if (!(lenb < MAX_L)) panic("replace inputs out of bounds");
    if (!(strlen(a) < MAX_L)) panic("replace inputs out of bounds");

    strcpy(bfa, a);
    *buf = '\0';  // initalize return string

    s = ap = bfa;

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
    free(bfa);
    bfa = NULL;
    return buf;
}


int replacechar(char *a, char b, char c, int number) {
    int len = strlen(a);
    char *p;
    int count = 0;

    p = a;

    while (*p != '\0') {
        if (*p == b) {
            *p = c;
            count ++;
            if (number > 0 && count == number) break;
        }
        p++;
    }
    return count;
}


/*  clist: Parses out values from a csv string
    that may use double quotes for explicit text.
    Delimiters inside double quoted fields are
    ignored.

example:

char * line; // some input csv string
clist list = clist_init(5, 64);
clist_parse(list, line, ",");  // returns nbr of cols found
    list.get[0] would be the first field
clist_cleanup(list);  // free dynamic memory

NOTE: the supplied input csv string is destroyed in the parsing
NOTE: to get a single field from a csv string see the field function
*/

// typedef struct clist {
//     int max_row;    // max lenght of a field
//     int max_col;    // number of fields
//     char ** get; // array of fields (array of strings)
// } clist;

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
       for the input string to 'field' function.
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
       Called from clist.
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

    while( (found = strsep(&str, delim)) != NULL ) {
        if( delim[0] == ' ' && *found == '\0' ) {  // handle ' ' delimiter
            continue;  // handle consecutive space as one delimiter
        }
        strcpy(csvf.get[finx++], trim(found));
    }

    qunmark(csvf.get, finx, delim[0]);  // put back hidden delimiters

    return finx;
}

void clist_display(clist csvf) {
    int x;
    for(x=0; x < csvf.max_col; x++) {
        printf("%03d - [%s] \n", x, csvf.get[x]);
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
/*================= END clist .. etc. ====================*/


int qunmark1(char *str, char delim) {
    /*  1d array version of qunmark
        used in field()
    */
    char *t;
    int count = 0;

   t = str;
   while(*t != '\0') {
      if (*t == 31) {
          *t = delim;
          count++;
      }
      t++;
   }
    return count;
}

/***
* field: extract one field out of a delimited string of fields
* r:     pointer for result storage
* s:     pointer to string literal
* deli:  character used for delimiting fields
* coln:  the 'column' of the field to retrieve
* strip: boolean: strip leading/trailing whitespace before returning field
* NOTE: Delimiter SPACE, when consecutive treated as one delimiter
* NOTE: field does not destroy the input string
***/
char * field(char *r, char * s, char deli, int coln, bool strip) {
   int i;   // parsed delimiter (or '\0') count
   int j;  // parsed column count
   int k; // column length
   char *p;  // pointer to char in haystack
   char *t; // pointer start of field

   i = j = k = 0;
   p = t = s;

   qmark(s, deli);  // hides delimiters inside dbl quotes

   while (true) {

      if (*p == deli || *p == '\0') {

         /* Special case for delimiter == SPACE
            multiple ' ' act as one delimiter
         */
         if (deli == ' ' && *(p+1) == ' ') {
            p++;
            continue;
         }

         j = i;  // inc column count
         i++;   // inc deli count

         if (j == coln) {  // is this the field wanted?
            if (j == 0) {  // time to return field
               strncpy(r, t, k);
               r[k] = '\0';
            } else {
               if ((t - s) > strlen(s)) {
                  // this column request is out of bounds
                  return NULL;
               } else {
                  strncpy(r, t, k-1);
                  r[k-1] = '\0';
               }
            }
            replacechar(s, 31, deli, 0);  // restore the original input string
            qunmark1(r, deli);  // show quoted delimiters and remove dbl quotes
            if (strip) {
               return trim(r);
            } else {
               return r;
            }

         } else {  // reset the marker variables
            k = 0;  // reset "length" counter
            t = p+1; // set next field start pointer
         }
      }  // end if delimiter
      p++;
      k++;
   }  // end while
}    // end field


bool file_exists (char *filename) {
  struct stat   buffer;
  return (stat (filename, &buffer) == 0);
}


FILE * open_for_read(char *fname) {
    if (!file_exists(fname)) {
        panic("open_for_read: No file or misspelled");
    }
    return fopen(fname,"rb");
}


FILE * open_for_append(char *fname) {
    FILE *f1;
    if ((f1 = fopen(fname,"ab")) == NULL) {
        panic("open_for_append: error on fopen");
    }
    return f1;
}


FILE * open_for_write(char *fname) {
    FILE *f1;
    if ((f1 = fopen(fname,"wb")) == NULL) {
        panic("open_for_write: error on fopen");
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

int writefile(char *buffer, const char *filename, bool append) {
    FILE *f1;
    if (append) {
        if ((f1 = fopen(filename,"ab")) == NULL) {
            panic("open_for_write: error on writefile append");
        }
    } else {
        if ((f1 = fopen(filename,"wb")) == NULL) {
            panic("open_for_write: error on writefile");
        }
    }
    fprintf(f1,"%s", buffer);
    fclose(f1);
    return 0;
}


char* chomp(char *line) {  // see also rtrim()
    // Remove very last character of a string
    line[strlen(line) - 1] = '\0';
    return line;
}


char * deletechar(char *s, char *in, char targ, int number) {
   char *t = s;
   char *p = in;
   int len = strlen(p);
   int newlen = len;
   int count = 0;

   for(int x=0; x < len; x++) {
      if (*p == targ) {
        if ( number == 0 || (number > 0 && ++count <= number) ) {
           p++;
           newlen--;
           continue;
        }
      }
      *t = *p;
      t++;
      p++;
   }
   *(s+newlen) = '\0';  // reset the zero byte
   return s;
}


char* today() {  // returns yyyy-mm-dd
    time_t rawtime;
    struct tm *info;
    static char buffer[80];

    time( &rawtime );
    info = localtime( &rawtime );
    strftime(buffer, 80, "%F", info);
    return buffer;
}


/***
* timeout(sec) function starts a timer
* for sec seconds and calls a user function
* after sec seconds. "sec" is passed along
* to "f" the user handler function.
***/
void timeout(int sec, void f(int)) {  // receive address of print
    signal(SIGALRM, f);
    alarm(sec);
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


int indexof(char* base, char* str) {
    // find the 'index' of a string in a string
    char *p;
    p = strstr(base, str);
    if (p == NULL) {
        return -1;
    } else {
        return p-base;
    }
}


int lastindexof (char* base, char* str) {
    // find the last 'index' of a string in a string
    char *p;
    p = strrstr(base, str);
    if (p == NULL) {
        return -1;
    } else {
        return p-base;
    }
}


int charat(char* base, char c) {
    // find index of a character in a string
    char *p;
    p = strchr(base, c);
    if (p == NULL) {
        return -1;
    } else {
        return p-base;
    }
}


int lastcharat(char* base, char c) {
    char *p;
    p = strrchr(base, c);
     if (p == NULL) {
        return -1;
    } else {
        return p-base;
    }
}


char *substr(char *p, char *string, int position, int length) {
   int c;
   int len = strlen(string);

   if (!(len < MAX_L)) panic("substr inputs out of bounds");
   if (!(position + length < len)) panic("substr inputs out of bounds");

   if (length == 0) {  // from position to end of string

      strcpy(p, string + position);
      return p;

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

    for(i = 0; i < length; i++) {
        str[i] = tolower(str[i]);
    }
    return str;
}


char *uppercase(char *str) {
    int i, length = strlen(str);

    for(i = 0; i < length; i++) {
        str[i] = toupper(str[i]);
    }
    return str;
}


bool equals(char *str1, char *str2) {
    return (strcmp(str1, str2) == 0);
}


bool equalsignorecase(char *str1, char *str2) {
    return (strcasecmp(str1, str2) == 0);
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



/***
* EXTENDED STRING FUNCTIONS
* Each string is allocated and managed
* within a struct of type cstr. Along with
* its functions (below) this provides a safer
* way to manipulate a char array when attempting
* actions that may cause buffer overflow.
*   cstr_init
*   cstr_fill
*   cstr_copy
*   cstr_insert
*   cstr_prepend
*   cstr_append
*   cstr_replace
*   cstr_cleanup
* Other string.h and myc.h functions can be
* used on a cstr string as well but without
* the added safty of the cstr functions.
***/

 // DECLARED AT TOP OF THIS FILE
// typedef struct cstr {
//     size_t length;  // allocated length
//     char *str;
// } cstr;

cstr cstr_init(size_t len, char fill) {
    cstr cs;
    cs.length = len;
    cs.str = calloc(len, sizeof(char));
    memset(cs.str, fill, len);
    *(cs.str+len) = '\0';
    return cs;
}

void cstr_cleanup(cstr cs) {
    cs.length = 0;
    free(cs.str);
    cs.str = NULL;
}

void cstr_fill(cstr cs, char c) {
    memset(cs.str, c, sizeof(char) * cs.length);
    *(cs.str+cs.length) = '\0';
}

bool cstr_append(cstr cs, char * s) {
    size_t actual = strlen(cs.str);
    size_t new = strlen(s);
    if (actual + new > cs.length - 1) {
        return false;
    } else {
        strcat(cs.str, s);
        return true;
    }
}

bool cstr_copy(cstr cs, char *s) {
    size_t new = strlen(s);
    if (new > cs.length - 1) {
        return false;
    } else {
        strcpy(cs.str, s);
        return true;
    }
}

bool cstr_prepend(cstr cs, char * s) {
    size_t actual = strlen(cs.str);
    size_t new = strlen(s);
    if (actual + new > cs.length - 1) {
        return false;
    } else {
        memmove(cs.str + new, cs.str, actual + 1);
        memcpy(cs.str, s, new);
        return true;
    }
}

bool cstr_insert(cstr cs, char *s, size_t inx) {
    size_t actual = strlen(cs.str + inx);
    size_t new = strlen(s);
    char *p = cs.str + inx; // update to index position
    if (actual + new > cs.length - 1) {
        return false;
    } else {
        memmove(p + new, p, actual + 1);
        memcpy(p, s, new);
        return true;
    }
}


/*
    Unlike the "replace" function (above) this one works only
    with strings created with CstrInit().
    Also here the maximum string length is determined by cstr.length
    so the MAX_L limit does not exist.
*/
void cstr_replace(cstr cs, char *b, char *c, int number) {
    char *buf = calloc(cs.length, sizeof(char));
    char *bfa = calloc(cs.length, sizeof(char));
    size_t lenb = strlen(b);  // length of string to be replaced
    size_t count = 0;
    char *ap;
    char *p;
    char *s;

    if (!(lenb < cs.length)) panic("replace inputs out of bounds");
    if (!(strlen(cs.str) < cs.length)) panic("replace inputs out of bounds");

    strcpy(bfa, cs.str);
    s = ap = bfa;

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
        if (number > 0 && count >= number) {
            break;
        }
    }

    strcat(buf, ap);  // concatenate final segment
    // copy result to cstr.str and clean up memory
    strcpy(cs.str, buf);
    free(bfa);
    free(buf);
}

/***
* functions for string
* to compliment ctype.h
* functions for characters
* isalpha -> strtype(buf, ALPHA)
* isalnum -> strtype(buf, ALNUM)
* isdigit -> strtype(buf, DIGIT)
* isprint -> strtype(buf, PRINT)
* isspace -> strtype(buf, SPACE)
* isupper -> strtype(buf, UPPER)
* islower -> strtype(buf, LOWER)
* ispunct -> strtype(buf, PUNCT)
* Returns int:
*    0 = none found
*    N = number found
*   -1 = all found
***/
enum styp {ALPHA, ALNUM, DIGIT, PRINT, SPACE, UPPER, LOWER, PUNCT};

int strtype(char *buf, int istype) {
    size_t len;
    size_t count = 0;

    len = strlen(buf);

    switch (istype) {
        case ALPHA:
            for(int x=0; x < len; x++) {
                if (isalpha(buf[x])) {
                    count++;
                }
            }
            break;
        case ALNUM:
            for(int x=0; x < len; x++) {
                if (isalnum(buf[x])) {
                    count++;
                }
            }
            break;
        case DIGIT:
            for(int x=0; x < len; x++) {
                if (isdigit(buf[x])) {
                    count++;
                }
            }
            break;
        case PRINT:
            for(int x=0; x < len; x++) {
                if (isprint(buf[x])) {
                    count++;
                }
            }
            break;
        case SPACE:
            for(int x=0; x < len; x++) {
                if (isspace(buf[x])) {
                    count++;
                }
            }
            break;
        case UPPER:
            for(int x=0; x < len; x++) {
                if (isupper(buf[x])) {
                    count++;
                }
            }
            break;
        case LOWER:
            for(int x=0; x < len; x++) {
                if (islower(buf[x])) {
                    count++;
                }
            }
            break;
        case PUNCT:
            for(int x=0; x < len; x++) {
                if (ispunct(buf[x])) {
                    count++;
                }
            }
            break;
        default:
            panic("invalid strtype enum value");
    }
    if (count == len) return -1;  // entire alpha
    return count;  // will be > -1 and less than len
}

#endif
