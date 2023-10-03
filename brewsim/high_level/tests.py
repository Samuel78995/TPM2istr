from django.test import TestCase

# Create your tests here.
from django.test import TestCase
from .models import Machine
from .models import Departement
from .models import Ingredient
from .models import QuantiteIngredient
from .models import Prix
from .models import Usine
'''
class MachineModelTests(TestCase):
	def test_usine_creation(self):
		self.assertEqual(Machine.objects.count(), 0)
		Machine.objects.create(nom="four",prix=1_000)
		self.assertEqual(Machine.objects.count(), 1)
'''
class MachineModelTests(TestCase):
	def test_usine_creation(self):
		dep = Departement.objects.create(numero = 31, prixm2 = 2000)
		four = Machine.objects.create(nom="four",prix = 1000)
		malaxeur = Machine.objects.create(nom="malaxeur",prix = 2000)
		houblon = Ingredient.objects.create(nom="houblon")
		orge = Ingredient.objects.create(nom="orge")

		stock_houblon = QuantiteIngredient.objects.create(ingredient = houblon, quantite = 50)
		stock_orge = QuantiteIngredient.objects.create(ingredient = orge, quantite = 100)
		
		Prix.objects.create(ingredient = houblon, departement = dep, prix = 20)
		Prix.objects.create(ingredient = orge, departement = dep, prix = 10)
		
		usine = Usine.objects.create(departement = dep, taille = 50)
		usine.machines.add(four)
		usine.machines.add(malaxeur)
		usine.stocks.add(stock_houblon)
		usine.stocks.add(stock_orge)
		
		self.assertEqual(Usine.objects.first().costs(), 105000)
