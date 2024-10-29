const http = require('http')
const fs = require('fs')
const path = require('path');
const url = require('url');
const exec = require('child_process').execFile;
const port = 8080

const getMimeType = (ext) => {
    const mimeTypes = {
      '.html': 'text/html',
      '.css': 'text/css',
      '.js': 'application/javascript',
      '.json': 'application/json',
      '.png': 'image/png',
      '.jpg': 'image/jpeg',
      '.jpeg': 'image/jpeg',
      '.gif': 'image/gif',
      '.svg': 'image/svg+xml',
      '.ico': 'image/x-icon',
      '.txt': 'text/plain'
    };
    return mimeTypes[ext.toLowerCase()] || 'application/octet-stream';
  };
  // Creating the server and also seting up the request function
  const server = http.createServer((req, res) => {
    // Geting the local path path
    let parsedUrl = url.parse(req.url);
    let pathname = `.${parsedUrl.pathname}`;
    console.log(pathname)
    // If there is an extension in the path then we are loading a file
    // Else we consider that the user accesed a page and its requesting the html page
    const ext = path.extname(pathname);
    if (ext) {
      if(ext == ".json"){
        exec('SelfPlayCandyCrush.exe', function(err, data) {
          fs.readFile("candy_grid.json", (err, data) => {
            if (err) {
              res.writeHead(404, { 'Content-Type': 'text/html' });
              res.end('<h1>404 File Not Found</h1>');
            } else {
              res.writeHead(200, { 'Content-Type': getMimeType(ext) });
              res.end(data);
            }
          });
        });
      }
      else{
        fs.readFile(pathname, (err, data) => {
          if (err) {
            res.writeHead(404, { 'Content-Type': 'text/html' });
            res.end('<h1>404 File Not Found</h1>');
          } else {
            res.writeHead(200, { 'Content-Type': getMimeType(ext) });
            res.end(data);
          }
        });
      }
    } else {
      const filePath = path.join(__dirname, pathname, 'index.html');
  
      fs.readFile(filePath, (err, data) => {
        if (err) {
          res.writeHead(404, { 'Content-Type': 'text/html' });
          res.end('<h1>404 Page Not Found</h1>');
        } else {
          res.writeHead(200, { 'Content-Type': 'text/html' });
          res.end(data);
        }
      });
    }
  });
  
  server.listen(3000, () => {
    console.log('Server is running on http://localhost:3000');
  });
