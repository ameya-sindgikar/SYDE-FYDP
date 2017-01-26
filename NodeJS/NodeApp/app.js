var express = require('express');
var path = require('path');
var favicon = require('serve-favicon');
var logger = require('morgan');
var cookieParser = require('cookie-parser');
var bodyParser = require('body-parser');
var five = require('johnny-five');
var mongo = require('mongodb').MongoClient;
var assert = require('assert');
var moment = require('moment');
var generator = require('knear');


var index = require('./routes/index');
var users = require('./routes/users');

var app = express();

//MongoDB variables
var url = 'mongodb://localhost:27017/accelDataDB';
var collectionName = 'FailureOneCollection';

//accelerometer variables
var aX;
var aY;
var aZ;
var aPitch;
var aRoll;
var aAcc;
var inclination;
var orientation;

//Gyroscope variables
var gX;
var gY;
var gZ;
var gPitch;
var gRoll;
var gYaw;

//altimeter variables
var altFeet;
var altMeters;

//kNN variables
var k = 3; //TBD
var machine = new generator.kNear(k);

//get data from johnny-five
var five = require("johnny-five");
var board = new five.Board();

board.on("ready", function() {
  //IMU
  var imu = new five.IMU({
    controller: "MPU6050",
    freq: 100 //100 milliseconds
  });
  imu.on("data", function(){
    aX = this.accelerometer.x;
    aY = this.accelerometer.y;
    aZ = this.accelerometer.z;
    aAcc = this.accelerometer.acceleration;
    aPitch = this.accelerometer.pitch;
    aRoll = this.accelerometer.roll;
    gX = this.gyro.x;
    gY = this.gyro.y;
    gZ = this.gyro.z;
    gPitch = this.gyro.pitch;
    gRoll = this.gyro.roll;
    gYaw = this.gyro.yaw;

    console.log("MPU6050");
    console.log("aX     ",aX);
    console.log("aY     ",aY);
    console.log("aZ     ",aZ);
    console.log("aAcc   ",aAcc);
    console.log("aPitch ",aPitch);
    console.log("aRoll  ",aRoll);
    console.log("gX     ",gX);
    console.log("gY     ",gY);
    console.log("gZ     ",gZ);
    console.log("gPitch ",gPitch);
    console.log("gRoll  ",gRoll);
    console.log("gYaw   ",gYaw);
  });

  //Altimeter
  var altimeter = new five.Altimeter({
    controller: "MPL3115A2",
    freq: 100,
    elevation: 333 //Current elevation
  });
  altimeter.on("data", function(){
    altFeet = this.feet;
    altMeters = this.meters;

    console.log("MPL3115A2");
    console.log("Altitude in feet: ", altFeet);
    console.log("Altitude in meters: ", altMeters);
    console.log("_______________________________");

    storeData();
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

function storeData(){
  var dataArray = [aX, aY, aZ, aAcc, gX, gY, gZ, aPitch, aRoll, altMeters];
  var item = {
    data: dataArray,
    created: moment().format('MMMM Do h:mm:ss:SS'),
    createdUnix: moment().valueOf() //number of milliseconds since Jan 1 1970 (Epoch)
  };

  mongo.connect(url, function (err, db){
    assert.equal(null, err);
    db.collection(collectionName).insertOne(item, function(err, result){
      assert.equal(null, err);
      console.log('Item inserted');
      console.log(item);
      console.log('_______________________');
      db.close();
    });
  });
}

module.exports = app;
