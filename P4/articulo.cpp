#include <iomanip>

#include "articulo.hpp"

using namespace std;

Articulo::Articulo(const Autores& autores, const Cadena& ref, const Cadena& tit, const Fecha& f_p, double precio)
 : referencia_(ref), titulo_(tit), f_publi_(f_p), precio_(precio) {
	if (autores.empty()) throw Autores_vacios();

	autores_=autores;

}



ostream& operator<<(ostream& os, const Articulo& a) noexcept
{
	os << "[" << setw(3) << setfill('0') << a.referencia() << "] " << "\"" << a.titulo() << "\", " << "de ";
	

	for(auto i= a.autores().begin(); i != a.autores().end(); ++i){
		if (i == a.autores().begin()) os << (*i)->apellidos();
		else os << ", " << (*i)->apellidos();
	}
	
	os << ". " << a.f_publi().anno() << ". ";
	os << fixed << setprecision(2) << a.precio() << " €" << endl;

	a.impresion_especifica(os);

	return os;
}
