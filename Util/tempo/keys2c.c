/* keys2fort.c -- redwards 1/12/97
 *
 * program to parse a keyword definition file and produce Fortran
 * arrays containing the information, for use by ephio.f. Produces
 * file keyinfo.com, based on file keys.dat. 
 */

#include <stdio.h>
#include <string.h>

/* fatal errors ... */
#define FATAL_1(s1) {fprintf(stderr, "Fatal: "); fprintf(stderr, s1); exit(1);}
#define FATAL_2(s1,s2) {fprintf(stderr, "Fatal: "); fprintf(stderr, s1,s2); exit(1);}
#define FATAL_3(s1,s2,s3) {fprintf(stderr, "Fatal: "); fprintf(stderr, s1,s2,s3); exit(1);}

int
main()
{
	FILE *fin, *fout;
	char key[1024][64];
	char line[256];
	int type[1024], err[1024];
	int i, nelem, len, maxlen = 0;

	fin = fopen("keys.dat", "r");
	if (!fin)
		FATAL_1("Couldn't open keys.dat");
	fout = fopen("ephio.h", "w");
	if (!fout)
		FATAL_1("Couldn't open ephio.h");

	i = 0;
	while (fgets(line,256,fin)!=NULL)
	{
		if ((sscanf(line,"%s", key[i])==1)	/* non empty line */
		   && (key[i][0]!='#'))	/* ignore comments */
		{
			sscanf(line,"%s %d %d", key[i], type+i, err+i);
			i++;
		}
	}
	nelem = i;

	fprintf(fout, 
		"/* This file was generated by keys2c. Do not modify directly */\n\n");
	fprintf(fout, "#ifndef EPHIO_H\n#define EPHIO_H\n\n");
	fprintf(fout, "#ifdef __cplusplus\nextern \"C\" {\n#endif\n");
	fprintf(fout, "#define EPH_NUM_KEYS %d\n", nelem);
	fprintf(fout, "#define EPH_STR_LEN 32\n");

/*   fprintf(fout, "extern {\n");*/
	fprintf(fout, "extern int rd_eph(char *, int *, char [EPH_NUM_KEYS][EPH_STR_LEN], double *,\n");
   fprintf(fout, "                    int *, double *);\n");
	fprintf(fout, "extern int rd_eph_lun(int, int *, char [EPH_NUM_KEYS][EPH_STR_LEN], double *,\n");
   fprintf(fout, "                    int *, double *);\n");
	fprintf(fout, "extern int wr_eph(char *, int *, char [EPH_NUM_KEYS][EPH_STR_LEN], double *,\n");
   fprintf(fout, "                    int *, double *);\n");
	fprintf(fout, "extern int wr_eph_lun(int, int *, char [EPH_NUM_KEYS][EPH_STR_LEN], double *,\n");
   fprintf(fout, "                    int *, double *);\n");
/*   fprintf(fout, "    }\n");*/

	for (i=0; i < nelem; i++)
	{
		fprintf(fout, "#define EPH_%s %d\n", key[i], i);
	}

	fprintf(fout, "\n\nstatic char parmNames[EPH_NUM_KEYS][16] = {\n");
	for (i=0; i < nelem-1; i++)
	{
		len = strlen(key[i]);
		if (len > maxlen)
			maxlen = len;
		fprintf(fout, "                      \"%s\",\n", key[i]);
	}
   fprintf(fout, "                      \"%s\"};\n", key[i]);
	fprintf(fout, "\n");

	fprintf(fout, "\n\nstatic int parmTypes[EPH_NUM_KEYS] = {\n");
	for (i=0; i < nelem-1; i++)
		fprintf(fout, "                      %d,\n", type[i]);
   fprintf(fout, "                      %d};\n", type[i]);
	fprintf(fout, "\n");
	fprintf(fout, "\n\nstatic int parmError[EPH_NUM_KEYS] = {\n");
	for (i=0; i < nelem-1; i++)
		fprintf(fout, "                      %d,\n", err[i]);
   fprintf(fout, "                      %d};\n", err[i]);
	fprintf(fout, "\n");

   fprintf(fout, "\n\n#define EPH_MAX_KEY_LEN %d\n", maxlen);
	fprintf(fout, "#ifdef __cplusplus\n}\n#endif\n");
	fprintf(fout, "\n\n#endif\n");

	fclose(fout);
	fclose(fin);

	return 0;
}

