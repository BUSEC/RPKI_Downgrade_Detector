#include<utility>
#include<map>
#include<unordered_map>
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<algorithm>
#include<assert.h>

#include<set>


using namespace std;


class IPAddr {
private:
  unsigned int a; // NOTE:  numBits is set to 32,  should this be int32?
  
public:
  static const int numBits = 32;
  static const unsigned int maxAddr = 0xFFFFFFFF;
  inline IPAddr(): a(0) {}
  inline IPAddr(int x): a(x) {}
  inline IPAddr(unsigned int x1, unsigned int x2, unsigned int x3, unsigned int x4) {a= ((x1&0xff)<<24) | ((x2&0xff)<<16) | ((x3&0xff)<<8) | (x4&0xff); }
  inline IPAddr(const IPAddr & x) {a=x.a;}
  inline IPAddr operator+(unsigned int i) const {assert((a+i)>=a); IPAddr ret (a+i); return ret;}
  inline IPAddr operator+(IPAddr i) const { return i+a;}
  inline IPAddr operator-(unsigned int i) const {assert(i <=a ); IPAddr ret (a-i); return ret;}
  inline IPAddr operator-(IPAddr i) const {IPAddr ret (a-i.a); return ret;} 
  inline bool operator<(const IPAddr b) const {return a<b.a;}
  inline bool operator<=(const IPAddr b) const {return a<=b.a;}
  inline bool operator==(const IPAddr b) const {return a==b.a;}
  inline IPAddr operator>>(int shift) const {assert(shift < 32); return a>>shift;}
  inline IPAddr truncate (int l) { // truncate the address to have only the top l bits
    IPAddr ret(a);
    if (l==0)
      ret.a=0;
    else {
      ret.a>>=(numBits-l);
      ret.a<<=(numBits-l);
    }
    return ret;
  }
  
  friend ostream& operator<<(ostream& os, const IPAddr a);
};

ostream& operator<<(ostream& os, IPAddr a)
{
  //os<<a.a<<":" ;
  os << (a.a>>24) << "." << ((a.a>>16)&0xff) << "." << ((a.a>>8) & 0xff) << "." << (a.a&0xff);
  return os;
}

/*** WARNING: THIS IS NOT NECESSARILY GOING TO WORK WHEN WE SWITCH TO IPv6 ADDRESSES ***/
/*** REWRITING endAddress (perhaps using truncate?) will probably make it IPv6 SAFE ***/ 
class IPPrefix {
private:
  IPAddr prefix; 
  int length;
public:
  inline IPPrefix(const IPPrefix & p): prefix (p.prefix), length(p.length) {}
  inline IPPrefix(IPAddr p, int l): prefix(p.truncate(l)), length(l){}
  inline int getLength() const { return length;}
  inline IPAddr getPrefix() const {return prefix;}
  // Computes the last subprefix of a given prefix at a given level
  // For example, if the prefix is 10.10.0.0/16
  // then at level 17, its two subprefixes are 10.10.0.0/17 and 10.10.128.0/17
  // The method, for level 17, will output 10.10.128.0
  // Another example: if the prefix is 10.10.0.0/16
  // then at level 24, its subprefixes range from 10.10.0.0/24 to 10.10.255.0/24
  // The method, for level 24, will output 10.10.255.0
  inline IPAddr endAddress (int level) const {
    if (level<length) return 0;
    
    //Can't bit shift by 32
    assert( length >= 0 ); 
    if (length == 0){
      if (level == 0) return prefix;
      // at this point we know numBits-level<32, so the shift is safe
      else return (IPAddr::maxAddr - (1<<(IPAddr::numBits-level)) ) +1; //~erh refactor
    }
    
    // at this point we know numBits - length and numBits - level < 32, so the shifts are safe
    // we know we are adding a nonnegative quantity because length<=level
    // we know this addtion won't overflow because prefix has all zero bits after
    // the first length bits
    return prefix+((1<<(IPAddr::numBits-length))-(1<<(IPAddr::numBits-level)));
  }
  inline bool isSubPrefixOf (IPPrefix other) { // this check is not strict--it's "subprefix or equal"
    return other.length<=length && prefix.truncate(other.length)==other.prefix;
  }
  
  friend ostream& operator<<(ostream& os,  IPPrefix a);  
};

ostream& operator<<(ostream& os, IPPrefix p)
{
  os<<p.prefix<<"/"<<p.length;
  return os;
}


template <typename T> class IntervalList;

template <typename T>
ostream& operator<< (ostream& os, const IntervalList<T> & l);

