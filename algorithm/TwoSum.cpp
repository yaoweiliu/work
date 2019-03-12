/*
 * Example: 
 * Given nums = [2, 7, 11, 15], target = 9,
 * Because nums[0] + nums[1] = 2 + 7 = 9,
 * return [0, 1].
 */

#include <iostream>
#include <vector>

using namespace std;

vector<int> twoSum(vector<int>& nums, int target)
{
	int i, j;
	vector<int> tmp;

	for(i = 0; i < nums.size(); i++) {
		for(j = i+1; j < nums.size(); j++) {
			if(nums[i] + nums[j] == target) {
				tmp.push_back(i);
				tmp.push_back(j);

				break;
			}
		}
	}

	return tmp;
}

int main(int argc, char const *argv[])
{
	vector<int> nums={1, 2, 3, 4, 5, 6, 7};//g++ -std=c++11, or c++98 not support this stytle.
	vector<int> tmp;
	vector<int>::iterator it;

	tmp = twoSum(nums, 13);

	for(it=tmp.begin(); it!=tmp.end(); it++)
		cout << *it << " ";
	cout << endl;

	return 0;
}
