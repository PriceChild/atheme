/*
 * Copyright (c) 2005 William Pitcock, et al.
 * Rights to this code are as documented in doc/LICENSE.
 *
 * Translation framework.
 *
 * $Id: culture.c 4261 2005-12-28 22:51:48Z nenolod $
 */

#include "atheme.h"

list_t transhash[HASHSIZE];

/*
 * translation_get()
 *
 * Inputs:
 *       string to get translation for
 *
 * Outputs:
 *       translated string, or the original string if no translation
 *       was found
 *
 * Side Effects:
 *       none
 */
char *translation_get(char *str)
{
	node_t *n;

	LIST_FOREACH(n, transhash[shash((unsigned char *) str)].head)
	{
		translation_t *t = (translation_t *) n->data;

		if (!strcmp(str, t->name))
			return t->translation;
	}

	return str;
}

/*
 * translation_create()
 *
 * Inputs:
 *       string to translate, translation of the string
 *
 * Outputs:
 *       none
 *
 * Side Effects:
 *       a new translation is added to the cache
 */
void translation_create(char *str, char *trans)
{
	translation_t *t = smalloc(sizeof(translation_t));

	t->name = sstrdup(str);
	t->translation = sstrdup(trans);

	node_add(t, node_create(), &transhash[shash((unsigned char *) str)]);
}

/*
 * translation_destroy()
 *
 * Inputs:
 *       string to remove from the translation cache
 *
 * Outputs:
 *       none
 *
 * Side Effects:
 *       a string is removed from the translation cache
 */
void translation_destroy(char *str)
{
	node_t *n;
	translation_t *t;

	LIST_FOREACH_SAFE(n, transhash[shash((unsigned char *) str)].head)
	{
		if (!strcmp(str, t->name))
		{
			node_del(n, &transhash[shash((unsigned char *) str)]);

			free(t->name);
			free(t->translation);
			free(t);
		}
	}
}
