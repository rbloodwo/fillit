// Test code to read in and output entire file

#include <fcntl.h> // needed for file reading
#include <unistd.h>
#include <stdlib.h>

#define BUFFSIZE 4096

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	ft_putstr(char *str)
{
	int i;

	i = 0;
	while (str[i] != '\0')
	{
		ft_putchar(str[i]);
		i++;
	}
}

void	ft_putnbr(int nb)
{
	if (nb >= 10)
	{
		ft_putnbr(nb / 10);
		ft_putchar(nb % 10 + '0');
	}
	if (nb >= 0 && nb < 10)
	{
		ft_putchar(nb + '0');
	}
	if (nb < 0)
	{
		ft_putchar('-');
		ft_putnbr(-nb);
	}
}

int		ft_atoi(char *str)
{
	int i;
	int n;
	int neg_flag;

	i = 0;
	n = 0;
	neg_flag = 0;

	while (str[i] == '\r' || str[i] == '\t' || str[i] == ' '|| str[i] == '\f' || str[i] == '\v' || str[i] == '\n')
	i++;
	if (str[i] == '-')
		neg_flag = 1;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		n *= 10;
		n += ((int)(str[i] - '0'));
		i++;
	}
	if (neg_flag == 1)
		return (-n);
	else
		return (n);
}

//-----------------------------------------------------------------------------------------------------------

int		main(int argc, char **argv)
{
	int fd;
	int buff;
	char buffer[BUFFSIZE + 1];

	if (argc == 2)
	{
		fd = open(argv[1], O_RDONLY);
		if (fd == -1)
			write(2, "No file.\n", 10);
		else
		{
			while ((buff = read(fd, buffer, BUFFSIZE)))
			{
				buffer[buff] = '\0';
				ft_putstr(buffer);
			}
		}
		close(fd);
	}
	else if (argc < 2)
		write(2, "File name missing.\n", 20);
	else if (argc > 2)
		write(2, "Too many arguments.\n", 21);
	return (0);
}

