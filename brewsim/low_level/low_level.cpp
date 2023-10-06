#include <iostream>
#include <cpr/cpr.h>

/*
class Departement(models.Model):
	numero = models.IntegerField()
	prixm2 = models.IntegerField()
	
	def __str__(self):
		return f"departement {self.numero}"
		
	def json(self) :
		dico = {}
		dico['numero'] = self.numero
		dico['prixm2'] = self.prixm2
		return dico
	def json_extended(self) :
		dico = {}
		dico['numero'] = self.numero
		dico['prixm2'] = self.prixm2
		return dico
*/
class Departement
{
	public :
		// Constructeur
		Departement(int numero, int prixm2);
		
		int numero_;
		int prixm2_;
		
		void afficher();	
};

// Constructeur
Departement::Departement(int numero, int prixm2) : numero_(numero), prixm2_(prixm2)
{}

//Affichage
void Departement::afficher()
{
	std::cout << "numero : " << this->numero_ << std::endl;
	std::cout << "prixm2 : " << this->prixm2_ << std::endl;
}

int main()
{
	Departement dep(81, 2000);
	dep.afficher();
	
	return 0;
}

