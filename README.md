# BWT-Encoding-And-Search
A C++ based implementation of Burrows Wheel Transformation and Searching using Backward Search

## Formats
### BWT encode
the encoding program accepts commandline input arguments:
- the delimiter - it can be any visible ASCII alphabet (with ASCII value from 32 to 126) or a newline. If it is a newline, it will be specified as '\n'
- the path to an index or temporary folder (see below for details);
- the path to a file containing text values separated by the specified delimiter; and
- the path to the encoded file to be generated.

### BWT search
the search program accepts commandline input arguments:
- the delimiter;
- the path to a BWT encoded file;
- the path to an index folder;
- either -m for the number of matching substrings (count duplicates), -n for the number of unique matching records, or -a for listing the identifiers of all the matching records (no duplicates and in ascending order), -i for displaying the content of the records with ids provided in the search term; and
- a quoted query string (i.e., the search term).


**Note:** Report bugs, improvements and suggestions at sahil.khalsaboy@gmail.com
