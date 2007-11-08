/*
 * Mesa 3-D graphics library
 * Version:  6.5
 *
 * Copyright (C) 2005-2006  Brian Paul   All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * BRIAN PAUL BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/**
 * \file slang_compile_struct.c
 * slang front-end compiler
 * \author Michal Krol
 */

#include "imports.h"
#include "slang_compile.h"

/* slang_struct_scope */

int slang_struct_scope_construct (slang_struct_scope *scope)
{
	scope->structs = NULL;
	scope->num_structs = 0;
	scope->outer_scope = NULL;
	return 1;
}

void slang_struct_scope_destruct (slang_struct_scope *scope)
{
	unsigned int i;

	for (i = 0; i < scope->num_structs; i++)
		slang_struct_destruct (scope->structs + i);
	slang_alloc_free (scope->structs);
	/* do not free scope->outer_scope */
}

int slang_struct_scope_copy (slang_struct_scope *x, const slang_struct_scope *y)
{
	slang_struct_scope z;
	unsigned int i;

	if (!slang_struct_scope_construct (&z))
		return 0;
	z.structs = (slang_struct *) slang_alloc_malloc (y->num_structs * sizeof (slang_struct));
	if (z.structs == NULL)
	{
		slang_struct_scope_destruct (&z);
		return 0;
	}
	for (z.num_structs = 0; z.num_structs < y->num_structs; z.num_structs++)
		if (!slang_struct_construct (&z.structs[z.num_structs]))
		{
			slang_struct_scope_destruct (&z);
			return 0;
		}
	for (i = 0; i < z.num_structs; i++)
		if (!slang_struct_copy (&z.structs[i], &y->structs[i]))
		{
			slang_struct_scope_destruct (&z);
			return 0;
		}
	z.outer_scope = y->outer_scope;
	slang_struct_scope_destruct (x);
	*x = z;
	return 1;
}

slang_struct *slang_struct_scope_find (slang_struct_scope *stru, slang_atom a_name, int all_scopes)
{
	unsigned int i;

	for (i = 0; i < stru->num_structs; i++)
		if (a_name == stru->structs[i].a_name)
			return &stru->structs[i];
	if (all_scopes && stru->outer_scope != NULL)
		return slang_struct_scope_find (stru->outer_scope, a_name, 1);
	return NULL;
}

/* slang_struct */

int slang_struct_construct (slang_struct *stru)
{
	stru->a_name = SLANG_ATOM_NULL;
	stru->fields = (slang_variable_scope *) slang_alloc_malloc (sizeof (slang_variable_scope));
	if (stru->fields == NULL)
		return 0;
	if (!slang_variable_scope_construct (stru->fields))
	{
		slang_alloc_free (stru->fields);
		return 0;
	}
	stru->structs = (slang_struct_scope *) slang_alloc_malloc (sizeof (slang_struct_scope));
	if (stru->structs == NULL)
	{
		slang_variable_scope_destruct (stru->fields);
		slang_alloc_free (stru->fields);
		return 0;
	}
	if (!slang_struct_scope_construct (stru->structs))
	{
		slang_variable_scope_destruct (stru->fields);
		slang_alloc_free (stru->fields);
		slang_alloc_free (stru->structs);
		return 0;
	}
	return 1;
}

void slang_struct_destruct (slang_struct *stru)
{
	slang_variable_scope_destruct (stru->fields);
	slang_alloc_free (stru->fields);
	slang_struct_scope_destruct (stru->structs);
	slang_alloc_free (stru->structs);
}

int slang_struct_copy (slang_struct *x, const slang_struct *y)
{
	slang_struct z;

	if (!slang_struct_construct (&z))
		return 0;
	z.a_name = y->a_name;
	if (!slang_variable_scope_copy (z.fields, y->fields))
	{
		slang_struct_destruct (&z);
		return 0;
	}
	if (!slang_struct_scope_copy (z.structs, y->structs))
	{
		slang_struct_destruct (&z);
		return 0;
	}
	slang_struct_destruct (x);
	*x = z;
	return 1;
}

int slang_struct_equal (const slang_struct *x, const slang_struct *y)
{
	unsigned int i;

	if (x->fields->num_variables != y->fields->num_variables)
		return 0;
	for (i = 0; i < x->fields->num_variables; i++)
	{
		slang_variable *varx = &x->fields->variables[i];
		slang_variable *vary = &y->fields->variables[i];

		if (varx->a_name != vary->a_name)
			return 0;
		if (!slang_type_specifier_equal (&varx->type.specifier, &vary->type.specifier))
			return 0;
		if (varx->type.specifier.type == slang_spec_array)
			if (varx->array_len != vary->array_len)
				return GL_FALSE;
	}
	return 1;
}

