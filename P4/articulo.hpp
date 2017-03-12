#ifndef ARTICULO_HPP_
#define ARTICULO_HPP_

#include <set>

#include "fecha.hpp"
#include "cadena.hpp"

using namespace std;

// Autor

class Autor{
	public:        
		Autor(const Cadena& nom, const Cadena& apell, const Cadena& dir) noexcept : nombre_(nom), apellidos_(apell), direccion_(dir){}
        
        const Cadena& nombre() const noexcept{return nombre_;}
        const Cadena& apellidos() const noexcept {return apellidos_;}
        const Cadena& direccion() const noexcept {return direccion_;}
        
	private:
     	Cadena nombre_;
		Cadena apellidos_;
		Cadena direccion_;
};

// Articulo

class Articulo{
	public:

		class Autores_vacios{};
     
		typedef set <Autor*> Autores;		

		Articulo(const Autores& autores, const Cadena& ref, const Cadena& tit, const Fecha& f_p, double precio);

		const Cadena& referencia() const noexcept {return referencia_;}
		const Cadena& titulo() const noexcept {return titulo_;}
		const Fecha& f_publi() const noexcept {return f_publi_;}
		double precio() const noexcept {return precio_;}
		double& precio() noexcept {return precio_;}
		const Autores& autores() const noexcept {return autores_;}

		virtual void impresion_especifica(ostream& os) const noexcept = 0; // Virtual puro -> Clase abstracta

		virtual ~Articulo() {}

	protected:
		Autores autores_;
		Cadena referencia_;
		Cadena titulo_;
		Fecha f_publi_;
		double precio_;
};

ostream& operator<<(ostream& os, const Articulo& a) noexcept;

class ArticuloAlmacenable : public Articulo{
	public:
		ArticuloAlmacenable(const Autores& autores, const Cadena& ref, const Cadena& tit, const Fecha& f_p, double precio, unsigned stock) : Articulo(autores, ref, tit, f_p, precio), stock_(stock) {}

		unsigned stock() const noexcept {return stock_;}
		unsigned& stock() noexcept {return stock_;}

	protected:
		unsigned stock_;
};

class Libro : public ArticuloAlmacenable{
	public:
		Libro(const Autores& autores, const Cadena& ref, const Cadena& tit, const Fecha& f_p, double precio, unsigned n_pag, unsigned stock) : ArticuloAlmacenable(autores, ref, tit, f_p, precio, stock), n_pag_(n_pag) {}

		unsigned n_pag() const noexcept {return n_pag_;}
		void impresion_especifica(ostream& os) const noexcept;
		
	private:
		unsigned n_pag_;
};
		

class Cederron : public ArticuloAlmacenable{
	public:
		Cederron(const Autores& autores, const Cadena& ref, const Cadena& tit, const Fecha& f_p, double precio, unsigned tam, unsigned stock) : ArticuloAlmacenable(autores, ref, tit, f_p, precio, stock), tam_(tam) {}
		
		unsigned tam() const noexcept {return tam_;}
		void impresion_especifica(ostream& os) const noexcept;
		
	private:
		unsigned tam_;
		
};

class LibroDigital : public Articulo{
	public:
		LibroDigital(const Autores& autores, const Cadena& ref, const Cadena& tit, const Fecha& f_p, double precio, const Fecha& f_expir) : Articulo(autores, ref, tit, f_p, precio), f_expir_(f_expir) {}

		Fecha f_expir() const noexcept {return f_expir_;}
		void impresion_especifica(ostream& os) const noexcept;

		
	private:
		Fecha f_expir_;
		
};

inline void Libro::impresion_especifica(ostream& os) const noexcept {
    os << '\t' << n_pag_ << " págs., " << stock_ << " unidades.";
}

inline void Cederron::impresion_especifica(ostream& os) const noexcept{
    os << '\t' << tam_ << " MB, " << stock_ << " unidades.";
}

inline void LibroDigital::impresion_especifica(ostream& os) const noexcept{
    os << "\tA la venta hasta el " << f_expir_ << '.';
}


#endif