// Keeps a set of nonoverlapping closed intervals, where beginning and end of
// each interval is of type T
// T should be a totally ordered type and provide
// < , <=, and == 
// It also needs to provide + and - operators,
// in order for us to be able to remove a closed interval from another
// closed interval and still end up with a closed interval
// Has a paremeter "increment"
// We think of intervals as only containing multiples of increment.
// Begin and end of any intervals supplied to methods
// of this class should be multiples of increment.  
// Begin and end of any interval produced by this class as a result of operations
// will also be multiples of increment.
// For example, if increment is 2, then union of [0,2] and [4,6] will be [0,6] 
// LEOCOM: overload % for IPAddr, add asserts about multiples of increment
template <typename T> class IntervalList {
private:
  // keeps a map keyed by the beginning of the interval, with end of the interval as value
  map<T,T> m;
  int increment;
public:
  inline IntervalList () {increment = 1;}
  inline bool operator != (const IntervalList<T> arg) const {
    return m!=arg.m;
  }  
  inline void clear () {m.clear();}
  inline bool empty () {return m.empty();}

  // reinitialize with a single interval
  inline void setToInterval(const T & begin, const T & end) {
    m.clear();
    if (begin<=end)
      m[begin]=end;		
  }
  
  // WARNING:
  // All the intervals must begin and end at multiples of i, else we are not guaranteed to work
  // Do not change once intervals have been added
  inline void setIncrement(int i) {
    assert(m.empty());
    increment = i;
  }

  inline int getIncrement() {
    return increment;
  }

  
  void add (T begin, T end) {
    if (end<begin) // nothing to do
      return;
    
    bool needToCreateInterval = true;
    
    // we want to find if begin overlaps or starts right after with any existing interval, in which case we will need to modify
    // the end of that existing interval
    typename map<T,T>::iterator iter = m.lower_bound(begin);
    // iter now points to the first interval whose start is at or after begin
    
    // if  begin was found exactly
    if (iter!=m.end() && iter->first == begin) { // this is the interval to modify
      if (end<=iter->second) return; // [begin,end] is wholly contained within [iter->first,iter->second]
      else iter->second = end;
      iter++;
      needToCreateInterval=false;
    }

    // we need to also consider the previous interval, if it exists, because its end may be after begin, equal to begin, or equal to begin-increment,
    // in which case we need to merge it with the new interval
    else if (!m.empty() && iter!=m.begin()) {
      iter--;

      // Order here is important since iter->second+increment might 
      // overflow, we must ensure that it can not but ensuring that end is
      // greater than iter->second prior to incrementing (since 
      // end>iter->second implies that there is atleast one increment of 
      // distance beyond iter->second).
      // Before this was fixed, there was an overflow bug here
      // in the case when iter points to the last interval that ends at the last possible value of T
      // The preconditions that used to trigger this former bug
      // can be found in AddToIntervalEdgeCaseBug1Test.
    if ( end <= iter->second ) return; // [begin,end] is wholly contained within [iter->first,iter->second]
    else if (begin<=iter->second+increment) { // at this point we know end>iter->second, so iter->second is safe to increment
      iter->second = end;
      needToCreateInterval=false;
    }  
      iter++;
    } 
    
    // Now we know begin does not overlap or start right after any existing interval
    // hence we create a new interval
    if (needToCreateInterval) m[begin]=end;
    
    // we are now sure that iter points to an interval whose start is strictly greater than begin    
    // we now iterate through all the intervals that start after begin and remove them if they are covered by our
    // newly modified/created interval
    while (iter != m.end() && iter->second <= end) {
      T intervalToRemove = iter->first;
      iter++;
      m.erase(intervalToRemove);
    }
    
    // the interval that iter now points may need to be merged with the one we just added, which is iter-1
    if (iter!=m.end() && iter->first<=end+increment) { 
      // the check iter!=m.end() ensures that we quit the loop above because iter->second > end
      // since iter->second > end, we know end is at least one increment away from overflowing, so end+increment cannot overflow
      T endToSave = iter->second;
      T intervalToRemove = iter->first;
      iter--;
      iter->second = endToSave;
      m.erase(intervalToRemove);
    }
  }
  
  
  void remove (T begin, T end) {
    if (end<begin) // nothing to do
      return;
  
    typename map<T,T>::iterator iter = m.upper_bound(end);
    // iter now points to the first interval whose start is after the end of the interval we are trying to remove
    // we don't need to remove the one pointed to by iter or anything to the right of it 
    if(iter==m.begin())
      return;
    --iter;
    bool done = false;
    // We now iterate through intervals, which are given by [iter->first, iter->second]
    // All the intervals we look at now have their start <= end of the inteval we are trying to remove
    // We need to look into removing  an interval (or a portion of it) as long as 
    // its end is >= begin of the interval we are trying to remove
    while (!done && begin<=iter->second) {
      // check if there is any tail of the current interval to preserve
      // This is Overflow safe because if end < iter-second it can't be on the overflow boundry.
      if (end < iter->second) { 
        // we know that end>=iter->first
        // insert a new interval starting with end+increment and ending with it->second
        // we know end+increment cannot overflow because iter->second is greater than end by at least increment
        m[end+increment]=iter->second;
      }
      // Move the iterator down, because now we will be either shortening or removing the interval that the iterator points to,
      // which will mess up the iterator.  Save iter->first in another variable
      T intervalStart = iter->first;
      if (iter!=m.begin()) // make sure we don't decrement past the beginning, because I couldn't find clear documentation on what that would do
        --iter;
      else
        done = true;
      
      // check if the head of the current interval needs preserving
      // This is underflow safe because begin > intervalStart so it can not be zero
      if (intervalStart < begin) 
        m[intervalStart] = begin-increment;
      // else remove the current interval
      else
        m.erase(intervalStart);
    }
  }
  
  

  IntervalList<T> subtractFromInterval (T begin, T end) {
    IntervalList<T> ret;
    ret.setIncrement(increment);
    subtractFromInterval(begin, end, ret);
    return ret;
  }
  
  void subtractFromInterval (T begin, T end, IntervalList<T> & ret) {
    ret.clear();    

    typename map<T,T>::iterator iter = m.lower_bound(begin);

    // iter now points to the first interval whose start is at or after begin
    // we need to also consider the previous interval, if it exists, because its end may be after begin
    T current;
    if (!m.empty() && iter!=m.begin()) {
      iter--;

      assert(iter->first < begin); //iter is previous of the lowerbound so this should always be true
      
      if (  end <=iter->second  ) return; //if true then begin-end contained completely within interval, ret should be empty

      // "if (  end <=iter->second  ) return;" prevents an overflow  in this line since
      // since we know end>iter->second at this point, and we work in multiples
      // of increment, so iter->second must be at least increment away from overflowing
      current = max(begin, iter->second+increment);
      iter++;
    }
    else current = begin; 
    // at any given point we have added to ret everything from begin (inclusive) to current (exclusive) that isn't covered by intervals in T
    // at any given point, iter->first is at or after current
    while (iter != m.end() && current <= end ) {

      // This line will not underflow since if current<iter->first then iter->first >= increment
      if (current<iter->first){
        ret.m[current]=min(iter->first-increment, end);
      }

      // We want to now move current to the right of the interval
      // [iter->first,iter->second].
      // However, we cannot do that if iter->second is at max value of T,
      // because current would overflow.
      // To avoid this problem, we observe that we are
      // done adding intervals if end<=iter->second, so we can return.
      // And  if end>iter>second, then iter-<second+increment cannot overflow.
      if (end<=iter->second) return;
      else current=iter->second+increment; // set current to be just beyond the interval

      ++iter; // advance the interval
    }
    if (current<=end) // no more intervals -- add everything until the end
      ret.m[current]=end;
  }

  
  // Let "a" be the number of intervals in arg
  // argDense should be set to true if a good fraction (about 1/log a or greater) 
  // of the total number of intervals in arg are likely to intersect something.
  // Functionaly, argDense determines how this algorithm will skip intervals in arg than intersect nothing
  // if t, it will skip them one-at-a-time
  // if f, it will skip an entire block of them until the next interval that
  // intersects something, at the cost of log a
  // 
  // Runnning time: let t, a, and i be the number of intervals in this, in arg, and in the intersection, respectively
  // if argDense = true, this operation will run in time O(a+(t+i)*log(t+i))
  // if argDense = false, this operation will run in time O((t+i)*((log a)+log(t+i)))
  // So its best to set arg to true if a < (t+i) log a and false otherwise
  //
  //
  // Note that when the size of the intersection is considerably smaller that the size of this
  // then it would be more efficient to compute the intersection as a separate
  // map, instead of changing this. It is possible to achieve running time O(i*(log a + log t + log i ))
  // by that method, which is NOT currently implemented.
  //
  // A method sparseIntersectWith (to be written) should be used in that case
  // 
  void denseIntersectWith (const IntervalList<T> & arg, bool argDense) {
    if (arg.m.empty()) {
      m.clear();
      return;
    }
    typename map<T,T>::iterator thisIter = m.begin();
    typename map<T,T>::const_iterator argIter=arg.m.begin();
    // consider one interval from this at a time
    // loop invariant: before each iteration of the loop starts, we know that
    // 1) Every interval in m before thisIter has already been considered, and all that's left
    //    of it is only whatever intersects with arg
    // 2) Interval pointed to by thisIter and after has not been considered
    // 3) No interval before argIter itersects with thisIter (but argIter itself may)
    //
    // Therefore, we want to end the loop with argIter pointing to the first interval
    // that ends after thisIter->second
    while (thisIter!=m.end()) {
      
      if (argIter == arg.m.end()) { // If arg has nothing left, erase the rest of m and quit
        // this check is not necessary for correctness -- the code below should work correctly without this check
        // but it will probably speed things up, especially in the argDense == false case, because it will save on
        // unnecessary invocations of lower_bound.
        // So we remove all subsequent intervals of this and break out of the loop.
        do {
          T thisBegin = thisIter->first;
          thisIter++;
          m.erase(thisBegin);
        } while (thisIter != m.end());
        break;
      }
      
      T thisBegin = thisIter->first;
      T thisEnd = thisIter->second;
      bool deleteCur = true;
      
      // We now look for the first interval in arg that may intersect [thisBegin,thisEnd],
      // Specifically, the first interval that ends at or after thisBegin.
      // The search is different depending on argDense
      if (argDense)
        while (argIter != arg.m.end() && argIter->second < thisBegin)
          argIter++;
      else {
        // We do the check below because if argIter->second>=thisBegin, then we argIter is already at the right spot
        // This check is not necessary for correctness -- the code below should work correctly even if we don't check whether argIter.second<thisBegin,
        // because lower_bound will find this interval, anyway.
        // However, it is a very easy check to do and immediately tells us that we are the right spot;
        // if it works, it will save an invocation of lower bound,
        // and it is likely to work reasonably often because one would expect intervals from arg that intersect intervals
        // from this to come in sequential order a reasonable fraction of the time
        if (argIter->second<thisBegin) { // we know argIter is not at the end, so this check is safe
          argIter = arg.m.lower_bound(thisBegin);
          // argIter now points to the first interval whose start is at or after thisBegin
          // we need to also consider the previous interval, if it exists, because its end may be after thisBegin
          // So we step back, check, and step forward if the check turns up nothing
          if (argIter!=arg.m.begin()) {
            argIter--;
            if (argIter->second<thisBegin) 
              argIter++;
          }
        }
      }
      
      // Now we know argIter is at the first iterval that ends at or after thisBegin
      // Check if it overlaps thisBegin and, if so, preserve thisBegin in m
      if (argIter!=arg.m.end() && argIter->first<=thisBegin) {
        if (thisEnd<argIter->second) {
          // this case, thisIter does not change and argIter should not advance
          thisIter++;
          continue; // so we short-circuit the loop, since there's nothing else to do -- and if we didn't short circuit, we'd have to move argIter forward,
                    // which we can't do, because argIter may overlap with the next interval in m
        }
        // else
        thisIter->second = argIter->second;
        deleteCur = false;
        // argIter ends at or before thisEnd, so we can advance and still preserve the loop invariant
        argIter++; 
      }
      
      // Now argIter begins after thisBegin
      // advance it until it ends after thisEnd
      //
      // Loop invariant: before every iteration of this loop, we know
      // 1) We have intersected [thisBegin, thisEnd] with everything before argIter,
      //    but not argIter itself or anything after that
      // 2) argIter and everything after begins after thisBegin
      //
      while (argIter!=arg.m.end() && argIter->second <= thisEnd) {        
        m[argIter->first]=argIter->second;
        thisIter++; // we need to advance thisIter every time we add a new interval
        argIter++;
      }
      // now argIter points to the first interval that ends after thisEnd
      // We know for sure that it this interval begins after thisBegin
      // We need to still intersect [thisBegin, thisEnd] with it
      // To preserve the loop invariant of the outer loop, we cannot move argIter any further
      // (because it may overlap with the next interval in m)
      if(argIter!=arg.m.end() && argIter->first<=thisEnd) {
          m[argIter->first]=thisEnd;
          thisIter++; // we need to advance thisIter every time we add a new interval
      }
      thisIter++;
      if (deleteCur) m.erase(thisBegin); // erasure is safe because thisIter has moved forward of the [thisBegin, thisEnd] interval
    }
  }

  
  
  friend ostream& operator<< <T> (ostream& os, const IntervalList<T> & l);
};



