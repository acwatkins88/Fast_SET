#include <stdlib.h>
#include <stdio.h>

#define MAX_LINE 256

int main(int argc, char **argv)
{
	int flag = 0;
	FILE *resfile, *outfile;
	char line[MAX_LINE];

	resfile = fopen(argv[1], "r");
	outfile = fopen(argv[2], "w+");
	
	while(fgets(line, MAX_LINE, resfile) != NULL)
	{
		if(line[0] == 'x')
		{
			flag = 1;
		}
		else if(line[0] == 'y')
		{
			flag = 0;
		}
		else if(flag == 1)
		{
			fprintf(outfile, "%s", line);
		}
	}
	fclose(resfile);
	fclose(outfile);
	return 0;
}
