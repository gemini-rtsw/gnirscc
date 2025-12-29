#include <genSubRecord.h>
#include <sirRecord.h>
#include <compLib.h>

int
wfsFolTestInSub(struct genSubRecord *pGenSub)
{
	((double *)pGenSub->valj)[0] = *(double *)pGenSub->a;
	((double *)pGenSub->valj)[1] = *(double *)pGenSub->j; /* Triggers proc */
	((double *)pGenSub->valj)[2] = *(double *)pGenSub->b;
	((double *)pGenSub->valj)[3] = *(double *)pGenSub->c;
	((double *)pGenSub->valj)[4] = *(double *)pGenSub->d;
	((double *)pGenSub->valj)[5] = *(double *)pGenSub->e;
	return 0;
}

int
wfsFolInSub(struct genSubRecord *pGenSub)
{
	const double xScale = *(double *)pGenSub->b;
	const double xOffset = *(double *)pGenSub->d;
	const double yScale = *(double *)pGenSub->f;
	const double yOffset = *(double *)pGenSub->h;
	const double zScale = *(double *)pGenSub->l;
	const double zOffset = *(double *)pGenSub->n;
	const double tSent = ((double *)pGenSub->j)[0];
	const double tAppl = ((double *)pGenSub->j)[1];
	const double trackId = ((double *)pGenSub->j)[2];
	const double x = ((double *)pGenSub->j)[3];
	const double y = ((double *)pGenSub->j)[4];
	const double z = ((double *)pGenSub->j)[5];

	*(double *)pGenSub->valb = tAppl;
	*(double *)pGenSub->vald = x * xScale + xOffset;
	*(double *)pGenSub->valf = tAppl;

	*(double *)pGenSub->valh = tAppl;
	*(double *)pGenSub->valj = y * yScale + yOffset;
	*(double *)pGenSub->vall = tAppl;

	*(double *)pGenSub->valn = tAppl;
	*(double *)pGenSub->valp = z * zScale + zOffset;
	*(double *)pGenSub->valr = tAppl;

	*(double *)pGenSub->vals = tSent;
	*(double *)pGenSub->valu = trackId;

	return 0;
}

int
wfsFolTestOutSub(struct genSubRecord *pGenSub)
{
	*(double *)pGenSub->vala = ((double *)pGenSub->j)[0];
	*(double *)pGenSub->valb = ((double *)pGenSub->j)[1];
	*(double *)pGenSub->valc = ((double *)pGenSub->j)[2];
	*(double *)pGenSub->vald = ((double *)pGenSub->j)[3];
	*(double *)pGenSub->vale = ((double *)pGenSub->j)[4];
	*(double *)pGenSub->valf = ((double *)pGenSub->j)[5];
	*(double *)pGenSub->valg = ((double *)pGenSub->j)[6];
	*(double *)pGenSub->valh = ((double *)pGenSub->j)[7];
	*(double *)pGenSub->vali = ((double *)pGenSub->j)[8];

	return 0;
}

int
wfsFolOutSub(struct genSubRecord *pGenSub)
{
	const double x = *(double *)pGenSub->a;
	const double y = *(double *)pGenSub->b;
	const double z = *(double *)pGenSub->c;
	const double xErr = *(double *)pGenSub->g;
	const double yErr = *(double *)pGenSub->h;
	const double zErr = *(double *)pGenSub->i;
	const double timestamp = *(double *)pGenSub->d;
	const double trackId = *(double *)pGenSub->e;
	const double angle = *(double *)pGenSub->f;
	const double xScale = *(double *)pGenSub->l;
	const double xOffset = *(double *)pGenSub->m;
	const double yScale = *(double *)pGenSub->n;
	const double yOffset = *(double *)pGenSub->o;
	const double zScale = *(double *)pGenSub->p;
	const double zOffset = *(double *)pGenSub->q;

	((double *)pGenSub->valj)[0] = timestamp;
	((double *)pGenSub->valj)[1] = trackId;
	((double *)pGenSub->valj)[2] = angle;
	((double *)pGenSub->valj)[3] = (x - xOffset) / xScale;
	((double *)pGenSub->valj)[4] = (xErr - xOffset) / xScale;
	((double *)pGenSub->valj)[5] = (y - yOffset) / yScale;
	((double *)pGenSub->valj)[6] = (yErr - yOffset) / yScale;
	((double *)pGenSub->valj)[7] = (z - zOffset) / zScale;
	((double *)pGenSub->valj)[8] = (zErr - zOffset) / zScale;

	return 0;
}

int
SIRwfsFolPresent(struct sirRecord *pSir)
{
	*(double *)pSir->val = compTime();
	return 0;
}
