#include "Header.h"

void WriteHeader() {
	cout << "\nAaron Grycner, GrycnerP8\n" << "Analyzing rainfall near Los Alamos since 2009\n";
}

bool ReadDataSizes(int& dataSize, string fileName, int& endYear, int& startYear, int& dataYears)
{
	ifstream input(fileName);
	string trash{};
	string temp{};

	if (!input)
		return false;

	// counts number of lines in data file
	while (!input.eof()) {
		getline(input, trash);
		++dataSize;
	}

	dataSize -= 1;

	input.clear();
	input.seekg(0);

	// determines the end year and stores it in endYear
	getline(input, trash);
	getline(input, trash, '/');
	getline(input, trash, '/');
	getline(input, temp, ',');
	endYear = stoi(temp);
	
	input.clear();
	input.seekg(0);

	// determines start year
	for (int i{ 1 }; i < dataSize; ++i) {
		getline(input, trash);
	}

	getline(input, trash, '/');
	getline(input, trash, '/');
	getline(input, temp, ',');
	startYear = stoi(temp);

	input.close();

	// calculates number of years of data
	dataYears = (endYear - startYear) + 1;

	return true;
}

bool Read(string fileName, YearData years[], int dataSize, int dataYears, int endYear)
{
	string trash{};
	string temp{};
	ifstream input{ fileName };
	string lineYear{};

	if (!input)
		return false;

	// trash first line
	getline(input, trash);

	// trash month and day in date column
	getline(input, trash, '/');
	getline(input, trash, '/');

	for (int i{}; i < dataYears; ++i) {
		int structYear{ endYear - i };
		

		// sets the year of the struct currently being operated on
		years[i].year = structYear;

		do { /// change code to annihilate invalid lines

			// collect current line year then reverts back to beginning of line
			int current{};
			bool valid{true};
			current = input.tellg();
			getline(input, trash, '/');
			getline(input, trash, '/');
			getline(input, lineYear, ',');
			input.seekg(current);

			// breaks the loop if the data line is not within the right year 
			if (lineYear != to_string(structYear))
				break;

			// trash date column
			getline(input, trash, ',');

			// gets datapoint for total precipitation and stores it in struct array
			getline(input, temp, ',');

			// checks for valid data (NV = not valid)
			if (temp != "NV" && temp != "")
				years[i].precipitation.push_back(stod(temp));
			else {
				years[i].precipitation.push_back(-1);
				valid = false;
			}
				
			// trash unit column
			getline(input, trash, ',');

			// gets datapoint for duration and stores it in struct vector
			getline(input, temp, ',');
			if (temp != "NV" && temp != "")
				years[i].duration.push_back(stod(temp));
			else {
				years[i].duration.push_back(-1);
				valid = false;
			}
				

			// gets datapoint for intensity and stores it in struct vector
			getline(input, temp, ',');
			if (temp != "NV" && temp != "")
				years[i].intensity.push_back(stod(temp));
			else {
				years[i].intensity.push_back(-1);
				valid = false;
			}
				

			// gets datapoint for time and stores it in struct vector
			getline(input, temp);
			if (temp != "NV" && temp != "")
				years[i].time.push_back(stod(temp));
			else {
				years[i].time.push_back(-1);
				valid = false;
			}
			
			// if data is not valid, destroys it
			if (!valid) {
				years[i].precipitation.pop_back();
				years[i].duration.pop_back();
				years[i].intensity.pop_back();
				years[i].time.pop_back();
			}

			// peeks next character, looking for end of file otherwise eof flag will not trigger
			input.peek();

		} while (!input.eof());

	}

	input.close();

	return true;
}

void AnalyzeData(YearData years[], int dataYears) {

	for (int i{}; i < dataYears; ++i) {
		double precipitationTotal{}, durationTotal{}, intensityTotal{}, timeTotal{};

		// sum up the vectors of raw data
		for (int x{}; x < years[i].precipitation.size(); ++x) {
			precipitationTotal += years[i].precipitation[x];
			durationTotal += years[i].duration[x];
			intensityTotal += years[i].intensity[x];
			timeTotal += years[i].time[x];
		}

		// calculate the averages from the totals and store them in their struct members
		years[i].yearPrecipitation = precipitationTotal / years[i].precipitation.size();
		years[i].yearDuration = durationTotal / years[i].duration.size();
		years[i].yearIntensity = intensityTotal / years[i].intensity.size();
		years[i].yearTime = timeTotal / years[i].time.size();
	}
}

