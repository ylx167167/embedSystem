#ifndef _CJSONFILE_H_
#define _CJSONFILE_H_

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "cJSON.h"

// 读取文件
int ReadJsonFile(char* filename, unsigned char *result, int maxSize);

// 解析json配置文件
cJSON* ANAJsonFile(char* filename, cJSON*root);

// 根据key修改value
void ChangeIntValuesByKey(cJSON* root, void *key, int values);
void ChangeDoubleValueByKey(cJSON *root, void *key, double values);
void ChangeStringValueByKey(cJSON* root,const char *key, char* values);

//获取object 
cJSON * GetObjectItimeByString(cJSON *root,const char *key_object);


//返回一个以没有格式的形式转换成普通的字符串
char * GetCharBufFromJsonFIle(char *filename);


// 将json写入文件
int writeJson2File(cJSON *root, char *filename);


#endif
