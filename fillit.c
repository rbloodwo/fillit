/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fillit.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rbloodwo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/30 18:19:15 by rbloodwo          #+#    #+#             */
/*   Updated: 2018/08/30 18:19:18 by rbloodwo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFFSIZE 4096

typedef struct		s_tetris {
	int				ascii;
	char			*piece_string;
	char			**p;
	struct s_tetris *next;
	int				ox;
	int				oy;
	int				x2;
	int				y2;
	int				x3;
	int				y3;
	int				x4;
	int				y4;
}					t_mino;

typedef struct		s_board_information {
	char			*boardstr;
	char			**board;
	int				rows;
	int				cols;
	int				size;
	int				tcount;
	int				board_size;
	int				valid_flag;
	t_mino			*listhead;
}					t_boardinfo;

/*
** - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - **
** Basic funcitons:
** - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - **
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

int		ft_atoi(char *str)
{
	unsigned int	i;
	int				n;
	int				neg_flag;

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

/*
** - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - **
** String functions:
** - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - **
*/

char					*ft_strncpy(char *dest, const char *src, size_t n)
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

char	*ft_strdup(const char *s1)
{
	int		i;
	int		len;
	char	*str;

	i = 0;
	len = 0;
	while (s1[len])
		len++;
	str = (char *)malloc(sizeof(*str) * (len + 1));
	if (str == NULL)
		return (NULL);
	while (s1[i] != '\0')
	{
		str[i] = s1[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

/*
** - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - **
** Getters and board information:
** - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - **
*/

int		get_size(char *boardstr)
{
	int size;

	size = 0;
	while (boardstr[size])
		size++;
	return (size);
}

int		get_rows(char *boardstr)
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

int		get_tcount(int rows)
{
	return ((rows + 1) / 5);
}

int		get_next_largest_square(int num)
{
	int n;

	n = 0;
	while ((n * n) < num)
		n++;
	return (n);
}

void	fill_board_info(t_boardinfo *binfo)
{
	binfo->size = get_size(binfo->boardstr);
	binfo->rows = get_rows(binfo->boardstr);
	binfo->tcount = get_tcount(binfo->rows);
	binfo->valid_flag = 0;
}

/*
** - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - **
** File validation:
** - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - **
*/

int		check_numrows(t_boardinfo *binfo)
{
	if (binfo->rows % 5 == 4)
		return (1);
	return (0);
}

int		check_cols(t_boardinfo *binfo)
{
	int		i;
	int		c;
	char	*s;

	i = 0;
	c = 0;
	s = binfo->boardstr;
	while (s[i] != '\0')
	{
		if (s[i] != '.' && s[i] != '#' && s[i] != '\n')
			return (0);
		if (i > 0 && s[i] == '\n' && s[i - 1] == '\n' && s[i + 1] == '\n')
			return (0);
		if (s[i] != '\n')
			c++;
		if (s[i] == '\n' && c != 0 && c != 4)
			return (0);
		if (s[i] == '\n')
			c = 0;
		i++;
	}
	return (1);
}

int		check_gaps(t_boardinfo *binfo) // new function to check blank spaces between pieces are blank
{
	int		i;
	int		row;
	char	*s;

	i = 0;
	row = 0;
	s = binfo->boardstr;
	while (s[i] != '\0')
	{
		if (s[i] == '\n')
			row++;
		if (row % 5 == 4 && s[i] != '\n')
			return (0);
		i++;
	}
	return (1);
}

int		validate_file(t_boardinfo *binfo)
{
	if (check_numrows(binfo) == 0)
		return (0);
	if (check_cols(binfo) == 0)
		return (0);
	if (check_gaps(binfo) == 0)
		return (0);
	if (binfo->tcount < 1 || binfo->tcount > 26)
		return (0);
	return (1);
}

/*
** - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - **
** List creation:
** - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - **
*/

void	append(t_mino **head_ref, char *tstring, int ascii)
{
	t_mino	*new_tetrimino;
	t_mino	*last;

	new_tetrimino = (t_mino*)malloc(sizeof(t_mino));
	last = *head_ref;
	new_tetrimino->piece_string = tstring;
	new_tetrimino->ascii = ascii;
	new_tetrimino->next = NULL;
	if (*head_ref == NULL)
	{
		*head_ref = new_tetrimino;
		return ;
	}
	while (last->next != NULL)
		last = last->next;
	last->next = new_tetrimino;
	return ;
}

void	print_list(t_mino *n)
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

char	*get_piece(const char *s, int i, int j)
{
	int		x;
	char	*p;

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

void	make_list_multi(t_boardinfo *binfo, t_mino *head)
{
	int		i;
	int		j;
	int		c;
	char	*piece;

	i = 0;
	j = 1;
	c = 0;
	while (binfo->boardstr[j])
	{
		if (binfo->boardstr[j] == '\n' && binfo->boardstr[j - 1] == '\n')
		{
			if (i == 0)
				piece = get_piece(binfo->boardstr, i, j - 1);
			else
				piece = get_piece(binfo->boardstr, i + 1, j - 1);
			append(&head, piece, 65 + c);
			i = j;
			c++;
		}
		j++;
	}
	piece = get_piece(binfo->boardstr, i + 1, j - 1);
	append(&head, piece, 65 + c);
	binfo->listhead = head;
}

void	make_list(t_boardinfo *binfo)
{
	char	*piece;
	t_mino	*head;

	head = NULL;
	if (binfo->tcount > 1)
		make_list_multi(binfo, head);
	else
	{
		piece = get_piece(binfo->boardstr, 0, 20);
		append(&head, piece, 65);
		binfo->listhead = head;
	}
}

/*
** - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - **
** List validation:
** - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - **
*/

void	fill_piece_array(char **p, char *piece)
{
	int i;
	int x;
	int y;

	y = 1;
	i = 0;
	while (y < 5)
	{
		x = 1;
		while (x < 5)
		{
			if (piece[i] == '.' || piece[i] == '#')
			{
				p[y][x] = piece[i];
				x++;
			}
			i++;
		}
		y++;
	}
}

char	**gen_piece_array(char *piece)
{
	int		i;
	char	**p;

	i = 0;
	p = (char **)malloc(6 * sizeof(char *));
	while (i < 6)
	{
		p[i] = (char *)malloc(6 * sizeof(char));
		i++;
	}
	fill_piece_array(p, piece);
	return (p);
}

int		check_numblocks(char **p)
{
	int x;
	int y;
	int count;

	y = 1;
	count = 0;
	while (y < 5)
	{
		x = 1;
		while (x < 5)
		{
			if (p[y][x] == '#')
				count++;
			x++;
		}
		y++;
	}
	if (count == 4)
		return (1);
	return (0);
}

int		check_connections(char **p, int y, int x)
{
	int con;

	y = 0;
	while (++y < 5)
	{
		x = 0;
		while (++x < 5)
		{
			con = 0;
			if (p[y][x] == '#')
			{
				if (p[y][x - 1] == '#')
					con++;
				if (p[y][x + 1] == '#')
					con++;
				if (p[y + 1][x] == '#')
					con++;
				if (p[y - 1][x] == '#')
					con++;
				if (con != 1 && con != 2 && con != 3)
					return (0);
			}
		}
	}
	return (1);
}

int		validate_list(t_mino *n)
{
	char *piece;
	char **p;

	while (n != NULL)
	{
		piece = n->piece_string;
		p = gen_piece_array(piece);
		n->p = p;
		if (check_numblocks(p) == 0)
			return (0);
		if (check_connections(p, 0, 0) == 0)
			return (0);
		n = n->next;
	}
	return (1);
}

/*
** - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - **
** Piece coordinates:
** - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - **
*/

void	set_offsets(t_mino *n, int count, int y, int x)
{
	if (count == 1)
	{
		n->x2 = x - n->ox;
		n->y2 = y - n->oy;
	}
	if (count == 2)
	{
		n->x3 = x - n->ox;
		n->y3 = y - n->oy;
	}
	if (count == 3)
	{
		n->x4 = x - n->ox;
		n->y4 = y - n->oy;
	}
}

void	set_coordinates(char **p, t_mino *n)
{
	int y;
	int x;
	int count;

	y = 0;
	count = 0;
	while (++y < 5)
	{
		x = 0;
		while (++x < 5)
		{
			if (p[y][x] == '#' && count > 0)
			{
				set_offsets(n, count, y, x);
				count++;
			}
			if (p[y][x] == '#' && count == 0)
			{
				n->oy = y;
				n->ox = x;
				count++;
			}
		}
	}
}

void	assign_coordinates(t_mino *n)
{
	while (n != NULL)
	{
		set_coordinates(n->p, n);
		n = n->next;
	}
}

/*
** - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - **
** Board creation:
** - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - **
*/

void	print_board_array(char **board, int n)
{
	int y;
	int x;

	y = 0;
	while (y < n)
	{
		x = 0;
		while (x < n)
		{
			ft_putchar(board[y][x]);
			x++;
		}
		ft_putchar('\n');
		y++;
	}
}

void	fill_board_array(char **board, int n)
{
	int y;
	int x;

	y = 0;
	while (y < n)
	{
		x = 0;
		while (x < n)
		{
			board[y][x] = '.';
			x++;
		}
		y++;
	}
}

char	**increase_board_size(t_boardinfo *binfo)
{
	int		i;
	int		n;
	char	**board;

	i = 0;
	n = binfo->board_size + 1;
	binfo->board_size = n;
	board = (char **)malloc(n * sizeof(char *));
	while (i < n)
	{
		board[i] = (char *)malloc(n * sizeof(char));
		i++;
	}
	fill_board_array(board, n);
	return (board);
}

char	**gen_board_array(t_boardinfo *binfo)
{
	int		i;
	int		n;
	char	**board;

	i = 0;
	n = get_next_largest_square(binfo->tcount * 4);
	if (n == 1)
		n = 4;
	binfo->board_size = n;
	board = (char **)malloc(n * sizeof(char *));
	while (i < n)
	{
		board[i] = (char *)malloc(n * sizeof(char));
		i++;
	}
	fill_board_array(board, n);
	return (board);
}

/*
** - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - **
** Solver:
** - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - **
*/

void	piece_remove(t_boardinfo *binfo, t_mino *t, int y1, int x1)
{
	binfo->board[y1][x1] = '.';
	binfo->board[y1 + t->y2][x1 + t->x2] = '.';
	binfo->board[y1 + t->y3][x1 + t->x3] = '.';
	binfo->board[y1 + t->y4][x1 + t->x4] = '.';
}

void	piece_place(t_boardinfo *binfo, t_mino *t, int y1, int x1)
{
	char c;

	c = t->ascii;
	binfo->board[y1][x1] = c;
	binfo->board[y1 + t->y2][x1 + t->x2] = c;
	binfo->board[y1 + t->y3][x1 + t->x3] = c;
	binfo->board[y1 + t->y4][x1 + t->x4] = c;
}

int		is_safe(t_boardinfo *binfo, t_mino *t, int y1, int x1)
{
	int	count;
	int	n;

	count = 0;
	n = binfo->board_size;
	if (y1 + t->y2 >= n || y1 + t->y3 >= n || y1 + t->y4 >= n
		|| x1 + t->x2 >= n || x1 + t->x3 >= n || x1 + t->x4 >= n)
		return (0);
	if (binfo->board[y1][x1] == '.')
		count++;
	if (binfo->board[y1 + t->y2][x1 + t->x2] == '.')
		count++;
	if (binfo->board[y1 + t->y3][x1 + t->x3] == '.')
		count++;
	if (binfo->board[y1 + t->y4][x1 + t->x4] == '.')
		count++;
	if (count == 4)
		return (1);
	return (0);
}

int		is_finished(t_boardinfo *binfo, t_mino *t)
{
	if (t == NULL)
	{
		print_board_array(binfo->board, binfo->board_size);
		return (1);
	}
	return (0);
}

int		solve_util(t_boardinfo *binfo, t_mino *t, int x, int y)
{
	if (is_finished(binfo, t) == 1)
		return (1);
	while (t != NULL)
	{
		while (y < binfo->board_size)
		{
			x = 0;
			while (x < binfo->board_size)
			{
				if (is_safe(binfo, t, y, x) == 1)
				{
					piece_place(binfo, t, y, x);
					if (solve_util(binfo, t->next, 0, 0) == 1)
						return (1);
					piece_remove(binfo, t, y, x);
				}
				if (y == binfo->board_size && x == binfo->size)
					return (0);
				x++;
			}
			y++;
		}
		t = t->next;
	}
	return (0);
}

/*
** - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - **
** Driver:
** - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - **
*/

void	fillit(t_boardinfo *binfo)
{
	fill_board_info(binfo);
	if (validate_file(binfo) == 0)
	{
		ft_putstr("error\n");
		return ;
	}
	make_list(binfo);
	if (validate_list(binfo->listhead) == 0)
	{
		ft_putstr("error\n");
		return ;
	}
	assign_coordinates(binfo->listhead);
	binfo->board = gen_board_array(binfo);
	while (solve_util(binfo, binfo->listhead, 0, 0) == 0)
		binfo->board = increase_board_size(binfo);
}

/*
** - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - **
** Main and file access:
** - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - **
*/

char	*access_file(char **argv)
{
	int		fd;
	int		buff;
	char	*b2;
	char	buffer[BUFFSIZE + 1];

	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
	{
		b2 = NULL;
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

int		main(int argc, char **argv)
{
	t_boardinfo *binfo;

	binfo = malloc(sizeof(t_boardinfo));
	if (argc == 2)
	{
		binfo->boardstr = access_file(argv);
		if (binfo->boardstr == NULL)
		{
			ft_putstr("error\n");
			return (0);
		}
		fillit(binfo);
	}
	return (0);
}
