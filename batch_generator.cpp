#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

void write(vector<string> &v, string &s)
{
	ofstream file;
	file.open(s.c_str());
	string formatter="";
    for(int i = 0; i < v.size(); i++)
    {
    	file << formatter;
        file << v[i];
        formatter = "\n";
    }
    file.close();
}

int main()
{
	string file_name = "urls.txt", batch1 = "batch1.txt", batch2 = "batch2.txt";
	ifstream infile;
	infile.open (file_name.c_str());

	string line;
	vector<string> myLines;
	while (getline(infile, line))
	{
	   myLines.push_back(line);
	}
	infile.close();

	// Creates a batch of randomly shuffled urls
	random_shuffle(myLines.begin(), myLines.end());
	write(myLines, batch1);

    // Creates a batch of sorted urls
	sort(myLines.begin(), myLines.end());
	write(myLines, batch2);

}