class tools{
public:
    static int letterToIndex(char c){
        return (int)tolower(c) - 97;
    }
    
    static std::string replaceAll(std::string str, const std::string& from, const std::string& to) {
        size_t start_pos = 0;
        while((start_pos = str.find(from, start_pos)) != std::string::npos) {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
        }
        return str;
    }
    
    static string camelCase(string line)  {
        bool active = true;
        string output;
        
        for(int i = 0; i < line.size(); i++) {
            if(i == 0) {
                output.push_back(toupper(line[i]));
            }
            else output.push_back(tolower(line[i]));
        }
        
        return output;
    }
    
    static string upperCase(string line){
        string output;
        
        for(int i = 0; i < line.size(); i++){
            output.push_back(toupper(line[i]));
        }
        
        return output;
    }
    
    static string lowerCase(string line){
        string output;
        
        for(int i = 0; i < line.size(); i++){
            output.push_back(tolower(line[i]));
        }
        
        return output;
    }

    static vector<string> splitWords(string line){
        vector<string> words;
          string str;
          
          for(auto x:line)
          {
              if(x==' ')
              {
                  words.push_back(str);
                  str.clear();
              }
              else if(tolower(x) > 96 && tolower(x) < 123) str=str+x;
          }
        words.push_back(str);
        return words;
    }
    
};
