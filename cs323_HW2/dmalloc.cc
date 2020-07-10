#define M61_DISABLE 1
#include "dmalloc.hh"
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cinttypes>
#include <cassert>

// You may write code here.
// (Helper functions, types, structs, macros, globals, etc.)
#define canary_val 0xDEADBEEF
#define buffer_val "This string is buffering!This string is buffering!This string is buffering!This string is buffering!This string is buffering!This string is buffering!This string is buffering!This string is bufferin\0"
#include <math.h>
typedef struct node {
    uintptr_t location;
    size_t size;
    char* file;
    long line;
    bool freed;
    node* next;
} node;

typedef struct header {
    long canary;
} header;

typedef struct footer {
    long canary;
    char buffer[200];
} footer;

typedef struct list {
    node* first;
    node* last;
} list;

dmalloc_statistics init();
list create_list();
void add_node(uintptr_t location, size_t size, const char* file, long line);
node* create_node(uintptr_t location, size_t size, const char* file, long line);
void delete_node(node* n);
bool check_boundary(uintptr_t location, size_t size);
size_t check_valid(uintptr_t location, const char* file, long line);
void print_one_leak(node* n);

typedef struct file_info {
    size_t bytes;
    char* file;
    long line;
    file_info* next;
} file_info;

typedef struct storage {
    file_info** table;
} storage;

storage init_storage();
size_t get_hash(const char* file, long line);
void add_entry(const char* file, long line, size_t bytes);
file_info* new_entry(const char* file, long line, size_t bytes);

dmalloc_statistics init() {
    dmalloc_statistics stats;
    stats.nactive = 0;
    stats.active_size = 0;
    stats.ntotal = 0;
    stats.total_size = 0;
    stats.nfail = 0;
    stats.fail_size = 0;
    stats.heap_min = 0;
    stats.heap_max = 0;
    return stats;
}

list create_list() {
    list li;
    li.first = NULL;
    li.last = NULL;
    return li;
}

dmalloc_statistics global_stats = init();
list l = create_list();

void add_node(uintptr_t location, size_t size, const char* file, long line) {
    node* n = create_node(location, size, file, line);
    if (l.first == NULL) {
        l.first = n;
        l.last = n;
    } else {
        l.last->next = n;
        l.last = n;
    }
}

node* create_node(uintptr_t location, size_t size, const char* file, long line) {
    node* n = (node *) malloc(sizeof(node));
    n->location = location;
    n->size = size;
    char* f = (char *) malloc(sizeof(char) * (strlen(file)+1));
    strcpy(f, file);
    n->file = f;
    n->line = line;
    n->freed = false;
    n->next = NULL;
    return n;
}

void delete_node(node* n) {
    free(n->file);
    free(n);
}

bool check_boundary(uintptr_t location, size_t size) {
    header* head = (header *) (location - sizeof(header));
    footer* foot = (footer *) (location + size);
    return((head->canary == canary_val) && (foot->canary == canary_val));
}

size_t check_valid(uintptr_t location, const char* file, long line) {
    if (location < global_stats.heap_min || location > global_stats.heap_max) {
        fprintf(stderr, "MEMORY BUG: %s:%ld: invalid free of pointer %p, not in heap\n", file, line, (void *)location);
        exit(1);
    }
    node* n = l.first;
    bool ever_allocated = false;

    while (n != NULL) {
        if (n->location == location) {
            ever_allocated = true;
        }
        if (n->location == location && !(n->freed)) {
            size_t size = n->size;
            n->freed = true;
            if (!check_boundary(location, size)) {
                fprintf(stderr, "MEMORY BUG: detected wild write during free of pointer %p\n", (void *)location);
                exit(1);
            }
            return(size);
        }
        n = n->next;
    }
    
    if (ever_allocated) {
        if (((header* )(location - sizeof(header)))->canary == canary_val) {
            fprintf(stderr, "MEMORY BUG: %s:%ld: invalid free of pointer %p, double free\n", file, line, (void *)location);
            exit(1);
        }
    }


    fprintf(stderr, "MEMORY BUG: %s:%ld: invalid free of pointer %p, not allocated\n", file, line, (void *)location);
    n = l.first;
    while (n != NULL) {
        if (location > n->location && location < n->location+n->size && !(n->freed)) {
            fprintf(stderr, "  %s:%ld: %ld is %ld bytes inside a %ld byte region allocated here\n", n->file, n->line, location, (location-(n->location)), n->size);
            break;
        }
    }
    exit(1);
}

/// dmalloc_malloc(sz, file, line)
///    Return a pointer to `sz` bytes of newly-allocated dynamic memory.
///    The memory is not initialized. If `sz == 0`, then dmalloc_malloc must
///    return a unique, newly-allocated pointer value. The allocation
///    request was at location `file`:`line`.

void* dmalloc_malloc(size_t sz, const char* file, long line) {
    (void) file, (void) line;   // avoid uninitialized variable warnings
    // Your code here.
    size_t total_size = sizeof(header) + sz + sizeof(footer);
    void* void_ptr;
    if (total_size < sizeof(header) + sizeof(footer)) {
        void_ptr = NULL;
    } else {
        void_ptr = base_malloc(sizeof(header) + sz + sizeof(footer));
    }
    
    if (void_ptr == NULL) {
        global_stats.nfail++;
        global_stats.fail_size += sz;
        return void_ptr;
    } else {
        uintptr_t ptr = (uintptr_t) void_ptr;
        uintptr_t user_ptr = ptr + sizeof(header);
        ((header* ) ptr)->canary = canary_val;
        ((footer* ) (ptr + sizeof(header) + sz))->canary = canary_val;
        strcpy(((footer* ) (ptr + sizeof(header) + sz))->buffer, buffer_val);
        add_node(user_ptr, sz, file, line);
        global_stats.ntotal++;
        global_stats.total_size += sz;
        global_stats.nactive++;
        global_stats.active_size += sz;
        if (global_stats.heap_min == 0) {
            global_stats.heap_min = user_ptr;
        }
        if (global_stats.heap_max == 0 || global_stats.heap_max < user_ptr + sz) {
            global_stats.heap_max = user_ptr + sz;
        }
        if (rand() % 10 == 0) {
            add_entry(file, line, sz);
        }

        return (void* )(user_ptr);
    }
}

