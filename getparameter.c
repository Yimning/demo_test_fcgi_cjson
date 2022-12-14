/*
================================================================
 * Copyright © 2022 Yimning. All rights reserved.
 *   
 * @File Name: getparameter.c
 * @Author: Yimning
 * @Email: 1148967988@qq.com
 * @Created Time: 2022-08-11 00:12:47
 * @Description:getparameter
 *
================================================================
*/

#include "getparameter.h"

arrT* getOneChar(char *query_string)
{
    /*  eg.    CMD=LOGON&SELECT=0&USERNAME=user&PASSWORD=root */
    char *pstr= (char *)malloc(strlen(query_string) + 1);
    int len = strlen(query_string); 
    char *p = query_string;
    for (int m = 0; m < len; m++)
    {
        pstr[m] = p[m];   // ptr[m] = *(p+m);
    }
    //fprintf(stdout, "getOneChar = %c\n",pstr[1]);
    return (arrT*)pstr;
}



char* getparameter(char *query_string, int idx)
{
    /*  eg.    CMD=LOGON&SELECT=0&USERNAME=user&PASSWORD=root */
    int len = strlen(query_string);
    char *index = NULL;
    int i = 0;
    char *get[100] = {0};    //一个(char *)类型的指针数组

    char *pstr = (char *)malloc(sizeof(char)*(len + 1));
    memset(pstr,0,(len+1) * sizeof(char));

    char *str = (char *)malloc(sizeof(char)*(len + 1));
    memcpy(str, query_string, sizeof(char)*(len + 1));

    while (NULL != (index = strchr(str, '&')))
    {
        get[i] = (char *)malloc(index - str + 1);

        memset(get[i],0,(index - str + 1) * sizeof(char));

        /* get[i++]未初始化时，是不可以用strcpy对get[i]进行赋值的。因为这将把数据复制到地址“不确定”的空间 */
        strncpy(get[i++], str, index - str);    //get the string before "&"

        str = index + 1; // remove "&" and get new str. <===>  strncpy(str, index + 1, strlen(str) - (index - str) + 1); 
    }
    // if not find '&', then the last char is end
    get[i] = (char *)malloc(strlen(str) + 1);  
    strncpy(get[i++], str, strlen(str) + 1);
    
    if((idx > 0)&&(idx <= i)){
        strcpy(pstr,get[idx-1]);
    }else{
        sprintf(pstr, "数组索引值异常!最小索引为1,最大索引值为%d",i);
    }
    //fprintf(stdout, "get[%d] = %s\n",idx,pstr);
    for (int m = 0; m < i; m++)
    {
        //printf("%d---%s\n",m, get[m]);    //打印全部解析结果
        free(get[m]);
    }
    return pstr;    //注意在主函数调用完之后要释放内存,如free(p);
}

/*该函数主要做解码工作*/

int sln_string_unescape(char **attr, const char *src, int len)
{
    int pos = 0, src_pos = 0;
    char escape_char = '\0';
    char *p = (char *)malloc((len+1) * sizeof(char));
    memset(p,0,(len+1) * sizeof(char));

    if (NULL == p)
    {
        //perror("point error!\r\n");
        return -1;
    }
    while (src_pos < len)
    {
        switch (src[src_pos])
        {
            case '%':                                        /*表单属性中存放的数据中十六进制数“%HH“转换成相应的字符*/
                escape_char = HexCharToBinBinChar(src[src_pos + 1]); /*sln_hex2bin() 函数负责将字符转换为其对应的ASCII码*/
                escape_char <<= 4;
                escape_char |= HexCharToBinBinChar(src[src_pos + 2]);
                p[pos] = escape_char;
                src_pos += 3;
                break;
            case '+': /*表单属性中有存放“+”的，则将其转换为空格*/
                p[pos] = '\x20';
                src_pos++;
                break;
            default:
                p[pos] = src[src_pos];
                src_pos++;
                break;
        }
        pos++;
    }
    *attr = p;
    return 0;
}

/*该函数解析传入的表单数据并输出解析结果*/

