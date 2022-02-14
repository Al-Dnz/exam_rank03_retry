#include <unistd.h>
#include <sys/wait.h>
#include <string.h>


void	ft_putstr_err(char *str)
{
	int  i = 0;
	while (str[i])
	{
		write(2, &str[i], 1);
		i++;
	}
}

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
		ft_putstr_err(argv[1]);
		ft_putstr_err("\n");
	}

}

int cmd_len(char **argv)
{
	int i = 0;
	while (argv[i] && strcmp(argv[i], ";") && strcmp(argv[i], "|"))
			i++;
	return i;
}

int is_last_cmd(char **argv, int index)
{
	if (!argv[index] || strcmp(argv[index], ";") == 0)
		return 1;
	return 0;
}

int ft_execute(char **argv, int index, int ft_stdin, char **envp)
{
	argv[index]  = NULL;
	close(ft_stdin);
	execve(argv[0], argv, envp);
	return 1;
}

int main(int argc, char **argv, char **envp)
{
	(void)argc;
	int fds[2] = {0};
	int ft_stdin = dup(STDIN_FILENO);
	int current_pid = 0;
	int index = 0;

	while (argv[index ] && argv[index +1])
	{
		argv = &argv[index+1];
		index  = cmd_len(argv);
		if (strcmp(argv[0], "cd") == 0)
			builtin_cd(index, argv);
		else if (is_last_cmd(argv, index))
		{
			current_pid = fork();
			//child
			if (current_pid == 0)
			{
				if (argv[0][0] == ';')
					return 1;
				dup2(ft_stdin, STDIN_FILENO);
				if (ft_execute(argv, index, ft_stdin, envp))
					return 1;

			}
			else
			{
				close(ft_stdin);
				waitpid(current_pid, NULL, 0);
				ft_stdin = dup(STDIN_FILENO);
			}
			
		}
		else if (strcmp(argv[index], "|") == 0)
		{
			pipe(fds);
			current_pid = fork();
			if (current_pid == 0)
			{
				dup2(ft_stdin, STDIN_FILENO);
				dup2(fds[1], STDOUT_FILENO);
				close(fds[0]);
				close(fds[1]);
				if (ft_execute(argv, index, ft_stdin, envp))
					return 1;
			}
			else
			{
				close(fds[1]);
				close(ft_stdin);
				waitpid(current_pid, NULL, 0);
				ft_stdin = dup(fds[0]);
				close(fds[0]);

			}
		}

	}

	close(ft_stdin);
	return 0;
}

