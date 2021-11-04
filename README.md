# UrlBuster
A simple and small program that attempts to find unknown paths in a URL

# Usage

```
Usage:
        UrlBuster [options] -u <argument> -d <argument> check url using dictionary
Options:
	-h, --help                                      display this help
      	-u, --url                                       target url
       	-d, --dictionary                                dictionary used by the program
       	-t, --thread                                    define how many threads the program uses
       	-o, --output                                    define where to output log
       	-ot, --output-type                              define output save type
       		-ot 0                                   save all into single output file
               	-ot 1                                   save all into multiple output files named by status code
       	-s, --slow-mode <argument>                      wait x miliseconds before next request
```