/*
usage :  sln_cgi_content_parse(query_string);
*/
int sln_cgi_content_parse(char *query_string)
{
    char *start = NULL, *end = NULL;
    char *attr = NULL, *value = NULL;
    int attr_len, value_len;
    int ret = 0;
    start = (char *)query_string;
    
    if(query_string == NULL)  return NULL;

    int len = strlen(query_string);

/*  eg.    CMD=LOGON&SELECT=0&USERNAME=user&PASSWORD=root */

    while ((end = strchr(start, '=')))
    {
        attr_len = end - start;

        ret = sln_string_unescape(&attr, start, attr_len);   // get and return the string before"="
  
        start = end + 1;                     //the Value of current position is  "="

        // if not find '&', then the last char is end
        end = strchr(start, '&');
        if(NULL == end)
        {
            end = (char *)&query_string[len];   //point to the end of the string.
        }

        value_len = end - start;     // get and return the string after"="
        ret = sln_string_unescape(&value, start, value_len);
        start = end + 1;

        if (end - query_string > len)
        {
            break;
        }
        //fprintf(stdout, "<P>NAME: %s, VALUE: %s</P>\n", attr, value);
    }
    return 0;
}

/* 
    每个程序都包含一或多个进程运行，而每个进程都有自己的进程堆。
    虽然程序中动态申请的空间如果没有手动释放，在程序运行结束，也就是所有进程消亡后，
    系统也会将进程中申请的所有的内存资源全部释放。但是如果一旦程序运行时间过程，或者程序占用内存过大，
    进程就会耗尽系统所有内存，最终造成内存泄漏。
    所以避免程序出现内存泄漏的最好办法是，当使用动态空间完毕后，我们应该人为释放内存空间。
*/

char* cjson_cgi_content_parse(char *query_string)
{
    char *start = NULL, *end = NULL;
    char *attr = NULL, *value = NULL;
    int attr_len, value_len;
    start = (char *)query_string;
    char *pstr= NULL;
    if(query_string == NULL){
        return NULL;
    }
    
    int len = strlen(query_string);

    // 创建JSON Object
    cJSON *root = cJSON_CreateObject();

/*  eg.    CMD=LOGON&SELECT=0&USERNAME=user&PASSWORD=root */

    while ((end = strchr(start, '=')))
    {
        attr_len = end - start;

        sln_string_unescape(&attr, start, attr_len);   // get and return the string before"="

        start = end + 1;                     //the Value of current position is  "="

        // if not find '&', then the last char is end
        end = strchr(start, '&');
        if(NULL == end)
        {
            end = (char *)&query_string[len];   //point to the end of the string.
        }    

        value_len = end - start;     // get and return the string after"="
        sln_string_unescape(&value, start, value_len);
        start = end + 1;
        if (end - query_string > len)
        {
            break;
        }
        //fprintf(stdout, "<P>NAME: %s, VALUE: %s</P>\n", attr, value);
        // 加入节点（键值对），节点名称为value，节点值为
        //cJSON_AddNumberToObject(root, attr, value);
        cJSON_AddStringToObject(root, attr, value);
        // 打印JSON数据包
        pstr = cJSON_Print(root);
    }
    //printf("%s\n", pstr);
    // 释放内存
    cJSON_Delete(root);
    //free(pstr);
    return pstr; 
}


/*
================================================================
 *
 * @Description:CJSON
 *
================================================================
*/
char *cJSON_GetStrValue(char *jsonString ,const char *const key)
{
    /* 解析JSON数据包 */
    cJSON *json, *json_value;
    char *pstr; 
    if(!jsonString) return NULL;
    pstr = (char *)malloc(strlen(jsonString)+1);
    memset(pstr,0,strlen(jsonString)+1);

    json = cJSON_Parse(jsonString);

    if (!json)
    {
        sprintf(pstr,"%s", cJSON_GetErrorPtr());
    }
    else
    {
        // 解析值---string
        json_value = cJSON_GetObjectItem(json, key); 
        if ((json_value != NULL)&&(json_value->type == cJSON_String))
        {
            //valuestring中获得结果
            //return json_value->valuestring;
            sprintf(pstr,"%s", json_value->valuestring);
        }

        if ((json_value != NULL)&&(json_value->type == cJSON_Number)) // 解析值---int/double
        {
            sprintf(pstr,"%lf", json_value->valuedouble);
        } 

        // 释放内存空间
        cJSON_Delete(json);
    }
    return pstr;
}