template <typename T>
ostream& operator<< (ostream& os, const IntervalList<T> & l)
{
  typename map<T, T>::const_iterator it = l.m.begin();
  if (it==l.m.end()) {
    os << "empty ";
  }
  while (it!=l.m.end()) {
    os << "["<<it->first<<"-" << it->second<<"] ";
    ++it;
  }
  return os;
}


/***** NOTE THAT THIS IS NOT SAFE IF WE GO TO IPv6 ADDRESSES, BECAUSE OF 1<< STUFF ***/
class PrefixRange {
private:
  IntervalList<IPAddr> * lists;
  IPPrefix bound;
  int endLength;
  int len; // len is the length of the array lists

  // Shared code between two of the constructors, note that bound is set in the constructor
  void Setup( int boundingMaxLength ) {
    if (boundingMaxLength<bound.getLength()) {
      lists = NULL;
      len = 0;
      endLength = boundingMaxLength;
      return;
    }
    endLength = boundingMaxLength;
    len = endLength-bound.getLength()+1;
    lists = new IntervalList<IPAddr>[len];
    for (int i = bound.getLength(); i<=endLength; i++) {
      // lists[i-bound.getLengt()] is responsible for storing intervals
      // at level i of the prefix tree, i.e., prefixes that end in /i
      // At this level, to consecutive prefixes differ by 2^(IPAdder::numBits-i)
      // For example, if i==24, then we store /24 prefixes in lists[i-bound.getLength()]
      // Two /24 prefixes are contiguous if they differ by 2^(32-24)=2^8=256
      if ( i == 0 ) lists[i-bound.getLength()].setIncrement(1); // avoiding issues from left shifting by 32
      else lists[i-bound.getLength()].setIncrement(1<<((IPAddr::numBits)-i)); //~erh refactor
    }
  }

public:
  // constructs a prefix range that will always be bounded by boundingPrefix and boundingMaxLength --
  // i.e., in can contain only the boundingPrefix or its subPrefixes of length up to boundingMaxLength
  // If you want an unbounded one, pass in bounding prefix 0f 0/0 and boundingMaxLength of IPAddr::numBits
  PrefixRange(IPPrefix boundingPrefix, int boundingMaxLength) : bound(boundingPrefix) { Setup(boundingMaxLength); }
  PrefixRange() : bound( IPPrefix( IPAddr(0,0,0,0), 0) ) { Setup( IPAddr::numBits ); }
  PrefixRange( const PrefixRange& original ) : bound(original.bound), endLength(original.endLength), len(original.len) {
    
    //copy everything in lists
    lists = new IntervalList<IPAddr>[len]; 
     for (int i = 0; i<len; i++){
       IntervalList<IPAddr> list = IntervalList<IPAddr>( original.lists[i] );
       lists[i] = list;
    }
  }

