#include "runcmd.h"

int status = 0;
struct cmd *parsed_pipe;

static void
handler()
{
	int status = 0;
	int result = waitpid(0, &status, WNOHANG);
	if (result > 0) {
		char buf[BUFLEN];
		if (isatty(1)) {
			snprintf(buf,
			         sizeof buf,
			         "%s==> finished: PID=%d%s\n",
			         COLOR_BLUE,
			         result,
			         COLOR_RESET);
			if (write(1, buf, sizeof buf) < 0) {
				perror("Handler write failed\n");
				return;
			}
		}
	}
}

// runs the command in 'cmd'
int
run_cmd(char *cmd)
{
	pid_t p;
	struct cmd *parsed;

	// if the "enter" key is pressed
	// just print the prompt again
	if (cmd[0] == END_STRING)
		return 0;

	// "history" built-in call
	if (history(cmd))
		return 0;

	// "cd" built-in call
	if (cd(cmd))
		return 0;

	// "exit" built-in call
	if (exit_shell(cmd))
		return EXIT_SHELL;

	// "pwd" built-in call
	if (pwd(cmd))
		return 0;

	// parses the command line
	parsed = parse_line(cmd);

	// forks and run the command
	if ((p = fork()) == 0) {
		// keep a reference
		// to the parsed pipe cmd
		// so it can be freed later
		if (parsed->type == PIPE)
			parsed_pipe = parsed;

		exec_cmd(parsed);
	}

	// stores the pid of the process
	parsed->pid = p;

	struct sigaction sa;
	stack_t ss;

	ss.ss_sp = malloc(SIGSTKSZ);
	if (ss.ss_sp == NULL) {
		perror("error: stack malloc");
		exit(EXIT_FAILURE);
	}

	ss.ss_size = SIGSTKSZ;
	ss.ss_flags = 0;
	if (sigaltstack(&ss, NULL) == -1) {
		perror("error: sigaltstack");
		exit(EXIT_FAILURE);
	}

	sa.sa_flags = SA_ONSTACK | SA_RESTART;
	sa.sa_handler = handler;
	sigfillset(&sa.sa_mask);
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		perror("sigaction");
		exit(EXIT_FAILURE);
	}

	// background process special treatment
	// Hint:
	// - check if the process is
	//		going to be run in the 'back'
	// - print info about it with
	// 	'print_back_info()'
	//
	// Your code here
	waitpid(-1, &status, WNOHANG);
	if (parsed->type == BACK) {
		print_back_info(parsed);
		free_command(parsed);
		return 0;
	}

	// waits for the process to finish
	waitpid(p, &status, 0);

	// free alternative stack of handler
	free(ss.ss_sp);

	print_status_info(parsed);

	free_command(parsed);

	return 0;
}
