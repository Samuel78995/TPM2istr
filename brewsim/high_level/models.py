from django.db import models

# Create your models here.

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

class Ingredient(models.Model):
	nom = models.CharField(max_length=100)
	
	def __str__(self) :
		return f"{self.nom}"
		
	def json(self) :
		dico = {}
		dico['nom'] = self.nom
		return dico
		
	def json_extended(self) :
		dico = {}
		dico['nom'] = self.nom
		return dico
	
class Prix(models.Model):
	ingredient = models.ForeignKey(Ingredient, # ou "self",
									on_delete=models.PROTECT,
									# blank=True, null=True,
									# related_name="+",
									)
	departement = models.ForeignKey(Departement, # ou "self",
									on_delete=models.PROTECT,
									# blank=True, null=True,
									# related_name="+",
									)
	prix = models.IntegerField()
	
	def __str__(self):
		return f"{self.ingredient} dans le {self.departement} {self.prix} euros/kg"
		
	def json(self) :
		dico = {}
		dico['ingredient'] = self.ingredient.id
		dico['departement'] = self.departement.id
		dico['prix'] = self.prix
		return dico
	
	def json_extended(self) :
		dico = {}
		dico['ingredient'] = self.ingredient.json()
		dico['departement'] = self.departement.json()
		dico['prix'] = self.prix
		return dico
	

class Machine(models.Model):
	nom = models.CharField(max_length=100)
	prix = models.IntegerField()
	
	def __str__(self):
		return f"{self.nom}"
		
	def costs(self):
		return self.prix
		
	def json(self) :
		dico = {}
		dico['nom'] = self.nom
		dico['prix'] = self.prix
		return dico
		
	def json_extended(self) :
		dico = {}
		dico['nom'] = self.nom
		dico['prix'] = self.prix
		return dico

class QuantiteIngredient(models.Model):
	ingredient = models.ForeignKey(Ingredient, # ou "self",
									on_delete=models.PROTECT,
									# blank=True, null=True,
									# related_name="+",
									)
	quantite = models.IntegerField()
	
	def __str__(self):
		return f"{self.quantite} kg de {self.ingredient}"
	
	def costs(self,departement):
		return self.ingredient.prix_set.get(departement__numero = departement.numero).prix * self.quantite
		
	def json(self) :
		dico = {}
		dico['ingredient'] = self.ingredient.id
		dico['quantite'] = self.quantite
		return dico
	
	def json_extended(self) :
		dico = {}
		dico['ingredient'] = self.ingredient.json()
		dico['quantite'] = self.quantite
		return dico

class Action(models.Model) :
	machine = models.ForeignKey(Machine, # ou "self",
									on_delete=models.PROTECT,
									# blank=True, null=True,
									# related_name="+",
									)
	commande = models.CharField(max_length=100)
	duree = models.IntegerField()
	ingredients = models.ManyToManyField(QuantiteIngredient)
	action = models.ForeignKey("self", # ou "self",
									on_delete=models.PROTECT,
									blank=True, null=True,
									related_name="+"
									)
	def __str__(self) :
		return f"{self.commande}"
		
	def json(self) :
		dico = {}
		dico['machine'] = self.machine.id
		dico['commande'] = self.commande
		dico['duree'] = self.duree
		
		ingredientsid = []
		for ingredient in self.ingredients.all() :
			ingredientsid.append(ingredient.id)
		dico['ingredients'] = ingredientsid

		if self.action is not None :
			dico['action'] = self.action.id
		return dico
		
	def json_extended(self) :
		dico = {}
		dico['machine'] = self.machine.json()
		dico['commande'] = self.commande
		dico['duree'] = self.duree
		
		ingredientsid = []
		for ingredient in self.ingredients.all() :
			ingredientsid.append(ingredient.json())
		dico['ingredients'] = ingredientsid

		if self.action is not None :
			dico['action'] = self.action.json_extended()
		return dico

class Recette(models.Model):
	nom = models.CharField(max_length=100)
	action = models.ForeignKey(Action, # ou "self",
									on_delete=models.PROTECT,
									# blank=True, null=True,
									# related_name="+",
									)
	def __str__(self):
		return f"{self.nom}"
		
	def json(self) :
		dico = {}
		dico['nom'] = self.nom
		dico['action'] = self.action.id
		return dico
	
	def json_extended(self) :
		dico = {}
		dico['nom'] = self.nom
		dico['action'] = self.action.json()
		return dico
									
class Usine(models.Model):
	departement = models.ForeignKey(Departement, # ou "self",
									on_delete=models.PROTECT,
									# blank=True, null=True,
									# related_name="+",
									)
	taille = models.IntegerField()
	machines = models.ManyToManyField(Machine)
	recettes = models.ManyToManyField(Recette)
	stocks = models.ManyToManyField(QuantiteIngredient)
	
	def __str__(self):
		return f"Usine de {self.taille} m2"
		
	def costs(self):
		prix_usine = self.departement.prixm2 * self.taille
		
		prix_machine = 0
		for machine in self.machines.all() :
			prix_machine += machine.prix
			
		prix_stocks = 0
		for stock in self.stocks.all() :
			prix_stocks += stock.costs(self.departement)
		
		return prix_usine + prix_machine + prix_stocks

	def json(self) :
		dico = {}
		dico['departement'] = self.departement.id
		dico['taille'] = self.taille
		#boucle
		machinesid = []
		for machine in self.machines.all() :
			machinesid.append(machine.id)
		dico['machines'] = machinesid
		
		recettesid = []
		for recette in self.recettes.all() :
			recettesid.append(recette.id)
		dico['recettes'] = recettesid
		
		stocksid = []
		for stock in self.stocks.all() :
			stocksid.append(stock.id)
		dico['stocks'] = stocksid
			
		return dico
		
	def json_extended(self) :
		dico = {}
		dico['departement'] = self.departement.json_extended()
		dico['taille'] = self.taille
		#boucle
		machinesid = []
		for machine in self.machines.all() :
			machinesid.append(machine.json_extended())
		dico['machines'] = machinesid
		
		recettesid = []
		for recette in self.recettes.all() :
			recettesid.append(recette.json_extended())
		dico['recettes'] = recettesid
		
		stocksid = []
		for stock in self.stocks.all() :
			stocksid.append(stock.json_extended())
		dico['stocks'] = stocksid
			
		return dico


"""	
	def approvisionnement(self) : 
		# creation du dictionnaire
		a_acheter = {}
		for ingredient in Ingredient.objects.all() :
			a_acheter[ingredient] = 0

		# quantites requises pour chaque ingredient
		for recette in self.recettes.all() :
			action = recette.action
			if action is not None :
				for ingredient in action.ingredients.all() :
					a_acheter[ingredient.ingredient.nom] += ingredient.quantite
				action = action.action

		for ingredient, quantite in a_acheter.items() :		
			self.stocks.add(QuantiteIngredient.objects.create(ingredient = ingredient, quantite = quantite)
			
"""
