#include<iostream>
#include<vector>
#include<map>
#include<set>
#include<string>
#include<memory>
#include<fstream>
#include<regex>
#include<algorithm>
using std::cout;
using std::endl;
using std::cin;
using std::string;
using std::map;
using std::vector;
using std::shared_ptr;
using std::set;
using std::ifstream;
class QueryResult;
class TextQuery
{
public:
	TextQuery(const string &filename);
	QueryResult query(const string &s) const;
	void displayMap();	//for debug
	void displayFile();
private:
	shared_ptr<vector<string>> _wm;
	shared_ptr<map<string, shared_ptr<set<int>>>> _lines;
	
};
TextQuery::TextQuery(const string &filename)
: _wm(new vector<string>)
, _lines(new map<string, shared_ptr<set<int>>>)
{
	ifstream ifs(filename);
	string line;
	std::regex reg("[A-Za-z]+");
	int lineNum=0;
	while(getline(ifs, line))
	{
		_wm->push_back(line);	
		auto word_begin=std::sregex_iterator(line.begin(), line.end(), reg);
		auto word_end=std::sregex_iterator();
		for(auto it=word_begin; it!=word_end; ++it)
		{
			//std::smatch mat=*it;
			//cout<<mat.str()<<endl;
			shared_ptr<set<int>> pset(new set<int>);
			pset->insert(lineNum);
			auto ret=_lines->insert(make_pair((*it).str(), pset));
			if(!ret.second)ret.first->second->insert(lineNum);
		}
		++lineNum;
	}
}
void TextQuery::displayMap()
{
	map<string, shared_ptr<set<int>>>::iterator it;
	for(it=_lines->begin(); it!=_lines->end(); ++it)
	{
		cout<<it->first<<" ";
		for(set<int>::iterator sit=it->second->begin(); sit!=it->second->end(); ++sit)
		cout<<*sit<<" ";
		cout<<endl;
	}
}
void TextQuery::displayFile()
{
	vector<string>::iterator it;
	for(it=_wm->begin(); it!=_wm->end(); ++it)
	{
		cout<<*it<<endl;
	}
}
class QueryResult
{
	friend void print(const QueryResult &qr);
public:
	QueryResult(const string &s, shared_ptr<set<int>> lineNum, shared_ptr<vector<string>> file)
	: _sought(s)
	, _lineNum(lineNum)
	, _file(file)
	{}
	set<int>::iterator begin()
	{
		return _lineNum->begin();
	}
	set<int>::iterator end()
	{
		return _lineNum->end();
	}
	shared_ptr<vector<string>> getFile()
	{
		return _file;
	}
private:
	string _sought;
	shared_ptr<set<int>> _lineNum;
	shared_ptr<vector<string>> _file;
};
QueryResult TextQuery::query(const string &s) const
{
	map<string, shared_ptr<set<int>>>::iterator ret=_lines->find(s);
	static shared_ptr<set<int>> noneset(new set<int>);
	if(ret==_lines->end())return QueryResult(s, noneset, _wm);
	else return QueryResult(s, ret->second, _wm);
}
void print(const QueryResult &qr)
{
	cout<<"'"<<qr._sought<<"' occurs " <<qr._lineNum->size()<<" times"<<endl;
	for(set<int>::iterator it=qr._lineNum->begin(); it!=qr._lineNum->end(); ++it)
	{
		cout<<"\t(line "<<*it+1<<" ) "<<(*qr._file)[*it]<<endl;
	}
}
void runTextQuery0(const string &filename)
{
	TextQuery tq(filename);
	string s;
	while(1)
	{
		cout<<"please input the word to look for, 'q' or 'exit' to quite:"<<endl;
		if(!(cin>>s) || s=="q" || s=="exit")break;
		print(tq.query(s));
	}
}
class QueryBase
{
	friend class Query;
protected:
	virtual QueryResult eval(const TextQuery &qt)=0;
	virtual string rep()=0;
};
class WordQuery : public QueryBase
{
	friend class Query;
	WordQuery(const string &s)
	: _sought(s)
	{}
	QueryResult eval(const TextQuery &qt)
	{
		return qt.query(_sought);
	}
	string rep()
	{
		return _sought;
	}
	string _sought;
};
class Query
{
	friend Query operator~(const Query &qr);
	friend Query operator&(const Query &lhs, const Query &rhs);
	friend Query operator|(const Query &lhs, const Query &rhs);
public:
	Query(const string &s)
	: _p(new WordQuery(s))
	{}
	QueryResult eval(const TextQuery &tq)
	{
		return _p->eval(tq);
	}
	string rep()
	{
		return _p->rep();
	}
private:
	Query(const shared_ptr<QueryBase> p)
	: _p(p)
	{}
	shared_ptr<QueryBase> _p;
};
class NotQuery : public QueryBase
{
	friend Query operator~(const Query &qr);
	NotQuery(const Query &qr)
	: query(qr)
	{}
	QueryResult eval(const TextQuery &tq);
	string rep()
	{
		return "~("+query.rep()+")";
	}
	Query query;
};
QueryResult NotQuery::eval(const TextQuery &tq)
{
	QueryResult res=query.eval(tq);
	set<int>::iterator it_begin=res.begin();
	set<int>::iterator it_end=res.end();
	int sum=(int)res.getFile()->size();
	shared_ptr<set<int>> pset(new set<int>);
	for(int i=0; i<sum; ++i)
	{
		if(it_begin==it_end || i!=*it_begin)pset->insert(i);
		else ++it_begin;
	}
	return QueryResult(rep(), pset, res.getFile());
}
Query operator~(const Query &qr)
{
	return shared_ptr<QueryBase>(new NotQuery(qr));
}
class AndQuery : public QueryBase
{
	friend Query operator&(const Query &lhs, const Query &rhs);
	AndQuery(const Query &lhs, const Query rhs)
	: _lhs(lhs)
	, _rhs(rhs)
	{}
	QueryResult eval(const TextQuery &tq);
	string rep()
	{
		return "("+_lhs.rep()+") & ("+_rhs.rep()+")";
	}
	Query _lhs;
	Query _rhs;
};
QueryResult AndQuery::eval(const TextQuery &tq)
{
	QueryResult lset=_lhs.eval(tq);
	QueryResult rset=_rhs.eval(tq);
	shared_ptr<set<int>> pres(new set<int>);
	set_intersection(lset.begin(), lset.end(), rset.begin(), rset.end(), inserter(*pres, pres->begin()));
	return QueryResult(rep(), pres, lset.getFile());
}
Query operator&(const Query &lhs, const Query &rhs)
{
	return shared_ptr<QueryBase>(new AndQuery(lhs, rhs));
}
class OrQuery : public QueryBase
{
	friend Query operator|(const Query &lhs, const Query &rhs);
	OrQuery(const Query &lhs, const Query &rhs)
	: _lhs(lhs)
	, _rhs(rhs)
	{}
	QueryResult eval(const TextQuery &tq);
	string rep()
	{
		return "("+_lhs.rep()+") | ("+_rhs.rep()+")";
	}
	Query _lhs;
	Query _rhs;
};
QueryResult OrQuery::eval(const TextQuery &tq)
{
	QueryResult lset=_lhs.eval(tq);
	QueryResult rset=_rhs.eval(tq);
	shared_ptr<set<int>> pset(new set<int>);
	pset->insert(lset.begin(), lset.end());
	pset->insert(rset.begin(), rset.end());
	return QueryResult(rep(), pset, lset.getFile());
}
Query operator|(const Query &lhs, const Query &rhs)
{
	return shared_ptr<QueryBase>(new OrQuery(lhs, rhs));
}
void runTextQuery1(const string &filename)
{
	TextQuery tq(filename);
	Query q=Query("and") & Query("the");
	print(q.eval(tq));
	cout<<endl<<endl;

	q=~(Query("and") & Query("the"));
	print(q.eval(tq));
	cout<<endl<<endl;

	q=Query("the") | Query("god");
	print(q.eval(tq));
}
int main()
{
	//runTextQuery0("note1.txt");
	runTextQuery1("note1.txt");
	return 0;
}
