noinst_LTLIBRARIES = libslang.la

AM_CFLAGS = \
	$(DIX_CFLAGS) \
	@SERVER_DEFINES@ \
	@MODULE_DEFINES@ \
	@LOADER_DEFINES@ \
	@GLX_DEFINES@

INCLUDES = -I@MESA_SOURCE@/include \
           -I../grammar \
           -I../../X \
           -I../../array_cache \
           -I../../glapi \
           -I../../main \
           -I../../math \
           -I../../shader \
           -I../../swrast \
           -I../../swrast_setup \
           -I../../tnl \
           -I../.. \
           -I$(top_srcdir)/hw/xfree86/os-support

nodist_libslang_la_SOURCES = slang_analyse.c \
		      slang_assemble_assignment.c \
		      slang_assemble.c \
		      slang_assemble_conditional.c \
		      slang_assemble_constructor.c \
		      slang_assemble_typeinfo.c \
		      slang_compile.c \
		      slang_compile_function.c \
		      slang_compile_operation.c \
		      slang_compile_struct.c \
		      slang_compile_variable.c \
		      slang_execute.c \
		      slang_execute_x86.c \
		      slang_export.c \
		      slang_library_texsample.c \
		      slang_library_noise.c \
		      slang_link.c \
		      slang_preprocess.c \
		      slang_storage.c \
		      slang_utility.c
