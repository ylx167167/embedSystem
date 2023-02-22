
#include "cJsonFile.h"
#include "global.h"
// 读取文件
int ReadJsonFile(char* filename, unsigned char *result, int maxSize)
{
	FILE *fp = NULL;
	fp = fopen(filename, "r");
	char buf[1024] = {0};
	int bufLen = 1024;
	int resultLen = 0;
	while(fgets(buf, 1024, fp) != NULL)
	{
		if(resultLen + strlen(buf) >= maxSize)
		{
			printf("缓冲区满，终止读取\n");
			break;
		}
		char *hasComment = strchr((const char*)buf, '#'); // 是否有注释存在
		if(!hasComment)
		{
			memcpy(&result[resultLen], buf, strlen(buf));
			resultLen += strlen(buf);
			memset(buf, 0, bufLen);
		}
		else 
		{
			memcpy(&result[resultLen], buf, hasComment - buf);
			resultLen += (hasComment - buf);
			memset(buf, 0, bufLen);
		}
	}
	fclose(fp);
	while(result[resultLen-1] == '\n' || result[resultLen-1] == '\r')
	{
		result[resultLen-1] = '\0';
		resultLen--;
	}
	return resultLen;
}

// 解析json配置文件
cJSON* ANAJsonFile(char* filename, cJSON*root)
{
	unsigned char jsonBuf[1024] = {0};
	int ret = ReadJsonFile(filename, jsonBuf,MAX_JSON);
	// printf("result[%d]:\n%s\n", ret, jsonBuf);
	root = cJSON_Parse((const char*)jsonBuf);
	if(root)
	{
		printf("读取的配置文件:\n %s\n", cJSON_Print(root));
	}
	else
	{
		printf("读取的配置文件出错: %s\n", cJSON_GetErrorPtr());
		exit(1);
	}
	return root;
}

// 根据key修改value
void ChangeIntValuesByKey(cJSON* root, void *key, int values)
{
	cJSON_ReplaceItemInObject(root, key, cJSON_CreateNumber(values));
}

void ChangeDoubleValueByKey(cJSON *root, void *key, double values )
{
	cJSON_ReplaceItemInObject(root, key, cJSON_CreateNumber(values));
}

void ChangeStringValueByKey(cJSON* root, const char *key, char* values)
{
	cJSON_ReplaceItemInObject(root, key, cJSON_CreateString(values));
}

cJSON * GetObjectItimeByString(cJSON *root,const char *key_object){
	return cJSON_GetObjectItem(root,key_object);
}
 char * GetCharBufFromJsonFIle(char *filename){
	cJSON *root;
	root=ANAJsonFile(filename,root);
	return cJSON_PrintUnformatted(root);
 }



// 将json写入文件
int writeJson2File(cJSON *root, char *filename)
{
	FILE *fp = NULL;
	fp = fopen(filename, "w");
	fwrite(cJSON_Print(root), 1, strlen(cJSON_Print(root)), fp);
	fflush(fp);
	fclose(fp);
	return 0;
}

