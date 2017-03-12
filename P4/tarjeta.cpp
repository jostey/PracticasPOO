#include <iomanip>
#include <algorithm>

#include "tarjeta.hpp"

using namespace std;

// Numero
bool luhn(const Cadena& numero, size_t n);

Numero::Numero(const Cadena& num){
	unsigned n=0;
	Cadena nres(num);

	Cadena::iterator ibegin=nres.begin();
	Cadena::iterator iend=nres.end();

	iend=remove_if(ibegin, iend, EsBlanco());
	n=distance(ibegin, iend); // Necesito saber la posición de iend
	nres[n]='\0';

	if(find_if(ibegin, iend, not1(EsDigito())) != iend) throw Incorrecto(DIGITOS);
	
	if(n < 13 || n > 19) throw Incorrecto(LONGITUD);

	if(!luhn(nres,n)) throw Incorrecto(NO_VALIDO);
	else numero_=nres;
}

Numero::operator const char*() const{
  return numero_.c_str();
}

bool operator <(const Numero& n1, const Numero& n2){
  return (strcmp(n1,n2) < 0);
  
}

// Tarjeta

Tarjeta::Tarjeta(Tarjeta::Tipo t, const Numero& n, Usuario& us, const Fecha& fecha) : tipo_(t), num(n), titular_(&us), caducidad_(fecha){ 
  	if(Fecha() > fecha) throw Tarjeta::Caducada(fecha);

	titular_facial_ = us.nombre();
	titular_facial_ += " ";
	titular_facial_ += us.apellidos();
  
  	us.es_titular_de(*this);
}
     
void Tarjeta::anula_titular(){
	titular_ = nullptr;
}
     
Tarjeta::~Tarjeta(){
	Usuario* us = const_cast<Usuario*>(titular_);
	if(us) us->no_es_titular_de(*this);
}

ostream& operator<<(ostream& os, const Tarjeta& t){
	os << t.tipo() << endl;
	os << t.numero() << endl;
	os << t.titular_facial() << endl;
	os << setfill('0');
	os << "Caduca: " << setw(2) << t.caducidad().mes() << "/" << t.caducidad().anno() % 100 << endl;
  
	return os;
}

ostream& operator<<(ostream& os, const Tarjeta::Tipo& tipo){
	switch(tipo){
		case 0: os << "VISA"; break;
		case 1: os << "Mastercard"; break;
		case 2: os << "Maestro"; break;
		case 3: os << "JCB"; break;
		case 4: os << "AmericanExpress"; break;
	}

	return os;
}

bool operator<(const Tarjeta& t1, const Tarjeta& t2){
	return t1.numero() < t2.numero();
}
