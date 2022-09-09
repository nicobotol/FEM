/*=============================================================================
					up entertainment, all rights reserved
=============================================================================*/

#ifndef __LIST_H__
#define __LIST_H__


/*
===============================================================================

	List template
	Does not allocate memory until the first item is added.

===============================================================================
*/



/*
================
fListSortCompare<type>
================
*/

template< class type >
inline int fListSortCompare( const type *a, const type *b ) {
	return type(*a) - type(*b);
}

/*
================
fListNewElement<type>
================
*/
template< class type >
inline type *fListNewElement( void ) {
	return new type;
}

/*
================
fSwap<type>
================
*/
template< class type >
inline void fSwap( type &a, type &b ) {
	type c = a;
	a = b;
	b = c;
}

template< class type >
class fList {
public:

	typedef int		cmp_t( const type *, const type * );
	typedef type	new_t( void );

					fList( int newgranularity = 16 );
					fList( const fList<type> &other );
					~fList<type>( void );

	void			Clear( void );										// clear the list
	int				Num( void ) const;									// returns number of elements in list
	int				NumAllocated( void ) const;							// returns number of elements allocated for
	void			SetGranularity( int newgranularity );				// set new granularity
	int				GetGranularity( void ) const;						// get the current granularity

	size_t			Allocated( void ) const;							// returns total size of allocated memory
	size_t			Size( void ) const;									// returns total size of allocated memory including size of list type
	size_t			MemoryUsed( void ) const;							// returns size of the used elements in the list

	fList<type> &	operator=( const fList<type> &other );
	const type &	operator[]( int index ) const;
	type &			operator[]( int index );
	bool			operator==( const fList<type> &other );

	void			SetVal( const type &value );

	void			Condense( void );									// resizes list to exactly the number of elements it contains
	void			Resize( int newsize );								// resizes list to the given number of elements
	void			Resize( int newsize, int newgranularity	 );			// resizes list and sets new granularity
	void			SetNum( int newnum, bool resize = false );			// set number of elements in list and resize to exactly this number if necessary
	void			AssureSize( int newSize);							// assure list has given number of elements, but leave them uninitialized
	void			AssureSize( int newSize, const type &initValue );	// assure list has given number of elements and initialize any new elements
	void			AssureSizeAlloc( int newSize, new_t *allocator );	// assure the pointer list has the given number of elements and allocate any new elements

	type *			Ptr( void );										// returns a pointer to the list
	const type *	Ptr( void ) const;									// returns a pointer to the list
	type &			Alloc( void );										// returns reference to a new data element at the end of the list
	int				Append( const type & obj );							// append element
	int				Append( const fList<type> &other );					// append list
	int				AddUnique( const type & obj );						// add unique element
	int				Insert( const type & obj, int index = 0 );			// insert the element at the given index
	int				FindIndex( const type & obj ) const;				// find the index for the given element
	type *			Find( type const & obj ) const;						// find pointer to the given element
	int				IndexOf( const type *obj ) const;					// returns the index for the pointer to an element in the list
	bool			RemoveIndex( int index );							// remove the element at the given index
	bool			Remove( const type & obj );							// remove the element
	void			Sort( cmp_t *compare = ( cmp_t * )&fListSortCompare<type> );
	void			SortSubSection( int startIndex, int endIndex, cmp_t *compare = ( cmp_t * )&fListSortCompare<type> );
	void			Swap( fList<type> &other );							// swap the contents of the lists
	//void			DeleteContents( bool clear );						// delete the contents of the list

private:
	int				num;
	int				size;
	int				granularity;
	type *			list;
};

/*
================
fList<type>::fList( int )
================
*/
template< class type >
inline fList<type>::fList( int newgranularity ) {
    
	list		= NULL;
	granularity	= newgranularity;
	Clear();
}

/*
================
fList<type>::fList( const fList<type> &other )
================
*/
template< class type >
inline fList<type>::fList( const fList<type> &other ) {
	list = NULL;
	*this = other;
}

/*
================
fList<type>::~fList<type>
================
*/
template< class type >
inline fList<type>::~fList( void ) {
	Clear();
}

/*
================
fList<type>::Clear

Frees up the memory allocated by the list.  Assumes that type automatically handles freeing up memory.
================
*/
template< class type >
inline void fList<type>::Clear( void ) {
	if ( list ) {
		delete[] list;
	}

	list	= NULL;
	num		= 0;
	size	= 0;
}

