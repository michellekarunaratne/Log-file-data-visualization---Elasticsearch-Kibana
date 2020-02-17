Using the LDEL 

1. Add the logfile which the user desires to convert into a strucutred logfile into ./log-data-extracter/LogAnalyzer/tests/LDEL_test1/log.txt
2. Add the grammar rules which will be used to structure the log file into ./log-data-extracter/LogAnalyzer/tests/LDEL_test1/script.txt
3. Run the LDEL program
4. The desired JSON output will be written into the ./log-data-extracter/LogAnalyzer/tests/LDEL_test1/resultJSON.json 

Using the LDAL

1. Retrieve the JSON output file from ./log-data-extracter/LogAnalyzer/tests/LDEL_test1/resultJSON.json
2. Convert the content of the resultJSON.json file into q JSON string.
3. Add the JSON string into ./FlexibleComputerLanguage/FlexibleComputerLanguage/QueryResult/LogFile.txt
4. Add the query into ./FlexibleComputerLanguage/FlexibleComputerLanguage/QueryResult/Query.txt
5. Run the LDAL program.
6. The JSON output of the query will be written into ./FlexibleComputerLanguage/FlexibleComputerLanguage/QueryResult/QueryResult.JSON

Inserting into the Elasticsearch cluster

1. Copy the query output from ./FlexibleComputerLanguage/FlexibleComputerLanguage/QueryResult/QueryResult.JSON and paste the content inside the any QueryResult.json file which will be contain within the folders which are inside the ./kibana visualization scripts folder.
2. The folders that are in the ./kibana visualization scripts folder will contain folders for each visualizations which have used in Kibana. In each folder the Elasticsearch client script will read the QueryResult.JSON within the same folder and add them into an Elasticsearch cluster.