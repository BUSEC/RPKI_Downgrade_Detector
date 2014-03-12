//With some modifications from the stackoverflow answer: http://stackoverflow.com/a/16821417/74359
template<typename T>
std::string toString(T& o)
{
    std::stringstream ss;
    ss << o;
    return ss.str();
}

TEST(DetectorUnitTest, IntervalSetsSimpleTests) {
	IntervalList<IPAddr> l, l2, l3, l4;

	EXPECT_FALSE(l2 != l3);

	//One interval vs. empty
	l2.setToInterval(5, 10);
  	EXPECT_TRUE( l2!=l3 );

	//Two interval vs. empty
  	l2.add(15,20);
	EXPECT_TRUE( l2!=l3 );

	//Two interval vs. one interval with non-matching key
  	l3.add(50,60);
	EXPECT_TRUE( l2!=l3 );

	//two interval vs. three interval with two matching
	l3.add(5, 10);
	l3.add(15,20);
  
  	EXPECT_TRUE( l2!=l3 );

 	//Two interval vs. two interval that match
  	l3.remove(50, 60);

  	EXPECT_FALSE( l2!=l3 );

	//Two interval vs. two interval with one end that doesn't match
  	l3.remove(19, 20);
  	EXPECT_TRUE( l2!=l3  );
}


