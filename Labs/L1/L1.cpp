#include <iostream>
#include <utility>
#include <vector>
#include <map>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <algorithm>

using namespace std;

int GetToHouse(int cur, int dest)
{
	if (abs(cur - dest) == 1) return 1;
	if (cur % 2 != dest % 2)
	{
		if (cur % 2 == 0) cur--;
		else cur++;
	}
	return abs(cur - dest) / 2;
}

int GetAway(int cur, int all)
{
	if (cur % 2 == all % 2)
	{
		if (cur % 2 == 0) cur--;
		else cur++;
	}
	return (all - cur) / 2 < (cur - 1) / 2 ? (all - cur) / 2 : (cur - 1) / 2;
}

vector<int> Routing1(int dest1, int dest2)
{
	if (dest1 > dest2) return { dest2, dest1 };
	return { dest1, dest2 };
}

int Housing1(int houses, int dest1, int dest2)
{
	if (dest1 < 1 || dest2 < 1 || houses < 1)
	{
		cerr << "Number of houses or one of destinations is negative or equal to zero" << endl;
		return -1;
	}
	if (dest1 > houses || dest2 > houses)
	{
		cerr << "One of destination exceeds number of houses" << endl;
		return -1;
	}

	int time = 0;
	int cur = 1;
	for (int i : Routing1(dest1, dest2))
	{
		time += GetToHouse(cur, i);
		cur = i;
	}
	time += GetAway(cur, houses);
	return time;
}

bool Routing2(int houses, vector<int>& dests, map<int, int>& eDests, map<int, int>& oDests)
{
	for (int i : dests)
	{
		if (i < 1)
		{
			cerr << "One of destinations is negative or equal to zero" << endl;
			return false;
		}
		if (i > houses)
		{
			cerr << "One of destination exceeds number of houses" << endl;
			return false;
		}
		if (i % 2 == 0)
		{
			if (eDests.find(i) != eDests.end()) eDests[i]++;
			else eDests[i] = 0;
		}
		else
		{
			if (oDests.find(i) != oDests.end()) oDests[i]++;
			else oDests[i] = 0;
		}
	}

	return true;
}

int Walking2(int startIndex, int endIndex, map<int, int>& dests, int& cur)
{
	if (dests.size() == 0) return 0;
	int time = 0;
	map<int, int>::iterator lIt;
	map<int, int>::iterator rIt;
	bool leftOrRight = false; // false to visit rightmost house, left to visit leftmost house

	if (startIndex == 0 && endIndex == dests.size()) // Even part of street
	{
		lIt = dests.begin();
		rIt = --dests.end();
	}
	else
	{
		if (startIndex == 0) // Left part of odd part of street
		{
			leftOrRight = true; // We need to visit leftmost house first
			lIt = dests.begin();
			rIt = dests.begin();
			for (int i = 0; i < endIndex - 1; i++)
				rIt++;
		}
		else // Right part of odd part of street
		{
			lIt = dests.end();
			rIt = --dests.end();
			for (int i = 0; i < startIndex; i++)
				lIt--;
		}
	}

	while (lIt != rIt)
	{
		if (leftOrRight)
		{
			time += GetToHouse(cur, lIt->first);
			time += lIt->second * 2;
			cur = lIt->first;
			lIt++;
		}
		else
		{
			time += GetToHouse(cur, rIt->first);
			time += rIt->second * 2;
			cur = rIt->first;
			rIt--;
		}
		leftOrRight = !leftOrRight;
	}
	// Since we can't use <= or >= with map iterators we need to use algorithm manualy one time 
	time += GetToHouse(cur, lIt->first);
	time += lIt->second * 2;
	cur = lIt->first;

	return time;
}

