#define R_NO_REMAP
#include <R.h>
#include <Rinternals.h>

int get_res_size(const double* vals, const R_len_t n);

SEXP my_c_rle(SEXP x) {

  const R_xlen_t n = Rf_xlength(x);

  // We're assuming doubles but rle() works on all types of data.
  // Only a specific case is covered here
  const double* vals = REAL(x);

  // Since loops are cheap in C, we can make a first loop to get exactly the
  // right size for the output
  int res_n = get_res_size(vals, n);

  SEXP lengths = PROTECT(Rf_allocVector(INTSXP, res_n));
  SEXP values  = PROTECT(Rf_allocVector(TYPEOF(x), res_n));
  SEXP res = PROTECT(Rf_allocVector(VECSXP, 2));

  SEXP nm = PROTECT(Rf_allocVector(STRSXP, 2));
  SEXP cl = PROTECT(Rf_allocVector(STRSXP, 1));

  int l = 0;
  // Spotted by Paul Murrell during discussion: we're pointing to memory if
  // input is of length 0
  REAL(values)[l] = vals[0];
  INTEGER(lengths)[l] = 1;

  for (R_xlen_t i = 1; i < n; i++)
  {
    if (vals[i] == vals[i-1])
    {
      INTEGER(lengths)[l]++;
    }
    else
    {
      l++;
      REAL(values)[l] = vals[i];
      INTEGER(lengths)[l] = 1;
    }
  }
  SET_VECTOR_ELT(res, 0, lengths);
  SET_VECTOR_ELT(res, 1, values);

  SET_STRING_ELT(nm, 0, Rf_mkCharCE("lengths", CE_UTF8));
  SET_STRING_ELT(nm, 1, Rf_mkCharCE("values", CE_UTF8));

  SET_STRING_ELT(cl, 0, Rf_mkCharCE("rle", CE_UTF8));

  Rf_setAttrib(res, R_NamesSymbol, nm);
  Rf_setAttrib(res, R_ClassSymbol, cl);

  UNPROTECT(5);
  return res;

}

int get_res_size(const double* vals, const R_len_t n) {

  int l = 1;
  for (R_xlen_t i = 1; i < n; i++)
  {
    if (vals[i] != vals[i-1])
    {
      l++;
    }
  }

  return l;
}
