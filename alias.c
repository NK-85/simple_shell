#include "shell.h"
list_t *add_node_end(list_t **head, char *dir);
void free_list(list_t *head);
alias_t *add_alias_end(alias_t **head, char *name, char *value);
void free_alias_list(alias_t *head);

/**
 * add_alias_end - Adds a node to the end of a alias_t linked list.
 * @head: A pointer to the head of the list_t list.
 * @name: The name of the new alias to be added.
 * @value: The value of the new alias to be added.
 *
 * Return: return null.
 */
alias_t *add_alias_end(alias_t **head, char *name, char *value)
{
	alias_t *last_node, *link_new = malloc(sizeof(alias_t));

	if (!link_new)
		return (NULL);

	link_new->next = NULL;
	link_new->name = malloc(sizeof(char) * (_strlen(name) + 1));
	if (!link_new->name)
	{
		free(link_new);
		return (NULL);
	}
	link_new->value = value;
	_strcpy(link_new->name, name);
	if (*head)
	{
		last_node = *head;
		while (last_node->next != NULL)
			last_node = last_node->next;
		last_node->next = link_new;
	}
	else
		*head = link_new;
	return (link_new);
}

/**
 * add_node_end - Add a new node to the end of the linked list.
 * @head: head of the linked list.
 * @dir: The path of the directory of the new node.
 *
 * Return: If Error return NULL.
 */
list_t *add_node_end(list_t **head, char *dir)
{
	list_t *link_new = malloc(sizeof(list_t));
	list_t *node_last;

	if (!link_new)
	return (NULL);
	link_new->dir = dir;
	link_new->next = NULL;
	if (*head)
	{
		node_last = *head;
		while (node_last->next != NULL)
			node_last = node_last->next;
		node_last->next = link_new;
	}
	else
	*head = link_new;
	return (link_new);
}
/**
 * free_list - This frees a linked list.
 * @head: Head of the linked list.
 */
void free_list(list_t *head)
{
	list_t *next;

	while (head)
	{
		next = head->next;
		free(head->dir);
		free(head);
		head = next;
	}
}

/**
 * free_alias_list - This frees linked list.
 * @head: The head of the list.
 */
void free_alias_list(alias_t *head)
{
	alias_t *next;

	while (head)
	{
		next = head->next;
		free(head->name);
		free(head->value);
		free(head);
		head = next;
	}
}
