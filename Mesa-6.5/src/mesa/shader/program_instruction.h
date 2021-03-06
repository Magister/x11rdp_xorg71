/*
 * Mesa 3-D graphics library
 * Version:  6.5
 *
 * Copyright (C) 1999-2005  Brian Paul   All Rights Reserved.
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
 * \file prog_instruction.h
 *
 * Private vertex program types and constants only used by files
 * related to vertex programs.
 *
 * \author Brian Paul
 * \author Keith Whitwell
 * \author Ian Romanick <idr@us.ibm.com>
 */


#ifndef PROG_INSTRUCTION_H
#define PROG_INSTRUCTION_H

/**
 * Condition codes for GL_NV_fragment_program
 */
/*@{*/
#define COND_GT  1  /* greater than zero */
#define COND_EQ  2  /* equal to zero */
#define COND_LT  3  /* less than zero */
#define COND_UN  4  /* unordered (NaN) */
#define COND_GE  5  /* greater then or equal to zero */
#define COND_LE  6  /* less then or equal to zero */
#define COND_NE  7  /* not equal to zero */
#define COND_TR  8  /* always true */
#define COND_FL  9  /* always false */
/*@}*/


/**
 * Instruction precision for GL_NV_fragment_program
 */
/*@{*/
#define FLOAT32  0x1
#define FLOAT16  0x2
#define FIXED12  0x4
/*@}*/


/**
 * Saturation modes when storing values.
 */
/*@{*/
#define SATURATE_OFF            0
#define SATURATE_ZERO_ONE       1
#define SATURATE_PLUS_MINUS_ONE 2
/*@}*/


/**
 * Program instruction opcodes, for both vertex and fragment programs.
 * \note changes to this opcode list must be reflected in t_vb_arbprogram.c
 */
                     /* ARB_vp   ARB_fp   NV_vp   NV_fp */
enum prog_opcode {   /*---------------------------------*/
   OPCODE_ABS,       /*   X        X       1.1          */
   OPCODE_ADD,       /*   X        X       X       X    */
   OPCODE_ARA,       /*                    2            */
   OPCODE_ARL,       /*   X                X            */
   OPCODE_ARL_NV,    /*                    2            */
   OPCODE_ARR,       /*                    2            */
   OPCODE_BRA,       /*                    2            */
   OPCODE_CAL,       /*                    2       2    */
   OPCODE_CMP,       /*            X                    */
   OPCODE_COS,       /*            X       2       X    */
   OPCODE_DDX,       /*                            X    */
   OPCODE_DDY,       /*                            X    */
   OPCODE_DP3,       /*   X        X       X       X    */
   OPCODE_DP4,       /*   X        X       X       X    */
   OPCODE_DPH,       /*   X        X       1.1          */
   OPCODE_DST,       /*   X        X       X       X    */
   OPCODE_END,       /*   X        X       X       X    */
   OPCODE_EX2,       /*   X        X       2       X    */
   OPCODE_EXP,       /*   X                X            */
   OPCODE_FLR,       /*   X        X       2       X    */
   OPCODE_FRC,       /*   X        X       2       X    */
   OPCODE_KIL,       /*            X                    */
   OPCODE_KIL_NV,    /*                            X    */
   OPCODE_LG2,       /*   X        X       2       X    */
   OPCODE_LIT,       /*   X        X       X       X    */
   OPCODE_LOG,       /*   X                X            */
   OPCODE_LRP,       /*            X               X    */
   OPCODE_MAD,       /*   X        X       X       X    */
   OPCODE_MAX,       /*   X        X       X       X    */
   OPCODE_MIN,       /*   X        X       X       X    */
   OPCODE_MOV,       /*   X        X       X       X    */
   OPCODE_MUL,       /*   X        X       X       X    */
   OPCODE_PK2H,      /*                            X    */
   OPCODE_PK2US,     /*                            X    */
   OPCODE_PK4B,      /*                            X    */
   OPCODE_PK4UB,     /*                            X    */
   OPCODE_POW,       /*   X        X               X    */
   OPCODE_POPA,      /*                    3            */
   OPCODE_PRINT,     /*                    X       X    */
   OPCODE_PUSHA,     /*                    3            */
   OPCODE_RCC,       /*                    1.1          */
   OPCODE_RCP,       /*   X        X       X       X    */
   OPCODE_RET,       /*                    2       2    */
   OPCODE_RFL,       /*            X               X    */
   OPCODE_RSQ,       /*   X        X       X       X    */
   OPCODE_SCS,       /*            X                    */
   OPCODE_SEQ,       /*                    2       X    */
   OPCODE_SFL,       /*                    2       X    */
   OPCODE_SGE,       /*   X        X       X       X    */
   OPCODE_SGT,       /*                    2       X    */
   OPCODE_SIN,       /*            X       2       X    */
   OPCODE_SLE,       /*                    2       X    */
   OPCODE_SLT,       /*   X        X       X       X    */
   OPCODE_SNE,       /*                    2       X    */
   OPCODE_SSG,       /*                    2            */
   OPCODE_STR,       /*                    2       X    */
   OPCODE_SUB,       /*   X        X       1.1     X    */
   OPCODE_SWZ,       /*   X        X                    */
   OPCODE_TEX,       /*            X       3       X    */
   OPCODE_TXB,       /*            X       3            */
   OPCODE_TXD,       /*                            X    */
   OPCODE_TXL,       /*                    3       2    */
   OPCODE_TXP,       /*            X                    */
   OPCODE_TXP_NV,    /*                    3       X    */
   OPCODE_UP2H,      /*                            X    */
   OPCODE_UP2US,     /*                            X    */
   OPCODE_UP4B,      /*                            X    */
   OPCODE_UP4UB,     /*                            X    */
   OPCODE_X2D,       /*                            X    */
   OPCODE_XPD,       /*   X        X                    */
   MAX_OPCODE
};


