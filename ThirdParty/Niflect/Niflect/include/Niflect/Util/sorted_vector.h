/* STL-conforming "sorted vector" container
 *
 * (C) 2002 Martin Holzherr (holzherr@infobrain.com). All rights reserved.
 *
 * Permission is granted to use, distribute and modify this code provided that:
 *   ?this copyright notice appears,
 *   ?
 * The author welcomes any suggestions on the code or reportings of actual
 * use of the code. Please send your comments to holzherr@infobrain.com.
 *
 * The author makes NO WARRANTY or representation, either express or implied,
 * with respect to this code, its quality, accuracy, merchantability, or
 * fitness for a particular purpose.  This software is provided "AS IS", and
 * you, its user, assume the entire risk as to its quality and accuracy.
 *
 * Created:			November 19th, 2002
 * Last modified:	November 27th, 2002 
						(changed namespace from std to codeproject;
						uses template member functions for MSCVER>=1300)
								
 */

#ifndef SORTED_VECTOR_
#define SORTED_VECTOR_
#define VERSION_SORTED_VECTOR_ 0x00010010


#include <algorithm>
#include <vector>
#include <utility>
#include <functional>

#pragma pack(push,8)
#pragma warning(push,3)


namespace codeproject{
		// TEMPLATE CLASS sorted_vector

	template<class K, bool bNoDuplicates= true,class Pr = std::less<K>, class A = std::allocator<K> >
	class sorted_vector {
public:
	typedef sorted_vector<K,bNoDuplicates,Pr,A> Myt_;
    typedef std::vector<K,A>		Cont;
	typedef typename Cont::allocator_type	allocator_type;
	typedef typename Cont::size_type			size_type;
	typedef typename Cont::difference_type	difference_type;
	typedef typename Cont::reference			reference;
	typedef typename Cont::const_reference	const_reference;
	typedef typename Cont::value_type		value_type;
    typedef K						key_type;
	typedef typename Cont::iterator			iterator;
	typedef typename Cont::const_iterator	const_iterator;
    typedef Pr						key_compare;
    typedef Pr						value_compare;

	typedef typename Cont::const_reverse_iterator
									const_reverse_iterator;
	typedef typename Cont::reverse_iterator	reverse_iterator;

    typedef typename std::pair<iterator, iterator> Pairii_;
	typedef typename std::pair<const_iterator, const_iterator> Paircc_;
    typedef typename std::pair<iterator, bool> Pairib_;
	explicit sorted_vector(const Pr& pred = Pr(),const A& al = A())
		:key_compare_(pred),vec_(al){}
#if (_MSC_VER >= 1300)
	template<class It>
	sorted_vector(It first, It beyond, 
					const Pr& pred = Pr(),const A& al = A())
		:key_compare_(pred),vec_(first,beyond,al)
        {stable_sort();}
#else
    sorted_vector(const_iterator first, const_iterator beyond, 
					const Pr& pred = Pr(),const A& al = A())
        :key_compare_(pred),vec_(first,beyond,al)
        {stable_sort();}
#endif
	sorted_vector(const Myt_& x)
		: vec_(x.vec_),key_compare_(x.key_compare_)
        {}
    ~sorted_vector()                {}
    Myt_& operator=(const Myt_& x) {vec_.operator=(x.vec_);
                                     key_compare_= x.key_compare_;
                                     return *this;}
    Myt_& operator=(const Cont& x){vec_.operator=(x);
                                    sort();return *this;}
		
	void				reserve(size_type n)	{vec_.reserve(n);}
	iterator			begin()					{return vec_.begin(); }
	const_iterator		begin() const			{return vec_.begin(); }
    iterator			end()					{return vec_.end();}
    const_iterator		end() const				{return vec_.end();}
    reverse_iterator	rbegin()				{return vec_.rbegin();}
    const_reverse_iterator rbegin() const   
												{return vec_.rbegin();}

    reverse_iterator rend()						{return vec_.rend();}
    const_reverse_iterator rend() const     
												{return vec_.rend();}


