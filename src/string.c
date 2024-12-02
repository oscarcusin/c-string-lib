#include "string.h"
#include <string.h>
#include <limits.h>
#include "sais.h"

#define max2(a, b) ((a) > (b) ? (a) : (b))
#define min2(a, b) ((a) < (b) ? (a) : (b))
#define max3(a, b, c) max2(a, max2(b, c))
#define min3(a, b, c) min2(a, min2(b, c))

#define A_SIZE 256
#define SSO_CAPACITY 16
#define RK_BASE 256
#define MSD_CUTOFF 16

struct string {
    size_t size, capacity;
    union {
        unsigned char buf[SSO_CAPACITY];
        unsigned char * ptr;
    };
};

string * string_new() {
    string * s = malloc(sizeof(string));
    if (!s) return NULL;
    s->size = 0;
    s->capacity = SSO_CAPACITY;
    return s;
}

string * string_new_from_buff(unsigned char * buff, size_t size) {
    if (!buff) return NULL;
    string * s = malloc(sizeof(string));
    if (!s) return NULL;
    if (size <= SSO_CAPACITY) {
        s->size = size;
        s->capacity = SSO_CAPACITY;
        memcpy(s->buf, buff, size);
        return s;
    }
    s->ptr = malloc(size);
    if (!s->ptr) {
        free(s);
        return NULL;
    }
    s->size = size;
    s->capacity = size;
    memcpy(s->ptr, buff, size);
    return s;
}

string * string_new_from_cstr(char * cstr) {
    if (!cstr) return NULL;
    return string_new_from_buff((unsigned char *) cstr, strlen(cstr));
}

string * string_new_from_file(FILE * file) {
    if (!file) return NULL;
    string * s = malloc(sizeof(string));
    if (!s) return NULL;
    fseek(file, 0, SEEK_END);
    s->size = ftell(file);
    fseek(file, 0, SEEK_SET);
    if (s->size <= SSO_CAPACITY) {
        s->capacity = SSO_CAPACITY;
        fread(s->buf, 1, s->size, file);
        return s;
    }
    s->ptr = malloc(s->size);
    if (!s->ptr) {
        free(s);
        return NULL;
    }
    s->capacity = s->size;
    fread(s->ptr, 1, s->size, file);
    return s;
}

string * string_copy(string * s) {
    if (!s) return NULL;
    string * copy = malloc(sizeof(string));
    if (!copy) return NULL;
    if (s->size <= SSO_CAPACITY) {
        copy->size = s->size;
        copy->capacity = SSO_CAPACITY;
        memcpy(copy->buf, s->buf, s->size);
        return copy;
    }
    copy->ptr = malloc(s->size);
    if (!copy->ptr) {
        free(copy);
        return NULL;
    }
    copy->size = s->size;
    copy->capacity = s->size;
    memcpy(copy->ptr, s->ptr, s->size);
    return copy;
}

string * string_substring(string * s, size_t start, size_t end) {
    if (!s || start > end || end > s->size) return NULL;
    string * sub = malloc(sizeof(string));
    if (!sub) return NULL;
    size_t size = end - start;
    if (size <= SSO_CAPACITY) {
        sub->size = size;
        sub->capacity = SSO_CAPACITY;
        memcpy(sub->buf, string_raw(s) + start, size);
        return sub;
    }
    sub->ptr = malloc(size);
    if (!sub->ptr) {
        free(sub);
        return NULL;
    }
    sub->size = size;
    sub->capacity = size;
    memcpy(sub->ptr, string_raw(s) + start, size);
    return sub;
}

void string_delete(string * s) {
    if (!s) return;
    if (s->capacity > SSO_CAPACITY && s->ptr) free(s->ptr);
    free(s);
}

size_t string_length(string * s) {
    if (!s) return 0;
    return s->size;
}

size_t string_capacity(string * s) {
    if (!s) return 0;
    return s->capacity;
}

unsigned char * string_raw(string * s) {
    if (!s) return NULL;
    return s->capacity == SSO_CAPACITY ? s->buf : s->ptr;
}

void string_output(string * s, FILE * file) {
    if (!s || !file) return;
    fwrite(string_raw(s), 1, string_length(s), file);
}

int string_compare(string * s1, string * s2) {
    if (!s1 && !s2) return 0;
    if (!s1) return -1;
    if (!s2) return 1;
    for (size_t i = 0; i < s1->size && i < s2->size; i++) {
        if (string_raw(s1)[i] < string_raw(s2)[i]) return -1;
        if (string_raw(s1)[i] > string_raw(s2)[i]) return 1;
    }
    if (s1->size < s2->size) return -1;
    if (s1->size > s2->size) return 1;
    return 0;
}

