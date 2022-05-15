var xValues = [];
var yValues = [];

var yTemp = [];
var xTemp = [];

var yCar = [];
var xCar = [];

var count = 0;
var count_Car = 0;
// get init value from database


function initHumi(){
	const xhttp = new XMLHttpRequest();
	xhttp.onload = function () {
		yValues = JSON.parse(this.responseText).humidata;
		xValues = JSON.parse(this.responseText).humitime;
		yTemp = JSON.parse(this.responseText).tempdata;
		xTemp = JSON.parse(this.responseText).temptime;
		yCar = JSON.parse(this.responseText).cardata;
		xCar = JSON.parse(this.responseText).cartime;
		// console.log(JSON.parse(this.responseText))
		updateChartInit(humiChart, xValues, yValues);
		updateChartInit(tempChart, xTemp, yTemp);
		updateChartInit(badCarChart, xCar, yCar);
	}
	xhttp.open("GET", "/initHumi");
	xhttp.send();
}


statistical_data();
function statistical_data(){
	const xhttp = new XMLHttpRequest();
	xhttp.onload = function () {
		temp = JSON.parse(this.responseText).temp;
		time = JSON.parse(this.responseText).time;
		carmonth = JSON.parse(this.responseText).carmonth;
		totalcar = JSON.parse(this.responseText).totalcar;
		totalcar2 = JSON.parse(this.responseText).totalcar2;
		document.querySelector('#report1').textContent = ""+ time.slice(0,9) +" (" + temp + " độ C)";
		document.querySelector('#report2').textContent = "Tháng "+ carmonth +" (" + totalcar + " xe)";
		document.querySelector('#report3').textContent =  totalcar2 + " xe";
		pushStatical(temp, time, totalcar2, carmonth)
	}
	xhttp.open("GET", "/statistical_data");
	xhttp.send();
}


function initDataForWeek(){
	const xhttp = new XMLHttpRequest();
	xhttp.onload = function () {
		updateChartInitWeek(humiChart_week, JSON.parse(this.responseText).humiweek, JSON.parse(this.responseText).humi)
		updateChartInitWeek(tempChart_week, JSON.parse(this.responseText).tempweek, JSON.parse(this.responseText).temp)
		updateChartInitWeek(humiChart_month, JSON.parse(this.responseText).humimonth, JSON.parse(this.responseText).humimonthdata)
		updateChartInitWeek(tempChart_month, JSON.parse(this.responseText).tempmonth, JSON.parse(this.responseText).tempmonthdata)
		updateChartInitWeek(badCarChart_week, JSON.parse(this.responseText).badcarweek, JSON.parse(this.responseText).badcarweekdata)
		updateChartInitWeek(badCarChart_month, JSON.parse(this.responseText).badcarmonth, JSON.parse(this.responseText).badcarmonthdata)
	}
	xhttp.open("GET", "/initWeek");
	xhttp.send();
}
function updateChartInitWeek(chartName, xarrayInput, yarrayInput) {
	yarray = chartName.data.datasets[0].data;
	xarray = chartName.data.labels;
	for (var i = 0; i < yarrayInput.length; i++) {
		yarray.push(yarrayInput[i])
		xarray.push(xarrayInput[i]);
	}
	chartName.options.scales.yAxes[0].ticks.max = Math.max.apply(Math, yarrayInput) + 15;
	chartName.update();
}
initDataForWeek()
initHumi();
setTimeout(live, 100);
function live() {
	$(document).ready(function () {
		function fetchData() {
			$.ajax({
				url: 'https://io.adafruit.com/api/v2/khanhtran01/feeds/humi-and-temp',
				success: function (result) {
					let time =  new Date().toLocaleTimeString();
					
					liveChartUpdate(humiChart,JSON.parse(result['last_value'])['humi'], time.slice(0,8), count);
					liveChartUpdate(tempChart,JSON.parse(result['last_value'])['temp'], time.slice(0,8), count);
					pushData(JSON.parse(result['last_value'])['humi'],JSON.parse(result['last_value'])['temp'])
					count ++;
				}
			});
		}
		fetchData()
		setInterval(fetchData, 1 * 60 * 60 * 1000);
	});
}
initTime()
function initTime() {
	$.ajax({
		url: 'https://io.adafruit.com/api/v2/khanhtran01/feeds/time',
		success: function (result) {
			// console.log(JSON.parse(result['last_value'])['valueTime'])
			var listTime = JSON.parse(result['last_value'])['valueTime'].map(Number);
			liveCar(listTime[2] + listTime[3])
		}
	});
}


var CarInterval;
function liveCar(time) {
	time = time * 1000
	clearInterval(CarInterval)
	$(document).ready(function () {
		function fetchData() {
			$.ajax({
				url: 'https://io.adafruit.com/api/v2/khanhtran01/feeds/bad-car',
				success: function (result) {
					let time =  new Date().toLocaleTimeString();
					liveChartUpdate(badCarChart,JSON.parse(result['last_value']), time.slice(0,8), count_Car);
					pushCar(JSON.parse(result['last_value']))
					count_Car ++;
				}
			});
		}
		CarInterval =setInterval(fetchData, time);
	});
}









function updateChartInit(chartName, xarrayInput, yarrayInput) {
	yarray = chartName.data.datasets[0].data;
	xarray = chartName.data.labels;
	for (var i = 0; i < yarrayInput.length; i++) {
		yarray.push(yarrayInput[i])
		if (count % 4 == 0) {
			xarray.push(xarrayInput[i].slice(11, 18));
		}
		else {
			xarray.push('');
		}
		count++;
		if (count > 4) {
			count = 1;
		}
	}
	chartName.options.scales.yAxes[0].ticks.max = Math.max.apply(Math, yarrayInput) + 5;
	// console.log(yarray);
	chartName.update();
}



function liveChartUpdate(chartName,y, x, count) {
	newy = y
	// newX = humiChart.data.labels[19] + 10;
	newX = count % 4 == 0 ? x : '';
	count++;
	// console.log(y);
	yarray = chartName.data.datasets[0].data;
	if (newy > Math.max.apply(Math, yarray)) {
		chartName.options.scales.yAxes[0].ticks.max = newy + 5;
	}
	chartName.data.labels.push(newX);
	chartName.data.datasets[0].data.push(newy);
	if (yarray.length > 20) {
		chartName.data.datasets[0].data.shift();
	}
	chartName.data.labels.shift()
	chartName.update();
}