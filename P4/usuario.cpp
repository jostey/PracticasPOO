#include <random>
#include <unistd.h>
#include <iomanip>

#include "usuario.hpp"

using namespace std;

unordered_set<Cadena> Usuario::historial;

// Clave
Clave::Clave(const char* clave_nc)
{
	if(strlen(clave_nc) < 5) throw Incorrecta(CORTA);

	random_device r;
	uniform_int_distribution<size_t> d{0, 63};

	char vector[] {"ABCDEFGHIJKLMNOPQRSTUVWXYZ.0123456789/abcdefghijklmnopqrstuvwxyz"};
	char salt[] { vector[d(r)], vector[d(r)] };

	const char * const contras= crypt(clave_nc, salt);

	if(!contras) throw Incorrecta(ERROR_CRYPT);

	clave_=contras;

}

bool Clave::verifica(const char* clave_nc) const
{	
	return clave_ == crypt(clave_nc,clave_.c_str()) ;
}


// Usuario
Usuario::Usuario(const Cadena& id, Cadena nombre, const Cadena& apellidos, const Cadena& direccion, const Clave& clave) : id_(id), nombre_(nombre), apellidos_(apellidos), direccion_(direccion), clave_(clave) 
{
	if(!(historial.insert(id)).second) throw Id_duplicado(id);
}

void Usuario::compra(Articulo& a, unsigned int cant){
  	if(cant == 0) articulos_.erase(&a);
  	else articulos_[&a] = cant;
}
     
void Usuario::es_titular_de(Tarjeta& tar){
	if(this == tar.titular()) tarjetas_.insert(make_pair(tar.numero(), &tar));
}

void Usuario::no_es_titular_de(Tarjeta& tar){
  	tar.anula_titular();
  	tarjetas_.erase(tar.numero());
}
     
Usuario::~Usuario(){
  	for(Tarjetas::iterator i=tarjetas_.begin(); i != tarjetas_.end(); ++i){
    	i->second->anula_titular();
  	}
  
  	historial.erase(id_);
}

ostream& operator<<(ostream& os, const Usuario& u){
	os << u.id() << " " << "[" << u.clave_.clave().c_str()<< "]" << " " << u.nombre() << " " << u.apellidos() << endl;
	os << u.direccion() << endl;
	os << "Tarjetas:" << endl;
	for(Usuario::Tarjetas::const_iterator i = u.tarjetas().begin(); i != u.tarjetas().end(); ++i){
		os << *(i->second) << endl;
	}
  
  	return os;
}

ostream& mostrar_carro(ostream& os, const Usuario& u){
	os << "Carrito de compra de " << u.id() << " [Artículos: " << u.n_articulos() << "]" << endl;
	os << " Cant. Artículo" << endl;
	os << "===========================================================" << endl;
  
	for(Usuario::Articulos::const_iterator i = u.compra().begin(); i != u.compra().end(); ++i){
		os << i->second << " [" << i->first->referencia() << "] ";
    	os << "\"" << i->first->titulo() << "\"";
    	os << ", " << i->first->f_publi().anno() << ". ";
		os << fixed << setprecision(2) << i->first->precio() << " €" << endl;
	}
  
  	return os;
}
