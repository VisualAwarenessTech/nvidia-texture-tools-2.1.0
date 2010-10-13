// This code is in the public domain -- Ignacio Casta�o <castano@gmail.com>

#pragma once
#ifndef NV_CORE_ARRAY_H
#define NV_CORE_ARRAY_H

/*
This array class requires the elements to be relocable; it uses memmove and realloc. Ideally I should be 
using swap, but I honestly don't care.

The foreach macros that I use are very non-standard and somewhat confusing. It would be nice to have
standard foreach as in Qt.
*/


#include "nvcore.h"
#include "Memory.h"
#include "Debug.h"
#include "Stream.h"
#include "Utils.h" // swap

#include <string.h>	// memmove
#include <new> // for placement new


#if NV_CC_GNUC // If typeof is available:

#define NV_FOREACH(i, container) \
    typedef typeof(container) NV_STRING_JOIN2(cont,__LINE__); \
    for(NV_STRING_JOIN2(cont,__LINE__)::PseudoIndex i((container).start()); !(container).isDone(i); (container).advance(i))
/*
#define NV_FOREACH(i, container) \
for(typename typeof(container)::PseudoIndex i((container).start()); !(container).isDone(i); (container).advance(i))
*/

#else // If typeof not available:

struct PseudoIndexWrapper {
    template <typename T>
    PseudoIndexWrapper(const T & container) {
        nvStaticCheck(sizeof(typename T::PseudoIndex) <= sizeof(memory));
        new (memory) typename T::PseudoIndex(container.start());
    }
    // PseudoIndex cannot have a dtor!

    template <typename T> typename T::PseudoIndex & operator()(const T * container) {
        return *reinterpret_cast<typename T::PseudoIndex *>(memory);
    }
    template <typename T> const typename T::PseudoIndex & operator()(const T * container) const {
        return *reinterpret_cast<const typename T::PseudoIndex *>(memory);
    }

    uint8 memory[4];	// Increase the size if we have bigger enumerators.
};

#define NV_FOREACH(i, container) \
    for(PseudoIndexWrapper i(container); !(container).isDone(i(&(container))); (container).advance(i(&(container))))

#endif

// Declare foreach keyword.
#if !defined NV_NO_USE_KEYWORDS
#   define foreach NV_FOREACH
#endif



namespace nv 
{
    /// Delete all the elements of a container.
    template <typename T>
    void deleteAll(T & container)
    {
        for (typename T::PseudoIndex i = container.start(); !container.isDone(i); container.advance(i))
        {
            delete container[i];
        }
    }



    /**
    * Replacement for std::vector that is easier to debug and provides
    * some nice foreach enumerators. 
    */
    template<typename T>
    class NVCORE_CLASS Array {
    public:

        /// Ctor.
        Array() : m_buffer(NULL), m_size(0), m_buffer_size(0)
        {
        }

        /// Copy ctor.
        Array( const Array & a ) : m_buffer(NULL), m_size(0), m_buffer_size(0)
        {
            copy(a.m_buffer, a.m_size); 
        }

        /// Ctor that initializes the vector with the given elements.
        Array( const T * ptr, int num ) : m_buffer(NULL), m_size(0), m_buffer_size(0)
        {
            copy(ptr, num);
        }

        /// Allocate array.
        explicit Array(uint capacity) : m_buffer(NULL), m_size(0), m_buffer_size(0)
        {
            allocate(capacity);
        }


        /// Dtor.
        ~Array()
        {
            clear();
            allocate(0);
        }


        /// Const element access.
        const T & operator[]( uint index ) const
        {
            nvDebugCheck(index < m_size);
            return m_buffer[index];
        }
        const T & at( uint index ) const
        {
            nvDebugCheck(index < m_size);
            return m_buffer[index];
        }

        /// Element access.
        T & operator[] ( uint index )
        {
            nvDebugCheck(index < m_size);
            return m_buffer[index];
        }
        T & at( uint index )
        {
            nvDebugCheck(index < m_size);
            return m_buffer[index];
        }

        /// Get vector size.
        uint size() const { return m_size; }

        /// Get vector size.
        uint count() const { return m_size; }

        /// Get const vector pointer.
        const T * buffer() const { return m_buffer; }

        /// Get vector pointer.
        T * mutableBuffer() { return m_buffer; }