int string_edit_distance(string * s1, string * s2, int insert_cost, int delete_cost, int replace_cost) {
    if (!s1 && !s2) return 0;
    if (!s1) return string_length(s2);
    if (!s2) return string_length(s1);
    size_t n = string_length(s1), m = string_length(s2);
    int (* dp)[m + 1] = malloc((n + 1) * sizeof(*dp));
    if (!dp) return -1;
    for (size_t i = 0; i <= n; i++) {
        dp[i][0] = i * delete_cost;
    }
    for (size_t j = 0; j <= m; j++) {
        dp[0][j] = j * insert_cost;
    }
    for (size_t i = 1; i <= n; i++) {
        for (size_t j = 1; j <= m; j++) {
            dp[i][j] = min3(dp[i - 1][j] + delete_cost,
                            dp[i][j - 1] + insert_cost,
                            dp[i - 1][j - 1] + (string_raw(s1)[i - 1] != string_raw(s2)[j - 1]) * replace_cost);
        }
    }
    int result = dp[n][m];
    free(dp);
    return result;
}

static int string_increase_capacity(string * s, size_t min) {
    if (!s) return 1;
    if (s->capacity >= min) return 0;
    size_t prev_capacity = s->capacity;
    while (s->capacity < min) {
        s->capacity *= 2;
    }
    if (prev_capacity == SSO_CAPACITY) {
        unsigned char * data = malloc(s->capacity);
        if (!data) return 1;
        memcpy(data, s->buf, s->size);
        s->ptr = data;
    } else {
        s->ptr = realloc(s->ptr, s->capacity);
        if (!s->ptr) return 1;
    }
    return 0;
}

int string_insert_char(string * s, unsigned char c, size_t index) {
    if (!s || index > s->size) return 1;
    if (s->size + 1 > s->capacity) {
        if (string_increase_capacity(s, s->size + 1)) return 1;
    }
    memmove(string_raw(s) + index + 1, string_raw(s) + index, s->size - index);
    string_raw(s)[index] = c;
    s->size++;
    return 0;
}

int string_insert_str(string * s, string * other, size_t index) {
    if (!s || !other || index > s->size) return 1;
    if (s->size + other->size > s->capacity) {
        if (string_increase_capacity(s, s->size + other->size)) return 1;
    }
    memmove(string_raw(s) + index + other->size, string_raw(s) + index, s->size - index);
    memcpy(string_raw(s) + index, string_raw(other), other->size);
    s->size += other->size;
    return 0;
}

int string_append_char(string * s, unsigned char c) {
    return string_insert_char(s, c, s->size);
}

int string_append_str(string * s, string * other) {
    return string_insert_str(s, other, s->size);
}

int string_erase(string * s, size_t index, size_t length) {
    if (!s || index + length > s->size) return 1;
    memmove(string_raw(s) + index, string_raw(s) + index + length, s->size - index - length);
    s->size -= length;
    return 0;
}

int string_border_array(string * s, int b[]) {
    if (!s || string_length(s) == 0 || string_length(s) > INT_MAX) return 1;
    int n = (int) string_length(s);
    unsigned char * s_raw = string_raw(s);
    b[0] = 0;
    for (int i = 1; i < n; i++) {
        int j = b[i - 1];
        while (j > 0 && s_raw[i] != s_raw[j]) {
            j = b[j - 1];
        }
        b[i] = s_raw[i] == s_raw[j] ? j + 1 : 0;
    }
    return 0;
}

int string_suffix_array(string * s, int sa[]) {
    if (!s || string_length(s) == 0 || string_length(s) > INT_MAX) return 1;
    return sais(string_raw(s), sa, (int) string_length(s));
}

int string_lcp_array(string * s, int sa[], int lcp[]) {
    if (!s || string_length(s) == 0 || string_length(s) > INT_MAX) return 1;
    int n = (int) string_length(s);
    unsigned char * s_raw = string_raw(s);
    int * phi = calloc(n, sizeof(int));
    if (!phi) return 1;
    int * plcp = calloc(n, sizeof(int));
    if (!plcp) {
        free(phi);
        return 1;
    }
    phi[sa[0]] = -1;
    for (int i = 1; i < n; i++) {
        phi[sa[i]] = sa[i - 1];
    }
    int l = 0;
    for (int i = 0; i < n; i++) {
        int j = phi[i];
        if (j == -1) {
            plcp[i] = 0;
            continue;
        }
        while (i + l < n && j + l < n && s_raw[i + l] == s_raw[j + l]) {
            l++;
        }
        plcp[i] = l;
        if (l > 0) l--;
    }
    for (int i = 0; i < n; i++) {
        lcp[i] = plcp[sa[i]];
    }
    free(phi);
    free(plcp);
    return 0;
}

