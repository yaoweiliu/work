#include <iostream>
#include <unordered_map>
#include <string>

using namespace std;

int main(int argc, char const *argv[])
{
	unordered_map<string, int> my_may = {{"A", 65}, {"B", 66}, {"C", 67}, {"D", 68}};
	string input;

	getline(std::cin, input);
	unordered_map<string, int>::const_iterator it = my_may.find(input);

	if(it == my_may.end())
		cout << "not find." << endl;
	else
		cout << it->first << " " << it->second << endl;
	
	return 0;
}