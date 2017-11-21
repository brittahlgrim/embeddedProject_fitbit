const express = require('express')
const app = express();
var port = process.env.PORT || 3000;
var mongoose = require('mongoose');
var Activity = require('./api/models/LPC_fitbitApiModel');
var bodyParser = require('body-parser');

mongoose.Promise = global.Promise;
mongoose.connect('mongodb://localhost/LPC_fitbitApidb', {useMongoClient: true});

app.use(bodyParser.urlencoded({extended:true}));
//todo: app.use(bodyParser.json);

var routes = require('./api/routes/LPC_fitbitApiRoutes');

app.get('/', (req, res) => res.send('Hello World!'));

app.get('/activities', function(req, res){
  Activity.find({}, function(err, activity){
    if(err)
      res.send(err);
    res.json(activity);
  });
});

app.listen(3000, () => console.log('Example app listening on port 3000!'))