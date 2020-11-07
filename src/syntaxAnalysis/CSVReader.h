#ifndef CSVREADER_HEADER
#define CSVREADER_HEADER

#include <string>
#include <vector>

class CSVReader {
	std::string fileName;
	std::string delimeter;

   public:
	CSVReader(std::string filename, std::string delm = ",");
	// Function to fetch data from a CSV File
	
	std::vector<std::vector<std::string> > getData();
};

#endif