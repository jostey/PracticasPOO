#ifndef FECHA_HPP_
#define FECHA_HPP_

#include <iostream>

using namespace std;

class Fecha{
public:
	// Constantes AnnoMinimo y AnnoMaximo
    static const int AnnoMinimo = 1902;
    static const int AnnoMaximo = 2037;
	
	// Clase Invalida (Excepciones)
    class Invalida
	{
        public:
            Invalida(const char* msg):motivo(msg){}
            const char* por_que() const {return motivo;};
        private:
            const char* motivo;
	};
	
	// Constructor Fecha: dia, mes y anno
    explicit Fecha(int d=0, int m=0, int a=0);

	// Constructor Fecha: const char* cad
    Fecha(const char* cad);

	// Conversión a const char*
    const char* cadena() const;

	// Observadores de dia, mes y anno
    int dia() const noexcept;
    int mes() const noexcept;
    int anno() const noexcept;

    Fecha& operator+=(int d);
    Fecha& operator-=(int d);
    Fecha operator+(int d) const;
    Fecha operator-(int d) const;
    Fecha& operator++(); // preincremento
    Fecha operator++(int); // postincremento
    Fecha& operator--(); // preincremento
    Fecha operator--(int); // postincremento



private:
	int dia_, mes_, anno_;
	void comprobarValidez() const;
	int fun_dia(int m, int y) const;
	void fecha_sistema(int &d, int &m, int &a);
};

inline int Fecha::dia() const noexcept {return dia_;}
inline int Fecha::mes() const noexcept {return mes_;}
inline int Fecha::anno() const noexcept {return anno_;}

long int operator-(const Fecha &f1, const Fecha &f2);

bool operator==(const Fecha &f1, const Fecha &f2) noexcept;
bool operator!=(const Fecha &f1, const Fecha &f2) noexcept;
bool operator<(const Fecha &f1, const Fecha &f2) noexcept;
bool operator>(const Fecha &f1, const Fecha &f2) noexcept;
bool operator<=(const Fecha &f1, const Fecha &f2) noexcept;
bool operator>=(const Fecha &f1, const Fecha &f2) noexcept;

ostream& operator<<(ostream& o, const Fecha& f);
istream& operator>>(istream& i, Fecha& f);

#endif  //Fecha_HPP_
