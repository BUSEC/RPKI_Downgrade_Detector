TEST(DetectorUnitTest, SubtractFromIntervalTest) {

	//This locks down a fix to a bug that I discovered
	IntervalList<IPAddr> a,b;
	a.setIncrement( 1<<31 );
	a.add( IPAddr(0,0,0,0), IPAddr(128,0,0,0) );
	a.subtractFromInterval( IPAddr(0,0,0,0), IPAddr(128,0,0,0), b );

	//a - a = {}
	EXPECT_EQ( toString(b), "empty ");

	IntervalList<IPAddr> c,d;
	c.setIncrement( 1 );
	c.add( IPAddr(0,0,0,20), IPAddr(0,0,0,30) );
	c.add( IPAddr(0,0,0,50), IPAddr(0,0,0,70) );

	c.subtractFromInterval( IPAddr(0,0,0,25), IPAddr(0,0,0,55), d );

	EXPECT_EQ( toString(d), "[0.0.0.31-0.0.0.49] ");

	IntervalList<IPAddr> e,f;
	e.setIncrement( 1 );
	e.add( IPAddr(0,0,0,20), IPAddr(0,0,0,30) );
	e.add( IPAddr(0,0,0,50), IPAddr(0,0,0,70) );

	e.subtractFromInterval( IPAddr(0,0,0,25), IPAddr(0,0,0,70), f );

	EXPECT_EQ( toString(f), "[0.0.0.31-0.0.0.49] ");

}

TEST(DetectorUnitTest, SubtractFromIntervalEdgeCaseBug1Test) {

	//This locks down a fix to an overflow bug
	IntervalList<IPAddr> a,b;
	a.setIncrement( 1 );
	a.add( IPAddr(0,0,0,0), IPAddr(255,255,255,255) );
	a.subtractFromInterval( IPAddr(255,255,255,254), IPAddr(255,255,255,255), b );

	EXPECT_EQ( toString(b), "empty ");
}

TEST(DetectorUnitTest, SubtractFromIntervalEdgeCaseTest) {

	//This locks down a fix to an overflow bug
	IntervalList<IPAddr> a,b;
	a.setIncrement( 1 );

	a.add( IPAddr(128,0,0,0), IPAddr(255,255,255,255) );

	a.subtractFromInterval( IPAddr(64,0,0,0), IPAddr(255,255,255,200), b );

	EXPECT_EQ( toString(b), "[64.0.0.0-127.255.255.255] ");
}

TEST(DetectorUnitTest, SubtractFromIntervalEdgeCaseBug2Test) {

	//This locks down a fix to an overflow bug
	IntervalList<IPAddr> a,b;
	a.setIncrement( 1 );

	a.add( IPAddr(0,0,0,0), IPAddr(0,0,0,0) );

	a.subtractFromInterval( IPAddr(255,255,255,251), IPAddr(255,255,255,255), b );

	EXPECT_EQ( toString(b), "[255.255.255.251-255.255.255.255] ");
}

TEST(DetectorUnitTest, AddToIntervalEdgeCaseBug1Test) {
	IntervalList<IPAddr> a;

	//Locking down an edge case bug
	a.setIncrement( 1 );
	a.add( IPAddr(0,0,0,255), IPAddr( IPAddr::maxAddr ) );
	a.add( IPAddr(0,0,1,0), IPAddr(0,0,1,5) );

	EXPECT_EQ( toString(a), "[0.0.0.255-255.255.255.255] ");

	a.add( IPAddr(0,0,2,0), IPAddr( IPAddr::maxAddr ) );
	a.add( IPAddr(0,0,0,0), IPAddr( IPAddr::maxAddr ) );
	a.add( IPAddr(0,0,0,244), IPAddr( IPAddr(0,0,0,250) ) );

	EXPECT_EQ( toString(a), "[0.0.0.0-255.255.255.255] ");
}


TEST(DetectorUnitTest, AddToIntervalEdgeCaseTest) {
	IntervalList<IPAddr> a;

	//Locking down an edge case bug
	a.setIncrement( 1 );
	a.add( IPAddr(255,255,255,255), IPAddr(255,255,255,255) );
	a.add( IPAddr(255,255,255,190), IPAddr(255,255,255,255) );


	EXPECT_EQ( toString(a), "[255.255.255.190-255.255.255.255] ");
}

TEST(DetectorUnitTest, ComplementPrefixTests) {

	PrefixRange knownPrefixes = 
		PrefixRange( IPPrefix(IPAddr(0,0,0,0), 0),  32);

	knownPrefixes.addROA( IPPrefix(IPAddr(0,0,0,0), 0), 32);

	//This should be the complete set
	EXPECT_EQ( toString(knownPrefixes), "[0.0.0.0-0.0.0.0] /0\n[0.0.0.0-128.0.0.0] /1\n[0.0.0.0-192.0.0.0] /2\n[0.0.0.0-224.0.0.0] /3\n[0.0.0.0-240.0.0.0] /4\n[0.0.0.0-248.0.0.0] /5\n[0.0.0.0-252.0.0.0] /6\n[0.0.0.0-254.0.0.0] /7\n[0.0.0.0-255.0.0.0] /8\n[0.0.0.0-255.128.0.0] /9\n[0.0.0.0-255.192.0.0] /10\n[0.0.0.0-255.224.0.0] /11\n[0.0.0.0-255.240.0.0] /12\n[0.0.0.0-255.248.0.0] /13\n[0.0.0.0-255.252.0.0] /14\n[0.0.0.0-255.254.0.0] /15\n[0.0.0.0-255.255.0.0] /16\n[0.0.0.0-255.255.128.0] /17\n[0.0.0.0-255.255.192.0] /18\n[0.0.0.0-255.255.224.0] /19\n[0.0.0.0-255.255.240.0] /20\n[0.0.0.0-255.255.248.0] /21\n[0.0.0.0-255.255.252.0] /22\n[0.0.0.0-255.255.254.0] /23\n[0.0.0.0-255.255.255.0] /24\n[0.0.0.0-255.255.255.128] /25\n[0.0.0.0-255.255.255.192] /26\n[0.0.0.0-255.255.255.224] /27\n[0.0.0.0-255.255.255.240] /28\n[0.0.0.0-255.255.255.248] /29\n[0.0.0.0-255.255.255.252] /30\n[0.0.0.0-255.255.255.254] /31\n[0.0.0.0-255.255.255.255] /32\n");

	PrefixRange unknownPrefixes = knownPrefixes.complement();

	//The complement of the complete set should be the empty set
	EXPECT_EQ( toString(knownPrefixes), "[0.0.0.0-0.0.0.0] /0\n[0.0.0.0-128.0.0.0] /1\n[0.0.0.0-192.0.0.0] /2\n[0.0.0.0-224.0.0.0] /3\n[0.0.0.0-240.0.0.0] /4\n[0.0.0.0-248.0.0.0] /5\n[0.0.0.0-252.0.0.0] /6\n[0.0.0.0-254.0.0.0] /7\n[0.0.0.0-255.0.0.0] /8\n[0.0.0.0-255.128.0.0] /9\n[0.0.0.0-255.192.0.0] /10\n[0.0.0.0-255.224.0.0] /11\n[0.0.0.0-255.240.0.0] /12\n[0.0.0.0-255.248.0.0] /13\n[0.0.0.0-255.252.0.0] /14\n[0.0.0.0-255.254.0.0] /15\n[0.0.0.0-255.255.0.0] /16\n[0.0.0.0-255.255.128.0] /17\n[0.0.0.0-255.255.192.0] /18\n[0.0.0.0-255.255.224.0] /19\n[0.0.0.0-255.255.240.0] /20\n[0.0.0.0-255.255.248.0] /21\n[0.0.0.0-255.255.252.0] /22\n[0.0.0.0-255.255.254.0] /23\n[0.0.0.0-255.255.255.0] /24\n[0.0.0.0-255.255.255.128] /25\n[0.0.0.0-255.255.255.192] /26\n[0.0.0.0-255.255.255.224] /27\n[0.0.0.0-255.255.255.240] /28\n[0.0.0.0-255.255.255.248] /29\n[0.0.0.0-255.255.255.252] /30\n[0.0.0.0-255.255.255.254] /31\n[0.0.0.0-255.255.255.255] /32\n");

}

TEST(DetectorUnitTest, IPPrefixEndAddressTests) {
	IPPrefix ipPrefix = IPPrefix(IPAddr(0,0,0,0), 0);

	IPAddr expectedEndAddress = IPAddr(0);
	for( int i = 1; i <= 32; i++) {
		IPAddr ip = ipPrefix.endAddress(i);
		expectedEndAddress = expectedEndAddress + (1<<(32-i));
		EXPECT_EQ( toString(ip), toString(expectedEndAddress) );
	}
}


TEST(DetectorUnitTest, IPAddrAddTests) {
	IPAddr ip1 = IPAddr(0,0,0,0);
	IPAddr ip2 = IPAddr(0,0,0,1);

	IPAddr ip3 = ip1 + ip2;

	EXPECT_EQ(ip3, ip2);

	IPAddr ip4 = IPAddr(0,0,0,3);
	IPAddr ip5 = IPAddr(0,0,0,9);

	IPAddr ip6 = ip4 + ip5;

	EXPECT_EQ(ip6, IPAddr(0,0,0,12));
}

TEST(DetectorUnitTest, IPAddrToIntTests) {
	IPAddr ip1 = IPAddr(0,0,0,3);
	IPAddr ip2 = IPAddr(0,0,0,5);

	EXPECT_EQ(ip1, IPAddr(3u) );
	EXPECT_EQ(ip2, IPAddr(5u) );
}


TEST(DetectorUnitTest, IPAddrMinusTests) {
	IPAddr ip1 = IPAddr(0,0,0,3);
	IPAddr ip2 = IPAddr(0,0,0,2);

	IPAddr ip3 = ip1 - ip2;

	EXPECT_EQ(ip3, IPAddr(0,0,0,1) );
}

TEST( DetectorUnitTest, GetMaxAddrTests) {

	IPAddr max = IPAddr(IPAddr::maxAddr);

	EXPECT_EQ(max, IPAddr(0xFFFFFFFF));

}

TEST(DetectorUnitTest, RemoveIntervalEdgeCaseTest) {
	IntervalList<IPAddr> a;

	//Locking down an edge case bug
	a.setIncrement( 1 );
	a.add( IPAddr(255,255,255,250), IPAddr(255,255,255,255) );
	a.remove( IPAddr(0,0,0,0), IPAddr(255,255,255,255) );
	a.remove( IPAddr(0,250,0,0), IPAddr(255,255,255,0) );
	a.remove( IPAddr(0,0,0,0), IPAddr(255,255,255,255) );

	EXPECT_EQ( toString(a), "empty ");
}




