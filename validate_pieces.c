// Confirms or denies that the file passed in contains only valid tetriminos

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
	int 	valid_flag; // validity flag: 1 if valid, 0 if not valid
} boardinfo;

typedef struct piece_information {
	int 	ascii; // ascii code for corresponding capital letter identifier
	char 	*piece_string; //stores entire block of text containing a single tetrimino
	struct piece_information *next;
} tetrimino;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
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
// Getters:

int 	get_size(char *boardstr) // returns total number of chars in file
{
	int size;

	size = 0;
	while (boardstr[size])
		size++;
	return (size);
}

int 	get_rows(char *boardstr) // returns number of rows in file
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

int 	get_tcount(int rows) // returns number of tetriminos in file
{
	return ((rows + 1) / 5);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Board information:

void	fill_board_info(boardinfo *binfo)
{
	binfo->size = get_size(binfo->boardstr);
	binfo->rows = get_rows(binfo->boardstr);
	binfo->tcount = get_tcount(binfo->rows);
	binfo->valid_flag = 0;
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

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// File validation functions:

int 	check_numrows(boardinfo *binfo) // checks that file has valid number of rows
{
	// Returns 1 for valid number of rows. Returns 0 otherwise
	if (binfo->rows % 5 == 4)
		return (1);
	return (0);
}

int 	check_cols(boardinfo *binfo) // checks columns for correct chars and length
{
	// Will probably have to split off into other helper functions
	int i;
	int c; // counter
	int col_flag; // 1 if valid, 0 if invalid
	char *s;

	col_flag = 1;
	s = binfo->boardstr;
	i = 0;
	c = 0;
	while (s[i] != '\0')
	{
		// confirm only valid chars present:
		if (s[i] != '.' && s[i] != '#' && s[i] != '\n')
		{
			col_flag = 0;
			ft_putstr("Error: invalid character detected\n");
			return (col_flag); // could eleminate col_flag and just return 0
		}

		// confirm each column contains a valid number of chars:
		if (s[i] != '\n')
			c++;
		if (s[i] == '\n' && c != 0 && c != 4)
		{
			col_flag = 0;
			ft_putstr("Error: invalid column length\n");
			return (col_flag);
		}
		if(s[i] == '\n')
			c = 0;
		
		i++;
	} // end while loop

	return (col_flag);
}

int 	validate_file(boardinfo *binfo) // main function for validation
{
	int flag; // return 1 if valid file, 0 if not

	flag = 1;
	if (check_numrows(binfo) == 0) // rows check
	{
		flag = 0;
		ft_putstr("Error: invalid number of rows\n");
	}
	flag = check_cols(binfo);

	return (flag);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Piece validation functions:
/*
int 	validate_piece(boardinfo *binfo)
{

}
*/
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// File access:

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

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

int 	main(int argc, char **argv)
{
	boardinfo *binfo;

	binfo = malloc(sizeof(boardinfo));
	if (argc == 2)
	{
		binfo->boardstr = access_file(argv);
		fill_board_info(binfo);
		output_board_info(binfo);
		validate_file(binfo);
	}
	else
		ft_putstr("Invalid number of arguments\n");
	return (0);
}
