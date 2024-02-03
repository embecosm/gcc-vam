/* Target Code for VAM
   Copyright (C) 2018-2024 Free Software Foundation, Inc.
   Contributed by Stafford Horne based on other ports.

   This file is part of GCC.

   GCC is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 3, or (at your
   option) any later version.

   GCC is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   You should have received a copy of the GNU General Public License
   along with GCC; see the file COPYING3.  If not see
   <http://www.gnu.org/licenses/>.  */

#define IN_TARGET_CODE 1

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "backend.h"
#include "target.h"
#include "rtl.h"
#include "tree.h"
#include "stringpool.h"
#include "attribs.h"
#include "df.h"
#include "regs.h"
#include "memmodel.h"
#include "emit-rtl.h"
#include "diagnostic-core.h"
#include "output.h"
#include "stor-layout.h"
#include "varasm.h"
#include "calls.h"
#include "expr.h"
#include "builtins.h"
#include "optabs.h"
#include "explow.h"
#include "cfgrtl.h"
#include "alias.h"
#include "targhooks.h"
#include "case-cfn-macros.h"

/* These 4 are needed to allow using satisfies_constraint_J.  */
#include "insn-config.h"
#include "recog.h"
#include "tm_p.h"
#include "tm-constrs.h"

/* This file should be included last.  */
#include "target-def.h"

/* Per-function machine data.  */
struct GTY(()) machine_function
{
  /* Number of bytes saved on the stack for callee saved registers.  */
  HOST_WIDE_INT callee_saved_reg_size;

  /* Number of bytes saved on the stack for local variables.  */
  HOST_WIDE_INT local_vars_size;

  /* Number of bytes saved on the stack for outgoing/sub-function args.  */
  HOST_WIDE_INT args_size;

  /* The sum of sizes: locals vars, called saved regs, stack pointer
     and an optional frame pointer.
     Used in expand_prologue () and expand_epilogue ().  */
  HOST_WIDE_INT total_size;

  /* Remember where the set_got_placeholder is located.  */
  rtx_insn *set_got_insn;

  /* Remember where mcount args are stored so we can insert set_got_insn
     after.  */
  rtx_insn *set_mcount_arg_insn;
};

/* Zero initialization is OK for all current fields.  */

static struct machine_function *
vam_init_machine_status (void)
{
  return ggc_cleared_alloc<machine_function> ();
}

#define TARGET_HAVE_TLS false

/* Calling Conventions.  */

/* Assembly generation.  */

/* Section anchor support.  */

struct gcc_target targetm = TARGET_INITIALIZER;

#include "gt-vam.h"
