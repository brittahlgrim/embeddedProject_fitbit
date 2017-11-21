const express = require('express');
var request = require('request');
const app = express();
var port = process.env.PORT || 3000;
var mongoose = require('mongoose');
var Activity = require('./api/models/LPC_fitbitApiModel');
var bodyParser = require('body-parser');

mongoose.Promise = global.Promise;
mongoose.connect('mongodb://localhost/LPC_fitbitApidb', {useMongoClient: true});

app.use(bodyParser.urlencoded({extended:true}));
//todo: app.use(bodyParser.json);

app.get('/', (req, res) => res.send('Hello World!'));

app.get('/activities', function(req, res){
  var fitBitResponse = JSON.stringify(exampleFitBitResponse);
  var responseParsed = JSON.parse(fitBitResponse);
  var dataByHour = responseParsed['activities-steps-intraday'].dataset;
  console.log(dataByHour);

  dataByHour.forEach(function(hour){
    console.log(hour.time);
    console.log(hour.value);
    setTimeout(function(){
      request({
        uri: 'http://192.168.4.1:80/?' + hour,
        function(error, response, body) {
        }
      });
    }, 3000);

    console.log("hour:\r\n");
    console.log(hour);
  });

  res.json(dataByHour);
});

app.listen(3000, () => console.log('Example app listening on port 3000!'));

var exampleFitBitResponse = {
  "activities-steps":[
        {"dateTime":"2014-09-05","value":1433}
    ],
    "activities-steps-intraday":{
        "dataset":[
            {"time":"00:00:00","value":0},
            {"time":"00:01:00","value":0},
            {"time":"00:02:00","value":0},
            {"time":"00:03:00","value":0},
            {"time":"00:04:00","value":0},
            {"time":"00:05:00","value":287},
            {"time":"00:06:00","value":287},
            {"time":"00:07:00","value":287},
            {"time":"00:08:00","value":287},
            {"time":"00:09:00","value":287},
            {"time":"00:10:00","value":0},
            {"time":"00:11:00","value":0},
        ],
        "datasetInterval":1,
        "datasetType": "minute"
    }
};