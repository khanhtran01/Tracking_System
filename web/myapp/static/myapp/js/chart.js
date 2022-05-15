
var xValues = [];
var yValues = [];

var humiChart = new Chart("humiChart", {
  type: "line",
  data: {
    labels: xValues,
    datasets: [{
      fill: false,
      lineTension: 0,
      backgroundColor: "rgba(0,0,255,1.0)",
      borderColor: "rgba(0,0,255,0.1)",
      data: yValues
    }]
  },
  options: {
    legend: { display: false },
    scales: {
      yAxes: [{ ticks: { min: 0, max: 16 } }],
    },
    title: {
      display: true,
      text: "Biểu đồ độ ẩm tại Ngã tư Hàng Xanh"
    }
  }
});
var tempy = [];
var tempx = [];

var tempChart = new Chart("tempChart", {
  type: "line",
  data: {
    labels: tempx,
    datasets: [{
      fill: false,
      lineTension: 0,
      backgroundColor: "rgba(0,0,255,1.0)",
      borderColor: "rgba(0,0,255,0.1)",
      data: tempy
    }]
  },
  options: {
    legend: { display: false },
    scales: {
      yAxes: [{ ticks: { min: 0, max: 16 } }],
    },
    title: {
      display: true,
      text: "Biểu đồ nhiệt độ tại Ngã tư Hàng Xanh"
    }
  }
});


var badcary= [];
var badcarx = [];
var badCarChart = new Chart("badCarChart", {
  type: "line",
  data: {
    labels: badcarx,
    datasets: [{
      fill: false,
      lineTension: 0,
      backgroundColor: "rgba(0,0,255,1.0)",
      borderColor: "rgba(0,0,255,0.1)",
      data: badcary
    }]
  },
  options: {
    legend: { display: false },
    scales: {
      yAxes: [{ ticks: { min: 0, max: 16 } }],
    },
    title: {
      display: true,
      text: "Biểu đồ số xe vượt đèn đỏ tại Ngã tư Hàng Xanh"
    }
  }
});

// humiChart_week
var badcary= [];
var badcarx = [];
var humiChart_week = new Chart("humiChart_week", {
  type: "line",
  data: {
    labels: badcarx,
    datasets: [{
      fill: false,
      lineTension: 0,
      backgroundColor: "rgba(0,0,255,1.0)",
      borderColor: "rgba(0,0,255,0.1)",
      data: badcary
    }]
  },
  options: {
    legend: { display: false },
    scales: {
      yAxes: [{ ticks: { min: 0, max: 16 } }],
    },
    title: {
      display: true,
      text: "Biểu đồ độ ẩm tại Ngã tư Hàng Xanh"
    }
  }
});
tempw = []
tempwd = []
// tempChart_week
var tempChart_week = new Chart("tempChart_week", {
  type: "line",
  data: {
    labels: tempw,
    datasets: [{
      fill: false,
      lineTension: 0,
      backgroundColor: "rgba(0,0,255,1.0)",
      borderColor: "rgba(0,0,255,0.1)",
      data: tempwd
    }]
  },
  options: {
    legend: { display: false },
    scales: {
      yAxes: [{ ticks: { min: 0, max: 16 } }],
    },
    title: {
      display: true,
      text: "Biểu đồ nhiệt độ tại Ngã tư Hàng Xanh"
    }
  }
});

humim = []
humimd = []
// tempChart_week
var humiChart_month = new Chart("humiChart_month", {
  type: "line",
  data: {
    labels: humim,
    datasets: [{
      fill: false,
      lineTension: 0,
      backgroundColor: "rgba(0,0,255,1.0)",
      borderColor: "rgba(0,0,255,0.1)",
      data: humimd
    }]
  },
  options: {
    legend: { display: false },
    scales: {
      yAxes: [{ ticks: { min: 0, max: 16 } }],
    },
    title: {
      display: true,
      text: "Biểu đồ độ ẩm tại Ngã tư Hàng Xanh"
    }
  }
});

tempm = []
tempmd = []
// tempChart_week
var tempChart_month = new Chart("tempChart_month", {
  type: "line",
  data: {
    labels: tempm,
    datasets: [{
      fill: false,
      lineTension: 0,
      backgroundColor: "rgba(0,0,255,1.0)",
      borderColor: "rgba(0,0,255,0.1)",
      data: tempmd
    }]
  },
  options: {
    legend: { display: false },
    scales: {
      yAxes: [{ ticks: { min: 0, max: 16 } }],
    },
    title: {
      display: true,
      text: "Biểu đồ nhiệt độ tại Ngã tư Hàng Xanh"
    }
  }
});

// badCarChart_week
// badCarChart_month
var badcarw = []
var badcarwd = []
var badCarChart_week = new Chart("badCarChart_week", {
  type: "line",
  data: {
    labels: badcarw,
    datasets: [{
      fill: false,
      lineTension: 0,
      backgroundColor: "rgba(0,0,255,1.0)",
      borderColor: "rgba(0,0,255,0.1)",
      data: badcarwd
    }]
  },
  options: {
    legend: { display: false },
    scales: {
      yAxes: [{ ticks: { min: 0, max: 16 } }],
    },
    title: {
      display: true,
      text: "Biểu đồ tổng số xe vượt đèn đỏ theo tháng tại Ngã tư Hàng Xanh"
    }
  }
});


var badcarm = []
var badcarmd = []

var badCarChart_month = new Chart("badCarChart_month", {
  type: "line",
  data: {
    labels: badcarm,
    datasets: [{
      fill: false,
      lineTension: 0,
      backgroundColor: "rgba(0,0,255,1.0)",
      borderColor: "rgba(0,0,255,0.1)",
      data: badcarmd
    }]
  },
  options: {
    legend: { display: false },
    scales: {
      yAxes: [{ ticks: { min: 0, max: 16 } }],
    },
    title: {
      display: true,
      text: "Biểu đồ tổng số xe vượt đèn đỏ theo tuần tại Ngã tư Hàng Xanh"
    }
  }
});