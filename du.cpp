/*
File Name: du.cpp
Author: Mahan Mehdipour
Date: April 18,2020
Purpose: Gets input from console and shows the results as file directories to the user
*/
//importing the libraries
#include <algorithm>
#include <iostream>
#include <Windows.h>
#include <vector>
#include <set>
#include <filesystem>
#include <string>
#include <iomanip>
#include <math.h>
using namespace std;
using namespace std::filesystem;

//declaring the global variables and vectors
vector<uintmax_t> numbers;
vector<string> folders;
vector<string> topFolders;
vector<uintmax_t> topNumbers;
vector<string> arguments;
uintmax_t clusterSize = 4096;
string version = "1.0.0";

//declaring the functions
uintmax_t getCluster(uintmax_t byteSize, uintmax_t cluster);
bool finder(string s);
void rscan(path const& f);
void scan(path const& f);
void printOut(vector<uintmax_t> numbers, vector<string> folders);
void printHelp();
bool duplicateCheck();
void displayHumanAll(vector<uintmax_t> numbers, vector<string> folders);
void sortBySizeReversed(vector<uintmax_t> & numbers, vector<string> & folders);
void sortBySize(vector<uintmax_t> & numbers, vector<string> & folders);
void displayByte(vector<uintmax_t> numbers, vector<string> folders);
void sortByName(vector<uintmax_t>& numbers, vector<string>& folders);

//main function
int main(int argc, char* argv[]) {
	//Getting info if user didn't input additional arguments
	if (argc == 1) {
		scan(".");
		printOut(numbers,folders);
		return EXIT_SUCCESS;
	}

	//storing arguments in a vector
	for (size_t i = 1; i < argc; i++)
		arguments.push_back(argv[i]);

	//Checks to see if there is a duplicate in arguments
	if (!duplicateCheck())
		return EXIT_FAILURE;

	//would print out help if 'help' argument is found
	if (finder("--help")) {
		printHelp();
		return EXIT_SUCCESS;
	}
	
	//would print out the version of the project 
	if (finder("--version")) {
		cout<< version<<endl;
		return EXIT_SUCCESS;
	}
	
	bool blockSize = false;

	//This for loop would go through arguments and would change the cluster size if needed, 
	//This for loop would also scan the file if the directory entered in the console exists
	for (size_t i = 0; i < arguments.size(); i++) {
		if (arguments[i].find("--block-size=") != string::npos) {
			blockSize = true;
			string blockSizeArgString = arguments.at(i).substr(13, 6);
			try {
				clusterSize = stoi(blockSizeArgString);
			}
			catch (exception a) {
				cout << "Error: block-size value is invalid <"<<blockSizeArgString<<">";
				return EXIT_FAILURE;
			
			}
			
				
		}
		if (arguments[i][0] != '-') {
			path corAdd= ".\\" +arguments[i];
			path finalAdd = arguments[i];
			
			if (exists(corAdd)||exists(absolute(finalAdd))) {
				scan(finalAdd);


			}
			else {
				cout << "Directory <" << arguments[i] << "> does not exist\n";
				return EXIT_FAILURE;
			}
		}
	}
	
	bool h = false;
	bool s = false;
	bool b = false;
	bool k = false;
	bool z = false;
	bool n = false;
	bool r = false;
	//would go through the switches and validates the the switches and would change the booleans to true if needed; 
	if (arguments[0][0] == '-' && arguments[0][1] != '-') {
		for (size_t i = 1; i < arguments[0].length() ; ++i) {
			switch (arguments[0][i]) {
			case 'h':
				if (h == true) {
					cout << "Error: duplicate switches: <h> \n";
					return EXIT_FAILURE;
				}
				else if (b == true) {
					cout << "Error: cannot use both -b and -h \n";
					return EXIT_FAILURE;
				}
				else {
					h = true;
					continue;
				}
				break;
			case 's':
				if (s == true) {
					cout << "Error: duplicate switches: <s> \n";
					return EXIT_FAILURE;
				}
				else {
					s = true;
					continue;
				}
				break;
			case 'b':
				if (b == true) {
					cout << "Error: duplicate switches: <b> \n";
					return EXIT_FAILURE;
				}
				else if (h == true) {
					cout << "Error: cannot use both -b and -h \n";
					return EXIT_FAILURE;
				}
				else {
					b = true;
					continue;
				}
				break;
			case 'k':
				if (k == true) {
					cout << "Error: duplicate switches: <k> \n";
					return EXIT_FAILURE;
				}
				else if (blockSize == true) {
					cout << "Error: -k and --block-size are incompatible. \n";
					return EXIT_FAILURE;
				}
				else {
					clusterSize = 1024;
					k = true;
					continue;
				}
				break;
			case 'z':
				if (z == true) {
					cout << "Error: duplicate switches: <z>\n";
					return EXIT_FAILURE;
				}
				else if (n == true) {
					cout << "Error: -n and -z switches are incompatible.\n";
					return EXIT_FAILURE;
				}
				else {
					z = true;
					continue;
				}
				break;
			case 'n':
				if (n == true) {
					cout << "Error: duplicate switches: <n>\n";
					return EXIT_FAILURE;
				}
				else if (z == true) {
					cout << "Error: -n and -z switches are incompatible. \n";
					return EXIT_FAILURE;
				}
				else {
					n = true;
					continue;
				}
				break;
			case 'r':
				if (r == true) {
					cout << "Error: duplicate switches: <r> \n";
					return EXIT_FAILURE;
				}
				else {
					r = true;
					continue;
				}
				break;
			default:
				cout << "Error: unknown switches: <"<<arguments[0][i] <<"> \n";
				return EXIT_FAILURE;
				break;
			}
		}
	}
	//get information if there is not any entered
	if (numbers.size() == 0)
		scan(".");

	//would sort by size if user entered z
	if (z == true) {
		
		sortBySize(numbers, folders);
		sortBySize(topNumbers, topFolders);
		
		
	}
	//would sort by address if user entered n
	if (n == true) {
		sortByName(numbers, folders);
		sortByName(numbers, folders);
	
	}

	//would reverse if user entered r
	if (r == true) {
		sortBySizeReversed(numbers, folders);
		sortBySizeReversed(topNumbers, topFolders);
	}

	//would display in human readable format if user entered h
	if (h == true && s == false) {
		displayHumanAll(numbers, folders);
		return EXIT_SUCCESS;
	}
	else if (h == true && s == true) {
		displayHumanAll(topNumbers, topFolders);
		return EXIT_SUCCESS;
	}

	//would display in binary format if user entered h
	if (b == true && s == false) {
		displayByte(numbers,folders);
		return EXIT_SUCCESS;
	}
	else if (b == true && s == true) {
		displayByte(topNumbers, topFolders);
		return EXIT_SUCCESS;
	}

	//would display the summary if user entered s
	if (s == true) {
		printOut(topNumbers, topFolders);
		return EXIT_SUCCESS;
	}

	//would print out if none of the above conditions were true
	printOut(numbers,folders);
	return EXIT_SUCCESS;

}
//definition of the functions used
void rscan(path const& f) {
	uintmax_t sizeClust = 0;
	for (auto d : recursive_directory_iterator(f)) {
		if (!is_directory(d.status())) {
			sizeClust += getCluster(d.file_size(), clusterSize);
		}
	}
	numbers.push_back(sizeClust);
} 

