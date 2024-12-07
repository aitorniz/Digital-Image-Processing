#include <iostream>
#include <string>

using namespace std;

int main(){
	cout << "Quel Ãge avez vous Grand Schtroupf ?" << endl;
	int ageGrandSchtroumpf;
	cin >> ageGrandSchtroumpf;
	cout << ageGrandSchtroumpf << " Ohhh mais vous n'Ãtiez pas si vieu en fait Grand Schtroupf?!" << endl;
	cin.ignore();
	cout << "Quel est votre nom Grand Schtroumpf ?" << endl;
	string nom;
	getline(cin, nom);
        cout <<"Oh mais nous ne savions pas que vous portiez le nom: " << nom << " Grand Schtroumpf"<< endl;	
	cout <<"Mais qui m'a foutu des abrutis pareil...Gargamel a pas de soucis a se faire..." << endl;	
	
	cout << "Veuillez renseigner la valeur de a" << endl;
	int a;
	cin >> a ;
	cin.ignore();

	cout << "Veuillez renseigner la valeur de b" << endl;
	int b;
	cin >> b ;
	
	
	cout << "Veuillez renseigner la valeur de d" << endl;
	int d;
	cin >> d;
	cout << "d = " << d << endl;
	d %= 3;
	cout << "d%3 = " << d << endl;	
	d /= 1;
	cout << "d/=3 = " << d << endl;
	
	cout << "a = " << a << endl;
	cout << "b = " << b << endl;
	cout << "d = " << d << endl;

	if (a < b){
		cout << "a est infÃ©rieur a b" << endl;
	}
	else if (a = b){
		cout << "a est egal a b. Incroyable !" << endl;
	}
	else{
		cout << "On s'en tamponne le Michel Barnier de a et b" << endl;
	}

	return 0;
}
