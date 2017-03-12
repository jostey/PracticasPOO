/* $Id: catalogo_check.cpp 362 2016-05-31 15:13:24Z gerardo $
 * Programa para comprobar diversas buenas prácticas en el código fuente de P4
 * POO - P4 
 * ©2014-16 Pedro Delgado y los profesores de POO
 */

#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchersInternal.h"
#include "clang/ASTMatchers/ASTMatchersMacros.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"
#include "clang/AST/ASTContext.h"
#include "clang/Basic/SourceManager.h"
 
#include "llvm/Support/CommandLine.h"
#include "clang/Rewrite/Frontend/Rewriters.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include <fstream>
#include <sstream>
#include <cstdio>
#include <regex>

using namespace clang::tooling;
using namespace llvm;

using namespace clang;
using namespace clang::ast_matchers;
using namespace clang::ast_matchers::internal;

using namespace std;

namespace clang{
  namespace ast_matchers{
	
    //Zona para definir matchers respecto a métodos incluidos en las clases.
		
    AST_MATCHER(clang::FunctionDecl, isImplicitFunction) {
      return Node.isImplicit();
    }		
	
    AST_MATCHER(clang::CXXMethodDecl, isImplicitMethod) {
      return Node.isImplicit();
    }		

    AST_MATCHER(clang::CXXConstructorDecl, hasBodyConstructor) {
      return Node.hasBody();
    }

    AST_MATCHER(clang::CXXDestructorDecl, isImplicitDestructor) {
      return Node.isImplicit();
    }
	
    AST_MATCHER(clang::CXXDestructorDecl, hasBodyDestructor) {
      return Node.hasBody();
    }
	
    const internal::VariadicDynCastAllOfMatcher<Decl, TranslationUnitDecl> translationUnitDecl;
  }
}

/*****************************************************************************/

DeclarationMatcher Clase_Articulo_Matcher =
  translationUnitDecl(
		      allOf(
			    hasDescendant(
					  recordDecl(
						     hasName("Articulo")
						     ).bind("clase_articulo")
					  ),
			    hasDescendant(
					  recordDecl(
						     hasName("ArticuloAlmacenable")
						     ).bind("clase_articuloalmacenable")
					  ),
			    hasDescendant(
					  recordDecl(
						     hasName("Libro")
						     ).bind("clase_libro")
					  ),
			    hasDescendant(
					  recordDecl(
						     hasName("Cederron")
						     ).bind("clase_cederron")
					  ),
			    hasDescendant(
					  recordDecl(
						     hasName("LibroDigital")
						     ).bind("clase_librodigital")
					  )
			    )
		      );

DeclarationMatcher Clase_Autor_Matcher =
  recordDecl(
	     hasName("Autor")
	     ).bind("clase_autor");

DeclarationMatcher Clase_Tarjeta_Matcher =
  recordDecl(
	     hasName("Tarjeta")
	     ).bind("clase_tarjeta");

DeclarationMatcher Clase_Usuario_Matcher =
  recordDecl(
	     hasName("Usuario")
	     ).bind("clase_usuario");

DeclarationMatcher Clase_Pedido_Matcher =
  recordDecl(
	     hasName("Pedido")
	     ).bind("clase_pedido");

DeclarationMatcher Clase_PedidoArticulo_Matcher =
  translationUnitDecl(
		      allOf(
			    hasDescendant(
					  recordDecl(
						     hasName("Pedido_Articulo")
						     ).bind("clase_pedidoarticulo")
					  ),
			    hasDescendant(
					  recordDecl(
						     hasName("LineaPedido")
						     ).bind("clase_lineapedido")
					  )
			    )
		      );

DeclarationMatcher Clase_UsuarioPedido_Matcher =
  recordDecl(
	     hasName("Usuario_Pedido")
	     ).bind("clase_usuariopedido");

DeclarationMatcher One_Constructor_Usuario_Matcher =
  recordDecl(
	     forEach(
		     constructorDecl(hasBodyConstructor(), unless(isImplicit())).bind("one_constructor")
		     ),
	     hasName("Usuario")
	     );

DeclarationMatcher One_Constructor_Pedido_Matcher =
  recordDecl(
	     forEach(
		     constructorDecl(
				     hasBodyConstructor(), 
				     unless(isImplicit())
				     ).bind("constructor_pedido")
		     ),
	     hasName("Pedido")
	     );

DeclarationMatcher Actualiza_Numero_Pedidos_Matcher =
  constructorDecl(
		  hasDescendant(
				compoundStmt(
					     has(
						 stmt(
						      anyOf(
							    unaryOperator(
									  hasOperatorName("++"),
									  has(
									      declRefExpr(
											  to(
											     varDecl(
												     hasName("N_pedidos")
												     )
											     )
											  )
									      )   
									  ),		   
							    binaryOperator(
									   hasOperatorName("="),
									   hasLHS(
										  declRefExpr(
											      to(
												 varDecl(
													 hasName("N_pedidos")
													 )
												 )
											      )
										  ),
									   hasRHS(
										  binaryOperator(
												 hasOperatorName("+"),
												 hasLHS(
													has(
													    declRefExpr(
															to(
															   varDecl(
																   hasName("N_pedidos")
																   )
															   )
															)
													    )  
													),
												 hasRHS(integerLiteral(equals(1)))
												 )
										  )
									   ),
							    binaryOperator(
									   hasOperatorName("="),
									   hasRHS(
										  binaryOperator(
												 hasOperatorName("+"),
												 hasRHS(
													has(
													    declRefExpr(
															to(
															   varDecl(
																   hasName("N_pedidos")
																   )
															   )
															)
													    )
													),
												 hasLHS(integerLiteral(equals(1)))
												 )
										  ),
									   hasLHS(
										  declRefExpr(
											      to(
												 varDecl(
													 hasName("N_pedidos")
													 )
												 )
											      )
										  )
									   ),
							    binaryOperator(
									   hasOperatorName("+="),
									   hasLHS(
										  declRefExpr(
											      to(
												 varDecl(
													 hasName("N_pedidos")
													 )
												 )
											      )
										  ),
									   hasRHS(integerLiteral(equals(1)))  
									   )
							    )
						      ).bind("stmt_found")	
						 )
					     ).bind("constructor_pedido_compSt")
				),
		  ofClass(
			  recordDecl(
				     hasName("Pedido")
				     )
			  )
		  );

DeclarationMatcher One_Constructor_LineaPedido_Matcher =
  recordDecl(
	     forEach(
		     constructorDecl(hasBodyConstructor(), unless(isImplicit())).bind("constructor_lineapedido")
		     ),
	     hasName("LineaPedido")
	     );

DeclarationMatcher Non_Constructor_Pedido_Matcher =
  recordDecl(
	     has(
		 constructorDecl(hasBodyConstructor(), unless(isImplicit())).bind("const_pedidoarticulo")
		 ),
	     hasName("Pedido_Articulo")
	     );

