#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;
string readFile(string filename){
    fstream fileStream(filename);
    string text="";
    if (fileStream.is_open()){
        string line;
        while(getline(fileStream,line)){
            text+=line;
        }
    }
    return text;
}
vector<int> getLPS(const string& text){
    int len = 0;  // Length of the current longest prefix suffix
    vector<int> lps(text.size(), 0);

    int i = 1;
    while (i < text.size()) {
        if (text[i] == text[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) {
                len = lps[len - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }

    return lps;
}
int findString(string text, string find){
    vector<int> lps = getLPS(find);
    int i = 0;
    int j = 0;
    while (i < text.size()) {
        if (find[j] == text[i]) {
            i++;
            j++;
        }

        if (j == find.size()) {
            // Pattern found at index i - j in the text
            return i-find.size();
            j = lps[j - 1];
        } else if (i < text.size() && find[j] != text[i]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }
    return -1;

}

string transString(const string& text1){

    string result = "#";
    
    //transform the string for it to include # at every position including beggining and end
    for (size_t i = 0; i < text1.size()+1; ++i) {
        result += text1[i]; 
        
        if (i < text1.size() - 1) {
            result += "#";
        }
    }
    
    return result;
}

string Manacher(const string& text1){
    
    string transformed= transString(text1);
    int n = text1.size();
    vector<int> pal(n, 0);
    int center=0, right=0;

    for (int i = 1; i < n - 1; i++) {
        int mirror = 2 * center - i;// calculate the mirror of i with center

        if (i < right) {
            pal[i] = min(right - i, pal[mirror]);
        }

        while (transformed[i + pal[i] + 1] == transformed[i - pal[i] - 1]) {
            pal[i]++;
        }

        if (i + pal[i] > right) {
            center = i;
            right = i + pal[i];
        }
    }
    int maxLen = 0;
    int centerIndex = 0;

    for (int i = 1; i < n - 1; i++) {
        if (pal[i] > maxLen) {
            maxLen = pal[i];
            centerIndex = i;
        }
    }

    int start = (centerIndex - maxLen) / 2;
    return text1.substr(start, maxLen);
}

pair<int, int> findLongestCommonSubstring(const string& text1, const string& text2) {
    int m = text1.size();//length of text1
    int n = text2.size();//length of text2

    //Longest common sufix for all pair of prefixes of the string (m x n)
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
    
    int maxLen = 0;
    int endIndex = 0;

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (text1[i - 1] == text2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
                if (dp[i][j] > maxLen) {
                    maxLen = dp[i][j];
                    endIndex = i; // Remember the ending position
                }
            }
        }
    }

    int startIndex = endIndex - maxLen + 1;
    return make_pair(startIndex, endIndex);
}


int main(){
    string transmission1=readFile("transmission1.txt");
    string transmission2=readFile("transmission2.txt");
    string m1code=readFile("mcode1.txt");
    string m2code=readFile("mcode2.txt");
    string m3code=readFile("mcode3.txt");
    int m;
    string pal;
    pair<int, int> c;
    // transmission1.txt
    m=findString(transmission1,m1code);
    cout<<((m>=0) ? "True "+to_string(m+1): "False ")<<endl;
    m=findString(transmission1,m2code);
    cout<<((m>=0) ? "True "+to_string(m+1): "False ")<<endl;
    m=findString(transmission1,m3code);
    cout<<((m>=0) ? "True "+to_string(m+1): "False ")<<endl;

    // transmission2.txt
    m=findString(transmission2,m1code);
    cout<<((m>=0) ? "True "+to_string(m+1): "False ")<<endl;
    m=findString(transmission2,m2code);
    cout<<((m>=0) ? "True "+to_string(m+1): "False ")<<endl;
    m=findString(transmission2,m3code);
    cout<<((m>=0) ? "True "+to_string(m+1): "False ")<<endl;

    //transmission1.txt
    pal= Manacher(transmission1);
    cout << pal << " transmission1"  << endl;

    //transmission2.txt
    pal= Manacher(transmission2);
    cout << pal << " transmission2"  << endl;

    
    c = findLongestCommonSubstring(transmission1, transmission2);
    cout<<c.first<<" "<<c.second<<endl;
    c = findLongestCommonSubstring(transmission2,transmission1);
    cout<<c.first<<" "<<c.second<<endl;
    
    return 0;
}