int cJSON_GetIntValue(char *jsonString ,const char *const key)
{
    /* 解析JSON数据包 */
    cJSON *json, *json_value;
    int ret = -1;

    if(!jsonString) return NULL;

    json = cJSON_Parse(jsonString);

    if (json)
    {
        json_value = cJSON_GetObjectItem(json, key); 
        if ((json_value != NULL)&&(json_value->type == cJSON_Number)) // 解析值---int
        {
            ret = json_value->valueint;
        } 

        // 释放内存空间
        cJSON_Delete(json);
    }
    return ret;
}

double cJSON_GetDoubleValue(char *jsonString, const char *const key)
{
    /* 解析JSON数据包 */
    cJSON *json, *json_value;
    double ret = -1;

    if(!jsonString) return ret;

    json = cJSON_Parse(jsonString);
    if (json)
    {
        json_value = cJSON_GetObjectItem(json, key); 
        if ((json_value != NULL)&&(json_value->type == cJSON_Number)) // 解析值---double
        {
            ret = json_value->valuedouble;
        } 

        // 释放内存空间
        cJSON_Delete(json);
    }
    return ret;
}


bool cJSON_GetBoolValue(char *jsonString, const char *const key)
{
    /* 解析JSON数据包 */
    cJSON *json, *json_value;
    bool ret;
    if(!jsonString) return NULL;

    json = cJSON_Parse(jsonString);
    if (json)
    {
        json_value = cJSON_GetObjectItem(json, key); 
        if ((json_value != NULL)&&(json_value->type == cJSON_False)) // 解析值---bool---false
        {
            ret = false;
        }
        if ((json_value != NULL)&&(json_value->type == cJSON_True)) // 解析值---double---true
        {
            ret = true;
        } 

        // 释放内存空间
        cJSON_Delete(json);
    }
    return ret;
}



/*
================================================================
 *
 * @Description:GET method
 *
================================================================
*/
char* cjson_cgi_GET_getStrValue(const char *const key)
{
    char *pstr; 
    char *query_string = getenv("QUERY_STRING");
    
    char *jsonString = cjson_cgi_content_parse(query_string);
    
    if((jsonString == NULL) || (key == NULL)) return NULL;
    pstr = cJSON_GetStrValue(jsonString , key);

    if(pstr == NULL)  return NULL;

    return pstr;
}



int cjson_cgi_GET_getIntValue(const char *const key)
{
    int ret = -1;
    char *query_string = getenv("QUERY_STRING");
    
    char *jsonString = cjson_cgi_content_parse(query_string);
    
    if((!jsonString) || (!key)) return NULL;
    ret = cJSON_GetIntValue(jsonString , key);

    return ret;
}

double cjson_cgi_GET_getDoubleValue(const char *const key)
{
    double ret = -1;
    char *query_string = getenv("QUERY_STRING");
    
    char *jsonString = cjson_cgi_content_parse(query_string);
    
    if((!jsonString) || (!key)) return ret;
    ret = cJSON_GetIntValue(jsonString , key);

    return ret;
}

bool cjson_cgi_GET_getBoolValue(const char *const key)
{
    bool ret;
    char *query_string = getenv("QUERY_STRING");
    char *jsonString = cjson_cgi_content_parse(query_string);
 
    if((!jsonString) || (!key)) return ret;
    ret = cJSON_GetIntValue(jsonString , key);

    return ret;
}


/*
================================================================
 *
 * @Description:POST method
 *
================================================================
*/

char* cjson_cgi_POST_getStrValue(char *parm_string,const char *const key)
{
    /* 解析JSON数据包 */
    cJSON *json, *json_value;
    char *pstr; 
    
    char *jsonString = cjson_cgi_content_parse(parm_string);
    if(!jsonString) return NULL;
    pstr = (char *)malloc(strlen(jsonString)+1);
    memset(pstr,0,strlen(jsonString)+1);

    json = cJSON_Parse(jsonString);

    if (!json)
    {
        sprintf(pstr,"%s", cJSON_GetErrorPtr());
    }
    else
    {
        // 解析值---string
        json_value = cJSON_GetObjectItem(json, key); 
        if ((json_value != NULL)&&(json_value->type == cJSON_String))
        {
            //valuestring中获得结果
            //return json_value->valuestring;
            sprintf(pstr,"%s", json_value->valuestring);
        }

        if ((json_value != NULL)&&(json_value->type == cJSON_Number)) // 解析值---int/double
        {
            sprintf(pstr,"%lf", json_value->valuedouble);
        } 

        // 释放内存空间
        cJSON_Delete(json);
    }
    return pstr;
}