bool WriteFile (YearData years[], int dataYears) {
	stringstream st;
	string newFile{};
	ofstream output;

	// assemble filename
	cout << "\nPlease enter a name for the file (do not include filetype): ";
	cin >> newFile;
	st << newFile << ".txt";
	newFile = st.str();

	// clear stringstream
	st.str(string());

	st << setprecision(3) << fixed << newFile << "\nData provided by Los Alamos National Laboratories\n-	https://www.intellusnm.com/reporting/quick-search/quick-search.cfm \n";

	st << "\n---Description---\n";

	st << "\nThis data contains measurements of rainfall around the Los Alamos area.\n"
		"\nThe data shows the average amount of rainfall when rainfall occurs, \n"
		<< "the average duration of rainfall when it occurs, the intensity\n"
		"in inches per hour of rainfall, and the average time of day that it occurs.\n";
		

	st << "\n---Average Changes---\n";

	// Determine change in average precipitation and write message to file
	if (years[0].yearPrecipitation < years[dataYears - 1].yearPrecipitation) {
		st << "\nAverage precipitation per rainfall has decreased by " << years[dataYears - 1].yearPrecipitation - years[0].yearPrecipitation << " inches from "
			<< years[dataYears - 1].year << " to " << years[0].year << ".";
	}
	else if (years[0].yearPrecipitation > years[dataYears - 1].yearPrecipitation) {
		st << "\nAverage precipitation per rainfall has increased by " << years[0].yearPrecipitation - years[dataYears - 1].yearPrecipitation << " inches from "
			<< years[dataYears - 1].year << " to " << years[0].year << ".";
	}
	else
		st << "\nAverage precipitation has not changed\n";

	// determine change in duration and write message to file
	if (years[0].yearDuration < years[dataYears - 1].yearDuration) {
		st << "\nAverage precipitation duration per rainfall has decreased by " << years[dataYears - 1].yearDuration - years[0].yearDuration << " hours from "
			<< years[dataYears - 1].year << " to " << years[0].year << ".";
	}
	else if (years[0].yearDuration > years[dataYears - 1].yearDuration) {
		st << "\nAverage precipitation duration per rainfall has increased by " << years[0].yearDuration - years[dataYears - 1].yearDuration << " hours from "
			<< years[dataYears - 1].year << " to " << years[0].year << ".";
	}
	else
		st << "\nAverage precipitation duration has not changed";

	// determine change in intensity and write message to file
	if (years[0].yearIntensity < years[dataYears - 1].yearIntensity) {
		st << "\nAverage precipitation intensity per rainfall has decreased by " << years[dataYears - 1].yearIntensity - years[0].yearIntensity << " in/hr from "
			<< years[dataYears - 1].year << " to " << years[0].year << ".";
	}
	else if (years[0].yearIntensity > years[dataYears - 1].yearIntensity) {
		st << "\nAverage precipitation intensity per rainfall has increased by " << years[0].yearIntensity - years[dataYears - 1].yearIntensity << " in/hr from "
			<< years[dataYears - 1].year << " to " << years[0].year << ".";
	}
	else
		st << "\n\nAverage precipitation intensity has not changed.";

	st << "\n\n---Averages by Year---\n";

	for (int i{}; i < dataYears; ++i) {

		st << endl << " - " << years[i].year << " - " << endl
			<< "Average precipitation per rainfall: " << years[i].yearPrecipitation << " inches." << endl
			<< "Average duration of precipitation per rainfall: " << years[i].yearDuration << " hours." << endl
			<< "Average intensity of precipitation per rainfall: " << years[i].yearIntensity << " in/hr." << endl
			<< "Average time of day of precipitation: " << years[i].yearTime << " o'clock." << endl;
	}

	output.open(newFile.c_str());

	if (!output)
		return false;

	output << st.str();
	output.close();

	return true;

}

