#ifndef HEADER_H
#define HEADER_H

using namespace std;
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

struct YearData {

	int year{};

	// vectors containing raw data
	vector <double> precipitation;
	vector <double> duration{};
	vector <double> intensity;
	vector <double> time{};

	// averages by year
	double yearPrecipitation{};
	double yearDuration{};
	double yearIntensity{};
	double yearTime{};

};

void WriteHeader();
bool ReadDataSizes(int &dataSize, string fileName, int &endYear, int &startYear, int &dataYears);
bool Read(string fileName, YearData years[], int dataSize, int dataYears, int endYear);
void AnalyzeData(YearData years[], int dataYears);
bool WriteFile(YearData years[], int dataYears);

#endif