    size_type size() const						{return vec_.size();}
    size_type max_size() const					{return vec_.max_size();}
    bool empty() const							{return vec_.empty();}
    A get_allocator() const						{return vec_.get_allocator();}
    const_reference at(size_type p) const		{return vec_.at(p);}
    reference at(size_type p)					{return vec_.at(p);}
	const_reference operator[](size_type p) const
												{return vec_.operator[](p);}
		
	reference operator[](size_type p)			{return vec_.operator[](p);}
    reference front()							{return vec_.front();}
	const_reference front() const				{return vec_.front();}
    reference back()							{return vec_.back();}
    const_reference back() const				{return vec_.back();}
    void pop_back()								{vec_.pop_back();}

    void assign(const_iterator first, const_iterator beyond)					
												{vec_.assign(first,beyond);}
	void assign(size_type n, const K& x = K())
												{vec_.assign(n,x);}
/*insert members*/
   Pairib_ insert(const value_type& x)
		{
            if(bNoDuplicates){
                iterator p= lower_bound(x);
                if(p==end()||key_compare_(x,*p)){
                    return Pairib_(InsertImpl_(p,x),true);
                }else{
                    return Pairib_(p,false);
                }
            }else{
                iterator p= upper_bound(x);
                return Pairib_(InsertImpl_(p,x),true);
            }
        }
   iterator insert(iterator it, const value_type& x)//it is the hint
        {
           if(it!=end() ){
               if(bNoDuplicates){
                   if(key_compare_(*it,x)){
                       if((it+1)==end()||KeyCompare_Gt_(*(it+1),x)){//use hint
                            return InsertImpl_(it+1,x);
                       }else if(KeyCompare_Geq_(*(it+1),x)){
                           return end();
                       }
                    }
               }else{
                   if(	KeyCompare_Leq_(*it,x)
					   &&((it+1)==end()||KeyCompare_Geq_(*(it+1),x))){
                       return InsertImpl_(it+1,x);
                   }
               }
           }
           return insert(x).first;
        }
#if (_MSC_VER >= 1300)
  template<class It>
	void insert(It first, It beyond)
    {
        size_type n= std::distance(first,beyond);
        reserve(size()+n);
        for( ;first!=beyond;++first){
            insert(*first);
        }
    }
#else
   void insert(const_iterator first, const_iterator beyond)
        {
            size_type n= std::distance(first,beyond);
            reserve(size()+n);
            for( ;first!=beyond;++first){
                insert(*first);
            }
        }
#endif
    iterator erase(iterator p)          {return vec_.erase(p);}
	iterator erase(iterator first, iterator beyond)
                                        {return vec_.erase(first,beyond);}
    size_type erase(const K& key)     
        {
            Pairii_ begEnd= equal_range(key);
            size_type n= std::distance(begEnd.first,begEnd.second);
            erase(begEnd.first,begEnd.second);
            return n;
        }
    void clear()                        {return vec_.clear();}
		
    bool Eq_(const Myt_& x) const      
		{return (size() == x.size()
		&& std::equal(begin(), end(), x.begin())); }
	bool Lt_(const Myt_& x) const
        {return (std::lexicographical_compare(begin(), end(),
										x.begin(), x.end()));}
	void swap(Myt_& x)
        {vec_.swap(x.vec_);std::swap(key_compare_,x.key_compare_);}
        
	friend void swap(Myt_& x, Myt_& Y_)
		{x.swap(Y_); }

