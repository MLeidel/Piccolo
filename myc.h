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
#include <dirent.h>
#include <sys/types.h>
#include <errno.h>
#include <stdarg.h>
#include <limits.h>


  // DECLARATIONS

// STRING FUNCTIONS
bool endswith (char*, char*);
bool equals(char*, char*);
bool equalsignorecase(char*, char*);
bool startswith (char*, char*);
char* chomp(char*);
char* concat(char*, int, ...);
char* deletechar(char*, char*, char, int);
char* field(char*, char*, char, int, bool);
char* insert(char*, char*, char*, size_t);
char* lowercase(char*);
char* leftof(char*, char*, char*, int);
char* ltrim(char*);
char* replace (char*, char*, char*, char*, int);
char* rightof(char*, char*, char*, int);
char* rtrim(char*);
char* strrev(char*);
char* strrstr(char*, char*);
char* substr(char*, char*, int, int);
char* trim(char*);
char* uppercase(char*);
char* urlencode(char*, char*);
int charat(char*, char);
int contains(char*, char*);
int indexof (char*, char*);
int lastcharat(char*, char);
int lastindexof (char*, char*);
int replacechar(char*, char, char, int);
int replacesz(char *, char *, char *, int);
int strtype(char*, int);

// ARRAY FUNCTIONS

    // ARRSIZE(x) THIS IS A MACRO

typedef struct clist {
    int nbr_rows;  // maximum records (columns, fields)
    int len_rows; // maximum length of one record (col, field)
    char** get;  // array of fields (char arrays or strings)
} clist;

clist clist_init(int, int);
clist clist_dir(const char*, int, bool);
int clist_parse(clist, char*, char*);
void clist_display(clist);
void clist_cleanup(clist);

// FILE & PATH FUNCTIONS
bool file_exists (char*);
FILE * open_for_append(char*);
FILE * open_for_read(char*);
FILE * open_for_write(char*);
int isfile(const char*);
int pathsize(const char*, int);
int readfile(char*, const char*);
int writefile(char*, const char*, bool append);

// DATE/TIME & OTHER FUNCTIONS
char* date(const char*);
void timeout(int, void f(int));
void multifree(int, ...);

// SORTING FUNCTIONS
void isort(int[], int);
void ssort(char*[], int, bool);
void dsort(double[], int);

/* END DECLARATIONS

-----------------------------------------
--------- Helpfull gcc functions --------
-----------------------------------------
char *realpath(const char *restrict path, char *restrict resolved_path);
        // realpath(filename, buff) // returns NULL on error
char *getenv(const char *name)
        // sprintf(descq_path, "%s/.config/descq", getenv("HOME"));
/*\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/*/

/*
            MACROS, DEFINES, AND UTILITIES
*/
#define MAX_L 4096  // used a lot for default string lengths
#define ARRSIZE(x)  (sizeof(x) / sizeof((x)[0]))
#define ERRMSG(a, b, c) (errmsg(a, b, c, __LINE__))

