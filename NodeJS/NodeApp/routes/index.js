var express = require('express');
var router = express.Router();
var mongo = require('mongodb').MongoClient;
var assert = require('assert');
var ObjectId = require('mongodb').ObjectID;

//MongoDB url
var url = 'mongodb://localhost:27017/TrainingDataDB';

var collectionName = 'TakeOffExp15';
var resultsCollectionName = 'ClassificationResults';

/* GET home page. */
router.get('/', function(req, res, next) {
  res.render('index', { title: 'SYDE 462 Team 16' });
});

/* GET all sensor data from mongoDB collection */
router.get('/get-data', function(req, res, next){
  var dataArray = [];
  mongo.connect(url, function(err, db) {
    assert.equal(null, err);
    var cursor = db.collection(collectionName).find(); //get all data in the collection
    cursor.forEach(function(doc, err){
      assert.equal(null, err);
      dataArray.push(doc);
    }, function() {
        db.close();
        res.render('get-data', {title: collectionName + ' Data Dump', items: dataArray});
    });
  });
});

/*GET classification result*/
router.get('/get-result', function(req, res, next){
  var data = [];
  mongo.connect(url, function(err, db) {
    assert.equal(null, err);
    var cursor = db.collection(resultsCollectionName).find(ObjectId("58ba70a8352a60259441d969"));
    cursor.forEach(function(doc, err){
      assert.equal(null, err);
      data.push(doc);
      console.log(doc);
    }, function() {
        db.close();
        res.render('get-result', {title: resultsCollectionName, items: data});
    });
  });
});


module.exports = router;
