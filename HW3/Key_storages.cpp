#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<sstream>
#include<map>
#include<filesystem>
#include<experimental/filesystem>
#include<bits/stdc++.h>

using namespace std;
namespace fs = std::experimental::filesystem;

int main(int argc, char* argv[])
{
        clock_t start, end;
	/*string curr_path = fs::current_path();
        cout << "current path is in " << curr_path << endl;*/
	string file_path = argv[1];
        fs::path pathToShow(file_path);
	/*string input_loc = pathToShow.parent_path();
        cout << "the input file is in: " << input_loc << endl;
        string file_name = pathToShow.filename();*/
        std::map<long long int, long long int> key_storage;
	//fs::current_path(input_loc);
        //ifstream infile(pathToShow.filename());
	ifstream infile(argv[1]);
	//fs::current_path(curr_path);
        ifstream infile2;
        ifstream infile3;
        ofstream outfile;
        ofstream outfile2;
        ofstream outfile3;
        ofstream outfile4;
        long long int counter = 0;
        string command;
        string value;
        string line;
        string line2;
        string line3;
        long long int key;
        long long int key2;
        string loc = fs::current_path();
	//cout << "output file is in: " << loc << endl;
        fs::path p1(loc);
        fs::create_directory("storage");
        p1 /= "storage";
	//cout << "storage is in: " << p1 << endl;
	fs::current_path(p1);
        long long int key_temp;
        long long int counter_temp = 0;
	start = clock();
        infile2.open("key_counter.temp");
        if(infile2){
        	while(getline(infile2,line2)){
			stringstream ss2(line2);
                        ss2 >> key_temp >> counter_temp;
                        key_storage[key_temp] = counter_temp;
                }
		counter_temp++;
		infile2.close();
        }
        while(getline(infile, line))
        {
		fs::current_path(loc);
		stringstream ss(line);
                ss >> command;
                if(command == "PUT")
                {
                        fs::current_path(p1);
                        outfile.open("key_counter.temp",ios::app);
                        outfile2.open("value.temp", ios::app);
                        ss >> key >> value;
                        outfile << key << " " << counter_temp << "\n";
                        outfile2 << value << "\n";
			key_storage[key] = counter_temp;
			counter_temp++;
                        outfile.close();
                        outfile2.close();
                }
                else if(command == "GET")
                {
                        ss >> key;
                        fs::current_path(p1);
                        infile3.open("value.temp");
                        fs::current_path(loc);
                        string out_name = pathToShow.stem();
                        string file_type = ".output";
                        string output_name = out_name + file_type;
                        outfile2.open(output_name, ios::app);
                        std::map<long long int, long long int>::iterator it = key_storage.find(key);
                        if(it->second >= 90000000000000){
                                outfile2 << "EMPTY\n";
                        }
                        else if(it->second < 90000000000000){
                                infile3.seekg(129*it->second, ios::beg);
                                char A[129];
                                infile3.read(A, 128);
                                A[129] = 0;
                                outfile2 << A << "\n";
                        }
                        outfile2.close();
                        fs::current_path(p1);
                        infile3.close();
                }
                else if(command == "SCAN")
                {
                        ss >> key >> key2;
                        string out_name = pathToShow.stem();
                        string file_type = ".output";
                        string output_name = out_name + file_type;
                        for(long long int key1 = key; key1 <= key2; key1++)
                        {
                                fs::current_path(p1);
				infile3.open("value.temp");
                                fs::current_path(loc);
                                outfile2.open(output_name, ios::app);
                                std::map<long long int, long long int>::iterator it = key_storage.find(key1);
                                if(it->second >= 90000000000000){
                                        outfile2 << "EMPTY\n";
                                }
                                else if(it->second < 90000000000000){
                                        infile3.seekg(129*it->second, ios::beg);
                                        char A[129];
                                        infile3.read(A, 128);
                                        A[129] = 0;
                                        outfile2 << A << "\n";
                                }
                                outfile2.close();
                                fs::current_path(p1);
                                infile3.close();
                        }
                }
        }
	end = clock();
	cout << "Time: " << double(end - start)/CLOCKS_PER_SEC << " sec" << "\n";

        return 0;
}