        /// Is vector empty.
        bool isEmpty() const { return m_size == 0; }

        /// Is a null vector.
        bool isNull() const	{ return m_buffer == NULL; }


        /// Push an element at the end of the vector.
        void push_back( const T & val )
        {
            uint new_size = m_size + 1;

            if (new_size > m_buffer_size)
            {
                const T copy(val);	// create a copy in case value is inside of this array.
                resize(new_size);
                m_buffer[new_size-1] = copy;
            }
            else
            {
                m_size = new_size;
                new(m_buffer+new_size-1) T(val);
            }
        }
        void pushBack( const T & val )
        {
            push_back(val);
        }
        void append( const T & val )
        {
            push_back(val);
        }

        /// Qt like push operator.
        Array<T> & operator<< ( T & t )
        {
            push_back(t);
            return *this;
        }

        /// Pop and return element at the end of the vector.
        void pop_back()
        {
            nvDebugCheck( m_size > 0 );
            resize( m_size - 1 );
        }
        void popBack()
        {
            pop_back();
        }

        /// Get back element.
        const T & back() const
        {
            nvDebugCheck( m_size > 0 );
            return m_buffer[m_size-1];
        }

        /// Get back element.
        T & back()
        {
            nvDebugCheck( m_size > 0 );
            return m_buffer[m_size-1];
        }

        /// Get front element.
        const T & front() const
        {
            nvDebugCheck( m_size > 0 );
            return m_buffer[0];
        }

        /// Get front element.
        T & front()
        {
            nvDebugCheck( m_size > 0 );
            return m_buffer[0];
        }

        /// Return true if element found.
        bool find(const T & element, uint * index) const
        {
            return find(element, 0, m_size, index);
        }

        /// Return true if element found within the given range.
        bool find(const T & element, uint first, uint count, uint * index) const
        {
            for (uint i = first; i < first+count; i++) {
                if (m_buffer[i] == element) {
                    if (index != NULL) *index = i;
                    return true;
                }
            }
            return false;
        }

        /// Check if the given element is contained in the array.
        bool contains(const T & e) const
        {
            return find(e, NULL);
        }

        /// Remove the element at the given index. This is an expensive operation!
        void removeAt( uint index )
        {
            nvCheck(index >= 0 && index < m_size);

            if( m_size == 1 ) {
                clear();
            }
            else {
                m_buffer[index].~T();

                memmove( m_buffer+index, m_buffer+index+1, sizeof(T) * (m_size - 1 - index) );
                m_size--;
            }
        }

        /// Remove the first instance of the given element.
        bool remove(const T & element)
        {
            uint index;
            if (find(element, &index)) {
                removeAt(index);
                return true;
            }
            return false;
        }

        /// Insert the given element at the given index shifting all the elements up.
        void insertAt( uint index, const T & val = T() )
        {
            nvCheck( index >= 0 && index <= m_size );

            resize( m_size + 1 );

            if( index < m_size - 1 ) {
                memmove( m_buffer+index+1, m_buffer+index, sizeof(T) * (m_size - 1 - index) );
            }

            // Copy-construct into the newly opened slot.
            new(m_buffer+index) T(val);
        }

        /// Append the given data to our vector.
        void append(const Array<T> & other)
        {
            append(other.m_buffer, other.m_size);
        }

        /// Append the given data to our vector.
        void append(const T other[], uint count)
        {
            if( count > 0 ) {
                const uint old_size = m_size;
                resize(m_size + count);
                // Must use operator=() to copy elements, in case of side effects (e.g. ref-counting).
                for( uint i = 0; i < count; i++ ) {
                    m_buffer[old_size + i] = other[i];
                }
            }
        }


        /// Remove the given element by replacing it with the last one.
        void replaceWithLast(uint index)
        {
            nvDebugCheck( index < m_size );
            swap(m_buffer[index], back());
            //m_buffer[index] = back();
            (m_buffer+m_size-1)->~T();
            m_size--;
        }