TEST(DetectorUnitTest, PrefixRangeDenseIntersectEmptyTest){

	//insection of nothing
	PrefixRange a = 
		PrefixRange( IPPrefix(IPAddr(0,0,0,0), 0),  32);

	PrefixRange b = 
		PrefixRange( IPPrefix(IPAddr(0,0,0,0), 0),  32);


	EXPECT_EQ( toString(a), toString(b) );	

	a.denseIntersectWith(b, true);
	EXPECT_EQ( toString(a), "empty /0\nempty /1\nempty /2\nempty /3\nempty /4\nempty /5\nempty /6\nempty /7\nempty /8\nempty /9\nempty /10\nempty /11\nempty /12\nempty /13\nempty /14\nempty /15\nempty /16\nempty /17\nempty /18\nempty /19\nempty /20\nempty /21\nempty /22\nempty /23\nempty /24\nempty /25\nempty /26\nempty /27\nempty /28\nempty /29\nempty /30\nempty /31\nempty /32\n" );	
	

	b.addROA( IPPrefix(IPAddr(128,128,128,0), 24), 32);

	a.denseIntersectWith(b, true);
	EXPECT_EQ( toString(a), "empty /0\nempty /1\nempty /2\nempty /3\nempty /4\nempty /5\nempty /6\nempty /7\nempty /8\nempty /9\nempty /10\nempty /11\nempty /12\nempty /13\nempty /14\nempty /15\nempty /16\nempty /17\nempty /18\nempty /19\nempty /20\nempty /21\nempty /22\nempty /23\nempty /24\nempty /25\nempty /26\nempty /27\nempty /28\nempty /29\nempty /30\nempty /31\nempty /32\n" );	

	b.denseIntersectWith(a, true);
	EXPECT_EQ( toString(b), "empty /0\nempty /1\nempty /2\nempty /3\nempty /4\nempty /5\nempty /6\nempty /7\nempty /8\nempty /9\nempty /10\nempty /11\nempty /12\nempty /13\nempty /14\nempty /15\nempty /16\nempty /17\nempty /18\nempty /19\nempty /20\nempty /21\nempty /22\nempty /23\nempty /24\nempty /25\nempty /26\nempty /27\nempty /28\nempty /29\nempty /30\nempty /31\nempty /32\n" );	

}


TEST(DetectorUnitTest, PrefixRangeDenseIntersectSameIntersectionTest){

	PrefixRange a = 
		PrefixRange( IPPrefix(IPAddr(0,0,0,0), 0),  32);

	PrefixRange b = 
		PrefixRange( IPPrefix(IPAddr(0,0,0,0), 0),  32);


	EXPECT_EQ( toString(a), toString(b) );	

	//add the same prefix to both and that should be the prefix of the intersection
	a.addROA( IPPrefix(IPAddr(128,128,128,0), 24), 32);
	b.addROA( IPPrefix(IPAddr(128,128,128,0), 24), 32);

	//confirm they are the same
	EXPECT_EQ( toString(a), toString(b) );	


	a.denseIntersectWith(b, true);
	EXPECT_EQ( toString(a), "empty /0\nempty /1\nempty /2\nempty /3\nempty /4\nempty /5\nempty /6\nempty /7\nempty /8\nempty /9\nempty /10\nempty /11\nempty /12\nempty /13\nempty /14\nempty /15\nempty /16\nempty /17\nempty /18\nempty /19\nempty /20\nempty /21\nempty /22\nempty /23\n[128.128.128.0-128.128.128.0] /24\n[128.128.128.0-128.128.128.128] /25\n[128.128.128.0-128.128.128.192] /26\n[128.128.128.0-128.128.128.224] /27\n[128.128.128.0-128.128.128.240] /28\n[128.128.128.0-128.128.128.248] /29\n[128.128.128.0-128.128.128.252] /30\n[128.128.128.0-128.128.128.254] /31\n[128.128.128.0-128.128.128.255] /32\n" );
	
	//after the intersection they should be the same as well
	EXPECT_EQ( toString(a), toString(b) );	
	
}


TEST(DetectorUnitTest, PrefixRangeDenseIntersectComplementTest){

	PrefixRange a = 
		PrefixRange( IPPrefix(IPAddr(0,0,0,0), 0),  32);

	a.addROA( IPPrefix(IPAddr(128,128,64,0), 27), 32);
	PrefixRange b = a.complement();

	//confirm they are not the same
	EXPECT_NE( toString(a), toString(b) );	

	//the intersection of a prefix and it is complement should always be empty
	b.denseIntersectWith(a, true);
	EXPECT_EQ( toString(b), "empty /0\nempty /1\nempty /2\nempty /3\nempty /4\nempty /5\nempty /6\nempty /7\nempty /8\nempty /9\nempty /10\nempty /11\nempty /12\nempty /13\nempty /14\nempty /15\nempty /16\nempty /17\nempty /18\nempty /19\nempty /20\nempty /21\nempty /22\nempty /23\nempty /24\nempty /25\nempty /26\nempty /27\nempty /28\nempty /29\nempty /30\nempty /31\nempty /32\n");	

}


TEST(DetectorUnitTest, PrefixRangeDenseIntersectCompleteTest){
	
	PrefixRange a = 
		PrefixRange( IPPrefix(IPAddr(0,0,0,0), 0),  32);
		PrefixRange b = 
		PrefixRange( IPPrefix(IPAddr(0,0,0,0), 0),  32);


	// add the complete set to a
	a.addROA( IPPrefix(IPAddr(0,0,0,0), 0), 32);

	//add a subset to b
	b.addROA( IPPrefix(IPAddr(0,64,0,0), 25), 3);

	//confirm they are not the same
	EXPECT_NE( toString(a), toString(b) );	

	//intersection should be the 
	a.denseIntersectWith(b, true);
	EXPECT_EQ( toString(a), "empty /0\nempty /1\nempty /2\nempty /3\nempty /4\nempty /5\nempty /6\nempty /7\nempty /8\nempty /9\nempty /10\nempty /11\nempty /12\nempty /13\nempty /14\nempty /15\nempty /16\nempty /17\nempty /18\nempty /19\nempty /20\nempty /21\nempty /22\nempty /23\nempty /24\nempty /25\nempty /26\nempty /27\nempty /28\nempty /29\nempty /30\nempty /31\nempty /32\n");	

	//they should be the same now
	EXPECT_EQ( toString(a), toString(b) );	
}


TEST(DetectorUnitTest, ROAGroupSimpleTest){

	int as = 4;
	int prefixlen = 8;	
	int maxlen = 24;

	IPPrefix prefix = IPPrefix(IPAddr(127,0,0,0), prefixlen);

	ROAGroup g = ROAGroup();
	g.addROA(as, prefix, maxlen);

	EXPECT_TRUE( g.hasAS( as ) );
	EXPECT_FALSE( g.hasAS( as-1 ) );


	g.addROA(as+1, IPPrefix(IPAddr(127,2,2,2), 32), 32);
	g.addROA(as+1, IPPrefix(IPAddr(192,1,1,1), 32), 32);

	g.addROA(as+2, IPPrefix(IPAddr(10,0,0,0), prefixlen), maxlen);

	EXPECT_TRUE( g.hasAS( as ) );
	EXPECT_TRUE( g.hasAS( as+1 ) );
	EXPECT_TRUE( g.hasAS( as+2 ) );

	EXPECT_FALSE( g.hasAS( as+3 ) );

	PrefixRange* r1 = g.getByAS(as+1);

	EXPECT_EQ( toString( *r1 ), "empty /0\nempty /1\nempty /2\nempty /3\nempty /4\nempty /5\nempty /6\nempty /7\nempty /8\nempty /9\nempty /10\nempty /11\nempty /12\nempty /13\nempty /14\nempty /15\nempty /16\nempty /17\nempty /18\nempty /19\nempty /20\nempty /21\nempty /22\nempty /23\nempty /24\nempty /25\nempty /26\nempty /27\nempty /28\nempty /29\nempty /30\nempty /31\n[127.2.2.2-127.2.2.2] [192.1.1.1-192.1.1.1] /32\n");
}	

TEST(DetectorUnitTest, ROAGroupKnownTest){
	ROAGroup g = ROAGroup();

	const PrefixRange* k1 = g.getKnown();
	EXPECT_EQ( toString( *k1 ), "empty /0\nempty /1\nempty /2\nempty /3\nempty /4\nempty /5\nempty /6\nempty /7\nempty /8\nempty /9\nempty /10\nempty /11\nempty /12\nempty /13\nempty /14\nempty /15\nempty /16\nempty /17\nempty /18\nempty /19\nempty /20\nempty /21\nempty /22\nempty /23\nempty /24\nempty /25\nempty /26\nempty /27\nempty /28\nempty /29\nempty /30\nempty /31\nempty /32\n");

	int as = 4;
	g.addROA(as, IPPrefix(IPAddr(123,0,0,0), 8), 3);

	PrefixRange* r1 = g.getByAS(as);

	//known should extend to the shadow of the prefix we added
	const PrefixRange* k2 = g.getKnown();
	EXPECT_NE( toString( *k2 ),  toString( *r1 ) );

	EXPECT_EQ( toString( *k2 ), "empty /0\nempty /1\nempty /2\nempty /3\nempty /4\nempty /5\nempty /6\nempty /7\n[123.0.0.0-123.0.0.0] /8\n[123.0.0.0-123.128.0.0] /9\n[123.0.0.0-123.192.0.0] /10\n[123.0.0.0-123.224.0.0] /11\n[123.0.0.0-123.240.0.0] /12\n[123.0.0.0-123.248.0.0] /13\n[123.0.0.0-123.252.0.0] /14\n[123.0.0.0-123.254.0.0] /15\n[123.0.0.0-123.255.0.0] /16\n[123.0.0.0-123.255.128.0] /17\n[123.0.0.0-123.255.192.0] /18\n[123.0.0.0-123.255.224.0] /19\n[123.0.0.0-123.255.240.0] /20\n[123.0.0.0-123.255.248.0] /21\n[123.0.0.0-123.255.252.0] /22\n[123.0.0.0-123.255.254.0] /23\n[123.0.0.0-123.255.255.0] /24\n[123.0.0.0-123.255.255.128] /25\n[123.0.0.0-123.255.255.192] /26\n[123.0.0.0-123.255.255.224] /27\n[123.0.0.0-123.255.255.240] /28\n[123.0.0.0-123.255.255.248] /29\n[123.0.0.0-123.255.255.252] /30\n[123.0.0.0-123.255.255.254] /31\n[123.0.0.0-123.255.255.255] /32\n");

	g.addROA(as+1, IPPrefix(IPAddr(124,0,0,0), 7), 10);

	const PrefixRange* k3 = g.getKnown();

	EXPECT_NE( toString( *k3 ),  toString( *r1 ) );

	EXPECT_EQ( toString( *k3 ), "empty /0\nempty /1\nempty /2\nempty /3\nempty /4\nempty /5\nempty /6\n[124.0.0.0-124.0.0.0] /7\n[123.0.0.0-125.0.0.0] /8\n[123.0.0.0-125.128.0.0] /9\n[123.0.0.0-125.192.0.0] /10\n[123.0.0.0-125.224.0.0] /11\n[123.0.0.0-125.240.0.0] /12\n[123.0.0.0-125.248.0.0] /13\n[123.0.0.0-125.252.0.0] /14\n[123.0.0.0-125.254.0.0] /15\n[123.0.0.0-125.255.0.0] /16\n[123.0.0.0-125.255.128.0] /17\n[123.0.0.0-125.255.192.0] /18\n[123.0.0.0-125.255.224.0] /19\n[123.0.0.0-125.255.240.0] /20\n[123.0.0.0-125.255.248.0] /21\n[123.0.0.0-125.255.252.0] /22\n[123.0.0.0-125.255.254.0] /23\n[123.0.0.0-125.255.255.0] /24\n[123.0.0.0-125.255.255.128] /25\n[123.0.0.0-125.255.255.192] /26\n[123.0.0.0-125.255.255.224] /27\n[123.0.0.0-125.255.255.240] /28\n[123.0.0.0-125.255.255.248] /29\n[123.0.0.0-125.255.255.252] /30\n[123.0.0.0-125.255.255.254] /31\n[123.0.0.0-125.255.255.255] /32\n");

}