int Housing2(int houses, vector<int> dests)
{
	if (houses < 1)
	{
		cerr << "Number of houses is negative or equal to zero" << endl;
		return -1;
	}
	if (dests.size() == 0) return 0;
	// Filling maps for even and odd houses
	map<int, int> evenDests;
	map<int, int> oddDests;
	if (!Routing2(houses, dests, evenDests, oddDests)) return -1;

	// Checking if we should rest at 1st house
	int time = 0;
	if (oddDests.find(1) != oddDests.end())
	{
		time += (oddDests[1] + 1) * 2;
		oddDests.erase(1);
	}

	// Walking through all even houses and change our side on odd
	int cur = 2; // We might not visit any even houses, so if it is equal to 1, it can turn 0 in line after the next 1;
	time += Walking2(0, evenDests.size(), evenDests, cur);
	cur--; // Changing side

	if (oddDests.size() != 0)
	{
		// Finding where most of houses are: on the left or on the right side of odd side
		int lefts = 0;
		int rights = 0;
		for (auto i = oddDests.begin(); i->first < cur; i++)
			if (i->first < cur) lefts++;
		rights = oddDests.size() - lefts;
		if (oddDests.find(cur) != oddDests.end()) lefts++;

		if (lefts > rights)
		{
			time += Walking2(0, lefts, oddDests, cur);
		}
		else if (lefts < rights)
		{
			time += Walking2(rights, 0, oddDests, cur);
		}
		else // If both sides contains equal amount of houses we check which side is faster to visit
		{
			int lCur = cur;
			int rCur = cur;
			int lTime = Walking2(0, lefts, oddDests, lCur);
			lTime += GetAway(lCur, houses);
			int rTime = Walking2(rights, 0, oddDests, rCur);
			rTime += GetAway(rCur, houses);

			if (lTime > rTime) return time + rTime;
			return time + lTime;
		}
	}

	// Leaving
	time += GetAway(cur, houses);

	return time;
}

int getRandomNumber(int min, int max)
{
	return min + rand() % (max - min + 1);
}

vector<int> RandomNumbersCalcs()
{
	int finalSum = 0;
	int signChangesCount = 0;

	int listSize = getRandomNumber(10, 30);
	vector<int> numbers(listSize);

	for (int i = 0; i < listSize; ++i) {
		numbers[i] = getRandomNumber(-153, 37);
	}

	// Checking negative and positive numbers
	while (true) {
		int negativeCount = 0;
		for (int num : numbers) {
			if (num < 0) negativeCount++;
		}

		if (negativeCount <= listSize / 2) break;

		for (int& num : numbers) {
			if (rand() % 2 == 0) {
				num = -num;
			}
		}
		signChangesCount++;
	}

	// Even positive number on even positions
	int evenPositiveOnEvenPositions = 0;
	for (int i = 1; i < numbers.size(); i += 2) {
		if (numbers[i] > 0 && numbers[i] % 2 == 0) {
			evenPositiveOnEvenPositions++;
		}
	}

	// Digits of positive 2-digits elements
	int twoDigitPositiveNumbersDigits = 0;
	for (int num : numbers) {
		if (num >= 10 && num <= 99) {
			twoDigitPositiveNumbersDigits += 2;
		}
	}

	// Sum of odd numbers on even positions
	int sumOddOnEvenPositions = 0;
	for (int i = 1; i < numbers.size(); i += 2) {
		if (numbers[i] % 2 != 0) {
			sumOddOnEvenPositions += numbers[i];
		}
	}

	// Final sum
	finalSum = (evenPositiveOnEvenPositions + twoDigitPositiveNumbersDigits + sumOddOnEvenPositions) * signChangesCount;

	cout << "Amount of changes of number signs: " << signChangesCount << endl;
	cout << "Even positive number on even positions: " << evenPositiveOnEvenPositions << endl;
	cout << "Digits of positive 2-digits elements: " << twoDigitPositiveNumbersDigits << endl;
	cout << "Sum of odd numbers on even positions: " << sumOddOnEvenPositions << endl;
	cout << "Final sum: " << finalSum << endl << endl;

	return numbers;
}

int RandomNumbers(string filename, int cycles)
{
	srand(static_cast<unsigned int>(time(0))); // Initialization for rand

	ofstream outputFile(filename); // Opening file to write
	if (!outputFile) {
		cerr << "Не удалось открыть файл для записи!" << endl;
		return 1;
	}

	for (int i = 0; i < cycles; i++)
	{
		vector<int> numbers = RandomNumbersCalcs();

		// Writing to the file
		for (int num : numbers) {
			outputFile << num << " ";
		}
		outputFile << endl;
	}

	outputFile.close();

	return 0;
}

