#include <iostream>
#include <string>

using namespace std;

int main(){
	cout << "Hello world!" << endl;
	string nomUtilisateur("Grand Schtroumpf");
	int ageGrandSchtroumpf(276);
	int& ageMax(ageGrandSchtroumpf);
	cout << "Le chapeau rouge de: " << nomUtilisateur << endl << "et l'Ãge du Grand Schtroumpf est de(variable): " << ageGrandSchtroumpf << "ce qui correspond a l'Ãge maximum du village(reference): " << ageMax;
	cout << "Quel Ãge avez vousGrand Schtroupf ?" << endl;
	cin >> ageGrandSchtroumpf;
	cout << ageGrandSchtroumpf << "Ohhh mais vous n'Ãtiez pas si vieu en fait Grand Schtroupf?!" << endl;

	return 0;
}