/*
================
fList<type>::DeleteContents

Calls the destructor of all elements in the list.  Conditionally frees up memory used by the list.
Note that this only works on lists containing pointers to objects and will cause a compiler error
if called with non-pointers.  Since the list was not responsible for allocating the object, it has
no information on whether the object still exists or not, so care must be taken to ensure that
the pointers are still valid when this function is called.  Function will set all pointers in the
list to NULL.
================
*/
/*
template< class type >
inline void fList<type>::DeleteContents( bool clear ) {
	int i;

	for( i = 0; i < num; i++ ) {
		delete list[ i ];
		list[ i ] = NULL;
	}

	if ( clear ) {
		Clear();
	} else {
		memset( list, 0, size * sizeof( type ) );
	}
}
*/

/*
================
fList<type>::Allocated

return total memory allocated for the list in bytes, but doesn't take into account additional memory allocated by type
================
*/
template< class type >
inline size_t fList<type>::Allocated( void ) const {
	return size * sizeof( type );
}

/*
================
fList<type>::Size

return total size of list in bytes, but doesn't take into account additional memory allocated by type
================
*/
template< class type >
inline size_t fList<type>::Size( void ) const {
	return sizeof( fList<type> ) + Allocated();
}

/*
================
fList<type>::MemoryUsed
================
*/
template< class type >
inline size_t fList<type>::MemoryUsed( void ) const {
	return num * sizeof( *list );
}

/*
================
fList<type>::Num

Returns the number of elements currently contained in the list.
Note that this is NOT an indication of the memory allocated.
================
*/
template< class type >
inline int fList<type>::Num( void ) const {
	return num;
}

/*
================
fList<type>::NumAllocated

Returns the number of elements currently allocated for.
================
*/
template< class type >
inline int fList<type>::NumAllocated( void ) const {
	return size;
}

/*
================
fList<type>::SetNum

Resize to the exact size specified irregardless of granularity
================
*/
template< class type >
inline void fList<type>::SetNum( int newnum, bool resize ) {

	if ( resize || newnum > size ) {
		Resize( newnum );
	}
	num = newnum;
}

/*
================
fList<type>::SetGranularity

Sets the base size of the array and resizes the array to match.
================
*/
template< class type >
inline void fList<type>::SetGranularity( int newgranularity ) {
	int newsize;

	granularity = newgranularity;

	if ( list ) {
		// resize it to the closest level of granularity
		newsize = num + granularity - 1;
		newsize -= newsize % granularity;
		if ( newsize != size ) {
			Resize( newsize );
		}
	}
}

/*
================
fList<type>::GetGranularity

Get the current granularity.
================
*/
template< class type >
inline int fList<type>::GetGranularity( void ) const {
	return granularity;
}

/*
================
fList<type>::Condense

Resizes the array to exactly the number of elements it contains or frees up memory if empty.
================
*/
template< class type >
inline void fList<type>::Condense( void ) {
	if ( list ) {
		if ( num ) {
			Resize( num );
		} else {
			Clear();
		}
	}
}

/*
================
fList<type>::Resize

Allocates memory for the amount of elements requested while keeping the contents intact.
Contents are copied using their = operator so that data is correnctly instantiated.
================
*/
template< class type >
inline void fList<type>::Resize( int newsize ) {
	type	*temp;
	int		i;

	// free up the list if no data is being reserved
	if ( newsize <= 0 ) {
		Clear();
		return;
	}

	if ( newsize == size ) {
		// not changing the size, so just exit
		return;
	}

	temp	= list;
	size	= newsize;
	if ( size < num ) {
		num = size;
	}

	// copy the old list into our new one
	list = new type[ size ];
	for( i = 0; i < num; i++ ) {
		list[ i ] = temp[ i ];
	}

	// delete the old list if it exists
	if ( temp ) {
		delete[] temp;
	}
}

/*
================
fList<type>::Resize

Allocates memory for the amount of elements requested while keeping the contents intact.
Contents are copied using their = operator so that data is correnctly instantiated.
================
*/
template< class type >
inline void fList<type>::Resize( int newsize, int newgranularity ) {
	type	*temp;
	int		i;

	granularity = newgranularity;

	// free up the list if no data is being reserved
	if ( newsize <= 0 ) {
		Clear();
		return;
	}

	temp	= list;
	size	= newsize;
	if ( size < num ) {
		num = size;
	}

	// copy the old list into our new one
	list = new type[ size ];
	for( i = 0; i < num; i++ ) {
		list[ i ] = temp[ i ];
	}

	// delete the old list if it exists
	if ( temp ) {
		delete[] temp;
	}
}

/*================
fList<type>::AssureSize

Makes sure the list has at least the given number of elements.
================*/
template< class type >
inline void fList<type>::AssureSize( int newSize ) {
	int newNum = newSize;

	if ( newSize > size ) {
		newSize += granularity - 1;
		newSize -= newSize % granularity;
		Resize( newSize );
	}

	num = newNum;
}


template< class type >
inline void fList<type>::SetVal( const type &value ) {
	for ( int i = 0; i < num; i++ ) {
		list[i] = value;
	}
}