/**
 * Instruction source register.
 */
struct prog_src_register
{
   GLuint File:4;	/**< One of the PROGRAM_* register file values. */
   GLint Index:9;	/**< May be negative for relative addressing. */
   GLuint Swizzle:12;
   GLuint RelAddr:1;

   /**
    * \name Source register "sign" control.
    *
    * The ARB and NV extensions allow varrying degrees of control over the
    * sign of the source vector components.  These values allow enough control
    * for all flavors of the extensions.
    */
   /*@{*/
   /**
    * Per-component negation for the SWZ instruction.
    *
    * \since
    * ARB_vertex_program, ARB_fragment_program
    */
   GLuint NegateBase:4;

   /**
    * Take the component-wise absolute value.
    *
    * \since
    * NV_fragment_program, NV_fragment_program_option, NV_vertex_program2,
    * NV_vertex_program2_option.
    */
   GLuint Abs:1;

   /**
    * Take the component-wise negation.  The negation occurs \b after the
    * (optional) absolute value operation.
    */
   GLuint NegateAbs:1;
   /*@}*/
};


/**
 * Instruction destination register.
 */
struct prog_dst_register
{
   /**
    * One of the PROGRAM_* register file values.
    */
   GLuint File:4;

   GLuint Index:8;
   GLuint WriteMask:4;

   /**
    * \name Conditional destination update control.
    *
    * \since
    * NV_fragment_program, NV_fragment_program_option, NV_vertex_program2,
    * NV_vertex_program2_option.
    */
   /*@{*/
   /**
    * Takes one of the 9 possible condition values (EQ, FL, GT, GE, LE, LT,
    * NE, TR, or UN).  Destination update is enabled if the matching
    * (swizzled) condition code value passes.  When a conditional update mask
    * is not specified, this will be \c COND_TR.
    */
   GLuint CondMask:4;

   /**
    * Condition code swizzle value.
    */
   GLuint CondSwizzle:12;
   
   /**
    * Selects the condition code register to use for conditional destination
    * update masking.  In NV_fragmnet_program or NV_vertex_program2 mode, only
    * condition code register 0 is available.  In NV_vertex_program3 mode, 
    * condition code registers 0 and 1 are available.
    */
   GLuint CondSrc:1;
   /*@}*/

   GLuint pad:31;
};


/**
 * Vertex/fragment program instruction.
 */
struct prog_instruction
{
   enum prog_opcode Opcode;
#if FEATURE_MESA_program_debug
   GLshort StringPos;
#endif
   /**
    * Arbitrary data.  Used for the PRINT, CAL, and BRA instructions.
    */
   void *Data;

   struct prog_src_register SrcReg[3];
   struct prog_dst_register DstReg;

   /**
    * Indicates that the instruction should update the condition code
    * register.
    *
    * \since
    * NV_fragment_program, NV_fragment_program_option, NV_vertex_program2,
    * NV_vertex_program2_option.
    */
   GLuint CondUpdate:1;

   /**
    * If prog_instruction::cc_update is \c GL_TRUE, this value selects the
    * condition code register that is to be updated.
    *
    * In GL_NV_fragment_program or GL_NV_vertex_program2 mode, only condition
    * code register 0 is available.  In GL_NV_vertex_program3 mode, condition
    * code registers 0 and 1 are available.
    *
    * \since
    * NV_fragment_program, NV_fragment_program_option, NV_vertex_program2,
    * NV_vertex_program2_option.
    */
   GLuint CondDst:1;

   /**
    * Saturate each value of the vectored result to the range [0,1] or the
    * range [-1,1].  \c SSAT mode (i.e., saturation to the range [-1,1]) is
    * only available in NV_fragment_program2 mode.
    * Value is one of the SATURATE_* tokens.
    *
    * \since
    * NV_fragment_program, NV_fragment_program_option, NV_vertex_program3.
    */
   GLuint SaturateMode:2;
   
   /**
    * Per-instruction selectable precision.
    *
    * \since
    * NV_fragment_program, NV_fragment_program_option.
    */
   GLuint Precision:3;

   /**
    * \name Texture source controls.
    * 
    * The texture source controls are only used with the \c TEX, \c TXD,
    * \c TXL, and \c TXP instructions.
    *
    * \since
    * ARB_fragment_program, NV_fragment_program, NV_vertex_program3.
    */
   /*@{*/
   /**
    * Source texture unit.  OpenGL supports a maximum of 32 texture
    * units.
    */
   GLuint TexSrcUnit:5;
   
   /**
    * Source texture target, one of TEXTURE_{1D,2D,3D,CUBE,RECT}_INDEX.
    */
   GLuint TexSrcTarget:3;
   /*@}*/
};


extern void
_mesa_init_instruction(struct prog_instruction *inst);

extern GLuint
_mesa_num_inst_src_regs(enum prog_opcode opcode);

extern const char *
_mesa_opcode_string(enum prog_opcode opcode);


#endif /* PROG_INSTRUCTION_H */
