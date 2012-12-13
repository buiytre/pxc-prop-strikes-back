#include <LoginRequest.h>
#include <ControlConnection.h>
#include <NewClient.h>
#include <Login.h>
#include <iostream>
#include <string.h>
#include <mutex>

using namespace std;

void LoginRequestRcvd::exec(Connection* c) const throw(){
  const char* userPointer;
  const char* pwdPointer;
  
  /*Parámetros a pasar*/
  int answerCode; //Código de respuesta al intento de login. 0 = login correcto, 1=Wrong username or password, los demás que os hagan falta.
  char routerIp[16]; //Ip del router al que se conectará seguida de uno o más caracteres NULL \0 para indicar el final de string. Si no es un login correcto se ignora.
  char routerPort[5]; // Puerto del router
  int clientId; //Id del cliente en nuestro sistema (mapeo directo id-username)
  int token; //Token de sesión si es un login correcto. Si no, se ignora. Login necesita mandar el mismo token con el id del cliente al router, junto a la zona en la que se encuentra inicialmente
  ///////////////////////
  cout << "Recibimos conexión" << std::endl;
  Login::instance().loginMutex.lock();
  
  userPointer = username;
  pwdPointer = password;
  if(Login::instance().validate(userPointer,pwdPointer) ){
	  answerCode = 0;
	  strcpy(routerIp, ROUTER_IP);
	  strcpy(routerPort,ROUTER_PORT);
	  clientId = Login::instance().nextFreeToken;
	  token = Login::instance().nextFreeToken;
	  Login::instance().userTokenMap.insert(pair<int,int>(clientId,token));
	  Login::instance().nextFreeToken++;
	  Login::instance().usersConnected++;
  }else{
	  answerCode = 1;
  }
  
  //Enviar info a balanceo  
  NewClientSend* newClientSend = new NewClientSend(clientId,token);
  Login::instance().controlConnection->send(*newClientSend);  
  //Enviar info a cliente
  LoginRequestSend* loginRequestSend = new LoginRequestSend(answerCode, routerIp,routerPort, clientId,token);
  c->sendAnswer(*loginRequestSend);
  
  //delete newClientSend;
  delete loginRequestSend;
  Login::instance().loginMutex.unlock();
}

LoginRequestRcvd::~LoginRequestRcvd(){

	
}

