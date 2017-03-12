#ifndef USUARIO_HPP_
#define USUARIO_HPP_

#include <unordered_set>
#include <map>
#include <unordered_map>

#include "cadena.hpp"
#include "tarjeta.hpp"
#include "articulo.hpp"

class Tarjeta;
class Numero;

using namespace std;

class Clave{
	public:
		enum Razon{CORTA, ERROR_CRYPT};
		class Incorrecta{
			public:
				Incorrecta(Razon r) : razon_(r) {}
				Razon razon() const {return razon_;}
			private:
				Razon razon_;
		};
		Clave(const char* clave_nc);
		Cadena clave() const {return clave_;}
		bool verifica(const char* clave_nc) const;

	private:
		Cadena clave_;
};

class Usuario{
	public:
		Usuario(const Cadena& id, Cadena nombre, const Cadena& apellidos, const Cadena& direccion, const Clave& clave);

		typedef map<Numero, Tarjeta*> Tarjetas;
		typedef unordered_map<Articulo*, unsigned int> Articulos;

		class Id_duplicado{
        	public:
	  			Id_duplicado(const Cadena& id) : id_(id){}
	  			Cadena idd()const{return id_;}
        	private:
	  			Cadena id_;
      	};

		Usuario(const Usuario& us) = delete;
		Usuario& operator =(const Usuario& us) = delete;
		
		void es_titular_de(Tarjeta& tar);
     	void no_es_titular_de(Tarjeta& tar);
		const Cadena& id() const {return id_;}
     	const Cadena& nombre() const {return nombre_;}
     	const Cadena& apellidos() const {return apellidos_;}
     	const Cadena& direccion() const {return direccion_;}
     	const Tarjetas& tarjetas() const {return tarjetas_;}
		void compra(Articulo& a, unsigned int cant = 1);
     	const Articulos& compra() const {return articulos_;}
     	unsigned n_articulos() const {return articulos_.size();}

		friend ostream& operator<<(ostream& os, const Usuario& u);

		~Usuario();

	private:
		Cadena id_;
		Cadena nombre_;
		Cadena apellidos_;
		Cadena direccion_;

		Clave clave_;

		Tarjetas tarjetas_;
		Articulos articulos_;
     
     	static unordered_set<Cadena> historial;
};


ostream& mostrar_carro(ostream& os, const Usuario& u);
#endif

