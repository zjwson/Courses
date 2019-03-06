#include <iostream>
#include <vector>

using namespace std;

vector<int> getIndex(vector<int>& nums, int target){
    vector<int> result;
    unsigned int i, j;
    for(i = 0; i < nums.size(); ++i){
        for(j = i+1; j < nums.size(); ++j){
            if(nums[i] + nums[j] == target){
                result.push_back(i);
                result.push_back(j);
            }
        }
    }
    return result;
}

int main(){

    vector<int> nums;
    nums.push_back(2);
    nums.push_back(7);
    nums.push_back(11);
    nums.push_back(15);
    int target = 9;
    vector<int>result = getIndex(nums, target);

    cout << result[0] << " " << result[1] << endl;

    return 0;
}


