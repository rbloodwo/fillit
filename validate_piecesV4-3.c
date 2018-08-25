// Confirms or denies that the file passed in contains only valid tetriminos

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFFSIZE 4096

typedef struct piece_information {
	int 	ascii; // ascii code for corresponding capital letter identifier
	char 	*piece_string; //stores entire block of text containing a single tetrimino
	char 	**p; // 2D piece array
	struct piece_information *next;
} tetrimino;

typedef struct board_information {
	char 		*boardstr;
	int 		rows;
	int 		cols;
	int 		size;
	int 		tcount; // number of tetris pieces
	int 		valid_flag; // validity flag: 1 if valid, 0 if not valid
	tetrimino 	*listhead;
} boardinfo;

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

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// String functions:

char	*ft_strncpy(char *dest, const char *src, size_t n)
{
	size_t i;

	i = 0;
	while (src[i] != '\0' && n > 0)
	{
		dest[i] = src[i];
		i++;
		n--;
	}
	while (n > 0)
	{
		dest[i] = '\0';
		i++;
		n--;
	}
	return (dest);
}

static	unsigned int	wordcount(char const *s, char c)
{
	unsigned int i;
	unsigned int count;

	i = 0;
	count = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i] != '\0')
			count++;
		while (s[i] != c && s[i])
			i++;
	}
	return (count);
}

static	char			*ft_strndup(const char *s, size_t n)
{
	char *dest;

	dest = (char *)malloc(sizeof(char) * n + 1);
	if (dest == NULL)
		return (NULL);
	dest = ft_strncpy(dest, s, n);
	dest[n] = '\0';
	return (dest);
}

