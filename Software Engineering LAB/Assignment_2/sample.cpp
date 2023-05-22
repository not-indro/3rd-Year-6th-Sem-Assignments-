#include <iostream>
using namespace std;

void hello(int num)
{
    if (num == 0)
        return;
    cout << "Hello :" << num << endl;
    hello(num - 1);
}
int main()
{
    int x = 4;
    cout << "Enter a number: ";
    int num;
    cin >> num;
    int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    for (int i = 0; i < 10; i++)
    {
        cout << arr[i] << " " << endl;
    }
    hello(x);
    return 0;
}