	key_compare key_comp() const			{return key_compare_; }
    value_compare value_comp() const		{return (key_comp()); }
	iterator find(const K& k)
		{	iterator p = lower_bound(k);
			return (p==end()||key_compare_(k, *p))? end():p;
		}
	const_iterator find(const K& k) const
		{const_iterator p = lower_bound(k);
        return (p==end()||key_compare_(k,*p))?end():p;}
	size_type count(const K& k) const
		{Paircc_ Ans_ = equal_range(k);
        size_type n = std::distance(Ans_.first, Ans_.second);
        return (n); }
	iterator lower_bound(const K& k)
        {return std::lower_bound(begin(), end(), k, key_compare_); }
	const_iterator lower_bound(const K& k) const
        {return std::lower_bound(begin(), end(), k, key_compare_); }
	iterator upper_bound(const K& k)
		{return std::upper_bound(begin(), end(), k, key_compare_); }
	const_iterator upper_bound(const K& k) const
		{return std::upper_bound(begin(), end(), k, key_compare_); }
	Pairii_ equal_range(const K& k)
        {return std::equal_range(begin(), end(), k, key_compare_); }
	Paircc_ equal_range(const K& k) const
		{return std::equal_range(begin(), end(), k, key_compare_); }

/*functions for use with direct std::vector-access*/
    Cont& get_container()
        {return vec_;}
    void sort()//restore sorted order after low level access 
        {   std::sort(vec_.begin(),vec_.end(),key_compare_);
            if( bNoDuplicates ){
                vec_.erase(Unique_(),vec_.end());
            }
        }
    void stable_sort()//restore sorted order after low level access 
        {   std::stable_sort(vec_.begin(),vec_.end(),key_compare_);
            if( bNoDuplicates ){
                erase(Unique_(),end());
            }
        }   
protected:
    iterator Unique_()
        {   iterator front_= vec_.begin(),out_= vec_.end(),end_=vec_.end();
            bool bCopy_= false;
            for(iterator prev_; (prev_=front_)!=end_ && ++front_!=end_; ){
                if( key_compare_(*prev_,*front_)){
                    if(bCopy_){
                        *out_= *front_;
                        out_++;
                    }
                }else{
                    if(!bCopy_){out_=front_;bCopy_=true;}
                }
            }
            return out_;
        }
    iterator InsertImpl_(iterator p,const value_type& x)
        {return vec_.insert(p,x);}
    bool KeyCompare_Leq_(const K& ty0,const K& ty1)
        {return !key_compare_(ty1,ty0);}
    bool KeyCompare_Geq_(const K& ty0,const K& ty1)
        {return !key_compare_(ty0,ty1);}
    bool KeyCompare_Gt_(const K& ty0,const K& ty1)
        {return key_compare_(ty1,ty0);}

    key_compare         key_compare_;
    Cont                vec_;
};


template<class K,bool bNoDuplicates,class Pr, class A> inline
	bool operator==(const sorted_vector<K, bNoDuplicates,Pr,A>& x,
		            const sorted_vector<K, bNoDuplicates,Pr,A>& Y_)
	{return x.Eq_(Y_); }
template<class K,bool bNoDuplicates,class Pr, class A> inline
	bool operator!=(const sorted_vector<K, bNoDuplicates,Pr,A>& x,
		            const sorted_vector<K, bNoDuplicates,Pr,A>& Y_)
	{return !(x == Y_); }
template<class K,bool bNoDuplicates,class Pr, class A> inline
	bool operator<(const sorted_vector<K, bNoDuplicates,Pr,A>& x,
		            const sorted_vector<K, bNoDuplicates,Pr,A>& Y_)
	{return x.Lt_(Y_);}
template<class K,bool bNoDuplicates,class Pr,class A> inline
	bool operator>(const sorted_vector<K, bNoDuplicates,Pr,A>& x,
		            const sorted_vector<K, bNoDuplicates,Pr,A>& Y_)
	{return Y_ < x; }
template<class K,bool bNoDuplicates,class Pr, class A> inline
	bool operator<=(const sorted_vector<K, bNoDuplicates,Pr,A>& x,
		            const sorted_vector<K, bNoDuplicates,Pr,A>& Y_)
	{return !(Y_ < x); }
template<class K, bool bNoDuplicates,class Pr,class A> inline
	bool operator>=(const sorted_vector<K, bNoDuplicates,Pr,A>& x,
		            const sorted_vector<K, bNoDuplicates,Pr,A>& Y_)
	{return (!(x < Y_)); }
}
#pragma warning(pop)
#pragma pack(pop)
#elif VERSION_SORTED_VECTOR_ != 0x00010010
#error You have included two sorted_vector.h with different version numbers
#endif

