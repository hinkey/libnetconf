#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "commands.h"
#include "mreadline.h"
#include "configuration.h"
#include "../../src/libnetconf.h"

#define VERSION "0.1"
#define PROMPT "netconf> "

volatile int done = 0;
extern COMMAND commands[];
struct nc_cpblts * client_supported_cpblts;

void clb_print(NC_VERB_LEVEL level, const char* msg)
{
	switch (level) {
	case NC_VERB_ERROR:
		fprintf(stderr, "libnetconf ERROR: %s\n", msg);
		break;
	case NC_VERB_WARNING:
		fprintf(stderr, "libnetconf WARNING: %s\n", msg);
		break;
	case NC_VERB_VERBOSE:
		fprintf(stderr, "libnetconf VERBOSE: %s\n", msg);
		break;
	case NC_VERB_DEBUG:
		fprintf(stderr, "libnetconf DEBUG: %s\n", msg);
		break;
	}
}

void clb_error_print(const char* tag,
		const char* type,
		const char* severity,
		const char* UNUSED(apptag),
		const char* UNUSED(path),
		const char* message,
		const char* UNUSED(attribute),
		const char* UNUSED(element),
		const char* UNUSED(ns),
		const char* UNUSED(sid))
{
	fprintf(stderr, "NETCONF %s: %s (%s) - %s\n", severity, tag, type, message);
}

void print_version()
{
	fprintf(stdout, "libnetconf client version: %s\n", VERSION);
	fprintf(stdout, "compile time: %s, %s\n", __DATE__, __TIME__);
}

int main(int UNUSED(argc), char** UNUSED(argv))
{
	char *cmdline, *cmdstart;
	int i, j;
	char *cmd;

	initialize_readline();

	/* set verbosity and function to print libnetconf's messages */
	nc_verbosity(NC_VERB_WARNING);
	nc_callback_print(clb_print);
	nc_callback_error_reply(clb_error_print);

	/* disable publickey authentication */
	nc_ssh_pref(NC_SSH_AUTH_PUBLIC_KEYS, -1);

	load_config (&client_supported_cpblts);

	while (!done) {
		/* get the command from user */
		cmdline = readline(PROMPT);

		/* EOF -> exit */
		if (cmdline == NULL) {
			done = 1;
			cmdline = strdup ("quit");
		}

		/* empty line -> wait for another command */
		if (*cmdline == 0) {
			free(cmdline);
			continue;
		}

		/* Isolate the command word. */
		for (i = 0; cmdline[i] && whitespace (cmdline[i]); i++);
		cmdstart = cmdline + i;
		for (j = 0; cmdline[i] && !whitespace (cmdline[i]); i++, j++);
		cmd = strndup(cmdstart, j);

		/* parse the command line */
		for (i = 0; commands[i].name; i++) {
			if (strcmp(cmd, commands[i].name) == 0) {
				break;
			}
		}

		/* execute the command if any valid specified */
		if (commands[i].name) {
			commands[i].func(cmdstart);
		} else {
			/* if unknown command specified, tell it to user */
			fprintf(stdout, "%s: no such command, type 'help' for more information.\n", cmd);
		}
		add_history(cmdline);

		free(cmd);
		free(cmdline);
	}

	store_config (client_supported_cpblts);
	nc_cpblts_free(client_supported_cpblts);
	/* bye, bye */
	return (EXIT_SUCCESS);
}
