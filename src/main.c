#include "incl/main.h"
#include "../libft/libft.h"
#include "incl/operations.h"
#include "incl/debug.h"
#include "incl/custom_sort.h"
#include "incl/utils.h"

#include <unistd.h>

t_bool	duplicates(int *stack, int amount_of_integers)
{
	int	i;
	int j;
	int	duplicates;

	i = 0;
	duplicates = FALSE;
	while (i < amount_of_integers && !duplicates)
	{
		j = 0;
		duplicates = -1;
		while (j < amount_of_integers)
		{
			if (stack[i] == stack[j])
				duplicates++;
			j++;
		}
		i++;
	}
	return (duplicates);
}

t_bool	parse_arguments(char **argv, t_ps_stacks *stacks)
{
	int 	i;
	long	num;

	argv++;
	while (*argv)
	{
		i = 0;
		if (*argv[0] == '-' || *argv[0] == '+')
			i++;
		while ((*argv)[i])
		{
			if (!ft_isdigit((*argv)[i]))
				return (FALSE);
			i++;
		}
		num = ft_atoi(*argv);
		if (num > 2147483647 || num < -2147483648)
			return (FALSE);
		init_num_to_stack((int)num, stacks->stack_b, stacks->amount_of_integers);
		argv++;
	}
	return (TRUE);
}

void	nums_to_indexes(t_ps_stacks *stacks)
{
	int	i;
	int	j;

	i = 0;
	while (i < stacks->amount_of_integers)
	{
		j = 0;
		stacks->stack_a[i] = 0;
		while (j < stacks->amount_of_integers)
		{
			if (stacks->stack_b[j] < stacks->stack_b[i])
				stacks->stack_a[i]++;
			j++;
		}
		i++;
	}
	ft_bzero(stacks->stack_b, stacks->amount_of_integers * sizeof(int));
}

t_bool	only_needs_rotates(int *stack, int amount_of_integers)
{
	int	i;

	i = 0;
	while (i < amount_of_integers - 1)
	{
		if (stack[i + 1] == stack[i] + 1 || (stack[i] == amount_of_integers - 1 && stack[i + 1] == 0))
			i++;
		else
			return (FALSE);
	}
	return (TRUE);
}

void	rotate_to_zero(t_ps_stacks *stacks, int amount_of_integers)
{
	int	shift;

	shift = stacks->stack_a[0];
	if (shift <= amount_of_integers / 2)
		while (shift > 0)
		{
			exec_operation(stacks, RRA);
			shift--;
		}
	else
		while (shift < amount_of_integers)
		{
			exec_operation(stacks, RA);
			shift++;
		}
}

void	sort(t_ps_stacks *stacks, int amount_of_integers)
{
	if (only_needs_rotates(stacks->stack_a, amount_of_integers))
		rotate_to_zero(stacks, amount_of_integers);
	else if (amount_of_integers < 56)
		custom_sort(stacks, amount_of_integers);
	else
		radix_sort(stacks, amount_of_integers);
}

int	print_error(void)
{
	write(1, "Error\n", 6);
	return (1);
}

int	main(int argc, char **argv)
{
	t_ps_stacks	stacks;
	int	stack_a[argc - 1];
	int	stack_b[argc - 1];

	stacks.stack_a = stack_a;
	stacks.stack_b = stack_b;
	stacks.amount_of_integers = argc - 1;
	stacks.stack_a_size = stacks.amount_of_integers;
	stacks.stack_b_size = 0;
	if (!parse_arguments(argv, &stacks) || duplicates(stacks.stack_b, stacks.amount_of_integers))
		return (print_error());
	nums_to_indexes(&stacks);
	if (is_sorted(stacks.stack_a, stacks.amount_of_integers))
		return (0);
	else
		sort(&stacks, stacks.amount_of_integers);
	return (0);
}
