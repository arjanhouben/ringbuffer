#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "ringbuffer.h"

SCENARIO( "initialize without arguments" )
{
	ringbuffer< int, 8 > buffer;
	THEN( "the size should match the specified number of elements" )
	{
		REQUIRE( buffer.capacity() == 8 );
	}
	THEN( "there should be 0 items in the buffer" )
	{
		REQUIRE( buffer.size() == 0 );
	}
	WHEN( "adding a single value" )
	{
		buffer.push_back( 1 );
		THEN( "there should be 1 item in the buffer" )
		{
			REQUIRE( buffer.size() == 1 );
			AND_THEN( "the value of that item should match" )
			{
				REQUIRE( buffer.front() == 1 );
			}
			WHEN( "added items until buffer is full" )
			{
				buffer.push_back( 2 );
				buffer.push_back( 3 );
				buffer.push_back( 4 );
				buffer.push_back( 5 );
				buffer.push_back( 6 );
				buffer.push_back( 7 );
				buffer.push_back( 8 );
				THEN( "the first value should remain the same" )
				{
					REQUIRE( buffer.front() == 1 );
					AND_THEN( "the number of items should match the size" )
					{
						REQUIRE( buffer.capacity() == buffer.size() );
					}
					AND_THEN( "adding an additional item will throw an error" )
					{
						REQUIRE_THROWS( buffer.push_back( 9 ) );
					}
				}
				THEN( "all the values should be correct" )
				{
					auto start = buffer.begin();
					REQUIRE( *start++ == 1 );
					REQUIRE( *start++ == 2 );
					REQUIRE( *start++ == 3 );
					REQUIRE( *start++ == 4 );
					REQUIRE( *start++ == 5 );
					REQUIRE( *start++ == 6 );
					REQUIRE( *start++ == 7 );
					REQUIRE( *start++ == 8 );
				}
				AND_WHEN( "removing some values" )
				{
					int v;
					buffer.pop_front( v );
					buffer.pop_front( v );
					buffer.pop_front( v );
					THEN( "the number of items inside the buffer will reflect the removed items" )
					{
						REQUIRE( buffer.size() == 5 );
					}
					
					WHEN( "adding back some values" )
					{
						buffer.push_back( 9 );
						buffer.push_back( 10 );
						buffer.push_back( 11 );
						THEN( "all the values should be correct" )
						{
							auto start = buffer.begin();
							REQUIRE( *start++ == 4 );
							REQUIRE( *start++ == 5 );
							REQUIRE( *start++ == 6 );
							REQUIRE( *start++ == 7 );
							REQUIRE( *start++ == 8 );
							REQUIRE( *start++ == 9 );
							REQUIRE( *start++ == 10 );
							REQUIRE( *start++ == 11 );
						}
					}
				}
			}
		}
	}
}