from django.shortcuts import redirect, render
from django.http import HttpResponse, JsonResponse
from .models import Humis, Temp, BadCar
from .forms import ResgisterForm
from django.views import View
from django.contrib.auth import authenticate, login, logout
from django.contrib.auth.decorators import login_required
from django.db.models.functions import ExtractWeek, ExtractYear, ExtractMonth
from django.db.models import Avg, Sum
from django.contrib import messages
from Adafruit_IO import Client
import json
import datetime

# @login_required(login_url='myapp:login')
def pushControl(data):
    aio = Client('khanhtran01', 'aio_aVYg97AKI9fGh3lqSa9lShUqm48Z')
    feed = aio.feeds('control')
    res = ''
    if data == 'emer':
        res = '{"emer":"1","slow":"0"}'
    else: res = '{"emer":"0","slow":"1"}'
    aio.send_data(feed.key, res)

def pushTime(data):
    # {"changed":"0","valueTime":"[23,5,25,3]"}
    aio = Client('khanhtran01', 'aio_aVYg97AKI9fGh3lqSa9lShUqm48Z')
    if 'yellow' in data:
        feed = aio.feeds('time')
        res = '{"changed":"'+ str(data['action']) +  '","valueTime":[' + str(data['green']) + ',' + str(data['yellow']) + ',' + str(data['green2']) + ',' + str(data['yellow2']) +  ']}'
    else :
        feed = aio.feeds('control')
        res = '{"emer":"'+ str(data['emer']) +'","slow":"'+ str(data['slow']) + '"}'
    aio.send_data(feed.key, res)

def pushStatic(data):
    # {"changed":"0","valueTime":"[23,5,25,3]"}
    aio = Client('khanhtran01', 'aio_aVYg97AKI9fGh3lqSa9lShUqm48Z')
    feed = aio.feeds('statical')
    res = json.dumps(data)
    aio.send_data(feed.key, res)
    

def is_ajax(request):
    return request.META.get('HTTP_X_REQUESTED_WITH') == 'XMLHttpRequest'
# @login_required(login_url='myapp:login')
def index(request):
    return render(request, "myapp/userBoard.html")

@login_required(login_url='myapp:login')
def pushHumi(request):
    if request.method == "POST":
        # humiInput = Humis(humi = int(request.POST['humi']), time= datetime.datetime.now()) 
        # humiInput.save()
        return JsonResponse({'data': "here is new data", 'message' : "input complete"}, status=200)
    else:
        return JsonResponse({'data': "here is new data", 'message' : "failed"}, status=200)
    
# @login_required(login_url='myapp:login')
# def detailsView(request, idx):
#     q = Question.objects.get(pk=idx)
#     res = {
#         'details': q
#     }
#     return render(request, "myapp/detail.html", res)


@login_required(login_url='myapp:login')
def dashBoardView(request):
    if request.method == "POST":
        pushControl(request.POST['Setlight'])
        return render(request, "myapp/dashboard.html")
        
    
    return render(request, "myapp/dashboard.html")
def statistical_data(request):
    temp = Temp.objects.all().order_by('-temp')[:1]
    cars = (BadCar.objects
                .annotate(month = ExtractMonth('time'))
                .values('month')
                .annotate(total=Sum('badcar')))
    highest = cars.order_by('-total')[:1]
    total = BadCar.objects.aggregate(Sum('badcar'))
    des = {
        'temp' : temp[0].temp,
        'time' : temp[0].time,
        'carmonth' : highest[0]['month'],
        'totalcar' : highest[0]['total'],
        'totalcar2' : total['badcar__sum']
    }
    return JsonResponse(des, status=200)


def initDataWeek(request):
    Humidata = (Humis.objects
                .annotate(week = ExtractWeek('time'))
                .values('week')
                .annotate(avg_humi=Avg('humi')))
    Tempdata = (Temp.objects
                .annotate(week = ExtractWeek('time'))
                .values('week')
                .annotate(avg_temp=Avg('temp')))
    HumidataMonth = (Humis.objects
                .annotate(month = ExtractMonth('time'))
                .values('month')
                .annotate(avg_humi=Avg('humi')))
    TempdataMonth = (Temp.objects
                .annotate(month = ExtractMonth('time'))
                .values('month')
                .annotate(avg_temp=Avg('temp')))
    
    BadcarWeek = (BadCar.objects
                .annotate(week = ExtractWeek('time'))
                .values('week')
                .annotate(avg=Avg('badcar')))
    BadcarMonth = (BadCar.objects
                .annotate(month = ExtractMonth('time'))
                .values('month')
                .annotate(avg=Avg('badcar')))
    # Humidata.
    week = []
    humi = []
    humimonthdata = []
    hummimonth = []
    
    badcarmd = []
    badcarw = []
    badcarwd = []
    badcarm = []
    
    tempw = []
    temp = []
    tempmonth = []
    tempmonthdata = []
    
    for i in Humidata:
        week += [i['week']]
        humi += [i['avg_humi']]
    
    for i in Tempdata:
        tempw += [i['week']]
        temp += [i['avg_temp']]
    
    for i in HumidataMonth:
        hummimonth += [i['month']]
        humimonthdata += [i['avg_humi']]
    
    for i in TempdataMonth:
        tempmonth += [i['month']]
        tempmonthdata += [i['avg_temp']]
        
    for i in BadcarWeek:
        badcarw += [i['week']]
        badcarwd += [i['avg']]
        
    for i in BadcarMonth:
        badcarm += [i['month']]
        badcarmd += [i['avg']]
    
    res = {
        'humiweek' : week,
        'humi' : humi,
        'tempweek' : tempw,
        'temp' : temp,
        'humimonth' : hummimonth,
        'humimonthdata' : humimonthdata,
        'tempmonth' : tempmonth,
        'tempmonthdata' : tempmonthdata,
        'badcarweek' : badcarw,
        'badcarweekdata' : badcarwd,
        'badcarmonth' : badcarm,
        'badcarmonthdata' : badcarmd
    }
    return JsonResponse(res, status= 200)

