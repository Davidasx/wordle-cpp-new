#include<bits/stdc++.h>
using namespace std;
unsigned int system(string s){
	return system(s.data());
}
int main(int argc,char** argv){
	ifstream fin("..\\wordle\\wordle.cpp");
	string ver,tmp;
	while(getline(fin,tmp)){
		if(tmp.substr(0,12)=="string ver=\"") break;
	}
	fin.close();
	for(int i=12;i<(int)(tmp.size());i++){
		if(tmp[i]=='\"') break;
		ver+=tmp[i];
	}
	system("taskkill /f /im wordle.exe");
	system("cls");
	system("msbuild ..\\wordle\\wordle.vcxproj -p:Configuration=Release");
	system("copy ..\\wordle\\x64\\Release\\wordle.exe ..\\wordle\\wordle.exe /Y");
	ifstream ain("assets.txt");
	string file,uver;
	map<string,string> assets;
	while(ain>>file>>uver){
		if(system("fc ..\\wordle\\"+file+" "+file)){
			uver=ver;
			system("copy ..\\wordle\\"+file+" "+file);
		}
		assets[file]=uver;
	}
	ain.close();
	ofstream aout("assets.txt");
	for(pair<string,string> P:assets)
		aout<<P.first<<" "<<P.second<<endl;
	aout.close();
	system("echo "+ver+" > latest.txt");
	system("copy ..\\wordle\\wordle.cpp wordle.cpp");
	system("copy ..\\wordle\\wordle.exe wordle.exe");
	system("commiter --no-pause");
	system("gh release create "+ver+" --generate-notes");
	system("gh release upload "+ver+" .\\wordle.cpp");
	system("gh release upload "+ver+" .\\wordle.exe");
	for(pair<string,string> P:assets)
		system("gh release upload "+ver+" .\\"+P.first);
	system("pause");
	return 0;
}

