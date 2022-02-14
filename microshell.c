#include <stlib.h>
#include <unistd.h>

#include <sys/wait.h>
#include <string.h>


void builtin_cd(int len,  char **argv)
{
	if (len != 2)
	{
		ft_putstr_err("error: cd: bad arguments\n");
		return ;
	}
	else if (chdir(argv[1]))
	{
		ft_putstr_err("error: cd: cannot change directory to ");
		ft_putstr_err(args[1]);
		ft_putstr_err("\n");
	}

}

int cmd_len(char **argv)
{
	int i = 0;
	int count = 0;
	while (argv[i] && strcmp(argv[i], ";") && strcmp(argv[i], "|")
			count++;
	count;
}


int main(int argc, char **argv, char **env)
{
	(void)argc;
	int fds[2] = {0};
	int ft_stdin = dup(STDIN_FILENO);
	int current_pid = 0;
	int index = 0;
	int len;

	while (argv[index ] && argv[index +1])
	{
		argv = &argv[index+1];
		len  = cmd_length(argv);
		if (strcmp(argv[0], "cd") == 0)
			builtin_cd(len, argv);

	}



	return 0;
}

