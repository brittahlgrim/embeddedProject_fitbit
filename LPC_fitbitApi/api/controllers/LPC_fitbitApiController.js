var mongoose = require('mongoose'),
	Activity = mongoose.model('Activities');

//returns only the activities for the current day
exports.list_all_activities = function(req, res){
	//todo: make a response to fitbit api here
	var fitBitResponse = exampleFitBitResponse;
	var responseParsed = JSON.parse(fitBitResponse);
	console.log(responseParsed);
	// Activity.find({}, function(err, activity){
	// 	if(err)
	// 		res.send(err);
	// 	//todo: uncomment this once the fitbit api call is actually being made
	// 	activity = exampleFitBitResponse;
	// 	res.json(activity);
	// });
};

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
}