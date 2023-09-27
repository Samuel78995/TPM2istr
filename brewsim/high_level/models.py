from django.db import models

# Create your models here.


class Departement(models.Model):
	numero = models.IntegerField()
	prixm2 = models.IntegerField()
	
	def __str__(self):
		return f"departement {self.numero}"

class Ingredient(models.Model):
	nom = models.CharField(max_length=100)
	
	def __str__(self):
		return f"{self.nom}"
	
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

class Machine(models.Model):
	nom = models.CharField(max_length=100)
	prix = models.IntegerField()
	
	def __str__(self):
		return f"{self.nom}"

class QuantiteIngredient(models.Model):
	ingredient = models.ForeignKey(Ingredient, # ou "self",
									on_delete=models.PROTECT,
									# blank=True, null=True,
									# related_name="+",
									)
	quantite = models.IntegerField()
	def __str__(self):
		return f"{self.quantite} kg de {self.ingredient}"

class Action(models.Model):
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
	def __str__(self):
		return f"{self.commande}"

class Recette(models.Model):
	nom = models.CharField(max_length=100)
	action = models.ForeignKey(Action, # ou "self",
									on_delete=models.PROTECT,
									# blank=True, null=True,
									# related_name="+",
									)
	def __str__(self):
		return f"{self.nom}"
									
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