TEST(DetectorUnitTest, ROAGroupUnknownTest) {
	ROAGroup g1 = ROAGroup();
	int as = 4;

	//Test the empty set
	// g1 should be empty so unknown should be the complete set
	const PrefixRange* u1 = g1.getUnknown();

	EXPECT_EQ( toString( *u1 ), "[0.0.0.0-0.0.0.0] /0\n[0.0.0.0-128.0.0.0] /1\n[0.0.0.0-192.0.0.0] /2\n[0.0.0.0-224.0.0.0] /3\n[0.0.0.0-240.0.0.0] /4\n[0.0.0.0-248.0.0.0] /5\n[0.0.0.0-252.0.0.0] /6\n[0.0.0.0-254.0.0.0] /7\n[0.0.0.0-255.0.0.0] /8\n[0.0.0.0-255.128.0.0] /9\n[0.0.0.0-255.192.0.0] /10\n[0.0.0.0-255.224.0.0] /11\n[0.0.0.0-255.240.0.0] /12\n[0.0.0.0-255.248.0.0] /13\n[0.0.0.0-255.252.0.0] /14\n[0.0.0.0-255.254.0.0] /15\n[0.0.0.0-255.255.0.0] /16\n[0.0.0.0-255.255.128.0] /17\n[0.0.0.0-255.255.192.0] /18\n[0.0.0.0-255.255.224.0] /19\n[0.0.0.0-255.255.240.0] /20\n[0.0.0.0-255.255.248.0] /21\n[0.0.0.0-255.255.252.0] /22\n[0.0.0.0-255.255.254.0] /23\n[0.0.0.0-255.255.255.0] /24\n[0.0.0.0-255.255.255.128] /25\n[0.0.0.0-255.255.255.192] /26\n[0.0.0.0-255.255.255.224] /27\n[0.0.0.0-255.255.255.240] /28\n[0.0.0.0-255.255.255.248] /29\n[0.0.0.0-255.255.255.252] /30\n[0.0.0.0-255.255.255.254] /31\n[0.0.0.0-255.255.255.255] /32\n");

	//Add the complete set
	g1.addROA(as, IPPrefix(IPAddr(0,0,0,0), 0), 32);

	//Now unknown should be the empty set
	const PrefixRange* u2 = g1.getUnknown();

	EXPECT_EQ( toString( *u2 ), "empty /0\nempty /1\nempty /2\nempty /3\nempty /4\nempty /5\nempty /6\nempty /7\nempty /8\nempty /9\nempty /10\nempty /11\nempty /12\nempty /13\nempty /14\nempty /15\nempty /16\nempty /17\nempty /18\nempty /19\nempty /20\nempty /21\nempty /22\nempty /23\nempty /24\nempty /25\nempty /26\nempty /27\nempty /28\nempty /29\nempty /30\nempty /31\nempty /32\n");

	//Lets make sure the ases are getting merged in known correctly
	ROAGroup g2 = ROAGroup();

	g2.addROA(as, IPPrefix(IPAddr(64,0,0,0), 8), 2);
	g2.addROA(as+1, IPPrefix(IPAddr(32,0,0,0), 8), 2);
	g2.addROA(as+2, IPPrefix(IPAddr(128,0,0,0), 8), 2);

	const PrefixRange* u3 = g2.getUnknown();

	EXPECT_EQ( toString( *u3 ), "[0.0.0.0-0.0.0.0] /0\n[0.0.0.0-128.0.0.0] /1\n[0.0.0.0-192.0.0.0] /2\n[0.0.0.0-224.0.0.0] /3\n[0.0.0.0-240.0.0.0] /4\n[0.0.0.0-248.0.0.0] /5\n[0.0.0.0-252.0.0.0] /6\n[0.0.0.0-254.0.0.0] /7\n[0.0.0.0-31.0.0.0] [33.0.0.0-63.0.0.0] [65.0.0.0-127.0.0.0] [129.0.0.0-255.0.0.0] /8\n[0.0.0.0-31.128.0.0] [33.0.0.0-63.128.0.0] [65.0.0.0-127.128.0.0] [129.0.0.0-255.128.0.0] /9\n[0.0.0.0-31.192.0.0] [33.0.0.0-63.192.0.0] [65.0.0.0-127.192.0.0] [129.0.0.0-255.192.0.0] /10\n[0.0.0.0-31.224.0.0] [33.0.0.0-63.224.0.0] [65.0.0.0-127.224.0.0] [129.0.0.0-255.224.0.0] /11\n[0.0.0.0-31.240.0.0] [33.0.0.0-63.240.0.0] [65.0.0.0-127.240.0.0] [129.0.0.0-255.240.0.0] /12\n[0.0.0.0-31.248.0.0] [33.0.0.0-63.248.0.0] [65.0.0.0-127.248.0.0] [129.0.0.0-255.248.0.0] /13\n[0.0.0.0-31.252.0.0] [33.0.0.0-63.252.0.0] [65.0.0.0-127.252.0.0] [129.0.0.0-255.252.0.0] /14\n[0.0.0.0-31.254.0.0] [33.0.0.0-63.254.0.0] [65.0.0.0-127.254.0.0] [129.0.0.0-255.254.0.0] /15\n[0.0.0.0-31.255.0.0] [33.0.0.0-63.255.0.0] [65.0.0.0-127.255.0.0] [129.0.0.0-255.255.0.0] /16\n[0.0.0.0-31.255.128.0] [33.0.0.0-63.255.128.0] [65.0.0.0-127.255.128.0] [129.0.0.0-255.255.128.0] /17\n[0.0.0.0-31.255.192.0] [33.0.0.0-63.255.192.0] [65.0.0.0-127.255.192.0] [129.0.0.0-255.255.192.0] /18\n[0.0.0.0-31.255.224.0] [33.0.0.0-63.255.224.0] [65.0.0.0-127.255.224.0] [129.0.0.0-255.255.224.0] /19\n[0.0.0.0-31.255.240.0] [33.0.0.0-63.255.240.0] [65.0.0.0-127.255.240.0] [129.0.0.0-255.255.240.0] /20\n[0.0.0.0-31.255.248.0] [33.0.0.0-63.255.248.0] [65.0.0.0-127.255.248.0] [129.0.0.0-255.255.248.0] /21\n[0.0.0.0-31.255.252.0] [33.0.0.0-63.255.252.0] [65.0.0.0-127.255.252.0] [129.0.0.0-255.255.252.0] /22\n[0.0.0.0-31.255.254.0] [33.0.0.0-63.255.254.0] [65.0.0.0-127.255.254.0] [129.0.0.0-255.255.254.0] /23\n[0.0.0.0-31.255.255.0] [33.0.0.0-63.255.255.0] [65.0.0.0-127.255.255.0] [129.0.0.0-255.255.255.0] /24\n[0.0.0.0-31.255.255.128] [33.0.0.0-63.255.255.128] [65.0.0.0-127.255.255.128] [129.0.0.0-255.255.255.128] /25\n[0.0.0.0-31.255.255.192] [33.0.0.0-63.255.255.192] [65.0.0.0-127.255.255.192] [129.0.0.0-255.255.255.192] /26\n[0.0.0.0-31.255.255.224] [33.0.0.0-63.255.255.224] [65.0.0.0-127.255.255.224] [129.0.0.0-255.255.255.224] /27\n[0.0.0.0-31.255.255.240] [33.0.0.0-63.255.255.240] [65.0.0.0-127.255.255.240] [129.0.0.0-255.255.255.240] /28\n[0.0.0.0-31.255.255.248] [33.0.0.0-63.255.255.248] [65.0.0.0-127.255.255.248] [129.0.0.0-255.255.255.248] /29\n[0.0.0.0-31.255.255.252] [33.0.0.0-63.255.255.252] [65.0.0.0-127.255.255.252] [129.0.0.0-255.255.255.252] /30\n[0.0.0.0-31.255.255.254] [33.0.0.0-63.255.255.254] [65.0.0.0-127.255.255.254] [129.0.0.0-255.255.255.254] /31\n[0.0.0.0-31.255.255.255] [33.0.0.0-63.255.255.255] [65.0.0.0-127.255.255.255] [129.0.0.0-255.255.255.255] /32\n");

}

TEST(DetectorUnitTest, ROAGroupGetASesTest) {

	ROAGroup g = ROAGroup();
	g.addROA(0, IPPrefix(IPAddr(64,0,0,0), 8), 2);
	g.addROA(4, IPPrefix(IPAddr(32,0,0,0), 8), 2);
	g.addROA(3, IPPrefix(IPAddr(128,0,0,0), 8), 2);
	g.addROA(0, IPPrefix(IPAddr(12,0,0,0), 8), 2);
	g.addROA(0, IPPrefix(IPAddr(13,0,0,0), 8), 2);
	g.addROA(3, IPPrefix(IPAddr(13,0,0,0), 8), 2);


	set<int>* ases = g.getASes();

	EXPECT_EQ( ases->size(), 3u);
	EXPECT_TRUE( ases->find(0) != ases->end() );
	EXPECT_TRUE( ases->find(3) != ases->end() );
	EXPECT_TRUE( ases->find(4) != ases->end() );
	EXPECT_FALSE( ases->find(2) != ases->end() );

}


