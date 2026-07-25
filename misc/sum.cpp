#include <cassert>
#include <cfloat>
#include <chrono>
#include <cmath>
#include <iostream>
#include <limits>
#include <vector>
#include <algorithm>

using namespace std;

bool nearly_equal(float a, float b, float epsilon = 128 * FLT_EPSILON, float abs_th = FLT_MIN);
bool nearly_equal(double a, double b, double epsilon = 128 * DBL_EPSILON, double abs_th = DBL_MIN);

bool nearly_equal(float a, float b, float epsilon, float abs_th) {
  assert(std::numeric_limits<float>::epsilon() <= epsilon);
  assert(epsilon < 1.f);

  if (a == b) return true;

  auto diff = std::abs(a-b);
  auto norm = std::min((std::abs(a) + std::abs(b)), std::numeric_limits<float>::max());
  return diff < std::max(abs_th, epsilon * norm);
}

bool nearly_equal(double a, double b, double epsilon, double abs_th) {
  assert(std::numeric_limits<double>::epsilon() <= epsilon);
  assert(epsilon < 1.f);

  if (a == b) return true;

  auto diff = std::abs(a-b);
  auto norm = std::min((std::abs(a) + std::abs(b)), std::numeric_limits<double>::max());
  return diff < std::max(abs_th, epsilon * norm);
}

vector<float> arr = {
    8.98,
    6.43,
    4.63,
    2.35,
    2.05,
    2,
    1.2,
    0.9,
    0.48,
    0.25,
    //0.07,
    //0.01,
};
float target = 30.41;
float min_val = 0.25;

void progress() {
    static auto max = std::pow(arr.size(), target / min_val );
    static int i = 0;
    i++;

    if(i % 10000 == 0) {
        std::cout << '\r' << "Progress: " << i << '/' << max << ' ' << std::flush;
    }
}

template<typename T>
void makeCombination(vector<T> &arr, T remSum, vector<T> &cur, vector<vector<T>> &res, T index) {
    progress();

    if (nearly_equal(remSum, 0)) {
        cout << '\r';
        for (float i : cur) {
            cout << i << " ";
        }
        cout << endl;

        return;
    }

    if (remSum < 0 || index >= arr.size())
        return;
	
    cur.push_back(arr[index]);

    makeCombination(arr, remSum - arr[index], cur, res, index);

    cur.pop_back();
    makeCombination(arr, remSum, cur, res, index + 1);
}

template<typename T>
vector<vector<T>> targetSumComb(vector<T> &arr, T target) {
    vector<T> cur;

    vector<vector<T>> res;
    makeCombination(arr, target, cur, res, static_cast<T>(0));

    return res;
}

int main() {
    float sum = 0;
    for(auto a : arr) sum += a;
    std::cout << sum << std::endl;

    vector<vector<float>> res = targetSumComb(arr, target);

    return 0;
}
