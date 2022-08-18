/*
 * @Author: Yimning 1148967988@qq.com
 * @Date: 2022-08-16 03:18:36
 * @Description: 要获取POST方式提交的数据，需要首先从环境变量 CONTENT_LENGTH中获取数据长度，
 * 然后再从标准输入中去读该长度个字节，最后再按照解析GET方式数据一样去解析数据
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <sys/stat.h>
#include "getparameter.h"


#if 0
int main(int argc, const char *argv[])
{
    char     *data = NULL, *data_len_str = NULL;
    int      data_len, read_len;
    fprintf(stdout, "Content-Type: text/html\r\n\r\n");
    data_len_str = getenv("CONTENT_LENGTH");
    if (NULL == data_len_str) {
        data_len_str = "";
    }
    data_len = atoi(data_len_str);
    if (data_len < 0) {
       return -1;
    }
    data = (char *)malloc(data_len);
    if (NULL == data) {
        return -1;
    }
    read_len = fread(data, 1, data_len, stdin);
    if (read_len != data_len) {
        return -1;
    }
    fprintf(stdout, "<HTML>\n");
    fprintf(stdout, "<HEAD>\n");
    fprintf(stdout, "<TITLE>CGI解析POST数据</TITLE>\n");
    fprintf(stdout, "<HEAD>\n");
    fprintf(stdout, "<BODY>\n");
    fprintf(stdout, "<H3>以下为解析后数据</H3>\n");
    sln_cgi_content_parse(data, data_len);
    fprintf(stdout, "本网页由CGI自动生成!\n");
    fprintf(stdout, "</BODY>");
    fprintf(stdout, "</HTML>");
    free(data);
    return 0;
}

#endif