//typedef codeproject::sorted_vector<std::string> StrSet;
//StrSet months;
//months.insert("jan");
//months.insert("feb");
//months.insert("mar");
//months.insert("apr");
//months.insert("may");
//months.insert("jun");
//auto a = months.insert("jul");
//if (a.second)
//{
//	auto& b = a.first;
//	auto c = std::distance(months.begin(), b);
//	printf("");
//}
//auto d = months.insert("jul");
//if (d.second)
//{
//	auto& b = d.first;
//	auto c = std::distance(months.begin(), b);
//	printf("");
//}
//months.insert("aug");
//months.insert("sep");
//months.insert("oct");
//months.insert("nov");
//months.insert("dec");
//StrSet::iterator it = months.find("jul");
//NIFLECT_ASSERT(*it == "jul");
//std::cout << "previous of jul (in alphabetical order) is " << (it[-1]) << std::endl;
//std::cout << "next of jul (in alphabetical order) is " << (it[1]) << std::endl;

//typedef codeproject::sorted_vector<std::string> StrSet;
//std::vector<std::string> vecUnordered;
//StrSet months;
//auto pos = months.insert("jan");
//vecUnordered.insert(vecUnordered.begin() + std::distance(months.begin(), pos.first), *pos.first);
//pos = months.insert("feb");
//vecUnordered.insert(vecUnordered.begin() + std::distance(months.begin(), pos.first), *pos.first);
//pos = months.insert("mar");
//vecUnordered.insert(vecUnordered.begin() + std::distance(months.begin(), pos.first), *pos.first);
//pos = months.insert("apr");
//vecUnordered.insert(vecUnordered.begin() + std::distance(months.begin(), pos.first), *pos.first);
//pos = months.insert("may");
//vecUnordered.insert(vecUnordered.begin() + std::distance(months.begin(), pos.first), *pos.first);
//pos = months.insert("jun");
//vecUnordered.insert(vecUnordered.begin() + std::distance(months.begin(), pos.first), *pos.first);
//pos = months.insert("jul");
//vecUnordered.insert(vecUnordered.begin() + std::distance(months.begin(), pos.first), *pos.first);
//pos = months.insert("aug");
//vecUnordered.insert(vecUnordered.begin() + std::distance(months.begin(), pos.first), *pos.first);
//pos = months.insert("sep");
//vecUnordered.insert(vecUnordered.begin() + std::distance(months.begin(), pos.first), *pos.first);
//pos = months.insert("oct");
//vecUnordered.insert(vecUnordered.begin() + std::distance(months.begin(), pos.first), *pos.first);
//pos = months.insert("nov");
//vecUnordered.insert(vecUnordered.begin() + std::distance(months.begin(), pos.first), *pos.first);
//pos = months.insert("dec");
//vecUnordered.insert(vecUnordered.begin() + std::distance(months.begin(), pos.first), *pos.first);
//for (size_t idx=0; idx<months.size(); ++idx)
//{
//	NIFLECT_ASSERT(months[idx] == vecUnordered[idx]);
//}
//StrSet::iterator it = months.find("jul");
//NIFLECT_ASSERT(*it == "jul");
//std::cout << "previous of jul (in alphabetical order) is " << (it[-1]) << std::endl;
//std::cout << "next of jul (in alphabetical order) is " << (it[1]) << std::endl;

