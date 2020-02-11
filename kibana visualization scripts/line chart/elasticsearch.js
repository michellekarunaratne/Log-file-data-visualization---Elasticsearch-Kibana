var fs = require('fs');
var path= require('path');
var filePath=path.join("..\\kibana visualization scripts\\line chart\\QueryResult.json");
const { Client } = require('@elastic/elasticsearch')
const client = new Client({ node: 'http://localhost:9200' })

function lineChartSort(){

    var promise = new Promise(function(resolve,reject){

        fs.readFile(filePath,{encoding:'utf-8'},function(err,rawdata){
            if(!err){
                var documents=[]
                var doc=JSON.parse(rawdata)
                for(i=0;i<doc.val.length;i++){
                    for(j=1;j<doc.val[i].length;j++){
                        var elasticsearchDoc=new Object();
                        elasticsearchDoc["Test_Suite_Name"]=doc.val[i][0]
                        elasticsearchDoc["Test_Suite_Execution_Time"]=doc.val[i][1] * 1000
                        documents.push(elasticsearchDoc)
                    }
    
                }
                resolve(documents)
                
            }
            else{
               reject(err)
            }
        })

    })

    return promise

}

function enterLineChartDocs(docs){

    for(i=0;i<docs.length;i++){
        client.index({
            index:'test_suite_execution_time_details',
            body:docs[i]
        })
        .then(function(doc){
            console.log(doc)
        })
        .catch(function(error){
            console.trace(error.message)
        })
    }

}

function clearIndex(){

    var promise =new Promise(function(resolve,reject){

        client.indices.delete({
            index: ['test_suite_execution_time_details']
          }).then(function(resp) {
            console.log("Successful query!");
            console.log(JSON.stringify(resp, null, 4));
            resolve(resp)
          }, function(err) {
            console.trace(err.message);
            reject(err)
          });
    })

    return promise
    
}


function writeOutput(docs){
   
    fs.writeFile("Output.json",JSON.stringify(docs, null, 4),function(error){
        if(error){
            console.log(error)
        }
    })
  
}

// lineChartSort()
// .then(function(docs){
//     writeOutput(docs)
// })
// .catch(function(error){
//     console.log(error)
// })

lineChartSort()
.then(function(docs){
    enterLineChartDocs(docs)
})
.catch(function(error){
    console.log(error)
})

// clearIndex()
