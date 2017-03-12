#include <ctime>
#include <cstdio>

#include "fecha.hpp"

using namespace std;

Fecha::Fecha(int d, int m, int a) : dia_(d),mes_(m),anno_(a)
{
    fecha_sistema(dia_,mes_,anno_);

    comprobarValidez();
}

Fecha::Fecha(const char* cad)
{
    if(sscanf(cad,"%d/%d/%d",&dia_,&mes_,&anno_) != 3){
        throw Invalida("[!] Error: Formato de fecha invalida.");
    }

    fecha_sistema(dia_,mes_,anno_);

    comprobarValidez();
}

const char* Fecha::cadena() const{
    const char* diasem[] = {"domingo","lunes","martes","miércoles","jueves","viernes","sábado"};
    const char* mes[] = {"enero","febrero","marzo","abril","mayo","junio","julio","agosto","septiembre","octubre","noviembre","diciembre"};
    static char res[36]; // La fecha más larga (36)

    struct tm tiempo_des={0,0,0,0,0,0,0,0,0};

    tiempo_des.tm_mday = dia_;
    tiempo_des.tm_mon = mes_ - 1;
    tiempo_des.tm_year = anno_ - 1900;

    // Normalizar tiempo con mktime: rellena el campo para saber que dia de la semana es.
    mktime(&tiempo_des);

    sprintf(res,"%s %d de %s de %d",diasem[tiempo_des.tm_wday],dia_,mes[mes_-1],anno_);

    return res;
}

void Fecha::fecha_sistema(int &d, int &m, int &a)
{
    // Si ninguno de los parámetros es 0, me lo ahorro. Cambio los valores por referencia.
    if (d == 0 || m == 0 || a == 0){

        time_t tiempo_calendario = time(nullptr);
        struct tm* tiempo_des = localtime(&tiempo_calendario);

        if (d == 0) d = tiempo_des->tm_mday;
        if (m == 0) m = tiempo_des->tm_mon + 1;
        if (a == 0) a = tiempo_des->tm_year + 1900;

    }
}

void Fecha::comprobarValidez() const
{
    // Dia entre 1 y el dia max del mes (para eso tengo una funcion: fun_dia)
	if(dia_ < 1 || dia_ > fun_dia(mes_,anno_)) throw Invalida("[!] Error: Dia invalido.");

	// Mes valido: 1-12
	if(mes_ < 1 || mes_ > 12) throw Invalida("[!] Error: Mes invalido.");

	// Anno valido: AnnoMinimo-AnnoMaximo
	if(anno_ < Fecha::AnnoMinimo || anno_ > Fecha::AnnoMaximo) throw Invalida("[!] Error: Anno invalido.");
}

// A partir de un mes (m) y aun anno (y) devuelve el día máximo de ese mes en ese anno. Si el mes no es válido devuelve 31 que es el máximo día de un mes.
int Fecha::fun_dia(int m, int y) const
{
    if(m==2){
        if(y%4 == 0 && (y%400 == 0 || y%100 != 0)) return 29; // anno bisiesto
        else return 28;
    }
    else{
        if((m>=1 && m<=12)){
            if(m<=7)
            {
                if(m%2 == 0) return 30;
                else return 31;
            }
            else
            {
                if(m%2 == 0) return 31;
                else return 30;
            }
        }
        else return 31; // Si el mes no está comprendido entre 1 y 12, devuelvo 31 (máximo día de un mes).
    }
}


Fecha& Fecha::operator+=(int d){
    if (d==0) return *this; // Si no hay nada que sumar, me lo ahorro.

    struct tm tiempo_des={0,0,0,0,0,0,0,0,0};

    tiempo_des.tm_mday = dia_+d;
    tiempo_des.tm_mon = mes_ - 1;
    tiempo_des.tm_year = anno_ - 1900;

    // Normalizar tiempo con mktime
    mktime(&tiempo_des);

    dia_=tiempo_des.tm_mday;
    mes_=tiempo_des.tm_mon+1;
    anno_=tiempo_des.tm_year+1900;

    comprobarValidez();

    return *this;
}
Fecha& Fecha::operator-=(int d){
    return (*this += -d);
}

Fecha Fecha::operator+(int d)const{
    Fecha temp = *this;
    temp += d;
    return temp;
}

Fecha Fecha::operator-(int d)const{
    Fecha temp = *this;
    temp += -d;
    return temp;
}

Fecha& Fecha::operator++(){
    return *this += 1;
}

Fecha& Fecha::operator--(){
    return *this += -1;
}

Fecha Fecha::operator++(int){
    Fecha temp = *this;
    *this += 1;
    return temp;
}

Fecha Fecha::operator--(int){
    Fecha temp = *this;
    *this += -1;
    return temp;
}

long int operator-(const Fecha &f1, const Fecha &f2)
{
    struct tm fecha1={0,0,0,0,0,0,0,0,0};
    struct tm fecha2={0,0,0,0,0,0,0,0,0};
    time_t time1;
    time_t time2;

	// Paso mis dos fechas a las estructuras fechas teniendo en cuenta el cambio del mes y el anno.
    fecha1.tm_mday=f1.dia();
    fecha1.tm_mon=f1.mes()-1;
    fecha1.tm_year=f1.anno()-1900;
    fecha2.tm_mday=f2.dia();
    fecha2.tm_mon=f2.mes()-1;
    fecha2.tm_year=f2.anno()-1900;

	// Formateo con mktime
    time1=mktime(&fecha1);
    time2=mktime(&fecha2);

	// La funcion difftime devuelve segundos que debo transformar en dias (dividir entre 24*60*60).
    return difftime(time1,time2)/(86400);
}


bool operator==(const Fecha &f1, const Fecha &f2) noexcept
{
    return (f1.dia() == f2.dia() && f1.mes() == f2.mes() && f1.anno() == f2.anno());
}

bool operator<(const Fecha &f1, const Fecha &f2) noexcept
{
    if(f1.anno() < f2.anno())
        return true;
    else if(f1.anno() == f2.anno() && f1.mes() < f2.mes())
        return true;
    else if(f1.anno() == f2.anno() && f1.mes() == f2.mes() && f1.dia() < f2.dia())
        return true;
    else
        return false;
}

bool operator!=(const Fecha &f1, const Fecha &f2) noexcept
{
    return !(f1==f2);
}

bool operator>(const Fecha &f1, const Fecha &f2) noexcept
{
    return f2<f1;
}
bool operator<=(const Fecha &f1, const Fecha &f2) noexcept
{
    return !(f1>f2);
}

bool operator>=(const Fecha &f1, const Fecha &f2) noexcept
{
    return (!(f1<f2));
}

ostream& operator<<(ostream& o, const Fecha& f){
    o << f.cadena();
    return o;
}

istream& operator>>(istream& i, Fecha& f){
    char c[11]={'\0'}; // Formato(11): xx/xx/xxxx
    i.width(11);
    i >> c;

    f=Fecha(c);

    return i;
}
