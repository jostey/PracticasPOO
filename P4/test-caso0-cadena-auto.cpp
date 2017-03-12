/* $Id: test-caso0-cadena-auto.cpp 348 2016-05-11 19:23:33Z gerardo $
 *
 * Pruebas unitarias automáticas para la clase Cadena (P0, P1)
 *
 * ©2014 Antonio G.ª Dguez.
 * ©2015-16 el resto de profesores de POO
 */
#include "test-auto.hpp"

using namespace std;

namespace {
  bool bPrimera = true;
}

FCTMF_SUITE_BGN(test_cadena) {

  if (bPrimera) {
    bPrimera = false;
    cerr << "\n---------- PRÁCTICA 1: CADENA ----------\n" << endl;
  }

  FCT_TEST_BGN(Cadena - Ctor: repetir caracter ciertas veces) {
    const Cadena a(5, '*');
    fct_chk(strcmp(a.c_str(), "*****") == 0);
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Cadena - Ctor: entero - repite espacios) {
    const Cadena a(10);
    fct_chk(strcmp(a.c_str(), "          ") == 0);
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Cadena - Ctor: sin parametros - vacia) {
    const Cadena a;
    fct_chk_eq_str(a.c_str(), "");
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Cadena - Ctor: por copia de otra cadena) {
    const Cadena a(3);
    const Cadena b(a);
    fct_chk(is_copy_constructible<Cadena>::value);
    fct_chk_eq_str(b.c_str(), "   ");
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Cadena - Ctor: por copia de otra sub-Cadena) {
    const Cadena a("Casa de muchos, casa de sucios"), // fuente de copias
      b(a, 16, 4),				      // normal
      c(a, 16, Cadena::npos),			      // ¿Existe npos público?
      d(a, 0),					      // 2 parámetros [npos]
      e(a, 0, 0),				      // SubCadena vacía
      f(a, 16, 40);				      // tamaño muy grande
    fct_chk_eq_str(b.c_str(), "casa");
    fct_chk_eq_str(c.c_str(), "casa de sucios");
    fct_chk_eq_str(d.c_str(), a.c_str());
    fct_chk_eq_str(e.c_str(), "");
    fct_chk_eq_str(f.c_str(), "casa de sucios");
    // posición fuera de límites
    fct_chk_ex(out_of_range, const Cadena g(a, 40, 4));
  }
  FCT_TEST_END();
  
  FCT_TEST_BGN(Cadena - Ctor: de movimiento) {
    Cadena a(3);
    const char* c = a.c_str();
    const Cadena b = move(a);
    fct_chk(is_move_constructible<Cadena>::value);
    fct_chk_eq_str(b.c_str(), c);
    fct_chk_eq_str(b.c_str(), "   ");
    fct_chk_eq_int(b.length(), 3);
    fct_chk_empty_str(a.c_str());
    fct_chk_eq_int(a.length(), 0);
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Cadena - Ctor: desde cadena de bajo nivel) {
    const Cadena a("hola");
    fct_chk_eq_str(a.c_str(), "hola");
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Cadena - Ctor: N primeros caracteres de cadena de C) {
    const Cadena a("No te digo trigo por no llamarte Rodrigo", 16);
    const Cadena b("Apetekawn", 0); // vacía
    const Cadena c("Mary had a little lamb", 30); // entera
    fct_chk_eq_str(a.c_str(), "No te digo trigo");
    fct_chk_eq_str(b.c_str(), "");
    fct_chk_eq_str(c.c_str(), "Mary had a little lamb");
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Cadena - Asignar una cadena a otra) {
    Cadena a("adios");
    const Cadena b;
    a = b;
    fct_chk(is_copy_assignable<Cadena>::value);
    fct_chk_eq_str(a.c_str(), b.c_str());
  }
  FCT_TEST_END();
 
  FCT_TEST_BGN(Cadena - Asignar una cadena a otra: movimiento) {
    Cadena a("hola"), b("adios");
    const char* c = b.c_str();
    a = move(b);
    fct_chk(is_move_assignable<Cadena>::value);
    fct_chk_eq_str(a.c_str(), c);
    fct_chk_eq_str(a.c_str(), "adios");
    fct_chk_eq_int(a.length(), 5);
    fct_chk_empty_str(b.c_str());
    fct_chk_eq_int(b.length(), 0);
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Cadena - Asignar una cadena de bajo nivel) {
    Cadena a;
    a = "lembas";
    fct_chk_eq_str(a.c_str(), "lembas");
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Cadena - Observadora de longitud: cadena vacia) {
    const Cadena a;
    fct_chk_eq_int(a.length(), 0);
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Cadena - Observadora de longitud: cadena no vacia) {
    const char *s = "saludos cordiales";
    const Cadena a(s);
    fct_chk_eq_int(a.length(), strlen(s));
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Cadena - Concatenacion: con +=) {
    Cadena a("cram");
    a += Cadena(" y lembas");
    fct_chk_eq_str(a.c_str(), "cram y lembas");
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Cadena - Concatenacion: con +) {
    const Cadena a("Cada cual,"), b(" en su corral");
    Cadena c = a + b;
    fct_chk_eq_str((a + b).c_str(), "Cada cual, en su corral");
    fct_chk_eq_int(c.length(), a.length() + b.length());
  }
  FCT_TEST_END();
 
  FCT_TEST_BGN(Cadena - Comparacion: igualdad) {
    fct_chk(Cadena("hola") == "hola");
    fct_chk(!("hola" == Cadena("adios")));
    fct_chk(!(Cadena("hola") == Cadena("holas")));
    fct_chk(!(Cadena("holas") == Cadena("hola")));
  }
  FCT_TEST_END();
  
  FCT_TEST_BGN(Cadena - Comparacion: desigualdad) {
    fct_chk(!("hola" != Cadena("hola")));
    fct_chk(Cadena("hola")  != "adios");
    fct_chk(Cadena("hola")  != Cadena("holas"));
    fct_chk(Cadena("holas") != Cadena("hola"));
  }
  FCT_TEST_END();
  
  FCT_TEST_BGN(Cadena - Comparacion: menor que) {
    fct_chk(Cadena("") < "x");
    fct_chk("a" < Cadena("b"));
    fct_chk(!(Cadena("ab") < Cadena("ab")));
    fct_chk(!(Cadena("ca") < Cadena("aa")));
  }
  FCT_TEST_END();
  
  FCT_TEST_BGN(Cadena - Comparacion: menor o igual que) {
    fct_chk("" <= Cadena("x"));
    fct_chk(Cadena("a") <= "b");
    fct_chk(Cadena("ab") <= Cadena("ab"));
    fct_chk(!(Cadena("ca") <= Cadena("aa")));
  }
  FCT_TEST_END();
  
  FCT_TEST_BGN(Cadena - Comparacion: mayor que) {
    fct_chk(!(Cadena("") > "x"));
    fct_chk(!("a" > Cadena("b")));
    fct_chk(!(Cadena("ab") > Cadena("ab")));
    fct_chk(Cadena("ca") > Cadena("aa"));
  }
  FCT_TEST_END();
  
  FCT_TEST_BGN(Cadena - Comparacion: mayor o igual que) {
    fct_chk(!("" >= Cadena("x")));
    fct_chk(!(Cadena("a") >= "b"));
    fct_chk(Cadena("ab") >= Cadena("ab"));
    fct_chk(Cadena("ca") >= Cadena("aa"));
  }
  FCT_TEST_END();
  
  FCT_TEST_BGN(Cadena - Leer caracter: [] y posicion valida) {
    const Cadena a("abcd");
    fct_chk(a.operator[](0) == 'a');
    fct_chk(a.operator[](3) == 'd');
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Cadena - Leer caracter: [] y posicion no valida) {
    const Cadena a;
    a.operator[](0);
    fct_chk(true);
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Cadena - Leer caracter: at y posicion valida) {
    const Cadena a("abcd");
    fct_chk(a.at(0) == 'a');
    fct_chk(a.at(3) == 'd');
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Cadena - Leer caracter: at y posicion no valida) {
    const Cadena a;
    fct_chk_ex(out_of_range, a.at(0));
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Cadena - Escribir caracter: []) {
    Cadena a("pava");
    a.operator[](2) = 'p';
    fct_chk_eq_str(a.c_str(), "papa");
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Cadena - Escribir caracter: at()) {
    Cadena a("Papa");
    a.at(2) = 'c';
    fct_chk_eq_str(a.c_str(), "Paca");
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Cadena - Subcadena: caso normal) {
    const Cadena a("eenie meenie moe");
    fct_chk_eq_str(a.substr(6, 6).c_str(), "meenie");
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Cadena - Subcadena: posicion inicial mayor que longitud) {
    const Cadena a("0123456789");
    fct_chk_ex(out_of_range, a.substr(10, 10));
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Cadena - Subcadena: posicion inicial negativa) {
    const Cadena a("0123456789");
    fct_chk_ex(out_of_range, a.substr(-1, 10));
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Cadena - Subcadena: tamanno mayor que longitud restante) {
    const Cadena a("0123456789");
    fct_chk_ex(out_of_range, a.substr(9, 2));
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Cadena - Subcadena: tamanno negativo) {
    const Cadena a("0123456789");
    fct_chk_ex(out_of_range, a.substr(9, -1));
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Cadena - Subcadena: tamanno excesivo) {
    const Cadena a("0123456789");
    fct_chk_ex(out_of_range, a.substr(9, 1000));
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Cadena - Extraccion: entrada vacia) {
    istringstream is("");
    Cadena c("algo");
    is >> c;
    fct_chk_empty_str(c.c_str());
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Cadena - Extraccion: solo espacios) {
    istringstream is(" \t\r\n\v");
    Cadena c("algo");
    is >> c;
    fct_chk_empty_str(c.c_str());
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Cadena - Extraccion: hasta fin de entrada) {
    istringstream is("prueba");
    Cadena c;
    is >> c;
    fct_chk_eq_str(c.c_str(), "prueba");
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Cadena - Extraccion: una palabra) {
    istringstream is("bueno bonito barato");
    Cadena c;
    is >> c;
    fct_chk_eq_str(c.c_str(), "bueno");
    fct_chk(is.peek() == ' ');
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Cadena - Extraccion: una palabra con espaciado inicial) {
    istringstream is("   hola adiós");
    Cadena c;
    is >> c;
    fct_chk_eq_str(c.c_str(), "hola");
    fct_chk(is.peek() == ' ');
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Cadena - Insercion) {
    ostringstream os;
    const char *s = "probando";
    const Cadena c(s);
    os << c;
    fct_chk_eq_str(os.str().c_str(), s);
  }
  FCT_TEST_END();
  
  FCT_TEST_BGN(Cadena - Iterador: begin()) {
    Cadena a("hola");
    *a.begin() = ' ';
    fct_chk(a.begin() == a.c_str());
    fct_chk(a[0] == ' ');
  }
  FCT_TEST_END();
  
  FCT_TEST_BGN(Cadena - Iterador: end()) {
    Cadena a("hola");
    fct_chk(a.end() == a.c_str() + 4);
  }
  FCT_TEST_END();
  
  FCT_TEST_BGN(Cadena - Iterador: begin() const) {
    const Cadena a("hola");
    fct_chk(a.begin() == a.c_str());
  }
  FCT_TEST_END();
  
  FCT_TEST_BGN(Cadena - Iterador: end() const) {
    const Cadena a("hola");
    fct_chk(a.end() == a.c_str() + 4);
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Cadena - Iterador: rbegin()/rend()) {
    Cadena a("hola");
    fct_chk(*a.rbegin() == 'a');
    fct_chk(*a.rend().base() == 'h');
  } FCT_TEST_END();

  FCT_TEST_BGN(Cadena - Iteradores: recorridos directos) {
    Cadena a("hola");
    for (Cadena::iterator i = a.begin(); i != a.end(); ++i)
      (*i)++;
    fct_chk(a == "ipmb");
    const Cadena b(a);
    ostringstream os;
    for (Cadena::const_iterator i = b.begin(); i != b.end(); ++i)
      os << *i;
    fct_chk(os.str() == "ipmb");
    for (auto i = a.cbegin(); i != a.cend(); ++i) os << *i;
    fct_chk(os.str() == "ipmbipmb");
  }
  FCT_TEST_END();

  FCT_TEST_BGN(Cadena - Iteradores: recorridos inversos) {
    const Cadena a("hola");
    Cadena b(a);
    ostringstream os;
    for (Cadena::reverse_iterator r = b.rbegin(); r != b.rend(); ++r)
      os << ++*r;
    fct_chk(os.str() == "bmpi");
    for (Cadena::const_reverse_iterator r = a.rbegin(); r != a.rend(); ++r)
      os << *r;
    fct_chk(os.str() == "bmpialoh");
    for (auto r = a.crbegin(); r != a.crend(); ++r) os << *r;
    fct_chk(os.str() == "bmpialohaloh");
  }
  FCT_TEST_END();
  
}
FCTMF_SUITE_END()