/// dmalloc_free(ptr, file, line)
///    Free the memory space pointed to by `ptr`, which must have been
///    returned by a previous call to dmalloc_malloc. If `ptr == NULL`,
///    does nothing. The free was called at location `file`:`line`.

void dmalloc_free(void* ptr, const char* file, long line) {
    (void) file, (void) line;   // avoid uninitialized variable warnings
    uintptr_t loc = (uintptr_t) ptr;
    if (ptr != NULL) {
        size_t size = check_valid(loc, file, line);
        base_free((void *)(loc - sizeof(header)));
        global_stats.nactive--;
        global_stats.active_size -= size;
    }
}


/// dmalloc_calloc(nmemb, sz, file, line)
///    Return a pointer to newly-allocated dynamic memory big enough to
///    hold an array of `nmemb` elements of `sz` bytes each. If `sz == 0`,
///    then must return a unique, newly-allocated pointer value. Returned
///    memory should be initialized to zero. The allocation request was at
///    location `file`:`line`.

void* dmalloc_calloc(size_t nmemb, size_t sz, const char* file, long line) {
    // Your code here (to fix test014).
    size_t mult = nmemb * sz;
    if (mult / sz != nmemb) {
        global_stats.nfail++;
        for (size_t i = 0; i < nmemb; i++) {
            global_stats.fail_size += sz;
        }
        return NULL;
    }
    void* ptr = dmalloc_malloc(nmemb * sz, file, line);
    if (ptr) {
        memset(ptr, 0, nmemb * sz);
    }
    return ptr;
}

/// dmalloc_get_statistics(stats)
///    Store the current memory statistics in `*stats`.

void dmalloc_get_statistics(dmalloc_statistics* stats) {
    // Stub: set all statistics to enormous numbers
    memset(stats, 255, sizeof(dmalloc_statistics));
    // Your code here.
    stats->nactive = global_stats.nactive;
    stats->active_size = global_stats.active_size;
    stats->ntotal = global_stats.ntotal;
    stats->total_size = global_stats.total_size;
    stats->nfail = global_stats.nfail;
    stats->fail_size = global_stats.fail_size;
    stats->heap_min = global_stats.heap_min;
    stats->heap_max = global_stats.heap_max;
}


/// dmalloc_print_statistics()
///    Print the current memory statistics.

void dmalloc_print_statistics() {
    dmalloc_statistics stats;
    dmalloc_get_statistics(&stats);

    printf("alloc count: active %10llu   total %10llu   fail %10llu\n",
           stats.nactive, stats.ntotal, stats.nfail);
    printf("alloc size:  active %10llu   total %10llu   fail %10llu\n",
           stats.active_size, stats.total_size, stats.fail_size);
}

void print_one_leak(node* n) {
    printf("LEAK CHECK: %s:%ld: allocated object %p with size %ld\n", n->file, n->line, (void *)n->location, n->size);
}

/// dmalloc_print_leak_report()
///    Print a report of all currently-active allocated blocks of dynamic
///    memory.
void dmalloc_print_leak_report() {
    node* n = l.first;
    while (n != NULL) {
        if (!(n->freed)) {
            print_one_leak(n);
        }
        n = n->next;
    }
}

/// dmalloc_print_heavy_hitter_report()
///    Print a report of heavily-used allocation locations.


storage hitters = init_storage();

storage init_storage() {
    storage s;
    s.table = (file_info** )malloc(sizeof(file_info*) * 100000);
    return s;
}

size_t get_hash(const char* file, long line) {
    size_t hash = 0;
    int size = 0;
    char* t = (char *)malloc(sizeof(char) * (strlen(file)+1));
    strcpy(t, file);
    for (size_t i = 0; i < strlen(t); i++) {
        hash += pow(32, size) * (int)(t[i]);
        size++;
    }
    hash += line;
    free(t);
    return(hash % 100000);
}

void add_entry(const char* file, long line, size_t bytes) {
    size_t hash = get_hash(file, line);
    file_info* f = hitters.table[hash];
    if (f == NULL) {
        hitters.table[hash] = new_entry(file, line, bytes);
    } else {
        while (f != NULL) {
            if (strcmp(file, f->file) == 0 && line == f->line) {
                f->bytes += bytes;
                break;
            } else {
                f = f->next;
            }
        }
    }
    f->next = new_entry(file, line, bytes);
}

file_info* new_entry(const char* file, long line, size_t bytes) {
    file_info* f = (file_info* ) malloc(sizeof(file_info));
    char* fi = (char *) malloc(sizeof(char) * (strlen(file)+1));
    strcpy(fi, file);
    f->file = fi;
    f->line = line;
    f->bytes = bytes;
    f->next = NULL;
    return f;
}

void dmalloc_print_heavy_hitter_report() {
    // Your heavy-hitters code here
    unsigned long long threshold = global_stats.total_size / 50;
    file_info* f;
    for (int i = 0; i < 100000; i++) {
        while (f != NULL) {
            if (f->bytes >= threshold) {
                float percent = f->bytes / (threshold*5);
                printf("HEAVY HITTER: %s:%ld:  %ld bytes (~%f%%)", f->file, f->line, f->bytes, percent);
            }
            f = f->next;
        }
    }
}
