#ifndef MYC
#define MYC

/***
 *     _  _  _  _  ___     _  _
 *    ( \/ )( \/ )/ __)   / )( \
 *    / \/ \ )  /( (__  _ ) __ (
 *    \_)(_/(__/  \___)(_)\_)(_/
 */

#include <assert.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <locale.h>
#include <signal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

// DECLARATIONS

char myc_quote_chars = '\"';    // if needed reset this variable

// STRING FUNCTIONS
bool compare(char*, const char*, char*);
bool endswith(char*, char*);
bool equals(char*, char*);
bool equalsignore(char*, char*);
bool isnum_us(char*);
bool startswith(char*, char*);

char* center(char*, char*, char*, int);
char* chomp(char*);
char* concat(char*, ...);
char* decrypt(char*, char*, char*);
char* deletechar(char*, char*, char*, size_t, size_t);
char* dollar(char*, double, int, int);
char* encrypt(char*, char*, char*);
char* field(char*, char*, char, int, bool);
char* insert(char*, char*, char*, size_t);
char* insert_new(char*, char*, size_t);
char* lastsub(char*, char*);
char* lowercase(char*);
char* lof(char*, char*, char*, int);
char* lpad(char*, char*, char*, int);
char* ltrim(char*);
char* replace(char*, char*, char*, char*, size_t, size_t);
char* replace_new (char*, char*, char*, size_t, size_t);
char* rof(char*, char*, char*, int);
char* rpad(char*, char*, char*, int);
char* rtrim(char*);
char* strcon(char*, char**, char*, int, int);
char* strrev(char*);
char* substr(char*, char*, int, int);
char* trim(char*);
char* uppercase(char*);
char* urlencode(char*, char*);

int between(char*, char*, char*, char*, int);
int charat(char*, char);
int contains(char*, char*);
int contvars(char*, char*);
int indexof(char*, char*);
int lastcharat(char*, char);
int lastindexof(char*, char*);
int replacechar(char*, char, char, size_t);
int replacesz(char*, char*, char*, int);
int strtype(char*, int);

// NUMBER TO STRING CONVERSIONS
char* intstr(char*, int, bool);
char* instr_new(int, bool);
char* lngstr(char*, long, bool);
char* lngstr_new(long, bool);
char* dblstr(char*, double, int, bool);
char* dblstr_new(double, int, bool);

// STRING ALLOCATION
typedef struct string {
    size_t length;  // allocated length
    char *value;
} string;

bool string_cat(string, char*);
bool string_cpy(string, char*);
void string_del(string);
string string_def(size_t, char);
string string_new(const char*);
string string_rsz(string, size_t);
string string_wrp(char*, size_t, char);


// LIST FUNCTIONS

typedef struct list {
    int nbr_rows;  // maximum records (columns, fields)
    int len_rows; // maximum length of one record (col, field)
    char** item;  // array of fields (char arrays or strings)
} list;

char* list_string(list, char*, char*, bool);
list list_def(int, int);
list list_dir(const char*, int, bool);
list list_read(char*, bool);
list list_redef(list, int, int);
int list_find(list, char*);
int list_split(list, char*, char*);
void list_update(list, char*, int);
void list_display(list);
void list_del(list);
void list_init(list, ...);
void list_inject(list, char*, int);
void list_remove(list, int);
void list_io(list, char*, char);

// FILE & PATH FUNCTIONS

bool file_exists (char*);
bool isbinary(char*);
FILE * open_for_append(char*);
FILE * open_for_read(char*);
FILE * open_for_write(char*);
long filesize(const char*);
char* getbasename(char*, bool);
char* getbasepath(char*, char*);
char* getfullpath(char*, char*);
char* getini(char*, char*, char*);
int isfile(const char*);
int pathsize(const char*, int);
int readfile(char*, const char*);
int writefile(char*, const char*, bool append);
void filecopy(char*, char*);
void filedelete(char*);
void filemove(char*, char*);

// DATE/TIME & OTHER FUNCTIONS
char* date(const char*);
void flogf(FILE*, char*, ...);
void multifree(int, ...);
void timeout(int, void f(int));
void randini();
long randnum(long, long);

// SORTING FUNCTIONS
void isort(int[], int);
void ssort(char*[], int, bool);
void dsort(double[], int);

// CLIPBOARD
int cbcopy(char*);
char* cbpaste(char*);

// ZEN DIALOGS
int zenmsg(char*, char*, char*);
void runproc(char*, char*);
void zen(char*, char*);
void zenfile(char*, char*, bool, bool);
void zenform(char*, char*);
void zenlist(char*, char*);
void zenotify(char*, bool);
void zentext(char* , char*, char*, bool);

// END DECLARATIONS


/*
            MACROS, DEFINES, AND UTILITIES
*/
#define ARRSIZE(x)  (sizeof(x) / sizeof((x)[0]))
#define ERRMSG(a, b, c) (errmsg(a, b, c, __LINE__, __FILE__))
#define ZENMSG_YES 0
#define END "end"

// compare
#define GT ">"
#define GreaterThan ">"
#define LT "<"
#define LessThan "<"
#define GTE ">="
#define GreaterThanOrEqual ">="
#define LTE "<="
#define LessThanOrEqual "<="
#define EQ "=="
#define Equal "=="
#define NEQ "!="
#define NotEqual "!="

// strtype
enum styp {ALPHA, ALNUM, DIGIT, PRINT, SPACE, UPPER, LOWER, PUNCT};

// colors for console printing
struct Colors {
    char dft[10];
    char black[10];
    char dark_red[10];
    char dark_green[10];
    char dark_yellow[10];
    char dark_blue[10];
    char dark_magenta[10];
    char dark_cyan[10];
    char light_gray[10];
    char dark_gray[10];
    char red[10];
    char green[10];
    char yellow[10];
    char blue[10];
    char magenta[10];
    char cyan[10];
    char white[10];
};
struct Colors clr_fg = {"\033[39m",
                        "\033[30m",
                        "\033[31m",
                        "\033[32m",
                        "\033[33m",
                        "\033[34m",
                        "\033[35m",
                        "\033[36m",
                        "\033[37m",
                        "\033[90m",
                        "\033[91m",
                        "\033[92m",
                        "\033[93m",
                        "\033[94m",
                        "\033[95m",
                        "\033[96m",
                        "\033[97m"};
