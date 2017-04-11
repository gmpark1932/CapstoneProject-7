var express = require('express');
var app = express();
var fs = require('fs');

mysql = require('mysql');
var connection = mysql.createConnection({
    host: 'localhost',
    user: 'sensor',
    password: 'Gyungmin1932!',
    database: 'data'
})
connection.connect();


app.get("/data", function(req, res) {
  console.log("param=" + req.query);

  //var qstr = 'select * from sensors where time > date_sub(now(), INTERVAL 1 DAY) ';
  var qstr = 'select * from sensors ';
  connection.query(qstr, function(err, rows, cols) {
    if (err) {
      throw err;
      res.send('query error: '+ qstr);
      return;
    }

    console.log("Got "+ rows.length +" records");
    var html = "<!doctype html><html><body>";
    //html += "<H1> Sensor Data for Last 24 Hours</H1>";
    //html += "<table border=1 cellpadding=3 cellspacing=0>";
    //html += "<tr><td>Seq#<td>Time Stamp<td>Temperature";
    for (var i=0; i< rows.length; i++) {
       html += "<br>"+ JSON.stringify(rows[i]);
    }
    //html += "</table>";
    //html += "</body></html>";
    res.send(html);
  });

});

getClientAddress = function (req){
	var ip = req.headers['x-forwarded-for'] || req.connection.remoteAddress;
	if (ip.substr(0, 7) == "::ffff:") {
  	ip = ip.substr(7)
	}

	return ip;
};

int count = 0;

app.get('/ading', function(req,res){
	console.log("file writing bring!");

	res.send(req.query.field1);

	inData = {};	
	inData.seq = count++;
	inData.device = req.query.device;
	inData.unit = req.query.unit;
	inData.type = req.query.type;
	inData.value = req.query.field1;
	inData.ip = getClientAddress(req);
	console.log("---------------------------------------");
	console.log("seq : " + inData.seq);
	console.log("device : " + inData.device);
	console.log("unit : " + inData.unit);
	console.log("type : " + inData.type);
	console.log("value : " + inData.value);
	console.log("IP : " + inData.ip);

	var query = connection.query('insert into sensors set ?',inData,function(err,rows,cols){
	if(err) throw err;
	console.log("done");

	});

	fs.appendFile("/home/20121590/WebServer/commingData.txt",inData.value+"\n" , function(err){
		if(err){
			return console.log(err);
		}
	});

	console.log("The file was saved!");

});

app.listen(3000, function () {
  console.log('Example app listening on port 3000!')
})
