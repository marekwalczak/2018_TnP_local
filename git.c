#include<iostream>
	using namespace std;

	int main()
	{
		cout << "**** git commit ****";
		
		system("git add *.c");
		system("git add *.C");
		system("git add *.h");
		system("git add *.py");
		system("git commit -m  \"adding changes... \"");
		system("git push");
		
		return 0;
	}

