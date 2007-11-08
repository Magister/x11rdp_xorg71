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
 * \file shaderobjects_3dlabs.c
 * shader objects definitions for 3dlabs compiler
 * \author Michal Krol
 */

/* Set this to 1 when we are ready to use 3dlabs' front-end */
#define USE_3DLABS_FRONTEND 0

#include "imports.h"
#include "hash.h"
#include "macros.h"
#include "shaderobjects.h"
#include "shaderobjects_3dlabs.h"

#if USE_3DLABS_FRONTEND
#include "slang_mesa.h"
#include "Public/ShaderLang.h"
#else
#include "slang_link.h"
#endif

struct gl2_unknown_obj
{
	GLuint reference_count;
	void (* _destructor) (struct gl2_unknown_intf **);
};

struct gl2_unknown_impl
{
	struct gl2_unknown_intf *_vftbl;
	struct gl2_unknown_obj _obj;
};

static void
_unknown_destructor (struct gl2_unknown_intf **intf)
{
}

static void
_unknown_AddRef (struct gl2_unknown_intf **intf)
{
	struct gl2_unknown_impl *impl = (struct gl2_unknown_impl *) intf;

	impl->_obj.reference_count++;
}

static void
_unknown_Release (struct gl2_unknown_intf **intf)
{
	struct gl2_unknown_impl *impl = (struct gl2_unknown_impl *) intf;

	impl->_obj.reference_count--;
	if (impl->_obj.reference_count == 0)
	{
		impl->_obj._destructor (intf);
		_mesa_free ((void *) intf);
	}
}

static struct gl2_unknown_intf **
_unknown_QueryInterface (struct gl2_unknown_intf **intf, enum gl2_uiid uiid)
{
	if (uiid == UIID_UNKNOWN)
	{
		(**intf).AddRef (intf);
		return intf;
	}
	return NULL;
}

static struct gl2_unknown_intf _unknown_vftbl = {
	_unknown_AddRef,
	_unknown_Release,
	_unknown_QueryInterface
};

static void
_unknown_constructor (struct gl2_unknown_impl *impl)
{
	impl->_vftbl = &_unknown_vftbl;
	impl->_obj.reference_count = 1;
	impl->_obj._destructor = _unknown_destructor;
}

struct gl2_unkinner_obj
{
	struct gl2_unknown_intf **unkouter;
};

struct gl2_unkinner_impl
{
	struct gl2_unknown_intf *_vftbl;
	struct gl2_unkinner_obj _obj;
};

static void
_unkinner_destructor (struct gl2_unknown_intf **intf)
{
}

static void
_unkinner_AddRef (struct gl2_unknown_intf **intf)
{
	struct gl2_unkinner_impl *impl = (struct gl2_unkinner_impl *) intf;

	(**impl->_obj.unkouter).AddRef (impl->_obj.unkouter);
}

static void
_unkinner_Release (struct gl2_unknown_intf **intf)
{
	struct gl2_unkinner_impl *impl = (struct gl2_unkinner_impl *) intf;

	(**impl->_obj.unkouter).Release (impl->_obj.unkouter);
}

static struct gl2_unknown_intf **
_unkinner_QueryInterface (struct gl2_unknown_intf **intf, enum gl2_uiid uiid)
{
	struct gl2_unkinner_impl *impl = (struct gl2_unkinner_impl *) intf;

	return (**impl->_obj.unkouter).QueryInterface (impl->_obj.unkouter, uiid);
}

static struct gl2_unknown_intf _unkinner_vftbl = {
	_unkinner_AddRef,
	_unkinner_Release,
	_unkinner_QueryInterface
};

static void
_unkinner_constructor (struct gl2_unkinner_impl *impl, struct gl2_unknown_intf **outer)
{
	impl->_vftbl = &_unkinner_vftbl;
	impl->_obj.unkouter = outer;
}

struct gl2_generic_obj
{
	struct gl2_unknown_obj _unknown;
	GLhandleARB name;
	GLboolean delete_status;
	GLcharARB *info_log;
};

struct gl2_generic_impl
{
	struct gl2_generic_intf *_vftbl;
	struct gl2_generic_obj _obj;
};

static void
_generic_destructor (struct gl2_unknown_intf **intf)
{
	GET_CURRENT_CONTEXT(ctx);
	struct gl2_generic_impl *impl = (struct gl2_generic_impl *) intf;

	_mesa_free ((void *) impl->_obj.info_log);

	_glthread_LOCK_MUTEX (ctx->Shared->Mutex);
	_mesa_HashRemove (ctx->Shared->GL2Objects, impl->_obj.name);
	_glthread_UNLOCK_MUTEX (ctx->Shared->Mutex);

	_unknown_destructor (intf);
}

static struct gl2_unknown_intf **
_generic_QueryInterface (struct gl2_unknown_intf **intf, enum gl2_uiid uiid)
{
	if (uiid == UIID_GENERIC)
	{
		(**intf).AddRef (intf);
		return intf;
	}
	return _unknown_QueryInterface (intf, uiid);
}

static void
_generic_Delete (struct gl2_generic_intf **intf)
{
	struct gl2_generic_impl *impl = (struct gl2_generic_impl *) intf;

	if (impl->_obj.delete_status == GL_FALSE)
	{
		impl->_obj.delete_status = GL_TRUE;
		(**intf)._unknown.Release ((struct gl2_unknown_intf **) intf);
	}
}

static GLhandleARB
_generic_GetName (struct gl2_generic_intf **intf)
{
	struct gl2_generic_impl *impl = (struct gl2_generic_impl *) intf;

	return impl->_obj.name;
}

static GLboolean
_generic_GetDeleteStatus (struct gl2_generic_intf **intf)
{
	struct gl2_generic_impl *impl = (struct gl2_generic_impl *) intf;

	return impl->_obj.delete_status;
}

static const GLcharARB *
_generic_GetInfoLog (struct gl2_generic_intf **intf)
{
	struct gl2_generic_impl *impl = (struct gl2_generic_impl *) intf;

	return impl->_obj.info_log;
}

static struct gl2_generic_intf _generic_vftbl = {
	{
		_unknown_AddRef,
		_unknown_Release,
		_generic_QueryInterface
	},
	_generic_Delete,
	NULL,		/* abstract GetType */
	_generic_GetName,
	_generic_GetDeleteStatus,
	_generic_GetInfoLog
};

static void
_generic_constructor (struct gl2_generic_impl *impl)
{
	GET_CURRENT_CONTEXT(ctx);

	_unknown_constructor ((struct gl2_unknown_impl *) impl);
	impl->_vftbl = &_generic_vftbl;
	impl->_obj._unknown._destructor = _generic_destructor;
	impl->_obj.delete_status = GL_FALSE;
	impl->_obj.info_log = NULL;

	_glthread_LOCK_MUTEX (ctx->Shared->Mutex);
	impl->_obj.name = _mesa_HashFindFreeKeyBlock (ctx->Shared->GL2Objects, 1);
	_mesa_HashInsert (ctx->Shared->GL2Objects, impl->_obj.name, (void *) impl);
	_glthread_UNLOCK_MUTEX (ctx->Shared->Mutex);
}

struct gl2_container_obj
{
	struct gl2_generic_obj _generic;
	struct gl2_generic_intf ***attached;
	GLuint attached_count;
};

