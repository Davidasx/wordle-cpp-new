#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<set>
#include<fstream>
#include<conio.h>
#include<windows.h>
#include<graphics.h>
using namespace std;
string ver="3.3.18";
string date="20240911";
string config="\\wordle.conf";
vector<string> keyboard={ "Q W E R T Y U I O P",
						 " A S D F G H J K L",
						 "  Z X C V B N M" };
string user_root;
string progname="wordle.exe",cppname="wordle.cpp";
bool havecpp;
vector<string> answers;
vector<string> words;
HWND window;
int cursorx,cursory;
void clear_screen(){
	solidrectangle(0,0,1000,500);
	cursorx=cursory=0;
	return;
}
void putstring(string s){
	for(int i=0;i<(int)(s.size());i++)
		outtextxy(cursorx,cursory,s[i]),cursorx+=10;
	return;
}
void putccc(char c){
	outtextxy(cursorx,cursory,c);
	cursorx+=10;
	return;
}
void putint(int x){
	string ss;
	bool neg=false;
	if(x<0) neg=true,x=-x;
	while(x){
		ss+=(char)(x%10+'0');
		x/=10;
	}
	reverse(ss.begin(),ss.end());
	if((int)(ss.size())==0) ss="0";
	if(neg) ss="-"+x;
	putstring(ss);
	return;
}
void button(){
	putstring("  ");
	line(10,cursory-2,10,cursory+18);
	line(10,cursory+18,290,cursory+18);
	line(290,cursory+18,290,cursory-2);
	line(290,cursory-2,10,cursory-2);
	return;
}
void button2(int row){
	line(198,row*23-7,198,row*23+16);
	line(198,row*23+16,302,row*23+16);
	line(302,row*23+16,302,row*23-7);
	line(302,row*23-7,198,row*23-7);
	return;
}
void changeline(){
	cursory+=20;
	cursorx=0;
	return;
}
string readstring(bool soft_keyboard=false){
	int basex=cursorx;
	string str;
	char c;
	ExMessage EM;
	int curpos=0;
	while(1){
		solidrectangle(basex,cursory,1000,cursory+20);
		cursorx=basex;
		putstring(str);
		line(basex+curpos*10,cursory,basex+curpos*10,cursory+20);
		while(1){
			EM=getmessage();
			if(soft_keyboard&&EM.lbutton){
				if(EM.y>=39&&EM.y<=108&&EM.x>=198&&EM.x<=302){
					if(EM.y>=85){
						if(curpos==0) continue;
						if(curpos!=(int)(str.size()))
							str=str.substr(0,curpos-1)+str.substr(curpos);
						else
							str=str.substr(0,curpos-1);
						curpos--;
						break;
					}
					else if(EM.y>=62) str="help!";
					solidrectangle(basex,cursory,1000,cursory+20);
					cursorx=basex;
					putstring(str);
					changeline();
					return str;
				}
				int R=(EM.y-40)/20;int C=EM.x/10;
				if(R>=0&&R<3&&C<(int)(keyboard[R].size())&&keyboard[R][C]!=' '){
					str=str.substr(0,curpos)+(char)(keyboard[R][C]-'A'+'a')+str.substr(curpos);
					curpos++;
					break;
				}
			}
			if(EM.message==WM_CHAR){
				c=EM.ch;
				if(c==' '){
					str=str.substr(0,curpos)+'_'+str.substr(curpos);
					curpos++;
				}
				else if(c=='\n'||c=='\r'){
					solidrectangle(basex,cursory,1000,cursory+20);
					cursorx=basex;
					putstring(str);
					changeline();
					return str;
				}
				else if(c=='\t'||c=='\b') continue;
				else if(c>=33&&c<=126){
					str=str.substr(0,curpos)+c+str.substr(curpos);
					curpos++;
				}
				break;
			}
			if(EM.message==WM_KEYDOWN){
				if(EM.vkcode==VK_LEFT) curpos=max(curpos-1,0);
				if(EM.vkcode==VK_RIGHT) curpos=min(curpos+1,(int)(str.size()));
				if(EM.vkcode==VK_BACK){
					if(curpos==0) continue;
					if(curpos!=(int)(str.size()))
						str=str.substr(0,curpos-1)+str.substr(curpos);
					else
						str=str.substr(0,curpos-1);
					curpos--;
				}
				break;
			}
		}
	}
}
void pause(){
	button();
	putstring("0)   continue");
	ExMessage EM;
	while(1){
		EM=getmessage();
		if(EM.message==WM_CHAR){
			if(EM.ch=='0') break;
		}
		if(EM.lbutton){
			if(EM.y>=cursory-2&&EM.y<=cursory+18&&EM.x>=10&&EM.x<=290)
				break;
		}
	}
	return;
}
string user;
#define back vals["COLOR_BACK"]
#define text vals["COLOR_TEXT"]
#define red vals["COLOR_RED"]
#define green vals["COLOR_GREEN"]
#define blue vals["COLOR_BLUE"]
#define yellow vals["COLOR_YELLOW"]
#define purple vals["COLOR_PURPLE"]
void scta(int clr){
	settextcolor(clr);
	return;
}
vector<pair<char,int> > hist[10010];
int cnt[26];
int col[10010];
int system(string s){
	return system(s.data());
}
int bad[26];
string det;
map<string,int> vals;
set<string> inter;
map<string,string> strv;
pair<int,int> rat;
void writeconf();
vector<int> levels;
vector<string> titles;
vector<int> colors;
void clr(int R){
	scta(text);
	for(int i=0;i<(int)(colors.size());i++){
		if(R>=levels[i]&&R<levels[i+1]){
			scta(colors[i]);
			break;
		}
	}
	return;
}
string level(int R){
	for(int i=0;i<(int)(titles.size());i++){
		if(R>=levels[i]&&R<levels[i+1]){
			return titles[i];
		}
	}
	return "ERROR";
}
string encode(int rating,int rounds){
	string ret="";
	rounds=rounds*2900+rating;
	rounds*=97;
	rounds+=rand()%96;
	rounds++;
	while(rounds){
		ret+=((rounds%26)+'A');
		rounds/=26;
	}
	while(ret.size()<=10) ret+="A";
	reverse(ret.begin(),ret.end());
	return ret;
}
pair<int,int> decode(string rating){
	int val=0;
	for(int i=0;i<(int)(rating.size());i++){
		val=(val*26+rating[i]-'A');
	}
	val/=97;
	return make_pair(val%2900,val/2900);
}
void printstate(int rounds,int wid,int rounds_disp){
	clear_screen();
	scta(text);
	putstring("Wordle > Guess Word");changeline();
	putstring("Try #");putint(rounds_disp);changeline();
	int CX=cursorx,CY=cursory;
	button2(2);button2(3);button2(4);
	cursorx=225;cursory=40;
	putstring("ENTER");
	cursorx=215;cursory=63;
	putstring("GIVE UP");changeline();
	cursorx=205;cursory=86;
	putstring("BACKSPACE");changeline();
	cursorx=CX;cursory=CY;
	for(int i=0;i<3;i++){
		for(int j=0;j<(int)(keyboard[i].size());j++){
			if(keyboard[i][j]!=' '){
				if(bad[keyboard[i][j]-'A']==0) scta(text);
				if(bad[keyboard[i][j]-'A']==1) scta(red);
				if(bad[keyboard[i][j]-'A']==2) scta(yellow);
				if(bad[keyboard[i][j]-'A']==3) scta(text);
			}
			putccc(keyboard[i][j]);
			scta(text);
		}
		changeline();
	}
	putint((int)(answers[wid].size()));
	putstring(" letters.");changeline();
	putstring(det);changeline();
	hist[rounds+1].clear();
	for(int i=1;i<=rounds;i++){
		for(int j=0;j<hist[i].size();j++){
			scta(hist[i][j].second);
			putccc(hist[i][j].first);
		}
		changeline();
	}
	scta(text);
	return;
}
void savescore(int rounds,int wid){
	string data;
	data+=answers[wid];
	string T=" ";
	for(int i=1;i<=rounds;i++){
		if(hist[i][0].second!=red&&hist[i][0].second!=yellow&&hist[i][0].second!=green)
			continue;
		for(int j=0;j<hist[i].size();j++){
			T[0]=hist[i][j].first;
			data+=T;
		}
		for(int j=0;j<hist[i].size();j++){
			if(hist[i][j].second==red) data+="r";
			if(hist[i][j].second==green) data+="g";
			if(hist[i][j].second==yellow) data+="y";
		}
	}
	string hist_name="HISTORY_";
	string tim,ntim;
	system("date /t > curtimewd.txt");
	ifstream dfin("curtimewd.txt");
	dfin>>tim;
	dfin.close();system("del curtimewd.txt");
	ntim=tim.substr(0,4)+tim.substr(5,2)+tim.substr(8,2);
	tim=ntim;
	hist_name+=tim;
	system("time /t > curtimewd.txt");
	ifstream tfin("curtimewd.txt");
	tfin>>tim;
	tfin.close();system("del curtimewd.txt");
	ntim=tim.substr(0,2)+tim.substr(3,2);
	tim=ntim;
	hist_name+=tim;
	int tint=time(NULL);
	tint%=60;
	T[0]='0'+tint/10;
	hist_name+=T;
	T[0]='0'+tint%10;
	hist_name+=T;
	strv[hist_name]=data;
	writeconf();
	return;
}
void win(int rounds,int wid){
	button();
	putstring("0)   continue");changeline();
	button();
	putstring("1)   save score");
	ExMessage EM;
	while(1){
		EM=getmessage();
		if(EM.message==WM_CHAR){
			if(EM.ch=='0') break;
			if(EM.ch=='1'){
				savescore(rounds,wid);
				break;
			}
		}
		if(EM.lbutton){
			if(EM.y>=cursory-22&&EM.y<=cursory-2&&EM.x>=10&&EM.x<=290)
				break;
			if(EM.y>=cursory-2&&EM.y<=cursory+18&&EM.x>=10&&EM.x<=290){
				savescore(rounds,wid);
				break;
			}
		}
	}
	return;
}
void wordle(){
	clear_screen();
	int id=rand()%(int)(answers.size());
	scta(text);
	int round=0;
	memset(bad,0,sizeof(bad));
	det="";
	for(int i=0;i<(int)(answers[id].size());i++) det+="_";
	int hists=0;
	while(1){
		hists++;
		round++;
		if(round==vals["TURNS"]+1){
			printstate(hists-1,id,round-1);
			scta(red);
			putstring("GAME OVER!");changeline();
			scta(green);
			putstring("ANSWER: ");putstring(answers[id]);
			changeline();
			scta(text);
			int score=0;
			for(int i=0;i<(int)(answers[id].size());i++)
				if(det[i]!='_') score+=100;
			for(int i=0;i<3;i++)
				for(int j=0;j<(int)(keyboard[i].size());j++)
					if(keyboard[i][j]!=' '){
						if(bad[keyboard[i][j]-'A']==2)  score+=50;
					}
			putstring("Score:");
			clr(score);
			putint(score);
			scta(text);
			changeline();
			rat.second=min(rat.second+1,1000);
			rat.first=(rat.first*(rat.second-1)+score)/rat.second;
			strv["RATING_"+strv["ACCOUNT"]]=encode(rat.first,rat.second);
			writeconf();
			pause();
			return;
		}
		printstate(hists-1,id,round);
		string ssss;
		while(1){
			ssss=readstring(true);
			if(ssss!="") break;
			cursory-=20;
		}
		if(ssss=="help!"){
			printstate(hists-1,id,round-1);
			scta(red);
			putstring("YOU GAVE UP!");changeline();
			scta(green);
			putstring("ANSWER: ");putstring(answers[id]);
			changeline();
			scta(text);
			pause();
			return;
		}
		bool flag=true;
		for(int i=0;i<(int)(ssss.size());i++){
			if(!isalpha(ssss[i])){
				flag=false;
				break;
			}
		}
		if(!flag){
			for(int i=0;i<(int)(ssss.size());i++)
				hist[hists].push_back(make_pair(ssss[i],blue));
			round--;
			continue;
		}
		for(int i=0;i<(int)(ssss.size());i++) if(ssss[i]<='Z')
			ssss[i]=ssss[i]-'A'+'a';
		if((int)(ssss.size())!=(int)(answers[id].size())){
			for(int i=0;i<(int)(ssss.size());i++)
				hist[hists].push_back(make_pair(ssss[i],purple));
			round--;
			continue;
		}
		flag=false;
		for(int i=0;i<words.size();i++) if(words[i]==ssss) flag=true;
		if(!flag){
			for(int i=0;i<(int)(ssss.size());i++)
				hist[hists].push_back(make_pair(ssss[i],blue));
			round--;
			continue;
		}
		memset(cnt,0,sizeof(cnt));
		for(int i=0;i<(int)(answers[id].size());i++)
			cnt[answers[id][i]-'a']++;
		int GR=0;
		for(int i=0;i<(int)(answers[id].size());i++){
			if(ssss[i]==answers[id][i]){
				cnt[ssss[i]-'a']--;
				col[i]=green;
				det[i]=ssss[i];
				bad[ssss[i]-'a']=3;
				ssss[i]=ssss[i]-'a'+'A';
				GR++;
			}
		}
		for(int i=0;i<(int)(answers[id].size());i++){
			if(ssss[i]>='a'&&cnt[ssss[i]-'a']){
				cnt[ssss[i]-'a']--;
				col[i]=yellow;
				if(!bad[ssss[i]-'a']) bad[ssss[i]-'a']=2;
				ssss[i]=ssss[i]-'a'+'A';
			}
		}
		for(int i=0;i<(int)(answers[id].size());i++){
			if(ssss[i]>='a'){
				col[i]=red;
				if(!bad[ssss[i]-'a']) bad[ssss[i]-'a']=1;
				ssss[i]=ssss[i]-'a'+'A';
			}
		}
		for(int i=0;i<(int)(answers[id].size());i++)
			hist[hists].push_back(make_pair(ssss[i]-'A'+'a',col[i]));
		if(GR==(int)(answers[id].size())){
			printstate(hists,id,round);
			putstring("Guessed correctly after ");
			putint(round);
			putstring(" tries.");changeline();
			putstring("Score:");
			clr(max(25-round,5)*100);
			putint(max(25-round,5)*100);
			changeline();
			scta(text);
			rat.second=min(rat.second+1,1000);
			rat.first=(rat.first*(rat.second-1)+max(25-round,5)*100)/rat.second;
			strv["RATING_"+strv["ACCOUNT"]]=encode(rat.first,rat.second);
			writeconf();
			scta(text);
			win(hists,id);
			return;
		}
	}
	return;
}
void about(){
	clear_screen();
	scta(text);
	putstring("Wordle > About");changeline();
	putstring("WORDLE (C++) Version ");
	putstring(ver);
	putstring(" (");
	putstring(date);
	putstring(") by David X");
	changeline();
	putstring("Copyright (C) DavidX 2022-2024. All rights reserved.");
	changeline();
	pause();
	return;
} 
void rules(){
	clear_screen();
	scta(text);
	putstring("Wordle > Help > Rules");changeline();
	putstring("You have ");
	putint(vals["TURNS"]);
	putstring(" chances to guess a secret word.");changeline();
	putstring("If you got a letter right,and in the same position,it will be shown like ");
	scta(green);putstring("this");scta(text);
	putstring(".");changeline();
	putstring("If you got a letter right,but in the wrong position,it will be shown like ");
	scta(yellow);putstring("this");scta(text);
	putstring(".");changeline();
	putstring("If you got a letter wrong,it will be shown like ");
	scta(red);putstring("this");scta(text);
	putstring(".");changeline();
	putstring("If the word isn't in the dictionary,it will be shown like ");
	scta(blue);putstring("this");scta(text);
	putstring(".");changeline();
	putstring("If the word is too long/short,it will be shown like ");
	scta(purple);putstring("this");scta(text);
	putstring(".");changeline();
	putstring("When you want to give up,type \"help!\" to see the answer.");
	changeline();
	pause();
	return;
}
void rating(){
	clear_screen();
	scta(text);
	putstring("Wordle > Help > Rating System");changeline();
	putstring("In every round of the game,you will be given a score.");changeline();
	putstring("It depends on how quickly you guessed the word correctly.");changeline();
	putstring("Your rating is the average score of all your games.");changeline();
	putstring("Ratings are shown in different colors:");changeline();
	for(int i=0;i<(int)(colors.size());i++){
		scta(colors[i]);
		if(i==0) putstring("0000");
		else putint(levels[i]);
		scta(text);
		putstring(" to ");
		scta(colors[i]);putint(levels[i+1]-1);scta(text);
		putstring(" : ");
		scta(colors[i]);putstring(titles[i]);scta(text);
		changeline();
	}
	pause();
	return;
}
void scores(){
	clear_screen();
	scta(text);
	putstring("Wordle > Help > History");changeline();
	putstring("The View History function allows you to view scores in previous games.");changeline();
	putstring("After every game, you have a choice of saving the game score.");changeline();
	putstring("By saving the score, you can view the game later on.");changeline();
	pause();
	return;
}
void accounts(){
	clear_screen();
	scta(text);
	putstring("Wordle > Help > accounts");changeline();
	putstring("Different Wordle accounts can be stored on the same computer.");changeline();
	putstring("You can change accounts and create accounts as you may.");changeline();
	putstring("Ratings of different accounts don't interfere with each other.");changeline();
	pause();
	return;
}
void help(){
	while(1){
		clear_screen();
		putstring("Wordle > Help");changeline();
		int BG=cursory;
		button();
		putstring("1)   rules");changeline();
		button();
		putstring("2)   rating system");changeline();
		button();
		putstring("3)   history");changeline();
		button();
		putstring("4)   accounts");changeline();
		button();
		putstring("0)   exit help");changeline();
		int ED=cursory;
		ExMessage EM;
		while(1){
			EM=getmessage();
			int t=0;
			if(EM.message==WM_CHAR){
				t=EM.ch-'0';
				if(t>=1&&t<=4);
				else if(t==0) t=5;
				else continue;
			}
			else if(EM.lbutton){
				if(EM.y>=BG-2&&EM.y<=ED-2&&EM.x>=10&&EM.x<=290){
					t=(EM.y-BG+22)/20;
				}
				else continue;
			}
			else continue;
			if(t==1) rules();
			if(t==2) rating();
			if(t==3) scores();
			if(t==4) accounts();
			if(t==5) return;
			break;
		}
	}
	return;
}
void readconf(){
	vals.clear();
	ifstream conf(config.data());
	string field,val_string;
	int val_int;
	while(conf>>field){
		if(field[0]=='['){
			conf>>val_int;
			vals[field.substr(1,field.size()-2)]=val_int;
		}
		else{
			conf>>val_string;
			strv[field.substr(1,field.size()-2)]=val_string;
		}
	}
	conf.close();
	return;
}
void writeconf(){
	ofstream conf(config.data());
	for(pair<string,int> conff:vals){
		conf<<"["<<conff.first<<"]"<<endl;
		conf<<conff.second<<endl;
	}
	for(pair<string,string> conff:strv){
		conf<<"<"<<conff.first<<">"<<endl;
		conf<<conff.second<<endl;
	}
	conf.close();
	return;
}
void readscore(string name,string save){
	scta(text);
	string TOC=name.substr(8,4);
	TOC+="/";TOC+=name.substr(12,2);
	TOC+="/";TOC+=name.substr(14,2);
	TOC+=" ";TOC+=name.substr(16,2);
	TOC+=":";TOC+=name.substr(18,2);
	putstring("Time of Completion: ");putstring(TOC);changeline();
	putstring(save.substr(0,5));save=save.substr(5);changeline();
	int sc=2500;
	while((int)(save.size())){
		sc-=100;
		for(int i=0;i<5;i++){
			if(save[i+5]=='r') scta(red);
			if(save[i+5]=='g') scta(green);
			if(save[i+5]=='y') scta(yellow);
			putccc(save[i]);
		}
		scta(text);
		changeline();
		save=save.substr(10);
	}
	putstring("Score: ");clr(sc);putint(sc);changeline();
	scta(text);
	return;
}
void history(){
	clear_screen();
	vector<pair<string,string> > available;
	for(pair<string,string> P:strv){
		if(P.first.substr(0,8)=="HISTORY_")
			available.push_back(P);
	}
	if((int)(available.size())==0){
		putstring("Wordle > View History");changeline();
		putstring("No saved scores. ");
		putstring("Select \"Save Score\" after finishing a game to save the score.");
		changeline();
		pause();
		return;
	}
	reverse(available.begin(),available.end());
	int id=0,tot=(int)(available.size());
	while(1){
		clear_screen();
		putstring("Wordle > View History");changeline();
		putstring("Currently viewing ");
		putint(id+1);
		putstring(" out of ");
		putint((int)(available.size()));
		changeline();
		readscore(available[id].first,available[id].second);
		int BG=cursory;
		button();
		putstring("1) next");changeline();
		button();
		putstring("2) last");changeline();
		button();
		putstring("3) delete entry");changeline();
		button();
		putstring("0) exit");changeline();
		int ED=cursory;
		ExMessage EM;
		while(1){
			EM=getmessage();
			int t=0;
			if(EM.message==WM_CHAR){
				t=EM.ch-'0';
				if(t>=1&&t<=3);
				else if(t==0) t=4;
				else continue;
			}
			else if(EM.lbutton){
				if(EM.y>=BG-2&&EM.y<=ED-2&&EM.x>=10&&EM.x<=290){
					t=(EM.y-BG+22)/20;
				}
				else continue;
			}
			else continue;
			if(t==1) id=(id+1)%((int)(available.size()));
			if(t==2) id=(id+tot-1)%((int)(available.size()));
			if(t==3){
				strv.erase(available[id].first);
				available.erase(available.begin()+id);
				id=id%((int)(available.size()));
				writeconf();
			}
			if(t==4) return;
			break;
		}
	}
	return;
}
map<string,string> desc;
map<string,vector<pair<string,int> > > item_i;
map<string,vector<pair<string,string> > > item_s;
void prep_settings(){
	desc["UPDATE_INTERVAL"]="Update interval";
	item_i["UPDATE_INTERVAL"].push_back({ "1 day",86400 });
	item_i["UPDATE_INTERVAL"].push_back({ "1 week",604800});
	item_i["UPDATE_INTERVAL"].push_back({ "10 days",864000});
	item_i["UPDATE_INTERVAL"].push_back({ "30 days",2592000});
	item_i["UPDATE_INTERVAL"].push_back({ "180 days",15552000});
	item_i["UPDATE_INTERVAL"].push_back({ "365 days",31536000});
	item_i["UPDATE_INTERVAL"].push_back({ "never",2e9});
	desc["DOWNLOAD_HEAD"]="Download mode";
	item_s["DOWNLOAD_HEAD"].push_back({ "Standard",""});
	item_s["DOWNLOAD_HEAD"].push_back({ "Mirror","https://gitdl.cn/"});
	return;
}
void editi(string name){
	clear_screen();
	putstring("Wordle > Settings > ");putstring(desc[name]);changeline();
	putstring("Current value:");
	for(pair<string,int> P:item_i[name])
		if(P.second==vals[name]){
			putstring(P.first);
			changeline();
			break;
		}
	int BG=cursory;
	int id=0;
	for(pair<string,int> P:item_i[name]){
		button();
		putint(++id);
		putstring(")   ");
		putstring(P.first);
		changeline();
	}
	button();
	putstring("0)   go back");changeline();
	int ED=cursory;
	ExMessage EM;
	while(1){
		EM=getmessage();
		int t=0;
		if(EM.message==WM_CHAR){
			t=EM.ch-'0';
			if(t>=1&&t<=(int)(item_i[name].size()));
			else if(t==0) t=(int)(item_i[name].size())+1;
			else continue;
		}
		else if(EM.lbutton){
			if(EM.y>=BG-2&&EM.y<=ED-2&&EM.x>=10&&EM.x<=290){
				t=(EM.y-BG+22)/20;
			}
			else continue;
		}
		else continue;
		if(t<=(int)(item_i[name].size())){
			vals[name]=item_i[name][t-1].second;
			break;
		}
		break;
	}
	return;
}
void edits(string name){
	clear_screen();
	putstring("Settings > ");putstring(desc[name]);changeline();
	putstring("Current value:");
	for(pair<string,string> P:item_s[name])
		if(P.second==strv[name]){
			putstring(P.first);
			changeline();
			break;
		}
	int BG=cursory;
	int id=0;
	for(pair<string,string> P:item_s[name]){
		button();
		putint(++id);
		putstring(")   ");
		putstring(P.first);
		changeline();
	}
	button();
	putstring("0)   go back");changeline();
	int ED=cursory;
	ExMessage EM;
	while(1){
		EM=getmessage();
		int t=0;
		if(EM.message==WM_CHAR){
			t=EM.ch-'0';
			if(t>=1&&t<=(int)(item_s[name].size()));
			else if(t==0) t=(int)(item_s[name].size())+1;
			else continue;
		}
		else if(EM.lbutton){
			if(EM.y>=BG-2&&EM.y<=ED-2&&EM.x>=10&&EM.x<=290){
				t=(EM.y-BG+22)/20;
			}
			else continue;
		}
		else continue;
		if(t<=(int)(item_s[name].size())){
			strv[name]=item_s[name][t-1].second;
			break;
		}
		break;
	}
	return;
}
void settings(){
	while(1){
		clear_screen();
		putstring("Wordle > Settings");changeline();
		int cnt1=0,cnt2=0;
		int BG=cursory;
		vector<string> vi,vs;
		for(pair<string,int> conff:vals) if(item_i.count(conff.first)){
			button();
			cnt1++;
			vi.push_back(conff.first);
			putstring(desc[conff.first]);putccc(':');
			for(pair<string,int> P:item_i[conff.first])
				if(P.second==conff.second){
					putstring(P.first);
					changeline();
					break;
				}
		}
		for(pair<string,string> conff:strv) if(item_s.count(conff.first)){
			button();
			cnt2++;
			vs.push_back(conff.first);
			putstring(desc[conff.first]);putccc(':');
			for(pair<string,string> P:item_s[conff.first])
				if(P.second==conff.second){
					putstring(P.first);
					changeline();
					break;
				}
		}
		button();
		putstring("save and exit settings");changeline();
		int ED=cursory;
		ExMessage EM;
		while(1){
			EM=getmessage();
			int t=0;
			if(EM.lbutton){
				if(EM.y>=BG-2&&EM.y<=ED-2&&EM.x>=10&&EM.x<=290){
					t=(EM.y-BG+22)/20;
				}
				else continue;
			}
			else continue;
			if(t==cnt1+cnt2+1){
				writeconf();
				return;
			}
			else if(t<=cnt1){
				editi(vi[t-1]);
			}
			else edits(vs[t-cnt1-1]);
			break;
		}
	}
	return;
}
int download_file(string pth,string out){
	string url=strv["DOWNLOAD_HEAD"]+"https://raw.githubusercontent.com/Davidasx/wordle-cpp/main/"+pth;
	return system("curl \""+url+"\" --silent -o "+out);
}
int download_release(string rls,string fil,string out){
	putstring("Downloading ");
	putstring(rls);
	putstring("\\");
	putstring(fil);
	changeline();
	string url=strv["DOWNLOAD_HEAD"]+"https://github.com/Davidasx/wordle-cpp/releases/download/"+rls+"/"+fil;
	return system("curl \""+url+"\" --silent -o "+out);
}
bool later(string a,string b){
	int cur=0;
	vector<int> va,vb;
	cur=0;
	for(int i=0;i<(int)(a.size());i++){
		if(a[i]=='.') va.push_back(cur),cur=0;
		else cur=(cur*10+a[i]-'0');
	}
	va.push_back(cur);
	cur=0;
	for(int i=0;i<(int)(b.size());i++){
		if(b[i]=='.') vb.push_back(cur),cur=0;
		else cur=(cur*10+b[i]-'0');
	}
	vb.push_back(cur);
	while((int)(va.size())<(int)(vb.size())) va.push_back(0);
	while((int)(vb.size())<(int)(va.size())) vb.push_back(0);
	for(int i=0;i<(int)(va.size());i++){
		if(va[i]>vb[i]) return true;
		if(vb[i]>va[i]) return false;
	}
	return true;
}
void update(){
	clear_screen();
	putstring("Wordle > Update");changeline();
	if(system("where curl /Q")){
		putstring("CURL wasn't found on this computer.");
		changeline();
		putstring("Please install CURL to use the update function.");
		changeline();
		pause();
		return;
	}
	putstring("Current Version:");putstring(ver);
	changeline();
	int ret=download_file("latest.txt","wdlatest.txt");
	if(ret){
		putstring("Failed to check latest version.");
		changeline();
		pause();
		return;
	}
	int ret2=download_file("assets.txt","wdassets.txt");
	if(ret2){
		putstring("Failed to check latest version.");
		changeline();
		pause();
		return;
	}
	string lver;
	ifstream vread("wdlatest.txt");
	vread>>lver;
	vread.close();
	system("del wdlatest.txt");
	map<string,string> assets;
	ifstream aread("wdassets.txt");
	string file_name,update_ver;
	while(aread>>file_name>>update_ver){
		assets[file_name]=update_ver;
	}
	aread.close();
	system("del wdassets.txt");
	if(later(ver,lver)){
		putstring("Current version is latest.");changeline();
		pause();
		return;
	}
	else{
		putstring("Latest version:");putstring(lver);
		changeline();
		int BG=cursory;
		button();
		putstring("1)   download");changeline();
		button();
		putstring("0)   exit");changeline();
		int ED=cursory;
		ExMessage EM;
		while(1){
			EM=getmessage();
			int t=0;
			if(EM.message==WM_CHAR){
				t=EM.ch-'0';
				if(t>=1&&t<=1);
				else if(t==0) t=2;
				else continue;
			}
			else if(EM.lbutton){
				if(EM.y>=BG-2&&EM.y<=ED-2&&EM.x>=10&&EM.x<=290){
					t=(EM.y-BG+22)/20;
				}
				else continue;
			}
			else continue;
			if(t==1){
				putstring("Downloading...");changeline();
				if(havecpp){
					ret=download_release(lver,"wordle.cpp","wordle_new_update.cpp");
					if(ret){
						putstring("Failed to download.");
						changeline();
						pause();
						return;
					}
				}
				ret=download_release(lver,"wordle.exe","wordle_new_update.exe");
				if(ret){
					putstring("Failed to download.");
					changeline();
					if(havecpp)
						system("del wordle_new_update.cpp");
					pause();
					return;
				}
				for(pair<string,string> P:assets){
					if(later(P.second,ver)){
						ret=download_release(P.second,P.first,P.first+"_temp");
						if(ret){
							putstring("Failed to download.");
							changeline();
							pause();
							return;
						}
					}
				}
				writeconf();
				pause();
				ofstream fout("wdupdater.bat");
				fout<<"@echo off"<<endl;
				fout<<"pause"<<endl;
				fout<<"copy wordle_new_update.exe "<<progname<<endl;
				if(havecpp)
					fout<<"copy wordle_new_update.cpp "<<cppname<<endl;
				//fout<<"del wordle_new_update.exe"<<endl;
				if(havecpp)
					fout<<"del wordle_new_update.cpp"<<endl;
				for(pair<string,string> P:assets){
					if(later(P.second,ver)){
						fout<<"copy "<<P.first<<"_temp "<<P.first<<endl;
						fout<<"del "<<P.first<<"_temp"<<endl;
					}
				}
				fout<<"cls"<<endl;
				fout<<"start "<<progname<<endl;
				fout<<"exit"<<endl;
				fout.close();
				system("start wdupdater.bat");
				exit(0);
			}
			if(t==2) return;
			break;
		}
	}
	return;
}
void account(){
	clear_screen();
	putstring("Wordle > Change Account");changeline();
	putstring("Current account:");
	putstring(strv["ACCOUNT"]);
	changeline();
	putstring("New account(leave blank to keep old account):");
	string tmp;
	tmp=readstring();
	if(tmp!=""){
		strv["ACCOUNT"]=tmp;
		if(!strv["RATING_"+strv["ACCOUNT"]].size())
			strv["RATING_"+strv["ACCOUNT"]]=encode(0,0);
		putstring("Successfully changed account.");changeline();
		writeconf();
		pause();
	}
	return;
}
void username(){
	clear_screen();
	putstring("Wordle > Change Username");changeline();
	putstring("Current username:");
	putstring(strv["ACCOUNT"]);
	changeline();
	putstring("New username(leave blank to keep old username):");
	string tmp;
	tmp=readstring();
	if(tmp!=""){
		strv["RATING_"+tmp]=strv["RATING_"+strv["ACCOUNT"]];
		strv.erase("RATING_"+strv["ACCOUNT"]);
		strv["ACCOUNT"]=tmp;
		putstring("Successfully changed username.");changeline();
		writeconf();
		pause();
	}
	return;
}
int main(int argc,char** argv){
	string WW;
	ifstream reader("dictionary.txt");
	while(reader>>WW) words.push_back(WW);
	ifstream reader2("answers.txt");
	while(reader2>>WW) answers.push_back(WW);
	reader.close();
	ifstream test_update("wdupdater.bat");
	if(!test_update) test_update.close();
	else{
		test_update.close();
		system("del wdupdater.bat");
	}
	ifstream cpptest("wordle.cpp");
	if(!cpptest) havecpp=false;
	else havecpp=true;
	cpptest.close();
	for(int i=0;i<(int)(answers.size());i++) for(int j=0;j<answers[i].size();j++)
		answers[i][j]-='A',answers[i][j]+='a';
	for(int i=0;i<(int)(words.size());i++) for(int j=0;j<words[i].size();j++)
		words[i][j]-='A',words[i][j]+='a';
	system("date /t > curtimewd.txt");
	ifstream tfin("curtimewd.txt");
	string tim;tfin>>tim;
	tfin.close();system("del curtimewd.txt");
	string ntim=tim.substr(0,4)+tim.substr(5,2)+tim.substr(8,2);
	tim=ntim;
	system("echo %USERPROFILE% > userrootwd.txt");
	ifstream ufin("userrootwd.txt");
	ufin>>user_root;
	ufin.close();system("del userrootwd.txt");
	config=user_root+config;
	ifstream tmpconf(config.data());
	if(!tmpconf){
		tmpconf.close();
		ofstream confgen(config.data());
		confgen<<endl;
		confgen.close();
	}
	else tmpconf.close();
	readconf();
	if(strv["DOWNLOAD_HEAD"]=="") strv["DOWNLOAD_HEAD"]="";
	if(!vals["LAST_UPDATE"]) vals["LAST_UPDATE"]=time(0);
	if(!vals["UPDATE_INTERVAL"]) vals["UPDATE_INTERVAL"]=604800;
	if(!vals["TURNS"]) vals["TURNS"]=20;
	if(!vals["COLOR_BACK"]) vals["COLOR_BACK"]=657930;
	if(!vals["COLOR_TEXT"]) vals["COLOR_TEXT"]=16448250;
	if(!vals["COLOR_RED"]) vals["COLOR_RED"]=658170;
	if(!vals["COLOR_GREEN"]) vals["COLOR_GREEN"]=719370;
	if(!vals["COLOR_BLUE"]) vals["COLOR_BLUE"]=16386570;
	if(!vals["COLOR_YELLOW"]) vals["COLOR_YELLOW"]=719610;
	if(!vals["COLOR_PURPLE"]) vals["COLOR_PURPLE"]=16386810;
	window=initgraph(1000,500);
	setfillcolor(back);
	setlinecolor(text);
	settextcolor(text);
	setbkcolor(back);
	solidrectangle(0,0,1000,290);
	settextstyle(18,0,LPCTSTR("CONSOLAS"));
	scta(text);
	if(!strv["ACCOUNT"].size()){
		strv["VERSION"]=ver;
		vals["LAST_UPDATE"]=time(0);
		putstring("Welcome to WORDLE ");
		putstring(ver);
		putstring("!");
		changeline();
		putstring("It seems that it's your first time using new Wordle.");
		changeline();
		putstring("Please create a username for yourself: ");
		string tmp;
		tmp=readstring();
		if(tmp==""){
			tmp="User";
			putstring("No username set. Default username will be used.");
			changeline();
		}
		if(tmp!=""){
			putstring("Set username to ");
			putstring(tmp);
			putstring(".");
			changeline();
			strv["ACCOUNT"]=tmp;
			strv["RATING_"+strv["ACCOUNT"]]=encode(0,0);
			writeconf();
		}
		putstring("Let's start with default settings. You can change them later.");
		changeline();
		putstring("You may refer to Help for details about this program.");
		changeline();
		putstring("Press the button or press 0 on the keyboard to start.");
		changeline();
		pause();
	}
	if(!strv["RATING_"+strv["ACCOUNT"]].size()) strv["RATING_"+strv["ACCOUNT"]]=encode(0,0);
	if(!strv["VERSION"].size()) strv["VERSION"]=ver;
	writeconf();
	if(ver!=strv["VERSION"]){
		strv["VERSION"]=ver;
		vals["LAST_UPDATE"]=time(0);
		putstring("Welcome to WORDLE ");
		putstring(ver);
		putstring("!");
		changeline();
		writeconf();
		pause();
	}
	if(time(0)-vals["LAST_UPDATE"]>vals["UPDATE_INTERVAL"])
		update();
	srand(unsigned(time(NULL)));
	prep_settings();
	levels={0,1500,1800,2000,2100,2401};
	titles={"Beginner","Learner","Expert","Master","Genius"};
	colors={text,green,blue,yellow,red};
	while(1){
		clear_screen();
		putstring("Wordle");changeline();
		rat=decode(strv["RATING_"+strv["ACCOUNT"]]);
		putstring("Hello,");
		clr(rat.first);
		putstring(strv["ACCOUNT"]);
		scta(text);
		putstring("!");
		changeline();
		putstring("WORDLE (C++) Version ");
		putstring(ver);
		putstring(" (");
		putstring(date);
		putstring(") by David X");
		changeline();
		putstring("Current Rating:");
		clr(rat.first);
		if(rat.first<1000) putint(0);
		if(rat.first<100) putint(0);
		if(rat.first<10) putint(0);
		putint(rat.first);
		changeline();
		scta(text);
		putstring("Current Level:");
		clr(rat.first);
		putstring(level(rat.first));
		changeline();
		scta(text);
		int BG=cursory;
		button();
		putstring("1)   guess word");changeline();
		button();
		putstring("2)   help");changeline();
		button();
		putstring("3)   settings");changeline();
		button();
		putstring("4)   update");changeline();
		button();
		putstring("5)   change account");changeline();
		button();
		putstring("6)   change username");changeline();
		button();
		putstring("7)   view history");changeline();
		button();
		putstring("9)   about");changeline();
		button();
		putstring("0)   exit");changeline();
		int ED=cursory;
		ExMessage EM;
		while(1){
			EM=getmessage();
			int t=0;
			if(EM.message==WM_CHAR){
				t=EM.ch-'0';
				if(t>=1&&t<=7);
				else if(t==9) t=8;
				else if(t==0) t=9;
				else continue;
			}
			else if(EM.lbutton){
				if(EM.y>=BG-2&&EM.y<=ED-2&&EM.x>=10&&EM.x<=290){
					t=(EM.y-BG+22)/20;
				}
				else continue;
			}
			else continue;
			if(t==1) wordle();
			if(t==2) help();
			if(t==3) settings();
			if(t==4) update();
			if(t==5) account();
			if(t==6) username();
			if(t==7) history();
			if(t==8) about();
			if(t==9) return 0;
			break;
		}
	}
	return 0;
}
