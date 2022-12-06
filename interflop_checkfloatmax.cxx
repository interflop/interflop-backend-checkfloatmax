
/*--------------------------------------------------------------------*/
/*--- Verrou: a FPU instrumentation tool.                          ---*/
/*--- Interface for floating-point operations overloading.         ---*/
/*---                                  interflop_check_float_max.h ---*/
/*--------------------------------------------------------------------*/

/*
   This file is part of Verrou, a FPU instrumentation tool.

   Copyright (C) 2014-2021 EDF
     B. Lathuilière <bruno.lathuiliere@edf.fr>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public License as
   published by the Free Software Foundation; either version 2.1 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307, USA.

   The GNU Lesser General Public License is contained in the file COPYING.
*/

#include <float.h>
#include <stddef.h>

#include "interflop/interflop_stdlib.h"
#include "interflop/iostream/logger.h"
#include "interflop_checkfloatmax.h"

static const char backend_name[] = "interflop-checkfloatmax";
static const char backend_version[] = "1.x-dev";
static File *stderr_stream;

template <typename REAL>
void ifmax_checkMax(const REAL &a, const REAL &b, const REAL &r);

template <typename REAL> bool ifmax_is_flt_max(const REAL &a);

template <> bool ifmax_is_flt_max(const float &a) { return a == FLT_MAX; }
template <> bool ifmax_is_flt_max(const double &a) { return a == DBL_MAX; }

template <typename REAL>
inline void ifmax_checkmax(const char *op, const REAL &a, const REAL &b,
                           const REAL &r) {

  if (ifmax_is_flt_max(a) || ifmax_is_flt_max(b)) {
    if (interflop_debug_print_op != NULL) {
      double param[2] = {a, b};
      double res[1] = {r};
      interflop_debug_print_op(2, op, param, res);
    }
    interflop_maxHandler();
  }
}

template <typename REAL>
inline void ifmax_checkmax(const char *op, const REAL &a, const REAL &b,
                           const REAL &c, const REAL &r) {

  if (ifmax_is_flt_max(a) || ifmax_is_flt_max(b) || ifmax_is_flt_max(c)) {
    if (interflop_debug_print_op != NULL) {
      double param[3] = {a, b, c};
      double res[1] = {r};
      interflop_debug_print_op(3, op, param, res);
    }
    interflop_maxHandler();
  }
}

template <typename REAL>
inline void ifmax_checkmax(const char *op, const REAL &a, const float &r) {

  if (ifmax_is_flt_max(a) || ifmax_is_flt_max(r)) {
    if (interflop_debug_print_op != NULL) {
      double param[1] = {a};
      double res[1] = {r};
      interflop_debug_print_op(1, op, param, res);
    }
    interflop_maxHandler();
  }
}