struct gl2_container_impl
{
	struct gl2_container_intf *_vftbl;
	struct gl2_container_obj _obj;
};

static void
_container_destructor (struct gl2_unknown_intf **intf)
{
	struct gl2_container_impl *impl = (struct gl2_container_impl *) intf;
	GLuint i;

	for (i = 0; i < impl->_obj.attached_count; i++)
	{
		struct gl2_generic_intf **x = impl->_obj.attached[i];
		(**x)._unknown.Release ((struct gl2_unknown_intf **) x);
	}

	_generic_destructor (intf);
}

static struct gl2_unknown_intf **
_container_QueryInterface (struct gl2_unknown_intf **intf, enum gl2_uiid uiid)
{
	if (uiid == UIID_CONTAINER)
	{
		(**intf).AddRef (intf);
		return intf;
	}
	return _generic_QueryInterface (intf, uiid);
}

static GLboolean
_container_Attach (struct gl2_container_intf **intf, struct gl2_generic_intf **att)
{
	GET_CURRENT_CONTEXT(ctx);
	struct gl2_container_impl *impl = (struct gl2_container_impl *) intf;
	GLuint i;

	for (i = 0; i < impl->_obj.attached_count; i++)
		if (impl->_obj.attached[i] == att)
		{
			_mesa_error (ctx, GL_INVALID_OPERATION, "_container_Attach");
			return GL_FALSE;
		}

	impl->_obj.attached = (struct gl2_generic_intf ***) _mesa_realloc (impl->_obj.attached,
		impl->_obj.attached_count * sizeof (*impl->_obj.attached), (impl->_obj.attached_count + 1) *
		sizeof (*impl->_obj.attached));
	if (impl->_obj.attached == NULL)
		return GL_FALSE;

	impl->_obj.attached[impl->_obj.attached_count] = att;
	impl->_obj.attached_count++;
	(**att)._unknown.AddRef ((struct gl2_unknown_intf **) att);
	return GL_TRUE;
}

static GLboolean
_container_Detach (struct gl2_container_intf **intf, struct gl2_generic_intf **att)
{
	GET_CURRENT_CONTEXT(ctx);
	struct gl2_container_impl *impl = (struct gl2_container_impl *) intf;
	GLuint i, j;

	for (i = 0; i < impl->_obj.attached_count; i++)
		if (impl->_obj.attached[i] == att)
		{
			for (j = i; j < impl->_obj.attached_count - 1; j++)
				impl->_obj.attached[j] = impl->_obj.attached[j + 1];
			impl->_obj.attached = (struct gl2_generic_intf ***) _mesa_realloc (impl->_obj.attached,
				impl->_obj.attached_count * sizeof (*impl->_obj.attached),
				(impl->_obj.attached_count - 1) * sizeof (*impl->_obj.attached));
			impl->_obj.attached_count--;
			(**att)._unknown.Release ((struct gl2_unknown_intf **) att);
			return GL_TRUE;
		}

	_mesa_error (ctx, GL_INVALID_OPERATION, "_container_Detach");
	return GL_FALSE;
}

static GLsizei
_container_GetAttachedCount (struct gl2_container_intf **intf)
{
	struct gl2_container_impl *impl = (struct gl2_container_impl *) intf;

	return impl->_obj.attached_count;
}

static struct gl2_generic_intf **
_container_GetAttached (struct gl2_container_intf **intf, GLuint index)
{
	struct gl2_container_impl *impl = (struct gl2_container_impl *) intf;

	(**impl->_obj.attached[index])._unknown.AddRef (
		(struct gl2_unknown_intf **)impl->_obj.attached[index]);
	return impl->_obj.attached[index];
}

static struct gl2_container_intf _container_vftbl = {
	{
		{
			_unknown_AddRef,
			_unknown_Release,
			_container_QueryInterface
		},
		_generic_Delete,
		NULL,		/* abstract GetType */
		_generic_GetName,
		_generic_GetDeleteStatus,
		_generic_GetInfoLog
	},
	_container_Attach,
	_container_Detach,
	_container_GetAttachedCount,
	_container_GetAttached
};

static void
_container_constructor (struct gl2_container_impl *impl)
{
	_generic_constructor ((struct gl2_generic_impl *) impl);
	impl->_vftbl = &_container_vftbl;
	impl->_obj._generic._unknown._destructor = _container_destructor;
	impl->_obj.attached = NULL;
	impl->_obj.attached_count = 0;
}

struct gl2_3dlabs_shhandle_obj
{
	struct gl2_unkinner_obj _unknown;
#if USE_3DLABS_FRONTEND
	ShHandle handle;
#endif
};

struct gl2_3dlabs_shhandle_impl
{
	struct gl2_3dlabs_shhandle_intf *_vftbl;
	struct gl2_3dlabs_shhandle_obj _obj;
};

static void
_3dlabs_shhandle_destructor (struct gl2_unknown_intf **intf)
{
#if USE_3DLABS_FRONTEND
	struct gl2_3dlabs_shhandle_impl *impl = (struct gl2_3dlabs_shhandle_impl *) intf;
	ShDestruct (impl->_obj.handle);
#endif
	_unkinner_destructor (intf);
}

static GLvoid *
_3dlabs_shhandle_GetShHandle (struct gl2_3dlabs_shhandle_intf **intf)
{
#if USE_3DLABS_FRONTEND
	struct gl2_3dlabs_shhandle_impl *impl = (struct gl2_3dlabs_shhandle_impl *) intf;
	return impl->_obj.handle;
#else
	return NULL;
#endif
}

static struct gl2_3dlabs_shhandle_intf _3dlabs_shhandle_vftbl = {
	{
		_unkinner_AddRef,
		_unkinner_Release,
		_unkinner_QueryInterface
	},
	_3dlabs_shhandle_GetShHandle
};

static void
_3dlabs_shhandle_constructor (struct gl2_3dlabs_shhandle_impl *impl, struct gl2_unknown_intf **outer)
{
	_unkinner_constructor ((struct gl2_unkinner_impl *) impl, outer);
	impl->_vftbl = &_3dlabs_shhandle_vftbl;
#if USE_3DLABS_FRONTEND
	impl->_obj.handle = NULL;
#endif
}

struct gl2_shader_obj
{
	struct gl2_generic_obj _generic;
	struct gl2_3dlabs_shhandle_impl _3dlabs_shhandle;
	GLboolean compile_status;
	GLcharARB *source;
	GLint *offsets;
	GLsizei offset_count;
	slang_translation_unit unit;
};

struct gl2_shader_impl
{
	struct gl2_shader_intf *_vftbl;
	struct gl2_shader_obj _obj;
};

static void
_shader_destructor (struct gl2_unknown_intf **intf)
{
	struct gl2_shader_impl *impl = (struct gl2_shader_impl *) intf;

	_mesa_free ((void *) impl->_obj.source);
	_mesa_free ((void *) impl->_obj.offsets);
	_3dlabs_shhandle_destructor ((struct gl2_unknown_intf **) &impl->_obj._3dlabs_shhandle._vftbl);
	_generic_destructor (intf);
}

