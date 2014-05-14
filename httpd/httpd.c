#include <stdio.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include "../inter-def.h"

void cat(char *file)
{
	size_t len = 0;
	char *line = NULL;
	FILE *f = fopen(file, "r");
	if (f == NULL)
		return;

	while (getline(&line, &len, f) != -1)
		printf("%s", line);

	free(line);
	fclose(f);
}

void replace_plus(char *str)
{
	while (str[0] != '\0') {
		if (str[0] == '+') 
			str[0] = ' ';
		str ++;
	}
}

int main()
{

	CURL *curl;
	char *unescape;
	char *get_str;
	char arg[2048];
	char cmd[2048];
	char *line;
	char last_line[4096];
	size_t len = 0;
	int toggle = 0;
	int start;
	FILE *f_rank = NULL;
	printf("Content-type: text/html\n\n");

	scanf("%s", arg);
	get_str = getenv("QUERY_STRING");

	if (get_str == NULL) {
		printf("no GET arg.\n");
		return 0;
	}

	sscanf(get_str, "s=%d", &start);

	if (start == 0) {
		replace_plus(arg);
		curl = curl_easy_init();
		unescape = curl_easy_unescape(curl, arg, 0, NULL);

		sprintf(arg, "%s", unescape);
		unescape[0] = ' ';
		unescape[1] = ' ';

		sprintf(cmd, "./ma-se '%s'", unescape);
		system(cmd);

		/* printf("<h3>results of query %s (%d to %d):</h3>", 
				unescape, start, start + 10); */
		strcpy(arg, unescape);
		free(unescape);
	} else {
		/* printf("<h3>results (%d to %d):</h3>", 
				start, start + 10); */
	}

	cat("head.cat");
	printf("[imath] \\quad %s [/imath] : </p><hr class=\"rgsep\">"
			"<ul style=\"list-style-type: none;\">", arg);

	f_rank = fopen("rank", "r");
	if (f_rank == NULL) {
		printf("no thing we can find :( <br/>");
		return 0;
	}

	while (getline(&line, &len, f_rank) != -1) {
		if (toggle == 0) {
			strcpy(last_line, line);
		} else {
			printf("<li><table border=\"0\"><tr><td><a href=%s"
					"><h3>%s</h3></a><span style=\"color:green;\">"
					"%s</span></td><td>[dmath]%s"
					"[/dmath]</td></tr></table></li>",
					line, last_line, line, last_line);
		}

		toggle = (toggle + 1) % 2;
	}
	free(line);
	
	fclose(f_rank);
	cat("tail.cat");

	return 0;
}