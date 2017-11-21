var mongoose = require('mongoose'),
	Activity = mongoose.model('Activities');

//returns only the activities for the current day
exports.list_all_activities = function(req, res){
	Activity.find({}, function(err, activity){
		if(err)
			res.send(err);
		res.json(activity);
	});
};