TEST(DetectorUnitTest, IntervalSetsFullFunctionalityTests) {
	IntervalList<IPAddr> l, l2, l3, l4, lOut;

	//remove from an empty list
	l.remove (5, 10);
	EXPECT_EQ( toString(l), "empty ");

	lOut = l.subtractFromInterval(10,5);
	EXPECT_EQ( toString(lOut), "empty ");

	
	//Complement an empty list within an empty interval
	lOut = l.subtractFromInterval(10,5);
	EXPECT_EQ( toString(lOut), "empty ");


	//complement an empty list within a one-element interval
	lOut = l.subtractFromInterval(200,200);
	EXPECT_EQ( toString(lOut), "[0.0.0.200-0.0.0.200] ");

	//complement an empty list within a multi-element interval
	lOut = l.subtractFromInterval(100,200);
	EXPECT_EQ( toString(lOut), "[0.0.0.100-0.0.0.200] ");

	//1 interval tests"
	l.setToInterval(10, 100); 
	//set to a single interval
	EXPECT_EQ( toString(l), "[0.0.0.10-0.0.0.100] ");


	//complement a one-interval list within an empty interval
  	lOut = l.subtractFromInterval(10,5);
 	EXPECT_EQ( toString(lOut), "empty ");

 	//complement a one-interval list within a nonnoverlapping interval
 	////dsfsfsdfsfsdf" << l.subtractFromInterval(150,200) << endl;
  	lOut = l.subtractFromInterval(150,200);
  	EXPECT_EQ( toString(lOut), "[0.0.0.150-0.0.0.200] ");
  	
  	//complement a one-interval list within a fully encompassed interval
 	lOut = l.subtractFromInterval(50,70);
 	EXPECT_EQ( toString(lOut), "empty ");


  	//complement a one-interval list within a fully encompassing interval
  	lOut = l.subtractFromInterval(5,150);
  	EXPECT_EQ( toString(lOut), "[0.0.0.5-0.0.0.9] [0.0.0.101-0.0.0.150] ");

  	//complement a one-interval list within an interval matching the right boundary and overlapping the left;
  	lOut = l.subtractFromInterval(5,100);
  	EXPECT_EQ( toString(lOut), "[0.0.0.5-0.0.0.9] ");
	
	//complement a one-interval list within an interval starting in the middle and overlapping the right boundary
  	lOut = l.subtractFromInterval(50,150);
  	EXPECT_EQ( toString(lOut), "[0.0.0.101-0.0.0.150] ");

	//complement a one-interval list within an interval starting in the middle and overlapping the left boundary
  	lOut = l.subtractFromInterval(5,50);
  	EXPECT_EQ( toString(lOut), "[0.0.0.5-0.0.0.9] ");
   	
   	//remove above a single interval -- shouldn't change
  	l.remove(101, 150);
	EXPECT_EQ( toString(l), "[0.0.0.10-0.0.0.100] "); 

	//EXPECT_EQ( toString(l), "x "); 

	//remove a single element from top of single interval, overlapping its boundary
	l.remove(100, 150); 
	EXPECT_EQ( toString(l), "[0.0.0.10-0.0.0.99] ");

	l.remove(90, 150); //remove a many elements from top of single interval, overlapping its boundary;
	EXPECT_EQ( toString(l), "[0.0.0.10-0.0.0.89] ");
	l.remove(89, 89); //remove a exactly single element from top of single interval;
	EXPECT_EQ( toString(l), "[0.0.0.10-0.0.0.88] ");
	l.remove(1, 9); //remove below a single interval -- shouldn't change;
	EXPECT_EQ( toString(l), "[0.0.0.10-0.0.0.88] ");	
	l.remove(1, 10); //remove a single element from top of single interval, overlapping its boundary;
	EXPECT_EQ( toString(l), "[0.0.0.11-0.0.0.88] ");	
	l.remove(1, 20); //remove many elements from top of single interval, overlapping its boundary;
	EXPECT_EQ( toString(l), "[0.0.0.21-0.0.0.88] ");
	l.remove(21, 21); //remove exactly a single element from top of a single interval interval;
	EXPECT_EQ( toString(l), "[0.0.0.22-0.0.0.88] ");
	l.remove(60, 59); //remove from an empty interval from middle of a single interval -- shouldn't change;  
	EXPECT_EQ( toString(l), "[0.0.0.22-0.0.0.88] ");
	l.remove(60, 70); //remove from a middle of a single interval;
	EXPECT_EQ( toString(l), "[0.0.0.22-0.0.0.59] [0.0.0.71-0.0.0.88] ");

	//2 interval tests"
	l.remove(55, 75); //remove with overlap of two intervals;
	EXPECT_EQ( toString(l), "[0.0.0.22-0.0.0.54] [0.0.0.76-0.0.0.88] ");
	l.remove(55, 76); //remove with overlap of one interval on right;
	EXPECT_EQ( toString(l), "[0.0.0.22-0.0.0.54] [0.0.0.77-0.0.0.88] ");
	l.remove(54, 76); //remove with overlap of one interval on left;
	EXPECT_EQ( toString(l), "[0.0.0.22-0.0.0.53] [0.0.0.77-0.0.0.88] ");
	l.remove(55, 75); //remove from middle region that contains nothing -- shouldn't change;
	EXPECT_EQ( toString(l), "[0.0.0.22-0.0.0.53] [0.0.0.77-0.0.0.88] ");
	l.remove(120, 160); //remove above uppermost interval not touching anything -- shouldn't change;
	EXPECT_EQ( toString(l), "[0.0.0.22-0.0.0.53] [0.0.0.77-0.0.0.88] ");
	l.remove(89, 160); //remove above uppermost interval right up against its boundary -- shouldn't change;
	EXPECT_EQ( toString(l), "[0.0.0.22-0.0.0.53] [0.0.0.77-0.0.0.88] ");
	l.remove(88, 160); //remove single element at the top of uppermost interval;
	EXPECT_EQ( toString(l), "[0.0.0.22-0.0.0.53] [0.0.0.77-0.0.0.87] ");
	l.remove(1, 5); //remove below lowermost interval not touching anything -- shouldn't change;
	EXPECT_EQ( toString(l), "[0.0.0.22-0.0.0.53] [0.0.0.77-0.0.0.87] ");
	l.remove(1, 21); //remove below uppermost interval right up against its boundary -- shouldn't change;
	EXPECT_EQ( toString(l), "[0.0.0.22-0.0.0.53] [0.0.0.77-0.0.0.87] ");
	l.remove(22, 22); //remove single element below lowermost interval; 
	EXPECT_EQ( toString(l), "[0.0.0.23-0.0.0.53] [0.0.0.77-0.0.0.87] ");
	l.remove(30, 30);  //remove single element in the middle of an interval; get 3 intervals;
	EXPECT_EQ( toString(l), "[0.0.0.23-0.0.0.29] [0.0.0.31-0.0.0.53] [0.0.0.77-0.0.0.87] ");

	//make lots of intervals tests
	l.remove(40, 42);  //get 4 intervals;
	EXPECT_EQ( toString(l), "[0.0.0.23-0.0.0.29] [0.0.0.31-0.0.0.39] [0.0.0.43-0.0.0.53] [0.0.0.77-0.0.0.87] ");
	l.remove(25, 26);  //get 5 intervals;
	EXPECT_EQ( toString(l), "[0.0.0.23-0.0.0.24] [0.0.0.27-0.0.0.29] [0.0.0.31-0.0.0.39] [0.0.0.43-0.0.0.53] [0.0.0.77-0.0.0.87] ");
	l.remove(35, 36);  //get 6 intervals;
	EXPECT_EQ( toString(l), "[0.0.0.23-0.0.0.24] [0.0.0.27-0.0.0.29] [0.0.0.31-0.0.0.34] [0.0.0.37-0.0.0.39] [0.0.0.43-0.0.0.53] [0.0.0.77-0.0.0.87] ");
	l.remove(45, 46);  //get 7 intervals;
	EXPECT_EQ( toString(l), "[0.0.0.23-0.0.0.24] [0.0.0.27-0.0.0.29] [0.0.0.31-0.0.0.34] [0.0.0.37-0.0.0.39] [0.0.0.43-0.0.0.44] [0.0.0.47-0.0.0.53] [0.0.0.77-0.0.0.87] ");
	l.remove(79, 80);  //get 8 intervals;
	EXPECT_EQ( toString(l), "[0.0.0.23-0.0.0.24] [0.0.0.27-0.0.0.29] [0.0.0.31-0.0.0.34] [0.0.0.37-0.0.0.39] [0.0.0.43-0.0.0.44] [0.0.0.47-0.0.0.53] [0.0.0.77-0.0.0.78] [0.0.0.81-0.0.0.87] ");
	l.remove(84, 85);  //get 9 intervals;
	EXPECT_EQ( toString(l), "[0.0.0.23-0.0.0.24] [0.0.0.27-0.0.0.29] [0.0.0.31-0.0.0.34] [0.0.0.37-0.0.0.39] [0.0.0.43-0.0.0.44] [0.0.0.47-0.0.0.53] [0.0.0.77-0.0.0.78] [0.0.0.81-0.0.0.83] [0.0.0.86-0.0.0.87] ");

	//remove one whole interval and portions of an interval on either side;
	l.remove(39, 49);  // 8 intervals left;
	EXPECT_EQ( toString(l), "[0.0.0.23-0.0.0.24] [0.0.0.27-0.0.0.29] [0.0.0.31-0.0.0.34] [0.0.0.37-0.0.0.38] [0.0.0.50-0.0.0.53] [0.0.0.77-0.0.0.78] [0.0.0.81-0.0.0.83] [0.0.0.86-0.0.0.87] ");
	//"remove uppermost interval (overlapping its right boundary) and portion of an interval below it;
	l.remove(83, 90);  // 7 intervals left;
	EXPECT_EQ( toString(l), "[0.0.0.23-0.0.0.24] [0.0.0.27-0.0.0.29] [0.0.0.31-0.0.0.34] [0.0.0.37-0.0.0.38] [0.0.0.50-0.0.0.53] [0.0.0.77-0.0.0.78] [0.0.0.81-0.0.0.82] ");
	//remove uppermost interval (exactly up to its right boundary) and portion of an interval below it;
	l.remove(78, 82);  // 6 intervals left;
	EXPECT_EQ( toString(l), "[0.0.0.23-0.0.0.24] [0.0.0.27-0.0.0.29] [0.0.0.31-0.0.0.34] [0.0.0.37-0.0.0.38] [0.0.0.50-0.0.0.53] [0.0.0.77-0.0.0.77] ");
	//remove lowermost interval (overlapping its left boundary) and portion of an interval above it;
	l.remove(20, 27);  // 5 intervals left;
	EXPECT_EQ( toString(l), "[0.0.0.28-0.0.0.29] [0.0.0.31-0.0.0.34] [0.0.0.37-0.0.0.38] [0.0.0.50-0.0.0.53] [0.0.0.77-0.0.0.77] ");
	//remove lowermost interval (exactly up to its left boundary) and portion of an interval above it;
	l.remove(28, 31);  // 4 intervals left;
	EXPECT_EQ( toString(l), "[0.0.0.32-0.0.0.34] [0.0.0.37-0.0.0.38] [0.0.0.50-0.0.0.53] [0.0.0.77-0.0.0.77] ");


	//Complement tests\n";
	//complement a four-interval list within an empty interval"<<endl;
	lOut = l.subtractFromInterval(10,5);
	EXPECT_EQ( toString(lOut), "empty ");
	//complement a four-interval list within a nonnoverlapping interval
	lOut = l.subtractFromInterval(150,200);
	EXPECT_EQ( toString(lOut), "[0.0.0.150-0.0.0.200] ");	
	//complement a four-interval list within a fully encompassed interval
	lOut = l.subtractFromInterval(51,52);
	EXPECT_EQ( toString(lOut), "empty ");	
	//complement a four-interval list within an interval that overlaps just a single empty range in the middle
	lOut = l.subtractFromInterval(60,70);
	EXPECT_EQ( toString(lOut), "[0.0.0.60-0.0.0.70] ");	
	//complement a four-interval list within an interval whose boundaries are in empty ranges in the middle
	lOut = l.subtractFromInterval(36,70);
	EXPECT_EQ( toString(lOut), "[0.0.0.36-0.0.0.36] [0.0.0.39-0.0.0.49] [0.0.0.54-0.0.0.70] ");	
	//complement a four-interval list within an interval whose boundaries are in empty ranges in the middle, right up against empty range outer borders
	lOut = l.subtractFromInterval(35,76);
	EXPECT_EQ( toString(lOut), "[0.0.0.35-0.0.0.36] [0.0.0.39-0.0.0.49] [0.0.0.54-0.0.0.76] ");	
	//complement a four-interval list within an interval whose boundaries are in empty ranges in the middle, right up against empty range inner borders
	lOut = l.subtractFromInterval(36,54);
	EXPECT_EQ( toString(lOut), "[0.0.0.36-0.0.0.36] [0.0.0.39-0.0.0.49] [0.0.0.54-0.0.0.54] ");	
	//complement a four-interval list within an interval whose boundaries are in middles of intervals
	lOut = l.subtractFromInterval(33,52);
	EXPECT_EQ( toString(lOut), "[0.0.0.35-0.0.0.36] [0.0.0.39-0.0.0.49] ");	
	//complement a four-interval list within an interval whose boundaries are inside intervals, against outer borders
	lOut = l.subtractFromInterval(32,53);
	EXPECT_EQ( toString(lOut), "[0.0.0.35-0.0.0.36] [0.0.0.39-0.0.0.49] ");	
	//complement a four-interval list within an interval whose boundaries are inside intervals, against inner borders
	lOut = l.subtractFromInterval(34,50);
	EXPECT_EQ( toString(lOut), "[0.0.0.35-0.0.0.36] [0.0.0.39-0.0.0.49] ");	
	//complement a four-interval list within a fully encompassing interval
	lOut = l.subtractFromInterval(5,150);
	EXPECT_EQ( toString(lOut), "[0.0.0.5-0.0.0.31] [0.0.0.35-0.0.0.36] [0.0.0.39-0.0.0.49] [0.0.0.54-0.0.0.76] [0.0.0.78-0.0.0.150] ");	
	//complement a four-interval list within an interval matching the left boundary and overlapping the right
	lOut = l.subtractFromInterval(32,150);
	EXPECT_EQ( toString(lOut), "[0.0.0.35-0.0.0.36] [0.0.0.39-0.0.0.49] [0.0.0.54-0.0.0.76] [0.0.0.78-0.0.0.150] ");	
	//complement a four-interval list within an interval matching the right boundary and overlapping the left
	lOut = l.subtractFromInterval(5,77);
	EXPECT_EQ( toString(lOut), "[0.0.0.5-0.0.0.31] [0.0.0.35-0.0.0.36] [0.0.0.39-0.0.0.49] [0.0.0.54-0.0.0.76] ");	
	//complement a four-interval list within an interval starting in the middle and overlapping the right boundary
	lOut = l.subtractFromInterval(33,150);
	EXPECT_EQ( toString(lOut), "[0.0.0.35-0.0.0.36] [0.0.0.39-0.0.0.49] [0.0.0.54-0.0.0.76] [0.0.0.78-0.0.0.150] ");	
	//complement a four-interval list within an interval starting in the middle and overlapping the left boundary
	lOut = l.subtractFromInterval(5,52);
	EXPECT_EQ( toString(lOut), "[0.0.0.5-0.0.0.31] [0.0.0.35-0.0.0.36] [0.0.0.39-0.0.0.49] ");

	//Addition tests
	//remove all that's left"<<endl;
	l.remove(1, 100); 

	//Add an empty interval to an empty list
	l.add (10, 5);
	EXPECT_EQ( toString(l), "empty ");

	//Add an nonempty interval to an empty list
	l.add (50, 60);
	EXPECT_EQ( toString(l), "[0.0.0.50-0.0.0.60] ");
	//Add an interval contained within the given interval -- shouldn't change
	l.add (54, 58);
	EXPECT_EQ( toString(l), "[0.0.0.50-0.0.0.60] ");
	//Add an interval exactly equal to the given interval -- shouldn't change
	l.add (50, 60);
	EXPECT_EQ( toString(l), "[0.0.0.50-0.0.0.60] ");

	//Add an interval from middle of the given interval to beyond its right border
	l.add (55, 65);
	EXPECT_EQ( toString(l), "[0.0.0.50-0.0.0.65] ");

	//Add an interval from middle of the given interval to beyond its left border
	l.add (45, 55);
	EXPECT_EQ( toString(l), "[0.0.0.45-0.0.0.65] ");

	//Add an interval containing the current interval
	l.add (40, 70);
	EXPECT_EQ( toString(l), "[0.0.0.40-0.0.0.70] ");  

	//Add an interval adjacent to the current interval on right
	l.add (71, 80);
	EXPECT_EQ( toString(l), "[0.0.0.40-0.0.0.80] ");

	//Add an interval adjacent to the current interval on left
	l.add (30, 39);
	EXPECT_EQ( toString(l), "[0.0.0.30-0.0.0.80] ");

	//Add an interval matching the current interval on left and extending to right
	l.add (30, 85);
	EXPECT_EQ( toString(l), "[0.0.0.30-0.0.0.85] ");
	//Add an interval matching the current interval on right and extending on left
	l.add (25, 85);
	EXPECT_EQ( toString(l), "[0.0.0.25-0.0.0.85] ");

	//Add an interval separated, on the right
	l.add (120, 150);
	EXPECT_EQ( toString(l), "[0.0.0.25-0.0.0.85] [0.0.0.120-0.0.0.150] ");

	//Add an interval between two separated ones
	l.add (110, 115);
	EXPECT_EQ( toString(l), "[0.0.0.25-0.0.0.85] [0.0.0.110-0.0.0.115] [0.0.0.120-0.0.0.150] ");
  
	//Merge two intervals via addition of an interval that overlaps both"<<endl;
	l.add (112, 125);
	EXPECT_EQ( toString(l), "[0.0.0.25-0.0.0.85] [0.0.0.110-0.0.0.150] ");
	//Split again"<<endl;
	l.remove (116, 119);
	EXPECT_EQ( toString(l), "[0.0.0.25-0.0.0.85] [0.0.0.110-0.0.0.115] [0.0.0.120-0.0.0.150] ");

	//Merge two intervals via addition of an interval that is exactly between both"<<endl;
	l.add (116, 119);
	EXPECT_EQ( toString(l), "[0.0.0.25-0.0.0.85] [0.0.0.110-0.0.0.150] ");
	//Split again"<<endl;
	l.remove (116, 119);
	EXPECT_EQ( toString(l), "[0.0.0.25-0.0.0.85] [0.0.0.110-0.0.0.115] [0.0.0.120-0.0.0.150] ");

	//Merge two intervals via addition of an interval that starts after left and overlaps right"<<endl;
	l.add (116, 125);
	EXPECT_EQ( toString(l), "[0.0.0.25-0.0.0.85] [0.0.0.110-0.0.0.150] ");
	//Split again"<<endl;
	l.remove (116, 119);
	EXPECT_EQ( toString(l), "[0.0.0.25-0.0.0.85] [0.0.0.110-0.0.0.115] [0.0.0.120-0.0.0.150] ");

	//Merge two intervals via addition of an interval that overlaps left and goes exactly up to right"<<endl;
	l.add (110, 119);
	EXPECT_EQ( toString(l), "[0.0.0.25-0.0.0.85] [0.0.0.110-0.0.0.150] ");
	//Split again"<<endl;
	l.remove (116, 119);
	EXPECT_EQ( toString(l), "[0.0.0.25-0.0.0.85] [0.0.0.110-0.0.0.115] [0.0.0.120-0.0.0.150] ");


	//Add an interval separated, leftmost"<<endl;
	l.add (15, 20);
	EXPECT_EQ( toString(l), "[0.0.0.15-0.0.0.20] [0.0.0.25-0.0.0.85] [0.0.0.110-0.0.0.115] [0.0.0.120-0.0.0.150] ");

	//Add an interval that spans from middle of leftmost to middle of rightmost"<<endl;
	l.add (17, 130);
	EXPECT_EQ( toString(l), "[0.0.0.15-0.0.0.150] ");
	//Split again"<<endl;
	l.remove (21, 24);
	l.remove (86, 109);
	l.remove (116, 119);
	EXPECT_EQ( toString(l), "[0.0.0.15-0.0.0.20] [0.0.0.25-0.0.0.85] [0.0.0.110-0.0.0.115] [0.0.0.120-0.0.0.150] ");

	//Add an interval that spans from middle of leftmost past end of rightmost"<<endl;
	l.add (17, 170);
	EXPECT_EQ( toString(l), "[0.0.0.15-0.0.0.170] ");
	//Split again"<<endl;
	l.remove (21, 24);
	l.remove (86, 109);
	l.remove (116, 119);
	EXPECT_EQ( toString(l), "[0.0.0.15-0.0.0.20] [0.0.0.25-0.0.0.85] [0.0.0.110-0.0.0.115] [0.0.0.120-0.0.0.170] ");

	//Add an interval that spans from middle of leftmost to exact end of rightmost"<<endl;
	l.add (17, 170);
	EXPECT_EQ( toString(l), "[0.0.0.15-0.0.0.170] ");
	//Split again"<<endl;
	l.remove (21, 24);
	l.remove (86, 109);
	l.remove (116, 119);
	EXPECT_EQ( toString(l), "[0.0.0.15-0.0.0.20] [0.0.0.25-0.0.0.85] [0.0.0.110-0.0.0.115] [0.0.0.120-0.0.0.170] ");

	//Add an interval that spans from middle of leftmost to right up to rightmost"<<endl;
	l.add (17, 119);
	EXPECT_EQ( toString(l), "[0.0.0.15-0.0.0.170] ");
	//Split again"<<endl;
	l.remove (21, 24);
	l.remove (86, 109);
	l.remove (116, 119);
	EXPECT_EQ( toString(l), "[0.0.0.15-0.0.0.20] [0.0.0.25-0.0.0.85] [0.0.0.110-0.0.0.115] [0.0.0.120-0.0.0.170] ");

	//Add an interval that spans from middle of leftmost to before rightmost"<<endl;
	l.add (17, 117);
	EXPECT_EQ( toString(l), "[0.0.0.15-0.0.0.117] [0.0.0.120-0.0.0.170] ");
	//Split again"<<endl;
	l.remove (21, 24);
	l.remove (86, 109);
	l.remove (116, 119);
	EXPECT_EQ( toString(l), "[0.0.0.15-0.0.0.20] [0.0.0.25-0.0.0.85] [0.0.0.110-0.0.0.115] [0.0.0.120-0.0.0.170] ");

	//Add an interval that spans from before begin of leftmost to middle of rightmost"<<endl;
	l.add (10, 160);
	EXPECT_EQ( toString(l), "[0.0.0.10-0.0.0.170] ");
	//Split again"<<endl;
	l.remove (21, 24);
	l.remove (86, 109);
	l.remove (116, 119);
	EXPECT_EQ( toString(l), "[0.0.0.10-0.0.0.20] [0.0.0.25-0.0.0.85] [0.0.0.110-0.0.0.115] [0.0.0.120-0.0.0.170] ");

	//Add an interval that spans from exact begin of leftmost to middle of rightmost"<<endl;
	l.add (10, 160);
	EXPECT_EQ( toString(l), "[0.0.0.10-0.0.0.170] ");
	//Split again"<<endl;
	l.remove (21, 24);
	l.remove (86, 109);
	l.remove (116, 119);
	EXPECT_EQ( toString(l), "[0.0.0.10-0.0.0.20] [0.0.0.25-0.0.0.85] [0.0.0.110-0.0.0.115] [0.0.0.120-0.0.0.170] ");

	//Add an interval that spans from right after leftmost to middle of rightmost"<<endl;
	l.add (21, 160);
	EXPECT_EQ( toString(l), "[0.0.0.10-0.0.0.170] ");
	//Split again"<<endl;
	l.remove (21, 24);
	l.remove (86, 109);
	l.remove (116, 119);
	EXPECT_EQ( toString(l), "[0.0.0.10-0.0.0.20] [0.0.0.25-0.0.0.85] [0.0.0.110-0.0.0.115] [0.0.0.120-0.0.0.170] ");

	//Add an interval that spans from after leftmost to middle of rightmost"<<endl;
	l.add (23, 160);
	EXPECT_EQ( toString(l), "[0.0.0.10-0.0.0.20] [0.0.0.23-0.0.0.170] ");
	//Split again"<<endl;
	l.remove (21, 24);
	l.remove (86, 109);
	l.remove (116, 119);
	EXPECT_EQ( toString(l), "[0.0.0.10-0.0.0.20] [0.0.0.25-0.0.0.85] [0.0.0.110-0.0.0.115] [0.0.0.120-0.0.0.170] ");  

	//Intersection tests; each, except symmetric ones, will be run four times, switching this, arg, true, and false" << endl;
	l3.clear(); // make l3 empty

	//Intersect two empty intervals" << endl;
	l.setToInterval(10,5); 
	l2.setToInterval(10, 5);
	l.denseIntersectWith(l2, true);
	EXPECT_FALSE(l!=l3);

	l.setToInterval(10,5); 
	l.denseIntersectWith(l2, false);
	EXPECT_FALSE(l!=l3);

	//Intersect empty with single-interval" << endl;
	l.setToInterval(10,5); 
	l2.setToInterval(5, 10);
	l.denseIntersectWith(l2, true);
	EXPECT_FALSE(l!=l3);

	l.setToInterval(10,5); 
	l2.setToInterval(5, 10);
	l.denseIntersectWith(l2, false);
	EXPECT_FALSE(l!=l3);

	l2.setToInterval(10,5); 
	l.setToInterval(5, 10);
	l.denseIntersectWith(l2, true);
	EXPECT_FALSE(l!=l3);

	l2.setToInterval(10,5); 
	l.setToInterval(5, 10);
	l.denseIntersectWith(l2, false);
	EXPECT_FALSE(l!=l3);

	//Intersect empty with two-interval" << endl;
	l.setToInterval(10,5); 
	l2.setToInterval(5, 10);
	l2.add(30, 40);
	l.denseIntersectWith(l2, true);
	EXPECT_FALSE(l!=l3);

	l.setToInterval(10,5); 
	l2.setToInterval(5, 10);
	l2.add(30, 40);
	l.denseIntersectWith(l2, false);
	EXPECT_FALSE(l!=l3);

	l2.setToInterval(10,5); 
	l.setToInterval(5, 10);
	l.add(30, 40);
	l.denseIntersectWith(l2, true);
	EXPECT_FALSE(l!=l3);

	l2.setToInterval(10,5); 
	l.setToInterval(5, 10);
	l.add(30, 40);
	l.denseIntersectWith(l2, false);
	EXPECT_FALSE(l!=l3);

	//Intersect two nonoverlapping intervals" << endl;
	l.setToInterval(5,10); 
	l2.setToInterval(50,100);
	l.denseIntersectWith(l2, true);
	EXPECT_FALSE(l!=l3);

	l.setToInterval(5,10); 
	l.denseIntersectWith(l2, false);
	EXPECT_FALSE(l!=l3);

	l.setToInterval(50,100); 
	l2.setToInterval(5,10);
	l.denseIntersectWith(l2, true);
	EXPECT_FALSE(l!=l3);

	l.setToInterval(50,100); 
	l.denseIntersectWith(l2, false);  
	EXPECT_FALSE(l!=l3);


	//Intersect [10,20] ";
	//with [5,205]" << endl;
	l3.setToInterval (10, 20);
	l.setToInterval(10,20);
	l2.setToInterval(5,205);
	l.denseIntersectWith(l2, true);
	EXPECT_FALSE(l!=l3);

	l.setToInterval(10,20);
	l2.setToInterval(5,205);
	l.denseIntersectWith(l2, false);
	EXPECT_FALSE(l!=l3);

	l2.setToInterval(10,20);
	l.setToInterval(5,205);
	l.denseIntersectWith(l2, true);
	EXPECT_FALSE(l!=l3);

	l2.setToInterval(10,20);
	l.setToInterval(5,205);
	l.denseIntersectWith(l2, false);
	EXPECT_FALSE(l!=l3);
	

	//Intersect [10,20] ";
	//with [5,15]" << endl;
	l.setToInterval(10,20);
	l2.setToInterval(5,15);
	l3.setToInterval(10,15);
	l.denseIntersectWith(l2, true);
	EXPECT_FALSE(l!=l3);
	
	l.setToInterval(10,20);
	l2.setToInterval(5,15);
	l.denseIntersectWith(l2, false);
	EXPECT_FALSE(l!=l3);
	
	l2.setToInterval(10,20);
	l.setToInterval(5,15);
	l.denseIntersectWith(l2, true);
	EXPECT_FALSE(l!=l3);
	
	l2.setToInterval(10,20);
	l.setToInterval(5,15);
	l.denseIntersectWith(l2, false);
	EXPECT_FALSE(l!=l3);
	


	//Intersect [10,20] [30,40] ... [90,100] ";
	//with [5,205]"<<endl;
	l.setToInterval(10,20);
	for (int i=30; i<100; i+=20)
	l.add(i, i+10);
	l2.setToInterval(5,205);
	l3.setToInterval(10,20);
	for (int i=30; i<100; i+=20)
	l3.add(i, i+10);
	l.denseIntersectWith(l2, true);
	EXPECT_FALSE(l!=l3);
	
	l.setToInterval(10,20);
	for (int i=30; i<100; i+=20)
	l.add(i, i+10);
	l2.setToInterval(5,205);
	l.denseIntersectWith(l2, false);
	EXPECT_FALSE(l!=l3);
	
	l2.setToInterval(10,20);
	for (int i=30; i<100; i+=20)
	l2.add(i, i+10);
	l.setToInterval(5,205);
	l.denseIntersectWith(l2, true);
	EXPECT_FALSE(l!=l3);
	
	l2.setToInterval(10,20);
	for (int i=30; i<100; i+=20)
	l2.add(i, i+10);
	l.setToInterval(5,205);
	l.denseIntersectWith(l2, false);
	EXPECT_FALSE(l!=l3);
	


	//Intersect [10,20] [30,40] ... [90,100] ";
	//with [15,95]"<<endl;
	l.setToInterval(10,20);
	for (int i=30; i<100; i+=20)
	l.add(i, i+10);
	l2.setToInterval(15,95);
	l3.setToInterval(15,20);
	for (int i=30; i<90; i+=20)
	l3.add(i, i+10);
	l3.add(90, 95);
	l.denseIntersectWith(l2, true);
	EXPECT_FALSE(l!=l3);
	
	l.setToInterval(10,20);
	for (int i=30; i<100; i+=20)
	l.add(i, i+10);
	l2.setToInterval(15,95);
	l.denseIntersectWith(l2, false);
	EXPECT_FALSE(l!=l3);
	
	l2.setToInterval(10,20);
	for (int i=30; i<100; i+=20)
	l2.add(i, i+10);
	l.setToInterval(15,95);
	l.denseIntersectWith(l2, true);
	EXPECT_FALSE(l!=l3);
	
	l2.setToInterval(10,20);
	for (int i=30; i<100; i+=20)
	l2.add(i, i+10);
	l.setToInterval(15,95);
	l.denseIntersectWith(l2, false);
	EXPECT_FALSE(l!=l3);
	

	//Intersect [10,20] [30,40] ... [90,100] ";
	//with [35,75]"<<endl;
	l.setToInterval(10,20);
	for (int i=30; i<100; i+=20)
	l.add(i, i+10);
	l2.setToInterval(35,75);
	l3.setToInterval(35,40);
	l3.add(50, 60);
	l3.add(70, 75);
	l.denseIntersectWith(l2, true);
	EXPECT_FALSE(l!=l3);
	
	l.setToInterval(10,20);
	for (int i=30; i<100; i+=20)
	l.add(i, i+10);
	l2.setToInterval(35,75);
	l.denseIntersectWith(l2, false);
	EXPECT_FALSE(l!=l3);
	
	l2.setToInterval(10,20);
	for (int i=30; i<100; i+=20)
	l2.add(i, i+10);
	l.setToInterval(35,75);
	l.denseIntersectWith(l2, true);
	EXPECT_FALSE(l!=l3);
	
	l2.setToInterval(10,20);
	for (int i=30; i<100; i+=20)
	l2.add(i, i+10);
	l.setToInterval(35,75);
	l.denseIntersectWith(l2, false);
	EXPECT_FALSE(l!=l3);
	

	// We want to try all kinds of intersections.
	// When two intervals x and y intersect, beginning of x can be inside, equal to, or outside beginning of y. (We will call these i, e, o).
	// Same for end of x. Moreover, when beginning of x is outside y, we will consider whether it is immediately outside y (o0), or inside (o1)
	// an interval that precedes y, or farther and not inside an interval (o2), or farther and inside another interval (o3).
	// This gives six possible begins and six possible ends, for 36 possible intersections. All 36 are exercised below.
	// Furthermore, we want to have the same possible combinations of end of one interval and beginning of the next,
	// which are exercised also all exercised by intersecting with the complement.

	//Intersect [10,20] ... [970,1180]" << endl;
	//with x= [15,55] (io3 for y=[10,20],o1o1 for y=[30,40], o3i for y=[50,60])"<<endl;
	//        [70,80] (ee) [93,97] (ii)"<< endl;
	//        [105,115] (o0i) [125,155] "<<endl;
	//        (o0o1 for y=[130,140] and o2i for y=[150,160])"<<endl;
	//        [162,163]  [164,165] [167,168] (no intersection)"<<endl;
	//        [175,205] (io2 for y=[170,180] and o1o0 for y=[190,200]" <<endl;
	//        [210, 215] (ei) [235,240] (ie)"<<endl;
	//        [255,275] (io1 for y=[250,260] and o1i for y=[270,280])"<<endl;
	//        [305,385] (o0o2 and o2o0 and o2o2)"<<endl;
	//        [415,495] (o1o3 and o3o1 and o3o3)"<<endl;
	//        [510,530] (e01) [550,581] (e02) [590,630] (e03)"<<endl;
	//        [645,695] (o0o3 and o2o1) [715,765] (o1o2 and o3o0)"<<endl;
	//        [775,800] (o1e) [805,840] (o2e) [855,900] (o3e)" << endl;
	//        [905,975] (o2o3) [1015,1085] (o3o2)"<<endl;
	//        [1087,1102] (o0o0) [1115, 1125] (io0) [1130,1145] (eo0) [1147,1160] (o0e)"<<endl;

 
	l3.clear();
	l3.add(15,20);
	l3.add(30,40);
	l3.add(50,55);
	l3.add(70,80);
	l3.add(93,97);
	l3.add(110,115);
	l3.add(130,140);
	l3.add(150,155);
	l3.add(175,180);
	l3.add(190,200);
	l3.add(210, 215);
	l3.add(235,240);
	l3.add(255, 260);
	l3.add(270, 275);
	for (int i=310; i<380; i+=20)
		l3.add(i, i+10);
	l3.add(415,420);
	for (int i=430; i<480; i+=20)
		l3.add(i, i+10);
	l3.add(490,495);
	l3.add(510,520);
	l3.add (530,530);
	l3.add (550,560);
	l3.add (570,580);
	l3.add (590,600);
	l3.add (610,620);
	l3.add (630,630);
	l3.add (650,660);
	l3.add (670,680);
	l3.add (690,695);
	l3.add (715,720);
	l3.add (730,740);
	l3.add (750,760);
	l3.add (775,780);
	l3.add (790,800);
	l3.add (810,820);
	l3.add (830,840);
	l3.add (855,860);
	l3.add (870,880);
	l3.add (890,900);
	for (int i=910; i<960; i+=20)
		l3.add (i, i+10);
	l3.add (970,975);
	l3.add(1015,1020);
	for (int i=1030; i<1100; i+=20)
		l3.add (i, i+10);
	l3.add(1115,1120);
	l3.add(1130,1140);
	l3.add(1150,1160);


	l.setToInterval(10,20);
	for (int i=30; i<1180; i+=20)
		l.add(i, i+10);
	l2.setToInterval(15,55);
	l2.add(70,80);
	l2.add(93,97);
	l2.add(105,115);
	l2.add(125,155);
	l2.add(162, 163);
	l2.add(164, 165);
	l2.add(167, 168);
	l2.add(175,205);
	l2.add(210,215);
	l2.add(235,240);
	l2.add(255,275);
	l2.add(305,385);
	l2.add(415,495);
	l2.add(510,530);
	l2.add(550,581);
	l2.add(590,630);  
	l2.add(645,695);
	l2.add(715,765);
	l2.add(775,800);
	l2.add(805,840); 
	l2.add(855,900);
	l2.add(905,975);
	l2.add(1015,1085);
	l2.add(1087,1102);
	l2.add(1115,1125);
	l2.add(1130,1145);
	l2.add(1147,1160);



	l.denseIntersectWith(l2, true);
	EXPECT_FALSE(l!=l3);

	l.setToInterval(10,20);
	for (int i=30; i<1180; i+=20)
		l.add(i, i+10);
	l.denseIntersectWith(l2, false);
	EXPECT_FALSE(l!=l3);

	l2.setToInterval(10,20);
	for (int i=30; i<1180; i+=20)
		l2.add(i, i+10);
	l.setToInterval(15,55);
	l.add(70,80);
	l.add(93,97);
	l.add(105,115);
	l.add(125,155);
	l.add(162, 163);
	l.add(164, 165);
	l.add(167, 168);
	l.add(175,205);
	l.add(210,215);
	l.add(235,240);
	l.add(255,275);
	l.add(305,385);
	l.add(415,495);
	l.add(510,530);
	l.add(550,581);
	l.add(590,630);
	l.add(645,695);
	l.add(715,765);
	l.add(775,800);
	l.add(805,840); 
	l.add(855,900);
	l.add(905,975);
	l.add(1015,1085);
	l.add(1087,1102);
	l.add(1115,1125);
	l.add(1130,1145);
	l.add(1147,1160);




	l.denseIntersectWith(l2, true);
	EXPECT_FALSE(l!=l3);


	l.setToInterval(15,55);
	l.add(70,80);
	l.add(93,97);
	l.add(105,115);
	l.add(125,155);
	l.add(162, 163);
	l.add(164, 165);
	l.add(167, 168);
	l.add(175,205);
	l.add(210,215);
	l.add(235,240);
	l.add(255,275);
	l.add(305,385);
	l.add(415,495);
	l.add(510,530);
	l.add(550,581);
	l.add(590,630);
	l.add(645,695);
	l.add(715,765);
	l.add(775,800);
	l.add(805,840); 
	l.add(855,900);
	l.add(905,975);
	l.add(1015,1085);
	l.add(1087,1102);
	l.add(1115,1125);
	l.add(1130,1145);
	l.add(1147,1160);


	l.denseIntersectWith(l2, false);
	EXPECT_FALSE(l!=l3);

	//ntersect [10,20] ... [970,1180] with the complement of the above set" << endl;
	l3.setToInterval(10,20);
	for (int i=30; i<1180; i+=20)
		l3.add(i, i+10);  
	l3.remove(15,55);
	l3.remove(70,80);
	l3.remove(93,97);
	l3.remove(105,115);
	l3.remove(125,155);
	l3.remove(162, 163);
	l3.remove(164, 165);
	l3.remove(167, 168);
	l3.remove(175,205);
	l3.remove(210,215);
	l3.remove(235,240);
	l3.remove(255,275);
	l3.remove(305,385);
	l3.remove(415,495);
	l3.remove(510,530);
	l3.remove(550,581);
	l3.remove(590,630);
	l3.remove(645,695);
	l3.remove(715,765);
	l3.remove(775,800);
	l3.remove(805,840); 
	l3.remove(855,900);
	l3.remove(905,975);
	l3.remove(1015,1085);
	l3.remove(1087,1102);
	l3.remove(1115,1125);
	l3.remove(1130,1145);
	l3.remove(1147,1160);


	l.setToInterval(10,20);
	for (int i=30; i<1180; i+=20)
		l.add(i, i+10);
	l2.setToInterval(15,55);
	l2.add(70,80);
	l2.add(93,97);
	l2.add(105,115);
	l2.add(125,155);
	l2.add(162, 163);
	l2.add(164, 165);
	l2.add(167, 168);
	l2.add(175,205);
	l2.add(210,215);
	l2.add(235,240);
	l2.add(255,275);
	l2.add(305,385);
	l2.add(415,495);
	l2.add(510,530);
	l2.add(550,581);
	l2.add(590,630);  
	l2.add(645,695);
	l2.add(715,765);
	l2.add(775,800);
	l2.add(805,840); 
	l2.add(855,900);
	l2.add(905,975);
	l2.add(1015,1085);
	l2.add(1087,1102);
	l2.add(1115,1125);
	l2.add(1130,1145);
	l2.add(1147,1160);  

	l.denseIntersectWith(l2.subtractFromInterval(0,1200), true);
	EXPECT_FALSE(l!=l3);

	l.setToInterval(10,20);
	for (int i=30; i<1180; i+=20)
		l.add(i, i+10);
	l.denseIntersectWith(l2.subtractFromInterval(0,1200), false);
	EXPECT_FALSE(l!=l3);


	l2.setToInterval(10,20);
	for (int i=30; i<1180; i+=20)
		l2.add(i, i+10);
	l.setToInterval(15,55);
	l.add(70,80);
	l.add(93,97);
	l.add(105,115);
	l.add(125,155);
	l.add(162, 163);
	l.add(164, 165);
	l.add(167, 168);
	l.add(175,205);
	l.add(210,215);
	l.add(235,240);
	l.add(255,275);
	l.add(305,385);
	l.add(415,495);
	l.add(510,530);
	l.add(550,581);
	l.add(590,630);
	l.add(645,695);
	l.add(715,765);
	l.add(775,800);
	l.add(805,840); 
	l.add(855,900);
	l.add(905,975);
	l.add(1015,1085);
	l.add(1087,1102);
	l.add(1115,1125);
	l.add(1130,1145);
	l.add(1147,1160);
	l4=l.subtractFromInterval(0,1200); // complement

	l4.denseIntersectWith(l2, true);
	EXPECT_FALSE(l4!=l3);

	l.setToInterval(15,55);
	l.add(70,80);
	l.add(93,97);
	l.add(105,115);
	l.add(125,155);
	l.add(162, 163);
	l.add(164, 165);
	l.add(167, 168);
	l.add(175,205);
	l.add(210,215);
	l.add(235,240);
	l.add(255,275);
	l.add(305,385);
	l.add(415,495);
	l.add(510,530);
	l.add(550,581);
	l.add(590,630);
	l.add(645,695);
	l.add(715,765);
	l.add(775,800);
	l.add(805,840); 
	l.add(855,900);
	l.add(905,975);
	l.add(1015,1085);
	l.add(1087,1102);
	l.add(1115,1125);
	l.add(1130,1145);
	l.add(1147,1160);
	l4=l.subtractFromInterval(0,1200); // complement


	l4.denseIntersectWith(l2, false);
	EXPECT_FALSE(l4!=l3);
}


