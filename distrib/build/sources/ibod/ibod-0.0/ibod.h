/* $Id: ibod.h 65 2005-08-11 10:24:07Z lawrence $
 * $Symbol$
 */

#define DEVICE			"ippp0"
#define ENABLE			1
#define INTERVAL		500
#define FILTER			5
#define LIMIT			7000
#define STAYUP			0
#define STAYUP_TIME		30

#define IBOD_DEFAULT_DIR	"/etc"
#define MAX_STR_LEN		512
#define ISDN_INFO_DEV		"/dev/isdninfo"
#define ISDN_CTLR_DEV		"/dev/isdnctrl"
#define IBOD_PORT		6050

/* Protocol specification */
#define MAX_MSG_LEN		1024
#define POBJ_SIZE		4
#define LINK_STATUS_POS		0
#define INBOUND_RATE_POS	4
#define OUTBOUND_RATE_POS	8
#define STATUS_FRAME_SIZE	12

#define CMD_OPEN		0
#define CMD_CLOSE		1
#define CMD_ENABLE		2
#define CMD_DISABLE		3
#define CMD_UP2			4
#define CMD_DOWN2		5

typedef struct {
	char	dev[32];
	int	enable;
	int	interval;
	int	limit;
	int	filter;
	int	stayup;
	int	stayup_time;
} Conf;

typedef struct {
	unsigned long ibytes;
	unsigned long obytes;
} Siobytes;