# @login_required(login_url='myapp:login')
def initHumi(request):
    humis = Humis.objects.all().order_by('-id')[:20:-1]
    temps = Temp.objects.all().order_by('-id')[:20:-1]
    badcars = BadCar.objects.all().order_by('-id')[:20:-1]
    humilist = []
    humitime = []
    templist = []
    temptime = []
    badcarlist = []
    badcartime = []
    for humi in humis:
        humilist += [humi.humi]
        humitime += [humi.time]
    for temp in temps:
        templist += [temp.temp]
        temptime += [temp.time]
    for car in badcars:
        badcarlist += [car.badcar]
        badcartime += [car.time]
    
    resu = {
        'humidata' : humilist,
        'humitime' : humitime,
        'tempdata' : templist,
        'temptime' : temptime,
        'cardata' : badcarlist,
        'cartime' : badcartime
    }
    return JsonResponse(resu, status= 200)


@login_required(login_url='myapp:login')
def getNewestData(request):
    q = Humis.objects.all().order_by('-id')[:1]
    resu = {
        'data' : q[0].humi,
        'time' : q[0].time
    }
    return JsonResponse(resu, status= 200)
class LoginClass(View):
    def get(self, request):
        if request.user.is_authenticated:
            return redirect('myapp:dashboard')
        return render(request, "myapp/login.htm")
    
    def post(self, request):
        username = request.POST.get('username')
        password = request.POST.get('password')
        my_user = authenticate(request ,username = username, password = password)
        
        if my_user is not None:
            login(request, my_user)
            return redirect('myapp:dashboard')
        else:
            messages.info(request, 'Tên người dùng hoặc mật khẩu không đúng')
        
        return render(request, 'myapp/login.htm')
class RegisterClass(View):
    def get(self,request):
        if request.user.is_authenticated:
            return redirect('myapp:index')
        form = ResgisterForm()
        context = {'forms': form}
        return render(request, "myapp/register.htm", context)
    
    def post(self,request):
        form = ResgisterForm(request.POST)
        if form.is_valid():
            form.save()
            return redirect('myapp:login')
        context = {'forms': form}
        return render(request, "myapp/register.htm", context)
    
    
def logoutUser(request):
    logout(request)
    return redirect('myapp:login')

# @login_required(login_url='myapp:login')
def pushData(request):
    if request.method == "POST":
        if 'yellow_time' in request.POST:            
            yellow = request.POST['yellow_time']
            green = request.POST['green_time']
            yellow2 = request.POST['yellow_time2']
            green2 = request.POST['green_time2']
            action = request.POST['action']
            if (int(yellow) + int(green)) != (int(yellow2) + int(green2)):
                return JsonResponse({'status':False}, status=200)
            res = {
                'yellow' : yellow,
                'green' : green,
                'yellow2' : yellow2,
                'green2' : green2,
                'action' : action
            }
            pushTime(res)
            if action == '0':
                message = {
                    'status' : True,
                    'time' : int(yellow2) + int(green2)
                }
            else:
                message = {
                    'status' : True,
                }
            return JsonResponse(message, status=200)
        elif 'Setlight' in request.POST:
            res = {
                'emer' : 0,
                'slow' : 1
            }
            if request.POST['Setlight'] == 'emer':
                res = {
                    'emer' : 1,
                    'slow' : 0
                }
            elif request.POST['Setlight'] == 'normal':
                res = {
                    'emer' : 0,
                    'slow' : 0
                }
            pushTime(res)
            return JsonResponse({'status': "complete", 'message' : "success"}, status=200)
    elif request.method == "GET":
        if 'car' in request.GET:
            car = BadCar(badcar = int(request.GET['car']), time = datetime.datetime.now())
            car.save()
            return JsonResponse({'status': "complete", 'message' : "push car"}, status=200)
        if 'highest_temp' in request.GET:
            data = {
            'highest_temp': request.GET['highest_temp'],
            'highest_temp_time': request.GET['highest_temp_time'][0:10],
            'total_car': request.GET['total_car'],
            'most_car_month': request.GET['most_car_month']
            }
            pushStatic(data)
            return JsonResponse({'status': "complete", 'message' : "push statical"}, status=200)
        else:
            humi = Humis(humi = int(request.GET['humi']), time= datetime.datetime.now())
            temp = Temp(temp = int(request.GET['temp']), time= datetime.datetime.now())
            humi.save()
            temp.save()
            return JsonResponse({'status': "complete", 'message' : "push humi and temp"}, status=200)