int cjson_cgi_POST_getIntValue(char *parm_string,const char *const key)
{
    /* 解析JSON数据包 */
    cJSON *json, *json_value;
    int ret = -1;
    char *jsonString = cjson_cgi_content_parse(parm_string);
    if(!jsonString) return NULL;

    json = cJSON_Parse(jsonString);

    if (json)
    {
        json_value = cJSON_GetObjectItem(json, key); 
        if ((json_value != NULL)&&(json_value->type == cJSON_Number)) // 解析值---int
        {
            ret = json_value->valueint;
        } 

        // 释放内存空间
        cJSON_Delete(json);
    }
    return ret;
}

double cjson_cgi_POST_getDoubleValue(char *parm_string,const char *const key)
{
    /* 解析JSON数据包 */
    cJSON *json, *json_value;
    double ret = -1;
    char *jsonString = cjson_cgi_content_parse(parm_string);

    if(!jsonString) return ret;

    json = cJSON_Parse(jsonString);
    if (json)
    {
        json_value = cJSON_GetObjectItem(json, key); 
        if ((json_value != NULL)&&(json_value->type == cJSON_Number)) // 解析值---double
        {
            ret = json_value->valuedouble;
        } 

        // 释放内存空间
        cJSON_Delete(json);
    }
    return ret;
}

bool cjson_cgi_POST_getBoolValue(char *parm_string,const char *const key)
{
    /* 解析JSON数据包 */
    cJSON *json, *json_value;
    bool ret;
    char *jsonString = cjson_cgi_content_parse(parm_string);
    if(!jsonString) return NULL;

    json = cJSON_Parse(jsonString);
    if (json)
    {
        json_value = cJSON_GetObjectItem(json, key); 
        if ((json_value != NULL)&&(json_value->type == cJSON_False)) // 解析值---bool---false
        {
            ret = false;
        }
        if ((json_value != NULL)&&(json_value->type == cJSON_True)) // 解析值---double---true
        {
            ret = true;
        } 

        // 释放内存空间
        cJSON_Delete(json);
    }
    return ret;
}


#if 0

#define FILENAME_MAX_LEN 256
#define ATTR_MAX_LEN 256
#define VALUE_MAX_LEN 256


