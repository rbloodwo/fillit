// finds the next largest square to number of pieces to determine board width

#include <fcntl.h>
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
	if (nb >= 0 && nb <= 9)
	{
		ft_putchar(nb + '0');
	}
	if (nb < 0)
	{
		ft_putchar('-');
		ft_putnbr(-nb);
	}
}

int 	ft_atoi(char *str)
{
	unsigned int 	i;
	int 			n;
	int 			neg_flag;

	i = 0;
	n = 0;
	neg_flag = 0;
	while ((str[i] == '\r') || (str[i] == '\t') || (str[i] == ' ')
		|| (str[i] == '\f') || (str[i] == '\v') || (str[i] == '\n'))
		i++;
	if (str[i] == '-')
		neg_flag = 1;
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		n *= 10;
		n += ((int)str[i] - '0');
		i++;
	}
	if (neg_flag == 1)
		return (-n);
	return (n);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

int 	get_next_largest_square(int num)
{
	int n;
	
	n = 0;
	while ((n * n) < num)
		n++;
	return (n * n);
}

int 	main(int argc, char **argv)
{
	int n = ft_atoi(argv[1]);

	ft_putnbr(get_next_largest_square(n));
	ft_putchar('\n');
	return (0);
}
