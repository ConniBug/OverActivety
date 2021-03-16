const { exec } = require("child_process");
const http = require ('http'); // to create server
const { platform } = require("os");
const url = require('url'); // to parse url
// var ping = require('net-ping');

///console.log(osu.cpus());
///console.log(osu.totalmem());
//console.log(osu.freemem())

// create basic server and implement handling different requests
const app = http.createServer( async (req, res) => {
    // parse the incoming url
    const parsedURL = url.parse(req.url, true);
    
    handle(parsedURL, res, req);
});

app.listen(5000);

platform_t = "unknown";

localIPs = [];

console.log("Looking for local IPv4 ips");
console.log("Detecting platform");
exec("ipconfig", (error, stdout, stderr) => {
    if (error) {
//        console.log(`error: ${error.message}`);
        exec("ifconfig", (error, stdout, stderr) => {
            if (error) {
                console.log(`error: ${error.message}`);
                platform_t == "err"
                return;
            }
            if (stderr) {
                console.log(`stderr: ${stderr}`);
                platform_t == "err"
                return;
            }
            console.log(`stdout: ${stdout}`);   
            //stdout = stdout.split("inet ")[0].split("  ")[0].split("/")[0];

            console.log("----");
            console.log(stdout.split("inet"))
            stdout.forEach(e => {
                if(e.includes("/")) {
                    localIPs.push(e.split(" ")[0]);
                }
            });
            console.log("----");
            
            //localIPs.push(stdout);

            platform_t = "linux";
            console.log(`Platform is ${platform} - ${platform_t}`);
            console.log("Found " + localIPs.length + " local ips.");
            console.log(localIPs);
        });
        return;
    }
    if (stderr) {
        console.log(`stderr: ${stderr}`);
        platform_t == "err"
        return;
    }

    stdout = stdout.split("\r\n");
    stdout[1] = "";

    stdout.forEach(e => {
        ip = e.split("   IPv4 Address. . . . . . . . . . . : ")[1];
        if(ip) {
            console.log("Found: " + ip);
            localIPs.push(ip);
        }
    });
    platform_t = "windows";
    console.log(`Platform is ${platform} - ${platform_t}`);
    console.log("Found " + localIPs.length + " local ips.");
    console.log(localIPs);
});


// Get client IP address from request object
getClientAddress = function (req) {
    console.log(req.headers['x-forwarded-for']);
    console.log(req.connection.remoteAddress);
    
    return (req.headers['x-forwarded-for'] || '').split(',')[0] 
    || req.connection.remoteAddress.split(":")[3];
};

async function handle(parsedURL, res, req) {
    if(parsedURL.pathname == "/info") {
        var target = getClientAddress(req);
        console.log(target);
        session.pingHost (target, function (error, target, sent, rcvd) {
            var ms = rcvd - sent;
            if (error) {
                console.log("session.pingHost error");
                console.log (target + ": " + error.toString ());
            } else {
                console.log(http.get('http://bot.whatismyipaddress.com'));
                res.statusCode = 202;
                res.end(
                    `
                    name:Connis Desktop,
                    wanIP:90.90.90.90,
                    lanIP:${LanIPs[LanNetworkAdaptorName]},
                    hostname:spookiebois.club,
                    uptime:12 Days,
                    cpuUsage:243%,
                    memoryUsage:12Gb/18Gb,
                    ping:${ms},
                    `
                );
                console.log (target + ": Alive (ms=" + ms + ")");
            }
        });

    }
    else {
        res.statusCode = 400;
        res.end("API Endpoint Not Supported");
    }
}