int sln_cgi_input_multi_content_parse(const char *content_input,
                                            int content_length, const char *boundary)
{
    char        attr[ATTR_MAX_LEN], value[VALUE_MAX_LEN],
                filename[FILENAME_MAX_LEN], tfilename[FILENAME_MAX_LEN],
                tmpname[FILENAME_MAX_LEN];
    char        *start, *end, *pname, *quote;
    int         boundary_len, attr_len, value_len, filename_len, file_size = 0;
    FILE        *fp = NULL;
 
    boundary_len = strlen(boundary);
 
    start = (char *)content_input;
 
    if ((end = strstr(start, boundary))) {
        start = end + boundary_len;
    }
 
    while (start) { // search boundary
        attr[0] = '\0', filename[0] = '\0', filename[0] = '\0', tfilename[0] = '\0';
        if (0 == memcmp(start, "--\r\n", 4)) { //reach the end
            break;
        } else {
            start += 2; //strlen("\r\n") = 2;
        }
 
        /*
         * betwen the start and end is a form data,
         * "name" and "filename"(probably) is in it
         */
        end = strstr(start, "\r\n\r\n");
        if (NULL == end) {
            break;
        }
        *end = '\0';
 
        /* get attr name */
        pname = strstr(start, "name=\"");
        if (NULL == pname) {
            start = end + 1;
            continue;
        }
        quote = strchr(pname + 6, '\"');    //strlen("name=\"") = 6;
        if (NULL == quote) {
            start = end + 1;
            continue;
        }
 
        attr_len = quote - (pname + 6);
        strncpy(attr, pname + 6, attr_len);
        attr[attr_len] = '\0';
        printf("<p>attr(len=%d): %s</p>\n", attr_len, attr);
 
        /* try to get filename */
        pname = strstr(start, "filename=\"");
        if (NULL != pname) {
            quote = strchr(pname + 10, '\"'); //strlen("filename=\"") = 10;
            if (NULL == quote) {
                start = end + 1;
                continue;
            }
            filename_len = quote - (pname + 10);
            strncpy(filename, pname + 10, filename_len);
            filename[filename_len] = '\0';
            printf("<p>filename(len=%d): %s</p>\n", filename_len, filename);
        }
 
        start = end + 4;                //strlen("\r\n\r\n") = 4;
        if ('\0' == filename[0]) {          // not file, to get value
            end = strstr(start, boundary);
            if (NULL != end) {
                value_len = (end-4) - start; //"\r\n--"
                if (value_len > sizeof(value)) {
                    value_len = sizeof(value) - 1;
                }
                strncpy(value, start, value_len);
                value[value_len - 1] = '\0';
                printf("<p>value(len=%d): %s</p>\n", value_len, value);
                start = end + strlen(boundary);
            }
        } else {                            //is file, to get file content
            end = sln_memsearch(start, content_length - (start-content_input), boundary, boundary_len);
            if (NULL != end) {
                file_size = (end-4) - start; //"\r\n--"
                //sln_dump_mem((unsigned char *)end, 8);
 
                // generate tmp file 
                if (NULL == tmpnam(tmpname)) {
                    snprintf(tfilename, FILENAME_MAX_LEN, "%s", filename);
                } else {
                    snprintf(tfilename, FILENAME_MAX_LEN, "%s", tmpname);
                }
 
                // write file content to tmp file
                file_size =
                    file_size > SLN_MAX_CONTENT_LEN ? SLN_MAX_CONTENT_LEN : file_size;
                fp = fopen(tfilename, "w+");
                if (NULL != fp) {
                    fwrite(start, 1, file_size, fp);
                    fclose(fp);
                } else {
                    //SLN_CGI_ERR("fopen <%s> failed!\n", tfilename);
                }
                printf("<p>tfilename: %s, file_size: %d</p>\n", tfilename, file_size);
                //sln_dump_mem((unsigned char *)start, file_size);
                start = end + strlen(boundary);
            }
        }
    }
 
    return 0;
}

#endif // 0

/* 把ASCII字符转换为16进制 */
unsigned char CharToHex(unsigned char bHex)
{
    if ((bHex >= 0) && (bHex <= 9))
    {
        bHex += 0x30;
    }
    else if ((bHex >= 10) && (bHex <= 15)) // Capital
    {
        bHex += 0x37;
    }
    else
    {
        bHex = 0xff;
    }
    return bHex;
}

/* 把16进制转换为ASCII字符 */
unsigned char HexToChar(unsigned char bChar)
{
    if ((bChar >= 0x30) && (bChar <= 0x39))
    {
        bChar -= 0x30;
    }
    else if ((bChar >= 0x41) && (bChar <= 0x46)) // Capital
    {
        bChar -= 0x37;
    }
    else if ((bChar >= 0x61) && (bChar <= 0x66)) // littlecase
    {
        bChar -= 0x57;
    }
    else
    {
        bChar = 0xff;
    }
    return bChar;
}


/********************************************************************************
input:
	c:单个字符('0'~'9' 'a'~'f', 'A'~'F')
output:
	单个字符转化为单个字符
********************************************************************************/
static uint8_t HexCharToBinBinChar(char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	else if (c >= 'a' && c <= 'z')
		return c - 'a' + 10;
	else if (c >= 'A' && c <= 'Z')
		return c - 'A' + 10;
	return 0xff;
}
 
/********************************************************************************
input:
	p: 两个文本字符
output:
	转化为1个字节
********************************************************************************/
static uint8_t Hex2Bin(const char *p)
{
	uint8_t tmp = 0;
	tmp = HexCharToBinBinChar(p[0]);
	tmp <<= 4;
	tmp |= HexCharToBinBinChar(p[1]);
	return tmp;
}
 
