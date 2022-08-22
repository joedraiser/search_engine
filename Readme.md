## Search Engine

### Description
Search Engine is designed to search requested strings through the text files. It calculates relevance ratio for each request per each text file. It has array of requests and array of text files to be searched as inputs and relevance array as output. Project has three main classes that were designed to do the job:

-**ConverterJSON**  provides parsing of _config.json_, _requests.json_. Also writes down _answers.json_.

-**InvertedIndex**  calculates absolute amount of occurrences of request in text document and puts it in an array.

-**SerchServer**    puts things together by calculating relevance ratio and sorting it in descending order. Than it puts it to an array.

Text files to be searched are located at _resource_ folder. 

_config.json_ contains various options for launching search engine also it contains paths to text files that should be searched. 

_request.json_ contains requests in a string array.

_answer.json_ is created in a process of execution of search engine. It contains relevance ratio for each document per request.

### Tech stack used

Nlohmann json library was involved in this project. 

For test purposes Google Test was used.

C++ Standard 17

### How to use the project

Add files that should be processed:
1. Add text files to _resource_ folder
2. Provide paths to that files in _config.json_ file

Build a project with CMake

Project has two targets of execution: 

test_all target is used to launch testcases

search_engine target is designed for search engine to use. It has main function as entry point where all three main classes(ConverterJSON, InvertedIndex and SearchServer) can be invoked.
