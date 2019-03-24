/* g++ -std=c++11 -o lengthOfLongestSubstring lengthOfLongestSubstring.cpp */

#include <iostream>
#include <string>
#include <unordered_map>
#include <cstddef>

using namespace std;

int lenthOfLongestSubstring(string s)
{
	size_t max_count = 0, new_count = 0;
	unordered_map<char, size_t> strMap;

	for(size_t i = 0; i < s.size(); i++) {
		auto it = strMap.find(s[i]);
		if(it == strMap.end()) {
			//not find.
			new_count++;
			if(new_count > max_count)
				max_count = new_count;
		}
		else {
			new_count = min(i-strMap[s[i]], new_count+1);
			max_count = max(max_count, new_count);
		}
		strMap[s[i]] = i;
	}

	return max_count;
}

int main(int argc, char const *argv[])
{
	string str = "abcdeabcd";
	size_t len = -1;

	len = lenthOfLongestSubstring(str);
	cout << "len = " << len << endl;

	return 0;
}