TEST(DetectorIntegrationTest, ValidToUnknownEmptyTest){

	//make new include a ROA for the complete set
	ROAGroup newPrefixes = ROAGroup();
	newPrefixes.addROA(1, IPPrefix(IPAddr(0,0,0,0), 0), 32);

	//The complete set 
	PrefixRange rOldFull = PrefixRange( IPPrefix(IPAddr(0,0,0,0), 0), 32 ); 
	rOldFull.addROA( IPPrefix(IPAddr(0,0,0,0), 0), 32 ); 


	//unknown should be empty since newPrefixes is the complete set
	ValidToUnknown( &newPrefixes, &rOldFull);

	//rOld should be empty
	EXPECT_EQ( toString(rOldFull),  "empty /0\nempty /1\nempty /2\nempty /3\nempty /4\nempty /5\nempty /6\nempty /7\nempty /8\nempty /9\nempty /10\nempty /11\nempty /12\nempty /13\nempty /14\nempty /15\nempty /16\nempty /17\nempty /18\nempty /19\nempty /20\nempty /21\nempty /22\nempty /23\nempty /24\nempty /25\nempty /26\nempty /27\nempty /28\nempty /29\nempty /30\nempty /31\nempty /32\n");


	//A partial set
	PrefixRange rOldPartial = PrefixRange( IPPrefix(IPAddr(0,0,0,0), 0), 32 ); 
	rOldPartial.addROA( IPPrefix(IPAddr(129,65,0,0), 16), 18 ); 


	ValidToUnknown( &newPrefixes, &rOldPartial);

	EXPECT_EQ( toString(rOldPartial),  "empty /0\nempty /1\nempty /2\nempty /3\nempty /4\nempty /5\nempty /6\nempty /7\nempty /8\nempty /9\nempty /10\nempty /11\nempty /12\nempty /13\nempty /14\nempty /15\nempty /16\nempty /17\nempty /18\nempty /19\nempty /20\nempty /21\nempty /22\nempty /23\nempty /24\nempty /25\nempty /26\nempty /27\nempty /28\nempty /29\nempty /30\nempty /31\nempty /32\n");


	//the empty set
	PrefixRange rOldEmpty = PrefixRange( IPPrefix(IPAddr(0,0,0,0), 0), 32 ); 
	
	ValidToUnknown( &newPrefixes, &rOldEmpty);

	EXPECT_EQ( toString(rOldEmpty),  "empty /0\nempty /1\nempty /2\nempty /3\nempty /4\nempty /5\nempty /6\nempty /7\nempty /8\nempty /9\nempty /10\nempty /11\nempty /12\nempty /13\nempty /14\nempty /15\nempty /16\nempty /17\nempty /18\nempty /19\nempty /20\nempty /21\nempty /22\nempty /23\nempty /24\nempty /25\nempty /26\nempty /27\nempty /28\nempty /29\nempty /30\nempty /31\nempty /32\n");

}

TEST(DetectorIntegrationTest, ValidToUnknownOverlappingTest){

	//ROAGroup that is the empty set
	ROAGroup newPrefixes = ROAGroup();
	
	//The complete set 
	PrefixRange rOldComplete = PrefixRange( IPPrefix(IPAddr(0,0,0,0), 0), 32 ); 
	rOldComplete.addROA( IPPrefix(IPAddr(0,0,0,0), 0), 32 ); 


	ValidToUnknown( &newPrefixes, &rOldComplete);

	//should return rOld unchanged
	EXPECT_EQ( toString(rOldComplete),  "[0.0.0.0-0.0.0.0] /0\n[0.0.0.0-128.0.0.0] /1\n[0.0.0.0-192.0.0.0] /2\n[0.0.0.0-224.0.0.0] /3\n[0.0.0.0-240.0.0.0] /4\n[0.0.0.0-248.0.0.0] /5\n[0.0.0.0-252.0.0.0] /6\n[0.0.0.0-254.0.0.0] /7\n[0.0.0.0-255.0.0.0] /8\n[0.0.0.0-255.128.0.0] /9\n[0.0.0.0-255.192.0.0] /10\n[0.0.0.0-255.224.0.0] /11\n[0.0.0.0-255.240.0.0] /12\n[0.0.0.0-255.248.0.0] /13\n[0.0.0.0-255.252.0.0] /14\n[0.0.0.0-255.254.0.0] /15\n[0.0.0.0-255.255.0.0] /16\n[0.0.0.0-255.255.128.0] /17\n[0.0.0.0-255.255.192.0] /18\n[0.0.0.0-255.255.224.0] /19\n[0.0.0.0-255.255.240.0] /20\n[0.0.0.0-255.255.248.0] /21\n[0.0.0.0-255.255.252.0] /22\n[0.0.0.0-255.255.254.0] /23\n[0.0.0.0-255.255.255.0] /24\n[0.0.0.0-255.255.255.128] /25\n[0.0.0.0-255.255.255.192] /26\n[0.0.0.0-255.255.255.224] /27\n[0.0.0.0-255.255.255.240] /28\n[0.0.0.0-255.255.255.248] /29\n[0.0.0.0-255.255.255.252] /30\n[0.0.0.0-255.255.255.254] /31\n[0.0.0.0-255.255.255.255] /32\n" );

	//A partial set
	PrefixRange rOldPartial = PrefixRange( IPPrefix(IPAddr(0,0,0,0), 0), 32 ); 
	rOldPartial.addROA( IPPrefix(IPAddr(129,65,0,0), 16), 18 ); 


	ValidToUnknown( &newPrefixes, &rOldPartial);

	EXPECT_EQ( toString(rOldPartial),  "empty /0\nempty /1\nempty /2\nempty /3\nempty /4\nempty /5\nempty /6\nempty /7\nempty /8\nempty /9\nempty /10\nempty /11\nempty /12\nempty /13\nempty /14\nempty /15\n[129.65.0.0-129.65.0.0] /16\n[129.65.0.0-129.65.128.0] /17\n[129.65.0.0-129.65.192.0] /18\nempty /19\nempty /20\nempty /21\nempty /22\nempty /23\nempty /24\nempty /25\nempty /26\nempty /27\nempty /28\nempty /29\nempty /30\nempty /31\nempty /32\n" );

	ROAGroup newPrefixes2 = ROAGroup();
	newPrefixes2.addROA( 1, IPPrefix(IPAddr(129,0,0,0), 8), 32 ); 

	ValidToUnknown( &newPrefixes2, &rOldPartial);

	EXPECT_EQ( toString(rOldPartial),  "empty /0\nempty /1\nempty /2\nempty /3\nempty /4\nempty /5\nempty /6\nempty /7\nempty /8\nempty /9\nempty /10\nempty /11\nempty /12\nempty /13\nempty /14\nempty /15\nempty /16\nempty /17\nempty /18\nempty /19\nempty /20\nempty /21\nempty /22\nempty /23\nempty /24\nempty /25\nempty /26\nempty /27\nempty /28\nempty /29\nempty /30\nempty /31\nempty /32\n" );

	PrefixRange rOldPartial2 = PrefixRange( IPPrefix(IPAddr(0,0,0,0), 0), 32 ); 
	rOldPartial2.addROA( IPPrefix(IPAddr(129,65,0,0), 16), 18 ); 

	ROAGroup newPrefixes3 = ROAGroup();
	newPrefixes3.addROA( 1, IPPrefix(IPAddr(129,65,128,0), 25), 32 ); 

	PrefixRange rOldPartial3 = PrefixRange( IPPrefix(IPAddr(0,0,0,0), 0), 32 ); 
	rOldPartial3.addROA( IPPrefix(IPAddr(129,65,0,0), 16), 32 ); 


	ValidToUnknown( &newPrefixes3, &rOldPartial3);

	EXPECT_EQ( toString(rOldPartial3),  "empty /0\nempty /1\nempty /2\nempty /3\nempty /4\nempty /5\nempty /6\nempty /7\nempty /8\nempty /9\nempty /10\nempty /11\nempty /12\nempty /13\nempty /14\nempty /15\n[129.65.0.0-129.65.0.0] /16\n[129.65.0.0-129.65.128.0] /17\n[129.65.0.0-129.65.192.0] /18\n[129.65.0.0-129.65.224.0] /19\n[129.65.0.0-129.65.240.0] /20\n[129.65.0.0-129.65.248.0] /21\n[129.65.0.0-129.65.252.0] /22\n[129.65.0.0-129.65.254.0] /23\n[129.65.0.0-129.65.255.0] /24\n[129.65.0.0-129.65.127.128] [129.65.128.128-129.65.255.128] /25\n[129.65.0.0-129.65.127.192] [129.65.128.128-129.65.255.192] /26\n[129.65.0.0-129.65.127.224] [129.65.128.128-129.65.255.224] /27\n[129.65.0.0-129.65.127.240] [129.65.128.128-129.65.255.240] /28\n[129.65.0.0-129.65.127.248] [129.65.128.128-129.65.255.248] /29\n[129.65.0.0-129.65.127.252] [129.65.128.128-129.65.255.252] /30\n[129.65.0.0-129.65.127.254] [129.65.128.128-129.65.255.254] /31\n[129.65.0.0-129.65.127.255] [129.65.128.128-129.65.255.255] /32\n");
}


TEST(DetectorIntegrationTest, ValidToInvalidEmptyTest){

	// Test (empty - empty) - empty = empty
	// for values of (R.valid - new.AS.valid) - new.AS.Unknown

	// R.valid = empty
	PrefixRange rOld = PrefixRange( IPPrefix(IPAddr(0,0,0,0), 0), 32 );
	int as = 4;

	// ROAGroup that is the empty set for as 4
	// new.AS.valid = empty
	ROAGroup newPrefixes = ROAGroup();

	// new.AS.Unknown = empty
	int otherAS = 5; 
	// make known the complete set so that unknown will be the empty set
	newPrefixes.addROA( otherAS, IPPrefix(IPAddr(0,0,0,0), 0), 32 ); 

	ValidToInvalid( &newPrefixes, &rOld, as);

	EXPECT_EQ( toString(rOld),  "empty /0\nempty /1\nempty /2\nempty /3\nempty /4\nempty /5\nempty /6\nempty /7\nempty /8\nempty /9\nempty /10\nempty /11\nempty /12\nempty /13\nempty /14\nempty /15\nempty /16\nempty /17\nempty /18\nempty /19\nempty /20\nempty /21\nempty /22\nempty /23\nempty /24\nempty /25\nempty /26\nempty /27\nempty /28\nempty /29\nempty /30\nempty /31\nempty /32\n");

}

TEST(DetectorIntegrationTest, ValidToInvalidTest){

	// Test (empty - empty) - empty = empty
	// for values of (R.valid - new.AS.valid) - new.AS.Unknown

	// R.valid = empty
	PrefixRange rOld = PrefixRange( IPPrefix(IPAddr(0,0,0,0), 0), 32 );
	int as = 4;

	// ROAGroup that is the empty set for as 4
	// new.AS.valid = empty
	ROAGroup newPrefixes = ROAGroup();

	// new.AS.Unknown = empty
	int otherAS = 5; 
	// make known the complete set so that unknown will be the empty set
	newPrefixes.addROA( otherAS, IPPrefix(IPAddr(0,0,0,0), 0), 32 ); 

	ValidToInvalid( &newPrefixes, &rOld, as);

	EXPECT_EQ( toString(rOld),  "empty /0\nempty /1\nempty /2\nempty /3\nempty /4\nempty /5\nempty /6\nempty /7\nempty /8\nempty /9\nempty /10\nempty /11\nempty /12\nempty /13\nempty /14\nempty /15\nempty /16\nempty /17\nempty /18\nempty /19\nempty /20\nempty /21\nempty /22\nempty /23\nempty /24\nempty /25\nempty /26\nempty /27\nempty /28\nempty /29\nempty /30\nempty /31\nempty /32\n");
				
}

