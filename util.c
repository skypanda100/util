//
// Created by zhengdongtian on 4/22/19.
//

#include "util.h"

int v_system(const char *command)
{
    pid_t pid;
    int status;

    if(command == NULL)
    {
        return -1;
    }

    if((pid = vfork()) < 0)
    {
        status = -1;
    }
    else if(pid == 0)
    {
        execl("/bin/sh", "sh", "-c", command, (char *)0);
        _exit(127);
    }
    else
    {
        while(waitpid(pid, &status, 0) < 0)
        {
            if(errno != EINTR){
                status = -1;
                break;
            }
        }
    }

    return status;
}

int make_dir(const char *path_ptr)
{
    char str[PATH_MAX] = {0};
    strcpy(str, path_ptr);
    int len = strlen(str);

    for(int i = 0;i < len;i++)
    {
        if(str[i] == '/')
        {
            str[i] = '\0';
            if(strlen(str) == 0)
            {
                str[i] = '/';
                continue;
            }
            if(access(str, F_OK) != 0)
            {
                if(mkdir(str, 0777) != 0)
                {
                    return -1;
                }
            }
            str[i] = '/';
        }
    }

    if(len > 0 && access(str, F_OK) != 0)
    {
        if(mkdir(str, 0777) != 0)
        {
            return -1;
        }
    }

    return 0;
}


int bz2_decompress(const char *src_file_path_ptr, const char *dst_file_path_ptr)
{
    char cmd[1024] = {0};
    sprintf(cmd, "bzip2 -d -c %s > %s", src_file_path_ptr, dst_file_path_ptr);

    return v_system(cmd);
}

void l_trim(const char *input_ptr, char *output_ptr)
{
    assert(input_ptr != NULL);
    assert(output_ptr != NULL);
    assert(output_ptr != input_ptr);
    for(;*input_ptr != '\0' && isspace(*input_ptr);++input_ptr)
    {
        ;
    }
    strcpy(output_ptr, input_ptr);
}

void a_trim(const char *input_ptr, char *output_ptr)
{
    char *p = NULL;
    assert(input_ptr != NULL);
    assert(output_ptr != NULL);
    l_trim(input_ptr, output_ptr);
    for(p = output_ptr + strlen(output_ptr) - 1;p >= output_ptr && isspace(*p);--p)
    {
        ;
    }
    *(++p) = '\0';
}

int reg_exec(const char *input_ptr, const char *pattern_ptr, int *so_ptr, int *eo_ptr)
{
    regmatch_t pm[1];
    size_t nmatch = 1;
    regex_t rgx;

    if(regcomp(&rgx, pattern_ptr, REG_EXTENDED) != 0)
    {
        regfree(&rgx);
        return -1;
    }

    if(so_ptr != NULL && eo_ptr != NULL)
    {
        if(regexec(&rgx, input_ptr, nmatch, pm, 0) == 0)
        {
            *so_ptr = pm[0].rm_so;
            *eo_ptr = pm[0].rm_eo;
        }
        else
        {
            *so_ptr = -1;
            *eo_ptr = -1;
            regfree(&rgx);
            return -1;
        }
    }
    else
    {
        if(regexec(&rgx, input_ptr, 0, NULL, 0) != 0)
        {
            regfree(&rgx);
            return -1;
        }
    }

    regfree(&rgx);

    return 0;
}

void sort_by_ascii(char **c_ptr_ptr, int len, int is_desc)
{
    for(int i = len - 1;i >= 0;i--)
    {
        for(int j = len - 1;j > len - i - 1;j--)
        {
            char *cur_ptr = c_ptr_ptr[j];
            char *nxt_ptr = c_ptr_ptr[j - 1];

            if(is_desc)
            {
                if(strcmp(cur_ptr, nxt_ptr) > 0)
                {
                    char *p_tmp = nxt_ptr;
                    c_ptr_ptr[j - 1] = cur_ptr;
                    c_ptr_ptr[j] = p_tmp;
                }
            }
            else
            {
                if(strcmp(cur_ptr, nxt_ptr) < 0)
                {
                    char *p_tmp = nxt_ptr;
                    c_ptr_ptr[j - 1] = cur_ptr;
                    c_ptr_ptr[j] = p_tmp;
                }
            }
        }
    }
}

int list_dir(const char *path_ptr, char ***dir_ptr_ptr_ptr)
{
    DIR *d;
    struct dirent *file;
    struct stat st;
    int dir_count = 0;

    if(!(d = opendir(path_ptr)))
    {
        printf("open dir %s error\n", path_ptr);
        return dir_count;
    }

    while((file = readdir(d)) != NULL)
    {
        if(strncmp(file->d_name, ".", 1) == 0)
        {
            continue;
        }
        char sub_path[PATH_MAX] = {0};
        sprintf(sub_path, "%s/%s", path_ptr, file->d_name);
        if(stat(sub_path, &st) == 0 && S_ISDIR(st.st_mode))
        {
            dir_count++;
            *dir_ptr_ptr_ptr = realloc(*dir_ptr_ptr_ptr, dir_count * sizeof(char *));
            *(*dir_ptr_ptr_ptr + (dir_count - 1)) = strdup(file->d_name);
        }
    }
    closedir(d);
    return dir_count;
}

int list_file(const char *path_ptr, char ***file_ptr_ptr_ptr)
{
    DIR *d;
    struct dirent *file;
    struct stat st;
    int file_count = 0;

    if(!(d = opendir(path_ptr)))
    {
        printf("open dir %s error\n", path_ptr);
        return file_count;
    }

    while((file = readdir(d)) != NULL)
    {
        if(strncmp(file->d_name, ".", 1) == 0)
        {
            continue;
        }
        char sub_path[PATH_MAX] = {0};
        sprintf(sub_path, "%s/%s", path_ptr, file->d_name);
        if(stat(sub_path, &st) == 0)
        {
            if(S_ISDIR(st.st_mode))
            {
                file_count += list_file(sub_path, file_ptr_ptr_ptr);
            }
            else if(S_ISREG(st.st_mode))
            {
                file_count++;
                *file_ptr_ptr_ptr = realloc(*file_ptr_ptr_ptr, file_count * sizeof(char *));
                *(*file_ptr_ptr_ptr + (file_count - 1)) = strdup(sub_path);
            }

        }
    }
    closedir(d);

    return file_count;
}
