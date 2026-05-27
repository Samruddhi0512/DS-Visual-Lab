const http = require("http");

const PORT = 10000;

const server = http.createServer((req, res) => {
    
    const response = {
        status: "C++ server working"
    };

    res.writeHead(200, {
        "Content-Type": "application/json",
        "Access-Control-Allow-Origin": "*"
    });

    res.end(JSON.stringify(response));
});

server.listen(PORT, () => {
    console.log(`Server running on http://127.0.0.1:${PORT}`);
});