TEST(DetectorIntegrationTest, ValidToInvalidSimpleTest){

	// Test (Everything - empty) - empty = Everything
	// for values of (R.valid - new.AS.valid) - new.AS.Unknown

	// R.valid = Everything
	PrefixRange rOld = PrefixRange( IPPrefix(IPAddr(0,0,0,0), 0), 32 );
	rOld.addROA( IPPrefix(IPAddr(0,0,0,0), 0), 32 );
	int as = 4;

	// ROAGroup that is the empty set for as 4
	// new.AS.valid = empty
	ROAGroup newPrefixes = ROAGroup();

	// new.unknown = empty
	int otherAS = 5; 
	// make known the complete set so that unknown will be the empty set
	newPrefixes.addROA( otherAS, IPPrefix(IPAddr(0,0,0,0), 0), 32 ); 

	ValidToInvalid( &newPrefixes, &rOld, as);

	EXPECT_EQ( toString(rOld),  "[0.0.0.0-0.0.0.0] /0\n[0.0.0.0-128.0.0.0] /1\n[0.0.0.0-192.0.0.0] /2\n[0.0.0.0-224.0.0.0] /3\n[0.0.0.0-240.0.0.0] /4\n[0.0.0.0-248.0.0.0] /5\n[0.0.0.0-252.0.0.0] /6\n[0.0.0.0-254.0.0.0] /7\n[0.0.0.0-255.0.0.0] /8\n[0.0.0.0-255.128.0.0] /9\n[0.0.0.0-255.192.0.0] /10\n[0.0.0.0-255.224.0.0] /11\n[0.0.0.0-255.240.0.0] /12\n[0.0.0.0-255.248.0.0] /13\n[0.0.0.0-255.252.0.0] /14\n[0.0.0.0-255.254.0.0] /15\n[0.0.0.0-255.255.0.0] /16\n[0.0.0.0-255.255.128.0] /17\n[0.0.0.0-255.255.192.0] /18\n[0.0.0.0-255.255.224.0] /19\n[0.0.0.0-255.255.240.0] /20\n[0.0.0.0-255.255.248.0] /21\n[0.0.0.0-255.255.252.0] /22\n[0.0.0.0-255.255.254.0] /23\n[0.0.0.0-255.255.255.0] /24\n[0.0.0.0-255.255.255.128] /25\n[0.0.0.0-255.255.255.192] /26\n[0.0.0.0-255.255.255.224] /27\n[0.0.0.0-255.255.255.240] /28\n[0.0.0.0-255.255.255.248] /29\n[0.0.0.0-255.255.255.252] /30\n[0.0.0.0-255.255.255.254] /31\n[0.0.0.0-255.255.255.255] /32\n");
				

	// Test ( EveryThing - Everything ) - empty =  empty

	// new.AS.valid
	newPrefixes.addROA( as, IPPrefix(IPAddr(0,0,0,0), 0), 32 ); 
	ValidToInvalid( &newPrefixes, &rOld, as);


	EXPECT_EQ( toString(rOld),  "empty /0\nempty /1\nempty /2\nempty /3\nempty /4\nempty /5\nempty /6\nempty /7\nempty /8\nempty /9\nempty /10\nempty /11\nempty /12\nempty /13\nempty /14\nempty /15\nempty /16\nempty /17\nempty /18\nempty /19\nempty /20\nempty /21\nempty /22\nempty /23\nempty /24\nempty /25\nempty /26\nempty /27\nempty /28\nempty /29\nempty /30\nempty /31\nempty /32\n");


	// Test ( EveryThing - Everything ) - Everything can never happen because if new.AS.valid
	// is complete when new.Unknown is empty


	// Test ( EveryThing - empty ) - Everything 

	// new.unknown = Everything and new.AS.valid = empty
	ROAGroup newPrefixes2 = ROAGroup();

	// R.valid = Everything
	rOld.addROA( IPPrefix(IPAddr(0,0,0,0), 0), 32 );

	ValidToInvalid( &newPrefixes, &rOld, as);

	EXPECT_EQ( toString(rOld),  "empty /0\nempty /1\nempty /2\nempty /3\nempty /4\nempty /5\nempty /6\nempty /7\nempty /8\nempty /9\nempty /10\nempty /11\nempty /12\nempty /13\nempty /14\nempty /15\nempty /16\nempty /17\nempty /18\nempty /19\nempty /20\nempty /21\nempty /22\nempty /23\nempty /24\nempty /25\nempty /26\nempty /27\nempty /28\nempty /29\nempty /30\nempty /31\nempty /32\n");

}



TEST(DetectorIntegrationTest, ValidToInvalidPartialTest){

	// Test (Partial0 - empty) - Partial1 =  (Partial0 - Partial1)
	// for values of (R.valid - new.AS.valid) - new.AS.Unknown

	// R.valid = Partial0
	PrefixRange rOld = PrefixRange( IPPrefix(IPAddr(0,0,0,0), 0), 32 );
	rOld.addROA( IPPrefix(IPAddr(128,65,0,0), 16), 24 );
	int as = 4;

	// ROAGroup that is the empty set for as 4
	// new.AS.valid = empty
	ROAGroup newPrefixes = ROAGroup();

	// new.unknown = Partial1
	int otherAS = 5; 
	// make known the complete set so that unknown will be the empty set
	newPrefixes.addROA( otherAS, IPPrefix(IPAddr(128,65,0,0), 20), 25 ); 

	ValidToInvalid( &newPrefixes, &rOld, as);

	EXPECT_EQ( toString(rOld),   "empty /0\nempty /1\nempty /2\nempty /3\nempty /4\nempty /5\nempty /6\nempty /7\nempty /8\nempty /9\nempty /10\nempty /11\nempty /12\nempty /13\nempty /14\nempty /15\nempty /16\nempty /17\nempty /18\nempty /19\n[128.65.0.0-128.65.0.0] /20\n[128.65.0.0-128.65.8.0] /21\n[128.65.0.0-128.65.12.0] /22\n[128.65.0.0-128.65.14.0] /23\n[128.65.0.0-128.65.15.0] /24\nempty /25\nempty /26\nempty /27\nempty /28\nempty /29\nempty /30\nempty /31\nempty /32\n");
				
	rOld.addROA( IPPrefix(IPAddr(128,65,0,0), 16), 24 );

	newPrefixes.addROA( as, IPPrefix(IPAddr(128,65,64,0), 18), 24 ); 

	newPrefixes.addROA( otherAS, IPPrefix(IPAddr(128,0,0,0), 8), 25 ); 

	ValidToInvalid( &newPrefixes, &rOld, as);

	EXPECT_EQ( toString(rOld),  "empty /0\nempty /1\nempty /2\nempty /3\nempty /4\nempty /5\nempty /6\nempty /7\nempty /8\nempty /9\nempty /10\nempty /11\nempty /12\nempty /13\nempty /14\nempty /15\n[128.65.0.0-128.65.0.0] /16\n[128.65.0.0-128.65.128.0] /17\n[128.65.0.0-128.65.0.0] [128.65.128.0-128.65.192.0] /18\n[128.65.0.0-128.65.32.0] [128.65.128.0-128.65.224.0] /19\n[128.65.0.0-128.65.48.0] [128.65.128.0-128.65.240.0] /20\n[128.65.0.0-128.65.56.0] [128.65.128.0-128.65.248.0] /21\n[128.65.0.0-128.65.60.0] [128.65.128.0-128.65.252.0] /22\n[128.65.0.0-128.65.62.0] [128.65.128.0-128.65.254.0] /23\n[128.65.0.0-128.65.63.0] [128.65.128.0-128.65.255.0] /24\nempty /25\nempty /26\nempty /27\nempty /28\nempty /29\nempty /30\nempty /31\nempty /32\n");

}


ostream& operator<<(ostream& os, unordered_map<int, PrefixRange*> a)
{

	for( unordered_map<int, PrefixRange*>::iterator i = a.begin(); i != a.end(); ++i){
		int as = i->first;
		os<<"AS:"<<as<<"={"<<endl;
		os<<*(i->second)<<endl;
		os<<"}"<<endl;
	}
	return os;
}

TEST(DetectorIntegrationTest, NotValidToValidSmokeTest){

	//just a quick smoke test
	ROAGroup oldPrefixes = ROAGroup();

	oldPrefixes.addROA( 1, IPPrefix(IPAddr(129,17,0,0), 32), 32 ); 
	oldPrefixes.addROA( 1, IPPrefix(IPAddr(128,65,0,0), 22), 32 );
	oldPrefixes.addROA( 2, IPPrefix(IPAddr(128,129,0,0), 22), 32 );
	oldPrefixes.addROA( 2, IPPrefix(IPAddr(128,129,0,0), 22), 32 );

	ROAGroup newPrefixes = ROAGroup();

	newPrefixes.addROA( 1, IPPrefix(IPAddr(129,17,0,0), 16), 30 ); 
	newPrefixes.addROA( 1, IPPrefix(IPAddr(128,0,0,0), 8), 30 ); 
	newPrefixes.addROA( 2, IPPrefix(IPAddr(128,129,0,0), 16), 30 );
	newPrefixes.addROA( 2, IPPrefix(IPAddr(128,129,0,0), 16), 30 );

	unordered_map<int, PrefixRange*> notValidToValidPrefixes;

	NotValidToValid( newPrefixes, oldPrefixes, &notValidToValidPrefixes);

	EXPECT_EQ( toString( notValidToValidPrefixes ), "AS:2={\nempty /0\nempty /1\nempty /2\nempty /3\nempty /4\nempty /5\nempty /6\nempty /7\nempty /8\nempty /9\nempty /10\nempty /11\nempty /12\nempty /13\nempty /14\nempty /15\n[128.129.0.0-128.129.0.0] /16\n[128.129.0.0-128.129.128.0] /17\n[128.129.0.0-128.129.192.0] /18\n[128.129.0.0-128.129.224.0] /19\n[128.129.0.0-128.129.240.0] /20\n[128.129.0.0-128.129.248.0] /21\n[128.129.4.0-128.129.252.0] /22\n[128.129.4.0-128.129.254.0] /23\n[128.129.4.0-128.129.255.0] /24\n[128.129.4.0-128.129.255.128] /25\n[128.129.4.0-128.129.255.192] /26\n[128.129.4.0-128.129.255.224] /27\n[128.129.4.0-128.129.255.240] /28\n[128.129.4.0-128.129.255.248] /29\n[128.129.4.0-128.129.255.252] /30\nempty /31\nempty /32\n\n}\nAS:1={\nempty /0\nempty /1\nempty /2\nempty /3\nempty /4\nempty /5\nempty /6\nempty /7\n[128.0.0.0-128.0.0.0] /8\n[128.0.0.0-128.128.0.0] /9\n[128.0.0.0-128.192.0.0] /10\n[128.0.0.0-128.224.0.0] /11\n[128.0.0.0-128.240.0.0] /12\n[128.0.0.0-128.248.0.0] /13\n[128.0.0.0-128.252.0.0] /14\n[128.0.0.0-128.254.0.0] /15\n[128.0.0.0-128.255.0.0] [129.17.0.0-129.17.0.0] /16\n[128.0.0.0-128.255.128.0] [129.17.0.0-129.17.128.0] /17\n[128.0.0.0-128.255.192.0] [129.17.0.0-129.17.192.0] /18\n[128.0.0.0-128.255.224.0] [129.17.0.0-129.17.224.0] /19\n[128.0.0.0-128.255.240.0] [129.17.0.0-129.17.240.0] /20\n[128.0.0.0-128.255.248.0] [129.17.0.0-129.17.248.0] /21\n[128.0.0.0-128.64.252.0] [128.65.4.0-128.255.252.0] [129.17.0.0-129.17.252.0] /22\n[128.0.0.0-128.64.254.0] [128.65.4.0-128.255.254.0] [129.17.0.0-129.17.254.0] /23\n[128.0.0.0-128.64.255.0] [128.65.4.0-128.255.255.0] [129.17.0.0-129.17.255.0] /24\n[128.0.0.0-128.64.255.128] [128.65.4.0-128.255.255.128] [129.17.0.0-129.17.255.128] /25\n[128.0.0.0-128.64.255.192] [128.65.4.0-128.255.255.192] [129.17.0.0-129.17.255.192] /26\n[128.0.0.0-128.64.255.224] [128.65.4.0-128.255.255.224] [129.17.0.0-129.17.255.224] /27\n[128.0.0.0-128.64.255.240] [128.65.4.0-128.255.255.240] [129.17.0.0-129.17.255.240] /28\n[128.0.0.0-128.64.255.248] [128.65.4.0-128.255.255.248] [129.17.0.0-129.17.255.248] /29\n[128.0.0.0-128.64.255.252] [128.65.4.0-128.255.255.252] [129.17.0.0-129.17.255.252] /30\nempty /31\nempty /32\n\n}\n");
}

