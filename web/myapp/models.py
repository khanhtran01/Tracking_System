from django.db import models
# from django.db.models.functions import 

# Create your models here


class Humis(models.Model):
    humi = models.IntegerField()
    time = models.DateTimeField()
    
class Temp(models.Model):
    temp = models.IntegerField()
    time = models.DateTimeField()
    
class BadCar(models.Model):
    badcar = models.IntegerField()
    time = models.DateTimeField()