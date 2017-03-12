#include <iomanip>

#include "usuario-pedido.hpp"
#include "pedido-articulo.hpp"

using namespace std;


unsigned Pedido::N_pedidos = 0;

Pedido::Pedido(Usuario_Pedido& up, Pedido_Articulo& pa, Usuario& u, const Tarjeta& t, const Fecha& f) : num_(N_pedidos + 1), tarjeta_(&t), fecha_(f), total_(0){
    
    if(&u != t.titular()) throw Pedido::Impostor(&u);
    
    if(t.caducidad() < fecha_) throw Tarjeta::Caducada(t.caducidad());

	auto carrito=u.compra();

    for(auto it = carrito.begin(); it != carrito.end(); ++it){
		if(ArticuloAlmacenable* const art_a = dynamic_cast<ArticuloAlmacenable* const> (it->first) ){
        	if(art_a->stock() < (it->second)){
	 			Articulo *a = it->first;
	 			const_cast<Usuario::Articulos&>(u.compra()).clear();  
				throw Pedido::SinStock(a);
    		}     
  		}
		else if(LibroDigital* lib_d = dynamic_cast<LibroDigital* const> (it->first) )
			if(lib_d->f_expir() < Fecha()){
				Articulo *a = it->first;
				u.compra(*a,0);
			}
	}

	if(u.compra().empty()) throw Pedido::Vacio(&u);

	up.asocia(u,*this);

	carrito=u.compra();
  
  	for(auto it = carrito.begin(); it != carrito.end(); ++it){
     	total_ += it->first->precio() * it->second; //total = Sumatorio(Precio_Articulo * Cantidad)
     	pa.pedir(*this, *(it->first), it->first->precio(), it->second);
		
		if(ArticuloAlmacenable* const art_a = dynamic_cast<ArticuloAlmacenable* const> (it->first))
			art_a->stock() = art_a->stock()-it->second;

     	u.compra(*(it->first),0);
  	}

	N_pedidos++;
}

ostream& operator<<(ostream& os, const Pedido& p){
	os << "Núm. pedido: " << p.numero() << endl;
	os << "Fecha: " << p.fecha() << endl;
  	os << "Pagado con: " << p.tarjeta()->tipo() << " n.º: " << p.tarjeta()->numero() << endl;
  	os << "Importe: " << fixed << setprecision(2) << p.total() << " €";
  
  	return os;
}