TEST(DetectorIntegrationTest, NotValidToValidSimpleTest){

	ROAGroup oldPrefixes = ROAGroup();

	oldPrefixes.addROA( 1, IPPrefix(IPAddr(16,16,0,0), 24), 32 );  //same as new for as 1
	oldPrefixes.addROA( 2, IPPrefix(IPAddr(65,65,77,0), 24), 32 );  //smaller than new
	oldPrefixes.addROA( 3, IPPrefix(IPAddr(11,0,0,0), 8), 32 );  //bigger than new


	ROAGroup newPrefixes = ROAGroup();

	newPrefixes.addROA( 1, IPPrefix(IPAddr(16,16,0,0), 24), 32 ); 
	newPrefixes.addROA( 2, IPPrefix(IPAddr(65,65,0,0), 16), 32 );
	newPrefixes.addROA( 3, IPPrefix(IPAddr(11,0,0,0), 16), 32 );  

	unordered_map<int, PrefixRange*> notValidToValidPrefixes;

	NotValidToValid( newPrefixes, oldPrefixes, &notValidToValidPrefixes);

	// since as1 doesn't change we should see the empty set for AS=1
	// since as2 grows we should new-old 
	// since as3 shrinks we should see the empty set
	EXPECT_EQ( toString( notValidToValidPrefixes ),  "AS:2={\nempty /0\nempty /1\nempty /2\nempty /3\nempty /4\nempty /5\nempty /6\nempty /7\nempty /8\nempty /9\nempty /10\nempty /11\nempty /12\nempty /13\nempty /14\nempty /15\n[65.65.0.0-65.65.0.0] /16\n[65.65.0.0-65.65.128.0] /17\n[65.65.0.0-65.65.192.0] /18\n[65.65.0.0-65.65.224.0] /19\n[65.65.0.0-65.65.240.0] /20\n[65.65.0.0-65.65.248.0] /21\n[65.65.0.0-65.65.252.0] /22\n[65.65.0.0-65.65.254.0] /23\n[65.65.0.0-65.65.76.0] [65.65.78.0-65.65.255.0] /24\n[65.65.0.0-65.65.76.128] [65.65.78.0-65.65.255.128] /25\n[65.65.0.0-65.65.76.192] [65.65.78.0-65.65.255.192] /26\n[65.65.0.0-65.65.76.224] [65.65.78.0-65.65.255.224] /27\n[65.65.0.0-65.65.76.240] [65.65.78.0-65.65.255.240] /28\n[65.65.0.0-65.65.76.248] [65.65.78.0-65.65.255.248] /29\n[65.65.0.0-65.65.76.252] [65.65.78.0-65.65.255.252] /30\n[65.65.0.0-65.65.76.254] [65.65.78.0-65.65.255.254] /31\n[65.65.0.0-65.65.76.255] [65.65.78.0-65.65.255.255] /32\n\n}\n");

}

TEST(DetectorIntegrationTest, UnknownToKnownSimpleTest) {

	// test that unknown to known returns the empty set when oldPrefixes is a superset of newPrefixes
	ROAGroup oldPrefixes1 = ROAGroup();
	oldPrefixes1.addROA( 1, IPPrefix(IPAddr(128,0,0,0), 2), 32 );  

	int as = 1;
	int maxlen = 32;
	ROA rNew1 =  ROA(as, IPPrefix(IPAddr(133,111,111,0), 25), 32); 

	const PrefixRange* oldUnknown1 = oldPrefixes1.getUnknown();

	PrefixRange unknownToKnown1 = PrefixRange( IPPrefix(IPAddr(0,0,0,0), 0), 32 );
	UnknownToKnown( oldUnknown1, rNew1, &unknownToKnown1);

	//should be the empty set
	EXPECT_EQ( toString(unknownToKnown1) ,"empty /0\nempty /1\nempty /2\nempty /3\nempty /4\nempty /5\nempty /6\nempty /7\nempty /8\nempty /9\nempty /10\nempty /11\nempty /12\nempty /13\nempty /14\nempty /15\nempty /16\nempty /17\nempty /18\nempty /19\nempty /20\nempty /21\nempty /22\nempty /23\nempty /24\nempty /25\nempty /26\nempty /27\nempty /28\nempty /29\nempty /30\nempty /31\nempty /32\n" );

	//outside the bounds of the oldPrefixes known space
	ROA rNew2 =  ROA(as, IPPrefix(IPAddr(64,111,111,0), 25), maxlen); 

	PrefixRange unknownToKnown2 = PrefixRange( IPPrefix(IPAddr(0,0,0,0), 0), 32 );
	UnknownToKnown( oldUnknown1, rNew2, &unknownToKnown2);

	//should be all of rNew2 since it is completely outside the bounds of oldPrefixes1
	EXPECT_EQ( toString(unknownToKnown2) ,"empty /0\nempty /1\nempty /2\nempty /3\nempty /4\nempty /5\nempty /6\nempty /7\nempty /8\nempty /9\nempty /10\nempty /11\nempty /12\nempty /13\nempty /14\nempty /15\nempty /16\nempty /17\nempty /18\nempty /19\nempty /20\nempty /21\nempty /22\nempty /23\nempty /24\n[64.111.111.0-64.111.111.0] /25\n[64.111.111.0-64.111.111.64] /26\n[64.111.111.0-64.111.111.96] /27\n[64.111.111.0-64.111.111.112] /28\n[64.111.111.0-64.111.111.120] /29\n[64.111.111.0-64.111.111.124] /30\n[64.111.111.0-64.111.111.126] /31\n[64.111.111.0-64.111.111.127] /32\n" );

	// test the case where they partially overlap
	ROAGroup oldPrefixes2 = ROAGroup();
	oldPrefixes2.addROA( 1, IPPrefix(IPAddr(64,64,0,0), 20), 24 ); 

	ROA rNew3 =  ROA(as, IPPrefix(IPAddr(64,64,0,0), 16), maxlen);

	const PrefixRange* oldUnknown2 = oldPrefixes2.getUnknown();

	PrefixRange unknownToKnown3 = PrefixRange( IPPrefix(IPAddr(0,0,0,0), 0), 32 );
	UnknownToKnown( oldUnknown2, rNew3, &unknownToKnown3);

	// should result in prefixes at \16 where rNew3 starts and lose prefixes at \20
	EXPECT_EQ( toString(unknownToKnown3) ,  "empty /0\nempty /1\nempty /2\nempty /3\nempty /4\nempty /5\nempty /6\nempty /7\nempty /8\nempty /9\nempty /10\nempty /11\nempty /12\nempty /13\nempty /14\nempty /15\n[64.64.0.0-64.64.0.0] /16\n[64.64.0.0-64.64.128.0] /17\n[64.64.0.0-64.64.192.0] /18\n[64.64.0.0-64.64.224.0] /19\n[64.64.16.0-64.64.240.0] /20\n[64.64.16.0-64.64.248.0] /21\n[64.64.16.0-64.64.252.0] /22\n[64.64.16.0-64.64.254.0] /23\n[64.64.16.0-64.64.255.0] /24\n[64.64.16.0-64.64.255.128] /25\n[64.64.16.0-64.64.255.192] /26\n[64.64.16.0-64.64.255.224] /27\n[64.64.16.0-64.64.255.240] /28\n[64.64.16.0-64.64.255.248] /29\n[64.64.16.0-64.64.255.252] /30\n[64.64.16.0-64.64.255.254] /31\n[64.64.16.0-64.64.255.255] /32\n" );
}

TEST(DetectorIntegrationTest, UnknownToInvalidSmokeTest){
	
	vector<ROA*> rNews;
	int as =1;

	ROA roa1 = ROA(as, IPPrefix(IPAddr(64,64,64,0), 24), 32 );
	rNews.push_back( &roa1 );

	ROA roa2 = ROA(as, IPPrefix(IPAddr(128,64,64,0), 8), 32 );
	rNews.push_back( &roa2 );

	ROAGroup oldPrefixes = ROAGroup();
	ROAGroup newPrefixes = ROAGroup();

	UnknownToInvalid( &newPrefixes, &oldPrefixes, &rNews);
}

