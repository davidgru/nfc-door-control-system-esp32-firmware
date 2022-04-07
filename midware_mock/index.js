const https = require('https');
const fs = require('fs');

const options = {
  cert: fs.readFileSync('cert.crt'),
  key: fs.readFileSync('cert.key')
};

let i = 0;

https.createServer(options, (req, res) => {
    console.log('-----------------------------------');
    console.log('method: ' + req.method);
    console.log('url: ' + req.url);
    console.log('headers: ' + req.rawHeaders);
    var body = "";
    req.on('data', function (chunk) {
      body += chunk;
    });
    req.on('end', function () {
      console.log('body: ' + body);
      console.log('-----------------------------------');
    });
    res.end();
    i++;
    console.log(`Request no. ${i}`)
}).listen(8080);
