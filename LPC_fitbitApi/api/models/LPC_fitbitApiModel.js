'use strict';
var mongoose = require('mongoose');
var Schema = mongoose.Schema;

var ActivitySchema = new Schema({
	activityId:{
		type: Number,
	},
	activityParentId:{
		type: Number
	},
	calories:{
		type: Number
	},
	description:{
		type: String
	},
	distance:{
		type: Number
	},
	duration:{
		type:Number
	},
	hasStartTime:{
		type:Boolean
	},
	isFavorite:{
		type: Boolean
	},
	logId:{
		type: Number
	},
	name:{
		type:String
	},
	Created_date:{
		type: Date,
		default: Date.now
	},
	startTimeHour:{
		type: Number
	},
	startTimeMinute:{
		type: Number
	},
	steps:{
		type:Number
	}
});
module.exports = mongoose.model('Activities', ActivitySchema);