DeclarationMatcher Funciones_Estandar_Matcher =
  functionDecl(
	       unless(isImplicitFunction()),
	       forEachDescendant(
				 declRefExpr(
					     to(
						functionDecl(
							     anyOf(
								   hasName("strlen"),
								   hasName("strcat"),
								   hasName("memset"),
								   hasName("strcpy"),
								   hasName("strcmp")
								   )
							     ).bind("method_cstring")
						)
					     )
				 )
	       );

DeclarationMatcher Friend_Matcher =
  recordDecl(
	     hasDescendant(
			   friendDecl(                                
				      unless(
					     has(
						 functionDecl(
							      hasName("operator<<"),
							      hasAncestor(recordDecl(hasName("Usuario")))
							      ).bind("friend_operator_usuario")
						 )	
					     )
								      )
			   )
	     ).bind("friend_class");

DeclarationMatcher Friend_Operator_Usuario_Matcher = 
  recordDecl(
	     hasDescendant(
			   friendDecl(
				      has(
					  functionDecl(
						       hasName("operator<<")
						       ).bind("friend_operator_usuario")
					  )
				      )
			   )
	     );

DeclarationMatcher Private_Fields_Matcher =
  recordDecl(
	     forEach(
		     fieldDecl().bind("field_decl")
		     )
	     ).bind("clase_atributo");

DeclarationMatcher Non_Member_Pedido_Matcher =
  methodDecl(
	     hasOverloadedOperatorName("<<"),
	     unless(isImplicitMethod()),
	     ofClass(
		     recordDecl(
				hasName("Pedido"),
				unless(isTemplateInstantiation())
				)
		     )
	     ).bind("member_operator_pedido");

DeclarationMatcher Tarjeta_Const_Matcher =
  recordDecl(
	     forEach(
		     fieldDecl().bind("tarjeta_const")
		     ),
	     hasName("Pedido"), 
	     unless(isTemplateInstantiation())
	     );

DeclarationMatcher Non_Member_LineaPedido_Matcher =
  methodDecl(
	     hasOverloadedOperatorName("<<"),
	     unless(isImplicitMethod()),
	     ofClass(
		     recordDecl(
				hasName("LineaPedido"),
				unless(isTemplateInstantiation())
				)
		     )
	     ).bind("member_operator_lineapedido");

DeclarationMatcher Non_Member_PedidoArticulo_Matcher =
  methodDecl(
	     hasOverloadedOperatorName("<<"),
	     unless(isImplicitMethod()),
	     ofClass(
		     recordDecl(
				hasName("Pedido_Articulo"),
				unless(isTemplateInstantiation())
				)
		     )
	     ).bind("member_operator_pedidoarticulo");

DeclarationMatcher Overloaded_Pedido_Matcher =
  recordDecl(
	     has(
		 methodDecl(
			    hasName("pedir")
			    ).bind("metodo_pedir")
		 ),
	     hasName("Pedido_Articulo")
	     ).bind("clase_metodo_pedir");

DeclarationMatcher Unorderedset_Matcher =
  recordDecl(
    forEach(
      varDecl().bind("unorderedset_decl")
    ),
    hasName("Usuario")
  );

DeclarationMatcher AlgorithmFunctions_Matcher =
	functionDecl(
	       unless(isImplicitFunction()),
	       forEachDescendant(
				 declRefExpr(
					     to(
						functionDecl(
								    anyOf(
								   	hasName("find_if"),
								   	hasName("remove_if")
								   	)
							    ).bind("nameFunctionAlgorithm")
						)
					     ).bind("functionAlgorithm")
				 )
	       );

DeclarationMatcher Hash_Cadena_Matcher =
	classTemplateSpecializationDecl(
		hasTemplateArgument(0, refersToType(asString("class Cadena"))),
		hasName("hash")
	).bind("templateHash");

DeclarationMatcher Const_Autor_Matcher =
  recordDecl(
	     forEach(
		     methodDecl(
				unless(isImplicitMethod())	
				).bind("metodo_autor_no_const")
		     ),
	     hasName("Autor")
	     );

DeclarationMatcher NoExcept_Autor_Matcher =
  recordDecl(
	     forEach(
		     methodDecl(
				unless(isImplicitMethod())	
				).bind("metodo_autor_noexcept")
		     ),
	     hasName("Autor")
	     );

DeclarationMatcher Destructor_Virtual_Articulo_Matcher =
  recordDecl(
	     has( 
		 destructorDecl(
				unless(isImplicitDestructor()),
				hasBodyDestructor()
				).bind("destructor_virtual_articulo")
		  ),
	     hasName("Articulo")
	     );

DeclarationMatcher Articulo_NoStock_Matcher = 
  recordDecl(
	     has( 
		 fieldDecl(
			   hasName("stock_")
			   ).bind("stock_articulo")
		  ),
	     hasName("Articulo")
	     );

DeclarationMatcher ArticuloAlmacenable_Stock_Matcher =
  recordDecl(
	     has( 
		 fieldDecl(
			   hasName("stock_")
			   ).bind("stock_articuloalmacenable")
		  ),
	     hasName("ArticuloAlmacenable")
	     );

DeclarationMatcher DynamicCast_Pedido_Matcher =
  constructorDecl(
		  unless(isImplicit()),
		  forEachDescendant(		
				    dynamicCastExpr().bind("dynamic_constructor_pedido")
						),
		  ofClass(recordDecl(hasName("Pedido")))
		  );


DeclarationMatcher Llamada_Impesp_Matcher =
  functionDecl(
	       hasName("operator<<"),
	       hasDescendant(
			     memberExpr(
					hasDeclaration(
						       methodDecl(
								  hasName("impresion_especifica")
								  )
						       ),
					has(
					    declRefExpr(
							to(
							   parmVarDecl(
								       hasType(
									       references(
											  qualType(
												   hasCanonicalType(
														    recordType(
															       hasDeclaration(
																	      recordDecl(
																			 hasName("Articulo")
																			 )
																	      )
															       )
														    )
												   )
											  )
									       )
								       )
							   )
							)
					    )	
					)
			     )
	       ).bind("member_ext_articulo");

DeclarationMatcher Definicion_Impesp_Libro_Matcher =
  methodDecl(
	     hasName("impresion_especifica"),
	     forEachDescendant(
			       memberExpr(
					  hasDeclaration(fieldDecl().bind("field_impesp_libro"))
					  )
			       ),
	     ofClass(
		     recordDecl(
				hasName("Libro"),
				unless(isTemplateInstantiation())
				)
		     )
	     );

DeclarationMatcher Definicion_Impesp_Cederron_Matcher =
  methodDecl(
	     hasName("impresion_especifica"),
	     forEachDescendant(
			       memberExpr(
					  hasDeclaration(fieldDecl().bind("field_impesp_cederron"))
					  )
			       ),
	     ofClass(
		     recordDecl(
				hasName("Cederron"),
				unless(isTemplateInstantiation())
				)
		     )
	     );

