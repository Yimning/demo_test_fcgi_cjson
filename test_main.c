/*
 * @Author: Yimning 1148967988@qq.com
 * @Date: 2022-08-15 04:56:40
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <sys/stat.h>

#include "getparameter.h"

// 被解析的JSON数据包
char text[] = "{\"timestamp\":\"2021-11-19T08:50:11\",\"value\":321,\"valf\":321.0998,\"bool\":true}";

int main()
{
    static char top_html_str[1024 * 1024];   // 1024kB
    static char left_html_str[1024 * 1024];  // 1024kB
    static char right_html_str[1024 * 1024]; // 1024kB
    int ret = 0;
    int count = 0;

    char *pstr = NULL;
    char *pstr1 = NULL;
    // char *query_string = getenv("QUERY_STRING");
    char *query_string = "CMD=LOGON&SELECT=0&USERNAME=user&PASSWORD=123";
    fprintf(stdout, "<HTML>\n");
    fprintf(stdout, "<HEAD>\n");
    fprintf(stdout, "<TITLE>CGI解析GET数据</TITLE>\n");
    fprintf(stdout, "<HEAD>\n");
    fprintf(stdout, "<BODY>\n");
    fprintf(stdout, "<H3>以下为解析后数据</H3>\n");

    sln_cgi_content_parse(query_string);
    fprintf(stdout, "Content-Type:text/html;charset=utf-8<br/>");
    fprintf(stdout, "本网页由CGI自动生成!\n");
    fprintf(stdout, "</BODY>");
    fprintf(stdout, "</HTML>\n");

    pstr =(char *)malloc(sizeof(100));
    bzero(pstr, sizeof(pstr));
    pstr = getparameter(query_string, 1);
    fprintf(stdout, "--get[1] = %s\n", pstr);
    free(pstr);
#if 1
    pstr1 = getOneChar(query_string);
    fprintf(stdout, "getOneChar[0] = %c\n", *(pstr1 + 1));

    char *str =NULL;
    str = cjson_cgi_content_parse(query_string);

    printf("------------------------ = %s\n", str);


#endif
    /* 组装JSON数据包 */

    // 创建JSON Object
    cJSON *root = cJSON_CreateObject();
    // 加入节点（键值对），节点名称为value，节点值为123.4
    cJSON_AddNumberToObject(root, "value", 123.4);
    // 打印JSON数据包
    char *out = cJSON_Print(root);
    printf("%s\n", out);
    // 释放内存
    cJSON_Delete(root);
    
    free(out);

    /* 解析JSON数据包 */
    cJSON *json, *json_value, *json_timestamp;
    // 解析数据包
    json = cJSON_Parse(text);
    if (!json)
    {
        printf("Error before: [%s]\n", cJSON_GetErrorPtr());
        
    }  
    else
    {
        // 解析开关值
        json_value = cJSON_GetObjectItem(json, "value");
        if (json_value->type == cJSON_Number)
        {
            // 从valueint中获得结果
            printf("value:%d\r\n", json_value->valueint);
        }
        // 解析时间戳
        json_timestamp = cJSON_GetObjectItem(json, "timestamp");
        if (json_timestamp->type == cJSON_String)
        {
            // valuestring中获得结果
            printf("%s\r\n", json_timestamp->valuestring);
        }  
        // 释放内存空间
        cJSON_Delete(json);
    }
 
    char *pchar = NULL;
    double f = 0;
    int a = 0;
    bool bl = 0;

    pchar = (char *) cjson_cgi_GET_getStrValue("timestamp");
    printf("cjson_cgi_getStrValue:%s\r\n", pchar);

    pchar = (char *) cjson_cgi_GET_getStrValue("value");  
    printf("cjson_cgi_getStrValue:%s\r\n", pchar);


    f = cjson_cgi_GET_getDoubleValue("valf");
    printf("cjson_cgi_getDoubleValue:%lf\r\n", f);

    a = cjson_cgi_GET_getIntValue("value");
    printf("cjson_cgi_getIntValue:%d\r\n", a);
    

    bl = cjson_cgi_GET_getBoolValue("bool");
    printf("cjson_cgi_getBoolValue:%d\r\n", bl);

    char *jsonstr = "{\"username\":\"admin\",\"password\":0,\"float\":123.056,\"bool\":true}";

    printf("cJSON_GetStringValue=%s\r\n",cJSON_GetStrValue(jsonstr,"username"));
    printf("cJSON_GetIntValue=%d\r\n",cJSON_GetIntValue(jsonstr,"password"));

    printf("cJSON_GetDoubleValue=%f\r\n",cJSON_GetDoubleValue(jsonstr,"float"));

    printf("cJSON_GetBoolValue=%d\r\n",cJSON_GetBoolValue(jsonstr,"bool")); 
    

    return 0;
}