static struct gl2_unknown_intf **
_shader_QueryInterface (struct gl2_unknown_intf **intf, enum gl2_uiid uiid)
{
#if USE_3DLABS_FRONTEND
	struct gl2_shader_impl *impl = (struct gl2_shader_impl *) intf;
#endif

	if (uiid == UIID_SHADER)
	{
		(**intf).AddRef (intf);
		return intf;
	}
#if USE_3DLABS_FRONTEND
	if (uiid == UIID_3DLABS_SHHANDLE)
	{
		(**intf).AddRef (intf);
		return (struct gl2_unknown_intf **) &impl->_obj._3dlabs_shhandle._vftbl;
	}
#endif
	return _generic_QueryInterface (intf, uiid);
}

static GLenum
_shader_GetType (struct gl2_generic_intf **intf)
{
	return GL_SHADER_OBJECT_ARB;
}

static GLboolean
_shader_GetCompileStatus (struct gl2_shader_intf **intf)
{
	struct gl2_shader_impl *impl = (struct gl2_shader_impl *) intf;

	return impl->_obj.compile_status;
}

static GLvoid
_shader_SetSource (struct gl2_shader_intf **intf, GLcharARB *src, GLint *off, GLsizei cnt)
{
	struct gl2_shader_impl *impl = (struct gl2_shader_impl *) intf;

	_mesa_free ((void *) impl->_obj.source);
	impl->_obj.source = src;
	_mesa_free ((void *) impl->_obj.offsets);
	impl->_obj.offsets = off;
	impl->_obj.offset_count = cnt;
}

static const GLcharARB *
_shader_GetSource (struct gl2_shader_intf **intf)
{
	struct gl2_shader_impl *impl = (struct gl2_shader_impl *) intf;

	return impl->_obj.source;
}

static GLvoid
_shader_Compile (struct gl2_shader_intf **intf)
{
	struct gl2_shader_impl *impl = (struct gl2_shader_impl *) intf;
#if USE_3DLABS_FRONTEND
	char **strings;
	TBuiltInResource res;
#else
	slang_unit_type type;
	slang_info_log info_log;
#endif

	impl->_obj.compile_status = GL_FALSE;
	_mesa_free ((void *) impl->_obj._generic.info_log);
	impl->_obj._generic.info_log = NULL;

#if USE_3DLABS_FRONTEND
	/* 3dlabs compiler expects us to feed it with null-terminated string array,
	we've got only one big string with offsets, so we must split it; but when
	there's only one string to deal with, we pass its address directly */

	if (impl->_obj.offset_count <= 1)
		strings = &impl->_obj.source;
	else
	{
		GLsizei i, offset = 0;

		strings = (char **) _mesa_malloc (impl->_obj.offset_count * sizeof (char *));
		if (strings == NULL)
			return;

		for (i = 0; i < impl->_obj.offset_count; i++)
		{
			GLsizei size = impl->_obj.offsets[i] - offset;

			strings[i] = (char *) _mesa_malloc ((size + 1) * sizeof (char));
			if (strings[i] == NULL)
			{
				GLsizei j;

				for (j = 0; j < i; j++)
					_mesa_free (strings[j]);
				_mesa_free (strings);
				return;
			}

			_mesa_memcpy (strings[i], impl->_obj.source + offset, size * sizeof (char));
			strings[i][size] = '\0';
			offset = impl->_obj.offsets[i];
		}
	}

	/* TODO set these fields to some REAL numbers */
	res.maxLights = 8;
	res.maxClipPlanes = 6;
	res.maxTextureUnits = 2;
	res.maxTextureCoords = 2;
	res.maxVertexAttribs = 8;
	res.maxVertexUniformComponents = 64;
	res.maxVaryingFloats = 8;
	res.maxVertexTextureImageUnits = 2;
	res.maxCombinedTextureImageUnits = 2;
	res.maxTextureImageUnits = 2;
	res.maxFragmentUniformComponents = 64;
	res.maxDrawBuffers = 1;

	if (ShCompile (impl->_obj._3dlabs_shhandle._obj.handle, strings, impl->_obj.offset_count,
			EShOptFull, &res, 0))
		impl->_obj.compile_status = GL_TRUE;
	if (impl->_obj.offset_count > 1)
	{
		GLsizei i;

		for (i = 0; i < impl->_obj.offset_count; i++)
			_mesa_free (strings[i]);
		_mesa_free (strings);
	}

	impl->_obj._generic.info_log = _mesa_strdup (ShGetInfoLog (
		impl->_obj._3dlabs_shhandle._obj.handle));
#else
	if (impl->_vftbl->GetSubType (intf) == GL_FRAGMENT_SHADER)
		type = slang_unit_fragment_shader;
	else
		type = slang_unit_vertex_shader;
	slang_info_log_construct (&info_log);
	if (_slang_compile (impl->_obj.source, &impl->_obj.unit, type, &info_log))
	{
		impl->_obj.compile_status = GL_TRUE;
	}
	if (info_log.text != NULL)
		impl->_obj._generic.info_log = _mesa_strdup (info_log.text);
	else
		impl->_obj._generic.info_log = _mesa_strdup ("");
	slang_info_log_destruct (&info_log);
#endif
}

static struct gl2_shader_intf _shader_vftbl = {
	{
		{
			_unknown_AddRef,
			_unknown_Release,
			_shader_QueryInterface
		},
		_generic_Delete,
		_shader_GetType,
		_generic_GetName,
		_generic_GetDeleteStatus,
		_generic_GetInfoLog
	},
	NULL,		/* abstract GetSubType */
	_shader_GetCompileStatus,
	_shader_SetSource,
	_shader_GetSource,
	_shader_Compile
};

static void
_shader_constructor (struct gl2_shader_impl *impl)
{
	_generic_constructor ((struct gl2_generic_impl *) impl);
	_3dlabs_shhandle_constructor (&impl->_obj._3dlabs_shhandle, (struct gl2_unknown_intf **)
		&impl->_vftbl);
	impl->_vftbl = &_shader_vftbl;
	impl->_obj._generic._unknown._destructor = _shader_destructor;
	impl->_obj.compile_status = GL_FALSE;
	impl->_obj.source = NULL;
	impl->_obj.offsets = NULL;
	impl->_obj.offset_count = 0;
}

struct gl2_program_obj
{
	struct gl2_container_obj _container;
	GLboolean link_status;
	GLboolean validate_status;
#if USE_3DLABS_FRONTEND
	ShHandle linker;
	ShHandle uniforms;
#endif
	slang_program prog;
};

struct gl2_program_impl
{
	struct gl2_program_intf *_vftbl;
	struct gl2_program_obj _obj;
};

static void
_program_destructor (struct gl2_unknown_intf **intf)
{
	struct gl2_program_impl *impl = (struct gl2_program_impl *) intf;
#if USE_3DLABS_FRONTEND
	ShDestruct (impl->_obj.linker);
	ShDestruct (impl->_obj.uniforms);
#endif
	_container_destructor (intf);
	slang_program_dtr (&impl->_obj.prog);
}

static struct gl2_unknown_intf **
_program_QueryInterface (struct gl2_unknown_intf **intf, enum gl2_uiid uiid)
{
	if (uiid == UIID_PROGRAM)
	{
		(**intf).AddRef (intf);
		return intf;
	}
	return _container_QueryInterface (intf, uiid);
}

static GLenum
_program_GetType (struct gl2_generic_intf **intf)
{
	return GL_PROGRAM_OBJECT_ARB;
}

