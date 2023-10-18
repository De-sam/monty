#include "monty.h"

/**
 * main - Monty ByteCode Interpreter
 *
 * @argc: Number of command line arguments.
 * @argv: Array of command line arguments.
 *
 * Return: 0 upon successful execution of the program.
 */
int main(int argc, char *argv[])
{
	char *file_name = NULL, *opcode, *arg;
	FILE *file;
	stack_t *stack = NULL, *temp;
	char line[1024];
	int line_number = 0, value;

	if (argc != 2)
	{
		fprintf(stderr, "USAGE: monty file\n");
		exit(EXIT_FAILURE);
	}

	file_name = argv[1];
	file = fopen(file_name, "r");
	if (!file)
	{
		fprintf(stderr, "Error: Can't open file %s\n", file_name);
		exit(EXIT_FAILURE);
	}

	while (fgets(line, sizeof(line), file))
	{
		line_number++;

		opcode = strtok(line, " \t\n");
		if (!opcode)
		{
			continue;
		}

		if (strcmp(opcode, "push") == 0)
		{
			arg = strtok(NULL, " \t\n");
			if (!arg)
			{
				fprintf(stderr, "L%d: usage: push integer\n", line_number);
				while (stack)
				{
					temp = stack;
					stack = stack->next;
					free(temp);
				}
				fclose(file);
				exit(EXIT_FAILURE);
			}

			value = atoi(arg);
			if (value == 0 && *arg != '0')
			{
				fprintf(stderr, "L%d: usage: push integer\n", line_number);
				while (stack)
				{
					temp = stack;
					stack = stack->next;
					free(temp);
				}
				fclose(file);
				exit(EXIT_FAILURE);
			}

			push(&stack, value);
		}
		else if (strcmp(opcode, "pall") == 0)
		{
			pall(stack);
		}
		else
		{
			fprintf(stderr, "L%d: unknown instruction %s\n", line_number, opcode);
			while (stack)
			{
				temp = stack;
				stack = stack->next;
				free(temp);
			}
			fclose(file);
			exit(EXIT_FAILURE);
		}
	}

	fclose(file);

	while (stack)
	{
		temp = stack;
		stack = stack->next;
		free(temp);
	}

	return (0);
}

/**
 * push - Pushes an integer onto the stack.
 *
 * @stack: A double pointer to the top of the stack.
 * @value: The integer value to push onto the stack.
 *
 * Return: void
 */
void push(stack_t **stack, int value)
{
	stack_t *new_node;

	new_node = malloc(sizeof(stack_t));
	if (!new_node)
	{
		fprintf(stderr, "Error: malloc failed\n");
		exit(EXIT_FAILURE);
	}

	new_node->n = value;
	new_node->prev = NULL;

	if (*stack)
	{
		new_node->next = *stack;
		(*stack)->prev = new_node;
	}
	else
	{
		new_node->next = NULL;
	}

	*stack = new_node;
}

/**
 * pall - Prints all values on the stack.
 *
 * @stack: A pointer to the top of the stack.
 *
 * Return: void
 */
void pall(stack_t *stack)
{
	while (stack)
	{
		fprintf(stdout, "%d\n", stack->n);
		stack = stack->next;
	}
}
