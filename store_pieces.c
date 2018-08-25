#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFFSIZE 4096

typedef struct board_information {
	char 	*boardstr;
	int 	rows;
	int 	cols;
	int 	size;
	int 	tcount; // number of tetris pieces
	int 	eflag; // error flag
} boardinfo;

/*
typedef struct tetrimino { // no idea if this will work (coordinates)
	int 				x1;
	int 				x2;
	int 				x3;
	int 				x4;
	int 				y1;
	int 				y2;
	int 				y3;
	int 				y4;
	struct tetrimino	*next;
} tmino;
*/

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Basic functions:

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
// Hardcoded tetris pieces:

int	check_square(char *boardstr) // check to see if piece is square
{
	int i;
	int flag; // set to 1 if piece is a square tetrimino

	i = 0;
	flag = 0;
	while(boardstr[i])
	{
		if (boardstr[i] == '#' && boardstr[i + 1] == '#' && boardstr[i + 5] == '#' && boardstr[i + 6] == '#')
			flag = 1;
		i++;
	}
	if (flag == 1)
		ft_putstr("Square piece\n"); // test code
	return (flag);
}

int	check_vline(char *boardstr) // vertical line piece
{
	int i;
	int flag;

	i = 0;
	flag = 0;
	while(boardstr[i])
	{
		if (boardstr[i] == '#' && boardstr[i + 5] == '#' && boardstr[i + 10] == '#' && boardstr[i + 15] == '#')
			flag = 1;
		i++;
	}
	if (flag == 1)
		ft_putstr("Vertical line piece\n"); // test code
	return (flag);
}

int check_hline(char *boardstr) // horizontal line piece
{
	int i;
	int flag;

	i = 0;
	flag = 0;
	while(boardstr[i])
	{
		if (boardstr[i] == '#' && boardstr[i + 1] == '#' && boardstr[i + 2] == '#' && boardstr[i + 3] == '#')
			flag = 1;
		i++;
	}
	if (flag == 1)
		ft_putstr("Horizontal line piece\n"); // test output
	return (flag);
}

int check_northl(char *boardstr) // north L piece
{
	int i;
	int flag;

	i = 0;
	flag = 0;
	while(boardstr[i])
	{
		if (boardstr[i] == '#' && boardstr[i + 1] == '#' && boardstr[i + 6] == '#' && boardstr[i + 11] == '#')
			flag = 1;
		i++;
	}
	if (flag == 1)
		ft_putstr("North L piece\n"); // test output
	return (flag);
}

int check_westl(char *boardstr) // west L piece
{
	int i;
	int flag;

	i = 0;
	flag = 0;
	while(boardstr[i])
	{
		if (boardstr[i] == '#' && boardstr[i + 1] == '#' && boardstr[i + 2] == '#' && boardstr[i + 5] == '#')
			flag = 1;
		i++;
	}
	if (flag == 1)
		ft_putstr("West L piece\n"); // test output
	return (flag);
}

int check_southl(char *boardstr) // south L piece
{
	int i;
	int flag;

	i = 0;
	flag = 0;
	while(boardstr[i])
	{
		if (boardstr[i] == '#' && boardstr[i + 5] == '#' && boardstr[i + 10] == '#' && boardstr[i + 11] == '#')
			flag = 1;
		i++;
	}
	if (flag == 1)
		ft_putstr("South L piece\n"); // test output
	return (flag);
}

int check_eastl(char *boardstr) // east L piece
{
	int i;
	int flag;

	i = 0;
	flag = 0;
	while(boardstr[i])
	{
		if (boardstr[i] == '#' && boardstr[i + 3] == '#' && boardstr[i + 4] == '#' && boardstr[i + 5] == '#')
			flag = 1;
		i++;
	}
	if (flag == 1)
		ft_putstr("East L piece\n"); // test output
	return (flag);
}

int check_northrl(char *boardstr) // north reverse L piece
{
	int i;
	int flag;

	i = 0;
	flag = 0;
	while(boardstr[i])
	{
		if (boardstr[i] == '#' && boardstr[i + 1] == '#' && boardstr[i + 5] == '#' && boardstr[i + 10] == '#')
			flag = 1;
		i++;
	}
	if (flag == 1)
		ft_putstr("North reverse L piece\n"); // test output
	return (flag);
}

int check_westrl(char *boardstr) // west reverse L piece
{
	int i;
	int flag;

	i = 0;
	flag = 0;
	while(boardstr[i])
	{
		if (boardstr[i] == '#' && boardstr[i + 5] == '#' && boardstr[i + 6] == '#' && boardstr[i + 7] == '#')
			flag = 1;
		i++;
	}
	if (flag == 1)
		ft_putstr("West reverse L piece\n"); // test output
	return (flag);
}

int check_southrl(char *boardstr) // south reverse L piece
{
	int i;
	int flag;

	i = 0;
	flag = 0;
	while(boardstr[i])
	{
		if (boardstr[i] == '#' && boardstr[i + 5] == '#' && boardstr[i + 9] == '#' && boardstr[i + 10] == '#')
			flag = 1;
		i++;
	}
	if (flag == 1)
		ft_putstr("South reverse L piece\n"); // test output
	return (flag);
}