static GLboolean
_program_Attach (struct gl2_container_intf **intf, struct gl2_generic_intf **att)
{
	GET_CURRENT_CONTEXT(ctx);
	struct gl2_unknown_intf **sha;

	sha = (**att)._unknown.QueryInterface ((struct gl2_unknown_intf **) att, UIID_SHADER);
	if (sha == NULL)
	{
		_mesa_error (ctx, GL_INVALID_OPERATION, "_program_Attach");
		return GL_FALSE;
	}

	(**sha).Release (sha);
	return _container_Attach (intf, att);
}

static GLboolean
_program_GetLinkStatus (struct gl2_program_intf **intf)
{
	struct gl2_program_impl *impl = (struct gl2_program_impl *) intf;

	return impl->_obj.link_status;
}

static GLboolean
_program_GetValidateStatus (struct gl2_program_intf **intf)
{
	struct gl2_program_impl *impl = (struct gl2_program_impl *) intf;

	return impl->_obj.validate_status;
}

static GLvoid
_program_Link (struct gl2_program_intf **intf)
{
	struct gl2_program_impl *impl = (struct gl2_program_impl *) intf;
#if USE_3DLABS_FRONTEND
	ShHandle *handles;
#endif
	GLuint i, count;
	slang_translation_unit *units[2];

	impl->_obj.link_status = GL_FALSE;
	_mesa_free ((void *) impl->_obj._container._generic.info_log);
	impl->_obj._container._generic.info_log = NULL;
	slang_program_dtr (&impl->_obj.prog);
	slang_program_ctr (&impl->_obj.prog);

#if USE_3DLABS_FRONTEND
	handles = (ShHandle *) _mesa_malloc (impl->_obj._container.attached_count * sizeof (ShHandle));
	if (handles == NULL)
		return;

	for (i = 0; i < impl->_obj._container.attached_count; i++)
	{
		struct gl2_generic_intf **gen = impl->_obj._container.attached[i];
		struct gl2_3dlabs_shhandle_intf **sh;

		sh = (struct gl2_3dlabs_shhandle_intf **) (**gen)._unknown.QueryInterface (
			(struct gl2_unknown_intf **) gen, UIID_3DLABS_SHHANDLE);
		if (sh != NULL)
		{
			handles[i] = (**sh).GetShHandle (sh);
			(**sh)._unknown.Release ((struct gl2_unknown_intf **) sh);
		}
		else
		{
			_mesa_free (handles);
			return;
		}
	}

	if (ShLink (impl->_obj.linker, handles, impl->_obj._container.attached_count,
			impl->_obj.uniforms, NULL, NULL))
		impl->_obj.link_status = GL_TRUE;

	impl->_obj._container._generic.info_log = _mesa_strdup (ShGetInfoLog (impl->_obj.linker));
#else
	count = impl->_obj._container.attached_count;
	if (count == 0 || count > 2)
		return;
	for (i = 0; i < count; i++)
	{
		struct gl2_generic_intf **obj;
		struct gl2_unknown_intf **unk;
		struct gl2_shader_impl *sha;

		obj = impl->_obj._container.attached[i];
		unk = (**obj)._unknown.QueryInterface ((struct gl2_unknown_intf **) obj, UIID_SHADER);
		(**obj)._unknown.Release ((struct gl2_unknown_intf **) obj);
		if (unk == NULL)
			return;
		sha = (struct gl2_shader_impl *) unk;
		units[i] = &sha->_obj.unit;
		(**unk).Release (unk);
	}

	impl->_obj.link_status = _slang_link (&impl->_obj.prog, units, count);
	if (impl->_obj.link_status)
		impl->_obj._container._generic.info_log = _mesa_strdup ("Link OK.\n");
	else
		impl->_obj._container._generic.info_log = _mesa_strdup ("Link failed.\n");
#endif
}

static GLvoid
_program_Validate (struct gl2_program_intf **intf)
{
	struct gl2_program_impl *impl = (struct gl2_program_impl *) intf;

	impl->_obj.validate_status = GL_FALSE;
	_mesa_free ((void *) impl->_obj._container._generic.info_log);
	impl->_obj._container._generic.info_log = NULL;

	/* TODO validate */
}

static GLvoid
write_common_fixed (slang_program *pro, GLuint index, const GLvoid *src, GLuint off, GLuint size)
{
	GLuint i;

	for (i = 0; i < SLANG_SHADER_MAX; i++)
	{
		GLuint addr;

		addr = pro->common_fixed_entries[i][index];
		if (addr != ~0)
		{
			GLubyte *dst;

			dst = (GLubyte *) pro->machines[i]->mem + addr + off * size;
			_mesa_memcpy (dst, src, size);
		}
	}
}

static GLvoid
write_common_fixed_mat4 (slang_program *pro, GLmatrix *matrix, GLuint off, GLuint i, GLuint ii,
                         GLuint it, GLuint iit)
{
	GLfloat mat[16];

	/* we want inverse matrix */
	if (!matrix->inv)
	{
		/* allocate inverse matrix and make it dirty */
		_math_matrix_alloc_inv (matrix);
		_math_matrix_loadf (matrix, matrix->m);
	}
	_math_matrix_analyse (matrix);

	write_common_fixed (pro, i, matrix->m, off, 16 * sizeof (GLfloat));

	/* inverse */
	write_common_fixed (pro, ii, matrix->inv, off, 16 * sizeof (GLfloat));

	/* transpose */
	_math_transposef (mat, matrix->m);
	write_common_fixed (pro, it, mat, off, 16 * sizeof (GLfloat));

	/* inverse transpose */
	_math_transposef (mat, matrix->inv);
	write_common_fixed (pro, iit, mat, off, 16 * sizeof (GLfloat));
}

static GLvoid
write_common_fixed_material (GLcontext *ctx, slang_program *pro, GLuint i, GLuint e, GLuint a,
                             GLuint d, GLuint sp, GLuint sh)
{
	GLfloat v[17];

	COPY_4FV(v, ctx->Light.Material.Attrib[e]);
	COPY_4FV((v + 4), ctx->Light.Material.Attrib[a]);
	COPY_4FV((v + 8), ctx->Light.Material.Attrib[d]);
	COPY_4FV((v + 12), ctx->Light.Material.Attrib[sp]);
	v[16] = ctx->Light.Material.Attrib[sh][0];
	write_common_fixed (pro, i, v, 0, 17 * sizeof (GLfloat));
}

static GLvoid
write_common_fixed_light_model_product (GLcontext *ctx, slang_program *pro, GLuint i, GLuint e,
                                        GLuint a)
{
	GLfloat v[4];

	SCALE_4V(v,	ctx->Light.Material.Attrib[a], ctx->Light.Model.Ambient);
	ACC_4V(v, ctx->Light.Material.Attrib[e]);
	write_common_fixed (pro, i, v, 0, 4 * sizeof (GLfloat));
}

static GLvoid
write_common_fixed_light_product (GLcontext *ctx, slang_program *pro, GLuint off, GLuint i, GLuint a,
                                  GLuint d, GLuint s)
{
	GLfloat v[12];

	SCALE_4V(v, ctx->Light.Light[off].Ambient, ctx->Light.Material.Attrib[a]);
	SCALE_4V((v + 4), ctx->Light.Light[off].Diffuse, ctx->Light.Material.Attrib[d]);
	SCALE_4V((v + 8), ctx->Light.Light[off].Specular, ctx->Light.Material.Attrib[s]);
	write_common_fixed (pro, i, v, off, 12 * sizeof (GLfloat));
}