  inline ~PrefixRange() {
    if (lists!=NULL)
      delete [] lists;
  }
  
  bool empty () {
    for (int i = 0; i<len; i++)
      if (!lists[i].empty())
        return false;
    return true;
  }
  
  void addROA(IPPrefix prefix, int maxLength) {
    int endIndex = min(maxLength, endLength);
    if (prefix.isSubPrefixOf(bound)){
      for (int i=prefix.getLength(); i<=endIndex; i++)
        lists[i-bound.getLength()].add(prefix.getPrefix(), prefix.endAddress(i));
    } else if (bound.isSubPrefixOf(prefix)) {
      for (int i=bound.getLength(); i<=endIndex; i++)
        lists[i-bound.getLength()].add(bound.getPrefix(), bound.endAddress(i));
    }
  }

  void removeROA(IPPrefix prefix, int maxLength) {
    int endIndex = min(maxLength, endLength);
    if (prefix.isSubPrefixOf(bound))
      for (int i=prefix.getLength(); i<=endIndex; i++)
        lists[i-bound.getLength()].remove(prefix.getPrefix(), prefix.endAddress(i));
    else if (bound.isSubPrefixOf(prefix))
      for (int i=bound.getLength(); i<=endIndex; i++)
        lists[i-bound.getLength()].remove(bound.getPrefix(), bound.endAddress(i));
  }
  
