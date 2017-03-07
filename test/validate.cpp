#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch/catch.hpp>
#include <string>
#include <vector>
#include "../ptrlist.h"

TEST_CASE( "vectors can be sized and resized", "[PtrList]" )
{
    PtrList<int> v( 5 );

    REQUIRE( v.size() == 5 );
    REQUIRE( v.capacity() >= 5 );

    SECTION( "resizing bigger changes size and capacity" )
    {
        v.resize( 10 );

        REQUIRE( v.size() == 10 );
        REQUIRE( v.capacity() >= 10 );
    }
    SECTION( "resizing smaller changes size but not capacity" ) {
        v.resize( 0 );

        REQUIRE( v.size() == 0 );
        REQUIRE( v.capacity() >= 5 );
    }
    SECTION( "reserving bigger changes capacity but not size" ) {
        v.reserve( 10 );

        REQUIRE( v.size() == 5 );
        REQUIRE( v.capacity() >= 10 );
    }
    SECTION( "reserving smaller does not change size or capacity" ) {
        v.reserve( 0 );

        REQUIRE( v.size() == 5 );
        REQUIRE( v.capacity() >= 5 );
    }
}


TEST_CASE( "Adding and removing items", "[PtrList]" )
{
    PtrList<int> v;
    REQUIRE( v.size() == 0 );

    SECTION( "Reserve 10 and store 5 elements, then insert one" ) {
        v.reserve( 10 );
        v.push_back(4);
        v.push_back(45);
        v.push_back(23);
        v.push_back(73);
        v.push_back(77);
        REQUIRE( v.size() == 5 );
        REQUIRE( v.capacity() >= 10 );

        REQUIRE( v[0] == 4 );
        REQUIRE( v[1] == 45 );
        REQUIRE( v[2] == 23 );
        REQUIRE( v[3] == 73 );
        REQUIRE( v[4] == 77 );

        int sum = 0;
        for(int &i : v)
            sum += i;
        REQUIRE( sum == 222 );

        v.insert(2, 88);

        REQUIRE( v.first() == 4 );
        REQUIRE( v.front() == 4 );
        REQUIRE( v[0] == 4 );
        REQUIRE( v[1] == 45 );
        REQUIRE( v[2] == 88 );
        REQUIRE( v[3] == 23 );
        REQUIRE( v[4] == 73 );
        REQUIRE( v[5] == 77 );
        REQUIRE( v.back() == 77 );
        REQUIRE( v.last() == 77 );
        REQUIRE( v.contains(88) );
        REQUIRE_FALSE( v.contains(42) );
        REQUIRE( v.find(88) != v.end() );
        REQUIRE( v.find(42) == v.end() );

        REQUIRE( v.size() == 6 );

        sum = 0;
        for(PtrList<int>::iterator i = v.begin(); i != v.end(); i++)
            sum += *i;
        REQUIRE( sum == 310 );

        v.clear();
        REQUIRE( v.size() == 0 );
    }

    SECTION( "Removing items" ) {
        v.reserve(5);
        v.push_back(4);
        v.push_back(45);
        v.push_back(23);
        v.push_back(73);
        v.push_back(77);
        REQUIRE( v.size() == 5 );
        REQUIRE( v.capacity() >= 5 );

        REQUIRE( v.size() == 5 );
        v.pop_back();
        REQUIRE( v.size() == 4 );
        REQUIRE( v[3] == 73 );
        v.pop_front();
        REQUIRE( v.size() == 3 );
        REQUIRE( v[0] == 45 );
        v.removeAt(1);
        REQUIRE( v.size() == 2 );
        REQUIRE( v[0] == 45 );
        REQUIRE( v[1] == 73 );

        PtrList<int>::iterator k = v.erase(v.begin() + 1);
        REQUIRE( k == v.end());
        REQUIRE( v.size() == 1 );
        //45 was at begin
        v.push_back(4);
        v.push_back(45);//Delete at this
        v.push_back(23);
        v.push_back(73);
        v.push_back(77);//Delete to this
        v.push_back(88);
        v.push_back(99);
        v.push_back(111);

        v.removeAt(2, 4);
        REQUIRE( v.size() == 5 );
        REQUIRE( v[0] == 45 );
        REQUIRE( v[1] == 4 );
        REQUIRE( v[2] == 88 );
        REQUIRE( v[3] == 99 );
        REQUIRE( v[4] == 111 );

        v.push_back(45);
        v.push_back(23);
        v.push_back(73);
        v.push_back(77);
        v.push_back(88);
        v.push_back(99);
        v.push_back(111);

        k = v.erase(v.begin() + 3, v.begin() + 7);
        REQUIRE( k == v.end() - 5 );

        REQUIRE( v[0] == 45 );
        REQUIRE( v[1] == 4 );
        REQUIRE( v[2] == 88 );
        REQUIRE( v[3] == 73 );//<-
        REQUIRE( v[4] == 77 );
        REQUIRE( v[5] == 88 );
        REQUIRE( v[6] == 99 );
        REQUIRE( v[7] == 111 );
        REQUIRE( v.size() == 8 );

        v.removeOne(88);
        REQUIRE( v[0] == 45 );
        REQUIRE( v[1] == 4 );
        REQUIRE( v[2] == 73 );
        REQUIRE( v[3] == 77 );
        REQUIRE( v[4] == 88 );
        REQUIRE( v[5] == 99 );
        REQUIRE( v[6] == 111 );
        REQUIRE( v.size() == 7 );

        v.insert(2, 88);
        v.removeAll(88);
        REQUIRE( v[0] == 45 );
        REQUIRE( v[1] == 4 );
        REQUIRE( v[2] == 73 );
        REQUIRE( v[3] == 77 );
        REQUIRE( v[4] == 99 );
        REQUIRE( v[5] == 111 );
        REQUIRE( v.size() == 6 );

        v.clear();
        REQUIRE( v.size() == 0 );
    }

    SECTION("Moving and Swapping items and copy of array")
    {
        PtrList<int> copypasta;
        v.reserve(5);
        v.push_back(4);
        v.push_back(45);
        v.push_back(23);
        v.push_back(73);
        v.push_back(77);
        REQUIRE( v.size() == 5 );
        REQUIRE( v.capacity() >= 5 );

        REQUIRE( v[0] == 4 );
        REQUIRE( v[1] == 45 );
        REQUIRE( v[2] == 23 );
        REQUIRE( v[3] == 73 );
        REQUIRE( v[4] == 77 );

        v.move(3, 1);

        REQUIRE( v[0] == 4 );
        REQUIRE( v[1] == 73 );
        REQUIRE( v[2] == 45 );
        REQUIRE( v[3] == 23 );
        REQUIRE( v[4] == 77 );

        v.swap(0, 2);

        REQUIRE( v[0] == 45 );
        REQUIRE( v[1] == 73 );
        REQUIRE( v[2] == 4 );
        REQUIRE( v[3] == 23 );
        REQUIRE( v[4] == 77 );

        copypasta = v;

        REQUIRE( copypasta[0] == 45 );
        REQUIRE( copypasta[1] == 73 );
        REQUIRE( copypasta[2] == 4 );
        REQUIRE( copypasta[3] == 23 );
        REQUIRE( copypasta[4] == 77 );

        v.clear();
        REQUIRE( v.size() == 0 );
        copypasta.clear();
        REQUIRE( copypasta.size() == 0 );
    }
}