static GLvoid
_program_UpdateFixedUniforms (struct gl2_program_intf **intf)
{
	GET_CURRENT_CONTEXT(ctx);
	struct gl2_program_impl *impl = (struct gl2_program_impl *) intf;
	slang_program *pro = &impl->_obj.prog;
	GLuint i;
	GLfloat v[29];
	GLfloat *p;

	/* MODELVIEW matrix */
	write_common_fixed_mat4 (pro, ctx->ModelviewMatrixStack.Top, 0,
		SLANG_COMMON_FIXED_MODELVIEWMATRIX,
		SLANG_COMMON_FIXED_MODELVIEWMATRIXINVERSE,
		SLANG_COMMON_FIXED_MODELVIEWMATRIXTRANSPOSE,
		SLANG_COMMON_FIXED_MODELVIEWMATRIXINVERSETRANSPOSE);

	/* PROJECTION matrix */
	write_common_fixed_mat4 (pro, ctx->ProjectionMatrixStack.Top, 0,
		SLANG_COMMON_FIXED_PROJECTIONMATRIX,
		SLANG_COMMON_FIXED_PROJECTIONMATRIXINVERSE,
		SLANG_COMMON_FIXED_PROJECTIONMATRIXTRANSPOSE,
		SLANG_COMMON_FIXED_PROJECTIONMATRIXINVERSETRANSPOSE);

	/* MVP matrix */
	write_common_fixed_mat4 (pro, &ctx->_ModelProjectMatrix, 0,
		SLANG_COMMON_FIXED_MODELVIEWPROJECTIONMATRIX,
		SLANG_COMMON_FIXED_MODELVIEWPROJECTIONMATRIXINVERSE,
		SLANG_COMMON_FIXED_MODELVIEWPROJECTIONMATRIXTRANSPOSE,
		SLANG_COMMON_FIXED_MODELVIEWPROJECTIONMATRIXINVERSETRANSPOSE);

	for (i = 0; i < ctx->Const.MaxTextureCoordUnits; i++)
	{
		/* TEXTURE matrix */
		write_common_fixed_mat4 (pro, ctx->TextureMatrixStack[i].Top, i,
			SLANG_COMMON_FIXED_TEXTUREMATRIX,
			SLANG_COMMON_FIXED_TEXTUREMATRIXINVERSE,
			SLANG_COMMON_FIXED_TEXTUREMATRIXTRANSPOSE,
			SLANG_COMMON_FIXED_TEXTUREMATRIXINVERSETRANSPOSE);

		/* EYE_PLANE texture-coordinate generation */
		write_common_fixed (pro, SLANG_COMMON_FIXED_EYEPLANES, ctx->Texture.Unit[i].EyePlaneS,
			i, 4 * sizeof (GLfloat));
		write_common_fixed (pro, SLANG_COMMON_FIXED_EYEPLANET, ctx->Texture.Unit[i].EyePlaneT,
			i, 4 * sizeof (GLfloat));
		write_common_fixed (pro, SLANG_COMMON_FIXED_EYEPLANER, ctx->Texture.Unit[i].EyePlaneR,
			i, 4 * sizeof (GLfloat));
		write_common_fixed (pro, SLANG_COMMON_FIXED_EYEPLANEQ, ctx->Texture.Unit[i].EyePlaneQ,
			i, 4 * sizeof (GLfloat));

		/* OBJECT_PLANE texture-coordinate generation */
		write_common_fixed (pro, SLANG_COMMON_FIXED_OBJECTPLANES, ctx->Texture.Unit[i].ObjectPlaneS,
			i, 4 * sizeof (GLfloat));
		write_common_fixed (pro, SLANG_COMMON_FIXED_OBJECTPLANET, ctx->Texture.Unit[i].ObjectPlaneT,
			i, 4 * sizeof (GLfloat));
		write_common_fixed (pro, SLANG_COMMON_FIXED_OBJECTPLANER, ctx->Texture.Unit[i].ObjectPlaneR,
			i, 4 * sizeof (GLfloat));
		write_common_fixed (pro, SLANG_COMMON_FIXED_OBJECTPLANEQ, ctx->Texture.Unit[i].ObjectPlaneQ,
			i, 4 * sizeof (GLfloat));
	}

	/* NORMAL matrix - upper 3x3 inverse transpose of MODELVIEW matrix */
	p = ctx->ModelviewMatrixStack.Top->inv;
	v[0] = p[0];
	v[1] = p[4];
	v[2] = p[8];
	v[3] = p[1];
	v[4] = p[5];
	v[5] = p[9];
	v[6] = p[2];
	v[7] = p[6];
	v[8] = p[10];
	write_common_fixed (pro, SLANG_COMMON_FIXED_NORMALMATRIX, v, 0, 9 * sizeof (GLfloat));

	/* normal scale */
	write_common_fixed (pro, SLANG_COMMON_FIXED_NORMALSCALE, &ctx->_ModelViewInvScale, 0, sizeof (GLfloat));

	/* depth range parameters */
	v[0] = ctx->Viewport.Near;
	v[1] = ctx->Viewport.Far;
	v[2] = ctx->Viewport.Far - ctx->Viewport.Near;
	write_common_fixed (pro, SLANG_COMMON_FIXED_DEPTHRANGE, v, 0, 3 * sizeof (GLfloat));

	/* CLIP_PLANEi */
	for (i = 0; i < ctx->Const.MaxClipPlanes; i++)
	{
		write_common_fixed (pro, SLANG_COMMON_FIXED_CLIPPLANE, ctx->Transform.EyeUserPlane[i], i,
			4 * sizeof (GLfloat));
	}

	/* point parameters */
	v[0] = ctx->Point.Size;
	v[1] = ctx->Point.MinSize;
	v[2] = ctx->Point.MaxSize;
	v[3] = ctx->Point.Threshold;
	COPY_3FV((v + 4), ctx->Point.Params);
	write_common_fixed (pro, SLANG_COMMON_FIXED_POINT, v, 0, 7 * sizeof (GLfloat));

	/* material parameters */
	write_common_fixed_material (ctx, pro, SLANG_COMMON_FIXED_FRONTMATERIAL,
		MAT_ATTRIB_FRONT_EMISSION,
		MAT_ATTRIB_FRONT_AMBIENT,
		MAT_ATTRIB_FRONT_DIFFUSE,
		MAT_ATTRIB_FRONT_SPECULAR,
		MAT_ATTRIB_FRONT_SHININESS);
	write_common_fixed_material (ctx, pro, SLANG_COMMON_FIXED_BACKMATERIAL,
		MAT_ATTRIB_BACK_EMISSION,
		MAT_ATTRIB_BACK_AMBIENT,
		MAT_ATTRIB_BACK_DIFFUSE,
		MAT_ATTRIB_BACK_SPECULAR,
		MAT_ATTRIB_BACK_SHININESS);

	for (i = 0; i < ctx->Const.MaxLights; i++)
	{
		/* light source parameters */
		COPY_4FV(v, ctx->Light.Light[i].Ambient);
		COPY_4FV((v + 4), ctx->Light.Light[i].Diffuse);
		COPY_4FV((v + 8), ctx->Light.Light[i].Specular);
		COPY_4FV((v + 12), ctx->Light.Light[i].EyePosition);
		COPY_2FV((v + 16), ctx->Light.Light[i].EyePosition);
		v[18] = ctx->Light.Light[i].EyePosition[2] + 1.0f;
		NORMALIZE_3FV((v + 16));
		v[19] = 0.0f;
		COPY_3V((v + 20), ctx->Light.Light[i].EyeDirection);
		v[23] = ctx->Light.Light[i].SpotExponent;
		v[24] = ctx->Light.Light[i].SpotCutoff;
		v[25] = ctx->Light.Light[i]._CosCutoffNeg;
		v[26] = ctx->Light.Light[i].ConstantAttenuation;
		v[27] = ctx->Light.Light[i].LinearAttenuation;
		v[28] = ctx->Light.Light[i].QuadraticAttenuation;
		write_common_fixed (pro, SLANG_COMMON_FIXED_LIGHTSOURCE, v, i, 29 * sizeof (GLfloat));

		/* light product */
		write_common_fixed_light_product (ctx, pro, i, SLANG_COMMON_FIXED_FRONTLIGHTPRODUCT,
			MAT_ATTRIB_FRONT_AMBIENT,
			MAT_ATTRIB_FRONT_DIFFUSE,
			MAT_ATTRIB_FRONT_SPECULAR);
		write_common_fixed_light_product (ctx, pro, i, SLANG_COMMON_FIXED_BACKLIGHTPRODUCT,
			MAT_ATTRIB_BACK_AMBIENT,
			MAT_ATTRIB_BACK_DIFFUSE,
			MAT_ATTRIB_BACK_SPECULAR);
	}

	/* light model parameters */
	write_common_fixed (pro, SLANG_COMMON_FIXED_LIGHTMODEL, ctx->Light.Model.Ambient, 0, 4 * sizeof (GLfloat));

	/* light model product */
	write_common_fixed_light_model_product (ctx, pro, SLANG_COMMON_FIXED_FRONTLIGHTMODELPRODUCT,
		MAT_ATTRIB_FRONT_EMISSION,
		MAT_ATTRIB_FRONT_AMBIENT);
	write_common_fixed_light_model_product (ctx, pro, SLANG_COMMON_FIXED_BACKLIGHTMODELPRODUCT,
		MAT_ATTRIB_BACK_EMISSION,
		MAT_ATTRIB_BACK_AMBIENT);

	/* TEXTURE_ENV_COLOR */
	for (i = 0; i < ctx->Const.MaxTextureImageUnits; i++)
	{
		write_common_fixed (pro, SLANG_COMMON_FIXED_TEXTUREENVCOLOR, ctx->Texture.Unit[i].EnvColor,
			i, 4 * sizeof (GLfloat));
	}

	/* fog parameters */
	COPY_4FV(v, ctx->Fog.Color);
	v[4] = ctx->Fog.Density;
	v[5] = ctx->Fog.Start;
	v[6] = ctx->Fog.End;
	v[7] = ctx->Fog._Scale;
	write_common_fixed (pro, SLANG_COMMON_FIXED_FOG, v, 0, 8 * sizeof (GLfloat));
}

