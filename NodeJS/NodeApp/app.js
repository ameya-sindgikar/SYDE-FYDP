var express = require('express');
var path = require('path');
var favicon = require('serve-favicon');
var logger = require('morgan');
var cookieParser = require('cookie-parser');
var bodyParser = require('body-parser');
var SerialPort = require('serialport');
var five = require('johnny-five');

var index = require('./routes/index');
var users = require('./routes/users');

var app = express();

//accelerometer variables
var x;
var y;
var z;
var accelPitch;
var accelRoll;
var acceleration;
var inclination;
var orientation;

//get data from johnny-five
var five = require("johnny-five");
var board = new five.Board();
var x

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
    console.log("___________");
  });

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

module.exports = app;