  PrefixRange complement() {
    PrefixRange ret(bound, endLength);

    for (int i = bound.getLength(); i<=endLength; i++) { 
      lists[i-bound.getLength()].subtractFromInterval(bound.getPrefix(), bound.endAddress(i), ret.lists[i-bound.getLength()]);
    }
    return ret;
  }

  PrefixRange subtractFromROA(IPPrefix prefix, int maxLength) {
    PrefixRange ret(prefix, maxLength);
    if (prefix.isSubPrefixOf(bound)) {
      int endIndex = min(maxLength, endLength);
      int i;
      for (i=prefix.getLength(); i<=endIndex; i++)
        lists[i-bound.getLength()].subtractFromInterval(prefix.getPrefix(), prefix.endAddress(i), ret.lists[i-prefix.getLength()]);
      for (; i<=maxLength; i++)
        ret.lists[i-prefix.getLength()].add(prefix.getPrefix(), prefix.endAddress(i));
    }
    else if (bound.isSubPrefixOf(prefix)) {
      int i;
      int endIndex = min(bound.getLength()-1, maxLength);
      for (i=prefix.getLength(); i<=endIndex; i++)
        ret.lists[i-prefix.getLength()].add(prefix.getPrefix(), prefix.endAddress(i));
      endIndex = min(endLength, maxLength);
      for (; i<=endIndex; i++)
        lists[i-bound.getLength()].subtractFromInterval(prefix.getPrefix(), prefix.endAddress(i), ret.lists[i-prefix.getLength()]);
      for (; i<=maxLength; i++)
        ret.lists[i-prefix.getLength()].add(prefix.getPrefix(), prefix.endAddress(i));
    }
    else ret.addROA(prefix, maxLength);
    return ret;
  }
  
  
  // denseInstersectWith modifies this, but it leaves arg untouched
  void denseIntersectWith(const PrefixRange & arg, bool argDense) {
    for (int i = max(bound.getLength(), arg.bound.getLength()); i<=min(endLength, arg.endLength); i++)
      lists[i-bound.getLength()].denseIntersectWith(arg.lists[i-arg.bound.getLength()],argDense);
  }

  string toJSON(){
    stringstream ss;
    ss<<"[";
    for (int i = 0; i<len; i++){
      ss << "\"" << lists[i] << "/"<<bound.getLength()+i << "\"";
      if (i!=len-1) ss<<", ";
    }
    ss<<"]";
    return ss.str();
  }

  
  friend ostream& operator<<(ostream& os, const PrefixRange& l);
};