DeclarationMatcher Definicion_Impesp_LibroDigital_Matcher =
  methodDecl(
	     hasName("impresion_especifica"),
	     forEachDescendant(
			       memberExpr(
					  hasDeclaration(fieldDecl().bind("field_impesp_librodigital"))
					  )
			       ),
	     ofClass(
		     recordDecl(
				hasName("LibroDigital"),
				unless(isTemplateInstantiation())
				)
		     )
	     );

/*****************************************************************************/

class CatalogoCheck : public MatchFinder::MatchCallback {
public :
  CatalogoCheck(): 
    clase_encontrada(false), 
    one_constructor(0), 
    constructor_correcto(false), 
    constante_tarjeta(false), 
    constructor_pedidoarticulo(false), 
    cstring(true), 	
    friend_member(false), 
    atributo_no_privado(false), 
    friend_operator_usuario(false), 
    explicit_specified(false),
    pedir_sobrecargado(false), 
    incrementa_pedidos(false),
    atributo_unorderedset(false), 
    templatehash(false),
    metodos_noexcept_autor(false), 
    metodos_constantes_autor(false), 
    destructor_virtual_articulo(false), 
    stock_articulo(false), 
    stock_articuloalmacenable(false), 
    dynamic_cast_encontrados(0), 
    llamada_impresion_especifica(false), 
    definicion_impesp(false) 
  {
    operadores_externos.insert(pair<string, bool>("operator<<", false));

    atributos.insert(pair<string, bool>("tarjeta_", false));
    atributos.insert(pair<string, bool>("num_", false));
    atributos.insert(pair<string, bool>("fecha_", false));
    atributos.insert(pair<string, bool>("total_", false));

    useAlgorithmFunctions.insert(pair<string, bool>("find_if", false));
    useAlgorithmFunctions.insert(pair<string, bool>("remove_if", false));
    foundAlgorithmFunctions.insert(pair<string, bool>("find_if", false));
    foundAlgorithmFunctions.insert(pair<string, bool>("remove_if", false));

    impresion_especifica_libro.insert(pair<string, bool>("n_pag_", false));
    impresion_especifica_libro.insert(pair<string, bool>("stock_", false));
    impresion_especifica_librodigital.insert(pair<string, bool>("f_expir_", false));
    impresion_especifica_cederron.insert(pair<string, bool>("tam_", false));
    impresion_especifica_cederron.insert(pair<string, bool>("stock_", false));	
  }

  virtual void run(const MatchFinder::MatchResult &Result) {
    Context = Result.Context;
    Rewrite.setSourceMgr(Context->getSourceManager(), Context->getLangOpts());

    if(!clase_encontrada)
      apply_clase(Result);
    else{
      apply_busqueda_cstring(Result);
      apply_friend(Result);
      apply_acceso_atributo(Result);

      if(isFileArticulo()){
	apply_stock_articulo(Result);
	apply_destructor_virtual(Result);
	apply_impesp_articulo(Result);
	apply_impesp_definicion(Result);
	apply_metodo_const(Result);
	apply_metodo_noexcept(Result);
      }
      else if(isFileTarjeta()){
	  apply_AlgorithmFunctions(Result);
	  apply_templatehash(Result);
	
      }
      else if(isFileUsuario()){
		apply_one_constructor(Result);
		apply_friend_operator(Result);
    	apply_unorderedset(Result);
      }
      if(isFilePedido()){
	apply_constructor_correcto(Result);
	apply_operadores_externos(Result);
	apply_tarjeta_constante(Result);
	apply_pedido_incrementado(Result);
	apply_dynamic_cast(Result);
      }
      else if(isFilePedidoArticulo()){
	apply_constructor_correcto(Result);
	apply_operadores_externos(Result);
	apply_constructor_pedidoarticulo(Result);
	apply_pedir_sobrecargado(Result);
      }
      else if(isFileUsuarioPedido()){

      }
    }
  }

  bool is_clase_encontrada(){
    return clase_encontrada;
  }

  bool tiene_one_constructor(){
    return one_constructor == 1 ? true : false;
  }

  bool tiene_constructor_pedidoarticulo(){
    return constructor_pedidoarticulo;	
  }

  bool encuentra_cstring(){
    return cstring;
  }

  bool existe_member_friend(){
    return friend_member;
  }

  bool tiene_atributo_no_privado(){
    return atributo_no_privado;
  }

  bool tiene_friend_operator_usuario(){
    return friend_operator_usuario;
  } 

  bool son_externos_operadores(){
    for (map<string, bool>::iterator it=operadores_externos.begin(); it!=operadores_externos.end(); it++){
      if( it->second )
	return false;    
    }
    return true;
  }

  bool is_explicit_specified(){
    return explicit_specified;
  }

  bool tiene_constructor_correcto(){
    return constructor_correcto;
  }

  bool es_constante_tarjeta(){
    return constante_tarjeta;
  }

  bool esta_pedir_sobrecargado(){
    return pedir_sobrecargado;
  }

  bool incrementa_numero_pedidos(){
    return incrementa_pedidos;
  }

  void setClase(string s){
    this->clase = s;
  }

  bool lista_inicializacion_correcta(){
    for (map<string, bool>::iterator it=atributos.begin(); it!=atributos.end(); it++){
      if( it->second )
	return false;    
    }
    return true;
  }

  bool tiene_atributo_unorderedset(){
  	return atributo_unorderedset;
  }

  bool usa_AlgorithmFunctions(){
    for (map<string, bool>::iterator it=useAlgorithmFunctions.begin(); it!=useAlgorithmFunctions.end(); it++){
    	if(it->second == false)
			return false;    
    }
    return true;
  }

  bool encontradas_AlgorithmFunctions(){
    for (map<string, bool>::iterator it=foundAlgorithmFunctions.begin(); it!=foundAlgorithmFunctions.end(); it++){
    	if(it->second == false){
			return false;   
		} 
    }
    return true;
  }

  bool existe_templateHash(){
	return templatehash;
  }

  bool algun_metodo_no_constante(){
    return metodos_constantes_autor;
  }

  bool algun_metodo_sin_noexcept(){
    return metodos_noexcept_autor;
  }

  bool tiene_destructor_virtual(){
    return destructor_virtual_articulo;
  }

  bool tiene_stock_articulo(){
    return stock_articulo;
  }

  bool tiene_stock_articuloalmacenable(){
    return stock_articuloalmacenable;
  }

  bool usa_dynamic_cast(){
    return dynamic_cast_encontrados >= 2;
  }

  bool llama_impresion_especifica(){
    return llamada_impresion_especifica;
  }

  bool define_mal_impresion_especifica(){
    return definicion_impesp;
  }

  bool imprime_atributos_impresion_especifica(){
    for (map<string, bool>::iterator it=impresion_especifica_libro.begin(); it!=impresion_especifica_libro.end(); it++){
      if( !it->second )
	return false;    
    }

    for (map<string, bool>::iterator it=impresion_especifica_librodigital.begin(); it!=impresion_especifica_librodigital.end(); it++){
      if( !it->second )
	return false;    
    }

    for (map<string, bool>::iterator it=impresion_especifica_cederron.begin(); it!=impresion_especifica_cederron.end(); it++){
      if( !it->second )
	return false;    
    }

    return true;
  }