size_t string_search_naive(string * s, string * p, match_callback_t cb, void * ctx) {
    if (!s || !p || string_length(p) > string_length(s)) return 0;
    size_t n = string_length(s), m = string_length(p);
    unsigned char * s_raw = string_raw(s), * p_raw = string_raw(p);
    size_t count = 0;
    for (size_t i = 0; i < n - m + 1; i++) {
        size_t j = 0;
        while (j < m && s_raw[i + j] == p_raw[j]) j++;
        if (j == m) {
            count++;
            if (cb) cb(ctx, i);
        }
    }
    return count;
}

size_t string_search_kmp(string * s, string * p, match_callback_t cb, void * ctx) {
    if (!s || !p || string_length(p) > string_length(s)) return 0;
    size_t n = string_length(s), m = string_length(p);
    unsigned char * s_raw = string_raw(s), * p_raw = string_raw(p);
    size_t count = 0;
    int * b = malloc(m * sizeof(int));
    string_border_array(p, b);
    size_t i = 0, j = 0;
    while (i < n) {
        if (s_raw[i] == p_raw[j]) {
            i++;
            j++;
            if (j == m) {
                count++;
                if (cb) cb(ctx, i - j);
                j = b[j - 1];
            }
        } else {
            if (j == 0) {
                i++;
            } else {
                j = b[j - 1];
            }
        }
    }
    free(b);
    return count;
}

size_t string_search_bms(string * s, string * p, match_callback_t cb, void * ctx) {
    if (!s || !p || string_length(p) > string_length(s)) return 0;
    size_t n = string_length(s), m = string_length(p);
    unsigned char * s_raw = string_raw(s), * p_raw = string_raw(p);
    size_t count = 0;
    int shift[A_SIZE];
    for (int i = 0; i < A_SIZE; i++) {
        shift[i] = m + 1;
    }
    for (size_t i = 0; i < m; i++) {
        shift[p_raw[i]] = m - i;
    }
    size_t i = 0;
    while (i <= n - m) {
        size_t j = 0;
        while (j < m && s_raw[i + j] == p_raw[j]) j++;
        if (j == m) {
            count++;
            if (cb) cb(ctx, i);
        }
        if (i + m < n) {
            i += shift[s_raw[i + m]];
        } else break;
    }
    return count;
}

size_t string_search_rk(string * s, string * p, match_callback_t cb, void * ctx) {
    if (!s || !p || string_length(p) > string_length(s)) return 0;
    size_t n = string_length(s), m = string_length(p);
    unsigned char * s_raw = string_raw(s), * p_raw = string_raw(p);
    size_t count = 0;
    unsigned int p_hash = 0, s_hash = 0, h = 1;
    for (size_t i = 0; i < m - 1; i++) {
        h = (h * RK_BASE);
    }
    for (size_t i = 0; i < m; i++) {
        p_hash = (RK_BASE * p_hash + p_raw[i]);
        s_hash = (RK_BASE * s_hash + s_raw[i]);
    }
    for (size_t i = 0; i <= n - m; i++) {
        if (p_hash == s_hash) {
            size_t j = 0;
            while (j < m && s_raw[i + j] == p_raw[j]) j++;
            if (j == m) {
                count++;
                if (cb) cb(ctx, i);
            }
        }
        if (i < n - m) {
            s_hash = (RK_BASE * (s_hash - s_raw[i] * h) + s_raw[i + m]);
        }
    }
    return count;
}

static void sort(size_t size, string * s[size], string * aux[size], size_t lo, size_t hi, size_t d) {
    if (hi - lo < MSD_CUTOFF) {
        for (size_t i = lo; i <= hi; i++) {
            for (size_t j = i; j > lo && string_compare(s[j], s[j - 1]) < 0; j--) {
                string * temp = s[j];
                s[j] = s[j - 1];
                s[j - 1] = temp;
            }
        }
        return;
    }
    int count[A_SIZE + 2] = {0};
    for (size_t i = lo; i <= hi; i++) {
        if (d >= string_length(s[i])) {
            count[1]++;
        } else {
            count[string_raw(s[i])[d] + 2]++;
        }
    }
    for (int i = 1; i < A_SIZE + 2; i++) {
        count[i] += count[i - 1];
    }
    for (size_t i = lo; i <= hi; i++) {
        if (d >= string_length(s[i])) {
            aux[count[0]++] = s[i];
        } else {
            aux[count[string_raw(s[i])[d] + 1]++] = s[i];
        }
    }
    for (size_t i = lo; i <= hi; i++) {
        s[i] = aux[i - lo];
    }
    for (int i = 0; i < A_SIZE; i++) {
        if (count[i + 1] > count[i]) {
            sort(size, s, aux, lo + count[i], lo + count[i + 1] - 1, d + 1);
        }
    }
    return;
}

int string_sort_msd(size_t size, string * s[size]) {
    if (!s || !size) return 1;
    string ** aux = malloc(size * sizeof(string *));
    if (!aux) return 1;
    sort(size, s, aux, 0, size - 1, 0);
    free(aux);
    return 0;
}
