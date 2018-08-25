// Test code for reading in and storing each tetrimino

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFFSIZE 4096

typedef struct board_information {
	char 	*boardstr;
	int 	rows;
	int 	cols;
	int 	size;
	int 	tcount; // number of tetriminos
	int 	eflag; // error flag
} boardinfo;

/*
typedef struct tetrimino {

} tmino;
*/

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
	unsigned int	i;
	int				n;
	int				neg_flag;

	i = 0;
	n = 0;
	neg_flag = 0;
	while ((str[i] == ' ') || (str[i] == '\f') || (str[i] == '\n')
		|| (str[i] == '\r') || (str[i] == '\t') || (str[i] == '\v'))
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

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

char	*access_file(char **argv)
{
	int fd;
	int buff;
	char *b2;
	char buffer[BUFFSIZE + 1];

	fd = open(argv[1], O_RDONLY);
	if (fd == -1) // if file access fails
	{
		buffer[0] = '\0';
		b2 = buffer;
		return (b2);
	}
	else
	{
		while ((buff = read(fd, buffer, BUFFSIZE)))
		{
			buffer[buff] = '\0';
		}
	}
	b2 = buffer;
	close(fd);
	return (b2);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

char 	*extract_piece(boardinfo *binfo) // on the right track. still some issues. check with exfile3 for example
{
	char *piece;
	unsigned int i;
	unsigned int j;

	piece = (char *)malloc((9) * sizeof(char));
	i = 0;
	j = 0;
	while (binfo->boardstr[i])
	{
		if (binfo->boardstr[i] == '#')
		{
			piece[j] = '#';
			j++;
		}
		else if (i > 0 && binfo->boardstr[i] == '.' && binfo->boardstr[i - 1] == '#')
		{
			piece[j] = '\n';
			j++;
		}
		else if(binfo->boardstr[i] == '.' && binfo->boardstr[i + 1] == '#')
		{
			piece[j] = '.';
			j++;
		}
		i++;
	}
	return (piece);
}

/*
void	 extract_piece(boardinfo *binfo) // maybe void instead?
{
	char *piece;

	piece = (char *)malloc((9) * sizeof(char)); // maybe 10
	piece[0] = '#';
	piece[1] = '#';
	piece[2] = '\n';
	piece[3] = '.';
	piece[4] = '#';
	piece[5] = '\n';
	piece[6] = '.';
	piece[7] = '#';
	piece[8] = '\n';
	//piece[9] = '\0';
	ft_putstr(piece);
}
*/

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

int 	main(int argc, char **argv)
{
	boardinfo 	*binfo;

	binfo = malloc(sizeof(boardinfo));
	if (argc == 2)
	{
		binfo->boardstr = access_file(argv);
		// Test output:
		ft_putstr(binfo->boardstr);
		ft_putchar('\n');
		ft_putstr(extract_piece(binfo));
		ft_putchar('\n');
	}
	else
		ft_putstr("Invalid number of arguments\n");
	return (0);
}