  void resetClaseEncontrada(){
    clase_encontrada = false;
  }
private: 

  void apply_clase(const MatchFinder::MatchResult &Result){
	
    if (Result.Nodes.getNodeAs<clang::CXXRecordDecl>("clase_articulo") &&
	Result.Nodes.getNodeAs<clang::CXXRecordDecl>("clase_articuloalmacenable") &&
	Result.Nodes.getNodeAs<clang::CXXRecordDecl>("clase_libro") &&
	Result.Nodes.getNodeAs<clang::CXXRecordDecl>("clase_cederron") &&
	Result.Nodes.getNodeAs<clang::CXXRecordDecl>("clase_librodigital") && isFileArticulo()){
      clase_encontrada = true;
    }
    else if( Result.Nodes.getNodeAs<clang::CXXRecordDecl>("clase_autor") && isFileArticulo()){
      clase_encontrada = true;
    }
    else if(Result.Nodes.getNodeAs<clang::CXXRecordDecl>("clase_tarjeta") && isFileTarjeta()){
      clase_encontrada = true;
    }
    else if(Result.Nodes.getNodeAs<clang::CXXRecordDecl>("clase_usuario") && isFileUsuario()){
      clase_encontrada = true;
    }
    else if(Result.Nodes.getNodeAs<clang::CXXRecordDecl>("clase_pedido") && isFilePedido()){
      clase_encontrada = true;
    }
    else if(Result.Nodes.getNodeAs<clang::CXXRecordDecl>("clase_pedidoarticulo") && 
	    Result.Nodes.getNodeAs<clang::CXXRecordDecl>("clase_lineapedido") && isFilePedidoArticulo()){
      clase_encontrada = true;
    }
    else if(Result.Nodes.getNodeAs<clang::CXXRecordDecl>("clase_usuariopedido") && isFileUsuarioPedido()){
      clase_encontrada = true;
    }
  }

  void apply_one_constructor(const MatchFinder::MatchResult &Result){
    if (Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("one_constructor")){
      one_constructor++;
    }
  }

  void apply_constructor_correcto(const MatchFinder::MatchResult &Result){
		
    //Solo entrara para la declaración de la clase, pero no para redeclaraciones fuera de la clase
    if (const CXXConstructorDecl *FS = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("constructor_pedido")){

      one_constructor++;

      if(one_constructor == 1 && FS->getNumParams() == 5 && !FS->getParamDecl(3)->hasDefaultArg() && FS->getParamDecl(4)->hasDefaultArg()){
	constructor_correcto = true;
      }
      else
	constructor_correcto = false;

      //Así tomamos la definición para hacer la comprobación de los inicializadores
      const CXXConstructorDecl *FS2;
      if((CXXConstructorDecl*)FS->getMostRecentDecl() != NULL){
	FS2 = (CXXConstructorDecl*)FS->getMostRecentDecl();
      }
      else
	FS2 = FS;

      if(FS2->getNumCtorInitializers() == 4){	
	for(CXXConstructorDecl::init_const_iterator i = FS2->init_begin(); i != FS2->init_end(); ++i){
	  if((*i)->isWritten() && (*i)->isMemberInitializer()){
	    FieldDecl *fd = (*i)->getMember(); 
	    map<string, bool>::iterator it;
	    if( (it = atributos.find(fd->getNameAsString())) != atributos.end() ){
	      it->second = true;
	    }
	  }
	}
      }
    }
    else if(const CXXConstructorDecl *FS = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("constructor_lineapedido")){

      one_constructor++;

      if(one_constructor == 1 && FS->getNumParams() == 2 && !FS->getParamDecl(0)->hasDefaultArg() && FS->getParamDecl(1)->hasDefaultArg()){
	constructor_correcto = true;
      }
      else
	constructor_correcto = false;

      if(FS->isExplicitSpecified()){
	explicit_specified = true;
      }
    }	
  }

  void apply_pedido_incrementado(const MatchFinder::MatchResult &Result){

    if(const CompoundStmt *FS1 = Result.Nodes.getNodeAs<clang::CompoundStmt>("constructor_pedido_compSt")){

      const Stmt *FS3 = Result.Nodes.getNodeAs<clang::Stmt>("stmt_found");
		
      if(FS1->size() > 0){
				
	//llvm::outs() << "Stmt final IOP: " << 
	//	Rewrite.getRewrittenText(FS3->getSourceRange()) << "\n";
	const Stmt* lst = FS1->body_back();
	if(check_same_stmts(lst,FS3))
	  incrementa_pedidos = true;
      }
    }
  }

  void apply_busqueda_cstring(const MatchFinder::MatchResult &Result){
    if (const FunctionDecl *FS = Result.Nodes.getNodeAs<clang::FunctionDecl>("method_cstring")){
      FullSourceLoc FullLocation1 = Context->getFullLoc(FS->getLocStart());

      if (FullLocation1.isValid() ){
	//Si la cabecera es string.h, con esto vale. Pero si es cstring, lo que te encuentra este fullLocation es la posición de string.h 
	//dentro de cstring.h. Por eso hay que discernir en qué situación nos encontramos y si estamos en la segunda, buscar una nueva
	//localización
	FullSourceLoc FullLocation2 = Context->getFullLoc(
							  Context->getSourceManager().getIncludeLoc(FullLocation1.getFileID())
							  );

	if(FullLocation2.isValid()){
	  if( string(Context->getSourceManager().getFileEntryForID(FullLocation2.getFileID())->getName()).find("cstring") != std::string::npos){
	    FullLocation2 = Context->getFullLoc(
						Context->getSourceManager().getIncludeLoc(FullLocation2.getFileID())
						);
	  }
	}

	if(!FullLocation2.isValid() || Context->getSourceManager().isInSystemHeader(FullLocation2)){
	  cstring = false;
	}
      }
    }
  }

  void apply_friend(const MatchFinder::MatchResult &Result){
    if (const CXXRecordDecl *FS = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("friend_class")){

      FullSourceLoc FullLocation1 = Context->getFullLoc(FS->getLocStart());
      if (FullLocation1.isValid() 
	  && !Context->getSourceManager().isInSystemHeader(FullLocation1) 
	  ){
	string file = Context->getSourceManager().getFilename(FullLocation1);
	if((isFileArticulo() && file.find("articulo.hpp") != std::string::npos) ||
	   (isFileUsuario() && file.find("usuario.hpp") != std::string::npos) ||
	   (isFileTarjeta() && file.find("tarjeta.hpp") != std::string::npos) ||
	   (isFilePedido() && file.find("pedido.hpp") != std::string::npos) ||	
	   (isFilePedidoArticulo() && file.find("pedido_articulo.hpp") != std::string::npos) ||
	   (isFileUsuarioPedido() && file.find("usuario-pedido.hpp") != std::string::npos) )
	  {
	    //llvm::outs () <<"Friend member: " << FS->getNameAsString() << " \n";
	    friend_member = true;
	  }
      }
    }
  } 

