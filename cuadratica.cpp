#include<iostream>
#include<math.h>
using namespace std;
int main (){
	float a, b, c;
	cout << "Ingrese el valor para a, b y c:";
	cin >> a >> b >> c;
	
	if(a == 0){
		cout << "Error: no es una ecuacion cuadratica, a no puede ser 0." << endl;
		return 0;
	}
	
	float discriminante = b*b - 4*a*c;
	
	if(discriminante < 0){
		cout << "El resultado no es un numero real." << endl;
		return 0;
	}
	
	float x1 = (-b + sqrt(discriminante)) / (2*a);
	float x2 = (-b + sqrt(discriminante)) / (2*a);
	
	
	cout << "Resultado de x1:" << x1 << endl;
	cout << "Resultado de x2:" << x2 << endl;
	
}