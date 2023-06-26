//import { Chart } from './chart.js';

const wipeButton = document.getElementById("wipeButton");
const submitDatesButton = document.getElementById("submitDatesButton");
var startDateInput = document.getElementById("start-date");
var endDateInput = document.getElementById("end-date");
var tempArray = [];
var timeArray = [];



const ctx = document.getElementById('myChart').getContext('2d');
const tempChart = new Chart(ctx, {
  type: 'line',
  data: {
    labels: timeArray,
    datasets: [{
      label: 'Temperature from ' + timeArray[0] + ' to ' + timeArray[timeArray.length - 1],
      data: tempArray,
      borderWidth: 1,
      borderColor: '#48727d',
      backgroundColor: '#48727d',
    }]
  },
  options: {
    scales: {
      y: {
        beginAtZero: true
      }
    },
    responsive: false,
    maintainAspectRatio: false,
    
  }

});

// Create Temperature Gauge
var gaugeTemp = new LinearGauge({
  renderTo: 'gauge-temperature',
  width: 120,
  height: 400,
  units: "Temperature C",
  minValue: 0,
  startAngle: 90,
  ticksAngle: 180,
  maxValue: 40,
  colorValueBoxRect: "#049faa",
  colorValueBoxRectEnd: "#049faa",
  colorValueBoxBackground: "#f1fbfc",
  valueDec: 2,
  valueInt: 2,
  majorTicks: [
    "0",
    "5",
    "10",
    "15",
    "20",
    "25",
    "30",
    "35",
    "40"
  ],
  minorTicks: 4,
  strokeTicks: true,
  highlights: [
    {
      "from": 30,
      "to": 40,
      "color": "rgba(200, 50, 50, .75)"
    }
  ],
  colorPlate: "#fff",
  colorBarProgress: "#CC2936",
  colorBarProgressEnd: "#049faa",
  borderShadowWidth: 0,
  borders: false,
  needleType: "arrow",
  needleWidth: 2,
  needleCircleSize: 7,
  needleCircleOuter: true,
  needleCircleInner: false,
  animationDuration: 1500,
  animationRule: "linear",
  barWidth: 10,
}).draw();


function toggleCard(card) {
  card.classList.toggle('open');
}

function convertEpochArrayToDateTimeArray(epochArray) {
  const dateTimeArray = epochArray.map(epoch => {
    const date = new Date(epoch * 1000); // Multiply by 1000 to convert from seconds to milliseconds

    const year = date.getFullYear();
    const month = (date.getMonth() + 1).toString().padStart(2, '0'); // Month is zero-based, so add 1 and pad with leading zero if needed
    const day = date.getDate().toString().padStart(2, '0');
    const hours = date.getHours().toString().padStart(2, '0');
    const minutes = date.getMinutes().toString().padStart(2, '0');
    const seconds = date.getSeconds().toString().padStart(2, '0');

    const formattedDateTime = `${day}/${month}/${year} ${hours}:${minutes}:${seconds}`;
    return formattedDateTime;
  });

  return dateTimeArray;
}



const sendHttpRequest = (method, url, data, contentType) => {
  const headers = {};

  if (data) {
    if (contentType === 'application/json') {
      headers['Content-Type'] = 'application/json';
      console.log(data);
    } else if (contentType === 'text/plain') {
      headers['Content-Type'] = 'text/plain';
      data = String(data);
    }
  }

  return fetch(url, {
    method: method,
    body: data,
    headers: headers
  }).then(response => {
    if (response.status >= 400) {
      return response.text().then(errResData => {
        const error = new Error('Something went wrong!');
        error.data = errResData;
        throw error;
      });
    }

    if (contentType === 'application/json') {
      return response.json();
    } else if (contentType === 'text/plain') {
      return response.text();
    } else {
      throw new Error('Invalid content type');
    }
  });
};



const sendLocalRequest = (method, file, data) => {
  return new Promise((resolve, reject) => {
    // Read the local JSON file
    fetch(file)
      .then(response => {
        if (response.status >= 400) {
          // !response.ok
          throw new Error('Something went wrong!');
        }
        return response.json();
      })
      .then(jsonData => {
        resolve(jsonData); // Resolve the promise with the JSON data
      })
      .catch(error => {
        reject(error); // Reject the promise with the error
      });
  });
};

