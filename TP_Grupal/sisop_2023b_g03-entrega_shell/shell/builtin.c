#include "builtin.h"

// returns true if the 'exit' call
// should be performed
//
// (It must not be called from here)
int
exit_shell(char *cmd)
{
	extern int status;
	if (strcmp(cmd, "exit") == 0) {
		status = 0;
		return true;
	}
	return false;
}

// returns true if "chdir" was performed
//  this means that if 'cmd' contains:
// 	1. $ cd directory (change to 'directory')
// 	2. $ cd (change to $HOME)
//  it has to be executed and then return true
//
//  Remember to update the 'prompt' with the
//  	new directory.
//
// Examples:
//  1. cmd = ['c','d', ' ', '/', 'b', 'i', 'n', '\0']
//  2. cmd = ['c','d', '\0']
int
cd(char *cmd)
{
	// Your code here
	extern int status;

	char cmd_copy[BUFLEN];
	strcpy(cmd_copy, cmd);

	char *arg = strchr(cmd_copy, ' ');
	strtok(cmd_copy, " ");

	if (strcmp(cmd_copy, "cd") == 0) {
		char *directory = getenv("HOME");
		if (arg != NULL)
			directory = arg + 1;

		if (chdir(directory) == 0) {
			status = 0;
			char *path = getcwd(path, BUFLEN);
			if (path == NULL)
				status = 1;
			snprintf(prompt, BUFLEN, "(%s)", path);
			free(path);
		} else
			status = 1;

		return true;
	}

	return 0;
}

// returns true if 'pwd' was invoked
// in the command line
//
// (It has to be executed here and then
// 	return true)
int
pwd(char *cmd)
{
	extern int status;
	if (strcmp(cmd, "pwd") == 0) {
		char buf[BUFLEN] = { 0 };
		if (getcwd(buf, sizeof buf) != NULL) {
			printf("%s\n", buf);
		} else {
			perror("directory not found");
			status = -1;
			return true;
		}
		status = 0;
		return true;
	}
	return false;
}

// returns true if `history` was invoked
// in the command line
//
// (It has to be executed here and then
// 	return true)
int
history(char *cmd)
{
	// Your code here

	return 0;
}