/*
#pragma warning(disable:4786)

#include <set>
#include "sorted_vector.h"
#include <iostream>
#include "assert.h"
#include <time.h>
#include <iomanip>

struct STest {
	std::string s;
	int         id;
};

STest MakeSTest()
{
	static int id = 0;
	int nLen = rand() % 10;
	char* letters[] = { "0","1" };
	STest stest;

	for (int i = 0; i < nLen; i++) {
		stest.s += letters[rand() % 2];
	}
	stest.id = ++id;
	return stest;
}

bool operator<(const STest& t0, const STest& t1)
{
	int nCmp;
	return (nCmp = t0.s.compare(t1.s)) < 0 || nCmp == 0 && t0.id < t1.id;
}


class SimpTimer {
public:
	SimpTimer(const char* msg) :m_msg(msg), m_start(clock()) {}
	~SimpTimer()
	{
		m_stop = clock();
		std::cout << "CPU usage (" << std::setw(35) << m_msg.c_str() << ") = "
			<< (double)(m_stop - m_start) / CLOCKS_PER_SEC
			<< " seconds" << std::endl;
	}
private:
	time_t m_start, m_stop;
	std::string     m_msg;
};


template<class It, class Pred>
bool is_unique(It beg, It beyond, Pred pred)
{
	return std::adjacent_find(beg, beyond, std::not2(pred)) == beyond;
}
template<class FwIt, class Pred>
FwIt unsorted_find(FwIt beg, FwIt beyond, Pred pred)
{
	for (FwIt prev; (prev = beg) != beyond && ++beg != beyond;) {
		if (pred(*beg, *prev)) {
			return prev;
		}
	}
	return beyond;
}
template<class T>
void TestSet(std::vector<T>& v)
{
	std::set<T> std_set;
	{SimpTimer t("build std::set");
	for (unsigned i = 0; i < v.size(); i++) {
		std_set.insert(v[i]);
	}
	}
	{SimpTimer t("find in std::set");
	for (unsigned i = 0; i < v.size(); i++) {
		std::set<T>::iterator it = std_set.find(v[i]);
		std::set<T>::size_type nCount = std_set.count(v[i]);
		assert(nCount == 0 && it == std_set.end()
			|| nCount != 0 && it != std_set.end());
	}
	}
}
template<class T>
void TestSortedVector_AsSet(std::vector<T>& v)
{
	codeproject::sorted_vector<T, true> svec;
	{SimpTimer t("build sortedvec_set (naiv)");
	for (unsigned i = 0; i < v.size(); i++) {
		svec.insert(v[i]);
	}
	}
	assert(is_unique(svec.begin(), svec.end(), svec.key_comp()));
	codeproject::sorted_vector<T, true> svec1;
	{SimpTimer t("build sortedvec_set (opt.)");
	codeproject::sorted_vector<T, true>::Cont& vec = svec1.get_container();
	for (unsigned i = 0; i < v.size(); i++) {
		vec.push_back(v[i]);

	}
	svec1.sort();
	}
	assert(svec == svec1);
	assert(is_unique(svec1.begin(), svec1.end(), svec1.key_comp()));

	{SimpTimer t("find in sortedvec_set");
	for (unsigned i = 0; i < v.size(); i++) {
		codeproject::sorted_vector<T, true>::iterator it = svec1.find(v[i]);
		codeproject::sorted_vector<T, true>::size_type nCount = svec1.count(v[i]);
		assert(nCount == 0 && it == svec1.end()
			|| nCount != 0 && it != svec1.end());
	}
	}
	for (unsigned i = 0; i < v.size(); i++) { svec1.erase(v[i]); }
	assert(svec1.empty());
}
template<class T>
void TestMultiSet(std::vector<T>& v)
{
	std::multiset<T> svec;

	{SimpTimer t("build multiset");
	for (unsigned i = 0; i < v.size(); i++) {
		svec.insert(v[i]);
	}
	}
	assert(unsorted_find(svec.begin(), svec.end(), svec.key_comp()) == svec.end());
	{SimpTimer t("find in multiset");
	for (unsigned i = 0; i < v.size(); i++) {
		std::multiset<T>::iterator it = svec.find(v[i]);
		std::multiset<T>::size_type nCount = svec.count(v[i]);
		assert(nCount == 0 && it == svec.end()
			|| nCount != 0 && it != svec.end());
	}
	}
}

template<class T>
void TestSortedVector_AsMultiSet(std::vector<T>& v)
{
	codeproject::sorted_vector<T> svec;
	{SimpTimer t("build sortedvec_multiset (naiv)");
	for (unsigned i = 0; i < v.size(); i++) {
		svec.insert(v[i]);
	}
	}
	codeproject::sorted_vector<T> svec1;
	{SimpTimer t("build sortedvec_multiset (opt.)");
	codeproject::sorted_vector<T>::Cont& vec = svec1.get_container();
	for (unsigned i = 0; i < v.size(); i++) {
		vec.push_back(v[i]);

	}
	svec1.stable_sort();
	}
	assert(svec == svec1);
	assert(unsorted_find(svec.begin(), svec.end(), svec.key_comp()) == svec.end());
	{SimpTimer t("find in sortedvec_multiset");
	for (unsigned i = 0; i < v.size(); i++) {
		int n = rand();
		codeproject::sorted_vector<T>::iterator it = svec1.find(v[i]);
		codeproject::sorted_vector<T>::size_type nCount = svec1.count(v[i]);
		assert(nCount == 0 && it == svec1.end()
			|| nCount != 0 && it != svec1.end());
	}
	}
	//test various functions
	const codeproject::sorted_vector<T> svec2(svec);
	assert(svec == svec2);
	for (unsigned i = 0; i < v.size(); i++) { assert(svec2.find(v[i]) != svec2.end()); }
	codeproject::sorted_vector<T> svec3(v.begin(), v.end());
	assert(svec3 == svec2);
	codeproject::sorted_vector<T> svec4(v.begin(), v.begin() + (v.end() - v.begin()) / 2);
	svec4 = svec3;
	assert(svec4 == svec3);
	while (svec4.size() > 0) {
		svec4.pop_back();
	}
}




template<class T>
void ExecTests(std::vector<T>& v)
{
	std::cout << "std::set versus 'sorted_vector as set'" << std::endl;
	TestSet(v);
	TestSortedVector_AsSet(v);
	std::cout << "std::multiset versus 'sorted_vector as multiset'" << std::endl;
	TestMultiSet(v);
	TestSortedVector_AsMultiSet(v);
}

template<class T, class Pred>
void TestSetOperations(const std::vector<T>& v0,
	const std::vector<T>& v1,
	const std::vector<T>& v2,
	Pred pred)
{
	//A) compute the result of the set-operation: (v0-v1)+v2 - intersect(v1,v2)
	codeproject::sorted_vector<T, true, Pred> svec(v0.begin(), v0.end(), pred);
	codeproject::sorted_vector<T, true, Pred> svec_v1(pred); svec_v1 = v1;
	codeproject::sorted_vector<T, true, Pred> svec_v2(pred); svec_v2 = v2;
	unsigned i, j;
	for (i = 0; i < v1.size(); i++) { svec.erase(v1[i]); }
	for (j = 0; j < v2.size(); j++) { svec.insert(v2[j]); }
	unsigned m;
	for (m = 0; m < svec_v1.size(); m++) {
		if (svec_v2.find(svec_v1[m]) != svec_v2.end()) {
			svec.erase(svec_v1[m]);
		}
	}
	//B) compute the result of the set-operation (v0+v2)-v1
	codeproject::sorted_vector<T, true, Pred> svec1(v0.begin(), v0.end(), pred);
	for (unsigned k = 0; k < v2.size(); k++) { svec1.insert(v2[k]); }
	for (unsigned l = 0; l < v1.size(); l++) { svec1.erase(v1[l]); }
	codeproject::sorted_vector<T, true, Pred>::size_type nSize = svec.size();
	codeproject::sorted_vector<T, true, Pred>::size_type nSize1 = svec1.size();
	//test whether results are the same
	assert(svec == svec1);
}


codeproject::sorted_vector<int, true>
BuildIntersection(std::vector<int>& v0, std::vector<int>& v1)
{
	codeproject::sorted_vector<int, true> svec(v0.begin(), v0.end());
	codeproject::sorted_vector<int, true> svecIntersection;
	for (unsigned i = 0; i < v1.size(); i++) {
		if (svec.find(v1[i]) != svec.end()) {
			svecIntersection.insert(v1[i]);
		}
	}
	return svecIntersection;
}
codeproject::sorted_vector<int, true>
BuildIntersection1(std::vector<int>& v0, std::vector<int>& v1)
{
	codeproject::sorted_vector<int, true> svec(v0.begin(), v0.end());
	codeproject::sorted_vector<int, true> svecIntersection;
	codeproject::sorted_vector<int, true>::Cont& vInterSect =
		svecIntersection.get_container();
	for (unsigned i = 0; i < v1.size(); i++) {
		if (svec.find(v1[i]) != svec.end()) {
			vInterSect.push_back(v1[i]);
		}
	}
	svecIntersection.sort();
	return svecIntersection;
}


struct ltstr
{
	bool operator()(const char* s1, const char* s2) const
	{
		return strcmp(s1, s2) < 0;
	}
};

int SGITest()
{
	using namespace std;
	using namespace codeproject;
	const int N = 6;
	const char* a[N] = { "isomer", "ephemeral", "prosaic",
		"nugatory", "artichoke", "serif" };
	const char* b[N] = { "flat", "this", "artichoke",
		"frigate", "prosaic", "isomer" };

	sorted_vector<const char*, true, ltstr> A(a, a + N);
	sorted_vector<const char*, true, ltstr> B(b, b + N);
	sorted_vector<const char*, true, ltstr> C;

	cout << "Set A: ";
	copy(A.begin(), A.end(), ostream_iterator<const char*>(cout, " "));
	cout << endl;
	cout << "Set B: ";
	copy(B.begin(), B.end(), ostream_iterator<const char*>(cout, " "));
	cout << endl;

	cout << "Union: ";
	set_union(A.begin(), A.end(), B.begin(), B.end(),
		ostream_iterator<const char*>(cout, " "),
		ltstr());
	cout << endl;
	return 0;
}

void TestAllSet()
{
	using namespace std;
	using namespace codeproject;
	typedef sorted_vector<const char*, true, ltstr> StrSet;
	StrSet months;
	months.insert("jan");
	months.insert("feb");
	months.insert("mar");
	months.insert("apr");
	months.insert("may");
	months.insert("jun");
	months.insert("jul");
	months.insert("aug");
	months.insert("sep");
	months.insert("oct");
	months.insert("nov");
	months.insert("dec");
	StrSet::iterator it = months.find("jul");
	assert(strcmp(*it, "jul") == 0);
	cout << "previous of jul (in alphabetical order) is " << (it[-1]) << endl;
	cout << "next of jul (in alphabetical order) is " << (it[1]) << endl;

	cout << "months in alphabetical order: " << endl;
	copy(months.begin(), months.end(), ostream_iterator<const char*>(cout, " "));
	cout << endl << "months in reverse alphabetical order: " << endl;
	copy(months.rbegin(), months.rend(), ostream_iterator<const char*>(cout, " "));
	//test count
	{
		for (StrSet::iterator it = months.begin(); it != months.end(); ++it) {
			assert(months.count(*it) == 1);
		}
	}
	//test copy construction and comparison operators
	StrSet monthsCopy(months);
	assert(months == monthsCopy
		&&  months <= monthsCopy && months >= monthsCopy
		&& !(months < monthsCopy) && !(months > monthsCopy));

	std::pair<StrSet::iterator, StrSet::iterator> pairMismatch =
		mismatch(months.begin(), months.end(), monthsCopy.begin());
	assert(pairMismatch.first == months.end() && pairMismatch.second == monthsCopy.end());

	//test insertion of already present element
	copy(months.begin(), months.end(), inserter(monthsCopy, monthsCopy.begin()));
	assert(months.size() == monthsCopy.size());

	//test insert member functions
	months.insert(monthsCopy.begin(), monthsCopy.end());
	assert(months == monthsCopy);
	StrSet months1(months.begin(), months.begin() + 3);
	months1.insert(months.begin() + 1, months.end());
	assert(months1 == months);
	months1.insert("aug");
	months1.insert("xxx");
	months1.insert(months1.find("xxx"), "yyy");
	months1.insert("zzz");
	assert(months1 > months && months1.size() == months.size() + 3);
	//test erase member functions
	months1.erase(months1.find("xxx"), months1.end());
	assert(months1.size() == months.size());

	//test lower_bound,upper_bound,equal_range
	assert(strcmp(*months.lower_bound("jul"), "jul") == 0);


	cout << endl;
}


int main()
{
	//timed tests
	std::vector<int> v;
	int i;
	for (i = 0; i < 50000; i++) { v.push_back(rand()); }
	std::cout << "--------------Tests with element type int-------------" << std::endl;
	ExecTests(v);

	std::vector<STest> vt;
	for (i = 0; i < 50000; i++) { vt.push_back(MakeSTest()); }
	std::cout << "-Tests with element type 'STest' (string,int)--------" << std::endl;
	ExecTests(v);

	//set operations-test
	std::vector<int> v1, v2;
	for (i = 0; i < 10000; i++) { v1.push_back(rand()); }
	for (i = 0; i < 10000; i++) { v2.push_back(rand()); }
	TestSetOperations(v, v1, v2, std::greater<int>());

	assert(BuildIntersection(v1, v2) == BuildIntersection1(v1, v2));
	SGITest();
	TestAllSet();
	return 0;
}
*/