void errmsg(int rc, bool quit, char *msg, int line) {
    fprintf(stderr,
        "ERRMSG near line: %d, errno: %d %s\n%s\n",
        line,
        rc,
        strerror(rc),
        msg
        );
    if (quit) {  // print additional line and terminate program
        fprintf(stderr, "Program Exited\n");
        exit(EXIT_FAILURE);
    }
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

/*
        STRINGS
*/

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

char *rightof(char *out, char *in, char *targ, int start) {
    char *p;
    char *s;

    s = in + start;
    p = strstr(s, targ);
    if (p == NULL)
        return NULL;
    p += strlen(targ);
    strcpy(out, p);
    return p;  // *in remains unchanged
}

char *leftof(char *out, char *in, char *targ, int start) {
    char *p;
    int len = 0;

    in += start;
    p = strstr(in, targ);
    if (p == NULL)
        return NULL;
    len = p - in;
    strncpy(out, in, len);
    out[len] = '\0';
    return out;  // *in remains unchanged
}


int replacesz(char *base, char *target, char *replacement, int nbr) {
    // calculate new string size for memory allocation
    int sz_targ = strlen(target);
    int sz_repl = strlen(replacement);
    int count = 0;

    if (nbr == 0)
        count = contains(base, target);
    else
        count = nbr;

    return strlen(base) + (count * (sz_repl - sz_targ)) + 1;
}


/***
* replace
* params:
*   buf: return string with replacements
*   a: haystack string
*   b: needle string
*   c: replacement string
*   number: number of replacements to make (0 means replace all)
* NOTE: buf must be big enough to hold new string with replacements.
***/
char * replace (char *buf, char *a, char *b, char *c, int number) {
    int count = 0;
    int lenb = strlen(b);  // length of string to be replaced
    char *s = 0;
    char *ap;
    char *p;
    char *bfa = calloc(strlen(a)+1, sizeof(char));
    // or char *bfa = strdup(a);

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
//     int nbr_rows;  // maximum records (columns, fields)
//     int len_rows; // maximum length of one record (col, field)
//     char ** get; // array of fields (array of strings)
// } clist;

clist clist_init(int col, int len) {
     /* Initialize variables and allocate memory
        Return pointer to clist struct
     */
    clist csvf;
    csvf.len_rows = len;
    csvf.nbr_rows = col;
    csvf.get = calloc(csvf.nbr_rows, sizeof(char*));  // pointers
    for(int x=0; x < csvf.nbr_rows; x++) {
        csvf.get[x] = calloc(csvf.len_rows, sizeof(char));
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
        ERRMSG(99, true, "clist_parse delimiter must be length of 1");
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
    for(x=0; x < csvf.nbr_rows; x++) {
        printf("%03d - [%s] \n", x, csvf.get[x]);
    }
}

void clist_cleanup(clist csvf) {
    /* free each column's data then free the column pointer's
    */
    for(int col=0; col < csvf.nbr_rows; col++) {
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

int isfile(const char* name)
{
    DIR* directory = opendir(name);

    if(directory != NULL) {
     closedir(directory);
     return 0;
    }
    if(errno == ENOTDIR) {
     return 1;
    }
    return -1;
}

int pathsize(const char *path, int dtype) {
    struct dirent *de;
    int count = 0;
    char fpath[256];
    DIR *dr = opendir(path);
    if (dr == NULL)  {
        return 0;
    }
    while ((de = readdir(dr)) != NULL) {
        if (strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0) {
            strcpy(fpath, path);
            strcat(fpath, de->d_name);
            if (dtype == 0) {
                count++;
            } else if (dtype == 1 && isfile(fpath)) {
                count++;
            } else if (dtype == 2 && !isfile(fpath)) {
                count++;
            } else {
                continue;
            }
        }
    }
    closedir(dr);
    return count;
}

clist clist_dir(const char *path, int dtype, bool sort) {
    /*
     dir = 0 files and directories
     dir = 1 just files
     dir = 2 just directories
    */
    struct dirent *de;
    int n = 0;

    clist plst = clist_init(pathsize(path, dtype), 256);
    DIR *dr = opendir(path);
    char fpath[256];

    if (dr == NULL)  {
        ERRMSG(errno, true, "invalid path for clist_dir");
    }
    while ((de = readdir(dr)) != NULL)
        if (strlen(de->d_name) < plst.len_rows) {
            if (strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0) {
                strcpy(fpath, path);
                strcat(fpath, de->d_name);
                if (dtype == 0) {
                    strcpy(plst.get[n++], de->d_name);
                } else if (dtype == 1 && isfile(fpath)) {
                    strcpy(plst.get[n++], de->d_name);
                } else if (dtype == 2 && !isfile(fpath)) {
                    strcpy(plst.get[n++], de->d_name);
                } else {
                    continue;
                }
            }
        } else {
            ERRMSG(99, true, "path > 256 for clist_dir");
        }

    closedir(dr);
    if (sort) {
        printf("%d\n", plst.nbr_rows);
        ssort(plst.get, plst.nbr_rows, true);
    }
    return plst;
}

bool file_exists (char *filename) {
  struct stat   buffer;
  return (stat (filename, &buffer) == 0);
}


FILE * open_for_read(char *fname) {
    FILE *f1;
    if ((f1 = fopen(fname,"r")) == NULL) {
        ERRMSG(errno, true, "open_for_read: error from fopen");
    }
    return f1;
}


FILE * open_for_append(char *fname) {
    FILE *f1;
    if ((f1 = fopen(fname,"ab")) == NULL) {
        ERRMSG(errno, true, "open_for_append: error on fopen");
    }
    return f1;
}


FILE * open_for_write(char *fname) {
    FILE *f1;
    if ((f1 = fopen(fname,"wb")) == NULL) {
        ERRMSG(errno, true, "open_for_write: error on fopen");
    }
    return f1;
}


int readfile(char *buffer, const char *filename) {
    FILE *f;
    if ((f = fopen(filename,"rb")) == NULL) {
        ERRMSG(errno, false, "fopen: error on readfile");
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
            ERRMSG(errno, true, "fopen: error on writefile append");
        }
    } else {
        if ((f1 = fopen(filename,"wb")) == NULL) {
            ERRMSG(errno, true, "fopen: error on writefile");
        }
    }
    fprintf(f1,"%s", buffer);
    fclose(f1);
    return 0;
}


char* chomp(char *line) {  // see also rtrim()
    // remove record separators
    size_t len = strlen(line);
    char *tmp = line;
    for(int x=len; x >= 0; x--) {
        if (strchr("\r\n", tmp[x]) == NULL ) {
            break;
        } else {
            tmp[x] = '\0';
        }
    }
    return tmp;
}


char *concat(char *dest, int num, ...) {
    char *p = dest;
    va_list ap;

    va_start(ap, num);

    strcpy(p, va_arg(ap, char*));  // first one

    for(int x=0; x < num-1; x++) {
        strcat(p, va_arg(ap, char*));
    }

    va_end(ap);
    return p;
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


char* date(const char* format) {  // see man strftime
    time_t rawtime;
    struct tm *info;
    static char buffer[80];

    time( &rawtime );
    info = localtime( &rawtime );
    strftime(buffer, 80, format, info);
    return buffer;
}


/***
* timeout(sec) function starts a timer
* for sec seconds and calls a user function
* after sec seconds. "sec" is passed along
* to "f" the user handler function.
***/
void timeout(int sec, void f()) {
    signal(SIGALRM, f);
    alarm(sec);
}


void multifree(int num, ...) {
  va_list ap;
  int i = 0;
  char *p;

  va_start(ap, num);
  for (i=0; i < num; i++) {
    p = va_arg(ap, char*);
    free(p);
    p = NULL;
  }
  va_end(ap);
}


bool startswith (char* base, char* str) {
    return (strstr(base, str) - base) == 0;
}


bool endswith (char* base, char* str) {
    int blen = strlen(base);
    int slen = strlen(str);
    return (blen >= slen) && (0 == strcmp(base + blen - slen, str));
}


int contains(char *s, char *targ) {
    int tlen = strlen(targ);
    int count = 0;
    char *p = s;

    while(true) {
        p = strstr(p, targ);
        if (p == NULL)
            return count;
        count++;
        p += tlen;
    }
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

   if (!(position + length < len))
        ERRMSG(99, true, "substr inputs out of bounds");

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


char* urlencode(char* dest, char* urltext) {
    char *pout = dest;
    char *hex = "0123456789abcdef";

    int pos = 0;
    for (int i = 0; i < strlen(urltext); i++) {
        if (('a' <= urltext[i] && urltext[i] <= 'z')
            || ('A' <= urltext[i] && urltext[i] <= 'Z')
            || ('0' <= urltext[i] && urltext[i] <= '9')) {
                pout[pos++] = urltext[i];
            } else {
                pout[pos++] = '%';
                pout[pos++] = hex[urltext[i] >> 4];
                pout[pos++] = hex[urltext[i] & 15];
            }
    }
    pout[pos] = '\0';
    return pout;
}


char *insert(char *buf, char *s, char *ins, size_t inx) {
    size_t actual = strlen(s + inx);
    size_t new = strlen(ins);
    char *str = calloc(strlen(s) + strlen(ins) + 1, sizeof(char));
    strcpy(str, s);
    char *p = str + inx; // point of insertion
    memmove(p + new, p, actual + 1);
    memcpy(p, ins, new);
    strcpy(buf, str);
    free(str);
    return buf;
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

    if (buf[0] == 0x0)
        return 0;

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
            ERRMSG(99, true, "invalid strtype enum value");
    }
    if (count == len) return -1;  // entire alpha
    return count;  // will be > -1 and less than len
}

#endif
