#include<iostream>
#include<thread>
#include<mutex>
#include<fstream>
#include<sstream>
#include<cstdio>
#include<string>
#include<limits>
#include<vector>
#include<ctime>
#include<chrono>

using namespace std;

void printer(ifstream &infile, ofstream &outfile, mutex &mtx_read, mutex &mtx_write, int id, int &turn_r, int &turn_w, int &threadnum)
{
	vector<string> row;
	string line;
	string number;
	int total = 0;
	int line_counter = 0;
	bool end_file = false;
	while(!end_file)
	{
		while(true)
		{
			mtx_read.lock();
			if(turn_r != id) //waiting for turn
			{
				mtx_read.unlock();
				continue;
			}
			end_file = getline(infile,line).eof();
			turn_r++;
			turn_r = turn_r % threadnum; //changing the turn ithout overflow
			mtx_read.unlock();
			break;
		}
		if(end_file)
			return;
		istringstream iss(line);
		ostringstream output;
		for(int i = 0; i <= 19 ; i++)
        	{
			getline(iss,number,'|');
                	row.push_back(number);
			if(i == 0)
        		{
                		output << "\t" << "{" << "\n";
                		output << "\t\t" << "\"col_" << i+1 << "\":" << row[i] << ",\n";
        		}
       			else if(i == 19)
        		{
				output << "\t\t" << "\"col_" << i+1 << "\":" << row[i] << "\n";
				output << "\t" << "},";
        		}
        		else if(i != 0 || i != 19)
        		{
                		output << "\t\t" << "\"col_" << i+1 << "\":" << row[i] << ",\n";
        		}	
        	}
		for(int i=0;i<=19;i++)
		{
			row.pop_back();
		
		}
		while(true)
		{
			mtx_write.lock();
			if(turn_w != id)
			{
				mtx_write.unlock();
				continue;
			}
			outfile << output.str() << endl;
			turn_w++;
			turn_w = turn_w % threadnum;
			mtx_write.unlock();
			break;
		}
	}
}

int main(int argc, char* argv[])
{
	chrono::high_resolution_clock::time_point start,end;
	start = chrono::high_resolution_clock::now();
	ifstream infile("input.csv");
	int numthread = atoi(argv[1]);
	int turn_r = 0;
	int turn_w = 0;
	thread t[numthread];
	mutex mtx_read;
	mutex mtx_write;
	string line;
	ofstream outfile("output.json");
	if(infile.fail())
        {
                cerr << "error opening the file" << argv[1] << "\n";
                return 2;
        }
	outfile  << "[" << "\n";
	for(int k = 0; k < numthread; ++k)
	{	
        	t[k] = thread(printer, ref(infile), ref(outfile), ref(mtx_read), ref(mtx_write), k, ref(turn_r), ref(turn_w), ref(numthread));
	}
        for(int k = 0; k < numthread; ++k)
        {
                t[k].join();
        }
	outfile.seekp(-2,std::ios_base::end);
        outfile << "\n]";// << "\n";
	infile.close();
	outfile.close();

	end = chrono::high_resolution_clock::now();
	cout << "execution time: " << chrono::duration_cast<chrono::milliseconds>(end-start).count()/1000.0 << " sec" << endl;
        return 0;
}
