#ifndef TARJETA_HPP_
#define TARJETA_HPP_

#include "fecha.hpp"
#include "usuario.hpp"

using namespace std;

class Usuario;

struct EsBlanco : public unary_function<char, bool> {
	bool operator()(const char& c) const {return isspace(c);} 
};

struct EsDigito : public unary_function<char, bool> {
	bool operator()(const char& c) const {return isdigit(c);}
};

// Numero
class Numero{
	public:
		enum Razon{LONGITUD, DIGITOS, NO_VALIDO};
     	class Incorrecto{
        	public:
				Incorrecto(Razon r) : razon_(r){}
	  			Razon razon()const{return razon_;}
        	private:
	 			Razon razon_;
    	};

     	explicit Numero(const Cadena& num);
     	operator const char*() const;
     
	private:
   		Cadena numero_;
  
};

bool operator<(const Numero& n1, const Numero& n2);

//Tarjeta

class Tarjeta{
	public:
		enum Tipo{VISA, Mastercard, Maestro, JCB, AmericanExpress};    
		Tarjeta(Tipo t, const Numero& n, Usuario& us, const Fecha& fecha);		
     
     	class Caducada{
        	public:
	  			Caducada(const Fecha& caduca) : caduca_(caduca){}
	  			Fecha cuando() const {return caduca_;}
        	private:
	  			Fecha caduca_;
     	};
		
		Tipo tipo() const {return tipo_;}
     	Numero numero() const {return num;}
     	Fecha caducidad() const {return caducidad_;}
     	Cadena titular_facial() const {return titular_facial_;}
     	const Usuario* titular() const {return titular_;}
   
     	void anula_titular();

		Tarjeta(const Tarjeta&) = delete;
     	Tarjeta operator=(const Tarjeta&) = delete;
     
     	~Tarjeta();
	private:
		Tipo tipo_;  
		Numero num;
		const Usuario* titular_;
		Fecha caducidad_;
		Cadena titular_facial_;  
};

ostream& operator<<(ostream& os, const Tarjeta& t);
ostream& operator<<(ostream& os, const Tarjeta::Tipo& tipo);
bool operator<(const Tarjeta& t1, const Tarjeta& t2);

#endif
