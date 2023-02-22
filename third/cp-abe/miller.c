#include "/usr/local/include/pbc/pbc.h"
#include "/usr/local/include/pbc/pbc_fp.h"
#include "/usr/local/include/pbc/pbc_fieldquadratic.h"

static void miller(element_t res, element_t P, element_ptr QR, element_ptr R, int n) {
  // Collate divisions.
  int m;
  element_t v, vd;
  element_t Z;
  element_t a, b, c;
  const element_ptr cca = curve_a_coeff(P);
  const element_ptr Px = curve_x_coord(P);
  const element_ptr Py = curve_y_coord(P);
  element_t e0, e1;
  mpz_t q;
  element_ptr Zx, Zy;
  const element_ptr numx = curve_x_coord(QR);
  const element_ptr numy = curve_y_coord(QR);
  const element_ptr denomx = curve_x_coord(R);
  const element_ptr denomy = curve_y_coord(R);

  void do_vertical(element_t e, element_t edenom)
  {
    element_sub(e0, numx, Zx);
    element_mul(e, e, e0);

    element_sub(e0, denomx, Zx);
    element_mul(edenom, edenom, e0);
  }

  void do_tangent(element_t e, element_t edenom)
  {
    //a = -slope_tangent(A.x, A.y);
    //b = 1;
    //c = -(A.y + a * A.x);
    //but we multiply by 2*A.y to avoid division

    //a = -Ax * (Ax + Ax + Ax + twicea_2) - a_4;
    //Common curves: a2 = 0 (and cc->a is a_4), so
    //a = -(3 Ax^2 + cc->a)
    //b = 2 * Ay
    //c = -(2 Ay^2 + a Ax);

    if (element_is0(Zy)) {
      do_vertical(e, edenom);
      return;
    }
    element_square(a, Zx);
    element_mul_si(a, a, 3);
    element_add(a, a, cca);
    element_neg(a, a);

    element_add(b, Zy, Zy);

    element_mul(e0, b, Zy);
    element_mul(c, a, Zx);
    element_add(c, c, e0);
    element_neg(c, c);

    element_mul(e0, a, numx);
    element_mul(e1, b, numy);
    element_add(e0, e0, e1);
    element_add(e0, e0, c);
    element_mul(e, e, e0);

    element_mul(e0, a, denomx);
    element_mul(e1, b, denomy);
    element_add(e0, e0, e1);
    element_add(e0, e0, c);
    element_mul(edenom, edenom, e0);
  }

  void do_line(element_ptr e, element_ptr edenom)
  {
    if (!element_cmp(Zx, Px)) {
      if (!element_cmp(Zy, Py)) {
        do_tangent(e, edenom);
      } else {
        do_vertical(e, edenom);
      }
      return;
    }

    element_sub(b, Px, Zx);
    element_sub(a, Zy, Py);
    element_mul(c, Zx, Py);
    element_mul(e0, Zy, Px);
    element_sub(c, c, e0);

    element_mul(e0, a, numx);
    element_mul(e1, b, numy);
    element_add(e0, e0, e1);
    element_add(e0, e0, c);
    element_mul(e, e, e0);

    element_mul(e0, a, denomx);
    element_mul(e1, b, denomy);
    element_add(e0, e0, e1);
    element_add(e0, e0, c);
    element_mul(edenom, edenom, e0);
  }

  element_init(a, res->field);
  element_init(b, res->field);
  element_init(c, res->field);
  element_init(e0, res->field);
  element_init(e1, res->field);

  element_init(v, res->field);
  element_init(vd, res->field);
  element_init(Z, P->field);

  element_set(Z, P);
  Zx = curve_x_coord(Z);
  Zy = curve_y_coord(Z);

  element_set1(v);
  element_set1(vd);

  mpz_init(q);
  mpz_set_ui(q, n);
  m = mpz_sizeinbase(q, 2) - 2;

  while(m >= 0) {
    element_square(v, v);
    element_square(vd, vd);
    do_tangent(v, vd);
    element_double(Z, Z);
    do_vertical(vd, v);

    if (mpz_tstbit(q, m)) {
      do_line(v, vd);
      element_add(Z, Z, P);
      if (m) {
        do_vertical(vd, v);
      }
    }
    m--;
  }

  mpz_clear(q);

  element_invert(vd, vd);
  element_mul(res, v, vd);

  element_clear(v);
  element_clear(vd);
  element_clear(Z);
  element_clear(a);
  element_clear(b);
  element_clear(c);
  element_clear(e0);
  element_clear(e1);
}