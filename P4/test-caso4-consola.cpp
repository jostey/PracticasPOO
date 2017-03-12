/* $Id: test-caso4-consola.cpp 334 2016-04-24 20:05:48Z gerardo $
 * Prueba del SGL, caso de uso 4
 *
 * LIBRERÍA
 * ©2009-16 los profesores de POO
 */

#include <cstdlib>
#include <exception>
#include <iostream>
#include <locale>
#include "usuario.hpp"
#include "tarjeta.hpp"
#include "articulo.hpp"
#include "pedido.hpp"
#include "usuario-pedido.hpp"
#include "pedido-articulo.hpp"

#define AZUL  "\033[0m\033[01;34m"
#define NORMAL "\033[0m"

using std::cout;
using std::endl;

int main()
{
  cout.imbue(std::locale(""));
  // std::set_terminate(__gnu_cxx::__verbose_terminate_handler);
  // Constructor de usuario: id, nombre, apellidos, dirección, clave
  Usuario 
    lucas("lucas", "Lucas", "Grijánder", 
	  "Avda. Generalísimo, 654 (Barbate)", "vaeg0Quo"),
    krispin("krispin", "Crispín", "Klánder", 
	    "C/ del Jurel, 876 (Barbate)", "Pheaj9li"),
    sanani("sanani", "Sanani", "de las Escobas", 
	   "Callejón de los negros, s/n (Cádiz)", "ahxo2Aht"),
    rosa("rosa", "Rosita", "de Jericó",
	 "C/ de Sor Sofía, 345 (2 Hermanas)", "Roh9aa5v");
  // Constructor de Tarjeta: Tipo, Numero, Usuario, fecha de caducidad
  // Para evitar problemas con las fechas y sus caducidades, ponemos año 0
  // (el actual) y sumamos uno o más años.
  using Tipo = Tarjeta::Tipo;
  Tarjeta
    visa1(Tipo::VISA, Numero("4539 4512 0398 7356"),
          lucas, Fecha(31,12,0) + 3*365),
    american_express(Tipo::AmericanExpress, Numero("378282246310005"),
                     sanani, Fecha(30,11,0) + 4*365),
    dinners_club(Tipo::Mastercard, Numero("30569309025904"),
                 rosa, Fecha(31,7) + 5*365),
    mastercard(Tipo::Mastercard, Numero("555555555555 4444  "),
               krispin, Fecha(31,1) + 3*365),
    australian_bank_card(Tipo::Maestro, Numero("5610591081018250"),
               krispin, Fecha(28,2)+ 365),
    jcb(Tipo::VISA, Numero("3530111333300000"), rosa, Fecha("31/7/0") + 2*365),
    visa2(Tipo::VISA, Numero(" 4222222222222"), lucas, Fecha("28/2/0") + 365);
   
  // Constructor de Autor: nombre, apellidos, dirección
  Autor 
    Pepe("Pepe", "Leches", "Avda. del Pollo, 55 (Barbate)"),
    Marchena("Marchena", "Picuito", "Callejón de los Negros, 5 (Ubrique)"),
    Carlos("Carlos", "El Legionario", "C/M.ª Arteaga, 155 (Bornos)"),
    Carmela("La Uchi", "Poyáquez", "Callejón Circo, 45 (Cádiz)");
  
  Articulo::Autores autores { &Marchena };
  
  // Constructor de Libro: autores, referencia, título, fecha publicación,
  //                       precio, páginas, stock
  Libro arte_insulto(autores, "100", "El arte del insulto",
	    "1/8/2000", 32.50, 245, 5); // autor = Marchena
  
  autores.insert(&Carmela);
  Libro montar_bici(autores, "110", "Cómo montar en bici sin sillín",
	    "20/6/1998)", 35.95, 650, 100); // autores = Marchena y Carmela
  autores.clear();
  
  autores.insert(&Pepe);
  Libro braille(autores, "111", "Aprenda Braille en 5 minutos",
	    "5/10/2002", 42.10, 150, 300); // autor = Pepe
  
  autores.insert(&Carlos);
  Libro baston(autores, "103", "Historia del bastón",
	    "1/2/2001", 39.95, 455, 4); // autores = Pepe y Carlos
  
  // Constructor de LibroDigital: autores, referencia, título,
  //                              fecha publicación, precio, fecha expiración
  LibroDigital  // autores = Pepe y Carlos
    eBook1(autores, "034", "Revisiones", "15/1/2009", 6., "1/7/2009");
  autores.clear();
  
  autores.insert(&Carmela);
  LibroDigital // autor = Carmela
    eBook2(autores, "035", "Horarios", "20/3/2009)", 
	   9., Fecha("20/6/00") + 365 * 4),
    eBook3(autores, "036", "Exámenes", "1/10/2007", 12., "30/9/2008"); // expdo.
  
  // Constructor de Cederron: autores, referencia, título, fecha publicación,
  //                          precio, megas, stock
  Cederron // autor = Carmela
    cd1(autores, "210", "Camela", "1/8/2000", 32.50, 245, 40);
  
  autores.insert(&Marchena);
  Cederron  // autores = Carmela y Marchena
    cd2(autores, "211", "Mecano nlaleche", "20/6/1998", 35.95, 650, 50);
  
  autores.insert(&Pepe);
  Cederron  // autores = Carmela, Marchena y Pepe
    cd3(autores, "220", "Jarabe de Falo", 
	"7/7/2007", 12.90, 547, 30);
  
  cout << "\n" AZUL "---------------INVENTARIO DE EXISTENCIAS-----------"
    NORMAL "\nLIBROS: \n" 
       << arte_insulto << '\n' << montar_bici << '\n' 
       << braille << '\n' << baston
       << "\nLIBROS DIGITALES: \n" 
       << eBook1 << '\n' << eBook2 << '\n' << eBook3
       << "\nCEDERRONES: \n" 
       << cd1 << '\n' << cd2 << '\n' << cd3 << endl;
  
  // Añadir y quitar artículos al carrito y ver el carrito
  // Comprar: Articulo, Cantidad (1 por omisión, 0 elimina el artículo)
  // Factura: Usuario_Pedido, Pedido_Articulo, Usuario, Tarjeta, Fecha [hoy]
  Usuario_Pedido usuario_ped;
  Pedido_Articulo pedido_art;
  
  cout << "\n\nCARRITOS VACÍOS\n\n";
  mostrar_carro(cout, lucas);
  mostrar_carro(cout, krispin);
  mostrar_carro(cout, sanani);
  mostrar_carro(cout, rosa);
  cout << endl;
  
  lucas.compra(montar_bici, 3);
  lucas.compra(braille, 1);
  lucas.compra(eBook1, 2);
  lucas.compra(cd2, 1);
  mostrar_carro(cout, lucas);
  Pedido p1(usuario_ped, pedido_art, lucas, visa1, Fecha(10, 3));
  
  cout << "*** Cambio de precio de MONTAR_BICI, BRAILLE, EBOOK1 y CD1\n\n";
  montar_bici.precio() = 29.95;	// cambiar precios
  braille.precio() = 44.50; 
  eBook1.precio() = 5.50;
  cd1.precio() = 24.05;
  
  krispin.compra(montar_bici, 2);
  krispin.compra(braille, 2);
  mostrar_carro(cout, krispin);
  cout << "\n*** krispin devuelve BRAILLE y compra ARTE_INSULTO y CD3\n";
  krispin.compra(braille, 0); // quita braille del carrito
  krispin.compra(arte_insulto);    // un ejemplar
  krispin.compra(cd3);
  mostrar_carro(cout, krispin);
  cout << "\n*** No se comprueban existencias\n"
    "*** existencias de BASTON = " << baston.stock() << endl;
  sanani.compra(baston, 6);	// no se comprueban las existencias
  cout << "\n*** No se comprueba la fecha de expiración\n"
    "*** fecha expir. eBook3 = " << eBook3.f_expir() << "\n\n";
  sanani.compra(eBook3, 2); // no se comprueba la fecha de expiración
  mostrar_carro(cout, sanani);
  cout << "\n*** sanani devuelve 4 BASTON\n";
  sanani.compra(baston, 2);	// quita 4 del carrito
  mostrar_carro(cout, sanani);
  Pedido p2(usuario_ped, pedido_art, sanani, american_express, 
	    --Fecha());	// pedido ayer
  Pedido p3(usuario_ped, pedido_art, krispin, mastercard, "5/4/2010");
  
  cout << "*** Cambio de precio de ARTE_INSULTO, BASTON y eBook2\n\n";
  arte_insulto.precio() = 35.20;	// cambiar precios
  baston.precio() = 34.9; 
  eBook2.precio() = 3.;
  
  rosa.compra(cd3, 2);
  rosa.compra(eBook2, 3);
  rosa.compra(braille);
  rosa.compra(cd2, 0); // no hace nada, cd2 no está en el carro
  mostrar_carro(cout, rosa);
  Pedido p4(usuario_ped, pedido_art, rosa, jcb, "5/4/2010");
  
  cout << "\n" AZUL "---------------------PEDIDOS-----------------------"
    NORMAL "\n" << p1 << endl << p2 << endl << p3 << endl << p4 << endl;
  
  // Recepción de mercancías
  arte_insulto.stock() += 5; 
  montar_bici.stock() *= 4; 
  braille.stock() += 2; 
  baston.stock() += 2;
  
  rosa.compra(braille); rosa.compra(baston);
  Pedido p5(usuario_ped, pedido_art, rosa, jcb); // pedido hoy
  rosa.compra(montar_bici, 3);
  Pedido p6(usuario_ped, pedido_art, rosa, dinners_club); // pedido hoy
  krispin.compra(baston); 
  krispin.compra(braille); 
  krispin.compra(arte_insulto, 3);
  Pedido p7(usuario_ped, pedido_art, krispin, 
	    australian_bank_card, "5/4/2010");
  sanani.compra(arte_insulto, 2); 
  sanani.compra(montar_bici);
  Pedido p8(usuario_ped, pedido_art, sanani, american_express, "5/4/2010");
  
  cout << p5 << endl << p6 << endl << p7 << endl << p8 << endl
    
       << "\n" AZUL "-------------------DETALLE DE PEDIDOS--------------"
    NORMAL "\n";
  pedido_art.mostrarDetallePedidos(cout);
  cout << "\n" AZUL "------------------VENTAS DE ARTÍCULOS--------------"
    NORMAL "\n";
  pedido_art.mostrarVentasArticulos(cout);
  
  cout << "\n" AZUL "-----------------PRUEBAS DE EXCEPCIONES------------"
    NORMAL "\n";
  try {
    Pedido nulo(usuario_ped, pedido_art, lucas, visa2);
  }
  catch(Pedido::Vacio& e) {
    std::cerr << "Error de pedido: el carrito de " << e.usuario().id() 
	      << " está vacío. " << endl;
  }
  try {
    krispin.compra(montar_bici);
    Pedido farsante(usuario_ped, pedido_art, krispin, visa2);
  }
  catch(Pedido::Impostor& e) {
    std::cerr << "Error de pedido: la tarjeta de pago no es de " 
	      << e.usuario().id() << endl;
  }
  try {
    rosa.compra(arte_insulto, 2); 
    rosa.compra(braille, 4); 
    rosa.compra(baston, 5);
    Pedido no_disponible(usuario_ped, pedido_art, rosa, dinners_club);
  }
  catch(Pedido::SinStock& e) {
    std::cerr << "Error de pedido: no hay existencias del artículo [" 
	      << e.articulo().referencia() << "]" << endl;
  }
  try {
    // Creamos una tarjeta que caducó ayer. ¿Qué pasa? ¡No somos un
    // maldito banco, esto es una prueba!
    Tarjeta caduca(Tipo::JCB, Numero("4222222222222"), lucas, Fecha() - 1); 
    lucas.compra(arte_insulto, 2);
    Pedido no_pagado(usuario_ped, pedido_art, lucas, caduca); // pedido hoy
  }
  catch(Tarjeta::Caducada& e) {
    std::cerr << "Error de pedido: tarjeta caducada el " << e.cuando()
	      << endl;
  }
  try {
    Articulo::Autores a;
    Libro anonimo(a, "133", "El Necronomicón", 
		  "10/3/2009", 35.00, 210, 4);
  }
  catch(Articulo::Autores_vacios) {
    std::cerr << "Error de artículo: No se puede crear uno anónimo.\n";
  }
  cout << "\nNúmero total de pedidos: " 
       << Pedido::n_total_pedidos() << '\n' << endl;
}

