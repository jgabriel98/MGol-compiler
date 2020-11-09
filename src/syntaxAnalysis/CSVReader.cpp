/* author: https://thispointer.com/how-to-read-data-from-a-csv-file-in-c/ */
#include "CSVReader.h"

#include <boost/algorithm/string.hpp>
#include <fstream>
#include <string>
#include <vector>

CSVReader::CSVReader(std::string filename, std::string delm) : fileName(filename), delimeter(delm) {}

/* Parses through csv file line by line and returns the data
 * in vector of vector of strings.
 */
std::vector<std::vector<std::string> > CSVReader::getData() {
	std::ifstream file(fileName);
	std::vector<std::vector<std::string> > dataList;
	std::string line = "";

	// Iterate through each line and split the content using delimeter
	while (getline(file, line)) {
		std::vector<std::string> vec;
		boost::algorithm::split(vec, line, boost::is_any_of(delimeter));
		dataList.push_back(vec);
	}

	file.close();
	return dataList;
}