// get readings with XmlHttpRequest when it loads for the first time
function getReadings() {
  // Create a new XMLHttpRequest object to make an HTTP request
  var xhr = new XMLHttpRequest();

  // Open a GET request to the server endpoint "/readings"
  xhr.open("GET", "/readings", true);

  xhr.setRequestHeader("Content-Type", "application/json");

  // Event handler for the state change of the request
  xhr.onreadystatechange = function () {
    if (xhr.readyState == 4 && xhr.status == 200) {
      // Parse the response JSON data
      var myObj = JSON.parse(xhr.responseText);
      console.log(myObj);

      // Update the temperature gauge value with the fetched temperature value
      var temp = myObj.current_temperature;
      gaugeTemp.value = temp;
    }
  };

  // Send the GET request to the server endpoint "/readings"
  xhr.send();
}

// events from server
if (!!window.EventSource) {
  // Check if the browser supports EventSource (Server-Sent Events)

  // Create a new EventSource object and connect to the server endpoint "/events"
  var source = new EventSource('/events');

  // Event listener for the 'open' event of the EventSource connection
  source.addEventListener('open', function (e) {
    console.log("Events Connected");
  }, false);

  // Event listener for the 'error' event of the EventSource connection
  source.addEventListener('error', function (e) {
    if (e.target.readyState != EventSource.OPEN) {
      console.log("Events Disconnected");
    }
  }, false);

  // Event listener for the 'message' event of the EventSource connection
  source.addEventListener('message', function (e) {
    console.log("message", e.data);
  }, false);

  // Event listener for the 'new_readings' event of the EventSource connection
  source.addEventListener('new_readings', function (e) {
    console.log("new_readings", e.data);
    var myObj = JSON.parse(e.data);
    console.log(myObj);

    // Update the temperature gauge value with the new temperature value received
    gaugeTemp.value = myObj.current_temperature;
  }, false);
}


function submitDates() {
  var startDate = new Date(startDateInput.value + "Z");
  var endDate = new Date(endDateInput.value + "Z");

  // Check if the start date is before the end date
  var isStartDateBeforeEndDate = startDate < endDate;
  console.log("Is the start date before the end date?", isStartDateBeforeEndDate);

  //debugging
  /* var formattedStartDate = startDate.toLocaleString();
  var formattedEndDate = endDate.toLocaleString();
  console.log("unix start date:", Math.floor(startDate.getTime() / 1000));
  console.log("Formatted end date:", formattedEndDate);
 */

  // Create a JavaScript object
  const dates = {
    start_date_unix: Math.floor(startDate.getTime() / 1000),
    end_date_unix: Math.floor(endDate.getTime() / 1000)
  };
  // Convert the JavaScript object to a JSON string
  const datesJson = JSON.stringify(dates);

  console.log(datesJson);
  return datesJson;
}


function refreshChart() {
  console.log("refreshChart");
  tempChart.data.labels = timeArray;
  tempChart.data.datasets[0].data = tempArray;
  tempChart.data.datasets[0].label = 'Temperature from ' + timeArray[0] + ' to ' + timeArray[timeArray.length - 1];
  tempChart.update();

}


function createTable(timestampArray, temperatureArray) {

  const table = document.createElement("my-table");
  const tbody = table.querySelector("table-body");
  tbody.innerHTML = "";

  // Create table rows dynamically
  for (let i = 0; i < timestampArray.length; i++) {
    const row = document.createElement("tr");

    const timeCell = document.createElement("td");
    timeCell.textContent = timestampArray[i];
    row.appendChild(timeCell);

    const tempCell = document.createElement("td");
    tempCell.textContent = temperatureArray[i];
    row.appendChild(tempCell);
    table.appendChild(row);

    tbody.appendChild(row);
  }
  table.style.display = "table";
}



// Send dates to server and receive array with fetch
const sendDatesAndReceiveArray = () => {
  sendHttpRequest('POST', '/dates', submitDates(), "application/json")
    .then(responseData => {
      console.log(responseData);
      var myObj = responseData;
      tempArray = myObj.temperature_array;
      timeArray = convertEpochArrayToDateTimeArray(myObj.timestamp_array);

      refreshChart();
      createTable(timeArray, tempArray);
    })
    .catch(err => {
      console.log(err, err.data);
    });
};



// Wipe memory with fetch
const wipeMem = () => {
  sendHttpRequest('GET', '/wipe', null, "text/plain").then(responseData => {
    console.log(responseData);
  });
};


/* 
const getData = () => {
  sendHttpRequest('GET', 'https://reqres.in/api/users').then(responseData => {
    console.log(responseData);
  });
};
 
const sendData = () => {
  sendHttpRequest('POST', 'https://reqres.in/api/register', {
    email: 'eve.holt@reqres.in',
    password: 'pistol'
  })
    .then(responseData => {
      console.log(responseData);
    })
    .catch(err => {
      console.log(err, err.data);
    });
}; */

submitDatesButton.addEventListener('click', sendDatesAndReceiveArray);
wipeButton.addEventListener('click', wipeMem);
window.addEventListener('load', getReadings);