#if defined(__cplusplus)
extern "C" {
#endif

// * C interface

const char *INTERFLOP_CHECKFLOATMAX_API(get_backend_name)() {
  return backend_name;
}

const char *INTERFLOP_CHECKFLOATMAX_API(get_backend_version)() {
  return backend_version;
}

void INTERFLOP_CHECKFLOATMAX_API(finalize)([[maybe_unused]] void *context) {}

void INTERFLOP_CHECKFLOATMAX_API(add_double)(double a, double b, double *res,
                                             [[maybe_unused]] void *context) {
  ifmax_checkmax("add_double", a, b, *res);
}

void INTERFLOP_CHECKFLOATMAX_API(add_float)(float a, float b, float *res,
                                            [[maybe_unused]] void *context) {
  ifmax_checkmax("add_float", a, b, *res);
}

void INTERFLOP_CHECKFLOATMAX_API(sub_double)(double a, double b, double *res,
                                             [[maybe_unused]] void *context) {
  ifmax_checkmax("sub_double", a, b, *res);
}

void INTERFLOP_CHECKFLOATMAX_API(sub_float)(float a, float b, float *res,
                                            [[maybe_unused]] void *context) {
  ifmax_checkmax("sub_float", a, b, *res);
}

void INTERFLOP_CHECKFLOATMAX_API(mul_double)(double a, double b, double *res,
                                             [[maybe_unused]] void *context) {
  ifmax_checkmax("mul_double", a, b, *res);
}

void INTERFLOP_CHECKFLOATMAX_API(mul_float)(float a, float b, float *res,
                                            [[maybe_unused]] void *context) {
  ifmax_checkmax("mul_float", a, b, *res);
}

void INTERFLOP_CHECKFLOATMAX_API(div_double)(double a, double b, double *res,
                                             [[maybe_unused]] void *context) {
  ifmax_checkmax("div_double", a, b, *res);
}

void INTERFLOP_CHECKFLOATMAX_API(div_float)(float a, float b, float *res,
                                            [[maybe_unused]] void *context) {
  ifmax_checkmax("div_float", a, b, *res);
}
void INTERFLOP_CHECKFLOATMAX_API(cast_double_to_float)(
    double a, float *res, [[maybe_unused]] void *context) {
  ifmax_checkmax("cast_double_to_float", a, *res);
}

void INTERFLOP_CHECKFLOATMAX_API(fma_double)(double a, double b, double c,
                                             double *res,
                                             [[maybe_unused]] void *context) {
  ifmax_checkmax("fma_double", a, b, c, *res);
}

void INTERFLOP_CHECKFLOATMAX_API(fma_float)(float a, float b, float c,
                                            float *res,
                                            [[maybe_unused]] void *context) {
  ifmax_checkmax("fma_float", a, b, c, *res);
}

#define CHECK_IMPL(name)                                                       \
  if (interflop_##name == Null) {                                              \
    interflop_panic("Interflop backend error: " #name " not implemented\n");   \
  }

void _checkfloatmax_check_stdlib(void) { CHECK_IMPL(maxHandler); }

void INTERFLOP_CHECKFLOATMAX_API(pre_init)(interflop_panic_t panic,
                                           File *stream,
                                           [[maybe_unused]] void **context) {
  stderr_stream = stream;
  interflop_set_handler("panic", (void *)panic);
  _checkfloatmax_check_stdlib();
  /* Initialize the logger */
  logger_init(panic, stream, backend_name);
}

static void print_information_header([[maybe_unused]] void *context) {
  /* Environnement variable to disable loading message */
  char *silent_load_env = interflop_getenv("VFC_BACKENDS_SILENT_LOAD");
  bool silent_load = ((silent_load_env == NULL) ||
                      (interflop_strcasecmp(silent_load_env, "True") != 0))
                         ? false
                         : true;

  if (silent_load)
    return;

  logger_info("load backend with:\n");
}

void INTERFLOP_CHECKFLOATMAX_API(cli)([[maybe_unused]] int argc,
                                      [[maybe_unused]] char **argv,
                                      [[maybe_unused]] void *context) {}

void INTERFLOP_CHECKFLOATMAX_API(configure)([[maybe_unused]] void *configure,
                                            [[maybe_unused]] void *context) {}
struct interflop_backend_interface_t
INTERFLOP_CHECKFLOATMAX_API(init)(void *context) {
  checkfloatmax_context_t *ctx = (checkfloatmax_context_t *)context;
  print_information_header(ctx);

  struct interflop_backend_interface_t interflop_checkfloatmax_backend = {
    interflop_add_float : INTERFLOP_CHECKFLOATMAX_API(add_float),
    interflop_sub_float : INTERFLOP_CHECKFLOATMAX_API(sub_float),
    interflop_mul_float : INTERFLOP_CHECKFLOATMAX_API(mul_float),
    interflop_div_float : INTERFLOP_CHECKFLOATMAX_API(div_float),
    interflop_cmp_float : NULL,
    interflop_add_double : INTERFLOP_CHECKFLOATMAX_API(add_double),
    interflop_sub_double : INTERFLOP_CHECKFLOATMAX_API(sub_double),
    interflop_mul_double : INTERFLOP_CHECKFLOATMAX_API(mul_double),
    interflop_div_double : INTERFLOP_CHECKFLOATMAX_API(div_double),
    interflop_cmp_double : NULL,
    interflop_cast_double_to_float :
        INTERFLOP_CHECKFLOATMAX_API(cast_double_to_float),
    interflop_fma_float : INTERFLOP_CHECKFLOATMAX_API(fma_float),
    interflop_fma_double : INTERFLOP_CHECKFLOATMAX_API(fma_double),
    interflop_enter_function : NULL,
    interflop_exit_function : NULL,
    interflop_user_call : NULL,
    interflop_finalize : INTERFLOP_CHECKFLOATMAX_API(finalize)
  };
  return interflop_checkfloatmax_backend;
}

struct interflop_backend_interface_t interflop_init(void *context)
    __attribute__((weak, alias("interflop_checkfloatmax_init")));

void interflop_pre_init(interflop_panic_t panic, File *stream, void **context)
    __attribute__((weak, alias("interflop_checkfloatmax_pre_init")));

void interflop_cli(int argc, char **argv, void *context)
    __attribute__((weak, alias("interflop_checkfloatmax_cli")));

#if defined(__cplusplus)
}
#endif