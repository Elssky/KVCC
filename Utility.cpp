#include"Utility.h"

TVec<TIntV> res;
TIntV track;
void backtrack(TIntV nums, int start, int k);

TVec<TIntV> subsets(TIntV nums, int k) {
	backtrack(nums, 0, k);
	return res;
}

void backtrack(TIntV nums, int start, int k) {
	if (track.Len() == k) {
		res.Add(track);
		return;
	}
	
	for (int i = start; i < nums.Len(); i++) {
		track.Add(nums[i]);
		backtrack(nums, i + 1, k);
		track.DelLast();
	}
}


//int main() {
//	vector<int> nums = { 1,2,3 };
//	subsets(nums);
//	for (int i = 0; i < res.size(); i++) {
//		cout << "[";
//		for (int j = 0; j < res[i].size(); j++) {
//			cout << res[i][j] << " ";
//		}
//		cout << "]";
//		cout << endl;
//	}
//
//}