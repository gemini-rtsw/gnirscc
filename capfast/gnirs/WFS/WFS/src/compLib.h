/* $Id: compLib.h,v 1.2 2009/05/27 19:35:02 fkraemer Exp $ */

#if !defined COMP_LIB_H
#define COMP_LIB_H

typedef struct {
	long ignore;
	long datumed;
	struct {
		long cyclic;
		long min;
		long max;
	} mech[4];
	double inp[8]; /* m-t */
} CompParms;

struct cadRecord;
typedef long (*CompCadFunction)(struct cadRecord *, const CompParms *);
#define COMP_CAD_NULL ((CompCadFunction)(0))

long compCad(struct cadRecord *, CompCadFunction);
long compLong(const char *, long *);
long compDouble(const char *, double *);
double compTime();

#endif /* COMP_LIB_H */