int check_eastrl(char *boardstr) // east reverse L piece
{
	int i;
	int flag;

	i = 0;
	flag = 0;
	while(boardstr[i])
	{
		if (boardstr[i] == '#' && boardstr[i + 1] == '#' && boardstr[i + 2] == '#' && boardstr[i + 7] == '#')
			flag = 1;
		i++;
	}
	if (flag == 1)
		ft_putstr("East reverse L piece\n"); // test output
	return (flag);
}

int check_hz(char *boardstr) // horizontal Z piece
{
	int i;
	int flag;

	i = 0;
	flag = 0;
	while(boardstr[i])
	{
		if (boardstr[i] == '#' && boardstr[i + 1] == '#' && boardstr[i + 6] == '#' && boardstr[i + 7] == '#')
			flag = 1;
		i++;
	}
	if (flag == 1)
		ft_putstr("Horizontal Z piece\n"); // test output
	return (flag);
}

int check_vz(char *boardstr) // vertical Z piece
{
	int i;
	int flag;

	i = 0;
	flag = 0;
	while(boardstr[i])
	{
		if (boardstr[i] == '#' && boardstr[i + 4] == '#' && boardstr[i + 5] == '#' && boardstr[i + 9] == '#')
			flag = 1;
		i++;
	}
	if (flag == 1)
		ft_putstr("Vertical Z piece\n"); // test output
	return (flag);
}

int check_rhz(char *boardstr) // horizontal reverse Z piece
{
	int i;
	int flag;

	i = 0;
	flag = 0;
	while(boardstr[i])
	{
		if (boardstr[i] == '#' && boardstr[i + 1] == '#' && boardstr[i + 4] == '#' && boardstr[i + 5] == '#')
			flag = 1;
		i++;
	}
	if (flag == 1)
		ft_putstr("Horizontal reverse Z piece\n"); // test output
	return (flag);
}

int check_rvz(char *boardstr) // vertical reverse Z piece
{
	int i;
	int flag;

	i = 0;
	flag = 0;
	while(boardstr[i])
	{
		if (boardstr[i] == '#' && boardstr[i + 5] == '#' && boardstr[i + 6] == '#' && boardstr[i + 11] == '#')
			flag = 1;
		i++;
	}
	if (flag == 1)
		ft_putstr("Vertical reverse Z piece\n"); // test output
	return (flag);
}

int check_northt(char *boardstr) // north T piece
{
	int i;
	int flag;

	i = 0;
	flag = 0;
	while(boardstr[i])
	{
		if (boardstr[i] == '#' && boardstr[i + 4] == '#' && boardstr[i + 5] == '#' && boardstr[i + 6] == '#')
			flag = 1;
		i++;
	}
	if (flag == 1)
		ft_putstr("North T piece\n"); // test output
	return (flag);
}

int check_westt(char *boardstr) // west T piece
{
	int i;
	int flag;

	i = 0;
	flag = 0;
	while(boardstr[i])
	{
		if (boardstr[i] == '#' && boardstr[i + 4] == '#' && boardstr[i + 5] == '#' && boardstr[i + 10] == '#')
			flag = 1;
		i++;
	}
	if (flag == 1)
		ft_putstr("West T piece\n"); // test output
	return (flag);
}

int check_southt(char *boardstr) // south T piece
{
	int i;
	int flag;

	i = 0;
	flag = 0;
	while(boardstr[i])
	{
		if (boardstr[i] == '#' && boardstr[i + 1] == '#' && boardstr[i + 2] == '#' && boardstr[i + 6] == '#')
			flag = 1;
		i++;
	}
	if (flag == 1)
		ft_putstr("South T piece\n"); // test output
	return (flag);
}

int check_eastt(char *boardstr) // east T piece
{
	int i;
	int flag;

	i = 0;
	flag = 0;
	while(boardstr[i])
	{
		if (boardstr[i] == '#' && boardstr[i + 5] == '#' && boardstr[i + 6] == '#' && boardstr[i + 10] == '#')
			flag = 1;
		i++;
	}
	if (flag == 1)
		ft_putstr("East T piece\n"); // test output
	return (flag);
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

int 	get_tcount(int rows) // number of tetris pieces in file
{
	return ((rows + 1) / 5);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Board info functions:

void	fill_board_info(boardinfo *binfo)
{
	binfo->size = get_size(binfo->boardstr);
	binfo->rows = get_rows(binfo->boardstr);
	binfo->tcount = get_tcount(binfo->rows);
}

void	output_board_info(boardinfo *binfo) // test output
{
	ft_putstr("File contents:\n");
	ft_putstr(binfo->boardstr);
	ft_putchar('\n');

	ft_putstr("Char count: ");
	ft_putnbr(binfo->size);
	ft_putchar('\n');

	ft_putstr("Row count: ");
	ft_putnbr(binfo->rows);
	ft_putchar('\n');

	ft_putstr("Piece count: ");
	ft_putnbr(binfo->tcount);
	ft_putchar('\n');
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Tetris piece storage:



// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

char 	*access_file(char **argv)
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
	return(b2);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

int 	main(int argc, char **argv)
{
	boardinfo *binfo;

	binfo = malloc(sizeof(boardinfo));
	if (argc == 2)
	{
		binfo->boardstr = access_file(argv);
		fill_board_info(binfo);
		output_board_info(binfo);
	}
	else
		ft_putstr("Invalid number of arguments\n");
	return (0);
}