void scan(path const& f) {
	directory_iterator d(f);
	directory_iterator e;
	while (d != e) {
		if (is_directory(d->status())) {
			rscan(d->path());
			folders.push_back(d->path().string());
		}
		++d;
	}
	uintmax_t whole = 0;
	for (auto d : recursive_directory_iterator(f)) {
		if (!is_directory(d.status()))
			whole += getCluster(file_size(d), clusterSize);
	}
	numbers.push_back(whole);
	folders.push_back(f.string());
	topNumbers.push_back(whole);
	topFolders.push_back(f.string());
}

uintmax_t getCluster(uintmax_t byteSize, uintmax_t cluster) {
	if (byteSize % cluster > 0) {
		byteSize = byteSize / cluster + 1;
	}
	else {
		byteSize /= cluster;
	}


	return byteSize;
}
//----
void printOut(vector<uintmax_t> numbers, vector<string> folders) {
	size_t highest=0;
	for (size_t i = 0; i < numbers.size(); ++i) {
		if (highest < numbers[i])
			highest = numbers[i];
	}
	highest = (size_t)log10(highest) + 1;


	for (size_t i = 0; i < numbers.size(); ++i)
		cout <<left<<setw(highest)<< numbers[i] << "   " << folders[i] << endl;

}

bool finder(string s) {
	vector<string>::iterator it = find(arguments.begin(), arguments.end(), s);
	if (it == arguments.end())
		return false;
	else
		return true;
}

void printHelp() {
	vector<string> strings;
	cout << endl << "du (c) 2020, Mahan Mehdipour\n" << "===========================================================\n";
	cout << "Version " << version;
	cout << "\n\nA disk usage utility inspired by the UNIX du command.\n\n";
	cout << "Usage: du [-skhb] [--help] [--version] [--block-size=dddd] [folder]*\n\nExamples:\n";
	cout << setw(4) << "du" << endl;
	cout << setw(61) << "> display the sum of the cluster sizes of each directory" << endl
		<< setw(23) << "starting the cwd" << "\n\n";
	cout << setw(11) << "du folder" << endl;
	cout << setw(61) << "> display the sum of the cluster sizes of each directory" << endl
		<< setw(29) << "starting with 'folder'" << "\n\n";

	strings.push_back("du -h");
	strings.push_back("  > display the results in a human readable form");

	strings.push_back("du -s");
	strings.push_back("  > display only the final summary");

	strings.push_back("du -b");
	strings.push_back("  > display in bytes");

	strings.push_back("du -k");
	strings.push_back("  > cluster size is 1024");

	strings.push_back("du -z");
	strings.push_back("  > display the list sorted by size");

	strings.push_back("du -n");
	strings.push_back("  > display the list sorted by name");

	strings.push_back("du -r");
	strings.push_back("  > display the list in reverse order");

	strings.push_back("du --block-size=dddd");
	strings.push_back("  > set the cluster size to the specified integer > 0");

	strings.push_back("du --help");
	strings.push_back("  > displays the help");

	strings.push_back("du --version");
	strings.push_back("  > diplays version number in the format d.d.d");

	for (int i = 0; i < strings.size()-1; i++) {
		cout << setw(strings[i].length() + 2) << strings[i] << endl;
		if (i % 2 == 1)cout << endl;
	}
	cout << setw(strings[strings.size()].length()-1)<<strings[strings.size()-1]<<endl;
}

