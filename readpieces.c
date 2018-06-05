// Test code for reading in and storing each tetrimino

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFFSIZE 4096

typedef struct board_information {
	int rows;
	int cols;
	int size;
	int tcount; // number of tetriminos
	int eflag; // error flag
} boardinfo;

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
// Getters:

int 	get_size(char *boardstr) // total number of chars in file
{
	int size;

	size = 0;
	while(boardstr[size])
		size++;
	return (size);
}

int 	get_rows(char *boardstr) // number of rows in file
{
	int i;
	int rows;

	i = 0;
	rows = 0;
	while (boardstr[i])
	{
		if (boardstr[i] == '\n')
			rows++;
		i++;
	}
	return (rows);
}

int 	get_tcount(int rows) // number of tetriminos in file
{
	return ((rows + 1) / 5);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void	fill_board_info(boardinfo *binfo, char *boardstr)
{
	binfo->size = get_size(boardstr);
	binfo->rows = get_rows(boardstr);
	binfo->tcount = get_tcount(binfo->rows);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void 	assign_board_mem(char **board, boardinfo *binfo) // !
{
	int i;

	i = 0;
	board = (char **)malloc(binfo->tcount * sizeof(char *));
	while (i < binfo->tcount) 
	{
		board[i] = (char *)malloc((9) * sizeof(char)); // assuming that no string representing a tetrimino can exceed 8 chars
		i++;
	}
}

char	*access_file(char **argv) // returns file as a single string
{
	int fd;
	int buff;
	char *b2;
	char buffer[BUFFSIZE + 1];

	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
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
	return(b2);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

int		main(int argc, char **argv)
{
	boardinfo 	*binfo; // board information
	char 		*boardstr; // single string containing full file text
	char		**board; // board to store each tetrimino

	binfo = malloc(sizeof(boardinfo));
	if (argc == 2)
	{
		boardstr = access_file(argv);
		fill_board_info(binfo, boardstr);
		assign_board_mem(board, binfo);
		//Test output:
			ft_putstr(boardstr);
			ft_putnbr(binfo->tcount);
			ft_putchar('\n');
	}
	else
		ft_putstr("Invalid number of arguments\n");
	return (0);
}