static GLvoid
_program_UpdateFixedAttribute (struct gl2_program_intf **intf, GLuint index, GLvoid *data,
							  GLuint offset, GLuint size, GLboolean write)
{
	struct gl2_program_impl *impl = (struct gl2_program_impl *) intf;
	slang_program *pro = &impl->_obj.prog;
	GLuint addr;

	addr = pro->vertex_fixed_entries[index];
	if (addr != ~0)
	{
		GLubyte *mem;

		mem = (GLubyte *) pro->machines[SLANG_SHADER_VERTEX]->mem + addr + offset * size;
		if (write)
			_mesa_memcpy (mem, data, size);
		else
			_mesa_memcpy (data, mem, size);
	}
}

static GLvoid
_program_UpdateFixedVarying (struct gl2_program_intf **intf, GLuint index, GLvoid *data,
							GLuint offset, GLuint size, GLboolean write)
{
	struct gl2_program_impl *impl = (struct gl2_program_impl *) intf;
	slang_program *pro = &impl->_obj.prog;
	GLuint addr;

	addr = pro->fragment_fixed_entries[index];
	if (addr != ~0)
	{
		GLubyte *mem;

		mem = (GLubyte *) pro->machines[SLANG_SHADER_FRAGMENT]->mem + addr + offset * size;
		if (write)
			_mesa_memcpy (mem, data, size);
		else
			_mesa_memcpy (data, mem, size);
	}
}

static GLvoid
_program_GetTextureImageUsage (struct gl2_program_intf **intf, GLbitfield *teximageusage)
{
	GET_CURRENT_CONTEXT(ctx);
	struct gl2_program_impl *impl = (struct gl2_program_impl *) intf;
	slang_program *pro = &impl->_obj.prog;
	GLuint i;

	for (i = 0; i < ctx->Const.MaxTextureImageUnits; i++)
		teximageusage[i] = 0;

	for (i = 0; i < pro->texture_usage.count; i++)
	{
		GLuint n, addr, j;

		n = pro->texture_usage.table[i].quant->array_len;
		if (n == 0)
			n = 1;
		addr = pro->texture_usage.table[i].frag_address;
		for (j = 0; j < n; j++)
		{
			GLubyte *mem;
			GLuint image;

			mem = (GLubyte *) pro->machines[SLANG_SHADER_FRAGMENT]->mem + addr + j * 4;
			image = (GLuint) *((GLfloat *) mem);
			if (image >= 0 && image < ctx->Const.MaxTextureImageUnits)
			{
				switch (pro->texture_usage.table[i].quant->u.basic_type)
				{
				case GL_SAMPLER_1D_ARB:
				case GL_SAMPLER_1D_SHADOW_ARB:
					teximageusage[image] |= TEXTURE_1D_BIT;
					break;
				case GL_SAMPLER_2D_ARB:
				case GL_SAMPLER_2D_SHADOW_ARB:
					teximageusage[image] |= TEXTURE_2D_BIT;
					break;
				case GL_SAMPLER_3D_ARB:
					teximageusage[image] |= TEXTURE_3D_BIT;
					break;
				case GL_SAMPLER_CUBE_ARB:
					teximageusage[image] |= TEXTURE_CUBE_BIT;
					break;
				}
			}
		}
	}

	/* TODO: make sure that for 0<=i<=MaxTextureImageUint bitcount(teximageuint[i])<=0 */
}

static GLboolean
_program_IsShaderPresent (struct gl2_program_intf **intf, GLenum subtype)
{
	struct gl2_program_impl *impl = (struct gl2_program_impl *) intf;
	slang_program *pro = &impl->_obj.prog;

	switch (subtype)
	{
	case GL_VERTEX_SHADER_ARB:
		return pro->machines[SLANG_SHADER_VERTEX] != NULL;
	case GL_FRAGMENT_SHADER_ARB:
		return pro->machines[SLANG_SHADER_FRAGMENT] != NULL;
	default:
		return GL_FALSE;
	}
}