string generateRandomString(int length) 
{
	const string digits = "0123456789";
	const string upperLetters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	const string lowerLetters = "abcdefghijklmnopqrstuvwxyz";
	const string specialChars = "!#%:,.;*()[]{}<>/?@&-+= ";

	vector<char> chars;

	// Adding at least 1 required symbol from every set of chars
	chars.push_back(digits[rand() % digits.size()]);
	chars.push_back(upperLetters[rand() % upperLetters.size()]);
	chars.push_back(lowerLetters[rand() % lowerLetters.size()]);
	chars.push_back(specialChars[rand() % specialChars.size()]);

	for (int i = 4; i < length; ++i) 
	{
		int choice = rand() % 4;
		switch (choice) 
		{
		case 0:
			chars.push_back(digits[rand() % digits.size()]);
			break;
		case 1:
			chars.push_back(upperLetters[rand() % upperLetters.size()]);
			break;
		case 2:
			chars.push_back(lowerLetters[rand() % lowerLetters.size()]);
			break;
		case 3:
			chars.push_back(specialChars[rand() % specialChars.size()]);
			break;
		}
	}

	random_shuffle(chars.begin(), chars.end());

	string result(chars.begin(), chars.end());
	return result;
}

int RandomString() 
{
	srand(static_cast<unsigned int>(time(0))); // Initialization for rand

	int length;
	cout << "Input string length (positive integer): ";
	cin >> length;

	if (length <= 0) 
	{
		cerr << "String length must be positive" << endl;
		return 1;
	}

	if (length < 4)
	{
		cerr << "Unable to create required string with length less than 4" << endl;
		return 1;
	}

	string randomString = generateRandomString(length);

	cout << "Generated string: " << randomString << endl;

	return 0;
}

int main()
{
	//// Housing1 test cases
	//cout << Housing1(100000000, 20, 999999) << endl; // Big data
	//cout << Housing1(30, 15, 7) << endl; // Average data without specific conditions
	//cout << Housing1(15, 7, 7) << endl; // Destinations are equal
	//cout << Housing1(10, 1, 6) << endl; // First destination in start position
	//cout << Housing1(22, 1, 1) << endl; // Both destinations in start position
	//cout << Housing1(13, 11, 4) << endl; // Further destination is on edge
	//cout << Housing1(12, 13, 5) << endl; // One of destination exceeds number of houses
	//cout << Housing1(0, 1, 1) << endl; // Number of houses equal to zero
	//cout << Housing1(7, 0, 3) << endl; // One of destinations or both equal to zero
	//cout << Housing1(3, 2, 3) << endl; // Testing case of moving only horizontaly and diagonaly
	//cout << Housing1(5, 3, 5) << endl; // Testing case of moving verticaly

	//// Housing2 test cases
	//cout << Housing2(0, {2, 8, 13, 5, 7}) << endl; // -1, Incorrect number of houses
	//cout << Housing2(12, {-2, 5, 6, 8, 2}) << endl; // -1, One of destinations is negative or equal to zero
	//cout << Housing2(100, {}) << endl; // 0, No destinations
	//cout << Housing2(20, {20, 4, 4, 16, 8, 16}) << endl; // 34, Only even destinations
	//cout << Housing2(15, {15, 3, 7, 7, 15, 15}) << endl; // 24, Only odd destinations
	//cout << Housing2(12, {10, 8, 3, 7, 5, 9, 3}) << endl; // 14, Both sides have houses; central odd house; more houses on the left
	//cout << Housing2(20, {4, 1, 8, 15, 20, 4, 5, 7, 9, 1 }) << endl; // 38, Both sides have houses; central odd house; more houses on the right
	//cout << Housing2(34, {14, 13, 3, 9, 33, 17}) << endl; // 22, Both sides have houses; central odd house; equal houses on right and left; faster to leave on left
	//cout << Housing2(12, {10, 8, 3, 5, 9, 9}) << endl; // 10, Both sides have houses; no central odd house; more houses on the right
	//cout << Housing2(20, {4, 1, 8, 15, 20, 4, 5, 9, 1}) << endl; // 36, Both sides have houses; no central odd house; more houses on the left
	//cout << Housing2(38, {14, 3, 9, 33, 17}) << endl; // 18, Both sides have houses; central odd house; equal houses on right and left; faster to leave on left

	// RandomNumbers
	//RandomNumbers("3.txt", 10);

	// RandomString
	//RandomString();

	return 0;
}