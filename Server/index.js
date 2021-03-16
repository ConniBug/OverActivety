platform_t = "unknown";

serverName = "Connis Server";

const { exec } = require("child_process");
const http = require ('http'); // to create server
const { platform, hostname } = require("os");
const url = require('url');
const request = require('request');
const colors = require('colors');

logLevel = "ERROR";
function getLogLevelNum(level) {
    if(level == "GENERIC")  return 1;
    if(level == "ERROR")    return 2;
    if(level == "DEBUG")    return 3;
    if(level == "ALL")      return 4;
    log("Unsure what log level " + level.red + " belongs to.", "GENERIC");
    return 4;

}
function log(message, type = "DEBUG") {
    if(getLogLevelNum(type) > getLogLevelNum(logLevel))
    {
        // console.log("Log level: " + getLogLevelNum(type));
        // console.log("Config log level: " + getLogLevelNum(logLevel));
        return;
    }

    maxSize = 55

    time = getDateTime().yellow;

    StartMessage = "";
    if(type == "ERROR") StartMessage = (`[${time}] - [` + type.red + `]`);
    if(type == "GENERIC") StartMessage = (`[${time}] - [` + type.green + `]`);
    if(type == "DEBUG") StartMessage = (`[${time}] - [` + type.gray + `]`);

    left = maxSize - StartMessage.length;
    function balence() {
        tmp = "";
        space = " ";
        while(left >= 0) {
            left = left-1;
            tmp = tmp + space;
        }
        return tmp;
    }
    console.log(StartMessage + balence(StartMessage) +  "-> " + message);
}
function char_count(str, letter)  {
    var letter_Count = 0;
    for (var position = 0; position < str.length; position++) {
        if (str.charAt(position) == letter) {
            letter_Count += 1;
        }
    }
    return letter_Count;
}
function getDateTime() {

    var date = new Date();

    var hour = date.getHours();
    hour = (hour < 10 ? "0" : "") + hour;

    var min  = date.getMinutes();
    min = (min < 10 ? "0" : "") + min;

    var sec  = date.getSeconds();
    sec = (sec < 10 ? "0" : "") + sec;

    var year = date.getFullYear();

    var month = date.getMonth() + 1;
    month = (month < 10 ? "0" : "") + month;

    var day  = date.getDate();
    day = (day < 10 ? "0" : "") + day;

    return (year + ":" + month + ":" + day + " - " + hour + ":" + min + ":" + sec);
}

const app = http.createServer( async (req, res) => {
    const parsedURL = url.parse(req.url, true);  
    handle(parsedURL, res, req);
});
app.listen(5000);

const asyncGetLanIPs = () => {
    return new Promise(resolve => {
        localIPs = [];

        log("--------------------------");
        log("Looking for local IPv4 ips");
        exec("ipconfig", (error, stdout, stderr) => {
            if (error) {
                exec("ifconfig", (error, stdout, stderr) => {
                    if (error) {
                        log(`if config error -> error`, "ERROR");
                        log(`error: ${error.message}`, "ERROR");
                        return;
                    }
                    if (stderr) {
                        log(`if config error -> stderr`, "ERROR");
                        log(`stderr: ${stderr}`);
                        log(`if config error: ${stderr}`, "ERROR");
                        return;
                    }
        
                    stdout.split("inet ").forEach(e => {
                        if(e.includes(" ")) {
                            if(char_count(e.split(" ")[0], ".") == 3) {
                                log(e.split(" ")[0])
                                localIPs.push(e.split(" ")[0]);
                            }
                        }
                    });
        
                    platform_t = "linux";
                    log("Found " + localIPs.length + " local ips.", "DEBUG");
                    log(localIPs, "DEBUG");
                });
                return;
            }
            if (stderr) {
                log(`stderr: ${stderr}`, "ERROR");
                return;
            }
            stdout = stdout.split("\r\n");

            // Remove windows banner cunt
            stdout[1] = "";

            // Some really weird fuckin logic
            stdout.forEach(e => {
                ip = e.split("   IPv4 Address. . . . . . . . . . . : ")[1];
                if(ip) {
                    log("Found: " + ip);
                    localIPs.push(ip);
                }
            });
            log("Found " + localIPs.length + " local ips.", "DEBUG");
            log(localIPs, "DEBUG");
            resolve(localIPs);
            log("--------------------------");
        });
    });
}
const asyncGetWanIP = () => {
    return new Promise(resolve => {
        request('http://bot.whatismyipaddress.com', { json: false }, (err, res, body) => {
            if (err) { 
                log("--------------------------", "ERROR");
                log("Looking for Wan IPv4 ip", "ERROR");
                log(err, "ERROR");
                log("--------------------------", "ERROR");
                return console.log(err); 
            }
            log("--------------------------", "DEBUG");
            log("Looking for Wan IPv4 ip", "DEBUG");
            log(body, "DEBUG");
            log("Found WanIP:" + body + ".", "DEBUG");
            log("--------------------------", "DEBUG");
            resolve(body);
        });
    });
}
const asyncHostname = () => {
    return new Promise(resolve => {
        exec("hostname", (error, stdout, stderr) => {
            if (error) {
                log(`getHostname error: ${error.message}`, "ERROR");
                platform_t == "err"
                return;
            }
            if (stderr) {
                log(`getHostname stderr: ${stderr}`);
                platform_t == "err"
                return;
            }       
            stdout = stdout.replace(/\n/g, "");
            stdout = stdout.replace(/\r/g, "");
            log("--------------------------", "DEBUG");
            log("Hostname is " + stdout + ".", "DEBUG");
            log("--------------------------", "DEBUG");
            resolve(stdout);
            return stdout;
        });
    });
}

// Get client IP address from request object
getClientAddress = function (req) {
    log("-----------------------------", "DEBUG");
    log("Getting client ip address", "DEBUG");
    log("x-forwarded-for: " + req.headers['x-forwarded-for'], "DEBUG");
    log("req.connection.remoteAddress: " + req.connection.remoteAddress, "DEBUG");
    log(`IP: ${(req.headers['x-forwarded-for'] || '').split(',')[0] || req.connection.remoteAddress.split(":")[3]}`);
    return (req.headers['x-forwarded-for'] || '').split(',')[0] || req.connection.remoteAddress.split(":")[3];
};

async function handle(parsedURL, res, req) {
    log(`-----------------------------`, "GENERIC");
    log(`Handling new request.`, "GENERIC");
    var target = getClientAddress(req);
    log(`Ip is: ${target.brightBlue}`, "GENERIC");
    log(`PathName: ${parsedURL.pathname.brightBlue}`, "GENERIC");
    
    if(parsedURL.pathname == "/info") {
        log(`Status Code: ${"202".brightBlue}`, "GENERIC");
            res.statusCode = 202;
            res.end(
                        `
                        name:${serverName},
                        wanIP:${await asyncGetWanIP()},
                        lanIP:${await asyncGetLanIPs()},
                        hostname:${await asyncHostname()},
                        uptime:,
                        cpuUsage:,
                        memoryUsage:,
                        ping:,
                        `
            );


    }
    else {
        res.statusCode = 400;
        log(`Status Code: ${"400".red}`, "GENERIC");
        log(`Accessed an API Endpoint that isnt supported!`.red, "GENERIC");
        res.end("API Endpoint Not Supported");
    }
}
