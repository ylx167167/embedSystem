#include "cJsonFile.h"
#include "bs.h"
#include "cJSON.h"
void Bs_GetbufFromJson(char *filename ,bsstruct * buf){
    cJSON *root;
    cJSON *item;
    root=ANAJsonFile(filename,root);
    item=GetObjectItimeByString(root,"machineid");
    buf->machineid=atoi(item->valuestring);
    printf("machineid:%d\r\n",buf->machineid);
    item=GetObjectItimeByString(root,"txtime");
    if(strcmp(buf->txtime,item->valuestring)!=0)
    {
    strcpy(buf->txtime,item->valuestring);
    printf("txtime   :%s\r\n",buf->txtime);
    }
    
    root=GetObjectItimeByString(root,"payload");
    item=GetObjectItimeByString(root,"sensorid");
    buf->sensorid=atoi(item->valuestring);
    printf("sensorid :%d\r\n",buf->sensorid);
    item=GetObjectItimeByString(root,"workid");
    buf->workid=atoi(item->valuestring);
    printf("workid   :%d\r\n",buf->workid);

    item=GetObjectItimeByString(root,"owner");
    if(strcmp(buf->owner,item->valuestring)!=0)
    {
    strcpy(buf->owner,item->valuestring);
    printf("owner    :%s\r\n",buf->owner);
    }
    item=GetObjectItimeByString(root,"macid");
    if(strcmp(buf->macid,item->valuestring)!=0)
    {
    strcpy(buf->macid,item->valuestring);
    printf("macid    :%s\r\n",buf->macid);
    }
    item=GetObjectItimeByString(root,"action");
    if(strcmp(buf->action,item->valuestring)!=0)
    {
    strcpy(buf->action,item->valuestring);
     printf("macid    :%s\r\n",buf->action);
    }
}

char *Bs_GetCharBufFromJsonFIle(char *filename){
    return GetCharBufFromJsonFIle(filename);
}


