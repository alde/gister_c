#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VERSION "0.0.0"

int show_help();
int beginswith(char const *string, char c);
int parse_args(int argc, char **argv);

char *gist_desc = "";
char **files = NULL;

enum {
	OPT_A = 0x01,
	OPT_V = 0x02,
	OPT_N = 0x04,
	OPT_P = 0x08,
	OPT_H = 0x10,
};

int main(int argc, char **argv)
{
	int opts = 0;
	opts = parse_args(argc, argv);

	if (opts & OPT_H || argc <= 1)
		return show_help();

	if (strcmp(gist_desc, "") != 0)
		fprintf(stderr, "Description: %s\n", gist_desc);

	if (opts & OPT_A)
		fputs("Anonymous!\n", stderr);

	if (opts & OPT_V)
		fputs("Verbose!\n", stderr);

	free(files);
	free(gist_desc);
}

int parse_args(int argc, char **argv) {
	size_t i;
	size_t file_count = 0;
	int opts = 0;

	for (i = 1; i < argc; i++) {
		char const *option = argv[i];
		if (beginswith(option, '-')) {
			if (match_option(option, "-a", "--anonymous"))
				opts |= OPT_A;

			if (match_option(option, "-v", "--verbose"))
				opts |= OPT_V;

			if (match_option(option, "-n", "--no-op"))
				opts |= OPT_N;

			if (match_option(option, "-p", "--public"))
				opts |= OPT_P;

			if (match_option(option, "-h", "--help"))
				opts |= OPT_H;

			if (match_option(option, "-d", "--description")) {
				gist_desc = malloc(sizeof(argv[i + 1]) * sizeof(char*));
				strcpy(gist_desc, argv[i + 1]);
				i++;
			}
		} else {
			files = realloc(files, sizeof(char *) * (file_count + 1));
			files[file_count] = malloc(sizeof(argv[i]) * sizeof(char*));
			strcpy(files[file_count], argv[i]);
			file_count++;
		}
	}
	size_t n;
	for (n = 0; n < file_count; n++)
		printf("file %d: %s\n", n + 1, files[n]);

	return opts;
}

int match_option(char const *option, char const *short_name, char const *long_name) {
	return (strcmp(option, short_name) == 0 || strcmp(option, long_name) == 0);
}


int beginswith(char const *string, char c)
{
	return (string[0] == c) ? 1 : 0;
}

int show_help()
{
	printf("Gister v%s\n", VERSION);
	printf("\nCreate a gist from given files.");
	printf("\nSee http://gist.github.com/ for more info.\n");
	printf("\nUsage:\ngister [options] file1 [file2 file3 ...]\n");
	printf("\nOptions:\n");
	printf("\t-h --help\t\t This message\n");
	printf("\t-v --verbose\t\t Output full response\n");
	printf("\t-d --description DESC\t Add a description for the gist\n");
	printf("\t-p --public\t\t Make the gist public\n");
	printf("\t-a --anonymous\t\t Make an anonymous gist\n");
	printf("\t-n --no-op\t\t Make no actual call to gist\n\n");
	printf("(c) 2014 Rickard Dybeck\n\n");

	return 0;
}