ostream& operator<<(ostream& os, const PrefixRange& l)
{
  for (int i = 0; i<l.len; i++)
    os << l.lists[i] << "/"<<l.bound.getLength()+i<<endl;
  return os;
}


class ROA{
private:
    IPPrefix prefix;
    int maxlen;
    int as;
    string path;
public:
    ROA(int newAS, IPPrefix newPrefix, int newMaxlen) : prefix(newPrefix) {
      as = newAS;
      maxlen = newMaxlen;
    }
    ROA(int newAS, IPPrefix newPrefix, int newMaxlen, string newPath) : prefix(newPrefix) {
      as = newAS;
      maxlen = newMaxlen;
      path = newPath;
    }
    inline IPPrefix getPrefix(){ return prefix; }
    inline int getMaxlen(){ return maxlen; }
    inline int getAS(){ return as; }
    inline string getPath(){ return path; }

    string toJSON(){
      stringstream ss;
      ss<<"{";
      ss<<"\"AS\":"<<getAS()<<", ";
      ss<<"\"PREFIX\":\""<<getPrefix()<<"-"<<getMaxlen()<<"\", ";
      ss<<"\"PATH\":"<<"\""<<getPath()<<"\""; 
      ss<<"}";

      return ss.str();
    }

};
ostream& operator<<(ostream& os, ROA r)
{
  os<<" {AS:"<<r.getAS()<<", Prefix:"<<r.getPrefix()<<"-"<<r.getMaxlen()<<", Path:"<<r.getPath()<<"}";
  return os;
}


class ROAGroup {
  private:
    unordered_map<int, PrefixRange*> AStoROA; 
    PrefixRange* known;
    PrefixRange* unknown;

    set<int> ases;
    bool changed;

    void Setup() {
      AStoROA.clear();    
      known = new PrefixRange(); 
      unknown = (PrefixRange*)NULL;

      changed = true;
    }

    inline void calcUnknown(){ 
      if( unknown != NULL )  delete unknown;

      unknown = new PrefixRange( known->complement() ); 
    }

  public:

    ROAGroup() { 
      Setup();
    }

    ROAGroup(vector<ROA*>* ROAs){
      Setup();
      for (ROA* r : *ROAs){
        addROA(r);
      }
    }

    ROAGroup(const ROAGroup& original){
      
      known = new PrefixRange( *original.known );
      ases = original.ases;

      unordered_map<int, PrefixRange*> origMap =  original.AStoROA;

      for( set<int>::iterator j = ases.begin(); j!= ases.end(); ++j){
        int as = *j;

        AStoROA[as] = new PrefixRange( *origMap[as] );
      }

    }

    void addROA(ROA* roa){
      addROA( roa->getAS(), roa->getPrefix(), roa->getMaxlen() );

    }
    void addROA(int as, IPPrefix prefix, int maxLength){
      // If we don't have a prefixRange setup for this AS then set one up
      if ( ! hasAS(as) ) { 
         AStoROA[as] = new PrefixRange();
         ases.insert(as);
      } 

      AStoROA[as]->addROA(prefix, maxLength);

      // known prefixes includes the invalid shadows of the valid prefixes
      known->addROA(prefix, IPAddr::numBits);

      changed = true;
    }

    inline set<int>* getASes(){ return &ases; }
    inline bool hasAS( int as ){ return AStoROA.find( as ) != AStoROA.end(); }
    inline PrefixRange* getByAS(int as){
      assert( hasAS(as) ); // Check you have the as before requesting it
      return AStoROA[as];
    }

     // We currently do not support this action, please be careful about known when implementing.
    inline void removeByAS(int as){ assert( 0 ); }
    inline const PrefixRange* getKnown(){ return known; }  

    inline const PrefixRange* getUnknown(){  
      if (changed) changed = false; calcUnknown();
      return unknown; 
    } 
    inline bool empty(){ return ases.empty(); }

    inline ~ROAGroup() {
      for( set<int>::iterator j = ases.begin(); j!= ases.end(); ++j){
        int as = *j;
        delete AStoROA[as];
      }
      delete known;
    }
  
};
ostream& operator<<(ostream& os, ROAGroup g)
{
  set<int>* ASes = g.getASes();

  for( set<int>::iterator i = ASes->begin(); i != ASes->end(); ++i){
    int as = *i;
    PrefixRange* r = g.getByAS( as );
    
    os<<"AS:"<<as<<"={"<<endl;
    os<<*r<<endl;
    os<<"}"<<endl;

  }
  return os;
}

class UnknownToInvalidDowngrade{
private:
  ROA roa;
  PrefixRange unknownToknown;
  unordered_map<int, PrefixRange*> exceptions;
public:
  inline UnknownToInvalidDowngrade(ROA newROA, PrefixRange newUnknownToknown) 
    : roa(newROA), unknownToknown(newUnknownToknown), exceptions(){}
  inline PrefixRange* getUnknownToKnown(){ return &unknownToknown; }
  inline unordered_map<int, PrefixRange*>* getExceptions(){ return &exceptions; }   inline void addException(int as, PrefixRange* range){ 
    exceptions.insert( make_pair(as, new PrefixRange( *range ) ) ); 
  }

