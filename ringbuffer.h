#pragma once

namespace helper
{
	
	template< size_t N, size_t Size >
	struct which_power_of_two
	{
		enum {
			value = ( ( 1 << N ) == Size ) * N + which_power_of_two< N - 1, Size >::value
		};
	};

	template< size_t Size >
	struct which_power_of_two< 0, Size >
	{
		enum {
			value = 0
		};
	};
	
	template< size_t N >
	struct is_power_of_two
	{
		enum {
			value = ( N >= 1 ) & !( N & ( N - 1 ) )
		};
	};
}

template < typename Type, size_t Size >
class ringbuffer
{
	public:
		
		static_assert( helper::is_power_of_two< Size >::value, "size of ringbuffer has to be a power of two" );

		static const size_t Power = helper::which_power_of_two< sizeof( Size ) * 8, Size >::value;
	
		using storage_type = std::vector< Type >;
		using value_type = Type;
	
		class iterator : public std::iterator< std::bidirectional_iterator_tag, value_type >
		{
			public:
				iterator( value_type *b, size_t offset ) :
					base_( b ),
					offset_( offset ) {}
			
				value_type& operator* ()
				{
					return base_[ offset_ ];
				}
			
				iterator& operator++ ()
				{
					++offset_;
					return *this;
				}
			
				iterator operator++ (int)
				{
					auto result = *this;
					operator++();
					return result;
				}
			
				iterator& operator-- ()
				{
					--offset_;
					return *this;
				}
			
				iterator operator-- (int)
				{
					auto result = *this;
					operator--();
					return result;
				}
			
				bool operator != ( const iterator &rhs ) const
				{
					return offset_ != rhs.offset_;
				}
	
			private:
			
				value_type *base_;
				size_t offset_ : Power;
		};
	
		template < typename ...Parameters >
		ringbuffer( Parameters ...p ) :
			storage_( Size ),
			newest_( storage_.data(), 0 ),
			oldest_( storage_.data(), 0 ),
			used_( std::distance( newest_, oldest_ ) )
		{
			
		}
	
		value_type& front()
		{
			return *oldest_;
		}
	
		template < typename T >
		void push_back( T &&t )
		{
			if ( used_ >= capacity() )
			{
				throw std::out_of_range( "ringbuffer can not hold more than " + std::to_string( Size ) + " values" );
			}
			auto &destination = *newest_;
			destination = std::forward< T >( t );
			++newest_;
			++used_;
		}
	
		size_t pop_front( value_type &v )
		{
			if ( used_ == 0 )
			{
				throw std::out_of_range( "ringbuffer is already empty" );
			}
			std::swap( v, *oldest_ );
			++oldest_;
			return --used_;
		}
	
		iterator begin()
		{
			return oldest_;
		}
	
		iterator end()
		{
			return newest_;
		}
	
		size_t size() const
		{
			return used_;
		}
	
		size_t capacity() const
		{
			return storage_.size();
		}
	
	private:
	
		storage_type storage_;
		iterator newest_, oldest_;
		size_t used_;
};