  void apply_acceso_atributo(const MatchFinder::MatchResult &Result){ 
    if (const FieldDecl *FS = Result.Nodes.getNodeAs<clang::FieldDecl>("field_decl")){
      FullSourceLoc FullLocation1 = Context->getFullLoc(FS->getLocStart());

      if (FullLocation1.isValid()
	  && !Context->getSourceManager().isInSystemHeader(FullLocation1)
	  ){
	//Pare evitar que informe dos veces del mismo fallo al estar varias cabeceras incluidas unas en otras, 
	//tengo que comprobar qué fichero se está analizando
	string file = Context->getSourceManager().getFilename(FullLocation1);
	if((isFileArticulo() && file.find("articulo.hpp") != std::string::npos) ||
	   (isFileUsuario() && file.find("usuario.hpp") != std::string::npos) ||
	   (isFileTarjeta() && file.find("tarjeta.hpp") != std::string::npos) ||
	   (isFilePedido() && file.find("pedido.hpp") != std::string::npos) ||	
	   (isFilePedidoArticulo() && file.find("pedido_articulo.hpp") != std::string::npos) ||
	   (isFileUsuarioPedido() && file.find("usuario-pedido.hpp") != std::string::npos) )
	  {
	    //llvm::outs() << FS->getNameAsString() << "\n";
	    const CXXRecordDecl *FS2 = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("clase_atributo");
	    if( (FS2->getNameAsString() == "Articulo") || (FS2->getNameAsString() == "ArticuloAlmacenable") ){
	      if(FS->getAccess() != AS_protected){
		atributo_no_privado = true;
	      }
	    }
	    else{
	      if(FS->getAccess() != AS_private){
		atributo_no_privado = true;
	      }
	    }
	  }
      }
    }
  }

  void apply_friend_operator(const MatchFinder::MatchResult &Result){
    if (Result.Nodes.getNodeAs<clang::FunctionDecl>("friend_operator_usuario")){
      //llvm::outs() << "Entro friend operator\n";
      friend_operator_usuario = true;
    }
  }

  void apply_operadores_externos(const MatchFinder::MatchResult &Result){
    CXXMethodDecl *FS;
    if ( (FS = const_cast<CXXMethodDecl*>(Result.Nodes.getNodeAs<clang::CXXMethodDecl>("member_operator_pedido"))) ||
	 (FS = const_cast<CXXMethodDecl*>(Result.Nodes.getNodeAs<clang::CXXMethodDecl>("member_operator_pedidoarticulo"))) ||
	 (FS = const_cast<CXXMethodDecl*>(Result.Nodes.getNodeAs<clang::CXXMethodDecl>("member_operator_lineapedido"))) ){
	
      map<string, bool>::iterator it;
      if( (it = operadores_externos.find(FS->getNameAsString())) != operadores_externos.end() ){
	it->second = true;
      }
    }
  }

  void apply_tarjeta_constante(const MatchFinder::MatchResult &Result){
    if (const FieldDecl *FS = Result.Nodes.getNodeAs<clang::FieldDecl>("tarjeta_const")){
      QualType ft =  FS->getType().getCanonicalType();
      if(ft->isPointerType()){
	ft = ft->getPointeeType();
			
	if(ft.isConstQualified())
	  constante_tarjeta = true;
      }
    }
  }

  void apply_constructor_pedidoarticulo(const MatchFinder::MatchResult &Result){

    if(Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("const_pedidoarticulo")){
      constructor_pedidoarticulo = true;
    }
  } 

  void apply_pedir_sobrecargado(const MatchFinder::MatchResult &Result){

    if (const CXXMethodDecl *FS = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("metodo_pedir")){

      const CXXRecordDecl *FS2 = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("clase_metodo_pedir");

      for(CXXRecordDecl::method_iterator me = FS2->method_begin(); me != FS2->method_end(); me++){

	CXXMethodDecl *m = *me;

	if( !check_same_decl_canonical(FS, m) && 
	    (m->getNameAsString() == FS->getNameAsString()) && (m->getNumParams() == FS->getNumParams())){

	  bool proceed = true;
	  for(unsigned int p = 0; p < FS->getNumParams(); p++){

	    if(p == 0){
	      if(m->getParamDecl(p)->getType().getCanonicalType().getAsString() != FS->getParamDecl(p+1)->getType().getCanonicalType().getAsString()){
		proceed = false; break;			
	      }
	    }
	    else if(p == 1){
	      if(m->getParamDecl(p-1)->getType().getCanonicalType().getAsString() != FS->getParamDecl(p)->getType().getCanonicalType().getAsString()){
		proceed = false; break;		
	      }
	    }
	    else{
	      if(m->getParamDecl(p)->getType().getCanonicalType().getAsString() != FS->getParamDecl(p)->getType().getCanonicalType().getAsString()){
		proceed = false; break;		
	      }
	    }
	  }

	  if(proceed)
	    pedir_sobrecargado = true;

	  //Solo debería haber un método sobrecargado, por lo que sino es este, ya no debe haber ningún otro
	  break;
	}
      }
    }
  }

  void apply_unorderedset(const MatchFinder::MatchResult &Result){ 
    if (const VarDecl *FS = Result.Nodes.getNodeAs<clang::VarDecl>("unorderedset_decl")){
        //llvm::outs() << FS->getNameAsString() << " - " << FS->getType().getCanonicalType().getAsString() <<"\n";
        if(std::regex_match (FS->getType().getCanonicalType().getAsString(), std::regex("(.*)(unordered_set<)(.*)(Cadena)(.*)(>)(.*)") ) ){ 
          atributo_unorderedset = true;
        }
    }
  }

  void apply_AlgorithmFunctions(const MatchFinder::MatchResult &Result){
	map<string, bool>::iterator i, j;

	const FunctionDecl *FS = Result.Nodes.getNodeAs<clang::FunctionDecl>("nameFunctionAlgorithm");
	const DeclRefExpr *DS = Result.Nodes.getNodeAs<clang::DeclRefExpr>("functionAlgorithm");

	if (FS && DS){
		FullSourceLoc FullLocationDS = Context->getFullLoc(DS->getLocStart());
		FullSourceLoc FullLocationFS1 = Context->getFullLoc(FS->getLocStart());
		if (FullLocationDS.isValid() && !Context->getSourceManager().isInSystemHeader(FullLocationDS)){
			if( (j = foundAlgorithmFunctions.find(FS->getNameAsString())) != foundAlgorithmFunctions.end() ){
				j->second = true;
      			}
			
			FullSourceLoc FullLocationFS2 = Context->getFullLoc(Context->getSourceManager().getIncludeLoc(FullLocationFS1.getFileID()));

			if(FullLocationFS2.isValid() && string(Context->getSourceManager().getFileEntryForID(FullLocationFS2.getFileID())->getName()).find("algorithm") != std::string::npos){
				if( (i = useAlgorithmFunctions.find(FS->getNameAsString())) != useAlgorithmFunctions.end() ){
					i->second = true;
      				}
			}
		}
	}
  }

