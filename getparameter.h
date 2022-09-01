/*
 * @Author: Yimning 1148967988@qq.com
 * @Date: 2022-08-11 00:13:43
 */


#ifndef GETPARAMETER_H
#define GETPARAMETER_H

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "cJSON.h"
#include "stdbool.h"

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long u32_t;
 
#define HEX_MAX_LENGTH		521
#define HEX_MIN_LEN		11
#define PARAMETER_LEN		20

#define CONTENT_MAX_LEN 1024*1024
typedef char (*arrT)[PARAMETER_LEN]; 

arrT* getOneChar(char *query_string);    
//arrT* 数组指针 同 char (*p)[]  等价于 char (*func(char i))[10] 

char* getparameter(char *query_string, int index);
 //返回二维数组的地址，必须制定它的一维的长度
int sln_string_unescape(char **attr, const char *src, int len);
int sln_cgi_content_parse(char *input_content);

char* cjson_cgi_content_parse(char *query_string);

char* cjson_cgi_getStrValue(const char *const key);

int cjson_cgi_getIntValue(const char *const key);

double cjson_cgi_getDoubleValue(const char *const key);

bool cjson_cgi_getBoolValue(const char *const key);

unsigned char CharToHex(unsigned char bHex);
unsigned char HexToChar(unsigned char bChar);

static uint8_t HexCharToBinBinChar(char c);
static uint8_t Hex2Bin(const char *p);

typedef enum {
	RES_OK = 0,		//正确
	RES_DATA_TOO_LONG,	//数据太长
	RES_DATA_TOO_SHORT,	//数据太短
	RES_NO_COLON,		//无标号
	RES_TYPE_ERROR,		//类型出错，或许不存在
	RES_LENGTH_ERROR,	//数据长度字段和内容的总长度不对应
	RES_CHECK_ERROR,	//校验错误
	RES_HEX_FILE_NOT_EXIST,	//HEX文件不存在
	RES_BIN_FILE_PATH_ERROR,//BIN文件路径可能不正确
	RES_WRITE_ERROR,	//写数据出错
	RES_HEX_FILE_NO_END	//hex文件没有结束符
} RESULT_STATUS;
 
typedef struct {
	uint8_t len;
	uint8_t	type;
	uint16_t addr;
	uint8_t *data;
} BinFarmat;
 
RESULT_STATUS HexFile2BinFile(char *src, char *dest);

#endif