/*================
fList<type>::AssureSize

Makes sure the list has at least the given number of elements and initialize any elements not yet initialized.
================*/
template< class type >
inline void fList<type>::AssureSize( int newSize, const type &initValue ) {
	int newNum = newSize;

	if ( newSize > size ) {
		newSize += granularity - 1;
		newSize -= newSize % granularity;
		num = size;
		Resize( newSize );

		for ( int i = num; i < newSize; i++ ) {
			list[i] = initValue;
		}
	}

	num = newNum;
}

/*
================
fList<type>::AssureSizeAlloc

Makes sure the list has at least the given number of elements and allocates any elements using the allocator.

NOTE: This function can only be called on lists containing pointers. Calling it
on non-pointer lists will cause a compiler error.
================
*/
template< class type >
inline void fList<type>::AssureSizeAlloc( int newSize, new_t *allocator ) {
	int newNum = newSize;

	if ( newSize > size ) {

		if ( granularity == 0 ) {	// this is a hack to fix our memset classes
			granularity = 16;
		}

		newSize += granularity - 1;
		newSize -= newSize % granularity;
		num = size;
		Resize( newSize );

		for ( int i = num; i < newSize; i++ ) {
			list[i] = (*allocator)();
		}
	}

	num = newNum;
}

/*
================
fList<type>::operator=

Copies the contents and size attributes of another list.
================
*/
template< class type >
inline fList<type> &fList<type>::operator=( const fList<type> &other ) {
	int	i;

	Clear();

	num			= other.num;
	size		= other.size;
	granularity	= other.granularity;

	if ( size ) {
		list = new type[ size ];
		for( i = 0; i < num; i++ ) {
			list[ i ] = other.list[ i ];
		}
	}

	return *this;
}


/*
================
fList<type>::operator==

Coppares the contents of the lists.
================
*/
template< class type >
inline bool fList<type>::operator==( const fList<type> &other ) {
	// Compare lists auxiliary variables
	if ( num != other.num ) 
		return false;
	if ( size != other.size ) 
		return false;
	if ( granularity != other.granularity ) 
		return false;

	// Compare the contents of the lists
	for( int i = 0; i < num; i++ ) {
		if ( !( list[i] == other.list[i] ) )
			return false;
	}

	return true;
}

/*
================
fList<type>::operator[] const

Access operator.  Index must be within range or an assert will be issued in debug builds.
Release builds do no range checking.
================
*/
template< class type >
inline const type &fList<type>::operator[]( int index ) const {
	return list[ index ];
}

/*
================
fList<type>::operator[]

Access operator.  Index must be within range or an assert will be issued in debug builds.
Release builds do no range checking.
================
*/
template< class type >
inline type &fList<type>::operator[]( int index ) {
	return list[ index ];
}

/*
================
fList<type>::Ptr

Returns a pointer to the begining of the array.  Useful for iterating through the list in loops.

Note: may return NULL if the list is empty.

FIXME: Create an iterator template for this kind of thing.
================
*/
template< class type >
inline type *fList<type>::Ptr( void ) {
	return list;
}

/*
================
fList<type>::Ptr

Returns a pointer to the begining of the array.  Useful for iterating through the list in loops.

Note: may return NULL if the list is empty.

FIXME: Create an iterator template for this kind of thing.
================
*/
template< class type >
const inline type *fList<type>::Ptr( void ) const {
	return list;
}

/*
================
fList<type>::Alloc

Returns a reference to a new data element at the end of the list.
================
*/
template< class type >
inline type &fList<type>::Alloc( void ) {
	if ( !list ) {
		Resize( granularity );
	}

	if ( num == size ) {
		Resize( size + granularity );
	}

	return list[ num++ ];
}

/*
================
fList<type>::Append

Increases the size of the list by one element and copies the supplied data into it.

Returns the index of the new element.
================
*/
template< class type >
inline int fList<type>::Append( type const & obj ) {
	if ( !list ) {
		Resize( granularity );
	}

	if ( num == size ) {
		int newsize;

		if ( granularity == 0 ) {	// this is a hack to fix our memset classes
			granularity = 16;
		}
		newsize = size + granularity;
		Resize( newsize - newsize % granularity );
	}

	list[ num ] = obj;
	num++;

	return num - 1;
}


/*
================
fList<type>::Insert

Increases the size of the list by at leat one element if necessary 
and inserts the supplied data into it.

Returns the index of the new element.
================
*/
template< class type >
inline int fList<type>::Insert( type const & obj, int index ) {
	if ( !list ) {
		Resize( granularity );
	}

	if ( num == size ) {
		int newsize;

		if ( granularity == 0 ) {	// this is a hack to fix our memset classes
			granularity = 16;
		}
		newsize = size + granularity;
		Resize( newsize - newsize % granularity );
	}

	if ( index < 0 ) {
		index = 0;
	}
	else if ( index > num ) {
		index = num;
	}
	for ( int i = num; i > index; --i ) {
		list[i] = list[i-1];
	}
	num++;
	list[index] = obj;
	return index;
}

