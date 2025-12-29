/* drvOms.h */
/* valid command types for the driver, the order is importance, 0 is of
   lowest importance.  Device support will choose the greatest one to
   use as the driver transaction type. */

#define HOMS_UNDEFINED (unsigned char)0 /* garbage type */
#define HOMS_IMMEDIATE (unsigned char)1 /* 'i' an execute immediate, no reply */
#define HOMS_MOVE_TERM (unsigned char)2 /* 't' terminate a previous active motion */
#define HOMS_MOTION    (unsigned char)3 /* 'm' will produce motion updates */
#define HOMS_VELOCITY  (unsigned char)4 /* 'v' make motion updates till MOVE_TERM */
#define HOMS_INFO      (unsigned char)5 /* 'f' get curr motor/encoder pos and stat */
#define HOMS_QUERY     (unsigned char)6 /* 'q' specialty type, not needed */

/*
 * VME8/4E default profile 
 */

#define HOMS_NUM_CARDS           8
#define HOMS_NUM_CHANNELS        8
#define HOMS_INTERRUPT_TYPE      intVME
#define HOMS_ADDRS_TYPE          atVMEA16
#define HOMS_NUM_ADDRS           0xFC00
#define HOMS_INT_VECTOR          180    /* default interrupt vector (64-255) */
#define HOMS_INT_LEVEL           5      /* default interrupt level (1-6) */
#define HOMS_BRD_SIZE            0x10   /* card address boundary */
#define HOMS_RESP_Q_SZ           0x100  /* maximum oms response message size */


struct homs_support {
	int	(*send)();
	int	(*free)();
	int	(*get_card_info)();
	int	(*get_axis_info)();
	};

/* message queue management - device and driver support only */
struct homs_mess_node {
	CALLBACK	callback;
	int	signal;			
	int	card;
	unsigned char	type;
	char	message[HMOTOR_MESS_SIZE];
	long	position;
	long	encoder_position;
        long    velocity;
	unsigned long status;
	struct	dbCommon *precord;
	struct	homs_mess_node *next;
	};
typedef struct homs_mess_node HOMS_MOTOR_CALL;
typedef struct homs_mess_node HOMS_MOTOR_RETURN;

/* initial position query to driver - device and driver support only */
typedef struct homs_mess_card_query {
	char	*card_name;
	char	*axis_names;
	int	total_axis;
	} HOMS_MOTOR_CARD_QUERY;

typedef struct homs_mess_axis_query {
	long	position;
	long	encoder_position;
	unsigned long status;
	} HOMS_MOTOR_AXIS_QUERY;

