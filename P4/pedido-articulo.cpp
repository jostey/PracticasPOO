#include <iomanip>

#include "pedido-articulo.hpp"

using namespace std;

// LineaPedido

LineaPedido::LineaPedido(double precio, unsigned stock): precio_venta_(precio), cantidad_(stock){}

ostream& operator<<(ostream& os, const LineaPedido& lp){
    os << fixed;
    os << setprecision(2) << lp.precio_venta() << " €";
    os << "	" << lp.cantidad();
    
    return os;
}

// Pedido_Articulo

void Pedido_Articulo::pedir(Pedido& p, Articulo& a, double precio, unsigned cant){
    pa[&p].insert(make_pair(&a,LineaPedido(precio,cant)));
    ap[&a].insert(make_pair(&p,LineaPedido(precio,cant)));
}

void Pedido_Articulo::pedir(Articulo& a, Pedido& p, double precio, unsigned cant){
    pedir(p,a,precio,cant);
}

Pedido_Articulo::Pedidos Pedido_Articulo::ventas(Articulo& a) {
	if(ap.find(&a) == ap.end()) 
	{
        Pedido_Articulo::Pedidos vacio;
		return vacio; 
	} else
		return ap.find(&a)->second;
}
            
void Pedido_Articulo::mostrarDetallePedidos(ostream& os){
     double precio=0;
     for(PedidoArticulo::const_iterator i = pa.begin(); i != pa.end(); ++i){
         os << "Pedido núm. " << i->first->numero();
         os << "\tCliente: " << i->first->tarjeta()->titular()->nombre() << " " << "\n";
         os << "Fecha: " << i->first->fecha() << i->second;
         
         precio = precio + i->first->total();
    }
    
    os << fixed;
    os << "TOTAL VENTAS: " << setprecision(2) << precio << " €" << endl;
}

void Pedido_Articulo::mostrarVentasArticulos(ostream& os){
    for(ArticuloPedido::const_iterator i = ap.begin(); i != ap.end(); ++i){
         os << "Ventas de " << "[" << i->first->referencia() << "] ";
         os << "\"" << i->first->titulo() << "\" \n" << i->second << endl;
    }
}

ostream& operator<<(ostream& os, const Pedido_Articulo::ItemsPedido& ip){
    double precio = 0;
    Pedido_Articulo::ItemsPedido::const_iterator i;
    
    os << "\n====================================================" << endl;
    os << " PVP \t Cant. \t Articulo \n";
    os << "====================================================" << endl;
    
    for(i = ip.begin(); i != ip.end(); ++i){
        os << " " << i->second.precio_venta() << "€\t";
        os << i->second.cantidad() << "\t";
        os << "[" << i->first->referencia() << "] ";
        os << "\"" << i->first->titulo() << "\" " << endl;
        
        precio = precio + i->second.precio_venta() * i->second.cantidad();
    }
    
    os << "===================================================" << endl;
    
    os << fixed;
    os << setprecision(2) << precio << " €" << endl;
    
    return os;
    
}
ostream& operator<<(ostream& os, const Pedido_Articulo::Pedidos& p){
    double precio = 0;
    unsigned total = 0;
    Pedido_Articulo::Pedidos::const_iterator i;
    
    os << "====================================================" << endl;
    os << " PVP \t Cant. \t Fecha venta \n";
    os << "====================================================" << endl;
    
    for(i = p.begin(); i != p.end(); ++i){
        os << " " << i->second.precio_venta() << "€\t";
        os << i->second.cantidad() << "\t";
        os << i->first->fecha() << endl;
        
        precio = precio + i->second.precio_venta() * i->second.cantidad();
        total = total + i->second.cantidad();
    }
    
    os << "====================================================" << endl;
    
    os << fixed;
    os << setprecision(2) << precio << " €\t" << total <<endl ;
 
    return os;
}
