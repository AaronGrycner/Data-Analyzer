// Aaron Grycner 
// agrycner@cnm.edu 
// grycnerP8

#include "Header.h"

int main()
{
	// array containing a yearData struct for each year of rainfall and monthdata structs for months

	string fileName{ "rainfall_data.csv" };
	int dataSize{}, endYear{}, startYear{}, dataYears{};
	double precipChange{}, durationChange{}, intensChange{}, timeChange{};
	YearData years[14];

	WriteHeader();

	cout << "\nAnalyzing data...\n";

	// checks for successful file open and reads 
	if (!ReadDataSizes(dataSize, fileName, endYear, startYear, dataYears)) {
		cout << "\nFile open error!\n";
		return 0;
	}

	// checks for successful file open and reads data into array of structs
	if (!Read(fileName, years, dataSize, dataYears, endYear)) {
		cout << "\nFile open error!\n";
		return 0;
	}

	AnalyzeData(years, dataYears);

	// checks for file write errors
	if (!WriteFile(years, dataYears)) {
		cout << "\nFile write error!\n";
		return 0;
	}

	cout << "\nFile created successfully!\n";
	
	return 0;

}