struct Colors clr_bg = {"\033[49m",
                        "\033[40m",
                        "\033[41m",
                        "\033[42m",
                        "\033[43m",
                        "\033[44m",
                        "\033[45m",
                        "\033[46m",
                        "\033[47m",
                        "\033[100m",
                        "\033[101m",
                        "\033[102m",
                        "\033[103m",
                        "\033[104m",
                        "\033[105m",
                        "\033[106m",
                        "\033[107m"};


void errmsg(int rc, bool quit, char *msg, int line, char *filename) {
    char em[64];
    if ( rc == -1 ) {
        strcpy(em, "application defined");
    } else {
        strcpy(em, strerror(rc));
    }
    fprintf(stderr,
            "ERRMSG  %s near line: %d, errno: %d %s\n%s\n",
            filename,
            line,
            rc,
            em,
            msg
           );
    if (quit) {  // print additional line and terminate program
        fprintf(stderr, "Program Exited\n");
        exit(EXIT_FAILURE);
    }
}

static int myisortcmp (const void * a, const void * b) {
    return ( *(int*)a - * (int*)b );
}
// sort an array of integers
void isort(int values[], int n) {
    qsort(values, n, sizeof(int), myisortcmp);
}

static int mydsortcmp (const void * a, const void * b) {
    return ( *(double*)a - * (double*)b );
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
    clipboard
    uses xclip
*/

int cbcopy(char *text) {
    /* copies text to the system clipboard
    usint xclip command */
    char cmd[10240]; // 10K
    int rc = 0;

    sprintf(cmd, "echo \"%s\" | xclip -selection clipboard", text);
    rc = system(cmd);
    return rc;
}

char *cbpaste(char *text) {
    /* pastes clipboard into text */
    char cmd[64]; // 10K

    strncpy(cmd, "xclip -o", 10);
    runproc(text, cmd);
    chomp(text);
    return text;
}


/*
        STRINGS
*/

// create a new dynamic string (string pointer) from a string literal
string string_new(const char *str) {
    string s;
    int length = strlen(str);
    if (length == 0) {
        ERRMSG(-1, true, "string_def malloc error 0 length");
    }
    s.value = malloc(length + 1);
    if (s.value == NULL) {
        ERRMSG(errno, true, "string_def malloc error");
    }
    strcpy(s.value, str);
    s.length = length;
    return s;
}

// create a new dynamic string (string pointer) of length and fill char
string string_def(size_t length, char fill) {
    string s;
    if (length == 0) {
        ERRMSG(-1, true, "string_new malloc error 0 length");
    }
    s.value = malloc(length + 1);
    if (s.value == NULL) {
        ERRMSG(errno, true, "string_new malloc error");
    }
    s.length = length;
    memset(s.value, fill, s.length);
    return s;
}

string string_rsz(string p, size_t length) {
    string z = p;
    if (length == 0) {
        ERRMSG(-1, true, "string_rsz realloc error 0 length");
    }
    z.value = realloc(p.value, length);
    if (z.value == NULL) {
        ERRMSG(errno, true, "string_rsz realloc error");
    }
    z.length = length;
    return z;
}

bool string_cat(string s, char *data) {
    size_t len;
    size_t slen;
    len = strlen(data);
    slen = strlen(s.value);

    if ((len + slen) > s.length - 1) {
        fprintf(stderr, "\nstring_cat boundary error\n");
        return false;
    }
    strcat(s.value, data);
    return true;
}

bool string_cpy(string s, char *data) {
    size_t len;
    len = strlen(data);
    if (len > s.length - 1) {
        fprintf(stderr, "\nstring_cpy boundary error\n");
        return false;
    }
    strcpy(s.value, data);
    return true;
}

void string_del(string s) {
    free(s.value);
    s.value = NULL;
    s.length = 0;
}

string string_wrp(char *in, size_t length, char sep) {
    int pos = 0, cinx = 0, linx = 0;
    int textsize = strlen(in);
    int newsize = ((textsize / length) + 1) * 3; // space + newline of 1 or 2 chars

    string cout = string_def(textsize + newsize, '\0');

    replacechar(in, sep, ' ', 0); // remove existing newlines

    while ( in[pos] != '\0') {

        if (++linx > length) {
            while ( in[pos] != ' ') { pos--; cinx--; }
            linx = 0;
            cout.value[cinx++] = sep;
            pos++;
        } else {
            cout.value[cinx++] = in[pos++];
        }
    }
    return cout;
}


char *strrev(char *str) {
    int i = 0, j = 0;
    char temp;

    j = strlen(str) - 1;
    while (i < j) {
        temp = str[j];
        str[j] = str[i];
        str[i] = temp;
        i++;
        j--;
    }
    return str;
}


char *ltrim(char *s) {
    char *forward = s;
    while (isspace(*forward++));
    return forward - 1;
}


char *rtrim(char *s) {
    char* back = s + strlen(s);
    while (isspace(*--back));
    *(back + 1) = '\0';
    return s;
}


char *strcon(   char *string,
                char *arr[],
                char *delim,
                int arr_size,
                int start) {
    /* Combine and Return an array-of-strings into
    one long string delimiting each string element by "delim"
    see myc/string/strcon.c and myc/list/list_strcon.c */

    for(int i = start; i < arr_size; i++) {
        if (i == arr_size - 1)
            strcat(string, arr[i]);
        else
            concat(string, arr[i], delim, END);
    }
    return string;
}


char *trim(char *s) {
    return rtrim(ltrim(s));
}


char *lof(char *buf, char *input, char *delim, int start) {
    int inx = indexof(input+start, delim);
    return substr(buf, input+start, 0, inx);
}


char *rof(char *buf, char *input, char *delim, int start) {
    int inx = indexof(input+start, delim) + strlen(delim);
    substr(buf, input+start, inx, 0);
}

/* WARNING DANGEROUS: POSSIBLE OVERFLOW
    expanded string space must be accounted for
*/
char *lpad(char *space, char *str, char *filler, int n) {
    strcpy(space, "\0");
    for(int x=0; x < n; x++) {
        strcat(space, filler);
    }
    strcat(space, str);
    return space;
}
/* WARNING DANGEROUS: POSSIBLE OVERFLOW
    expanded string space must be accounted for
*/
char *rpad(char *space, char *str, char *filler, int n) {
    strcpy(space, str);
    for(int x=0; x < n; x++) {
        strcat(space, filler);
    }
    return space;
}


/* WARNING DANGEROUS: POSSIBLE OVERFLOW
    expanded string space must be accounted for
*/
char *dollar(char *space, double amount, int fsize, int type) {
    char fmt[64] = {'\0'};
    int siz = 0;
    strcpy(space, "\0");
    setlocale(LC_NUMERIC, "");
    switch(type) {
        case 0:
            sprintf(fmt,"%.2f", amount); // no $ no ,
            break;
        case 1:
            sprintf(fmt,"$%.2f", amount); // yes $ no ,
            break;
        case 2:
            sprintf(fmt,"$%'.2f", amount); // yes $ yes ,
            break;
        default:
            sprintf(fmt,"%.2f", amount); // no $ no ,
    }
    siz = strlen(fmt);
    siz = fsize - siz;
    lpad(space, fmt, " ", siz);
    return space;
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
 WARNING DANGEROUS: POSSIBLE OVERFLOW
 expanded string space must be accounted for
*   ALSO: see 'replace_new'
***/
char * replace (char *buf, char *a, char *b, char *c, size_t start, size_t number) {
    int count = 0;
    int lenb = strlen(b);  // length of string to be replaced
    char *s = 0;
    char *ap;
    char *p;
    char *bfa = calloc(strlen(a) + 1, 1);
    // or char *bfa = strdup(a);

    strcpy(bfa, a);
    *buf = '\0';  // initalize return string

    s = ap = bfa;
    s += start;

    while (1) {
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

char * replace_new (char *a, char *b, char *c, size_t start, size_t number) {
    int count = 0;
    int lenb = strlen(b);  // length of string to be replaced
    char *s = 0;
    char *ap;
    char *p;
    char *bfa = calloc(strlen(a) + 1, 1);
    // or char *bfa = strdup(a);

    int newlen = replacesz(a, b, c, number);
    char *buf = calloc(newlen, sizeof(char));

    strcpy(bfa, a);
    *buf = '\0';  // initalize return string

    s = ap = bfa;
    s += start;

    while (1) {
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


int replacechar(char *a, char b, char c, size_t number) {
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

/*
   ======= Encryption functions ======== These functions use a
   character value shift similar to a Ceasar Cipher. The difference
   is that here each character has a different shift value which is
   determined by a secret key consisting of a string of characters.
   Part of the ascii value of each character in the key string is
   used to shift a character in the plain text string. Only the
   second digit of each ascii key character code is used for the
   shift value which is added to the plain text character value.
   After each character of the key has been applied it simply repeats
   until all of the plain text has been coded.

   NOTE: These functions only work on Plain Text Files, Not binary files.
*/

int getKeyShift(int inx, char *ckey) {
    char skey[16];
    char ck1;
    int ck1n = 0;
    sprintf(skey, "%d", ckey[inx]);  // make string of the int key code
    ck1 = skey[1];  // take 2nd digit of the (numeric) string
    ck1n = ck1 - '0';  // convert and return as int
    if (ck1n == 0) ck1n = 3;  // many zeros with no shift value
    return ck1n;
}

char *encrypt(char *etext, char *plain, char *key) {
    char ch;
    uint kix = 0;
    uint klen = strlen(key) - 1;  // valid index bounds
    uint ix = 0;
    char cval = 'A';
    uint plen = strlen(plain);
    for(ix=0; ix < plen; ix++) {
        ch = plain[ix];
        cval = ch + getKeyShift(kix, key);
        etext[ix] = cval;
        kix++;
        if (kix > klen) kix = 0;
    }
    etext[ix] = '\0';
    return etext;
}

char *decrypt(char *plain, char *etext, char *key) {
    char ch;
    uint kix = 0;
    uint klen = strlen(key) - 1;
    uint ix = 0;
    char cval;
    uint elen = strlen(etext);
    for(ix=0; ix < elen; ix++) {
        ch = etext[ix];
        cval = ch - getKeyShift(kix, key);
        plain[ix] = cval;
        kix++;
        if (kix > klen) kix = 0;
    }
    plain[ix] = '\0';
    return plain;
}

/*================= BEGIN "list" .. etc. ====================*/
/*
    typedef struct list {
        int nbr_rows;  // maximum records (columns, fields)
        int len_rows; // maximum length of one record (col, field)
        char ** item; // array of fields (array of strings)
    } list;
*/

list list_def(int col, int len) {
    /* Initialize variables and allocate memory
       Return pointer to list struct
    */
    list csvf;
    csvf.len_rows = len;
    csvf.nbr_rows = col;
    csvf.item = calloc(csvf.nbr_rows, sizeof(char*));  // pointers
    for (int x = 0; x < csvf.nbr_rows; x++) {
        csvf.item[x] = calloc(csvf.len_rows, sizeof(char));
    }
    return csvf;
}

/*
    Returns a Re-DEFined list with new dimensions.
    Retains items from first list and removes it.
*/
list list_redef(list l1, int new_rows, int new_len) {
    int x = 0;
    list lnew = list_def(new_rows, new_len);
    for(x=0; x < l1.nbr_rows; x++) {
        list_update(lnew, l1.item[x], x);
    }
    list_del(l1);
    return lnew;
}

char * qmark(char * str, char delim) {
    /* Hides delimiters within dbl quotes
       for the input string to 'field' function.
    */
    char *p = str;
    bool marking = false;

    while (*p != '\0') {

        if (*p == myc_quote_chars) { // quotes found
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
       Called from list.
    */
    char **p = str;
    char *t;
    int count = 0;

    for (int x = 0; x < sz; x++) {
        t = p[x];
        while (*t != '\0') {
            if (*t == 31) {
                *t = delim;
                count++;
            }
            t++;
        }
    }
    return count;
}

int list_split(list csvf, char *str, char *delim) {
    /*  parse the csv fields into the array elements
    */
    int finx = 0;
    char * found;

    if (strlen(delim) != 1) {
        ERRMSG(-1, true, "list_parse delimiter must be length of 1");
    }

    qmark(str, delim[0]);  // hide quoted delimiters

    while ( (found = strsep(&str, delim)) != NULL ) {
        if ( delim[0] == ' ' && *found == '\0' ) { // handle ' ' delimiter
            continue;  // handle consecutive space as one delimiter
        }
        strcpy(csvf.item[finx++], trim(found));
    }

    qunmark(csvf.item, finx, delim[0]);  // put back hidden delimiters

    return finx;
}

void list_display(list csvf) {
    int x;
    for (x = 0; x < csvf.nbr_rows; x++) {
        printf("%03d - [%s] \n", x, csvf.item[x]);
    }
}

void list_del(list csvf) {
    /* free each column's data then free the column pointer's
    */
    for (int col = 0; col < csvf.nbr_rows; col++) {
        free(csvf.item[col]);
        csvf.item[col] = NULL;
    }
    free(csvf.item);
    csvf.item = NULL;
}

/*  Very safe: both index and string length are checked
*/
void list_update(list lst, char *str, int element) {
    if (strlen(str) > lst.len_rows)
        ERRMSG(-1, true, "list_update item overflows length");
    if (element > lst.nbr_rows)
        ERRMSG(-1, true, "list_update invalid row index");
    strcpy(lst.item[element], str);
}

/*  Caution: will segfault if number of args
             does not match the number of rows!
*/
void list_init(list lst, ...) {
    va_list ap;
    string tmp = string_def(lst.len_rows * 3, '\0');

    va_start (ap, lst); /* make ap point to 1st unnamed arg */

    for (int x = 0; x < lst.nbr_rows; x++) {
        strcpy(tmp.value, va_arg(ap, char*));
        if (strlen(tmp.value) > lst.len_rows)
            ERRMSG(-1, true, "list_init item too large for def");
        strcpy(lst.item[x], tmp.value);
    }
    va_end (ap);
    string_del(tmp);
}

/*  textlines reads a text file and finds number
    of lines and longest line.
    used in list_read function and not in myc.md
*/
void textlines(int count[2], char * fn) {
    string line = string_def(1000000, '\0');
    int maxline = 0;
    FILE * f = open_for_read(fn);
    while (1) {
        fgets(line.value, 1000000, f);
        count[0]++;
        if (feof(f)) break;
        if (strlen(line.value) > maxline) {
            maxline = strlen(line.value);
        }
    }
    count[1] = maxline;
    fclose(f);
    string_del(line);
}

/*
    list_read reads a text file's lines
    into a list and returns the new list.
*/
list list_read(char *filename, bool strip) {
    int c[2] = {0, 0};  // lines, longest line
    textlines(c, filename);
    list lines = list_def(c[0], c[1]);
    FILE * f = open_for_read(filename);
    for (int x = 0; x < lines.nbr_rows; x++) {
        fgets(lines.item[x], lines.len_rows + 1, f);
        chomp(lines.item[x]);
        if (strip)
            trim(lines.item[x]);
    }
    fclose(f);
    return lines;
}

void list_io(list lst, char *fn, char mode) {
    int x = 0;
    char line[lst.len_rows];
    if (mode == 'r') {
        FILE * f = open_for_read(fn);
        for( x=0; x < lst.nbr_rows; x++) {
            fgets(line, lst.len_rows, f);
            list_update(lst, chomp(line), x);
        }
        fclose(f);
    } else if(mode == 'w') {
        FILE * f = open_for_write(fn);
        for( x=0; x < lst.nbr_rows; x++) {
            fprintf(f,"%s\n",lst.item[x]);
        }
        fclose(f);
    } else {
        ERRMSG(-1, true, "list_io: invalid mode");
    }
}

/*
    pushes a new item value into the list
    at a designated index ...
    shifts list items down and drops last item
    thus changing some of the indexes
*/
void list_inject(list lst, char *value, int inx) {
    if(inx < 0 || inx >= lst.nbr_rows) {
        ERRMSG(-1, true, "tried to inject into list with an invalid index");
    }
    for(int x = lst.nbr_rows - 1; x > inx; x--) {
        strcpy(lst.item[x], lst.item[x-1]);
    }
    list_update(lst, value, inx);
}

/*
    removes an item from the list at a
    designated index ...
    shifts list items up thus changing
    some of the indexes and creating
    zero length strings in the last row(s)
    BUT: list dimensions ARE NOT CHANGED.
*/
void list_remove(list lst, int inx) {
    int x = 0;
    if(inx < 0 || inx >= lst.nbr_rows) {
        ERRMSG(-1, true, "tried to remove from list with an invalid index");
    }
    for(x = inx; x < lst.nbr_rows - 1; x++) {
        strcpy(lst.item[x], lst.item[x+1]);
    }
    lst.item[x][0] = '\0';
}

/*
    finds an item in the list and
    returns its index
*/
int list_find(list lst, char *str) {
    int x = 0;
    for(x = 0; x < lst.nbr_rows; x++) {
        if(equals(lst.item[x], str))
            return x;
    }
    return -1;
}

/*  List to String
    combines list items into a field delimited string;
    May 2023 - added bool quote parameter and removal
    of possible empty trailing list items.
*/
char *list_string(list lst, char *str, char *delim, bool quote) {
    int x = 0;
    int has_comma, has_space, has_apost, has_quote;
    char separator[8] = {'\0'};
    string sbuf = string_def(lst.nbr_rows * lst.len_rows + 1, '\0');

    strcpy(separator, delim);
    strcpy(str, "\0");

    // eliminate trailing empty rows
    int new_rows = lst.nbr_rows;
    x = lst.nbr_rows - 1;
    while (strlen(lst.item[x]) == 0) {
        new_rows--;
        x--;
    }
    // now use "new_rows" instead of lst.nbr_rows
    // to calculate the return conatenated string

    for (x=0; x < new_rows; x++) {
        if (x == new_rows - 1)
            strcpy(separator, "\0"); // no delim on last item
        if (isnum_us(lst.item[x]) || !quote) {
            concat(str, lst.item[x], separator, END);
        } else {
            concat(str, "\"", lst.item[x], "\"", separator, END);
        }
        strcat(sbuf.value, str); // building the csv string
        strcpy(str, "\0");  // clear for next item+delim
    }
    strcpy(str, sbuf.value);
    string_del(sbuf);
    return str;
}

/*================= END list .. etc. ====================*/

int qunmark1(char *str, char delim) {
    /*  1d array version of qunmark
        used in field()
    */
    char *t;
    int count = 0;

    t = str;
    while (*t != '\0') {
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
* coln:  the 'column' of the field to retrieve numeric zero relative
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
            if (deli == ' ' && *(p + 1) == ' ') {
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
                        strncpy(r, t, k - 1);
                        r[k - 1] = '\0';
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
                t = p + 1; // set next field start pointer
            }
        }  // end if delimiter
        p++;
        k++;
    }  // end while
}     // end field function


int isfile(const char* name)
{
    DIR* directory = opendir(name);

    if (directory != NULL) {
        closedir(directory);
        return 0;
    }
    if (errno == ENOTDIR) {
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

list list_dir(const char *path, int dtype, bool sort) {
    /*
     dir = 0 files and directories
     dir = 1 just files
     dir = 2 just directories
    */
    struct dirent *de;
    int n = 0;

    list plst = list_def(pathsize(path, dtype), 256);
    DIR *dr = opendir(path);
    char fpath[256];

    if (dr == NULL)  {
        ERRMSG(errno, true, "invalid path for list_dir");
    }
    while ((de = readdir(dr)) != NULL)
        if (strlen(de->d_name) < plst.len_rows) {
            if (strcmp(de->d_name, ".") != 0 && strcmp(de->d_name, "..") != 0) {
                strcpy(fpath, path);
                strcat(fpath, de->d_name);
                if (dtype == 0) {
                    strcpy(plst.item[n++], de->d_name);
                } else if (dtype == 1 && isfile(fpath)) {
                    strcpy(plst.item[n++], de->d_name);
                } else if (dtype == 2 && !isfile(fpath)) {
                    strcpy(plst.item[n++], de->d_name);
                } else {
                    continue;
                }
            }
        } else {
            ERRMSG(-1, true, "path > 256 for list_dir");
        }

    closedir(dr);
    if (sort) {
        printf("%d\n", plst.nbr_rows);
        ssort(plst.item, plst.nbr_rows, true);
    }
    return plst;
}

bool file_exists (char *filename) {
    struct stat   buffer;
    return (stat (filename, &buffer) == 0);
}

bool isbinary(char *filename) {
    FILE *file = fopen(filename, "r");
    int ch;

    if (file == NULL) {
        printf("Error opening file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    while ((ch = fgetc(file)) != EOF) {
        if (ch == NULL) {
            fclose(file);
            return true;
        }
    }

    fclose(file);
    return false;
}

FILE * open_for_read(char *fname) {
    FILE *f1;
    if ((f1 = fopen(fname, "rb")) == NULL) {
        ERRMSG(errno, true, "open_for_read: error from fopen");
    }
    return f1;
}


FILE * open_for_append(char *fname) {
    FILE *f1;
    if ((f1 = fopen(fname, "ab")) == NULL) {
        ERRMSG(errno, true, "open_for_append: error on fopen");
    }
    return f1;
}


FILE * open_for_write(char *fname) {
    FILE *f1;
    if ((f1 = fopen(fname, "wb")) == NULL) {
        ERRMSG(errno, true, "open_for_write: error on fopen");
    }
    return f1;
}


int readfile(char *buffer, const char *filename) {
    FILE *f;
    if ((f = fopen(filename, "rb")) == NULL) {
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
        if ((f1 = fopen(filename, "ab")) == NULL) {
            ERRMSG(errno, true, "fopen: error on writefile append");
        }
    } else {
        if ((f1 = fopen(filename, "wb")) == NULL) {
            ERRMSG(errno, true, "fopen: error on writefile");
        }
    }
    fprintf(f1, "%s", buffer);
    fclose(f1);
    return 0;
}

long filesize(const char *filename) {
    struct stat sb;

    if (stat(filename, &sb) == -1) {
        ERRMSG(errno, false, "Trying to obtain 'filesize'");
        return -1;
    }
    return sb.st_size;
}

/*
  just a "replay" of realpath function
*/
char *getfullpath(char *fn, char *buff) {
    return realpath(fn, buff);
}


char* getbasename(char *fn, bool withext) {
    char *s = strrchr(fn, '/');  // find the last "/" in the path
    char *p = strrchr(fn, '.');  // last "." in path

    if (withext) {  // WITH extension

        if (s == NULL) { // no path present
            return fn;   // already base - nothing to do
        } else {         // path present
            return ++s;  // return past end of path/
        }

    } else {  // WITHOUT extension

        if (s == NULL) {     // no path present
            if (p == NULL) { // no ext present
                return fn;   // already base & no ext - nothing to do
            } else {         // ext is present
                *p = '\0';   // cut off extension at '.'
                return fn;
            }
        } else {             // path present
            if (p == NULL) { // no no extension present
                return ++s;  // already base & no ext - nothing to do
            } else {
                *p = '\0';   // cut off extension at '.'
                return ++s;
            }
        }
    }
}

char *getbasepath(char *fn, char *buff) {
    realpath(fn, buff);
    char *s = strrchr(buff, '/');  // find the last "/" in the path
    if (s != NULL) {
        *s = '\0';
    }
    return buff;
}

char *getini(char *value, char *inifile, char *ininame) {
    /*
        Returns a named value from an ini file
    */
    int lines[2];
    int inx=0;
    int p=0;

    list flst = list_read(inifile, true);

    for(inx=0; inx < flst.nbr_rows; inx++) {
        if (startswith(flst.item[inx], ininame)) {
            p = indexof(flst.item[inx], "=");
            substr(value, flst.item[inx], p+1, 0);
            strncpy(value, trim(value), strlen(value));
            list_del(flst);
            return value;
        }
    }
    if (inx >= flst.nbr_rows) {
        printf("\nDid not find ini file key name %s\n", ininame);
        list_display(flst);
        list_del(flst);
        exit(1);
    }
}


void filecopy(char *src, char *dst) {
    FILE * source = open_for_read(src);
    FILE * target = open_for_write(dst);
    char ch;
    while ((ch = fgetc(source)) != EOF)
        fputc(ch, target);
    fclose(target);
    fclose(source);
}

void filedelete(char *file) {
    unlink(file);
}

void filemove(char *src, char *dst) {
    filecopy(src, dst);
    filedelete(src);
}


char* chomp(char *line) {  // see also rtrim()
    // remove record separators
    size_t len = strlen(line);
    char *tmp = line;
    for (int x = len; x >= 0; x--) {
        if (strchr("\r\n", tmp[x]) == NULL ) {
            break;
        } else {
            tmp[x] = '\0';
        }
    }
    return tmp;
}


/* WARNING DANGEROUS: POSSIBLE OVERFLOW
    expanded string `space` must be accounted for
    allow enough characters in space
    HERE n is the length of the final string (space)
*/
char * center(char *space, char *str, char *filler, int n) {
    int sides = 0;
    int size = 0;
    int i = 0;

    size = strlen(str);
    sides = (n - size) / 2;

    lpad(space, str, filler, sides);
    rpad(space, space, filler, sides);

    return space;
}


/* WARNING DANGEROUS: POSSIBLE OVERFLOW
    expanded string space must be accounted for
*/
char *concat(char *dest, ...) {
    string buf = string_def(10240, '\0');
    va_list ap;

    va_start(ap, dest);
    // NOTE: dest MUST BE INITIALIZED to allow room for concatenation

    while(1) {
        strcpy(buf.value, va_arg(ap, char*));
        if (equalsignore(buf.value, "END")) break;
        strcat(dest, buf.value);
    }

    va_end(ap);
    string_del(buf);
    return dest;
}


char * deletechar(char *s, char *in, char *targ, size_t start, size_t number) {
    char *t = s;
    char *p = in;
    int len = strlen(p);
    int newlen = len;
    int count = 0, x = 0;

    for (x = 0; x < start; x++) {
        *t = *p;  // copy char before start index, if any
        t++;
        p++;
    }

    for (x = start; x < len; x++) {
        if (strchr(targ, *p)) {
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
    *(s + newlen) = '\0'; // reset the zero byte
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


void randini() {
   srand(time(0));
}

long randnum(long min, long max) {
   if(min>max)
   {
      printf("Min value is greater than max value\n");
      return 0;
   } else {
      return rand() % (max - min +1) + min;
   }
}


void multifree(int num, ...) {
    va_list ap;
    int i = 0;
    char *p;

    va_start(ap, num);
    for (i = 0; i < num; i++) {
        p = va_arg(ap, char*);
        free(p);
        p = NULL;
    }
    va_end(ap);
}


void flogf(FILE * fs, char *fmt, ...) {
    va_list ap;
    char *p, *sval;
    int ival;
    double dval;
    long lval;

    va_start (ap, fmt); /* make ap point to 1st unnamed arg */

    for (p = fmt; *p; p++) {
        if (*p != '%') {
            fputc(*p, fs);
            continue;
        }
        switch (*++p) {
        case 'd':
            ival = va_arg(ap, int);
            fprintf(fs, "%d", ival);
            break;
        case 'f':
            dval = va_arg(ap, double);
            fprintf(fs, "%f", dval);
            break;
        case 'l':  // long (%ld)
            lval = va_arg(ap, long);
            fprintf(fs, "%ld", lval);
            break;
        case 's':
            for (sval = va_arg(ap, char *); *sval; sval++)
                fputc(* sval, fs);
            break;
        case '$':  // for dollor format (double)
            dval = va_arg(ap, double);
            fprintf(fs, "%.02f", dval);
            break;
        default:
            fputc(*p, fs);
            break;
        }
    }
    va_end (ap);  /* clean up when done */
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

    if (tlen < 1)
        return 0;

    while (true) {
        p = strstr(p, targ);
        if (p == NULL)
            return count;
        count++;
        p += tlen;
    }
}

/*
    contvars returns count of delimited fields in a string
    taking into account delimiters fount in double-quotes
*/
int contvars(char *str, char *delim) {
    int count = 0;
    if (strlen(delim) > 1)
        ERRMSG(-1, false, "contvars delimiter length > 1");
    qmark(str, delim[0]); // exclude quoted delimiters
    count = contains(str, delim);
    if (count == 0) return 0;
    return (count + 1);
}


char * lastsub(char *s, char *t) {
    // strrstr does not come with gcc as far as I know
    char *r;
    char *i = NULL;

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
        return p - base;
    }
}


int lastindexof (char* base, char* str) {
    // find the last 'index' of a string in a string
    char *p;
    p = lastsub(base, str);
    if (p == NULL) {
        return -1;
    } else {
        return p - base;
    }
}


/*  between()
    Gets the substring between to substrings
    Returns index to next position
*/
int between(char *buf, char *input, char *str1, char *str2, int offset) {
    int len1 = 0, len2 = 0;
    int inx1 = 0, inx2 = 0;
    char *s = input + offset;

    len1 = strlen(str1);
    len2 = strlen(str2);
    inx1 = indexof(s, str1); // first target after offset
    inx2 = indexof(s+inx1+1, str2); // 2nd target after offset
    inx2 += inx1+1; // this now the 0 relative offset for str2
    if (inx1 == -1 || inx2 == -1) {
        ERRMSG(-1, false, "between - indexof failed");
        return -1;
    }
    if (inx1 >= inx2) {
        ERRMSG(-1, false, "between - inx1 >= inx2");
        return -1;
    }

    inx1 = inx1 + len1; // offset of target substring
    len1 = inx2 - inx1; // length for target
    substr(buf, s, inx1, len1); // target substring stored into buf
    return offset + inx2 + len2; // returns 1st position after str2
}


int charat(char* base, char c) {
    // find index of a character in a string
    char *p;
    p = strchr(base, c);
    if (p == NULL) {
        return -1;
    } else {
        return p - base;
    }
}

// return index to last c in a string
int lastcharat(char* base, char c) {
    char *p;
    p = strrchr(base, c);
    if (p == NULL) {
        return -1;
    } else {
        return p - base;
    }
}


char *substr(char *p, char *string, int position, int length) {
    int c;
    int len = strlen(string);

    if (!(position + length < len))
        ERRMSG(-1, true, "substr inputs out of bounds");

    if (length == 0) {  // from position to end of string
        strcpy(p, string + position);
        return p;
    }

    for (c = 0; c < length; c++) {  // return position for length
        *(p + c) = *(string + position);
        string++;
    }

    *(p + c) = '\0';

    return p;
}


char *lowercase(char *str) {
    int i, length = strlen(str);

    for (i = 0; i < length; i++) {
        str[i] = tolower(str[i]);
    }
    return str;
}


char *uppercase(char *str) {
    int i, length = strlen(str);

    for (i = 0; i < length; i++) {
        str[i] = toupper(str[i]);
    }
    return str;
}


bool equals(char *str1, char *str2) {
    return (strcmp(str1, str2) == 0);
}


bool equalsignore(char *str1, char *str2) {
    return (strcasecmp(str1, str2) == 0);
}


/*
    Tests if a character string is a number
    ( whole or decimal ) or not. US $ only.
*/
bool isnum_us(char *data) {
    int alpha, decimal, digit, punct;
    char item[128] = {'\0'};
    alpha = decimal = digit = punct = 0;

    strcpy(item, data);
    trim(item);
    alpha = strtype(item, ALPHA);
    decimal = contains(item, ".");
    digit = strtype(item, DIGIT);
    punct = strtype(item, PUNCT);

    if (alpha == 0 &&
        (digit > 0 || digit == -1)) {
        if (punct == decimal) {
            return true;
        }
    }
    return false;
}


bool compare(char *f1, const char *op, char *f2) {
    int r = strcmp(f1, f2);

    if (strcmp(op, "==") == 0) {
        if (r == 0)
            return true;
        else
            return false;
    }
    if (strcmp(op, "!=") == 0) {
        if (r != 0)
            return true;
        else
            return false;
    }
    if (strcmp(op, ">") == 0) {
        if (r > 0)
            return true;
        else
            return false;
    }
    if (strcmp(op, "<") == 0) {
        if (r < 0)
            return true;
        else
            return false;
    }
    if (strcmp(op, "<=") == 0) {
        if (r <= 0)
            return true;
        else
            return false;
    }
    if (strcmp(op, ">=") == 0) {
        if (r >= 0)
            return true;
        else
            return false;
    }

    // Must have been and invalid conditional value
    ERRMSG(-1, true, "Invalid conditional const value");
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


/* WARNING DANGEROUS: POSSIBLE OVERFLOW
    expanded string space must be accounted for
*/
char *insert(char *buf, char *s, char *ins, size_t inx) {
    size_t actual = strlen(s + inx); // length of insertion point to end
    size_t new = strlen(ins); // length of insertion text
    char *str = calloc(strlen(s) + strlen(ins) + 1, 1);
    strcpy(str, s);
    char *p = str + inx; // point of insertion
    memmove(p + new, p, actual + 1); // move over -> text-after-insertion
    memcpy(p, ins, new); // move in text to insert
    strcpy(buf, str);
    free(str);
    return buf;
}

char *insert_new(char *s, char *ins, size_t inx) {
    size_t actual = strlen(s + inx); // length of insertion point to end
    size_t new = strlen(ins); // length of insertion text
    char *str = calloc(strlen(s) + strlen(ins) + 1, sizeof(char));
    strcpy(str, s);
    char *p = str + inx; // point of insertion
    memmove(p + new, p, actual + 1); // move over -> text-after-insertion
    memcpy(p, ins, new); // move in text to insert
    return str;
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
// enum styp {ALPHA, ALNUM, DIGIT, PRINT, SPACE, UPPER, LOWER, PUNCT};

int strtype(char *buf, int istype) {
    size_t len;
    size_t count = 0;

    len = strlen(buf);

    if (buf[0] == 0x0)
        return 0;

    switch (istype) {
    case ALPHA:
        for (int x = 0; x < len; x++) {
            if (isalpha(buf[x])) {
                count++;
            }
        }
        break;
    case ALNUM:
        for (int x = 0; x < len; x++) {
            if (isalnum(buf[x])) {
                count++;
            }
        }
        break;
    case DIGIT:
        for (int x = 0; x < len; x++) {
            if (isdigit(buf[x])) {
                count++;
            }
        }
        break;
    case PRINT:
        for (int x = 0; x < len; x++) {
            if (isprint(buf[x])) {
                count++;
            }
        }
        break;
    case SPACE:
        for (int x = 0; x < len; x++) {
            if (isspace(buf[x])) {
                count++;
            }
        }
        break;
    case UPPER:
        for (int x = 0; x < len; x++) {
            if (isupper(buf[x])) {
                count++;
            }
        }
        break;
    case LOWER:
        for (int x = 0; x < len; x++) {
            if (islower(buf[x])) {
                count++;
            }
        }
        break;
    case PUNCT:
        for (int x = 0; x < len; x++) {
            if (ispunct(buf[x])) {
                count++;
            }
        }
        break;
    default:
        ERRMSG(-1, true, "invalid strtype enum value");
    }
    if (count == len) return -1;  // entire alpha
    return count;  // will be > -1 and less than len
}

/***
* converting numbers to strings
* for both automatic and dynamic memory variables
* counter parts for:
*   atoi, atol, atof
*
* with decimal precision and thousands separator options
*
* intstr and intstr_new
* lngstr and lngstr_new
* dblstr and dblstr_new
***/

/*
    convert integer to string using automatic memory
*/
char *intstr(char *buf, int n, bool separator) {
    if (separator) {
        setlocale(LC_NUMERIC, "");
        sprintf(buf, "%'d", n);
    } else
        sprintf(buf, "%d", n);
    return buf;
}

/*
    convert inteter to string using dynamic memory
*/
char *intstr_new(int n, bool separator) {
    int len = snprintf( NULL, 0, "%d", n );
    char* buf = malloc( len + 1 );
    if (separator) {
        setlocale(LC_NUMERIC, "");
        sprintf(buf, "%'d", n);
    } else
        sprintf(buf, "%d", n);
    return buf;
}

/*
    convert long to string using automatic memory
*/
char *lngstr(char *buf, long n, bool separator) {
    if (separator) {
        setlocale(LC_NUMERIC, "");
        sprintf(buf, "%'ld", n);
    } else
        sprintf(buf, "%ld", n);
    return buf;
}

/*
    convert long to string using dynamic memory
*/
char *lngstr_new(long n, bool separator) {
    int len = snprintf( NULL, 0, "%ld", n );
    char* buf = malloc( len + 1 );
    if (separator) {
        setlocale(LC_NUMERIC, "");
        sprintf(buf, "%'ld", n);
    } else
        sprintf(buf, "%ld", n);
    return buf;
}

/*
    convert double to string using automatic memory
*/
char *dblstr(char *buf, double n, int decimal, bool separator) {
    char fmt[32];
    if (separator) {
        setlocale(LC_NUMERIC, "");
        sprintf(fmt, "%s%d%s", "%'.0", decimal, "lf");
    } else {
        sprintf(fmt, "%s%d%s", "%.0", decimal, "lf");
    }
    sprintf(buf, fmt, n);
    return buf;
}

/*
    convert double to string using dynamic memory
*/
char *dblstr_new(double n, int decimal, bool separator) {
    char fmt[32];
    sprintf(fmt, "%s%d%s", "%.0", decimal, "lf");
    int len = snprintf( NULL, 0, fmt, n );
    char* buf = malloc( len + 1 );
    if (separator) {
        setlocale(LC_NUMERIC, "");
        sprintf(fmt, "%s%d%s", "%'.0", decimal, "lf");
    } else {
        sprintf(fmt, "%s%d%s", "%.0", decimal, "lf");
    }
    sprintf(buf, fmt, n);
    return buf;
}

/*
                                  _   _
             ____   ___   _ __   (_) | |_   _   _
            |_  /  / _ \ | '_ \  | | | __| | | | |
             / /  |  __/ | | | | | | | |_  | |_| |
            /___|  \___| |_| |_| |_|  \__|  \__, |
                                            |___/
Simple Gtk Dialogs
https://help.gnome.org/users/zenity/stable/index.html.en#dialogs
----------------------------------------------------------------
zenmsg      message dialog 4 types: "question", "error", "warning", "info"
zenfile     filedialog 2 types: open (false) and save (true)
zenform     form dialog: entry, calendar, list, combo
zenlist     list dialog with columns
zentry      1 field entry dialog (prompt?)
zentext     simple multi-line text dialog
zenotify    display a system notification message
zen         all-purpose zenity

*/

/* runproc
    function --------------------------------------------------------
*/
void runproc(char *output, char *proc) {
    FILE *fp;

    fp = popen(proc, "r");
    if (fp == NULL) {
        ERRMSG(-1, true, "runproc failure");
    }
    fgets(output, 4096, fp);
    pclose(fp);
}

/* zenmsg
    function --------------------------------------------------------
*/
int zenmsg(char *title, char *msg, char *type) {
    char cmd[512];
    int rc = 0;

    sprintf(cmd, "zenity --%s --title '%s' --text '%s'", type, title, msg);
    rc = system(cmd);
    return rc;  // question "No" returns 256, all others return 0
}

/* z-entry
    function --------------------------------------------------------
*/
char *zentry(char *entry, char *title, char *text, char *starting) {
    char cmd[512];

    strcpy(entry, "None");
    sprintf(cmd, "zenity --entry --title='%s' --text='%s' --entry-text '%s'",
            title, text, starting);
    runproc(entry, cmd);
    chomp(entry);
    return entry;
}

/* zenfile
    function --------------------------------------------------------
*/
void zenfile(char *selected, char *start, bool savemode, bool dirmode) {
    char cmd[512];

    if (savemode) {
        if (dirmode)
            sprintf(cmd, "zenity --file-selection --directory --save --title=\"Save File\" --filename=%s", start);
        else
            sprintf(cmd, "zenity --file-selection --save --title=\"Save File\" --filename=%s", start);
    } else {
        if (dirmode)
            sprintf(cmd, "zenity --file-selection --directory --title=\"Select a File\" --filename=%s", start);
        else
            sprintf(cmd, "zenity --file-selection --title=\"Select a File\" --filename=%s", start);
    }
    runproc(selected, cmd);
    chomp(selected);
}

/* zenform
    function --------------------------------------------------------
*/
void zenform(char *formdata, char *layout) {
    char cmd[4096];

    sprintf(cmd, "zenity --forms %s", layout);
    runproc(formdata, cmd);
    chomp(formdata);
}

/* zenlist
    function --------------------------------------------------------
*/
void zenlist(char *selected, char *layout) {
    char cmd[4096];

    sprintf(cmd, "zenity --list %s", layout);
    runproc(selected, cmd);
    chomp(selected);
}

/* zentext
    function --------------------------------------------------------
*/
void zentext(char* content, char *title, char *filename, bool edit) {
    FILE *fp;
    char line[4096] = {'\0'};
    char cmd[4096] = {'\0'};

    if (edit) {
        sprintf(cmd, "zenity --text-info --title='%s' --editable --filename='%s'",
                title, filename);
        strcpy(content, "\0");
        fp = popen(cmd, "r");
        if (fp == NULL)
            ERRMSG(-1, true, "zentext: popen failed");
        do {
            fgets(line, 4096, fp);
            if (feof(fp)) break;
            strcat(content, line);
        } while (1);
        pclose(fp);
    } else {
        sprintf(cmd, "zenity --text-info --title='%s' --filename='%s'",
                title, filename);
        system(cmd);
    }
}

/* zenotify
    function --------------------------------------------------------
*/
void zenotify(char *text, bool icon) {
    char cmd[512];

    if (icon) {
        sprintf(cmd, "zenity --notification --window-icon='info' --text='%s'", text);
    } else {
        sprintf(cmd, "zenity --notification --text='%s'", text);
    }
    system(cmd);
}

void zen(char *buf, char *code) {
    char cmd[4096];

    sprintf(cmd, "zenity %s", code);
    runproc(buf, cmd);
    chomp(buf);
}

#endif
