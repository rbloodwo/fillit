#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include "sheader.h"
#define BUFFSIZE 4098

void	putchar(char c)
{
	write(1, &c, 1);
}

void	putstr(char *str)
{
	int i;

	i = 0;
	while(str[i])
	{
		putchar(str[i]);
		i++;
	}
}

void	putnbr(int nb)
{
	if (nb >= 10)
	{
		putnbr(nb / 10);
		putchar(nb % 10 + '0');
	}
	if (nb >= 0 && nb < 10)
	{
		putchar(nb + '0');
	}
	if (nb < 0)
	{
		putchar('-');
		putnbr(-nb);
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
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Getter functions:

int 	get_rows(char *mapstr)
{
	int num;

	num = ft_atoi(mapstr);
	if (num >= 1)
		return(num);
	else
	{
		return (0);
	}
}

int 	get_cols(char *mapstr)
{
	int i;
	int cols;

	i = 0;
	cols = 0;
	while (mapstr[i] != '\n')
		i++;
	i++;
	while (mapstr[i] != '\n')
	{
		cols++;
		i++;
	}
	return (cols);
}

int 	get_size(char *mapstr)
{
	int size;

	size = 0;
	while(mapstr[size])
		size++;
	return(size);
}

char 	get_empty(char *mapstr)
{
	int i;

	i = 0;
	while (mapstr[i] >= '0' && mapstr[i] <= '9')
		i++;
	return(mapstr[i]);
}

char 	get_obstacle(char *mapstr)
{
	int i;

	i = 0;
	while (mapstr[i] >= '0' && mapstr[i] <= '9')
		i++;
	return(mapstr[i + 1]);
}

char 	get_fill(char *mapstr)
{
	int i;

	i = 0;
	while (mapstr[i] >= '0' && mapstr[i] <= '9')
		i++;
	return(mapstr[i + 2]);
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Map information:

void 	fill_map_info(mapinfo *minfo, char *mapstr)
{
	int s;

	s = get_size(mapstr);
	minfo->size = s;
	minfo->rows = get_rows(mapstr);
	minfo->cols = get_cols(mapstr);
	minfo->max = -1;
	minfo->empty_char = get_empty(mapstr);
	minfo->obstacle_char = get_obstacle(mapstr);
	minfo->fill_char = get_fill(mapstr);
	if (minfo->rows < 1)
		minfo->eflag = 1;
	else
		minfo->eflag = 0;
}
// - - - - - - -  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Test output:

void 	display_map_info(mapinfo *minfo) // can be removed in final code
{
	putnbr(minfo->size);
	putchar('\n');
	putnbr(minfo->cols);
	putchar('\n');
	putnbr(minfo->rows);
	putchar(minfo->empty_char);
	putchar(minfo->obstacle_char);
	putchar(minfo->fill_char);
	putchar('\n');
	putnbr(minfo->max);
	putchar('\n');
}

void 	display_map_array(char **map, mapinfo *minfo)
{
	int x;
	int y;

	x = 0;
	y = 0;
	while (x < minfo->rows)
	{
		y = 0;
		while (y <= minfo->cols)
		{
			putchar(map[x][y]);
			y++;
		}
		x++;
	}
	putchar('\n');
}

void 	display_int_map(int **vmap, mapinfo *minfo)
{
	int x;
	int y;

	x = 0;
	y = 0;
	while (x < minfo->rows + 2)
	{
		y = 0;
		while (y < minfo->cols + 2) // !
		{
			putnbr(vmap[x][y]);
			y++;
		}
		putchar('\n');
		x++;
	}
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// 2D character array creation and reassignment:

void 	fill_map_array(char **map, char *mapstr, mapinfo *minfo)
{
	int i;
	int x;
	int y;

	x = 0;
	i = 0;
	while(mapstr[i] != '\n')
		i++;
	i++;
	while (x < minfo->rows)
	{
		y = 0;
		while (y <= minfo->cols)
		{
			map[x][y] = mapstr[i];
			y++;
			i++;
		}
		x++;
	}
	map[minfo->rows - 1][minfo->cols] = '\n';
}

char 	**gen_map(char *mapstr, mapinfo *minfo)
{
	int  i;
	char **map;

	i = 0;
	map = (char **)malloc(minfo->rows * sizeof(char *));
	while (i < minfo->rows)
	{
		map[i] = (char *)malloc((minfo->cols + 1) * sizeof(char));
		i++;
	}
	fill_map_array(map, mapstr, minfo);
	return (map);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// 2D integer array creation:

void 	calc_index_value(int **vmap, int x, int y, mapinfo *minfo)
{
	int val;
	int min;
	int n;
	int nw;
	int w;

	n = vmap[x][y - 1];
	nw = vmap[x - 1][y - 1];
	w = vmap[x - 1][y];
	min = n;
	if (nw < min)
		min = nw;
	if (w < min)
		min = w;
	val = min + 1;
	if (val > minfo->max)
		minfo->max = val;
	vmap[x][y] = val;
}

void 	alg_fill_int_map(int **vmap, char **map, mapinfo *minfo)
{
	int x;
	int y;

	x = 1;
	y = 1;
	while (x < minfo->rows + 1)
	{
		y = 1;
		while (y < minfo->cols + 1)
		{
			if (map[x - 1][y - 1] == minfo->empty_char)
				calc_index_value(vmap, x, y, minfo);
			else
				vmap[x][y] = 0;
			y++;
		}
		x++;
	}
}

int 	**gen_int_map(char **map, mapinfo *minfo)
{
	int x;
	int y;
	int **vmap;

	x = 0;
	vmap = (int **)malloc((minfo->rows + 2) * sizeof(int *));
	while (x < minfo->rows + 2)
	{
		vmap[x] = (int *)malloc((minfo->cols + 1) * sizeof(int));
		x++;
	}
	x = 0;
	while (x < minfo->rows + 2)
	{
		y = 0;
		while (y < minfo->cols + 1)
		{
			vmap[x][y] = 0;
			y++;
		}
		x++;
	}
	alg_fill_int_map(vmap, map, minfo);
	return (vmap);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Fill first instance of largest square:

void 	fill_largest_square(char **map, mapinfo *minfo, int x, int y)
{
	int i;
	int j;

	i = x - (minfo->max - 1);
	j = y - (minfo->max - 1);

	while (i <= x)
	{
		j = y - (minfo->max - 1);
		while (j <= y)
		{
			map[i][j] = minfo->fill_char;
			j++;
		}
		i++;
	}
}

int 	map_largest_square(int **vmap, char **map, mapinfo *minfo)
{
	int x;
	int y;

	x = 1;
	y = 1;
	while (x < minfo->rows + 1)
	{
		y = 1;
		while (y < minfo->cols + 1)
		{
			if (vmap[x][y] == minfo->max)
			{
				fill_largest_square(map, minfo, (x - 1), (y - 1));
				return (0);
			}
			y++;
		}
		x++;
	}
	return (0);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Mapstring creation files:

char  	*access_input()
{
	int buff;
	char *b2;
	char buffer[BUFFSIZE + 1];

	while((buff = read(0, buffer, BUFFSIZE)))
		buffer[buff] = '\0';
	b2 = buffer;
	return (b2);
}

char	*access_file(char **argv, mapinfo *minfo)
{
	int fd;
	int buff;
	char *b2;
	char buffer[BUFFSIZE + 1];

	fd = open(argv[minfo->fnum], O_RDONLY);
	if (fd == -1)
	{
		minfo->eflag = 1;
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
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Error handling:

int 	check_cols(char **map, mapinfo *minfo) 
{
	int x;
	int y;
	int num_cols;

	x = 0;
	num_cols = 0;
	while (x < minfo->rows)
	{
		y = 0;
		num_cols = 0;
		while (map[x][y])
		{
			num_cols++;
			y++;
		}
		if (num_cols - 1 != minfo->cols)
		{
			minfo->eflag = 1;
			return (1);
		}
		x++;
	}
	return (0);
}

int 	check_lbreaks(char **map, mapinfo *minfo)
{
	int x;
	int y;

	x = 0;
	y = 0;
	while (x < minfo->rows)
	{
		y = 0;
		while (y < minfo->cols)
		{
			y++;
		}
		if (map[x][y] != '\n')
			return (1);
		x++;
	}
	return (0);
}

int 	check_chars(char **map, mapinfo *minfo)
{
	int x;
	int y;
	int empties;

	x = 0;
	empties = 0;
	while (x < minfo->rows)
	{
		y = 0;
		while (y < minfo->cols)
		{
			if (map[x][y] != minfo->empty_char 
				&& map[x][y] != minfo->obstacle_char 
				&& map[x][y] != minfo->fill_char && map[x][y] != '\n')
				return (1);
			if (map[x][y] == minfo->empty_char)
				empties++;
			y++;
		}
		x++;
	}
	if (empties == 0)
		return (1);
	else
		return (0);
}

int 	check_rows(char *mapstr, mapinfo *minfo)
{
	int newlines;
	int i;

	newlines = 0;
	i = 0;
	while(mapstr[i])
	{
		if (mapstr[i] == '\n')
			newlines++;
		i++;
	}
	if (newlines != minfo->rows)
		return (1);
	else
		return (0);
}

int 	check_validity(char **map, char *mapstr, mapinfo *minfo)
{
	if (check_cols(map, minfo) == 1)
	{
		minfo->eflag = 1;
		return (0);
	}
	if (check_chars(map, minfo) == 1)
	{
		minfo->eflag = 1;
		return (0);
	}
	if (check_lbreaks(map, minfo) == 1)
	{
		minfo->eflag = 1;
		return (0);
	}
	if (check_rows(mapstr, minfo) == 1)
	{
		minfo->eflag = 1;
		return (0);
	}
	return (0);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// split between file or input

void 	from_file(char *mapstr, char **map, char **argv, mapinfo *minfo)
{
	int **vmap;

	minfo->eflag = 0;
	mapstr = access_file(argv, minfo);
	fill_map_info(minfo, mapstr);
	if (minfo->eflag != 1)
	{
		map = gen_map(mapstr, minfo);
		check_validity(map, mapstr, minfo);
		if (minfo->eflag != 1)
		{
			vmap = gen_int_map(map, minfo);
			map_largest_square(vmap, map, minfo);
			display_map_array(map, minfo);
		}
		else
			write(2, "map error\n\n", 17);
	}
	else
		write(2, "map error\n\n", 17);
}

void 	from_input(char *mapstr, char **map, int **vmap, mapinfo *minfo)
{
	mapstr = access_input();
	fill_map_info(minfo, mapstr);
	if (minfo->eflag != 1)
	{
		map = gen_map(mapstr, minfo);
		check_validity(map, mapstr, minfo);
		if (minfo->eflag != 1)
		{
			vmap = gen_int_map(map, minfo);
			map_largest_square(vmap, map, minfo);
			display_map_array(map, minfo);
		}
		else
			write(2, "map error\n\n", 17);
	}
	else
		write(2, "map error\n\n", 17);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

int 	main(int argc, char **argv)
{
	mapinfo *minfo;
	char 	*mapstr;
	char 	**map;
	int 	**vmap;
	int 	i;

	i = 1;
	minfo = malloc(sizeof(mapinfo));
	if (argc < 2)
		from_input(mapstr, map, vmap, minfo);
	else
	{
		while (argv[i])
		{
			minfo->fnum = i;
			from_file(mapstr, map, argv, minfo);
			i++;
		}
	}
}
