;; Machine description for OpenRISC
;; Copyright (C) 2018-2024 Free Software Foundation, Inc.
;; Contributed by Stafford Horne

;; This file is part of GCC.

;; GCC is free software; you can redistribute it and/or modify it
;; under the terms of the GNU General Public License as published
;; by the Free Software Foundation; either version 3, or (at your
;; option) any later version.

;; GCC is distributed in the hope that it will be useful, but WITHOUT
;; ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
;; or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
;; License for more details.

;; You should have received a copy of the GNU General Public License
;; along with GCC; see the file COPYING3.  If not see
;; <http://www.gnu.org/licenses/>.

;; -------------------------------------------------------------------------
;; OpenRISC specific constraints, predicates and attributes
;; -------------------------------------------------------------------------

;;(include "constraints.md")
;;(include "predicates.md")

;; Register numbers
(define_constants
  [(SP_REGNUM       1)
   (HFP_REGNUM      2)
   (LR_REGNUM       3)
   (RV_REGNUM       4)
   (SR_REGNUM    32)]
)

;; -------------------------------------------------------------------------
;; nop instruction
;; -------------------------------------------------------------------------

(define_insn "nop"
  [(const_int 0)]
  ""
  "NOP")

;; -------------------------------------------------------------------------
;; Arithmetic instructions
;; -------------------------------------------------------------------------

(define_insn "addsi3"
  [(set (match_operand:SI 0 "register_operand" "=r")
	  (plus:SI
	   (match_operand:SI 1 "register_operand"   "r")
	   (match_operand:SI 2 "register_operand" "0")))]
  ""
  "ADD\t%1, %0")

(define_insn "subsi3"
  [(set (match_operand:SI 0 "register_operand" "=r")
	  (minus:SI
	   (match_operand:SI 1 "register_operand" "r")
	   (match_operand:SI 2 "register_operand" "0")))]
  ""
  "SUB\t%1, %0")

(define_insn "mulsi3"
  [(set (match_operand:SI 0 "register_operand" "=r")
	  (mult:SI
	   (match_operand:SI 1 "register_operand" "r")
	   (match_operand:SI 2 "register_operand" "0")))]
  "!TARGET_SOFT_MUL"
  "MUL\t%1, %0")

(define_insn "divsi3"
  [(set (match_operand:SI 0 "register_operand" "=r")
	  (div:SI
	   (match_operand:SI 1 "register_operand" "r")
	   (match_operand:SI 2 "register_operand" "0")))]
  "!TARGET_SOFT_DIV"
  "DIV\t%1, %0")

;; -------------------------------------------------------------------------
;; Logical operators
;; -------------------------------------------------------------------------

(define_code_iterator SHIFT  [ashift ashiftrt lshiftrt])
(define_code_attr shift_op   [(ashift "ashl") (ashiftrt "ashr")
			      (lshiftrt "lshr")])
(define_code_attr shift_asm  [(ashift "SLL") (ashiftrt "SRA")
			      (lshiftrt "SRL")])

(define_insn "<shift_op>si3"
  [(set (match_operand:SI 0 "register_operand" "=r")
	(SHIFT:SI (match_operand:SI 1 "register_operand"  "0")
		  (match_operand:SI 2 "immediate_operand" "i")))]
  ""
  "<shift_asm>\t%0, %2")

(define_insn "andsi3"
  [(set (match_operand:SI 0 "register_operand" "=r")
	  (and:SI
	   (match_operand:SI 1 "register_operand"   "r")
	   (match_operand:SI 2 "register_operand" "0")))]
  ""
  "AND\t%1, %0")

(define_insn "xorsi3"
  [(set (match_operand:SI 0 "register_operand" "=r")
	  (xor:SI
	   (match_operand:SI 1 "register_operand"   "r")
	   (match_operand:SI 2 "register_operand" "0")))]
  ""
  "XOR\t%1, %0")

(define_insn "iorsi3"
  [(set (match_operand:SI 0 "register_operand" "=r")
	  (ior:SI
	   (match_operand:SI 1 "register_operand"   "r")
	   (match_operand:SI 2 "register_operand" "0")))]
  ""
  "OR\t%1, %0")

;; -------------------------------------------------------------------------
;; Move instructions
;; -------------------------------------------------------------------------

;; 32-bit move
(define_insn "movsi"
  [(set (match_operand:SI 0 "register_operand" "=r,r")
	(match_operand:SI 1 "nonmemory_operand"        " r,i"))]
  ""
  "@
   MOV\t%1, %0
   LDA\t%0, %1(r0")

;; -------------------------------------------------------------------------
;; Sign Extending
;; -------------------------------------------------------------------------

;; -------------------------------------------------------------------------
;; Compare instructions
;; -------------------------------------------------------------------------

;; -------------------------------------------------------------------------
;; Conditional Store instructions
;; -------------------------------------------------------------------------

;; -------------------------------------------------------------------------
;; Branch instructions
;; -------------------------------------------------------------------------

;; -------------------------------------------------------------------------
;; Jump instructions
;; -------------------------------------------------------------------------

(define_insn "jump"
  [(set (pc) (label_ref (match_operand 0 "" "")))]
  ""
  "BRA\t%0%#")

(define_insn "indirect_jump"
  [(set (pc) (match_operand:SI 0 "register_operand" "r"))]
  ""
  "BAL\t%0,r0")

;; -------------------------------------------------------------------------
;; Prologue & Epilogue
;; -------------------------------------------------------------------------

(define_expand "simple_return"
  [(parallel [(simple_return) (use (match_dup 0))])]
  ""
{
  operands[0] = gen_rtx_REG (Pmode, LR_REGNUM);
})

(define_insn "*simple_return"
  [(simple_return)
   (use (match_operand:SI 0 "register_operand" "r"))]
  ""
  "l.jr\t%0%#")

;; -------------------------------------------------------------------------
;; Atomic Operations
;; -------------------------------------------------------------------------

;; -------------------------------------------------------------------------
;; Call Instructions
;; -------------------------------------------------------------------------

(define_insn "*call"
  [(call (mem:SI (match_operand:SI 0 "register_operand" "r"))
	 (match_operand 1))
   (clobber (reg:SI LR_REGNUM))]
  "!SIBLING_CALL_P (insn)"
  "BAL\t%0,r3")
