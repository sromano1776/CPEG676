class Pair{ //defines a class of a "Pair" of words being the first word and whatever word follows it
    public:
        string word;
        vector<string> followers;

        Pair(string wrd, string suffix){//constructor for the Pair of words. Takes the word, 
										//and adds the follower onto an array
            word = wrd;
            followers.push_back(suffix);
        }
};


vector<Pair> markov_chain;
string tweet = "";


int find_markov_link_index(string word){//finds the index of the markov link if it exists already
										//if not it returns -1 and the markov link is added
    for(int i = 0; i < markov_chain.size(); i++){
        if(markov_chain.at(i).word.compare(word) == 0){
            return i;
        }
    }
    return -1;
}


void add_pair(string word, string suffix){//physically adds the markov link into the chain
    int index = find_markov_link_index(word);

    if(index == -1){
        markov_chain.push_back(Pair(word, suffix));
    }
    else{
        markov_chain.at(index).followers.push_back(suffix);
    }
}


void parse_line(string line){//breaks the current tweet being fed in into a word and whatever word follows it
    long prefix_start_index = 0;
    long prefix_end_index;

    long suffix_start_index;
    long suffix_end_index;

    long prefix_length;
    long follower_length;

    string word = "";
    string suffix = "";


    while(prefix_start_index != string::npos){
        prefix_end_index = line.find(" ", prefix_start_index);
        prefix_length = prefix_end_index - prefix_start_index;
        while(prefix_length == 0){
            prefix_start_index++;
            prefix_end_index = line.find(" ", prefix_start_index);
            prefix_length = prefix_end_index - prefix_start_index;


            if(prefix_start_index >= line.length()){
                prefix_start_index = line.length();
                break;
            }
        }
        word = line.substr(prefix_start_index, prefix_length);
        if(prefix_end_index != string::npos){
            suffix_start_index = prefix_end_index + 1;
            suffix_end_index = line.find(" ", suffix_start_index);
            follower_length = suffix_end_index - suffix_start_index;

            while(follower_length == 0){
                suffix_start_index++;
                suffix_end_index = line.find(" ", suffix_start_index);
                follower_length = suffix_end_index - suffix_start_index;
                if(suffix_start_index >= line.length()){
                    suffix_start_index = line.length();
                    break;
                }
            }
            word = line.substr(prefix_start_index, prefix_length);
            suffix = line.substr(suffix_start_index, follower_length);

            prefix_start_index = suffix_start_index;
        }
        else{
            suffix = "";
            prefix_start_index = string::npos;
        }
        add_pair(word, suffix); // adds the pair of words into the markov chain
    }
}

int parse_text_file(string filename){//reads in the text file with given tweets line by line to add them to the markov chain
    string line;
    ifstream myfile(filename.c_str());
    if (myfile.is_open()){
        while(getline (myfile,line)){
            parse_line(line);
        }
        myfile.close();
        return 1;
    }
    else{
        return 0;
    }
}

int get_rand_index(int choices){//randomly chooses starting word for generated tweet
    return rand() % choices;
}

void generate_tweet(){//function to generate the tweet
    int prefix_index = get_rand_index(markov_chain.size());
    int suffix_index = 0;
    int suffix_number = 0;
    bool tweet_done = false;
    string last_word = "";
    tweet = "";
    tweet = markov_chain.at(prefix_index).word;
    last_word = tweet; //the tweet now contains a singe word
    while(!tweet_done){
        prefix_index = find_markov_link_index(last_word);
        Pair dummy = markov_chain.at(prefix_index);
        suffix_number = dummy.followers.size();
        suffix_index = get_rand_index(suffix_number);
        last_word = dummy.followers.at(suffix_index);

        if(last_word.length() == 0){
            tweet_done = true;
        }
        else{
            if(tweet.length() + last_word.length() > 279){
                tweet_done = true;
            }
            else{
                tweet = tweet + " " + last_word;
            }
        }
    }
    cout << tweet << endl;
}

int main(int argc, char* argv[]){
    if(argc <= 1){
        cout << "Please provide a path to text file (/path/to/file) or @Twitter_Handle" << endl;
        return 0;
    }

    string user_input(argv[1]);
    srand(time(NULL));

    if(user_input.at(0) == '@'){
        cout << "Remain calm! Accessing Twitter now using handle"<< user_input<< endl;
        string arg = "python get_tweets.py " + user_input;
        cout<<arg<<endl;
        int err = system(arg.c_str());

        if(err == -1){
            cout << "Could not run script to get tweets. Is python installed?" << endl;
        }

        user_input = user_input.substr(1, user_input.length()) + ".txt";
    }

    cout << "Reading text file " << user_input << "..." << endl;

    if(parse_text_file(user_input)){

        cout << "Parsing complete...."<<endl;
        cout << "Generating a tweet using " <<user_input<<endl<< endl;
        generate_tweet();
    }
    else{
        cout << "File "<< user_input << " was not found" << endl;
    }

    return 0;
}
