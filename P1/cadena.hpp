#ifndef CADENA_HPP_
#define CADENA_HPP_

#include <iostream>
#include <cstring>
#include <stdexcept>

using namespace std;

class Cadena{
public:
	// Tipos definidos (iteradores)
    typedef char* iterator;
    typedef const char* const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
	
	// Constante npos
    static const size_t npos = -1;
	
	// Constructor Cadena: tam y c de relleno
    explicit Cadena(size_t tam=0, const char c=' ');
	
	// Constructor Cadena: cadena bajo nivel (y tam)
    Cadena(const char* cad);
    Cadena(const char* cad, size_t tam);

	// Constructor Cadena: Cadena, inicio y tam
    Cadena(const Cadena &c, size_t inicio, size_t tam=npos);
	
	// Constructor de Copia
	Cadena(const Cadena& c);
	// Constructor de Movimiento
	Cadena(Cadena&& c);
	// Operador "="
    Cadena& operator=(const Cadena& c);
	// Operador "=" de Movimiento
    Cadena& operator=(Cadena&& c);

	// Operador "+="
	Cadena& operator+=(const Cadena& cad);

	// Conversión  a const char*
    const char* c_str() const;
	// Observador de longitud (length())
    size_t length() const noexcept {return tam_;}
	// Operador "[]" de Lectura y Escritura
	char operator [] (size_t ind) const noexcept{return s_[ind];}
    char& operator [] (size_t ind) noexcept {return s_[ind];}
	// Operador "at" de Lectura y Escritura
	char at(size_t ind) const;
    char& at(size_t ind);
	// Método subcadena (substr)
    Cadena substr(size_t ind, size_t tam) const;	
	
	// Iteradores de Cadena
    iterator begin(){return s_;}
    const_iterator begin() const {return s_;}
    const_iterator cbegin() const {return s_;}
    iterator end() {return s_+tam_;}
    const_iterator end() const {return s_+tam_;}
    const_iterator cend() const {return s_+tam_;}

    reverse_iterator rbegin() noexcept;
    const_reverse_iterator rbegin() const noexcept;
    const_reverse_iterator crbegin() const noexcept;
    reverse_iterator rend() noexcept;
    const_reverse_iterator rend() const noexcept;
    const_reverse_iterator crend() const noexcept;
	
	// Destructor de Cadena
    ~Cadena();

private:
    char *s_;
    size_t tam_;
};

// Operadores de Comparación
bool operator==(const Cadena& c1, const Cadena& c2) noexcept;
bool operator!=(const Cadena& c1, const Cadena& c2) noexcept;
bool operator>(const Cadena& c1, const Cadena& c2) noexcept;
bool operator<=(const Cadena& c1, const Cadena& c2) noexcept;
bool operator<(const Cadena& c1, const Cadena& c2) noexcept;
bool operator>=(const Cadena& c1, const Cadena& c2) noexcept;

// Operador "+"
Cadena operator+(const Cadena &c1, const Cadena &c2);

// Operadores de entrada y salida de flujo
ostream& operator<<(ostream& o, const Cadena& c);
istream& operator>>(istream& i, Cadena& c);

// Definición de iteradores inline
inline Cadena::reverse_iterator Cadena::rbegin() noexcept
{
    return reverse_iterator(end());
}
inline Cadena::const_reverse_iterator Cadena::rbegin() const noexcept
{
    return const_reverse_iterator(end());
}
inline Cadena::const_reverse_iterator Cadena::crbegin() const noexcept
{
    return const_reverse_iterator(cend());
}

inline Cadena::reverse_iterator Cadena::rend() noexcept
{
    return reverse_iterator(begin());
}
inline Cadena::const_reverse_iterator Cadena::rend() const noexcept
{
    return const_reverse_iterator(begin());
}
inline Cadena::const_reverse_iterator Cadena::crend() const noexcept
{
    return const_reverse_iterator(cbegin());
}

// Hash
namespace std {
	template <> struct hash<Cadena> {
		size_t operator()(const Cadena& cad) const { // conversión const char* ->string
			return hash<string>{}(cad.c_str());
		}
	};
}


#endif // CADENA_HPP_
