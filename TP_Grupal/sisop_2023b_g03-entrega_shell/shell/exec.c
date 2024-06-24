#include "exec.h"
const int OVERWRITE = 1;
#define STDIN 0
#define STDOUT 1

// sets "key" with the key part of "arg"
// and null-terminates it
//
// Example:
//  - KEY=value
//  arg = ['K', 'E', 'Y', '=', 'v', 'a', 'l', 'u', 'e', '\0']
//  key = "KEY"
//
static void
get_environ_key(char *arg, char *key)
{
	int i;
	for (i = 0; arg[i] != '='; i++)
		key[i] = arg[i];

	key[i] = END_STRING;
}

// sets "value" with the value part of "arg"
// and null-terminates it
// "idx" should be the index in "arg" where "=" char
// resides
//
// Example:
//  - KEY=value
//  arg = ['K', 'E', 'Y', '=', 'v', 'a', 'l', 'u', 'e', '\0']
//  value = "value"
//
static void
get_environ_value(char *arg, char *value, int idx)
{
	size_t i, j;
	for (i = (idx + 1), j = 0; i < strlen(arg); i++, j++)
		value[j] = arg[i];

	value[j] = END_STRING;
}

static void
xsetenv(const char *key, const char *value, int overwrite)
{
	int result = setenv(key, value, overwrite);
	if (result == 1) {
		err(1, "Error: setenv failure\n");
	}
}

// sets the environment variables received
// in the command line
//
// Hints:
// - use 'block_contains()' to
// 	get the index where the '=' is
// - 'get_environ_*()' can be useful here
static void
set_environ_vars(char **eargv, int eargc)
{
	// Your code here
	char key[BUFLEN];
	char value[BUFLEN];

	for (int i = 0; i < eargc; i++) {
		int idx = block_contains(eargv[i], '=');
		get_environ_key(eargv[i], key);
		get_environ_value(eargv[i], value, idx);
		xsetenv(key, value, OVERWRITE);
	}
}

// opens the file in which the stdin/stdout/stderr
// flow will be redirected, and returns
// the file descriptor
//
// Find out what permissions it needs.
// Does it have to be closed after the execve(2) call?
//
// Hints:
// - if O_CREAT is used, add S_IWUSR and S_IRUSR
// 	to make it a readable normal file
static int
open_redir_fd(char *file, int flags)
{
	// Your code here
	int fd = open(file, flags, S_IRUSR | S_IWUSR);
	if (fd < 0)
		err(1, "%s", file);

	return fd;
}

// executes a command - does not return
//
// Hint:
// - check how the 'cmd' structs are defined
// 	in types.h
// - casting could be a good option
void
exec_cmd(struct cmd *cmd)
{
	// To be used in the different cases
	struct execcmd *e;
	struct backcmd *b;
	struct execcmd *r;
	struct pipecmd *p;

	switch (cmd->type) {
	case EXEC:
		// spawns a command
		//
		// Your code here
		e = (struct execcmd *) cmd;
		// set a pgid equal to pid for exec command.
		setpgid(0, 0);
		set_environ_vars(e->eargv, e->eargc);
		execvp(e->argv[0], e->argv);

		printf("Commands are not yet implemented\n");
		_exit(-1);
		break;

	case BACK: {
		// runs a command in background
		//
		// Your code here
		b = (struct backcmd *) cmd;
		e = (struct execcmd *) b->c;

		// set a pgid equal to pid for the parent of the background command
		setpgid(0, getppid());

		execvp(e->argv[0], e->argv);

		printf("Background process are not yet implemented\n");
		_exit(-1);
		break;
	}

	case REDIR: {
		// changes the input/output/stderr flow
		//
		// To check if a redirection has to be performed
		// verify if file name's length (in the execcmd struct)
		// is greater than zero
		//
		// Your code here
		r = (struct execcmd *) cmd;


		if (strcmp(r->err_file, "&1") == 0) {
			int stdout_fd = dup(1);

			if (strlen(r->out_file) > 0) {
				int fd = open_redir_fd(r->out_file,
				                       O_CREAT | O_WRONLY);
				dup2(fd, 2);
				dup2(fd, 1);

				close(fd);
			}
			set_environ_vars(r->eargv, r->eargc);
			execvp(r->argv[0], r->argv);

			dup2(stdout_fd, 1);
			close(stdout_fd);

		} else if (strlen(r->err_file) > 0) {
			int fd = open_redir_fd(r->err_file, O_CREAT | O_WRONLY);
			dup2(fd, 2);
			close(fd);
			set_environ_vars(r->eargv, r->eargc);
			execvp(r->argv[0], r->argv);


		}

		else {
			if (strlen(r->out_file) > 0) {
				int fd = open_redir_fd(r->out_file,
				                       O_TRUNC | O_CREAT |
				                               O_WRONLY);
				dup2(fd, 1);
				close(fd);
			} else if (strlen(r->in_file) > 0) {
				int fd = open_redir_fd(r->in_file, O_RDONLY);
				dup2(fd, 0);
				close(fd);
			}

			set_environ_vars(r->eargv, r->eargc);
			execvp(r->argv[0], r->argv);
		}

		printf("Redirections are not yet implemented\n");
		_exit(-1);
		break;
	}

	case PIPE: {
		// pipes two commands
		//
		// Your code here
		p = (struct pipecmd *) cmd;

		// only used if leftcmd has temporally environ vars
		e = (struct execcmd *) p->leftcmd;

		int pipe_fd[2];
		if (pipe(pipe_fd) < 0) {
			printf("Error in pipe\n");
			exit(-1);
		}

		pid_t pid_izquierdo = fork();
		if (pid_izquierdo < 0) {
			printf("Error in fork\n");
			exit(-1);
		}

		if (pid_izquierdo == 0) {
			close(pipe_fd[0]);

			// redirigir la stdout al extremo de escritura del pipe
			if (dup2(pipe_fd[1], STDOUT) < 0) {
				printf("Error in dup\n");
				exit(-1);
			}

			close(pipe_fd[1]);

			set_environ_vars(e->eargv, e->eargc);
			exec_cmd(p->leftcmd);

			exit(1);
		} else {
			pid_t pid_derecho = fork();
			if (pid_derecho == -1) {
				printf("Error in fork\n");
				exit(-1);
			}

			if (pid_derecho == 0) {
				close(pipe_fd[1]);

				// redirigir la stdin al extremo de lectura del pipe
				if (dup2(pipe_fd[0], STDIN) < 0) {
					printf("Error in dup\n");
					exit(-1);
				}

				close(pipe_fd[0]);

				exec_cmd(p->rightcmd);

				exit(1);
			} else {
				close(pipe_fd[0]);
				close(pipe_fd[1]);


				waitpid(pid_izquierdo, NULL, 0);
				waitpid(pid_derecho, NULL, 0);
			}
		}


		// printf("Pipes are not yet implemented\n");

		break;
	}
	}
}
