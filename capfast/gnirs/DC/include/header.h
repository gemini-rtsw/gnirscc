/* assumptions: char's are unsigned by default.  char's are coalesced
	into an int in msb to lsb order.  */


typedef struct header {
    char node;
    char length;
    char from;
    char message;
} header;

typedef struct var {
    short arr_index;
    short varnum;
} var;

typedef struct message {
    header head;
    union buffer {
	int buf[head.length];
	struct image_done {
	    /* nada */
	};
	struct begin_xmit {
	    /* nothing */
	};
	struct set_var {
	    var v;
	    int val[head.length - 1];
	};
	struct read_var {
	    var v;
	    int reply_to;
	    union length {
		int length;	/* 0 == use strlen, otherwise # of ints */
		void nada;
	    };
	};
	struct var_read {
	    var v;
	    int from;
	    int val[head.length - 2];
	}
	struct debug_msg {
	    int msg[head.length];	/* this is filled with chars */
	}
    };
} message;