        /// Resize the vector preserving existing elements.
        NV_NOINLINE void resize(uint new_size)
        {
            uint i;
            uint old_size = m_size;
            m_size = new_size;

            // Destruct old elements (if we're shrinking).
            for( i = new_size; i < old_size; i++ ) {
                (m_buffer+i)->~T();                         // Explicit call to the destructor
            }

            if( m_size == 0 ) {
                //allocate(0);	// Don't shrink automatically.
            }
            else if( m_size <= m_buffer_size/* && m_size > m_buffer_size >> 1*/) {
                // don't compact yet.
                nvDebugCheck(m_buffer != NULL);
            }
            else {
                uint new_buffer_size;
                if( m_buffer_size == 0 ) {
                    // first allocation
                    new_buffer_size = m_size;
                }
                else {
                    // growing
                    new_buffer_size = m_size + (m_size >> 2);
                }
                allocate( new_buffer_size );
            }

            // Call default constructors
            for( i = old_size; i < new_size; i++ ) {
                new(m_buffer+i) T;	// placement new
            }
        }


        /// Resize the vector preserving existing elements and initializing the
        /// new ones with the given value.
        NV_NOINLINE void resize( uint new_size, const T &elem )
        {
            uint i;
            uint old_size = m_size;
            m_size = new_size;

            // Destruct old elements (if we're shrinking).
            for( i = new_size; i < old_size; i++ ) {
                (m_buffer+i)->~T();                         // Explicit call to the destructor
            }

            if( m_size == 0 ) {
                //allocate(0);	// Don't shrink automatically.
            }
            else if( m_size <= m_buffer_size && m_size > m_buffer_size >> 1 ) {
                // don't compact yet.
            }
            else {
                uint new_buffer_size;
                if( m_buffer_size == 0 ) {
                    // first allocation
                    new_buffer_size = m_size;
                }
                else {
                    // growing
                    new_buffer_size = m_size + (m_size >> 2);
                }
                allocate( new_buffer_size );
            }

            // Call copy constructors
            for( i = old_size; i < new_size; i++ ) {
                new(m_buffer+i) T( elem );	// placement new
            }
        }

        /// Clear the buffer.
        void clear()
        {
            resize(0);
        }

        /// Shrink the allocated vector.
        void shrink()
        {
            if (m_size < m_buffer_size) {
                allocate(m_size);
            }
        }

        /// Preallocate space.
        void reserve(uint desired_size)
        {
            if (desired_size > m_buffer_size) {
                allocate( desired_size );
            }
        }

        /// Copy elements to this array. Resizes it if needed.
        void copy(const T * ptr, uint num)
        {
            resize( num );
            for (uint i = 0; i < m_size; i++) {
                m_buffer[i] = ptr[i];
            }
        }

        /// Assignment operator.
        Array<T> & operator=( const Array<T> & a )
        {
            copy(a.m_buffer, a.m_size);
            return *this;
        }


        /// Array serialization.
        friend Stream & operator<< ( Stream & s, Array<T> & p )
        {
            if( s.isLoading() ) {
                uint size;
                s << size;
                p.resize( size );
            }
            else {
                s << p.m_size;
            }

            for( uint i = 0; i < p.m_size; i++ ) {
                s << p.m_buffer[i];
            }

            return s;
        }


        // Array enumerator.
        typedef uint PseudoIndex;

        PseudoIndex start() const { return 0; }
        bool isDone(const PseudoIndex & i) const { nvDebugCheck(i <= this->m_size); return i == this->m_size; };
        void advance(PseudoIndex & i) const { nvDebugCheck(i <= this->m_size); i++; }

#if NV_CC_MSVC
        T & operator[]( const PseudoIndexWrapper & i ) {
            return m_buffer[i(this)];
        }
        const T & operator[]( const PseudoIndexWrapper & i ) const {
            return m_buffer[i(this)];		
        }
#endif


        /// Swap the members of this vector and the given vector.
        friend void swap(Array<T> & a, Array<T> & b)
        {
            swap(a.m_buffer, b.m_buffer);
            swap(a.m_size, b.m_size);
            swap(a.m_buffer_size, b.m_buffer_size);
        }


    private:

        /// Change buffer size.
        NV_NOINLINE void allocate( uint rsize )
        {
            m_buffer_size = rsize;

            // free the buffer.
            if (m_buffer_size == 0) {
                if (m_buffer) {
                    free( m_buffer );
                    m_buffer = NULL;
                }
            }

            // realloc the buffer
            else {
                m_buffer = (T *) realloc( m_buffer, sizeof(T) * m_buffer_size );
            }
        }


    private:
        T * m_buffer;
        uint m_size;
        uint m_buffer_size;
    };

} // nv namespace

#endif // NV_CORE_ARRAY_H