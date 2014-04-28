#include <stdlib.h>
#include <stdbool.h>
#include <hiredis/hiredis.h>
#include "function.h"
#include "protocol.h"
#include "config.h"

char *explode(char *s, char c)
{
	char t=(char)(*s);
	char *p=NULL;
	do
	{
		if(t==c)
		{
			*s=0;	
			p=s+1;
			break;
		}
	t=*(++s);
	}while(t!=0);
	return p;
}


bool subscribe(char *channel, CONN* c, redisContext *redis)
{
	bool ret=true;
	char cmdbuffer[30];
	int err=sprintf(cmdbuffer,"sadd %s %x",channel,c);
	redisReply *reply=redisCommand(redis, cmdbuffer);
	if(reply->type==REDIS_REPLY_ERROR)
	{
		printf("error%s\n",reply->str);
		ret=false;
		
	}
	err=sprintf(cmdbuffer,"sadd %x %s",c,channel);
	reply=redisCommand(redis, cmdbuffer);
	if(reply->type==REDIS_REPLY_ERROR)
	{
		printf("error%s\n",reply->str);
		ret=false;
		
	}

	freeReplyObject(reply);
	int er=bufferevent_write(c->bufev,"success\r\n",strlen("success\r\n"));
	return ret;
}
void publish(char * channel,char *data, redisContext *redis)
{
	char cmdbuffer[30];
	int err=sprintf(cmdbuffer,"smembers %s",channel);
	redisReply *reply=redisCommand(redis, cmdbuffer);
	if(reply->type==REDIS_REPLY_ERROR)
	{
		printf("error%s\n",reply->str);
	}
	if(reply->type==REDIS_REPLY_ARRAY)
	{
		int num=reply->elements;
		int i=0;
		for(;i<num;i++)
		{
			char  *errend;
			char *conn=reply->element[i]->str;
			CONN *c=(CONN *)strtoul(conn,&errend,16);
			bufferevent_write(c->bufev,data,strlen(data));
			bufferevent_write(c->bufev,"\r\n",2);
		}
	}
	freeReplyObject(reply);
	
}

sint32 mysub(CONN* c) {
	HEAD *hreq = (HEAD*) c->in_buf;
	LOGIN_REQ *login=(LOGIN_REQ *)malloc(sizeof(LOGIN_REQ));
	login->channel=	(char *)(c->in_buf+sizeof(HEAD));
	char *channel=login->channel;
	free(login);
	redisContext *redis=redisConnect("127.0.0.1",6379);
	if(redis->err)
	{
		printf("connect redis err:%s",redis->errstr);
		redisFree(redis);
		return;
	}
	subscribe(channel,c,redis);
	redisFree(redis);
	return 0;
}


sint32 mypub(CONN* c) {
	HEAD *hreq = (HEAD*) c->in_buf;
	char *cmd=c->in_buf+sizeof(HEAD);
	char *data=explode(cmd,'|');
	redisContext *redis=redisConnect("127.0.0.1",6379);
	if(redis->err)
	{
		printf("connect redis err:%s",redis->errstr);
		redisFree(redis);
		return;
	}
	publish(cmd, data, redis);
	redisFree(redis);
	return 0;
}

sint32 logout(CONN* c) {
	redisContext *redis=redisConnect("127.0.0.1",6379);
	if(redis->err)
	{
		printf("connect redis err:%s",redis->errstr);
		redisFree(redis);
		return;
	}
	char cmdbuffer[30];
	int err=sprintf(cmdbuffer,"smembers %x",c);
	redisReply *reply=redisCommand(redis, cmdbuffer);
	if(reply->type==REDIS_REPLY_ERROR)
	{
		printf("error%s\n",reply->str);
	}
	if(reply->type==REDIS_REPLY_ARRAY)
	{
		int num=reply->elements;
		int i=0;
		for(;i<num;i++)
		{
			char  *errend;
			char *channel=reply->element[i]->str;
			err=sprintf(cmdbuffer,"srem %s %x",channel,c);
			redisReply *reply=redisCommand(redis, cmdbuffer);
			if(reply->type==REDIS_REPLY_ERROR)
			{
				printf("error%s\n",reply->str);
			}
		}
	}
	freeReplyObject(reply);

}

