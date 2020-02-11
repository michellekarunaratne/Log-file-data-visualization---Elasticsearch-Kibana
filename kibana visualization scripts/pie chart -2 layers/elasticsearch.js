const { Client } = require('@elastic/elasticsearch')
const client = new Client({ node: 'http://localhost:9200' })

//Reading query result from the json file
var fs = require('fs');
var path= require('path');
var filePath=path.join("..\\kibana visualization scripts\\pie chart -2 layers\\QueryResult.json");

function union(setA, setB) {
    var _union = new Set(setA);
    for (var elem of setB) {
        _union.add(elem);
    }
    return _union;
}

function intersection(setA, setB) {
    var _intersection = new Set();
    for (var elem of setB) {
        if (setA.has(elem)) {
            _intersection.add(elem);
        }
    }
    return _intersection;
}

function symmetricDifference(setA, setB) {
    var _difference = new Set(setA);
    for (var elem of setB) {
        if (_difference.has(elem)) {
            _difference.delete(elem);
        } else {
            _difference.add(elem);
        }
    }
    return _difference;
}

function difference(setA, setB) {
    var _difference = new Set(setA);
    for (var elem of setB) {
        _difference.delete(elem);
    }
    return _difference;
}



function sortDocuments(doc){
    var failedTestSuites=new Set()
    var warningTestSuites=new Set()
    var passedTestSuites=new Set()
    var i;
    for(i=0;i<doc.length;i++){
        if(doc[i].Status=="Failed" && doc[i].Count!=0){
            failedTestSuites.add(doc[i].Test_Suite_Name)
        }
        if(doc[i].Status=="Warning" && doc[i].Count!=0){
            warningTestSuites.add(doc[i].Test_Suite_Name)
        }
        if(doc[i].Status=="Passed" && doc[i].Count!=0){
            passedTestSuites.add(doc[i].Test_Suite_Name)
        }
    }

    var passedWithoutFails=difference(passedTestSuites,failedTestSuites)
    //test suites that have passed without failed test cases but have warning test cases.
    var passedWithWarnings=intersection(passedWithoutFails,warningTestSuites)
    //test suites that have passed without failed and warning test cases.
    var passedWithoutFailsWarning=difference(passedWithoutFails,warningTestSuites)

    console.log(passedWithWarnings)
    console.log(passedWithoutFailsWarning)
    console.log(failedTestSuites)

    for(i=0;i<doc.length;i++){

        if(passedWithWarnings.has(doc[i].Test_Suite_Name)){
            client.index({
                index:'passed_with_warnings_test_suites',
                body:doc[i]
            })
            .then(function(doc){
                console.log(doc)
            })
            .catch(function(error){
                console.trace(error.message)
            })
        }
        if(passedWithoutFailsWarning.has(doc[i].Test_Suite_Name)){
            client.index({
                index:'passed_without_fails_warning_test_suites',
                body:doc[i]
            })
            .then(function(doc){
                console.log(doc)
            })
            .catch(function(error){
                console.trace(error.message)
            })
        }
        if(failedTestSuites.has(doc[i].Test_Suite_Name)){
            client.index({
                index:'failed_test_suites',
                body:doc[i]
            })
            .then(function(doc){
                console.log(doc)
            })
            .catch(function(error){
                console.trace(error.message)
            })
           
        }

        client.index({
            index:'test_suite_log',
            body:doc[i]
        })
        .then(function(doc){
            console.log(doc)
        })
        .catch(function(error){
            console.trace(error.message)
        })
    }

        
}

function seprateDocument(){

    var promise=new Promise(function(resolve,reject){

        fs.readFile(filePath,{encoding:'utf-8'},function(err,rawdata){
            if(!err){
                var documents=[]
                var status=["Passed","Failed","Warning"]
                var doc=JSON.parse(rawdata)
                for(i=0;i<doc.val.length;i++){
                    for(j=1;j<doc.val[i].length;j++){
                        var elasticsearchDoc=new Object();
                        elasticsearchDoc["Test_Suite_Name"]=doc.val[i][0]
                        elasticsearchDoc["Status"]=status[j-1]
                        elasticsearchDoc["Count"]=doc.val[i][j]
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

function clearIndex(){

    var promise =new Promise(function(resolve,reject){

        client.indices.delete({
            index: ['test_suite_log','passed_with_warnings_test_suites','passed_without_fails_warning_test_suites','failed_test_suites']
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

// seprateDocument()
// .then(function(docs){
//     writeOutput(docs)
// })
// .catch(function(error){
//     console.log(error)
// })

seprateDocument()
.then(function(doc){
    sortDocuments(doc)
})
.catch(function(error){
    console.log(error)
})

// clearIndex()