  void apply_templatehash(const MatchFinder::MatchResult &Result){
	if (const ClassTemplateSpecializationDecl* FS = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("templateHash")){
		FullSourceLoc FullLocation = Context->getFullLoc(FS->getLocStart());
		if (FullLocation.isValid()){
			if( string(Context->getSourceManager().getFileEntryForID(FullLocation.getFileID())->getName()).find("cadena.hpp") != std::string::npos){
				templatehash = true;
			}
		}
	}
  }

  void apply_metodo_const(const MatchFinder::MatchResult &Result){

    if(const CXXMethodDecl *FS = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("metodo_autor_no_const")){
      //llvm::outs() << "Entro en metodo const con el método " << FS->getNameAsString() << "\n";
      if(!FS->isConst() && !llvm::isa<CXXConstructorDecl>(FS))
	metodos_constantes_autor = true;
    }
  }

  void apply_metodo_noexcept(const MatchFinder::MatchResult &Result){

    if (const CXXMethodDecl *FS = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("metodo_autor_noexcept")){
		
      const FunctionProtoType *FP = FS->getType()->castAs<FunctionProtoType>();
      if(FP && !FP->isNothrow(*Context)){
	//llvm::outs() << "Entro en noexcept\n";
	metodos_noexcept_autor = true;
      }
    }	 
  }
	
  void apply_stock_articulo(const MatchFinder::MatchResult &Result){
	
    if(Result.Nodes.getNodeAs<clang::FieldDecl>("stock_articulo")){
      stock_articulo = true;
    }
    else if(Result.Nodes.getNodeAs<clang::FieldDecl>("stock_articuloalmacenable")){
      stock_articuloalmacenable = true;
    }
  }

  void apply_destructor_virtual(const MatchFinder::MatchResult &Result){

    if(const CXXDestructorDecl *FS = Result.Nodes.getNodeAs<clang::CXXDestructorDecl>("destructor_virtual_articulo")){

      if(FS->isVirtualAsWritten())
	destructor_virtual_articulo = true;
    }
  }

  void apply_dynamic_cast(const MatchFinder::MatchResult &Result){
	
    if (Result.Nodes.getNodeAs<clang::CXXDynamicCastExpr>("dynamic_constructor_pedido")){
      dynamic_cast_encontrados++;
    }
  }

  void apply_impesp_articulo(const MatchFinder::MatchResult &Result){

    if (const FunctionDecl *FS = Result.Nodes.getNodeAs<clang::FunctionDecl>("member_ext_articulo")){
		
      FullSourceLoc FullLocation1 = Context->getFullLoc(FS->getLocStart());
      if (FullLocation1.isValid()
	  && !Context->getSourceManager().isInSystemHeader(FullLocation1)
	  ){
	string file = Context->getSourceManager().getFilename(FullLocation1);
	if(isFileArticulo()){
	  llamada_impresion_especifica = true;
	}
      }
    }
  }

  void apply_impesp_definicion(const MatchFinder::MatchResult &Result){

    if(const FieldDecl *FS = Result.Nodes.getNodeAs<clang::FieldDecl>("field_impesp_libro")){	
      if( (FS->getNameAsString() == "n_pag_")  || (FS->getNameAsString() == "stock_") ){
	(impresion_especifica_libro.find(FS->getNameAsString()))->second = true;
      }
      else{
	definicion_impesp = true;	//Esto es para marcar que está mal que haya otras referencias a atributos.
      }
    }
    else if(const FieldDecl *FS = Result.Nodes.getNodeAs<clang::FieldDecl>("field_impesp_cederron")){
      if( (FS->getNameAsString() == "tam_")  || (FS->getNameAsString() == "stock_") ){
	(impresion_especifica_cederron.find(FS->getNameAsString()))->second = true;
      }
      else{
	definicion_impesp = true;
      }
    }
    else if(const FieldDecl *FS = Result.Nodes.getNodeAs<clang::FieldDecl>("field_impesp_librodigital")){
      if( FS->getNameAsString() == "f_expir_" ){
	(impresion_especifica_librodigital.find(FS->getNameAsString()))->second = true;
      }
      else{
	definicion_impesp = true;
      }
    }
  }

  bool check_same_decl_canonical(const Decl *c1, const Decl *c2){
    return c1 && c2 && c1->getCanonicalDecl() == c2->getCanonicalDecl();
  }

  bool check_same_stmts(const Stmt *First, const Stmt *Second) {
    return First && Second && First == Second;
  }

  bool isFileArticulo(){
    return clase == "Articulo, ArticuloAlmacenable, Libro, Cederrón, LibroDigital y Autor";
  }

  bool isFileTarjeta(){
    return clase == "Tarjeta";
  }

  bool isFileUsuario(){
    return clase == "Usuario";
  }

  bool isFilePedido(){
    return clase == "Pedido";
  }

  bool isFilePedidoArticulo(){
    return clase == "Pedido_Articulo y LineaPedido";
  }

  bool isFileUsuarioPedido(){
    return clase == "Usuario_Pedido";
  }

  bool clase_encontrada;
  unsigned one_constructor;
  bool constructor_correcto;
  bool constante_tarjeta;
  bool constructor_pedidoarticulo;
  bool cstring;
  bool friend_member;
  bool atributo_no_privado;
  bool friend_operator_usuario;
  map<string, bool> operadores_externos;
  bool explicit_specified;
  map<string, bool> atributos;
  bool pedir_sobrecargado;
  bool incrementa_pedidos;
  bool atributo_unorderedset;
  map<string, bool> useAlgorithmFunctions;
  map<string, bool> foundAlgorithmFunctions;
  bool templatehash;
  bool metodos_noexcept_autor;
  bool metodos_constantes_autor;
  bool destructor_virtual_articulo;
  bool stock_articulo;
  bool stock_articuloalmacenable;
  unsigned dynamic_cast_encontrados;
  bool llamada_impresion_especifica;
  bool definicion_impesp;
  map<string, bool> impresion_especifica_libro;
  map<string, bool> impresion_especifica_librodigital;
  map<string, bool> impresion_especifica_cederron;

  string clase;
  ASTContext *Context;
  Rewriter Rewrite;
};	

list<string> cabeceras_leidas(string file_name, string orden){

  int res = system(orden.c_str());
  ifstream read_file;

  read_file.open(file_name.c_str(), std::ifstream::in);
  list<string> lstr;

  if (read_file.is_open() && res ==0) {
    string s;
    getline(read_file, s);

    while (! read_file.eof()) {
      if(s != ""){
	std::size_t first = s.find_first_not_of(' ');
	s = s.substr(first);
	lstr.push_back(string("-isystem" + s));
      }
      getline(read_file, s);
    }
    read_file.close();
  }
  return lstr;
}

static llvm::cl::OptionCategory MyToolCategory("pedido_check options");