TEST(DetectorUnitTest, IPPrefixTests) {
	IPAddr a1(110,120, 130,140), a1Out;
	a1Out = a1.truncate(32);
	EXPECT_EQ( toString(a1Out), "110.120.130.140");

	a1Out = a1.truncate(24);
	EXPECT_EQ( toString(a1Out), "110.120.130.0");

	a1Out = a1.truncate(8);
	EXPECT_EQ( toString(a1Out), "110.0.0.0");

	a1Out = a1.truncate(0);
	EXPECT_EQ( toString(a1Out), "0.0.0.0");

	a1Out = a1.truncate(10);
	EXPECT_EQ( toString(a1Out), "110.64.0.0");

	IPPrefix b1(a1, 24);
	IPPrefix b2(a1, 8);
	IPPrefix b3(a1, 16);
	IPPrefix b4(a1, 0);

	EXPECT_FALSE (b2.isSubPrefixOf(b1));
	EXPECT_FALSE (!b1.isSubPrefixOf(b2));
	EXPECT_FALSE (!b1.isSubPrefixOf(b1));
	EXPECT_FALSE (b2.isSubPrefixOf(b3));
	EXPECT_FALSE(!b3.isSubPrefixOf(b2));
	EXPECT_FALSE(!b3.isSubPrefixOf(b4));
	EXPECT_FALSE(b4.isSubPrefixOf(b3));

	IPAddr c1(111,120, 130,140);
	IPPrefix d1(c1, 24);
	EXPECT_FALSE(d1.isSubPrefixOf(b1));
	EXPECT_FALSE(d1.isSubPrefixOf(b2));
	EXPECT_FALSE(!d1.isSubPrefixOf(b4));

	IPPrefix e1 (IPAddr(0,0,0,0),0);
	EXPECT_FALSE (!e1.isSubPrefixOf(e1));
	EXPECT_FALSE (e1.isSubPrefixOf(d1));
	EXPECT_FALSE (!d1.isSubPrefixOf(e1));

}