TEST(DetectorIntegrationTest, UnknownToInvalidLockdownTest){
	
	vector<ROA*> rNews;

	ROA roa1 = ROA(1, IPPrefix(IPAddr(64,64,64,0), 24), 32 );
	rNews.push_back( &roa1 );
	
	ROA roa2 = ROA(2, IPPrefix(IPAddr(65,65,0,0), 16), 32 );
	rNews.push_back( &roa2 );
	
	ROA roa3 = ROA( 3, IPPrefix(IPAddr(11,0,0,0), 16), 32 );  
	rNews.push_back( &roa3 );



	ROAGroup oldPrefixes = ROAGroup();
	oldPrefixes.addROA( 1, IPPrefix(IPAddr(16,16,0,0), 24), 32 ); 
	oldPrefixes.addROA( 2, IPPrefix(IPAddr(65,65,0,0), 16), 32 );
	
	ROAGroup newPrefixes = ROAGroup();
	newPrefixes.addROA( 1, IPPrefix(IPAddr(64,64,64,0), 24), 32 ); 
	newPrefixes.addROA( 2, IPPrefix(IPAddr(65,65,0,0), 16), 32 );
	newPrefixes.addROA( 3, IPPrefix(IPAddr(11,0,0,0), 16), 32 );  

	vector<UnknownToInvalidDowngrade> outputs = UnknownToInvalid( &newPrefixes, &oldPrefixes, &rNews);

	EXPECT_EQ( outputs.size(), 4u);

	// Test values are assumed to be true but not verified
	// EXPECT_EQ( toString(outputs[0]), "[R:{AS:1,Prefix:64.64.64.0/24-32},R.UnknownToKnown:{empty /0\nempty /1\nempty /2\nempty /3\nempty /4\nempty /5\nempty /6\nempty /7\nempty /8\nempty /9\nempty /10\nempty /11\nempty /12\nempty /13\nempty /14\nempty /15\nempty /16\nempty /17\nempty /18\nempty /19\nempty /20\nempty /21\nempty /22\nempty /23\n[64.64.64.0-64.64.64.0] /24\n[64.64.64.0-64.64.64.128] /25\n[64.64.64.0-64.64.64.192] /26\n[64.64.64.0-64.64.64.224] /27\n[64.64.64.0-64.64.64.240] /28\n[64.64.64.0-64.64.64.248] /29\n[64.64.64.0-64.64.64.252] /30\n[64.64.64.0-64.64.64.254] /31\n[64.64.64.0-64.64.64.255] /32\n},Except:{AS:1,Prefix:empty /0\nempty /1\nempty /2\nempty /3\nempty /4\nempty /5\nempty /6\nempty /7\nempty /8\nempty /9\nempty /10\nempty /11\nempty /12\nempty /13\nempty /14\nempty /15\nempty /16\nempty /17\nempty /18\nempty /19\nempty /20\nempty /21\nempty /22\nempty /23\nempty /24\nempty /25\nempty /26\nempty /27\nempty /28\nempty /29\nempty /30\nempty /31\nempty /32\n}]\n");
	// EXPECT_EQ( toString(outputs[1]), "[R:{AS:3,Prefix:11.0.0.0/16-32},R.UnknownToKnown:{empty /0\nempty /1\nempty /2\nempty /3\nempty /4\nempty /5\nempty /6\nempty /7\nempty /8\nempty /9\nempty /10\nempty /11\nempty /12\nempty /13\nempty /14\nempty /15\n[11.0.0.0-11.0.0.0] /16\n[11.0.0.0-11.0.128.0] /17\n[11.0.0.0-11.0.192.0] /18\n[11.0.0.0-11.0.224.0] /19\n[11.0.0.0-11.0.240.0] /20\n[11.0.0.0-11.0.248.0] /21\n[11.0.0.0-11.0.252.0] /22\n[11.0.0.0-11.0.254.0] /23\n[11.0.0.0-11.0.255.0] /24\n[11.0.0.0-11.0.255.128] /25\n[11.0.0.0-11.0.255.192] /26\n[11.0.0.0-11.0.255.224] /27\n[11.0.0.0-11.0.255.240] /28\n[11.0.0.0-11.0.255.248] /29\n[11.0.0.0-11.0.255.252] /30\n[11.0.0.0-11.0.255.254] /31\n[11.0.0.0-11.0.255.255] /32\n},Except:{AS:1,Prefix:empty /0\nempty /1\nempty /2\nempty /3\nempty /4\nempty /5\nempty /6\nempty /7\nempty /8\nempty /9\nempty /10\nempty /11\nempty /12\nempty /13\nempty /14\nempty /15\n[11.0.0.0-11.0.0.0] /16\n[11.0.0.0-11.0.128.0] /17\n[11.0.0.0-11.0.192.0] /18\n[11.0.0.0-11.0.224.0] /19\n[11.0.0.0-11.0.240.0] /20\n[11.0.0.0-11.0.248.0] /21\n[11.0.0.0-11.0.252.0] /22\n[11.0.0.0-11.0.254.0] /23\n[11.0.0.0-11.0.255.0] /24\n[11.0.0.0-11.0.255.128] /25\n[11.0.0.0-11.0.255.192] /26\n[11.0.0.0-11.0.255.224] /27\n[11.0.0.0-11.0.255.240] /28\n[11.0.0.0-11.0.255.248] /29\n[11.0.0.0-11.0.255.252] /30\n[11.0.0.0-11.0.255.254] /31\n[11.0.0.0-11.0.255.255] /32\n}]\n");
	// EXPECT_EQ( toString(outputs[2]), "[R:{AS:1,Prefix:64.64.64.0/24-32},R.UnknownToKnown:{empty /0\nempty /1\nempty /2\nempty /3\nempty /4\nempty /5\nempty /6\nempty /7\nempty /8\nempty /9\nempty /10\nempty /11\nempty /12\nempty /13\nempty /14\nempty /15\nempty /16\nempty /17\nempty /18\nempty /19\nempty /20\nempty /21\nempty /22\nempty /23\n[64.64.64.0-64.64.64.0] /24\n[64.64.64.0-64.64.64.128] /25\n[64.64.64.0-64.64.64.192] /26\n[64.64.64.0-64.64.64.224] /27\n[64.64.64.0-64.64.64.240] /28\n[64.64.64.0-64.64.64.248] /29\n[64.64.64.0-64.64.64.252] /30\n[64.64.64.0-64.64.64.254] /31\n[64.64.64.0-64.64.64.255] /32\n},Except:{AS:3,Prefix:empty /0\nempty /1\nempty /2\nempty /3\nempty /4\nempty /5\nempty /6\nempty /7\nempty /8\nempty /9\nempty /10\nempty /11\nempty /12\nempty /13\nempty /14\nempty /15\nempty /16\nempty /17\nempty /18\nempty /19\nempty /20\nempty /21\nempty /22\nempty /23\n[64.64.64.0-64.64.64.0] /24\n[64.64.64.0-64.64.64.128] /25\n[64.64.64.0-64.64.64.192] /26\n[64.64.64.0-64.64.64.224] /27\n[64.64.64.0-64.64.64.240] /28\n[64.64.64.0-64.64.64.248] /29\n[64.64.64.0-64.64.64.252] /30\n[64.64.64.0-64.64.64.254] /31\n[64.64.64.0-64.64.64.255] /32\n}]\n");
	// EXPECT_EQ( toString(outputs[3]), "[R:{AS:3,Prefix:11.0.0.0/16-32},R.UnknownToKnown:{empty /0\nempty /1\nempty /2\nempty /3\nempty /4\nempty /5\nempty /6\nempty /7\nempty /8\nempty /9\nempty /10\nempty /11\nempty /12\nempty /13\nempty /14\nempty /15\n[11.0.0.0-11.0.0.0] /16\n[11.0.0.0-11.0.128.0] /17\n[11.0.0.0-11.0.192.0] /18\n[11.0.0.0-11.0.224.0] /19\n[11.0.0.0-11.0.240.0] /20\n[11.0.0.0-11.0.248.0] /21\n[11.0.0.0-11.0.252.0] /22\n[11.0.0.0-11.0.254.0] /23\n[11.0.0.0-11.0.255.0] /24\n[11.0.0.0-11.0.255.128] /25\n[11.0.0.0-11.0.255.192] /26\n[11.0.0.0-11.0.255.224] /27\n[11.0.0.0-11.0.255.240] /28\n[11.0.0.0-11.0.255.248] /29\n[11.0.0.0-11.0.255.252] /30\n[11.0.0.0-11.0.255.254] /31\n[11.0.0.0-11.0.255.255] /32\n},Except:{AS:3,Prefix:empty /0\nempty /1\nempty /2\nempty /3\nempty /4\nempty /5\nempty /6\nempty /7\nempty /8\nempty /9\nempty /10\nempty /11\nempty /12\nempty /13\nempty /14\nempty /15\nempty /16\nempty /17\nempty /18\nempty /19\nempty /20\nempty /21\nempty /22\nempty /23\nempty /24\nempty /25\nempty /26\nempty /27\nempty /28\nempty /29\nempty /30\nempty /31\nempty /32\n}]\n");

}



TEST(DetectorIntegrationTest, UnknownToInvalidSimpleTest){
	
	vector<ROA*> rNews;


	ROA roa1 = ROA(1, IPPrefix(IPAddr(64,5,5,0), 24), 32 );
	rNews.push_back( &roa1 );
	
	ROA roa2 = ROA(2, IPPrefix(IPAddr(190,5,5,0), 24), 32 );
	rNews.push_back( &roa2 );

	ROA roa3 = ROA( 2, IPPrefix(IPAddr(128,0,0,0), 1), 16 );  
	rNews.push_back( &roa3 );
	

	ROAGroup oldPrefixes = ROAGroup();
	oldPrefixes.addROA( 1, IPPrefix(IPAddr(64,5,5,0), 24), 32 ); 
	oldPrefixes.addROA( 2, IPPrefix(IPAddr(190,5,5,0), 24), 32 );
	
	ROAGroup newPrefixes = ROAGroup();
	newPrefixes.addROA( 1, IPPrefix(IPAddr(64,5,5,0), 24), 32 ); 
	newPrefixes.addROA( 2, IPPrefix(IPAddr(190,5,5,0), 24), 32 );
	newPrefixes.addROA( 2, IPPrefix(IPAddr(128,0,0,0), 1), 16 );  

	vector<UnknownToInvalidDowngrade> outputs = UnknownToInvalid( &newPrefixes, &oldPrefixes, &rNews);

	EXPECT_EQ( outputs.size(), 1u);

	// Test values are assumed to be true but not verified
	EXPECT_EQ( toString(outputs[0]), "[R:{AS:2,Prefix:128.0.0.0/1-16},R.UnknownToKnown:{empty /0\n[128.0.0.0-128.0.0.0] /1\n[128.0.0.0-192.0.0.0] /2\n[128.0.0.0-224.0.0.0] /3\n[128.0.0.0-240.0.0.0] /4\n[128.0.0.0-248.0.0.0] /5\n[128.0.0.0-252.0.0.0] /6\n[128.0.0.0-254.0.0.0] /7\n[128.0.0.0-255.0.0.0] /8\n[128.0.0.0-255.128.0.0] /9\n[128.0.0.0-255.192.0.0] /10\n[128.0.0.0-255.224.0.0] /11\n[128.0.0.0-255.240.0.0] /12\n[128.0.0.0-255.248.0.0] /13\n[128.0.0.0-255.252.0.0] /14\n[128.0.0.0-255.254.0.0] /15\n[128.0.0.0-255.255.0.0] /16\n[128.0.0.0-255.255.128.0] /17\n[128.0.0.0-255.255.192.0] /18\n[128.0.0.0-255.255.224.0] /19\n[128.0.0.0-255.255.240.0] /20\n[128.0.0.0-255.255.248.0] /21\n[128.0.0.0-255.255.252.0] /22\n[128.0.0.0-255.255.254.0] /23\n[128.0.0.0-190.5.4.0] [190.5.6.0-255.255.255.0] /24\n[128.0.0.0-190.5.4.128] [190.5.6.0-255.255.255.128] /25\n[128.0.0.0-190.5.4.192] [190.5.6.0-255.255.255.192] /26\n[128.0.0.0-190.5.4.224] [190.5.6.0-255.255.255.224] /27\n[128.0.0.0-190.5.4.240] [190.5.6.0-255.255.255.240] /28\n[128.0.0.0-190.5.4.248] [190.5.6.0-255.255.255.248] /29\n[128.0.0.0-190.5.4.252] [190.5.6.0-255.255.255.252] /30\n[128.0.0.0-190.5.4.254] [190.5.6.0-255.255.255.254] /31\n[128.0.0.0-190.5.4.255] [190.5.6.0-255.255.255.255] /32\n},Except:{AS:2,Prefix:empty /0\nempty /1\nempty /2\nempty /3\nempty /4\nempty /5\nempty /6\nempty /7\nempty /8\nempty /9\nempty /10\nempty /11\nempty /12\nempty /13\nempty /14\nempty /15\nempty /16\n[128.0.0.0-255.255.128.0] /17\n[128.0.0.0-255.255.192.0] /18\n[128.0.0.0-255.255.224.0] /19\n[128.0.0.0-255.255.240.0] /20\n[128.0.0.0-255.255.248.0] /21\n[128.0.0.0-255.255.252.0] /22\n[128.0.0.0-255.255.254.0] /23\n[128.0.0.0-190.5.4.0] [190.5.6.0-255.255.255.0] /24\n[128.0.0.0-190.5.4.128] [190.5.6.0-255.255.255.128] /25\n[128.0.0.0-190.5.4.192] [190.5.6.0-255.255.255.192] /26\n[128.0.0.0-190.5.4.224] [190.5.6.0-255.255.255.224] /27\n[128.0.0.0-190.5.4.240] [190.5.6.0-255.255.255.240] /28\n[128.0.0.0-190.5.4.248] [190.5.6.0-255.255.255.248] /29\n[128.0.0.0-190.5.4.252] [190.5.6.0-255.255.255.252] /30\n[128.0.0.0-190.5.4.254] [190.5.6.0-255.255.255.254] /31\n[128.0.0.0-190.5.4.255] [190.5.6.0-255.255.255.255] /32\n}]\n");

}

