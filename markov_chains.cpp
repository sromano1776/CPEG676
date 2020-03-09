#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <time.h>
#include <unistd.h>
using namespace std;

class Pair{
	public:
		string prefix;
		vector<string> suffix;
		
		Pair(string pre, string suff){
			prefix=pre;
			suffix.push_back(suff);
		}
};

vector<Pair> markov_chain;

int find_word_in_chain(string word){
	for (int i=0; i<markov_chain.size(); i++){
		if (markov_chain.at(i).prefix.compare(word)==0){
			return i;
		}
	}
	return -1;
}

void add_markov_link(string prefix, string suffix){
	int link_index=find_word_in_chain(prefix);
	
	if(link_index==-1){
		markov_chain.push_back(Pair(prefix, suffix));
	}
	else{
		markov_chain.at(link_index).suffix.push_back(suffix);
	}
}

void parse_line(string line){
    long prefix_starting_index = 0;
    long prefix_ending_index;
    
    long suffix_starting_index;
    long suffix_ending_index;
    
    long prefix_len;
    long sufix_len;
    
    string prefix = "";
    string suffix = "";
    
    while(prefix_starting_index != string::npos){
        prefix_ending_index = line.find(" ", prefix_starting_index);
        prefix_len = prefix_ending_index - prefix_starting_index;
        
        while(prefix_len == 0){
            prefix_starting_index++;
            prefix_ending_index = line.find(" ", prefix_starting_index);
            prefix_len = prefix_ending_index - prefix_starting_index;
            
            if(prefix_starting_index >= line.length()){
                prefix_starting_index = line.length(); 
                break;
            }
        }
        
        prefix = line.substr(prefix_starting_index, prefix_len); 
        
        if(prefix_ending_index != string::npos){
            suffix_starting_index = prefix_ending_index + 1;
            suffix_ending_index = line.find(" ", suffix_starting_index);
            sufix_len = suffix_ending_index - suffix_starting_index;
            
            while(sufix_len == 0){
                suffix_starting_index++;
                suffix_ending_index = line.find(" ", suffix_starting_index);
                sufix_len = suffix_ending_index - suffix_starting_index;
                if(suffix_starting_index >= line.length()){
                    suffix_starting_index = line.length(); 
                    break;
                }
            }
            
            prefix = line.substr(prefix_starting_index, prefix_len);
            suffix = line.substr(suffix_starting_index, sufix_len); 
            
            prefix_starting_index = suffix_starting_index;    
        }
        else{
            suffix = "";  
            prefix_starting_index = string::npos;
        }
        
        add_markov_link(prefix, suffix);                                      
    }
}


int parse_text_file(string filename){
    string tweet;
    ifstream myfile((filename+".txt").c_str());
    while(getline (myfile,tweet)){
        parse_line(tweet);
    }
    myfile.close();
}

int main(int argc, char* argv[]){
	if(argc <= 1)
	{
        	cout << "Please provide a path to a text file containing tweets!" << endl;
        	return 0;
	}
	string user=argv[1];
	parse_text_file(user);
	return 0;
}
