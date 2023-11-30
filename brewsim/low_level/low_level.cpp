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
		//Departement(json fich);
		
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
//Departement::Departement(json fich) : numero_(fich["numero"]), prixm2_(fich["prixm2"])
//{}
// Constructeur ID
Departement::Departement(int id)
{
	std::string lien = "http://localhost:8000/departement/" + std::to_string(id);
	cpr::Response r  = cpr::Get(cpr::Url(lien));
	json fich = json::parse(r.text);
	numero_ = fich["numero"]; 
	prixm2_ = fich["prixm2"];
}

class Ingredient
{
	public :
		// Constructeur ID
		Ingredient(int id);
		std::string nom_;
		
		void afficher();
};

class Prix
{
	public :
		Prix(int id);
		
		std::unique_ptr<Ingredient> ingredient_;
		std::unique_ptr<Departement> departement_;
		int prix_;
		
		void afficher();
		
};

class Machine
{
	public :
		Machine(int id);
	
		std::string nom_;
		int prix_;
		
		void afficher();
		
};

class QuantiteIngredient
{
	public :
		QuantiteIngredient(int id);
		
		std::unique_ptr<Ingredient> ingredient_;
		//std::make_unique<Ingredient>(4)
		int quantite_;
		
		void afficher();
};

class Action
{
	public :
		Action(int id);
	
		std::unique_ptr<Machine> machine_;
		std::string commande_;
		int duree_;
		std::vector<std::unique_ptr<QuantiteIngredient>> ingredients_;
		std::optional<std::unique_ptr<Action>> action_;	
		
		void afficher();
};

class Recette
{
	public :
		Recette(int id);
		std::string nom_;
		std::unique_ptr<Action> action_;	
		
		void afficher();
};

class Usine
{
	public :
		Usine(int id);
	
		std::unique_ptr<Departement> departement_;
		int taille_;
		std::vector<std::unique_ptr<Machine>> machines_;
		std::vector<std::unique_ptr<Recette>> recettes_;
		std::vector<std::unique_ptr<QuantiteIngredient>> stocks_;
		
		void afficher();
};



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

	std::vector<std::unique_ptr<QuantiteIngredient>> ingredients_;

	for (const auto &ingredient: (fich["ingredients"])) {
		ingredients_.push_back(std::make_unique<QuantiteIngredient>(ingredient));
	}
	
	action_ = std::make_unique<Action>(fich["action"]);
}

Recette::Recette(int id)
{
	std::string lien = "http://localhost:8000/recette/" + std::to_string(id);
	cpr::Response r  = cpr::Get(cpr::Url(lien));
	json fich = json::parse(r.text);
	
	nom_ = fich["nom"];
	action_ = std::make_unique<Action>(fich["action"]);
}

Usine::Usine(int id)
{
	std::string lien = "http://localhost:8000/usine/" + std::to_string(id);
	cpr::Response r  = cpr::Get(cpr::Url(lien));
	json fich = json::parse(r.text);
	
	departement_ = std::make_unique<Departement>(fich["departement"]);
	taille_ = fich["taille"];
	
	std::vector<std::unique_ptr<Machine>> machines_;
	for (const auto &machine : (fich["machines"])) {
		machines_.push_back(std::make_unique<Machine>(machine));
	}
	
	std::vector<std::unique_ptr<QuantiteIngredient>> stocks_;
	for (const auto &stock : (fich["stocks"])) {
		stocks_.push_back(std::make_unique<QuantiteIngredient>(stock));
	}
}


//Affichage
void Departement::afficher()
{
	std::cout << "numero : " << this->numero_ << std::endl;
	std::cout << "prixm2 : " << this->prixm2_ << std::endl;
}

void Ingredient::afficher()
{
	std::cout << "nom : " << this->nom_ << std::endl;
}


void Prix::afficher()
{
	std::cout << "ingredient : " << this->ingredient_->nom_ << std::endl;
	std::cout << " departement : " << this->departement_->numero_ << std::endl;
	std::cout << " prix : " << this->prix_ << std::endl;
}


void Machine::afficher()
{
	std::cout << "nom : " << this->nom_ << std::endl;
	std::cout << "prix : " << this->prix_ << std::endl;
}

void QuantiteIngredient::afficher()
{
	std::cout << "ingredient : " << this->ingredient_->nom_ << std::endl;
	std::cout << "quantite : " << this->quantite_ << std::endl;
}



void Action::afficher()
{
	std::cout << "machine : " << this->machine_->nom_ << std::endl;
	std::cout << "commande : " << this->commande_ << std::endl;
	std::cout << "duree : " << this->duree_ << std::endl;
	std::cout << "ingredients : " << this->prixm2_ << std::endl;
	std::cout << "action : " << this->prixm2_ << std::endl;
}
/*
void Recette::afficher()
{
	std::cout << "numero : " << this->numero_ << std::endl;
	std::cout << "prixm2 : " << this->prixm2_ << std::endl;
}

void Usine::afficher()
{
	std::cout << "numero : " << this->numero_ << std::endl;
	std::cout << "prixm2 : " << this->prixm2_ << std::endl;
}

*/
int main()
{
	/*
	Departement dep(81, 2000);
	dep.afficher();*/
	/*
	// REQUETE HTTP
	cpr::Response r  = cpr::Get(cpr::Url("http://localhost:8000/action/1"));
	//std::cout << r.status_code << "\n";
	std::cout << "Reponse HTTP :" << std::endl;
    std::cout << r.text << "\n";     // JSON text string
    
    // JSON PARSE
    json ex1 = json::parse(r.text);
    std::cout << "Le JSON :" << std::endl;
    std::cout << ex1 << "\n";
    */
    /*
    std::cout << "---------------------------------" << std::endl;
    std::cout << "Constructeur avec JSON :" << std::endl;
    Action dep2(ex1);
    dep2.afficher();
    
    std::cout << "Constructeur avec ID :" << std::endl;
	Action dep3(1);
    dep3.afficher();
    */
    
    //Departement dep(1);
    //dep.afficher();
    std::cout << "---------------------------------" << std::endl;
    Ingredient ing(1);
    ing.afficher();
    
    std::cout << "---------------------------------" << std::endl;
    Prix p(1);
    p.afficher();
    
    std::cout << "---------------------------------" << std::endl;
    Machine m(1);
    m.afficher();
    
    std::cout << "---------------------------------" << std::endl;
    QuantiteIngredient qi(3);
    qi.afficher();
    
    std::cout << "---------------------------------" << std::endl;
    Action a(1);
    a.afficher();
    
    
	return 0;
}