TEST(DetectorUnitTest, SegfaultBugLockdown){
	vector<ROA*> newROAs;

	ROA roa1b = ROA(1, IPPrefix(IPAddr(64,5,5,0), 24), 32 );
	ROA roa2b = ROA(2, IPPrefix(IPAddr(190,5,5,0), 24), 32 );
	ROA roa3b = ROA(2, IPPrefix(IPAddr(128,0,0,0), 1), 16 );  
	
	newROAs.push_back( &roa1b );
	newROAs.push_back( &roa2b );
	newROAs.push_back( &roa3b );

	ROAGroup newPrefixes = ROAGroup( &newROAs );

	newPrefixes.getUnknown();
}

TEST(DetectorIntegrationTest, NoChanges){

	std::ostringstream output;


	ROA roa1a = ROA(1, IPPrefix(IPAddr(64,5,5,0), 24), 32 );
	ROA roa2a = ROA(2, IPPrefix(IPAddr(190,5,5,0), 24), 32 );
	ROA roa3a = ROA(2, IPPrefix(IPAddr(128,0,0,0), 1), 16 );  
	
	vector<ROA*> oldROAs;
	oldROAs.push_back( &roa1a );
	oldROAs.push_back( &roa2a );
	oldROAs.push_back( &roa3a );

	ROA roa1b = ROA(1, IPPrefix(IPAddr(64,5,5,0), 24), 32 );
	ROA roa2b = ROA(2, IPPrefix(IPAddr(190,5,5,0), 24), 32 );
	ROA roa3b = ROA(2, IPPrefix(IPAddr(128,0,0,0), 1), 16 ); 

	vector<ROA*> newROAs;
	newROAs.push_back( &roa1b );
	newROAs.push_back( &roa2b );
	newROAs.push_back( &roa3b );

	OutputDowngrades( &oldROAs, &newROAs, &output );

	EXPECT_EQ( output.str(), "");
}


TEST(DetectorIntegrationTest, Simple){

	std::ostringstream output;


	ROA roa1a = ROA(1, IPPrefix(IPAddr(64,5,5,0), 24), 32 );
	ROA roa2a = ROA(2, IPPrefix(IPAddr(190,5,5,0), 24), 32 );
	ROA roa3a = ROA(2, IPPrefix(IPAddr(128,0,0,0), 1), 16 );  
	
	vector<ROA*> oldROAs;
	oldROAs.push_back( &roa1a );
	oldROAs.push_back( &roa2a );
	oldROAs.push_back( &roa3a );

	ROA roa1b = ROA(1, IPPrefix(IPAddr(64,5,5,0), 24), 32 );
	ROA roa2b = ROA(2, IPPrefix(IPAddr(190,5,5,0), 24), 32 );
	ROA roa3b = ROA(2, IPPrefix(IPAddr(128,0,0,0), 2), 16 ); 

	vector<ROA*> newROAs;
	newROAs.push_back( &roa1b );
	newROAs.push_back( &roa2b );
	newROAs.push_back( &roa3b );

	OutputDowngrades( &oldROAs, &newROAs, &output );

	EXPECT_EQ( output.str(), "{\"CHANGE\":\"-V2U\", \"ROA\":{\"AS\":2, \"PREFIX\":\"128.0.0.0/1-16\", \"PATH\":\"\"}, \"EFFECT\":[\"[128.0.0.0-128.0.0.0] /1\", \"[192.0.0.0-192.0.0.0] /2\", \"[192.0.0.0-224.0.0.0] /3\", \"[192.0.0.0-240.0.0.0] /4\", \"[192.0.0.0-248.0.0.0] /5\", \"[192.0.0.0-252.0.0.0] /6\", \"[192.0.0.0-254.0.0.0] /7\", \"[192.0.0.0-255.0.0.0] /8\", \"[192.0.0.0-255.128.0.0] /9\", \"[192.0.0.0-255.192.0.0] /10\", \"[192.0.0.0-255.224.0.0] /11\", \"[192.0.0.0-255.240.0.0] /12\", \"[192.0.0.0-255.248.0.0] /13\", \"[192.0.0.0-255.252.0.0] /14\", \"[192.0.0.0-255.254.0.0] /15\", \"[192.0.0.0-255.255.0.0] /16\"]}\n");

}


TEST(DetectorIntegrationTest, Complex){

	std::ostringstream output;


	ROA roa1 = ROA(7341, IPPrefix(IPAddr(63,174,26,0), 23), 23 );
	ROA roa2 = ROA(7341, IPPrefix(IPAddr(63,174,20,0), 23), 23 );
	ROA roa3 = ROA(7341, IPPrefix(IPAddr(63,174,30,0), 24), 24 );  
	ROA roa4 = ROA(7341, IPPrefix(IPAddr(63,174,16,0), 22), 22 );


	vector<ROA*> oldROAs;
	oldROAs.push_back( &roa1 );
	oldROAs.push_back( &roa2 );
	oldROAs.push_back( &roa3 );
	oldROAs.push_back( &roa4 );


	ROA roa5 = ROA(17054, IPPrefix(IPAddr(63,174,16,0), 20), 20 );


	vector<ROA*> newROAs;
	newROAs.push_back( &roa1 );
	newROAs.push_back( &roa2 );
	newROAs.push_back( &roa3 );
	newROAs.push_back( &roa4 );
	newROAs.push_back( &roa5 );


	OutputDowngrades( &oldROAs, &newROAs, &output );

	EXPECT_EQ( output.str(),  "{\"CHANGE\":\"+U2I\", \"ROA\":{\"AS\":17054, \"PREFIX\":\"63.174.16.0/20-20\", \"PATH\":\"\"}, \"EFFECT\":[\"empty /0\", \"empty /1\", \"empty /2\", \"empty /3\", \"empty /4\", \"empty /5\", \"empty /6\", \"empty /7\", \"empty /8\", \"empty /9\", \"empty /10\", \"empty /11\", \"empty /12\", \"empty /13\", \"empty /14\", \"empty /15\", \"empty /16\", \"empty /17\", \"empty /18\", \"empty /19\", \"[63.174.16.0-63.174.16.0] /20\", \"[63.174.16.0-63.174.24.0] /21\", \"[63.174.20.0-63.174.28.0] /22\", \"[63.174.22.0-63.174.24.0] [63.174.28.0-63.174.30.0] /23\", \"[63.174.22.0-63.174.25.0] [63.174.28.0-63.174.29.0] [63.174.31.0-63.174.31.0] /24\", \"[63.174.22.0-63.174.25.128] [63.174.28.0-63.174.29.128] [63.174.31.0-63.174.31.128] /25\", \"[63.174.22.0-63.174.25.192] [63.174.28.0-63.174.29.192] [63.174.31.0-63.174.31.192] /26\", \"[63.174.22.0-63.174.25.224] [63.174.28.0-63.174.29.224] [63.174.31.0-63.174.31.224] /27\", \"[63.174.22.0-63.174.25.240] [63.174.28.0-63.174.29.240] [63.174.31.0-63.174.31.240] /28\", \"[63.174.22.0-63.174.25.248] [63.174.28.0-63.174.29.248] [63.174.31.0-63.174.31.248] /29\", \"[63.174.22.0-63.174.25.252] [63.174.28.0-63.174.29.252] [63.174.31.0-63.174.31.252] /30\", \"[63.174.22.0-63.174.25.254] [63.174.28.0-63.174.29.254] [63.174.31.0-63.174.31.254] /31\", \"[63.174.22.0-63.174.25.255] [63.174.28.0-63.174.29.255] [63.174.31.0-63.174.31.255] /32\"], \"EXCEPT\":{\"17054\":[\"empty /0\", \"empty /1\", \"empty /2\", \"empty /3\", \"empty /4\", \"empty /5\", \"empty /6\", \"empty /7\", \"empty /8\", \"empty /9\", \"empty /10\", \"empty /11\", \"empty /12\", \"empty /13\", \"empty /14\", \"empty /15\", \"empty /16\", \"empty /17\", \"empty /18\", \"empty /19\", \"empty /20\", \"[63.174.16.0-63.174.24.0] /21\", \"[63.174.20.0-63.174.28.0] /22\", \"[63.174.22.0-63.174.24.0] [63.174.28.0-63.174.30.0] /23\", \"[63.174.22.0-63.174.25.0] [63.174.28.0-63.174.29.0] [63.174.31.0-63.174.31.0] /24\", \"[63.174.22.0-63.174.25.128] [63.174.28.0-63.174.29.128] [63.174.31.0-63.174.31.128] /25\", \"[63.174.22.0-63.174.25.192] [63.174.28.0-63.174.29.192] [63.174.31.0-63.174.31.192] /26\", \"[63.174.22.0-63.174.25.224] [63.174.28.0-63.174.29.224] [63.174.31.0-63.174.31.224] /27\", \"[63.174.22.0-63.174.25.240] [63.174.28.0-63.174.29.240] [63.174.31.0-63.174.31.240] /28\", \"[63.174.22.0-63.174.25.248] [63.174.28.0-63.174.29.248] [63.174.31.0-63.174.31.248] /29\", \"[63.174.22.0-63.174.25.252] [63.174.28.0-63.174.29.252] [63.174.31.0-63.174.31.252] /30\", \"[63.174.22.0-63.174.25.254] [63.174.28.0-63.174.29.254] [63.174.31.0-63.174.31.254] /31\", \"[63.174.22.0-63.174.25.255] [63.174.28.0-63.174.29.255] [63.174.31.0-63.174.31.255] /32\"]}}\n");

}