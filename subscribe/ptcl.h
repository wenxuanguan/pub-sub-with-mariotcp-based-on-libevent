#ifndef STATS_PTCL_H_
#define STATS_PTCL_H_

#include "_type.h"
typedef enum _CMD {
	CMD_FUNCTION_BASE = 0x6100,
	CMD_FUNCTION_LOGIN = 0x6101,
	CMD_FUNCTION_SUBSCRIBE = 0x6102,
	CMD_FUNCTION_PUBLISH=0x6103
} CMD;

#define MY_STX	(0x8eb69af7)
#define SIZE_OF_HEAD	((uint16_t) 8)


typedef struct _LOGIN {
	char *channel;
} LOGIN;

typedef struct _PUB
{
	char *cmd;
} PUBLISH;

typedef struct _HEAD {
	uint32_t stx;
	uint16_t pkglen;
	uint16_t cmd;
} HEAD;




#endif /* STATS_PTCL_H_ */