/********************************************************************************
input:
	src: hex单行字符串
	p->type: 如果函数返回结果正确，这里就存着转化后的类型
	p->len: 如果函数运行正确，这里就存着转化后的bin数据长度
	p->data: 如果函数运行正确，长度并且不为0，该指针就只想转化后的数据
	p->addr[0]: 如果函数返回结果正确，这里就存着地址的低字节
	p->addr[1]: 如果函数返回结果正确，这里就存着地址的低字节
output:
	返回hex格式流分析的结果
********************************************************************************/
static RESULT_STATUS HexFormatUncode(const char *src, BinFarmat *p)
{
	uint8_t check = 0, tmp[4], binLen;
	uint16_t hexLen = strlen(src);
	uint16_t num = 0, offset = 0;
	if (hexLen > HEX_MAX_LENGTH)		//数据内容过长
		return RES_DATA_TOO_LONG;
	if (hexLen < HEX_MIN_LEN)
		return RES_DATA_TOO_SHORT;	//数据内容过短
	if (src[0] != ':')
		return RES_NO_COLON;		//没有冒号
	if ((hexLen - 1) % 2 != 0)
		return RES_LENGTH_ERROR;	//hexLen的长度应该为奇数
	binLen = (hexLen - 1) / 2;		//bin总数据的长度，包括长度，地址，类型校验等内容
	while (num < 4)
	{
		offset = (num << 1) + 1;
		tmp[num] = Hex2Bin(src + offset);
		check += tmp[num];
		num++;
	}
	p->len = tmp[0];			//把解析的这些数据保存到结构体中
	p->addr = tmp[1];
	p->addr <<= 8;
	p->addr += tmp[2];
	p->type = tmp[3];
	while (num < binLen)
	{
		offset = (num << 1) + 1;        //保存真正的bin格式数据流
		p->data[num - 4] = Hex2Bin(src + offset);
		check += p->data[num - 4];		
		num++;
	}
	if (p->len != binLen - 5)		//检查hex格式流的长度和数据的长度是否一致
		return RES_LENGTH_ERROR;
	if (check != 0)				//检查校验是否通过
		return RES_CHECK_ERROR;
	return RES_OK;
}
 
RESULT_STATUS HexFile2BinFile(char *src, char *dest)  
{  
    FILE *src_file, *dest_file; 
	uint16_t addr_low = 0;
	u32_t addr_hign = 0;
	char buffer_hex[600];
	uint8_t buffer_bin[255];
	BinFarmat gBinFor;
	RESULT_STATUS res;
	gBinFor.data = buffer_bin;
	src_file = fopen(src, "r");		//以文本的形式打开一个hex文件
	if (!src_file)
		return RES_HEX_FILE_NOT_EXIST;
	dest_file = fopen(dest, "wb");		//以二进制写的方式打开文件，文件不存在也没影响
	if (!dest_file)
		return RES_BIN_FILE_PATH_ERROR;
	fseek(src_file, 0, SEEK_SET);           //定位到开头，准备开始读取数据  
	while (!feof(src_file))
	{
		fscanf(src_file, "%s\r\n", buffer_hex);
		res = HexFormatUncode(buffer_hex, &gBinFor);
		if (res != RES_OK)
		{
			fclose(src_file);
			fclose(dest_file);
			return res;
		}
		switch (gBinFor.type)
		{
			case 0:			//数据记录
				addr_low = gBinFor.addr;
                                //数据指针偏移
				fseek(dest_file, addr_low + addr_hign, SEEK_SET);	
				if (fwrite((const uint8_t*)gBinFor.data, gBinFor.len, 1, dest_file) != 1)
				{
					fclose(src_file);
					fclose(dest_file);
					return RES_WRITE_ERROR;
				}
				break;
			case 1:		        //数据结束
				fclose(src_file);
				fclose(dest_file);
				return RES_OK;
			case 2:
				addr_hign = ((u32_t)gBinFor.addr) << 2;
				break;
			case 4:			//线性段地址
				addr_hign = ((u32_t)gBinFor.addr) << 16;
				break;
			default:
				fclose(src_file);
				fclose(dest_file);
				return RES_TYPE_ERROR;
		}
	}
	fclose(src_file);
	fclose(dest_file);
	return RES_HEX_FILE_NO_END;
}