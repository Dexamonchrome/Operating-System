#include<iostream>
#include<fstream>
#include<sstream>
#include<climits>
#include<algorithm>
#include<string>
#include<cstdio>
#include<queue>

using namespace std;

//for minheap data structure
class MinHeapNode{
public:
	int ele;
	int idx;
	MinHeapNode(int num, int loc)
	{
		ele = num;
		idx = loc;
	}
};

//to be used in heap to compare
class comparison{
public:
	bool operator() (MinHeapNode num1, MinHeapNode num2)
	{
		return num1.ele > num2.ele;
	}

};

int main(int argc, char* argv[])
{

	ifstream see(argv[1]);
        see.seekg(0,ios::end);
	cout << "Size of the file is " << see.tellg()/1000000000.0 << "GB" << "\n";
	int maxreadnum = see.tellg()/5/sizeof(int); //reads that maximum amount of number in a text file
	see.close();
	int x = 0;
	int a = 0; //numbering of sorted temp files
	bool process = false;
	
	clock_t start, end;
	start = clock();
	//int maxreadnum = 400000000/sizeof(int);
	ifstream infile(argv[1]);
	//FILE* infile = fopen(argv[1], "r");
	int* input = new int[maxreadnum]; //dynamic array to store the input numbers
	//int input[maxreadnum];
	int counter = 0;

	//cout << "size of the file is " << infile.tellg()/1000000000.0 << "GB" << endl;
	if(infile.fail())
	{
		cerr << "error opening the file" << argv[1] << "\n";
		return 2;
	}
	
	//step 1: store and sort every 10000 numbers of input text
	while(infile >> x)
	//while(fscanf(infile, "%d", &x));
	{
		process = false;
		input[counter] = x;
		counter++;
		if(counter == maxreadnum)
		{
			sort(input, input+counter);//sort number from starting of the input until input + counter
			stringstream stream;
			stream << a;
			string temp = ("temp_" + stream.str()  + ".txt");
			a++;
			//ofstream output(temp.c_str());
			FILE* output = fopen(temp.c_str(), "w");
			for(int i = 0; i < counter; i++)
				//output << input[i] << "\n";
				fprintf(output, "%d\n", input[i]);
			//output.close();
			fclose(output);
			counter = 0;
			process = true;
		}
	}

	// sort the remaining input numbers
	if(!process)
	{
		sort(input, input+counter);//sort number from starting of the input until input + counter
                stringstream stream;
                stream << a;
                string temp = ("temp_" + stream.str()  + ".txt");
		//ofstream output(temp.c_str());
		FILE* output = fopen(temp.c_str(), "w");
                for(int i = 0; i < counter; i++)
		//for(int i = counter; i--; )
                	//output << input[i] << "\n";
			fprintf(output, "%d\n", input[i]);
                //output.close();
		fclose(output);
	}

	infile.close();
	//fclose(infile);
	delete[] input; //free memory
	
	//step 2:comparing and merging the sorted files

	//opening the files to read
	ifstream* files = new ifstream[a+1];
	priority_queue<MinHeapNode, vector<MinHeapNode>, comparison>pq;
	for(int i = a+1; i--; )
	{
		//stringstream stream;
		//stream << i;
		ostringstream filename;
		filename << "temp_" << i << ".txt";
		files[i].open(filename.str());
		//files[i].open("temp_" + stream.str() + ".txt");
	}

	//ofstream output("output.txt");	
	FILE* output = fopen("output.txt", "w");
	//using the minheap to compare the min values of each file then place it to the output
	int num;
	//for(int i = 0; i < a+1; ++i)
	for(int i = a+1; i--; )
	{
		files[i] >> num;
		MinHeapNode obj(num,i);
		pq.push(obj);
	}

	while(pq.size() > 0)
	{

		MinHeapNode top = pq.top();
		pq.pop();
		//output << top.ele << "\n";
		fprintf(output, "%d\n", top.ele);
		if(files[top.idx] >> num)
		{
			top.ele = num;
			pq.push(top);
		}
	}

	end = clock();

	//closing the files and removing the temp sorted text files
	//for(int i = a+1; i < a+1; i++)
	for(int i = a+1; i--; )
	{
		files[i].close();
		string stream;
                stream = to_string(i);
		string filename("temp_" + stream + ".txt");
	
	        remove(filename.c_str());	
                //string temp = ("temp_" + stream.str()  + ".txt");
		//remove(("temp_" + stream.str() + ".txt").c_str());
	}

	//output.close();
	fclose(output);

	cout << "Time: " << double(end - start)/CLOCKS_PER_SEC << " sec" << "\n";
	
	return 0;
}