  inline ROA* getROA(){ return &roa; }
  bool empty(){
    return unknownToknown.empty() && exceptions.empty();
  }
  
  string toJSON(){
    stringstream ss;

    ss<<"{";
    ss<<"\"CHANGE\":\"+U2I\", ";
    ss<<"\"ROA\":"<<getROA()->toJSON()<<", ";      
    ss<<"\"EFFECT\":"<<getUnknownToKnown()->toJSON()<<", "; 
    ss<<"\"EXCEPT\":{";
      for(unordered_map<int,PrefixRange*>::iterator i = getExceptions()->begin(); i != getExceptions()->end(); ++i){
        int as = i->first;
        PrefixRange* exception = i->second;
        if ( i != getExceptions()->begin() ) ss<<",";

        ss<<"\""<<as<<"\":"<<exception->toJSON();
      }
    ss<<"}"; // Closes EXCEPT
    
    ss<<"}"<<endl; // Closes hash
    
    return ss.str();
  }

};

//R, U2I: (*, R.UnknownToKnown) except (AS, R.UnknownToInvalid(AS))
ostream& operator<<(ostream& os, UnknownToInvalidDowngrade g) //LEOCOM: should be a const &, but I can do that until I fix replace range
{

  for(unordered_map<int,PrefixRange*>::iterator i = g.getExceptions()->begin(); i != g.getExceptions()->end(); ++i){
    int as = i->first;
    PrefixRange* exception =  i->second;

    os<<"[";
      os<<"R:{";
        os<<"AS:"<<g.getROA()->getAS();
        os<<",";
        os<<"Prefix:"<<g.getROA()->getPrefix()<<"-"<<g.getROA()->getMaxlen();
      os<<"},";
      os<<"R.UnknownToKnown:{";
        os<<*g.getUnknownToKnown(); 
      os<<"},";
      os<<"Except:{";
        os<<"AS:"<<as;
      os<<",";
        os<<"Prefix:"<<*exception;
      os<<"}";
    os<<"]";
    os<<endl;
  }
  return os;
}


// Note this method returns the valid to unknown prefixes using rOlds
// R.ValidToUnknown = R.valid intersection new.unknown
void ValidToUnknown( ROAGroup* newPrefixes, PrefixRange* rOld){ 
  bool argDense = false; // most likely, unknown has many entries and the intersections have few entries

  const PrefixRange* unknown = newPrefixes->getUnknown(); //calculate this only once if computing a list of rOlds

  rOld->denseIntersectWith( *unknown, argDense );  

}

// R.ValidToInvalid = (R.valid - new.AS.valid) -new.unknown
// Destructive to rOld since it returns the result in rOld
void ValidToInvalid( ROAGroup* newPrefixes, PrefixRange* rOld, int as){
  bool argDense = false;  // most likely, known has many entries and the intersections have few entries

  PrefixRange noLongerValid =  PrefixRange(); 

  if( newPrefixes->hasAS( as ) ) { 

    // ( R.valid -  new.AS.Valid)
    PrefixRange* newASValid = newPrefixes->getByAS(as);
    PrefixRange newASUnknownOrInvalid = newASValid->complement();
    rOld->denseIntersectWith( newASUnknownOrInvalid, argDense); // LEOCOM: would it be more efficent to do a subtraction, ETHANCOM: unfortunately substraction only applies to IPPrefixes which don't have MaxLen

  } // if new.AS.valid doesn't exist then ( R.valid -  new.AS.valid) = (R.valid) and we can skip ahead

  // subtracting new.unknown is the same as intersecting with new.known
  rOld->denseIntersectWith( *(newPrefixes->getKnown()), argDense);
  
}


// This method is not destructive because we pass by value 
void NotValidToValid(ROAGroup newPrefixes, ROAGroup oldPrefixes, unordered_map<int, PrefixRange*>* notValidToValidPrefixes){ 
  set<int>* ASes = newPrefixes.getASes();

  for( set<int>::iterator i = ASes->begin(); i != ASes->end(); ++i) {
    int as = *i;

    PrefixRange* rPrimeValid = newPrefixes.getByAS(as); 

    PrefixRange empty = PrefixRange();

    PrefixRange* oldValid = &empty; 

    // If oldPrefixes has the as we are working on then set it
    if ( oldPrefixes.hasAS(as) ) oldValid = oldPrefixes.getByAS(as); 

    rPrimeValid->denseIntersectWith( oldValid->complement() , true);

    if ( ! rPrimeValid->empty() ) notValidToValidPrefixes->insert( make_pair(as, new PrefixRange( *rPrimeValid ) ) );
  }
}

