#include <stdio.h>
#include "util.h"

int main() {
/*    char *str_ptr = "  te st  ";
    char out[20] = {0};
    a_trim(str_ptr, out);
    printf("|%s|\n", out);*/

    char *src_ptr = "/home/test.log";
    char *dst_ptr = "/home/new_dir/test.log";
    int ret = copy_file(src_ptr, dst_ptr);
    printf("ret is %d\n", ret);

    return 0;
}