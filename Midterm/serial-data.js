var redis = require('redis');
var client = redis.createClient();
var HOST = "127.0.0.1";
var rPORT = "6379";
var client = redis.createClient(rPORT, HOST);


var hum;
var temp;
var hein;
var uvin;

var icon;

var bodyParser = require('body-parser');
var express = require("express");
var app = express();
var port = 8000;
var url='localhost'
var server = app.listen(port);
var io = require("socket.io").listen(server);

var serialport = require("serialport");
var SerialPort = serialport.SerialPort;
var sport = new SerialPort("/dev/ttyAMA0", {
  baudrate: 9600,
  parser: serialport.parsers.readline("\n")
}, false); 

app.use(express.static(__dirname + '/'));
console.log('Simple static server listening at '+url+':'+port);



io.sockets.on('connection', function (socket) {
  sport.open(function(error) {

    if (error) {
      console.log('failed to open: ' + error);
    } else {
      console.log('Serial open');
      sport.on('data', function(data) {    

        console.log(data);
        
        socket.emit('toScreen', data);



        hum = data.substring(0,2);
        temp = data.substring(3, 5);
        hein = data.substring(6, 8);
        uvin = data.substring(9,12);


        //get time to check for data update
        var d = new Date();
        var hour = d.getHours();
        var min = d.getMinutes();
        var sec = d.getSeconds();

        var weekDay = d.getDay();
        var day = d.getDate();
        var month = d.getMonth();
        var year = d.getYear();
        var today = day,month,year;

        //declare variables for saving data
        var todayHum = 'Hum'+weekDay;
        var todayTemp = 'Temp'+weekDay;
        var todayHein = 'Hein'+weekDay;

        //redis connection
        client.on('connect', function(err){
        });

        client.hmset('Weather-Test', todayHum, hum , todayTemp, temp, todayHein, hein);

        client.hgetall('Weather-Test', function(err, reply){
          yesTemp = reply.Temp2;
          yesHum = reply.Hum2;
          yes2Temp = reply.Temp6;
          yes2Hum = reply.Hum6;

          var data = [yesTemp,yesHum,yes2Temp,yes2Hum];

          socket.emit('toChart', data);
        });

        

          // ////////Code for saving info once at noon everyday (untested)//////////////
          // if((hour == 12) && (min == 00) && (sec == 00)){
          //   setTimeout(function(){
          //     client.hmset('Weather-Test', 'Hum', todayHum, 'Temp', todayTemp, 'HeIn', todayHein);
          //   }, 5000);
          // }
        


      });


    }

  });
});