static struct gl2_program_intf _program_vftbl = {
	{
		{
			{
				_unknown_AddRef,
				_unknown_Release,
				_program_QueryInterface
			},
			_generic_Delete,
			_program_GetType,
			_generic_GetName,
			_generic_GetDeleteStatus,
			_generic_GetInfoLog
		},
		_program_Attach,
		_container_Detach,
		_container_GetAttachedCount,
		_container_GetAttached
	},
	_program_GetLinkStatus,
	_program_GetValidateStatus,
	_program_Link,
	_program_Validate,
	_program_UpdateFixedUniforms,
	_program_UpdateFixedAttribute,
	_program_UpdateFixedVarying,
	_program_GetTextureImageUsage,
	_program_IsShaderPresent
};

static void
_program_constructor (struct gl2_program_impl *impl)
{
	_container_constructor ((struct gl2_container_impl *) impl);
	impl->_vftbl = &_program_vftbl;
	impl->_obj._container._generic._unknown._destructor = _program_destructor;
	impl->_obj.link_status = GL_FALSE;
	impl->_obj.validate_status = GL_FALSE;
#if USE_3DLABS_FRONTEND
	impl->_obj.linker = ShConstructLinker (EShExVertexFragment, 0);
	impl->_obj.uniforms = ShConstructUniformMap ();
#endif
	slang_program_ctr (&impl->_obj.prog);
}

struct gl2_fragment_shader_obj
{
	struct gl2_shader_obj _shader;
};

struct gl2_fragment_shader_impl
{
	struct gl2_fragment_shader_intf *_vftbl;
	struct gl2_fragment_shader_obj _obj;
};

static void
_fragment_shader_destructor (struct gl2_unknown_intf **intf)
{
	struct gl2_fragment_shader_impl *impl = (struct gl2_fragment_shader_impl *) intf;

	(void) impl;
	/* TODO free fragment shader data */

	_shader_destructor (intf);
}

static struct gl2_unknown_intf **
_fragment_shader_QueryInterface (struct gl2_unknown_intf **intf, enum gl2_uiid uiid)
{
	if (uiid == UIID_FRAGMENT_SHADER)
	{
		(**intf).AddRef (intf);
		return intf;
	}
	return _shader_QueryInterface (intf, uiid);
}

static GLenum
_fragment_shader_GetSubType (struct gl2_shader_intf **intf)
{
	return GL_FRAGMENT_SHADER_ARB;
}

static struct gl2_fragment_shader_intf _fragment_shader_vftbl = {
	{
		{
			{
				_unknown_AddRef,
				_unknown_Release,
				_fragment_shader_QueryInterface
			},
			_generic_Delete,
			_shader_GetType,
			_generic_GetName,
			_generic_GetDeleteStatus,
			_generic_GetInfoLog
		},
		_fragment_shader_GetSubType,
		_shader_GetCompileStatus,
		_shader_SetSource,
		_shader_GetSource,
		_shader_Compile
	}
};

static void
_fragment_shader_constructor (struct gl2_fragment_shader_impl *impl)
{
	_shader_constructor ((struct gl2_shader_impl *) impl);
	impl->_vftbl = &_fragment_shader_vftbl;
	impl->_obj._shader._generic._unknown._destructor = _fragment_shader_destructor;
#if USE_3DLABS_FRONTEND
	impl->_obj._shader._3dlabs_shhandle._obj.handle = ShConstructCompiler (EShLangFragment, 0);
#endif
}

struct gl2_vertex_shader_obj
{
	struct gl2_shader_obj _shader;
};

struct gl2_vertex_shader_impl
{
	struct gl2_vertex_shader_intf *_vftbl;
	struct gl2_vertex_shader_obj _obj;
};

static void
_vertex_shader_destructor (struct gl2_unknown_intf **intf)
{
	struct gl2_vertex_shader_impl *impl = (struct gl2_vertex_shader_impl *) intf;

	(void) impl;
	/* TODO free vertex shader data */

	_shader_destructor (intf);
}

static struct gl2_unknown_intf **
_vertex_shader_QueryInterface (struct gl2_unknown_intf **intf, enum gl2_uiid uiid)
{
	if (uiid == UIID_VERTEX_SHADER)
	{
		(**intf).AddRef (intf);
		return intf;
	}
	return _shader_QueryInterface (intf, uiid);
}

static GLenum
_vertex_shader_GetSubType (struct gl2_shader_intf **intf)
{
	return GL_VERTEX_SHADER_ARB;
}

static struct gl2_vertex_shader_intf _vertex_shader_vftbl = {
	{
		{
			{
				_unknown_AddRef,
				_unknown_Release,
				_vertex_shader_QueryInterface
			},
			_generic_Delete,
			_shader_GetType,
			_generic_GetName,
			_generic_GetDeleteStatus,
			_generic_GetInfoLog
		},
		_vertex_shader_GetSubType,
		_shader_GetCompileStatus,
		_shader_SetSource,
		_shader_GetSource,
		_shader_Compile
	}
};

static void
_vertex_shader_constructor (struct gl2_vertex_shader_impl *impl)
{
	_shader_constructor ((struct gl2_shader_impl *) impl);
	impl->_vftbl = &_vertex_shader_vftbl;
	impl->_obj._shader._generic._unknown._destructor = _vertex_shader_destructor;
#if USE_3DLABS_FRONTEND
	impl->_obj._shader._3dlabs_shhandle._obj.handle = ShConstructCompiler (EShLangVertex, 0);
#endif
}

GLhandleARB
_mesa_3dlabs_create_shader_object (GLenum shaderType)
{
	switch (shaderType)
	{
	case GL_FRAGMENT_SHADER_ARB:
		{
			struct gl2_fragment_shader_impl *x = (struct gl2_fragment_shader_impl *)
				_mesa_malloc (sizeof (struct gl2_fragment_shader_impl));

			if (x != NULL)
			{
				_fragment_shader_constructor (x);
				return x->_obj._shader._generic.name;
			}
		}
		break;
	case GL_VERTEX_SHADER_ARB:
		{
			struct gl2_vertex_shader_impl *x = (struct gl2_vertex_shader_impl *)
				_mesa_malloc (sizeof (struct gl2_vertex_shader_impl));

			if (x != NULL)
			{
				_vertex_shader_constructor (x);
				return x->_obj._shader._generic.name;
			}
		}
		break;
	}

	return 0;
}

GLhandleARB
_mesa_3dlabs_create_program_object (void)
{
	struct gl2_program_impl *x = (struct gl2_program_impl *) 
		_mesa_malloc (sizeof (struct gl2_program_impl));

	if (x != NULL)
	{
		_program_constructor (x);
		return x->_obj._container._generic.name;
	}

	return 0;
}

#include "slang_assemble.h"
#include "slang_execute.h"

int _slang_fetch_discard (struct gl2_program_intf **pro, GLboolean *val)
{
	struct gl2_program_impl *impl;

	impl = (struct gl2_program_impl *) pro;
	*val = impl->_obj.prog.machines[SLANG_SHADER_FRAGMENT]->kill ? GL_TRUE : GL_FALSE;
	return 1;
}

static GLvoid exec_shader (struct gl2_program_intf **pro, GLuint i)
{
	struct gl2_program_impl *impl;
	slang_program *p;

	impl = (struct gl2_program_impl *) pro;
	p = &impl->_obj.prog;

	slang_machine_init (p->machines[i]);
	p->machines[i]->ip = p->code[i][SLANG_COMMON_CODE_MAIN];

	_slang_execute2 (p->assemblies[i], p->machines[i]);
}

