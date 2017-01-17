var express = require('express');
var path = require('path');
var favicon = require('serve-favicon');
var logger = require('morgan');
var cookieParser = require('cookie-parser');
var bodyParser = require('body-parser');
var SerialPort = require('serialport');
var five = require('johnny-five');
var mongo = require('mongodb').MongoClient;
var assert = require('assert');
var moment = require('moment');


var index = require('./routes/index');
var users = require('./routes/users');

var app = express();

//MongoDB url
var url = 'mongodb://localhost:27017/accelDataDB';

//accelerometer variables
var x;
var y;
var z;
var accelPitch;
var accelRoll;
var acceleration;
var inclination;
var orientation;

//altimeter variables
var altFeet;
var altMeters;

//get data from johnny-five
var five = require("johnny-five");
var board = new five.Board();

board.on("ready", function() {
  var accelerometer = new five.Accelerometer({
    controller: "MMA8452",
    sensitivity: 256  //1024-2g, 512-4g, 256-8g
  });

  accelerometer.on("change", function() {
    x = this.x;
    y = this.y;
    z = this.z;
    accelPitch = this.pitch;
    accelRoll = this.roll;
    acceleration = this.acceleration;
    inclination = this.inclination;
    orientation = this.orientation;

    console.log("MMA8452");
    console.log("X ", x);
    console.log("Y ", y);
    console.log("Z ", z);
    console.log("accelPitch ", accelPitch);
    console.log("accelRoll ", accelRoll);

    storeData(x, y, z, accelPitch, accelRoll);

  });

  // var altimeter = new five.Altimeter({
  //   controller: "MPL3115A2",
  //   elevation: 329 //City of Waterloo elevation
  // });
  //
  // altimeter.on("data", fundction(){
  //   altFeet = this.feet;
  //   altMeters = this.meters;
  //
  //   console.log("MPL3115A2");
  //   console.log("Altitude in feet: ", altFeet);
  //   console.log("Altitude in meters: ", altMeters);
  // });

  ["tap", "tap:single", "tap:double"].forEach(function(event) {
    accelerometer.on(event, function() {
      console.log(event);
    });
  });
});

// view engine setup
app.set('views', path.join(__dirname, 'views'));
app.set('view engine', 'jade');

// uncomment after placing your favicon in /public
//app.use(favicon(path.join(__dirname, 'public', 'favicon.ico')));
app.use(logger('dev'));
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));

app.use('/', index);
app.use('/users', users);

// catch 404 and forward to error handler
app.use(function(req, res, next) {
  var err = new Error('Not Found');
  err.status = 404;
  next(err);
});

// error handler
app.use(function(err, req, res, next) {
  // set locals, only providing error in development
  res.locals.message = err.message;
  res.locals.error = req.app.get('env') === 'development' ? err : {};

  // render the error page
  res.status(err.status || 500);
  res.render('error');
});

function storeData(aX, aY, aZ, aPitch, aRoll){
  var item = {
    x: aX,
    y: aY,
    z: aZ,
    pitch: aPitch,
    roll: aRoll,
    created: moment().format('MMMM Do h:mm:ss:SS')
  };

  mongo.connect(url, function (err, db){
    assert.equal(null, err);
    db.collection('TestCollection').insertOne(item, function(err, result){
      assert.equal(null, err);
      console.log('Item inserted');
      console.log(item);
      console.log('_______________________');
      db.close();
    });
  });
}

module.exports = app;
