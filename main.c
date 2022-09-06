#include <stdio.h>
#include <stdint.h>
#include <malloc.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdbool.h>

void perform();

int main() {
    perform();
    return 0;
}

struct array_int {
    int64_t* data;
    size_t size;
};

struct maybe_array_int {
    struct array_int value;
    bool valid;
};

struct maybe_int64 {
    int64_t value;
    bool valid;
};

const struct maybe_int64 none_int64 = { 0 };

size_t read_size() {
    size_t i = 0;
    scanf("%d", &i);  //should be "zu"
    return i;
}

void array_int_fill( int64_t* array, size_t sz );

struct array_int array_int_read();
struct maybe_int64 array_int_get( struct array_int a, size_t i );
void array_int_print( struct array_int array );
struct maybe_int64 array_int_min( struct array_int array );
void array_int_free( struct array_int a );

void array_int_normalize( struct array_int array, int64_t m ) {
    for (size_t i = 0; i < array.size; i = i + 1) {
        array.data[i] = array.data[i] - m;
    }
}

struct array_int array_int_read() {
    const size_t size = read_size();
    if (size > 0) {
        int64_t* array = malloc( sizeof(int64_t) * size);
        array_int_fill( array, size );
        return (struct array_int) { .data = array, .size = size };
    }
    else return (struct array_int) {0};
}

struct maybe_int64 array_int_get( struct array_int a, size_t i ) {
    if (a.size > i) return (struct maybe_int64) { .value = (a.data)[i], .valid = true};
    return none_int64;
}

int64_t read_int64() {
    int64_t x = 0;
    scanf("%" SCNd64, &x);
    return x;
}

void array_int_fill( int64_t* array, size_t sz ) {
    for( size_t i = 0; i < sz; i = i + 1 ) {
        array[i] = read_int64();
    }
}

void array_int_print( struct array_int array ) {
    for (size_t i = 0; i < array.size; i = i + 1) {
        printf("%" PRId64 " " , array_int_get( array, i).value);
    }
}

struct maybe_int64 array_int_min( struct array_int array ) {
    struct maybe_int64 minimum = {9223372036854775807, false};
    if (array.size > 0) {
        for (int64_t i = 0; i < array.size; i++){
            if ((array.data)[i] < minimum.value) {
                minimum.value = (array.data)[i];
                minimum.valid = true;
            }
        }
    }
    return minimum;
}

void array_int_free( struct array_int a ) {
    if ( a.size > 0 ) {
        free(a.data);
        a.size = 0;
    }
}

struct maybe_array_int some_array_int(struct array_int array) {
    return (struct maybe_array_int) { array, true };
}
const struct maybe_array_int none_array_int = { {NULL, 0}, false };

struct array_array_int {
    struct array_int* data;
    size_t size;
};

struct maybe_array_int array_array_int_get_row( struct array_array_int a, size_t i ) {
    if ( 0 <= i && i < a.size ) { return some_array_int( a.data[i] ); }
    else { return none_array_int; }
}

struct array_array_int array_array_int_read() {
    size_t rows = read_size();
    struct array_int* sizes = malloc(sizeof(struct array_int) * rows);
    for (size_t i = 0; i < rows; i++){
        sizes[i] = array_int_read();
    }
    return (struct array_array_int) {.data = sizes, .size = rows};
}


void array_array_int_print( struct array_array_int array) {
    for (size_t i = 0; i < array.size; i++) {
        array_int_print(array.data[i]);
        if (i != array.size - 1) printf("\n");
    }
}

struct maybe_int64 array_array_int_min( struct array_array_int array ) {
    struct maybe_int64 minimum = {9223372036854775807, false};
    for (size_t i = 0; i < array.size; i++) {
        struct maybe_int64 local_minimum = array_int_min(array.data[i]);
        if ((local_minimum.value < minimum.value) && (local_minimum.valid == true)) {
            minimum.value = local_minimum.value;
            minimum.valid = true;
        }
    }
    return minimum;
}

void array_array_int_normalize( struct array_array_int array, int64_t m) {
    for (size_t i = 0; i < array.size; i = i + 1) {
        const struct maybe_array_int cur_row = array_array_int_get_row( array, i );
        if (cur_row.valid) {
            array_int_normalize( cur_row.value, m );
        }
    }
}

void array_array_int_free( struct array_array_int array ) {
    for (size_t i = 0; i < array.size; i++){
        array_int_free(array.data[i]);
    }
    free(array.data);
}

void perform() {
    struct array_array_int array = array_array_int_read();
    struct maybe_int64 m = array_array_int_min( array );
    if (m.valid) {
        array_array_int_normalize( array, m.value );
        array_array_int_print( array );
    }
    array_array_int_free( array );
}