int main(int argc, const char **argv) {

  string option = argv[1];

  if(option.compare("--help") == 0){
    llvm::outs() << "Uso: " << argv[0] << " articulo.cpp tarjeta.cpp usuario.cpp" 
    	" pedido.cpp pedido-articulo.cpp usuario-pedido.hpp -- -std=c++11 -I../P1 -I..\n";
    exit(EXIT_SUCCESS);
  }

  int fixed = 0;
  for(int i = 2; i < argc; i++){	//argv[0] is "mucpp" and argv[1] is the option, so we begin in 2
    string arg = argv[i];
    if(arg.compare("--")==0){
      fixed = argc - i;		
    }
  }
 
  string file= "";
  unsigned startSources = 1;
  list<string> listSources;
  list<string>::iterator it;

  while(argc - fixed > int(startSources)){
    file = string(argv[startSources]);

    it = find(listSources.begin(), listSources.end(), file);
    if(listSources.empty() || it == listSources.end()){
      listSources.push_back(file);
    }

    startSources++;
  }

  string file_name = "catalogo_check.tmp";
  string orden =  "echo | clang -E -v -x c++ - 2>&1 | sed -n '/<...> search starts here/,/End of search list./p' | sed 's/End of search list.//g' | sed 's/#include <...> search starts here://g' >" + file_name;

  list<string> lstr = cabeceras_leidas(file_name, orden);

  orden =  "echo | LANG=C cpp -v -x c++ - 2>&1 | sed -n '/<...> search starts here/,/End of search list./p' | sed 's/End of search list.//g' | sed 's/#include <...> search starts here://g' >" + file_name;

  list<string> lstr_clang = cabeceras_leidas(file_name, orden);
  lstr.insert(lstr.end(), lstr_clang.begin(), lstr_clang.end());

  for(auto pos = lstr.begin(); pos != lstr.end(); pos++){
    argv[argc] = (*pos).c_str();
    //llvm::outs() << argv[argc] << " ";
    argc++;
  }

  //for(int i=0; i< argc; i++)
  //    llvm::outs() << argv[i] << " ";

  orden = "rm " + file_name  + " > /dev/null 2>&1";

  system(orden.c_str());
 
  bool sin_fallos;
  CommonOptionsParser OptionsParser(argc, argv, MyToolCategory);
  for(auto pos = listSources.begin(); pos != listSources.end(); pos++){

    CatalogoCheck FC;	
    ClangTool Tool(OptionsParser.getCompilations(), *pos);
    MatchFinder FinderClase;
    MatchFinder Finder;
    string clase = "";

    //General
    Finder.addMatcher(Funciones_Estandar_Matcher, &FC);
    Finder.addMatcher(Friend_Matcher, &FC);
    Finder.addMatcher(Private_Fields_Matcher, &FC);

    if(*pos == "articulo.cpp"){
      FinderClase.addMatcher(Clase_Articulo_Matcher, &FC);
      clase = "Articulo, ArticuloAlmacenable, Libro, Cederrón, LibroDigital y Autor";
      FC.setClase(clase);
      Tool.run(newFrontendActionFactory(&FinderClase).get());
      if(!FC.is_clase_encontrada()){
	llvm::outs() << "No se ha encontrado la clase 'Artículo', 'ArticuloAlmacenable', 'Libro', 'Cederrón y/o 'LibroDigital'.\n";
      }
      else{
	MatchFinder FinderClase2;
	FC.resetClaseEncontrada();
	FinderClase2.addMatcher(Clase_Autor_Matcher, &FC);
	Tool.run(newFrontendActionFactory(&FinderClase2).get());

	llvm::outs() << "* articulo.cpp:\n";

	if(FC.is_clase_encontrada()){
	  sin_fallos = true;
	  Finder.addMatcher(Destructor_Virtual_Articulo_Matcher, &FC);
	  Finder.addMatcher(Articulo_NoStock_Matcher, &FC);
	  Finder.addMatcher(ArticuloAlmacenable_Stock_Matcher, &FC);
	  Finder.addMatcher(Llamada_Impesp_Matcher, &FC);
	  Finder.addMatcher(Definicion_Impesp_Cederron_Matcher, &FC);
	  Finder.addMatcher(Definicion_Impesp_Libro_Matcher, &FC);
	  Finder.addMatcher(Definicion_Impesp_LibroDigital_Matcher, &FC);
	  Finder.addMatcher(Const_Autor_Matcher, &FC);
	  Finder.addMatcher(NoExcept_Autor_Matcher, &FC);

	  Tool.run(newFrontendActionFactory(&Finder).get());

	  if(FC.tiene_stock_articulo() || !FC.tiene_stock_articuloalmacenable()){
	    sin_fallos = false;
	    llvm::outs() << "Revisa el enunciado respecto a los atributos que deben estar en cada clase.\n";
	  }

	  if(!FC.tiene_destructor_virtual()){
	    sin_fallos = false;
	    llvm::outs() << "Contempla crear un destructor apropiado para esta clase.\n";
	  }

	  if(!FC.llama_impresion_especifica()){
	    sin_fallos = false;
	    llvm::outs() << "Revisa el operador << definido para la impresión de Artículo.\n";;
	  }
			
	  if(FC.define_mal_impresion_especifica() || !FC.imprime_atributos_impresion_especifica()){
	    sin_fallos = false;
	    llvm::outs() << "Revisa qué debe imprimir la definición del método impresion_especifica en cada clase.\n";
	  }

	  if(FC.algun_metodo_no_constante()){
	    sin_fallos = false;
	    llvm::outs() << "Revisa el enunciado respecto a la modificación de los datos del Autor.\n";
	  }

	  if(FC.algun_metodo_sin_noexcept()){
	    sin_fallos = false;
	    llvm::outs() << "Revisa el enunciado respecto a las excepciones.\n";
	  }			
	}
	else
	  llvm::outs() << "No se ha encontrado la clase 'Autor'.\n";
      }
    }
    else if(*pos == "tarjeta.cpp"){
      FinderClase.addMatcher(Clase_Tarjeta_Matcher, &FC);
      clase = "Tarjeta";
      FC.setClase(clase);
      Tool.run(newFrontendActionFactory(&FinderClase).get());

      llvm::outs() << "* tarjeta.cpp:\n";
		
      if(FC.is_clase_encontrada()){
	sin_fallos = true;

	Finder.addMatcher(AlgorithmFunctions_Matcher, &FC);
	Finder.addMatcher(Hash_Cadena_Matcher, &FC);
	
	Tool.run(newFrontendActionFactory(&Finder).get());

	if(!FC.encontradas_AlgorithmFunctions()){
		sin_fallos = false;
		llvm::outs() << "Revisa el enunciado sobre el uso de find_if y remove_if.\n";
	}
	if(sin_fallos && !FC.usa_AlgorithmFunctions()){
		sin_fallos = false;
		llvm::outs() << "Revisa de dónde son tomadas las funciones find_if y remove_if.\n";
	}
	if(!FC.existe_templateHash()){
		sin_fallos = false;
		llvm::outs() << "No se ha localizado template hash en la cabecera adecuada.\n";
	}
      }
      else
	llvm::outs() << "No se ha encontrado la clase 'Tarjeta'\n";
    }
    else if(*pos == "usuario.cpp"){
      FinderClase.addMatcher(Clase_Usuario_Matcher, &FC);
      clase = "Usuario";
      FC.setClase(clase);
      Tool.run(newFrontendActionFactory(&FinderClase).get());

      llvm::outs() << "* usuario.cpp:\n";
		
      if(FC.is_clase_encontrada()){

	sin_fallos = true;
	Finder.addMatcher(One_Constructor_Usuario_Matcher, &FC);
	Finder.addMatcher(Friend_Operator_Usuario_Matcher, &FC);
	Finder.addMatcher(Unorderedset_Matcher, &FC);

	Tool.run(newFrontendActionFactory(&Finder).get());

	if(!FC.tiene_one_constructor()){
	  sin_fallos = false;
	  llvm::outs() << "Revisa el enunciado respecto a los construcción de objetos.\n";
	}

	if(!FC.tiene_friend_operator_usuario()){
	  sin_fallos = false;
	  llvm::outs() << "Revisa el enunciado respecto al operador de extracción.\n";
	}
	if(!FC.tiene_atributo_unorderedset()){
      	  sin_fallos = false;
     	  llvm::outs() << "Revisa cómo se almacena el conjunto de identificadores del usuario.\n";
    	}
      }
      else
	llvm::outs() << "No se ha encontrado la clase 'Usuario'.\n";
    }
    else if(*pos == "pedido.cpp"){
      FinderClase.addMatcher(Clase_Pedido_Matcher, &FC);
      clase = "Pedido";
      FC.setClase(clase);
      Tool.run(newFrontendActionFactory(&FinderClase).get());

      llvm::outs() << "* pedido.cpp:\n";
		
      if(FC.is_clase_encontrada()){
	sin_fallos = true;
	Finder.addMatcher(One_Constructor_Pedido_Matcher, &FC);
	Finder.addMatcher(Actualiza_Numero_Pedidos_Matcher, &FC);
	Finder.addMatcher(Non_Member_Pedido_Matcher, &FC);
	Finder.addMatcher(Tarjeta_Const_Matcher, &FC);
	Finder.addMatcher(DynamicCast_Pedido_Matcher, &FC);
		
	Tool.run(newFrontendActionFactory(&Finder).get());

	if(!FC.tiene_constructor_correcto()){
	  sin_fallos = false;
	  llvm::outs() << "Revisa el enunciado respecto a los construcción de objetos.\n";
	}

	if(sin_fallos && FC.lista_inicializacion_correcta()){
	  sin_fallos = false;
	  llvm::outs() << "Revisa la lista de inicialización del constructor.\n";		
	}

	if(sin_fallos && !FC.incrementa_numero_pedidos()){
	  sin_fallos = false;
	  llvm::outs() << "Revisa que el número total de pedidos quede actualizado justo al final del constructor.\n";
	}

	if(!FC.es_constante_tarjeta()){
	  sin_fallos = false;
	  llvm::outs() << "Revisa el enunciado respecto al atributo de la tarjeta de pago.\n";
	}

	if(!FC.son_externos_operadores()){
	  sin_fallos = false;
	  llvm::outs() << "Revisa el lugar de la declaración de los operadores.\n";
	}

	if(!FC.usa_dynamic_cast()){
	  sin_fallos = false;
	  llvm::outs() << "Es necesario que emplees el operador de molde apropiado en el constructor de la clase.\n";
	}
      }
      else
	llvm::outs() << "No se ha encontrado la clase 'Pedido'.\n";
    }
    else if(*pos == "pedido-articulo.cpp"){
      FinderClase.addMatcher(Clase_PedidoArticulo_Matcher, &FC);
      clase = "Pedido_Articulo y LineaPedido";
      FC.setClase(clase);
      Tool.run(newFrontendActionFactory(&FinderClase).get());

      llvm::outs() << "* pedido-articulo.cpp:\n";
		
      if(FC.is_clase_encontrada()){
	sin_fallos = true;
	Finder.addMatcher(Non_Member_PedidoArticulo_Matcher, &FC);
	Finder.addMatcher(Non_Member_LineaPedido_Matcher, &FC);
	Finder.addMatcher(One_Constructor_LineaPedido_Matcher, &FC);
	Finder.addMatcher(Non_Constructor_Pedido_Matcher, &FC);
	Finder.addMatcher(Overloaded_Pedido_Matcher, &FC);

	Tool.run(newFrontendActionFactory(&Finder).get());

	if(!FC.tiene_constructor_correcto()){
	  sin_fallos = false;
	  llvm::outs() << "Revisa el enunciado respecto a los construcción de objetos.\n";
	}

	if(sin_fallos && !FC.is_explicit_specified()){
	  sin_fallos = false;
	  llvm::outs() << "Revisa el enunciado respecto a conversiones implícitas.\n";
	}

	if(FC.tiene_constructor_pedidoarticulo()){
	  sin_fallos = false;
	  llvm::outs() << "Revisa la necesidad de definir constructores.\n";
	}

	if(!FC.son_externos_operadores()){
	  sin_fallos = false;
	  llvm::outs() << "Revisa el lugar de la declaración de los operadores.\n";
	}

	if(!FC.esta_pedir_sobrecargado()){
	  sin_fallos = false;
	  llvm::outs() << "Se sugiere la sobrecarga del método 'pedir'.\n";
	}
      }
      else
	llvm::outs() << "No se ha encontrado la clase 'Pedido_Articulo' y/o 'LineaPedido'.\n";
    }
    else if(*pos == "usuario-pedido.hpp"){
      FinderClase.addMatcher(Clase_UsuarioPedido_Matcher, &FC);
      clase = "Usuario_Pedido";
      FC.setClase(clase);

      Tool.run(newFrontendActionFactory(&FinderClase).get());

      llvm::outs() << "* usuario-pedido.hpp:\n";
		
      if(FC.is_clase_encontrada()){
	sin_fallos = true;

	Tool.run(newFrontendActionFactory(&Finder).get());
      }
      else
	llvm::outs() << "No se ha encontrado la clase 'Usuario_Pedido'.\n";
    }
    //General
    if(FC.is_clase_encontrada()){
      //llvm::outs() << "-------------\n";
      if(!FC.encuentra_cstring()){
	sin_fallos = false;
	llvm::outs() << "Revisa de dónde son tomadas las funciones de la biblioteca estándar como strlen, strcpy...\n";
      }

      if(FC.existe_member_friend()){
	sin_fallos = false;
	llvm::outs() << "Revisa por qué es necesario incluir 'friend'.\n";
      }
	
      if(FC.tiene_atributo_no_privado()){
	sin_fallos = false;
	llvm::outs() << "Revisa el acceso a los atributos.\n";
      }

      if(sin_fallos)
	llvm::outs() << "Verificación correcta de la clase " << clase  << ".\n";
    }
  }

  exit(EXIT_SUCCESS);
}