GLvoid _slang_exec_fragment_shader (struct gl2_program_intf **pro)
{
	exec_shader (pro, SLANG_SHADER_FRAGMENT);
}

GLvoid _slang_exec_vertex_shader (struct gl2_program_intf **pro)
{
	exec_shader (pro, SLANG_SHADER_VERTEX);
}

GLint _slang_get_uniform_location (struct gl2_program_intf **pro, const char *name)
{
	struct gl2_program_impl *impl;
	slang_uniform_bindings *bind;
	GLuint i;

	impl = (struct gl2_program_impl *) pro;
	bind = &impl->_obj.prog.uniforms;
	for (i = 0; i < bind->count; i++)
		if (_mesa_strcmp (bind->table[i].name, name) == 0)
			return i;
	return -1;
}

GLboolean _slang_write_uniform (struct gl2_program_intf **pro, GLint loc, GLsizei count,
	const GLvoid *data, GLenum type)
{
	struct gl2_program_impl *impl;
	slang_uniform_bindings *bind;
	slang_uniform_binding *b;
	GLuint i;
	GLboolean convert_float_to_bool = GL_FALSE;
	GLboolean convert_int_to_bool = GL_FALSE;
	GLboolean convert_int_to_float = GL_FALSE;
	GLboolean types_match = GL_FALSE;

	if (loc == -1)
		return GL_TRUE;

	impl = (struct gl2_program_impl *) pro;
	bind = &impl->_obj.prog.uniforms;
	if (loc >= bind->count)
		return GL_FALSE;

	b = &bind->table[loc];
	/* TODO: check sizes */
	if (b->quant->structure != NULL)
		return GL_FALSE;

	switch (b->quant->u.basic_type)
	{
	case GL_BOOL_ARB:
		types_match = (type == GL_FLOAT) || (type == GL_INT);
		if (type == GL_FLOAT)
			convert_float_to_bool = GL_TRUE;
		else
			convert_int_to_bool = GL_TRUE;
		break;
	case GL_BOOL_VEC2_ARB:
		types_match = (type == GL_FLOAT_VEC2_ARB) || (type == GL_INT_VEC2_ARB);
		if (type == GL_FLOAT_VEC2_ARB)
			convert_float_to_bool = GL_TRUE;
		else
			convert_int_to_bool = GL_TRUE;
		break;
	case GL_BOOL_VEC3_ARB:
		types_match = (type == GL_FLOAT_VEC3_ARB) || (type == GL_INT_VEC3_ARB);
		if (type == GL_FLOAT_VEC3_ARB)
			convert_float_to_bool = GL_TRUE;
		else
			convert_int_to_bool = GL_TRUE;
		break;
	case GL_BOOL_VEC4_ARB:
		types_match = (type == GL_FLOAT_VEC4_ARB) || (type == GL_INT_VEC4_ARB);
		if (type == GL_FLOAT_VEC4_ARB)
			convert_float_to_bool = GL_TRUE;
		else
			convert_int_to_bool = GL_TRUE;
		break;
	case GL_SAMPLER_1D_ARB:
	case GL_SAMPLER_2D_ARB:
	case GL_SAMPLER_3D_ARB:
	case GL_SAMPLER_CUBE_ARB:
	case GL_SAMPLER_1D_SHADOW_ARB:
	case GL_SAMPLER_2D_SHADOW_ARB:
		types_match = (type == GL_INT);
		break;
	default:
		types_match = (type == b->quant->u.basic_type);
		break;
	}

	if (!types_match)
		return GL_FALSE;

	switch (type)
	{
	case GL_INT:
	case GL_INT_VEC2_ARB:
	case GL_INT_VEC3_ARB:
	case GL_INT_VEC4_ARB:
		convert_int_to_float = GL_TRUE;
		break;
	}

	if (convert_float_to_bool)
	{
		for (i = 0; i < SLANG_SHADER_MAX; i++)
			if (b->address[i] != ~0)
			{
				const GLfloat *src = (GLfloat *) (data);
				GLfloat *dst = (GLfloat *) (&impl->_obj.prog.machines[i]->mem[b->address[i] / 4]);
				GLuint j;

				for (j = 0; j < count * b->quant->size / 4; j++)
					dst[j] = src[j] != 0.0f ? 1.0f : 0.0f;
			}
	}
	else if (convert_int_to_bool)
	{
		for (i = 0; i < SLANG_SHADER_MAX; i++)
			if (b->address[i] != ~0)
			{
				const GLuint *src = (GLuint *) (data);
				GLfloat *dst = (GLfloat *) (&impl->_obj.prog.machines[i]->mem[b->address[i] / 4]);
				GLuint j;

				for (j = 0; j < count * b->quant->size / 4; j++)
					dst[j] = src[j] ? 1.0f : 0.0f;
			}
	}
	else if (convert_int_to_float)
	{
		for (i = 0; i < SLANG_SHADER_MAX; i++)
			if (b->address[i] != ~0)
			{
				const GLuint *src = (GLuint *) (data);
				GLfloat *dst = (GLfloat *) (&impl->_obj.prog.machines[i]->mem[b->address[i] / 4]);
				GLuint j;

				for (j = 0; j < count * b->quant->size / 4; j++)
					dst[j] = (GLfloat) src[j];
			}
	}
	else
	{
		for (i = 0; i < SLANG_SHADER_MAX; i++)
			if (b->address[i] != ~0)
			{
				_mesa_memcpy (&impl->_obj.prog.machines[i]->mem[b->address[i] / 4], data,
					count * b->quant->size);
			}
	}
	return GL_TRUE;
}

GLuint _slang_get_active_uniform_count (struct gl2_program_intf **pro)
{
	struct gl2_program_impl *impl;

	impl = (struct gl2_program_impl *) pro;
	return impl->_obj.prog.active_uniforms.count;
}

GLuint _slang_get_active_uniform_max_length (struct gl2_program_intf **pro)
{
	struct gl2_program_impl *impl;
	GLuint i, len = 0;

	impl = (struct gl2_program_impl *) pro;
	for (i = 0; i < impl->_obj.prog.active_uniforms.count; i++)
	{
		GLuint n = _mesa_strlen (impl->_obj.prog.active_uniforms.table[i].name);
		if (n > len)
			len = n;
	}
	return len;
}

GLvoid _slang_get_active_uniform (struct gl2_program_intf **pro, GLuint index, GLsizei maxLength,
	GLsizei *length, GLint *size, GLenum *type, char *name)
{
	struct gl2_program_impl *impl;
	slang_active_uniform *u;
	GLsizei len;

	impl = (struct gl2_program_impl *) pro;
	u = &impl->_obj.prog.active_uniforms.table[index];

	len = _mesa_strlen (u->name);
	if (len >= maxLength)
		len = maxLength - 1;
	_mesa_memcpy (name, u->name, len);
	name[len] = '\0';
	if (length != NULL)
		*length = len;
	*type = u->quant->u.basic_type;
	if (u->quant->array_len == 0)
		*size = 1;
	else
		*size = u->quant->array_len;
}

void
_mesa_init_shaderobjects_3dlabs (GLcontext *ctx)
{
#if USE_3DLABS_FRONTEND
	_glslang_3dlabs_InitProcess ();
	_glslang_3dlabs_ShInitialize ();
#endif
}

