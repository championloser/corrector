#include"../../projects/RssReader/tinyxml2.h"
#include<sys/types.h>
#include<dirent.h>
#include<iostream>
#include<string>
#include<vector>
#include<regex>
#include<fstream>
using namespace tinyxml2;
using std::cout;
using std::endl;
using std::string;
using std::vector;
struct RssItem
{
	string title;
	string link;
	string description;
	string content;
};
class RssReader
{
public:
	void findParseDumpRss(const char *);
	void parseRss(const char *);
	void dump(const char *);
private:
	vector<RssItem> _rss;
};
void RssReader::findParseDumpRss(const char *path)
{
	std::regex reg("[A-Za-z0-9]+\\.xml");
	DIR *dir=opendir(path);
	struct dirent *p;
	static int totalItem=0;
	while((p=readdir(dir))!=NULL)
	{
		if(regex_match(p->d_name, reg))
		{
			cout<<"parsing '"<<p->d_name<<"'..."<<endl;
			parseRss(p->d_name);
			cout<<"dumpping '"<<p->d_name<<"'...  "<<_rss.size()<<" items"<<endl;
			totalItem+=_rss.size();
			dump("pagelib.dat");
		}
	}
	cout<<"The total item is "<<totalItem<<endl;
}
void RssReader::parseRss(const char * filename)
{
	std::regex reg("<.+?>");
	XMLDocument doc;	
	doc.LoadFile(filename);
	XMLElement *rss=doc.FirstChildElement("rss");
	if(NULL==rss)return;
	XMLElement *channel=rss->FirstChildElement("channel");
	if(NULL==channel)return;
	XMLElement *item=channel->FirstChildElement("item");
	XMLElement *xtmp;
	RssItem tmp;
	while(item)
	{
		xtmp=item->FirstChildElement("title");
		if(NULL==xtmp)tmp.title="";
		else {
			const char *cstr=xtmp->GetText();
			if(NULL==cstr)tmp.title="";
			else tmp.title=cstr;
		}
		xtmp=item->FirstChildElement("link");
		if(NULL==xtmp)tmp.link="";
		else {
			const char *cstr=xtmp->GetText();
			if(NULL==cstr)tmp.link="";
			else tmp.link=cstr;
		}
		xtmp=item->FirstChildElement("description");
		if(NULL==xtmp)tmp.description="";
		else {
			const char *cstr=xtmp->GetText();
			if(NULL==cstr)tmp.description="";
			else tmp.description=cstr;
		}
		xtmp=item->FirstChildElement("content:encoded");
		if(NULL==xtmp)tmp.content=tmp.description;
		else {
			const char *cstr=xtmp->GetText();
			if(NULL==cstr)tmp.content=tmp.description;
			else tmp.content=std::regex_replace(cstr, reg, "");
		}
		_rss.push_back(tmp);
		item=item->NextSiblingElement("item");
	}
}
void RssReader::dump(const char *filename)
{
	std::ofstream ofs(filename, std::ios::app);
	static int num=1;
	for(vector<RssItem>::iterator it=_rss.begin(); it!=_rss.end(); ++it,++num)
	{
		ofs<<"<doc>\n"
		  <<"<docid>"<<num<<"</docid>\n"
		  <<"<title>"<<it->title<<"</title>\n"
		  <<"<link>"<<it->link<<"</link>\n"
		  <<"<description>"<<it->description<<"</description>\n"
		  <<"<content>"<<it->content<<"</content>\n"
		  <<"</doc>\n";
	}
	_rss.clear();
	ofs.close();
}
int main(int argc, char *argv[])
{
	if(argc!=2){cout<<"argc error!"<<endl;return 0;}
	RssReader rRead;
	rRead.findParseDumpRss(argv[1]);
	return 0;
}
