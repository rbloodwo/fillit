#include <unistd.h>
#include <stdlib.h>

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
// Algorithm functions:

int 	is_safe(int n, int **board, int row, int col) // only checks to the left
{
	int y;
	int x;

	y = 0;
	x = 0;
	while (x < col) // checks to the left within current row
	{
		if (board[row][x] == 1)
			return (0); // the space is not safe
		x++;
	}
	y = row;
	x = col;
	while (y >= 0 && x >= 0) // checks NW diagonal
	{
		if (board[y][x] == 1)
			return (0); // the space is not safe
		y--;
		x--;
	}
	y = row;
	x = col;
	while (y < n && x >= 0) // checks SW diagonal
	{
		if (board[y][x] == 1)
			return (0); // the space is not safe
		y++;
		x--;
	}
	return (1); // the space is safe
}

int 	solve_util(int n, int **board, int col) // recursive backtracking function
{
	int row; // y axis
	// col is passed in as parameter

	row = 0;
	if (col >= n) // all queens have been placed
		return (1); // true
	while (row < n)
	{
		if (is_safe(n, board, row, col))
		{
			board[row][col] = 1;
			if (solve_util(n, board, col + 1) == 1) // to break out of recursion loop
				return (1); // true
			board[row][col] = 0; // BACKTRACK if a solution is not found

		}
		row++;
	}
	return (0); // false
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void	print_board(int **board, int n)
{
	int y;
	int x;

	y = 0;
	while (y < n)
	{
		x = 0;
		while (x < n)
		{
			ft_putnbr(board[y][x]);
			ft_putchar(' ');
			x++;
		}
		ft_putchar('\n');
		y++;
	}
}

int 	**generate_board(int n) // n being number of queens
{
	int y;
	int x;
	int **board;

	board = (int **)malloc(n * sizeof(int *));
	y = 0;
	x = 0;
	while (x < n)
	{
		board[x] = (int *)malloc(n * sizeof(int));
		x++;
	}
	while (y < n)
	{
		x = 0;
		while (x < n)
		{
			board[y][x] = 0;
			x++;
		}
		y++;

	}
	return (board);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

int 	main(int argc, char **argv)
{
	int n;
	int **board;

	if (argc == 2)
	{
		n = ft_atoi(argv[1]);
		board = generate_board(n);
		print_board(board, n);
		ft_putstr("\n- - - - - - - - - - - - -\n\n");
		if (solve_util(n, board, 0) == 1)
			print_board(board, n);
		else
			ft_putstr("No solution found\n");

	}
	else
		ft_putstr("Invalid number of entries\n");
	return (0);
}