bool duplicateCheck() {
	for (int i = 0; i < arguments.size() - 1; ++i) {
		for (int j = i + 1; j < arguments.size(); j++) {
			if (arguments[i] == arguments[j]&&(arguments[i][0]=='-'||arguments[i][1]=='-')) {
				cout << "The argument <" << arguments[i] << "> is used more than once!";
				return false;
			}
		}
	}
	return true;
}
//----
void displayHumanAll( vector<uintmax_t> numbers, vector<string> folders) {
	double tByte = pow(2, 40);
	double gByte = pow(2, 30);
	double mByte = pow(2, 20);
	double kByte = pow(2, 10);
	double ans;
	for (size_t i = 0; i < numbers.size();i++) {
		
		if (numbers[i] * clusterSize > tByte) {
			ans = numbers[i] * clusterSize / (double)tByte;
			if (ans < 10) {
				ans = round(ans * 10) / 10.0;
				cout <<setprecision(1) <<fixed << ans << "T" << "   " << folders[i] << endl;
			}
			else {
				ans = round(ans);
				cout << setprecision(0) << fixed << ans << "T" << "   " << folders[i] << endl;
			}
			
		}
		else if (numbers[i] * clusterSize > gByte) {
			ans = numbers[i] * clusterSize / (double)gByte;
			if (ans < 10) {
				ans = round(ans * 10) / 10.0;
				cout << setprecision(1) << fixed << ans << "G" << "   " << folders[i] << endl;
			}
			else {
				ans = round(ans);
				cout << setprecision(0) << fixed << ans << "G" << "   " << folders[i] << endl;
			}
		}
		else if (numbers[i] * clusterSize > mByte) {
			ans = numbers[i] * clusterSize / (double)mByte;
			if (ans < 10) {
				ans = round(ans * 10) / 10.0;
				cout << setprecision(1) << fixed << ans << "M" << "   " << folders[i] << endl;
			}
			else {
				ans = round(ans);
				cout << setprecision(0) << fixed << ans << "M" << "   " << folders[i] << endl;
			}
		}
		else if (numbers[i] * clusterSize > kByte) {
			ans = numbers[i] * clusterSize / (double)kByte;
			if (ans < 10) {
				ans = round(ans * 10) / 10.0;
				cout << setprecision(1) << fixed << ans << "K" << "   " << folders[i] << endl;
			}
			else {
				ans = round(ans);
				cout << setprecision(0) << fixed << ans << "K" << "   " << folders[i] << endl;
			}
		}
		else {
			ans = numbers[i] * (double)clusterSize;
			ans = round(ans * 10) / 10.0;
			cout << ans << "B" << "   " << folders[i] << endl;
		}

	}


}

void sortBySize(vector<uintmax_t> & numbers, vector<string> & folders) {
	
	for (size_t p = 0; p < numbers.size() - 1; p++)
	{
		for (size_t i = 0; i < numbers.size() - 1; i++)
		{
			if (numbers[i] > numbers[i + 1])
			{
				swap(numbers[i], numbers[i + 1]);
				swap(folders[i], folders[i + 1]);
			}
		}
	}
	
	
} 

void sortByName(vector<uintmax_t>& numbers, vector<string>& folders) {

	for (size_t p = 0; p < folders.size() - 1; p++)
	{
		for (size_t i = 0; i < folders.size() - 1; i++)
		{
			if (folders[i] > folders[i + 1])
			{
				swap(numbers[i], numbers[i + 1]);
				swap(folders[i], folders[i + 1]);
			}
		}
	}


}

void sortBySizeReversed(vector<uintmax_t> & numbers, vector<string>& folders) {

	reverse(numbers.begin(), numbers.end());
	reverse(folders.begin(), folders.end());
}
//----
void displayByte(vector<uintmax_t> numbers, vector<string> folders) {
	size_t highest = 0;
	for (size_t i = 0; i < numbers.size(); ++i) {
		if (highest < numbers[i]*clusterSize)
			highest = numbers[i]*clusterSize;
	}
	highest = (size_t)log10(highest) + 1;
	
	for (size_t i = 0; i < numbers.size(); ++i) {
		cout << left << setw(highest) << numbers[i] * clusterSize << "   " << folders[i] << endl;
	}
}