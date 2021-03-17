platform_t = "unknown";

serverName = "Connis Server";

const { exec } = require("child_process");
const http = require ('http'); // to create server
const { platform, hostname } = require("os");
const url = require('url');
const request = require('request');
const colors = require('colors');

logLevel = "ALL";
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

const asyncFunction = () => {
    return new Promise(resolve => {
        resolve(body);
    });
}

// Get client IP address from request object
getClientAddress = function (req) {
    log("-----------------------------", "DEBUG");
    log("Getting client ip address", "DEBUG");
    log("x-forwarded-for: " + req.headers['x-forwarded-for'], "DEBUG");
    log("req.connection.remoteAddress: " + req.connection.remoteAddress, "DEBUG");
    ip = (req.headers['x-forwarded-for'] || '').split(',')[0] || req.connection.remoteAddress.split(":")[3];
    log(`IP: ${ip}`);
    if(!ip) return "localhost";
    return ip;
};

isAllowed = function(ip) {
    log("Checking request sender!", "DEBUG");

}

notifications = [
    [
        {
            "Issuer"            :   "SERVER",
            "Description"       :   "clientEndpoint has booted up.",
            "Reason"            :   "idfk",
            "ShortDescription"  :   "clientEndpoint has booted up.",
            "Title"             :   "Endpoint has started."
        }
    ]
]

function processNotif(notif, id) {
    return (
        `
        ${id}Title:${notif[0].Title},
        ${id}ShortDescription:${notif[0].ShortDescription},
        ${id}Description:${notif[0].Description},
        ${id}Reason:${notif[0].Reason},
        ${id}Issuer:${notif[0].Issuer},

        `
    )
}

async function handle(parsedURL, res, req) {
    log(`-----------------------------`, "GENERIC");
    log(`Handling new request.`, "GENERIC");
    var target = getClientAddress(req);
    log(`Ip is: ${target}`, "GENERIC");
    log(`Ip is: ${target.brightBlue}`, "GENERIC");
    log(`PathName: ${parsedURL.pathname.brightBlue}`, "GENERIC");
    
    if(parsedURL.pathname == "/notifications") {
        log(`Status Code: ${"202".brightBlue}`, "GENERIC");
        
        notifications_saved = notifications;
        notifications = [];

        response = 
        `
        `;

        i = 0;
        notifications_saved.forEach(e => {
            response += processNotif(e, i) 
            i++;
        });
        res.statusCode = 202;
        res.end(response);


    } else if(parsedURL.pathname == "/ncount") {
        log(`Requesting notification count - ${notifications.length}`, "GENERIC");

        log(`Status Code: ${"202".brightBlue}`, "GENERIC");
        log(`Finished.`, "GENERIC");
        res.statusCode = 202;
        res.end(`count:${notifications.length},`);

    } else if (parsedURL.pathname == "/new") {
        if(parsedURL.query.payload) {
                log(`New notification: ${parsedURL.query.payload}`)
                //localhost:5000/new?Title=CUNT&ShortDescription=Short Desc&Description=Description&Reason=Reason?&Issuer=Conni issued
                splitUp = parsedURL.query.payload.split("--");
                notifications.push(
                    [
                        {
                            "Title"             :   splitUp[0],
                            "ShortDescription"  :   splitUp[1],
                            "Description"       :   splitUp[2],
                            "Reason"            :   splitUp[3],
                            "Issuer"            :   splitUp[4]
                        }
                    ]
                )

                log(`Status Code: ${"202".brightBlue}`, "GENERIC");
                log(`Finished.`, "GENERIC");
                res.statusCode = 202;
                res.end("Submitted");
            } else {
                log(`Status Code: ${"202".brightBlue}`, "GENERIC");
                log(`Finished.`, "GENERIC");
                res.statusCode = 202;

                res.end("Failed to submit.");
            }
    }
    else {
        res.statusCode = 400;
        log(`Status Code: ${"400".red}`, "GENERIC");
        log(`Accessed an API Endpoint that isnt supported!`.red, "GENERIC");
    }
}
