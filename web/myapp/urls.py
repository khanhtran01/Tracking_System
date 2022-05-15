from django.urls import path
from . import views
from .views import LoginClass, RegisterClass

app_name = "myapp"

urlpatterns = [
    path('', views.index, name="index"),
    path('dashboard/', views.dashBoardView, name="dashboard"),
    path('pushHumi/', views.pushHumi, name="pushHumi"),
    path('initHumi/', views.initHumi, name="trang2"),
    path("initWeek/", views.initDataWeek, name="initWeek"),
    path('trang3', views.getNewestData, name="getNewestData"),
    path('login/', LoginClass.as_view(), name="login"),
    path('register/', RegisterClass.as_view(), name="register"),
    path('logout/', views.logoutUser, name="logout"),
    path('pushData/', views.pushData, name="pushData"),
    path('statistical_data/', views.statistical_data, name="staticaldata"),
]