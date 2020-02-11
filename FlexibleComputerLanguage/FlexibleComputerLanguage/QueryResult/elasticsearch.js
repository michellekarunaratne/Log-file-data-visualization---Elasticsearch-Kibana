const { Client } = require('@elastic/elasticsearch')
const client = new Client({ node: 'http://localhost:9200' })

//Reading query result from the json file
var fs = require('fs');
var path= require('path');
var filePath=path.join("..\\FlexibleComputerLanguage\\QueryResult\\QueryResult.json");

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
        if(doc[i].Status=="Fail" && doc[i].Count!=0){
            failedTestSuites.add(doc[i].Test_Suite_Name)
        }
        if(doc[i].Status=="Warning" && doc[i].Count!=0){
            warningTestSuites.add(doc[i].Test_Suite_Name)
        }
        if(doc[i].Status=="Pass" && doc[i].Count!=0){
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
                var status=["Pass","Fail","Warning"]
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

// function clearIndex(){

//     var promise =new Promise(function(resolve,reject){

//         client.indices.delete({
//             index: ['test_suite_execution_time_details']
//           }).then(function(resp) {
//             console.log("Successful query!");
//             console.log(JSON.stringify(resp, null, 4));
//             resolve(resp)
//           }, function(err) {
//             console.trace(err.message);
//             reject(err)
//           });
//     })

//     return promise
    
// }

// seprateDocument()
// .then(function(doc){
//     sortDocuments(doc)
// })
// .catch(function(error){
//     console.log(error)
// })

// clearIndex()

function dataTableSort(){

    var promise = new Promise(function(resolve,reject){

        fs.readFile(filePath,{encoding:'utf-8'},function(err,rawdata){
            if(!err){
                var documents=[]
                var doc=JSON.parse(rawdata)
                for(i=0;i<doc.val.length;i++){
                    for(j=1;j<doc.val[i].length;j++){
                        var elasticsearchDoc=new Object();
                        elasticsearchDoc["Test_Suite_Name"]=doc.val[i][0]
                        elasticsearchDoc["Test_Case_Name"]=doc.val[i][1]
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

function enterDataTableDocs(docs){

    for(i=0;i<docs.length;i++){
        client.index({
            index:'test_cases_details',
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


// dataTableSort()
// .then(function(docs){
//     enterDataTableDocs(docs)
// })
// .catch(function(error){
//     console.log(error)
// })

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
                        elasticsearchDoc["Test_Suite_Execution_Time"]=parseFloat(doc.val[i][1])
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

// barChartSort()
// .then(function(docs){
//     enterBarChartDocs(docs)
// })
// .catch(function(error){
//     console.log(error)
// })

//clearIndex()

function barChartSort(){

    var promise = new Promise(function(resolve,reject){

        fs.readFile(filePath,{encoding:'utf-8'},function(err,rawdata){
            if(!err){
                var documents=[]
                var doc=JSON.parse(rawdata)
                for(i=0;i<doc.val.length;i++){
                    for(j=1;j<doc.val[i].length;j++){
                        var elasticsearchDoc=new Object();
                        elasticsearchDoc["Test_Suite_Name"]=doc.val[i][0]
                        elasticsearchDoc["Total_Test_Cases"]=doc.val[i][1]
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

function enterBarChartDocs(docs){

    for(i=0;i<docs.length;i++){
        client.index({
            index:'total_test_cases_details',
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

// barChartSort()
// .then(function(docs){
//     enterBarChartDocs(docs)
// })
// .catch(function(error){
//     console.log(error)
// })

function heatMapSort(){

    var promise = new Promise(function(resolve,reject){

        fs.readFile(filePath,{encoding:'utf-8'},function(err,rawdata){
            if(!err){
                var documents=[]
                var doc=JSON.parse(rawdata)
                for(i=0;i<doc.val.length;i++){
                    for(j=1;j<doc.val[i].length;j++){
                        var elasticsearchDoc=new Object();
                        elasticsearchDoc["Test_Suite_Name"]=doc.val[i][0]
                        var dateTime=doc.val[i][1].split(" ")
                        var dateString=dateTime[0].split("-")
                        var date=new Date(dateString[0],dateString[1]-1,dateString[2])
                        elasticsearchDoc["Timestamp"]=date
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

function enterHeatMapDocs(docs){

    for(i=0;i<docs.length;i++){
        client.index({
            index:'test_suite_timestamp',
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
            index: ['test_suite_timestamp']
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

heatMapSort()
.then(function(docs){
    enterHeatMapDocs(docs)
})
.catch(function(error){
    console.log(error)
})

// clearIndex()