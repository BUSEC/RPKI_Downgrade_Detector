#include<utility>
#include<map>
#include<unordered_map>
#include<iostream>
#include<fstream>
#include<string>
#include<algorithm>
#include<assert.h>
#include<set>

#include "exp.cpp"

using namespace std;



int readScanROAFile(vector<ROA*>* ROAs, string* filepath){

	ifstream f;
	f.open(*filepath);
	if ( f.is_open() ) {
		string line;

		while( getline(f, line) ){
			string path;
			string datetime;
			int as = 0;

			istringstream iss(line);
			int i = 0;
			do {
	
				if ( i == 0 ) iss>>path;
				if ( i == 1 ) iss>>datetime;
				if ( i == 2 ) iss>>as;

				if ( i > 2 ) {
					string prefix;
					iss>>prefix;

					// Ignore ipv6 prefixes (ipv6 prefixes contain ':' )
					// and
					// Ignore empty strings
					if ( prefix.find(":") == std::string::npos && prefix != "") {
						string formatStr = "%d.%d.%d.%d/%d-%d";
						int ip1, ip2, ip3, ip4, prefixlen, maxlen;

						sscanf(prefix.c_str(), formatStr.c_str(), &ip1, &ip2, &ip3, &ip4, &prefixlen, &maxlen);

						ROA* roa = new ROA(as, IPPrefix(IPAddr(ip1,ip2,ip3,ip4), prefixlen), maxlen, path );
						ROAs->push_back(roa);
					}
				}

				i++;
			} while(iss);
			assert(i > 3); // Something is wrong if we didn't read in atleast for tokens from this line (bad format)

		}
		f.close();
	} else return 0;
	return 1;
}


int main(int argc, char* argv[]){
	if (argc != 4){ cout<<"detector <old file> <new file> <outputfile>"<<endl; return 0; }

	string oldFilepath = string(argv[1]);
	string newFilepath = string(argv[2]);
	string outputFilepath = string(argv[3]);

	vector<ROA*> oldROAs;
	vector<ROA*> newROAs;

	int success = readScanROAFile(&oldROAs, &oldFilepath);
	if (!success) { return 1; cout<<"Failed to load old roas :"<<oldFilepath<<endl;}

	success = readScanROAFile(&newROAs, &newFilepath);
	if (!success) { return 1; cout<<"Failed to load new roas :"<<newFilepath<<endl;}
	
	ofstream outputFile(outputFilepath.c_str());

	OutputDowngrades(&oldROAs, &newROAs, &outputFile);

	cout<<"finished sucessfully"<<endl;
}