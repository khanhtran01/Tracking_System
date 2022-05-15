from django.contrib import admin
from .models import  Humis, Temp, BadCar
# Register your models here.
admin.site.register(Humis)
admin.site.register(Temp)
admin.site.register(BadCar)