TEST(DetectorUnitTest, PrefixRangeTests) {

	PrefixRange r (IPPrefix(IPAddr(110,0,0,0), 8), 16);
	
	EXPECT_EQ( toString(r), "empty /8\nempty /9\nempty /10\nempty /11\nempty /12\nempty /13\nempty /14\nempty /15\nempty /16\n");
	EXPECT_TRUE( r.empty() );
	//"Empty method correct on empty range\n";



	r.addROA(IPPrefix(IPAddr(110,0,0,0), 8), 16); // equal to range

	EXPECT_FALSE(r.empty());
	

	PrefixRange r1(IPPrefix(IPAddr(110,0,0,0), 8), 16);
	r1.addROA(IPPrefix(IPAddr(110,0,0,0), 8), 14); // starts at the same prefix, subrange
	EXPECT_EQ( toString(r1), "[110.0.0.0-110.0.0.0] /8\n[110.0.0.0-110.128.0.0] /9\n[110.0.0.0-110.192.0.0] /10\n[110.0.0.0-110.224.0.0] /11\n[110.0.0.0-110.240.0.0] /12\n[110.0.0.0-110.248.0.0] /13\n[110.0.0.0-110.252.0.0] /14\nempty /15\nempty /16\n");

	
	r1.addROA(IPPrefix(IPAddr(110,192,0,0), 12), 18); // subrange, starts below extends down
	EXPECT_EQ( toString(r1), "[110.0.0.0-110.0.0.0] /8\n[110.0.0.0-110.128.0.0] /9\n[110.0.0.0-110.192.0.0] /10\n[110.0.0.0-110.224.0.0] /11\n[110.0.0.0-110.240.0.0] /12\n[110.0.0.0-110.248.0.0] /13\n[110.0.0.0-110.252.0.0] /14\n[110.192.0.0-110.206.0.0] /15\n[110.192.0.0-110.207.0.0] /16\n");

	//EXPECT_EQ( toString(r1), "");


	PrefixRange r2(IPPrefix(IPAddr(110,0,0,0), 8), 16);
	r2.addROA(IPPrefix(IPAddr(196,0,0,0), 2), 12); // not an ancestor 
	EXPECT_EQ( toString(r2), "empty /8\nempty /9\nempty /10\nempty /11\nempty /12\nempty /13\nempty /14\nempty /15\nempty /16\n");
	r2.addROA(IPPrefix(IPAddr(64,0,0,0), 2), 12); // above but doesn't extend all the way
	EXPECT_EQ( toString(r2), "[110.0.0.0-110.0.0.0] /8\n[110.0.0.0-110.128.0.0] /9\n[110.0.0.0-110.192.0.0] /10\n[110.0.0.0-110.224.0.0] /11\n[110.0.0.0-110.240.0.0] /12\nempty /13\nempty /14\nempty /15\nempty /16\n");
	r2.addROA(IPPrefix(IPAddr(110,192,0,0), 12), 18); // subrange, extends down
	EXPECT_EQ( toString(r2), "[110.0.0.0-110.0.0.0] /8\n[110.0.0.0-110.128.0.0] /9\n[110.0.0.0-110.192.0.0] /10\n[110.0.0.0-110.224.0.0] /11\n[110.0.0.0-110.240.0.0] /12\n[110.192.0.0-110.200.0.0] /13\n[110.192.0.0-110.204.0.0] /14\n[110.192.0.0-110.206.0.0] /15\n[110.192.0.0-110.207.0.0] /16\n");

	
	PrefixRange r3(IPPrefix(IPAddr(110,0,0,0), 8), 16);
	r3.addROA(IPPrefix(IPAddr(110,192,0,0), 12), 14); // subrange fully contained
	EXPECT_EQ( toString(r3), "empty /8\nempty /9\nempty /10\nempty /11\n[110.192.0.0-110.192.0.0] /12\n[110.192.0.0-110.200.0.0] /13\n[110.192.0.0-110.204.0.0] /14\nempty /15\nempty /16\n");
	r3.addROA(IPPrefix(IPAddr(64,0,0,0), 2), 24); // strict superset
	EXPECT_EQ( toString(r3), "[110.0.0.0-110.0.0.0] /8\n[110.0.0.0-110.128.0.0] /9\n[110.0.0.0-110.192.0.0] /10\n[110.0.0.0-110.224.0.0] /11\n[110.0.0.0-110.240.0.0] /12\n[110.0.0.0-110.248.0.0] /13\n[110.0.0.0-110.252.0.0] /14\n[110.0.0.0-110.254.0.0] /15\n[110.0.0.0-110.255.0.0] /16\n");

	PrefixRange r4(IPPrefix(IPAddr(110,0,0,0), 8), 16);
	r4.addROA(IPPrefix(IPAddr(64,0,0,0), 2), 7); // enclosing prefix but too far above
	EXPECT_EQ( toString(r4),  "empty /8\nempty /9\nempty /10\nempty /11\nempty /12\nempty /13\nempty /14\nempty /15\nempty /16\n");
	r4.addROA(IPPrefix(IPAddr(110,120,0,0), 17), 24); // subprefix but too far below
	EXPECT_EQ( toString(r4), "empty /8\nempty /9\nempty /10\nempty /11\nempty /12\nempty /13\nempty /14\nempty /15\nempty /16\n");
	r4.addROA(IPPrefix(IPAddr(112, 120, 0, 0), 17), 24); // not a subprefix

	
	//complement full\n";
	PrefixRange rOut = r.complement();
	
	EXPECT_EQ( toString(rOut), "empty /8\nempty /9\nempty /10\nempty /11\nempty /12\nempty /13\nempty /14\nempty /15\nempty /16\n");

	//complement empty\n";
	PrefixRange r4Out = r4.complement();
	EXPECT_EQ( toString(rOut), "empty /8\nempty /9\nempty /10\nempty /11\nempty /12\nempty /13\nempty /14\nempty /15\nempty /16\n");



	//complement something else\n";
	PrefixRange r1Out = r1.complement();
	EXPECT_EQ( toString(r1Out), "empty /8\nempty /9\nempty /10\nempty /11\nempty /12\nempty /13\nempty /14\n[110.0.0.0-110.190.0.0] [110.208.0.0-110.254.0.0] /15\n[110.0.0.0-110.191.0.0] [110.208.0.0-110.255.0.0] /16\n");

	

	
	PrefixRange s (IPPrefix(IPAddr(110,0,0,0), 8), 16);
	s.addROA(IPPrefix(IPAddr(110,0,0,0), 8), 16);
	s.removeROA(IPPrefix(IPAddr(110,192,0,0), 12), 18); // subrange, extends down
	EXPECT_EQ( toString(s), "[110.0.0.0-110.0.0.0] /8\n[110.0.0.0-110.128.0.0] /9\n[110.0.0.0-110.192.0.0] /10\n[110.0.0.0-110.224.0.0] /11\n[110.0.0.0-110.176.0.0] [110.208.0.0-110.240.0.0] /12\n[110.0.0.0-110.184.0.0] [110.208.0.0-110.248.0.0] /13\n[110.0.0.0-110.188.0.0] [110.208.0.0-110.252.0.0] /14\n[110.0.0.0-110.190.0.0] [110.208.0.0-110.254.0.0] /15\n[110.0.0.0-110.191.0.0] [110.208.0.0-110.255.0.0] /16\n");

	
	PrefixRange t (IPPrefix(IPAddr(110,0,0,0), 8), 16);
	t.addROA(IPPrefix(IPAddr(110,0,0,0), 8), 16);
	s.denseIntersectWith(t, false);
	EXPECT_EQ( toString(s), "[110.0.0.0-110.0.0.0] /8\n[110.0.0.0-110.128.0.0] /9\n[110.0.0.0-110.192.0.0] /10\n[110.0.0.0-110.224.0.0] /11\n[110.0.0.0-110.176.0.0] [110.208.0.0-110.240.0.0] /12\n[110.0.0.0-110.184.0.0] [110.208.0.0-110.248.0.0] /13\n[110.0.0.0-110.188.0.0] [110.208.0.0-110.252.0.0] /14\n[110.0.0.0-110.190.0.0] [110.208.0.0-110.254.0.0] /15\n[110.0.0.0-110.191.0.0] [110.208.0.0-110.255.0.0] /16\n");

	PrefixRange s3(IPPrefix(IPAddr(110,0,0,0), 5), 17);
	s3.addROA(IPPrefix(IPAddr(110,192,0,0), 12), 14);
	s.denseIntersectWith(s3, false);
	EXPECT_EQ( toString(s), "empty /8\nempty /9\nempty /10\nempty /11\nempty /12\nempty /13\nempty /14\nempty /15\nempty /16\n");


	PrefixRange s4 (IPPrefix(IPAddr(110,0,0,0), 8), 16);
	s4.addROA(IPPrefix(IPAddr(110,0,0,0), 8), 16);
	s4.removeROA(IPPrefix(IPAddr(110,192,0,0), 12), 18); // subrange, extends down

	s3.denseIntersectWith(s4,false);
	EXPECT_EQ( toString(s3), "empty /5\nempty /6\nempty /7\nempty /8\nempty /9\nempty /10\nempty /11\nempty /12\nempty /13\nempty /14\nempty /15\nempty /16\nempty /17\n");

	//subtract something that is in the wrong part of the tree but has a wide bound\n";
	PrefixRange t1 (IPPrefix(IPAddr(0,0,0,0), 0), 32);
	t1.addROA (IPPrefix(IPAddr(110,0,0,0), 8), 16);
	PrefixRange t1Out = t1.subtractFromROA(IPPrefix(IPAddr(12, 0, 0, 0), 8), 16); 
	EXPECT_EQ( toString(t1Out), "[12.0.0.0-12.0.0.0] /8\n[12.0.0.0-12.128.0.0] /9\n[12.0.0.0-12.192.0.0] /10\n[12.0.0.0-12.224.0.0] /11\n[12.0.0.0-12.240.0.0] /12\n[12.0.0.0-12.248.0.0] /13\n[12.0.0.0-12.252.0.0] /14\n[12.0.0.0-12.254.0.0] /15\n[12.0.0.0-12.255.0.0] /16\n");


	//subtract  something that is a subrange but has a wide bound\n";
	t1.removeROA(IPPrefix(IPAddr(110,0,0,0), 8), 16);
	t1.addROA (IPPrefix(IPAddr(12,128,0,0), 10), 12);
	PrefixRange t1Out2 = t1.subtractFromROA(IPPrefix(IPAddr(12, 0, 0, 0), 8), 16); 
	EXPECT_EQ( toString(t1Out2), "[12.0.0.0-12.0.0.0] /8\n[12.0.0.0-12.128.0.0] /9\n[12.0.0.0-12.64.0.0] [12.192.0.0-12.192.0.0] /10\n[12.0.0.0-12.96.0.0] [12.192.0.0-12.224.0.0] /11\n[12.0.0.0-12.112.0.0] [12.192.0.0-12.240.0.0] /12\n[12.0.0.0-12.248.0.0] /13\n[12.0.0.0-12.252.0.0] /14\n[12.0.0.0-12.254.0.0] /15\n[12.0.0.0-12.255.0.0] /16\n");



	//subtract something that is in the wrong part of the tree\n";
	PrefixRange t2 (IPPrefix(IPAddr(110,0,0,0), 8), 16);
	t2.addROA (IPPrefix(IPAddr(110,0,0,0), 8), 16);
	PrefixRange t2Out = t2.subtractFromROA(IPPrefix(IPAddr(12, 0, 0, 0), 8), 16); 
	EXPECT_EQ( toString(t2Out), "[12.0.0.0-12.0.0.0] /8\n[12.0.0.0-12.128.0.0] /9\n[12.0.0.0-12.192.0.0] /10\n[12.0.0.0-12.224.0.0] /11\n[12.0.0.0-12.240.0.0] /12\n[12.0.0.0-12.248.0.0] /13\n[12.0.0.0-12.252.0.0] /14\n[12.0.0.0-12.254.0.0] /15\n[12.0.0.0-12.255.0.0] /16\n");

	
	//subtract something that begins above and ends above\n"; 
	PrefixRange t3 (IPPrefix(IPAddr(12,0,0,0), 5), 7);
	t3.addROA (IPPrefix(IPAddr(12,0,0,0), 5), 7);
	PrefixRange t3Out = t3.subtractFromROA(IPPrefix(IPAddr(12, 0, 0, 0), 8), 16); 
	EXPECT_EQ( toString(t3Out), "[12.0.0.0-12.0.0.0] /8\n[12.0.0.0-12.128.0.0] /9\n[12.0.0.0-12.192.0.0] /10\n[12.0.0.0-12.224.0.0] /11\n[12.0.0.0-12.240.0.0] /12\n[12.0.0.0-12.248.0.0] /13\n[12.0.0.0-12.252.0.0] /14\n[12.0.0.0-12.254.0.0] /15\n[12.0.0.0-12.255.0.0] /16\n");

	//subtract something that begins above and ends exactly at the beginning\n";
	PrefixRange t4 (IPPrefix(IPAddr(12,0,0,0), 5), 8);
	t4.addROA (IPPrefix(IPAddr(12,0,0,0), 5), 8);
	PrefixRange t4Out = t4.subtractFromROA(IPPrefix(IPAddr(12, 0, 0, 0), 8), 16); 
	EXPECT_EQ( toString(t4Out), "empty /8\n[12.0.0.0-12.128.0.0] /9\n[12.0.0.0-12.192.0.0] /10\n[12.0.0.0-12.224.0.0] /11\n[12.0.0.0-12.240.0.0] /12\n[12.0.0.0-12.248.0.0] /13\n[12.0.0.0-12.252.0.0] /14\n[12.0.0.0-12.254.0.0] /15\n[12.0.0.0-12.255.0.0] /16\n");

	//subtract something that begins above and ends in the middle\n";
	PrefixRange t5 (IPPrefix(IPAddr(12,0,0,0), 5), 12);
	t5.addROA (IPPrefix(IPAddr(12,0,0,0), 5), 12);
	PrefixRange t5Out = t5.subtractFromROA(IPPrefix(IPAddr(12, 0, 0, 0), 8), 16); 
	EXPECT_EQ( toString(t5Out), "empty /8\nempty /9\nempty /10\nempty /11\nempty /12\n[12.0.0.0-12.248.0.0] /13\n[12.0.0.0-12.252.0.0] /14\n[12.0.0.0-12.254.0.0] /15\n[12.0.0.0-12.255.0.0] /16\n");

	//subtract something that begins above and ends exactly at the end\n";
	PrefixRange t6 (IPPrefix(IPAddr(12,0,0,0), 5), 16);
	t6.addROA (IPPrefix(IPAddr(12,0,0,0), 5), 16);
	PrefixRange t6Out = t6.subtractFromROA(IPPrefix(IPAddr(12, 0, 0, 0), 8), 16); 
	EXPECT_EQ( toString(t6Out), "empty /8\nempty /9\nempty /10\nempty /11\nempty /12\nempty /13\nempty /14\nempty /15\nempty /16\n");

	//subtract something that begins above and ends below\n";
	PrefixRange t7 (IPPrefix(IPAddr(12,0,0,0), 5), 24);
	t7.addROA (IPPrefix(IPAddr(12,0,0,0), 5), 24);
	PrefixRange t7Out = t7.subtractFromROA(IPPrefix(IPAddr(12, 0, 0, 0), 8), 16); 
	EXPECT_EQ( toString(t7Out), "empty /8\nempty /9\nempty /10\nempty /11\nempty /12\nempty /13\nempty /14\nempty /15\nempty /16\n");


	//subtract something that begins and ends exactly at the beginning\n";
	PrefixRange t8 (IPPrefix(IPAddr(12,0,0,0), 8), 8);
	t8.addROA (IPPrefix(IPAddr(12,0,0,0), 8), 8);
	PrefixRange t8Out = t8.subtractFromROA(IPPrefix(IPAddr(12, 0, 0, 0), 8), 16); 
	EXPECT_EQ( toString(t8Out), "empty /8\n[12.0.0.0-12.128.0.0] /9\n[12.0.0.0-12.192.0.0] /10\n[12.0.0.0-12.224.0.0] /11\n[12.0.0.0-12.240.0.0] /12\n[12.0.0.0-12.248.0.0] /13\n[12.0.0.0-12.252.0.0] /14\n[12.0.0.0-12.254.0.0] /15\n[12.0.0.0-12.255.0.0] /16\n");

	//subtract something that begins at the beginning and ends in the middle\n";
	PrefixRange t9 (IPPrefix(IPAddr(12,0,0,0), 8), 12);
	t9.addROA (IPPrefix(IPAddr(12,0,0,0), 8), 12);
	PrefixRange t9Out = t9.subtractFromROA(IPPrefix(IPAddr(12, 0, 0, 0), 8), 16); 
	EXPECT_EQ( toString(t9Out), "empty /8\nempty /9\nempty /10\nempty /11\nempty /12\n[12.0.0.0-12.248.0.0] /13\n[12.0.0.0-12.252.0.0] /14\n[12.0.0.0-12.254.0.0] /15\n[12.0.0.0-12.255.0.0] /16\n");


	
	//subtract something that begins at the beginning and ends at the end\n";
	PrefixRange t10 (IPPrefix(IPAddr(12,0,0,0), 8), 16);
	t10.addROA (IPPrefix(IPAddr(12,0,0,0), 8), 16);
	PrefixRange t10Out =t10.subtractFromROA(IPPrefix(IPAddr(12, 0, 0, 0), 8), 16); 
	EXPECT_EQ( toString(t10Out), "empty /8\nempty /9\nempty /10\nempty /11\nempty /12\nempty /13\nempty /14\nempty /15\nempty /16\n");

	//subtract something that begins at the beginning and ends below the end\n";
	PrefixRange t11 (IPPrefix(IPAddr(12,0,0,0), 8), 24);
	t11.addROA (IPPrefix(IPAddr(12,0,0,0), 8), 24);
	PrefixRange t11Out =t11.subtractFromROA(IPPrefix(IPAddr(12, 0, 0, 0), 8), 16); 
	EXPECT_EQ( toString(t11Out),  "empty /8\nempty /9\nempty /10\nempty /11\nempty /12\nempty /13\nempty /14\nempty /15\nempty /16\n");


	//subtract something that begins and ends in the middle\n";
	PrefixRange t12 (IPPrefix(IPAddr(12,128,0,0), 10), 12);
	t12.addROA (IPPrefix(IPAddr(12,128,0,0), 10), 12);
	PrefixRange t12Out =t12.subtractFromROA(IPPrefix(IPAddr(12, 0, 0, 0), 8), 16); 
	EXPECT_EQ( toString(t12Out), "[12.0.0.0-12.0.0.0] /8\n[12.0.0.0-12.128.0.0] /9\n[12.0.0.0-12.64.0.0] [12.192.0.0-12.192.0.0] /10\n[12.0.0.0-12.96.0.0] [12.192.0.0-12.224.0.0] /11\n[12.0.0.0-12.112.0.0] [12.192.0.0-12.240.0.0] /12\n[12.0.0.0-12.248.0.0] /13\n[12.0.0.0-12.252.0.0] /14\n[12.0.0.0-12.254.0.0] /15\n[12.0.0.0-12.255.0.0] /16\n");


	//subtract something that begins in the middle and ends at the end\n";
	PrefixRange t13 (IPPrefix(IPAddr(12,128,0,0), 12), 16);
	t13.addROA (IPPrefix(IPAddr(12,128,0,0), 12), 16);
	PrefixRange t13Out =t13.subtractFromROA(IPPrefix(IPAddr(12, 0, 0, 0), 8), 16); 
	EXPECT_EQ( toString(t13Out), "[12.0.0.0-12.0.0.0] /8\n[12.0.0.0-12.128.0.0] /9\n[12.0.0.0-12.192.0.0] /10\n[12.0.0.0-12.224.0.0] /11\n[12.0.0.0-12.112.0.0] [12.144.0.0-12.240.0.0] /12\n[12.0.0.0-12.120.0.0] [12.144.0.0-12.248.0.0] /13\n[12.0.0.0-12.124.0.0] [12.144.0.0-12.252.0.0] /14\n[12.0.0.0-12.126.0.0] [12.144.0.0-12.254.0.0] /15\n[12.0.0.0-12.127.0.0] [12.144.0.0-12.255.0.0] /16\n");


	
	//subtract something that begins in the middle and ends below the end\n";
	PrefixRange t14 (IPPrefix(IPAddr(12,128,0,0), 12), 24);
	t14.addROA (IPPrefix(IPAddr(12,128,0,0), 12), 24);
	PrefixRange t14Out =t14.subtractFromROA(IPPrefix(IPAddr(12, 0, 0, 0), 8), 16); 
	EXPECT_EQ( toString(t14Out), "[12.0.0.0-12.0.0.0] /8\n[12.0.0.0-12.128.0.0] /9\n[12.0.0.0-12.192.0.0] /10\n[12.0.0.0-12.224.0.0] /11\n[12.0.0.0-12.112.0.0] [12.144.0.0-12.240.0.0] /12\n[12.0.0.0-12.120.0.0] [12.144.0.0-12.248.0.0] /13\n[12.0.0.0-12.124.0.0] [12.144.0.0-12.252.0.0] /14\n[12.0.0.0-12.126.0.0] [12.144.0.0-12.254.0.0] /15\n[12.0.0.0-12.127.0.0] [12.144.0.0-12.255.0.0] /16\n");

	//subtract something that begins and ends in the same spot (i.e., one row only)\n";
	PrefixRange t15 (IPPrefix(IPAddr(12,128,0,0), 10), 10);
	t15.addROA (IPPrefix(IPAddr(12,128,0,0), 10), 10);
	PrefixRange t15Out =t15.subtractFromROA(IPPrefix(IPAddr(12, 0, 0, 0), 8), 16); 
	EXPECT_EQ( toString(t15Out), "[12.0.0.0-12.0.0.0] /8\n[12.0.0.0-12.128.0.0] /9\n[12.0.0.0-12.64.0.0] [12.192.0.0-12.192.0.0] /10\n[12.0.0.0-12.224.0.0] /11\n[12.0.0.0-12.240.0.0] /12\n[12.0.0.0-12.248.0.0] /13\n[12.0.0.0-12.252.0.0] /14\n[12.0.0.0-12.254.0.0] /15\n[12.0.0.0-12.255.0.0] /16\n");

	//subtract something that begins and ends at the end\n";
	PrefixRange t16 (IPPrefix(IPAddr(12,128,0,0), 16), 16);
	t16.addROA (IPPrefix(IPAddr(12,128,0,0), 16), 16);
	PrefixRange t16Out =t16.subtractFromROA(IPPrefix(IPAddr(12, 0, 0, 0), 8), 16); 
	EXPECT_EQ( toString(t16Out), "[12.0.0.0-12.0.0.0] /8\n[12.0.0.0-12.128.0.0] /9\n[12.0.0.0-12.192.0.0] /10\n[12.0.0.0-12.224.0.0] /11\n[12.0.0.0-12.240.0.0] /12\n[12.0.0.0-12.248.0.0] /13\n[12.0.0.0-12.252.0.0] /14\n[12.0.0.0-12.254.0.0] /15\n[12.0.0.0-12.127.0.0] [12.129.0.0-12.255.0.0] /16\n");

	//subtract something that begins at the end and ends below the end\n";
	PrefixRange t17 (IPPrefix(IPAddr(12,128,0,0), 16), 24);
	t17.addROA (IPPrefix(IPAddr(12,128,0,0), 16), 24);
	PrefixRange t17Out =t17.subtractFromROA(IPPrefix(IPAddr(12, 0, 0, 0), 8), 16); 
	EXPECT_EQ( toString(t17Out), "[12.0.0.0-12.0.0.0] /8\n[12.0.0.0-12.128.0.0] /9\n[12.0.0.0-12.192.0.0] /10\n[12.0.0.0-12.224.0.0] /11\n[12.0.0.0-12.240.0.0] /12\n[12.0.0.0-12.248.0.0] /13\n[12.0.0.0-12.252.0.0] /14\n[12.0.0.0-12.254.0.0] /15\n[12.0.0.0-12.127.0.0] [12.129.0.0-12.255.0.0] /16\n");

	//subtract something that begins and ends below the end\n";
	PrefixRange t18 (IPPrefix(IPAddr(12,128,0,0), 17), 17);
	t18.addROA (IPPrefix(IPAddr(12,128,0,0), 17), 17);
	PrefixRange t18Out =t18.subtractFromROA(IPPrefix(IPAddr(12, 0, 0, 0), 8), 16); 
	EXPECT_EQ( toString(t18Out), "[12.0.0.0-12.0.0.0] /8\n[12.0.0.0-12.128.0.0] /9\n[12.0.0.0-12.192.0.0] /10\n[12.0.0.0-12.224.0.0] /11\n[12.0.0.0-12.240.0.0] /12\n[12.0.0.0-12.248.0.0] /13\n[12.0.0.0-12.252.0.0] /14\n[12.0.0.0-12.254.0.0] /15\n[12.0.0.0-12.255.0.0] /16\n");	

}