char					**ft_strsplit(char const *s, char c)
{
	int		i;
	int		j;
	int		k;
	char	**str;

	i = 0;
	k = 0;
	if (!s)
		return (NULL);
	str = (char **)malloc(sizeof(char *) * (wordcount(s, c)) + 1);
	if (str == NULL)
		return (NULL);
	while (s[i] != '\0')
	{
		while (s[i] == c)
			i++;
		j = i;
		while (s[i] != c && s[i])
			i++;
		if (i > j)
			str[k++] = ft_strndup(s + j, i - j);
		str[k] = NULL;
	}
	return (str);
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

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Tetris piece storage:

void 	append(tetrimino **head_ref, char *tstring, int ascii)
{
	tetrimino *new_tetrimino;
	tetrimino *last;

	new_tetrimino = (tetrimino*)malloc(sizeof(tetrimino));
	last = *head_ref;
	new_tetrimino->piece_string = tstring;
	new_tetrimino->ascii = ascii;
	new_tetrimino->next = NULL;
	if (*head_ref == NULL)
	{
		*head_ref = new_tetrimino;
		return;
	}
	while (last->next != NULL)
		last = last->next;
	last->next = new_tetrimino;
	return;
}

void 	print_list(tetrimino *n)
{
	while (n != NULL)
	{
		ft_putstr(n->piece_string);
		ft_putchar('\n');
		ft_putnbr(n->ascii);
		ft_putchar('\n');
		n = n->next;
	}
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Tetris piece validation and piece 2D array functions:

void	print_piece_array(char **p) // test output
{
	int x;
	int y;

	x = 1;
	while (x < 5)
	{
		y = 1;
		while (y < 5)
		{
			ft_putchar(p[x][y]);
			y++;
		}
		ft_putchar('\n');
		x++;
	}
}

static void fill_piece_array(char **p, char *piece)
{
	int i;
	int x;
	int y;

	x = 1;
	i = 0;
	while (x < 5)
	{
		y = 1;
		while (y <  5)
		{
			if (piece[i] == '.' || piece[i] == '#')
			{
				p[x][y] = piece[i];
				y++;
			}
			i++;
		}
		x++;
	}
}

char 	**gen_piece_array(char *piece)
{
	int 	i;
	char 	**p;

	i = 0;
	p = (char **)malloc(6 * sizeof(char *)); // $$$ changed from 4
	while (i < 6)
	{
		p[i] = (char *)malloc(6 * sizeof(char)); // $$$ changed from 4
		i++;
	}
	fill_piece_array(p, piece);
	return (p);
}

static int check_connections(char **p) // checks to confirm each block connects to exactly 1 or 2 others
{
	int x;
	int y;
	int con; // number of connections

	x = 1;
//	con = 0;
	while (x < 5)
	{
		y = 1;
		while (y < 5)
		{
			con = 0;
			if (p[x][y] == '#')
			{
				if (p[x][y - 1] == '#') // north
					con++;
				if (p[x][y + 1] == '#') // south
					con++;
				if (p[x + 1][y] == '#') // east
					con++;
				if (p[x - 1][y] == '#') // west
					con++;
				if (con != 1 && con != 2)
					return (0); // invalid number of connections
			}
			y++;
		} // end y loop
		x++;
	} // end x loop

	return (1); // valid number of connections
}

static int check_numblocks(char **p) // checks to confirm piece only contains 4 blocks
{
//	print_piece_array(p); // $$$
	int x;
	int y;
	int count;

	x = 1;
	count = 0;
	while (x < 5)
	{
		y = 1;
		while (y < 5)
		{
			if (p[x][y] == '#')
				count++;
			y++;
		}
		x++;
	}
	ft_putstr("Block count: "); // $$$
	ft_putnbr(count); // $$$
	ft_putchar('\n'); // $$$
	if (count == 4)
		return (1); // 1 means number of blocks is valid
	return (0); // 0 means invalid
}

void 	validate_list(tetrimino *n) // validate the list
{
	char *piece;
	char **p; // 2D piece array

	while (n != NULL)
	{
		piece = n->piece_string;

		p = gen_piece_array(piece);
		n->p = p;
		print_piece_array(p); // test output

		if (check_numblocks(p) == 0)
		{
			ft_putstr("Error: invalid number of blocks\n");
		}

		if (check_connections(p) == 0)
		{
			ft_putstr("Error: invalid number of connections\n");
		}

//		ft_putstr(piece); // test output
//		ft_putchar('\n'); // test output
		n = n->next;
	}
//	return (1); // every piece is valid
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Functions to break apart boardstr, validate each element, and add each element to list:

char 	*get_piece(const char *s, int i, int j) // extracts block of text from i to j indices
{
	// maybe make function static
	int x;
	char *p; // the piece to be extracted

	x = 0;
	p = (char *)malloc(sizeof(char) * (j - i) + 1);
	while (i < j)
	{
		p[x] = s[i];
		i++;
		x++;
	}
	return (p);
}

void	make_list(boardinfo *binfo) // if this gets messed up just copy and paste in from V4-2
{
	// add in piece validation
	int 		i;
	int 		j;
	int 		c;
	char 		*piece;
	tetrimino 	*head;
	
	i = 0;
	j = 1;
	c = 0;
	head = NULL;
	if (binfo->tcount > 1) // if there are multiple tetriminos
	{
		while (binfo->boardstr[j]) // deals with all but last piece
		{
			if (binfo->boardstr[j] == '\n' && binfo->boardstr[j - 1] == '\n')
			{
				if (i == 0)
					piece = get_piece(binfo->boardstr, i, j - 1);
				else
					piece = get_piece(binfo->boardstr, i + 1, (j - 1));
				append(&head, piece, 65 + c);
				i = j;
				c++;
			}
			j++;
		}
		piece = get_piece(binfo->boardstr, (i + 1), (j - 1)); // gets last piece
	}
	else // if there's only one tetrimino
		piece = get_piece(binfo->boardstr, i, 20);
	append(&head, piece, 65 + c);
	print_list(head); // test output
	binfo->listhead = head; // $$$
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
		make_list(binfo);
		validate_list(binfo->listhead); // I need to make a function that validates each piece in the list
	}
	else
		ft_putstr("Invalid number of arguments\n");
	return (0);
}