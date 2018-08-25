// Most recent code as of 7/16/18

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

typedef struct piece_information {
	int 	ascii; // ascii code for corresponding capital letter identifier
	char 	*piece_string; //stores entire block of text containing a single tetrimino
	struct piece_information *next;
} tetrimino;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
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

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// List functions:


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Hardcoded tetris pieces:

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
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

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
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

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Tetris piece storage:

void 	append(tetrimino **head_ref, char *tstring)
{
	tetrimino *new_tetrimino;
	tetrimino *last;

	new_tetrimino = (tetrimino*)malloc(sizeof(tetrimino));
	last = *head_ref;
	new_tetrimino->piece_string = tstring;
	//new_tetrimino->ascii = ascii;
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

void 	printList(tetrimino *n)
{
	while (n != NULL)
	{
		ft_putstr(n->piece_string);
		ft_putchar('\n');
		n = n->next;
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Functions to break apart boardstr and add each element to list:
/*
	To do here:
	- make the list
	- split boardstr into separate elements
	- add each element to the list
*/

char 	*get_piece(const char *s, int i, int j) // extracts block of text from i to j indices
{
	int x;
	char *p; // the piece to be extracted

	x = 0;
	p = (char *)malloc(sizeof(char) * (j - i) + 1);
	while(i < j)
	{
		p[x] = s[i];
		i++;
		x++;
	}
	return (p);
}

void 	make_list(boardinfo *binfo)
{
	int 		i;
	int 		j;
	char 	  	*piece;
	tetrimino 	*head;

	i = 0;
	j = 1;
	head = NULL;
	while (binfo->boardstr[j]) // works for all but last piece
	{
		if (binfo->boardstr[j] == '\n' && binfo->boardstr[j - 1] == '\n')
		{
			if (i == 0)
				piece = get_piece(binfo->boardstr, i, j - 1);
			else
				piece = get_piece(binfo->boardstr, i + 1, (j - 1));
			append(&head, piece);
			//ft_putstr(piece); // test output
			i = j;
		}
		j++;
	}
	piece = get_piece(binfo->boardstr, (i + 1), (j - 1)); // gets last piece
	append(&head, piece);
	printList(head);
	//ft_putchar('\n'); // test output
	//ft_putstr(piece); // test output
	
	//piece = get_piece(binfo->boardstr, 0, 20);
	//ft_putstr(piece);
}
	//append(&head, "hello, world");
	//printList(head);
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

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

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

int 	main(int argc, char **argv)
{
	boardinfo *binfo;
	//tetrimino *head;
	//tetrimino *tmino; // maybe **tmino

	binfo = malloc(sizeof(boardinfo));
	if (argc == 2)
	{
		binfo->boardstr = access_file(argv);
		fill_board_info(binfo);
		output_board_info(binfo);
		make_list(binfo);
	}
	else
		ft_putstr("Invalid number of arguments\n");
	return (0);
}
