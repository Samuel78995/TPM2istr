#include <iostream>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;


class Departement
{
	public :
		// Constructeur
		Departement(int numero, int prixm2);
		
		// Constructeur JSON
		Departement(json fich);
		
		// Constructeur ID
		Departement(int id);
		
		int numero_;
		int prixm2_;
		
		void afficher();	
};

// Constructeur
Departement::Departement(int numero, int prixm2) : numero_(numero), prixm2_(prixm2)
{}
// Constructeur JSON
Departement::Departement(json fich) : numero_(fich["numero"]), prixm2_(fich["prixm2"])
{}
// Constructeur ID
Departement::Departement(int id)
{
	std::string lien = "http://localhost:8000/departement/" + std::to_string(id);
	cpr::Response r  = cpr::Get(cpr::Url(lien));
	json fich = json::parse(r.text);
	numero_ = fich["numero"]; prixm2_ = fich["prixm2"];
}

class Ingredient
{
	public :
		// Constructeur ID
		Ingredient(int id);
		std::string nom_;
};

class Prix
{
	public :
		Prix(int id);
		
		std::unique_ptr<Ingredient> ingredient_;
		std::unique_ptr<Departement> departement_;
		int prix_;
		
};

class Machine
{
	public :
		Machine(int id);
	
		std::string nom_;
		int prix_;
		
};

class QuantiteIngredient
{
	public :
		QuantiteIngredient(int id);
		
		std::unique_ptr<Ingredient> ingredient_;
		//std::make_unique<Ingredient>(4)
		int quantite_;
		
};


class Action
{
	public :
		Action(int id);
	
		std::unique_ptr<Machine> machine_;
		std::string commande_;
		int duree_;
		std::unique_ptr<QuantiteIngredient> ingredients_;
		std::optional<std::unique_ptr<Action>> action_;
		
};

class Recette
{
	public :
		Recette(int id);
	
		std::string nom_;
		std::unique_ptr<Action> action_;	
};
/*
class Usine
{
	public :
		Usine(int id);
	
		std::unique_ptr<Departement> departement_;
		int taille_;
		std::unique_ptr<Departement> machines_;
		std::vector<unique_ptr<Recette>> recettes_;
		std::unique_ptr<QuantiteIngredient> stocks_;
};
*/


Ingredient::Ingredient(int id)
{
	std::string lien = "http://localhost:8000/ingredient/" + std::to_string(id);
	cpr::Response r  = cpr::Get(cpr::Url(lien));
	json fich = json::parse(r.text);
	nom_ = fich["nom"];
}

Prix::Prix(int id)
{
	std::string lien = "http://localhost:8000/prix/" + std::to_string(id);
	cpr::Response r  = cpr::Get(cpr::Url(lien));
	json fich = json::parse(r.text);
	ingredient_ = std::make_unique<Ingredient>(fich["ingredient"]); 
	departement_ = std::make_unique<Departement>(fich["departement"]);
	prix_ = fich["prix"];
}

Machine::Machine(int id)
{
	std::string lien = "http://localhost:8000/machine/" + std::to_string(id);
	cpr::Response r  = cpr::Get(cpr::Url(lien));
	json fich = json::parse(r.text);
	nom_ = fich["nom"]; prix_ = fich["prix"];
}

QuantiteIngredient::QuantiteIngredient(int id)
{
	std::string lien = "http://localhost:8000/quantiteingredient/" + std::to_string(id);
	cpr::Response r  = cpr::Get(cpr::Url(lien));
	json fich = json::parse(r.text);
	ingredient_ = std::make_unique<Ingredient>(fich["ingredient"]); 
	quantite_ = fich["quantite"];
}

Action::Action(int id)
{
	std::string lien = "http://localhost:8000/action/" + std::to_string(id);
	cpr::Response r  = cpr::Get(cpr::Url(lien));
	json fich = json::parse(r.text);
	machine_ = std::make_unique<Machine>(fich["machine"]); 
	commande_ = fich["commande"];
	duree_ = fich["duree"];
	ingredients_ = std::make_unique<QuantiteIngredient>(fich["ingredients"]); 
	action_ = std::make_unique<Action>(fich["action"]); 
}



//Affichage
void Departement::afficher()
{
	std::cout << "numero : " << this->numero_ << std::endl;
	std::cout << "prixm2 : " << this->prixm2_ << std::endl;
}


int main()
{
	/*
	Departement dep(81, 2000);
	dep.afficher();*/
	
	// REQUETE HTTP
	cpr::Response r  = cpr::Get(cpr::Url("http://localhost:8000/ingredient/1"));
	//std::cout << r.status_code << "\n";
	std::cout << "Reponse HTTP :" << std::endl;
    std::cout << r.text << "\n";     // JSON text string
    
    // JSON PARSE
    json ex1 = json::parse(r.text);
    std::cout << "Le JSON :" << std::endl;
    std::cout << ex1 << "\n";
    /*
    std::cout << "---------------------------------" << std::endl;
    std::cout << "Constructeur avec JSON :" << std::endl;
    Departement dep2(ex1);
    dep2.afficher();
    
    std::cout << "Constructeur avec ID :" << std::endl;
	Departement dep3(1);
    dep3.afficher();*/
    
	return 0;
}

