#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <time.h>
#include <unistd.h>
using namespace std;

void parse_line(string line){
    cout<<"Got to parse_line"<<endl;
    long word_start_index = 0;
    long word_end_index;
    
    long follower_start_index;
    long follower_end_index;
    
    long word_len;
    long follower_len;
    
    string word = "";
    string follower = "";
    
    // Goes through all the words in the tweet
    while(word_start_index != string::npos){
        word_end_index = line.find(" ", word_start_index);
        word_len = word_end_index - word_start_index;
        
        // Find the next base word
        while(word_len == 0){
            word_start_index++;
            word_end_index = line.find(" ", word_start_index);
            word_len = word_end_index - word_start_index;
            
            // Case where the last word in the tweet has been reached
            if(word_start_index >= line.length()){
                word_start_index = line.length(); // Should always be the length anyway due to logic of the find function, but set in case of boundary conditions 
                break;
            }
        }
        
        word = line.substr(word_start_index, word_len); // Now holds a single word from the tweet.
        
        // Now find the following word (if there is one)
        if(word_end_index != string::npos){
            follower_start_index = word_end_index + 1;
            follower_end_index = line.find(" ", follower_start_index);
            follower_len = follower_end_index - follower_start_index;
            
            while(follower_len == 0){
                follower_start_index++;
                follower_end_index = line.find(" ", follower_start_index);
                follower_len = follower_end_index - follower_start_index;
                if(follower_start_index >= line.length()){
                    follower_start_index = line.length(); // should always be the length anyway due to logic of the find function, but set in case of boundary conditions 
                    break;
                }
            }
            
            word = line.substr(word_start_index, word_len);
            follower = line.substr(follower_start_index, follower_len); // Now contains the word right after the base word
            
            word_start_index = follower_start_index;    // The next base word will be the current base word's follower
        }
        else{
            follower = "";  // If the base word is that last word in the tweet, then it does not have a following word
            word_start_index = string::npos;
        }
        
        //add_pair(word, follower);   // Add the base word and its follower to the vector of Markov links
	cout<<word<<" "<<follower<<endl;
	                                    // or update the base word's vector of followers if there is already a record
    }
    cout<<"End of tweet"<<endl;
}


int parse_text_file(string filename){
    string line;
    ifstream myfile((filename+".txt").c_str());
    while(getline (myfile,line)){
        parse_line(line);
    }
    myfile.close();
}

int main(int argc, char* argv[]){
	if(argc <= 1)
	{
        	cout << "Please provide path/to/file.txt or @Twitter_Username" << endl;
        	return 0;
	}
	string user=argv[1];
	parse_text_file(user);
	return 0;
}
