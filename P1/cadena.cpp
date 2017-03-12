#include "cadena.hpp"

using namespace std;

Cadena::Cadena(size_t tam, char c): tam_(tam)
{
    s_=new char[tam+1];

    for(size_t i=0;i<tam;i++) s_[i]=c;
    s_[tam]='\0';
}

Cadena::Cadena(const char* cad) : tam_(strlen(cad))
{
    s_=new char[tam_+1];

    strcpy(s_,cad);
}

Cadena::Cadena(const char* cad, size_t tam)
{
    if(tam > strlen(cad)) tam=strlen(cad);
    tam_=tam;
    s_=new char[tam+1];

    for(size_t i=0;i<tam;i++) s_[i]=cad[i];
    s_[tam]='\0';
}

Cadena::Cadena(const Cadena &c, size_t inicio, size_t tam) : tam_(tam)
{
    if(inicio > c.tam_ - 1) throw out_of_range("[!] Error: Fuera de rango.");
    if(tam == Cadena::npos) tam_ = c.tam_ - inicio;
    if(tam > c.tam_ - inicio) tam_ = c.tam_ - inicio;
	
    s_ = new char[tam_ + 1];
    for(size_t i=0;i<tam_;i++) s_[i] = c.s_[i + inicio];
    s_[tam_]='\0';
}

Cadena::Cadena(const Cadena& c)
{
    tam_ = c.tam_;
    s_=new char[tam_+1];

    strcpy(s_, c.s_);
}

Cadena& Cadena::operator=(const Cadena& c)
{
	if(&c!=this){
    	tam_=c.tam_;

    	delete[] s_;
    	s_=new char[tam_+1];

    	strcpy(s_, c.s_);
	}

    return *this;

}
Cadena::Cadena(Cadena&& c) : s_(c.s_), tam_(c.tam_)
{
	c.s_= nullptr;
	c.tam_=0;
	
}

Cadena& Cadena::operator=(Cadena&& c)
{
    if(&c!=this){
        s_=c.s_;
        tam_=c.tam_;
  	    c.s_= nullptr;
	    c.tam_=0;
    }
    return *this;
}

const char* Cadena::c_str() const
{
    return s_;
}

Cadena& Cadena::operator +=(const Cadena& cad) {
	char* tmp = new char[tam_ + 1];
	strcpy(tmp, s_);

	tam_ += cad.tam_;
	delete[] s_;
	s_ = new char[tam_ + 1];

	strcpy(s_, tmp);
	strcat(s_, cad.s_);
	return *this;
}

Cadena operator+(const Cadena &c1, const Cadena &c2){
    Cadena cad(c1);
    cad+=c2;

    return cad;
}

bool operator==(const Cadena& c1, const Cadena& c2) noexcept{
    return (strcmp(c1.c_str(), c2.c_str()) == 0);
}

bool operator!=(const Cadena& c1, const Cadena& c2) noexcept{
    return !(c1 == c2);
}

bool operator>(const Cadena& c1, const Cadena& c2) noexcept{
    return (strcmp(c1.c_str(), c2.c_str()) > 0);
}

bool operator<=(const Cadena& c1, const Cadena& c2) noexcept{
    return !(c1 > c2);
}

bool operator<(const Cadena& c1, const Cadena& c2) noexcept{
    return !(c1 > c2) && !(c1 == c2);
}

bool operator>=(const Cadena& c1, const Cadena& c2) noexcept{
    return !(c1 < c2);
}

char Cadena::at(size_t ind) const
{
	if(ind >= tam_) throw out_of_range("[!] Error: Fuera de rango (Indice)");
	else return s_[ind];
}

char& Cadena::at(size_t ind)
{
	if(ind >= tam_) throw out_of_range("[!] Error: Fuera de rango (Indice)");
	else return s_[ind];
}

Cadena Cadena::substr(size_t ind, size_t tam) const
{
	if(tam > (tam_ - ind)) throw out_of_range("[!] Error: Fuera de rango (Tamaño)");
	else if(ind >= tam_) throw out_of_range("[!] Error: Fuera de rango (Indice)");
	else{
		size_t i=0;
		char* temp=new char[tam+1];
		while(i<tam){
			temp[i]=this->s_[ind];
			ind++;
			i++;
		}
		temp[tam]='\0';
		return Cadena(temp);
	}
}

ostream& operator<<(ostream& o, const Cadena& c){
    o << c.c_str();
    return o;
}
istream& operator>>(istream& i, Cadena& c){
    char cad[32]={'\0'};
    i.width(32);

    i >> cad;

    Cadena temp(cad);
    c=temp;

    return i;
}


Cadena::~Cadena()
{
    delete[] s_;
    tam_ = 0;
}