void UnknownToKnown(const PrefixRange* oldUnknown, ROA rNew, PrefixRange* unknownToKnown){
  bool argDense = true;
  assert(unknownToKnown->empty());

  unknownToKnown->addROA( rNew.getPrefix(), IPAddr::numBits);

  // unknownToKnown =  R.known INTERSECTS old
  unknownToKnown->denseIntersectWith( *oldUnknown, argDense);
}

vector<UnknownToInvalidDowngrade> UnknownToInvalid( ROAGroup* newPrefixes, ROAGroup* oldPrefixes, vector<ROA*>* rNews){
  vector<UnknownToInvalidDowngrade> downgrades;
  
  unordered_map<int, PrefixRange*> notValidToValidPrefixes; // This map is every Prefix which was invalid/unknown in old and is valid in new
  NotValidToValid( *newPrefixes, *oldPrefixes, &notValidToValidPrefixes);

  // Find those AS \in A, where A is all the ASes which created newly valid prefixes.
  // A should just much smaller than the set of all ASes.
  vector<int> A;

  // Save all the keys of notValidToValidPrefixes, done to maintain consistant notion between paper and program. A = notValidToValidPrefixes.keys()
  for(unordered_map<int,PrefixRange*>::iterator i = notValidToValidPrefixes.begin(); i != notValidToValidPrefixes.end(); ++i) A.push_back(i->first);
  
  vector<ROA*> rNewlyValid;
  map<ROA*, PrefixRange*> unknownToKnownByR;
  // Compute all the ROAs that creately newly valid prefixes, since we need to compute 
  // unknown to known to do this lets save the result and reuse it below.

  const PrefixRange* oldUnknown = oldPrefixes->getUnknown(); 

  for( ROA* r: *rNews){  
    PrefixRange* rUnknownToKnown = new PrefixRange(); // Don't free this
    UnknownToKnown( oldUnknown, *r, rUnknownToKnown);

    if( ! rUnknownToKnown->empty() ){
      unknownToKnownByR[r] = rUnknownToKnown;
      rNewlyValid.push_back(r); 
    }
  }

  for (int as: A) {

    PrefixRange complementNotValidToValidPrefixes = notValidToValidPrefixes[as]->complement();

    for( ROA* r: rNewlyValid){      
      PrefixRange* rUnknownToKnown = new PrefixRange( *unknownToKnownByR[r] );

      UnknownToInvalidDowngrade downgrade = UnknownToInvalidDowngrade(*r, *rUnknownToKnown);

      rUnknownToKnown->denseIntersectWith( complementNotValidToValidPrefixes, true);

      PrefixRange* unknownToInvalid = rUnknownToKnown;

      downgrade.addException(as, unknownToInvalid); 

      if ( ! downgrade.empty() ){
        downgrades.push_back(downgrade);
      }
    } // R.New.valid loop
  } // as loop

  //clean up
  for( ROA* r: rNewlyValid) delete unknownToKnownByR[r];

  return downgrades;
}




void OutputDowngrades( vector<ROA*>* oldROAs, vector<ROA*>* newROAs, ostream* output, bool U2I ) {
  vector<PrefixRange*> validToUnknownPrefixes; 


  ROAGroup oldPrefixes = ROAGroup(oldROAs); // old.AS.valid
  ROAGroup newPrefixes = ROAGroup(newROAs); // new.AS.valid

  //valid to invalid move this into method 
  for ( ROA* rOld: *oldROAs){
    int as = rOld->getAS();

    PrefixRange effect = PrefixRange( rOld->getPrefix(), rOld->getMaxlen() ); 
    effect.addROA( rOld->getPrefix(), rOld->getMaxlen() ); 

    ValidToInvalid( &newPrefixes, &effect, as);

    if( ! effect.empty() ) {
      (*output)<<"{";
      (*output)<<"\"CHANGE\":\"-V2I\", ";
      (*output)<<"\"ROA\":"<<rOld->toJSON()<<", ";      
      (*output)<< "\"EFFECT\":"<<effect.toJSON(); 
      (*output)<<"}"<<endl;
    }
  }

  //valid to unknown
  for ( ROA* rOld: *oldROAs){
    PrefixRange effect = PrefixRange( rOld->getPrefix(), rOld->getMaxlen() );
    effect.addROA( rOld->getPrefix(), rOld->getMaxlen() );

    ValidToUnknown( &newPrefixes, &effect);

    if( ! effect.empty() )  {
      (*output)<<"{";
      (*output)<<"\"CHANGE\":\"-V2U\", ";
      (*output)<<"\"ROA\":"<<rOld->toJSON()<<", ";      
      (*output)<< "\"EFFECT\":"<<effect.toJSON(); 
      (*output)<<"}"<<endl;
    }

  }
  
  if( U2I ){
    //unknown to invalid
    vector<UnknownToInvalidDowngrade> unknownToInvalid = UnknownToInvalid( &newPrefixes, &oldPrefixes, newROAs);
      
    for (UnknownToInvalidDowngrade effect:  unknownToInvalid){
      if( ! effect.empty() ) (*output)<<effect.toJSON();
    }
  }

}

