#include <iostream>
#include <string>
using namespace std;

/**
 * Ask user to enter their name, and then greet them
 */
/* YOUR CODE HERE */
int main(){
    string name;
    cout<<"Enter name: ";
    cin>> name;

    cout<<"Hello, " + name + "!" <<endl;
    cout<<"Welcome to CSE 100!" <<endl;
}
