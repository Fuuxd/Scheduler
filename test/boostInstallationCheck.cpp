#include <iostream>
#include <boost/array.hpp>

using namespace std;
int main(){
  boost::array<int, 4> arr = {{1,2,3,4}};
  cout << "Boost Installed" << endl << arr[0] << endl;
  return 0;
}