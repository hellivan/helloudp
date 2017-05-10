const Promise = require('bluebird');
const sql = require('mssql');

function getConnection(config){
    return new Promise(function(resolve, reject){
        new (sql.Connection)(config).connect()
            .then(connection => resolve(connection))
            .catch(err => reject(err));
    }).disposer((connection) => connection.close());
}


function executeQuery(config, query) {
    return Promise.using(getConnection(config), connection => {
        return connection.request().query(query);
    });
}


const config = {
    "user": "sa",
    "password": "system",
    "server": "192.168.0.241",
    "debug": true,
    "options": {
	"instanceName": "sqlexpress",
	"appName": "TEST"
    }
};

if(process.argv.length > 2){
    config.server = process.argv[2];
}

executeQuery(config, "select macId, name from [Werma-Win].[dbo].[slaveDevice]")
    .then(console.log)
    .catch(console.error);