/*
================
fList<type>::Append

adds the other list to this one

Returns the size of the new combined list
================
*/
template< class type >
inline int fList<type>::Append( const fList<type> &other ) {
	if ( !list ) {
		if ( granularity == 0 ) {	// this is a hack to fix our memset classes
			granularity = 16;
		}
		Resize( granularity );
	}

	int n = other.Num();
	for (int i = 0; i < n; i++) {
		Append(other[i]);
	}

	return Num();
}

/*
================
fList<type>::AddUnique

Adds the data to the list if it doesn't already exist.  Returns the index of the data in the list.
================
*/
template< class type >
inline int fList<type>::AddUnique( type const & obj ) {
	int index;

	index = FindIndex( obj );
	if ( index < 0 ) {
		index = Append( obj );
	}

	return index;
}

/*
================
fList<type>::FindIndex

Searches for the specified data in the list and returns it's index.  Returns -1 if the data is not found.
================
*/
template< class type >
inline int fList<type>::FindIndex( type const & obj ) const {
	int i;

	for( i = 0; i < num; i++ ) {
		if ( list[ i ] == obj ) {
			return i;
		}
	}

	// Not found
	return -1;
}

/*
================
fList<type>::Find

Searches for the specified data in the list and returns it's address. Returns NULL if the data is not found.
================
*/
template< class type >
inline type *fList<type>::Find( type const & obj ) const {
	int i;

	i = FindIndex( obj );
	if ( i >= 0 ) {
		return &list[ i ];
	}

	return NULL;
}


/*
================
fList<type>::IndexOf

Takes a pointer to an element in the list and returns the index of the element.
This is NOT a guarantee that the object is really in the list. 
Function will assert in debug builds if pointer is outside the bounds of the list,
but remains silent in release builds.
================
*/
template< class type >
inline int fList<type>::IndexOf( type const *objptr ) const {
	int index;

	index = objptr - list;

	return index;
}

/*
================
fList<type>::RemoveIndex

Removes the element at the specified index and moves all data following the element down to fill in the gap.
The number of elements in the list is reduced by one.  Returns false if the index is outside the bounds of the list.
Note that the element is not destroyed, so any memory used by it may not be freed until the destruction of the list.
================
*/
template< class type >
inline bool fList<type>::RemoveIndex( int index ) {
	int i;

	if ( ( index < 0 ) || ( index >= num ) ) {
		return false;
	}

	num--;
	for( i = index; i < num; i++ ) {
		list[ i ] = list[ i + 1 ];
	}

	return true;
}

/*
================
fList<type>::Remove

Removes the element if it is found within the list and moves all data following the element down to fill in the gap.
The number of elements in the list is reduced by one.  Returns false if the data is not found in the list.  Note that
the element is not destroyed, so any memory used by it may not be freed until the destruction of the list.
================
*/
template< class type >
inline bool fList<type>::Remove( type const & obj ) {
	int index;

	index = FindIndex( obj );
	if ( index >= 0 ) {
		return RemoveIndex( index );
	}
	
	return false;
}

/*
================
fList<type>::Sort

Performs a qsort on the list using the supplied comparison function.  Note that the data is merely moved around the
list, so any pointers to data within the list may no longer be valid.
================
*/
template< class type >
inline void fList<type>::Sort( cmp_t *compare ) {
	if ( !list ) {
		return;
	}
	typedef int cmp_c(const void *, const void *);

	cmp_c *vCompare = (cmp_c *)compare;
	qsort( ( void * )list, ( size_t )num, sizeof( type ), vCompare );
}

/*
================
fList<type>::SortSubSection

Sorts a subsection of the list.
================
*/
template< class type >
inline void fList<type>::SortSubSection( int startIndex, int endIndex, cmp_t *compare ) {
	if ( !list ) {
		return;
	}
	if ( startIndex < 0 ) {
		startIndex = 0;
	}
	if ( endIndex >= num ) {
		endIndex = num - 1;
	}
	if ( startIndex >= endIndex ) {
		return;
	}
	typedef int cmp_c(const void *, const void *);

	cmp_c *vCompare = (cmp_c *)compare;
	qsort( ( void * )( &list[startIndex] ), ( size_t )( endIndex - startIndex + 1 ), sizeof( type ), vCompare );
}

/*
================
fList<type>::Swap

Swaps the contents of two lists
================
*/
template< class type >
inline void fList<type>::Swap( fList<type> &other ) {
	fSwap( num, other.num );
	fSwap( size, other.size );
	fSwap( granularity, other.granularity );
	fSwap( list, other.list );
}

#endif
