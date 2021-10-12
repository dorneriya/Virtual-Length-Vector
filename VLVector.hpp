/**
 * @file    VLVector.hpp
 * @author  Dor Neriya
 * 
 *
 * @brief   Virtual Length Vector.
 */

#include <iostream>

#define DEFAULT_STATIC_CAPACITY 16

static size_t capfunc(size_t s, int stat, size_t nowCap);

/**
 * @brief needed date structure
 * @tparam T generic type
 * @tparam StaticCapacity
 */
template<class T, unsigned long StaticCapacity = DEFAULT_STATIC_CAPACITY>
class VLVector
{
private:
	size_t _size;
	size_t _capacity;
	T *heapArr;
	T stackArr[StaticCapacity];
	
	void _reCap(size_t preSize);

public:
	/**
	 * iterator traits
	 */
	typedef T *iterator;
	typedef const T *const_iterator;
	typedef T value_type;
	typedef T &reference;
	typedef const T &const_reference;
	typedef T *pointer;
	typedef const T *const_pointer;
	typedef size_t difference_type;
	typedef std::random_access_iterator_tag iterator_category;
	
	/**
	 * @brief default constructor - creates a size 0 vector
	 */
	VLVector() : _size(0), _capacity(StaticCapacity), heapArr(nullptr) {};
	
	/**
	 * @brief destructor - if the vector was longer than the static size,
	 * we release the dynamic allocated memory
	 */
	~VLVector()
	{
		if (_capacity > StaticCapacity)
		{
			delete[] (heapArr);
		}
	}
	
	/**
	 * @brief Creates a vector from a section of an iterative data structure
	 * @tparam InputIterator the type of the iterator that holds the data to insert into a vector
	 * @param first iterator to the first element in the section
	 * @param last iterator to the last element in the section
	 */
	template<class InputIterator>
	VLVector(InputIterator const &first, InputIterator const &last): VLVector()
	{
		for (auto in = first; in != last; in++)
		{
			push_back(*in);
		}
	}
	
	/**
	 * @brief copy constractor
	 * @param other the vector to be copied into a new vector
	 */
	VLVector(VLVector const &other) : VLVector(other.begin(), other.end()) {};
	
	/**
	 * @brief define operator '=' for vector assignment
	 * @param rhs right hand side
	 * @return reference to the result vector
	 */
	VLVector &operator=(VLVector const &rhs)
	{
		if (&rhs == *this)
		{
			return *this;
		}
		this->clear();
		*this = VLVector(rhs);
		return *this;
	}
	
	/**
	 * @brief getter for size attribute
	 * @return size
	 */
	size_t size() const { return _size; }
	
	/**
	 * @brief append the new element to the end of the vector
	 * @param add element to add
	 */
	void push_back(const T &add)
	{
		_size++;
		_reCap(_size - 1);
		if (_capacity > StaticCapacity)
		{
			heapArr[_size - 1] = add;
		}
		else
		{
			stackArr[_size - 1] = add;
		}
	}
	
	/**
	 * @brief getter for capacity attribute
	 * @return capacity
	 */
	size_t capacity() const { return _capacity; }
	
	/**
	 * @brief checks if the vector is empty
	 * @return if empty - true, otherwise - false
	 */
	bool empty() const { return _size == 0; }
	
	/**
	 * @brief remove the last element in the vector if exists
	 */
	void pop_back()
	{
		if (_size == 0)
		{
			return;
		}
		_size--;
		_reCap(_size + 1);
	}
	
	/**
	 * @brief empty the vector, release allocated memory if needed
	 */
	void clear()
	{
		if (_size == 0)
		{
			return;
		}
		size_t preSize = _size;
		_size = 0;
		_reCap(preSize);
	}
	
	/**
	 * @brief Gives read-only access to information contained in Vector
	 * @return returns a pointer to the data type that holds the information within the vector
	 */
	const T *data() const noexcept
	{
		if (_capacity > StaticCapacity)
		{
			return heapArr;
		}
		else
		{
			return stackArr;
		}
	}
	
	/**
	 * @brief Gives full access to information contained in Vector
	 * @return returns a pointer to the data type that holds the information within the vector
	 */
	T *data() noexcept
	{
		if (_capacity > StaticCapacity)
		{
			return heapArr;
		}
		else
		{
			return stackArr;
		}
	}
	
	/**
	 * @brief access the requested index and returns the value found in it
	 * @param idx index to access
	 * @return value in given access
	 */
	T &operator[](const size_t &idx)
	{
		if (_capacity > StaticCapacity)
		{
			return heapArr[idx];
		}
		else
		{
			return stackArr[idx];
		}
	}
	
	/**
	 * @brief access the requested index and returns the value found in it
	 * @param idx index to access
	 * @return read-only value in given access
	 */
	const T &operator[](const size_t &idx) const
	{
		if (_capacity > StaticCapacity)
		{
			return heapArr[idx];
		}
		else
		{
			return stackArr[idx];
		}
	}
	
	/**
	 * @brief access the requested index and returns the value found in it,
	 * while verifying that the index is in the vector range
	 * @param idx index to access
	 * @return read-only value in given access
	 */
	const T &at(const size_t idx) const
	{
		if (idx < _size)
		{
			return (*this)[idx];
		}
		else
		{
			throw std::out_of_range("index out of range");
		}
	}
	
	/**
	 * @brief access the requested index and returns the value found in it,
	 * while verifying that the index is in the vector range
	 * @param idx index to access
	 * @return value in given access
	 */
	T &at(const size_t idx)  //
	{
		if (idx < _size)
		{
			return (*this)[idx];
		}
		else
		{
			throw std::out_of_range("index out of range");
		}
	}
	
	/**
	 * @brief Define a comparison between vectors
	 * @param rhs right hand size vector to compere to
	 * @return if equal - true otherwise - false
	 */
	bool operator==(const VLVector &rhs) const
	{
		if (_size != rhs._size)
		{
			return false;
		}
		for (size_t i = 0; i < _size; i++)
		{
			if ((*this)[i] != rhs[i])
			{
				return false;
			}
		}
		return true;
	}
	
	/**
	 * @brief use the comparison between vectors function to assess if the
	 * vectors are not equal
	 * @param rhs right hand size vector to compere to
	 * @return if not equal - true otherwise - false
	 */
	bool operator!=(const VLVector &rhs) const { return !(*this == rhs); }
	
	/**
	 * @brief Insert a section of elements into a specified location
	 * in the Vector by performing the indentation of the old elements
	 * @tparam InputIterator the type of the iterator that holds the data to insert into the vector
	 * @param position specified location
	 * @param first iterator to the first element in the section
	 * @param last iterator to the last element in the section (we don't insert it)
	 * @return iterator to the first element we insert
	 */
	template<class InputIterator>
	iterator insert(iterator const &position, InputIterator const &first, InputIterator const &last)
	{
		auto temp = VLVector(first, last); // we put the values in VLVector and now we have random access.
		size_t numAdd = temp._size;
		size_t disPos = position - begin();
		if (_capacity > StaticCapacity) // we are on the heap
		{
			size_t orgSize = _size;
			_capacity = capfunc(_size + numAdd, StaticCapacity, _capacity);
			_size += numAdd;
			T *newArr = new T[_capacity];
			std::copy(begin(), position, newArr);
			std::copy(temp.begin(), temp.end(), newArr + disPos);
			std::copy(position, begin() + orgSize, newArr + disPos + numAdd);
			delete[](heapArr);
			heapArr = newArr;
		}
		else if (_size + numAdd > StaticCapacity) // stack to heap
		{
			size_t orgSize = _size;
			_capacity = capfunc(_size + numAdd, StaticCapacity, _capacity);
			_size += numAdd;
			T *newArr = new T[_capacity];
			std::copy(stackArr, position, newArr);
			std::copy(temp.begin(), temp.end(), newArr + disPos);
			std::copy(position, stackArr + orgSize, newArr + disPos + numAdd);
			heapArr = newArr;
		}
		else // we stay on the stack
		{
			T newArr[StaticCapacity];
			size_t orgSize = _size;
			_size += numAdd;
			std::copy(begin(), begin() + orgSize, newArr);
			std::copy(temp.begin(), temp.end(), stackArr + disPos);
			std::copy(newArr + disPos, newArr + orgSize, (stackArr + disPos) + numAdd);
		}
		temp.clear();
		return begin() + numAdd;
	}
	
	/**
	 * @brief insert a singel data unit to the vector in a specified location
	 * @param position the specified location.
	 * @param toAdd the data unit, have to be in type T
	 * @return
	 */
	iterator insert(iterator const &position, T toAdd)
	{
		size_t numAdd = 1;
		size_t disPos = position - begin();
		if (_capacity > StaticCapacity) // we are on the heap
		{
			size_t orgSize = _size;
			_capacity = capfunc(_size + numAdd, StaticCapacity, _capacity);
			_size += numAdd;
			T *newArr = new T[_capacity];
			std::copy(begin(), position, newArr);
			newArr[disPos] = toAdd;
			std::copy(position, begin() + orgSize, newArr + disPos + numAdd);
			delete[](heapArr);
			heapArr = newArr;
		}
		else if (_size + numAdd > StaticCapacity) // stack to heap
		{
			size_t orgSize = _size;
			_capacity = capfunc(_size + numAdd, StaticCapacity, _capacity);
			_size += numAdd;
			T *newArr = new T[_capacity];
			std::copy(stackArr, position, newArr);
			newArr[disPos] = toAdd;
			std::copy(position, stackArr + orgSize, newArr + disPos + numAdd);
			heapArr = newArr;
		}
		else // we stay on the stack
		{
			T newArr[StaticCapacity];
			size_t orgSize = _size;
			_size += numAdd;
			std::copy(begin(), begin() + orgSize, newArr);
			stackArr[disPos] = toAdd;
			std::copy(newArr + disPos, newArr + orgSize, (stackArr + disPos) + numAdd);
		}
		return begin() + disPos;
	}
	
	/**
	 * @brief Deletes a section of items in the vector
	 * @param first Iterator for the first item in the section
	 * @param lastIterator for the item after the last item in the section
	 * @return iterator to the item to the right of the section
	 */
	iterator erase(iterator const &first, iterator const &last)
	{
		size_t numSub = last - first;
		size_t disFirst = first - begin();
		if (_size <= StaticCapacity) // we stay on the stack
		{
			size_t orgSize = _size;
			_size -= numSub;
			std::copy(last, begin() + orgSize, first); // shift to the left and override the section we deleting
		}
		else if (_size > StaticCapacity && (_size - numSub) <= StaticCapacity) //  heap to stack
		{
			std::copy(heapArr, first, stackArr);
			std::copy(last, end(), stackArr + disFirst);
			_capacity = StaticCapacity;
			_size -= numSub;
			delete[](heapArr);
			heapArr = nullptr;
		}
		else // we stay on the heap
		{
			std::copy(last, end(), heapArr + disFirst);
			_size -= numSub;
		}
		return begin() + disFirst;
	}
	
	/**
	 * @brief erase a specific item in the vector
	 * @param toRemove iterator to the item we want to erase
	 * @return iterator to the item to the right of the item we delete
	 */
	iterator erase(iterator const &toRemove)
	{
		iterator end = toRemove + 1;
		return erase(toRemove, end);
	}
	
	/**
	 * @brief
	 * @return iterator to the vector's begin
	 */
	iterator begin()
	{
		if (this->_capacity > StaticCapacity)
		{
			return heapArr;
		}
		else
		{
			return stackArr;
		}
	}
	
	/**
	 * @brief
	 * @return iterator to the vector's end
	 */
	iterator end() { return (begin() + this->_size); }
	
	/**
	 * @brief
	 * @return const iterator to the vector's begin
	 */
	const_iterator begin() const
	{
		if (this->_capacity > StaticCapacity)
		{
			return heapArr;
		}
		else
		{
			return stackArr;
		}
	}
	
	/**
	 * @brief
	 * @return const iterator to the vector's end
	 */
	const_iterator end() const { return (begin() + this->_size); }
	
	/**
	 * @brief
	 * @return const iterator to the vector's begin
	 */
	const_iterator cbegin() const { return begin(); }
	
	/**
	 * @brief
	 * @return const iterator to the vector's end
	 */
	const_iterator cend() const { return end(); }
};

/**
 * @brief private function that change the memory location and size if we need to.
 * allocate or delete the heap memory.
 * @tparam T the type of the vector elements
 * @tparam StaticCapacity
 */
template<typename T, unsigned long StaticCapacity>
void VLVector<T, StaticCapacity>::_reCap(size_t prevSize) // we know the new size we update the capacity
{
	if (_size <= _capacity && _size > StaticCapacity) // no need to do anything
	{
		return;
	}
	if (_capacity == StaticCapacity) // we on the stack
	{
		if (_size <= _capacity) // no need to do anything
		{
			return; // no need to reAlloc
		}
		else // size > _capacity = StaticCapacity -> we need to go to the heap
		{
			size_t newCap = capfunc(_size, StaticCapacity, _capacity);
			heapArr = new T[newCap];
			std::copy(stackArr, stackArr + prevSize, heapArr);
			_capacity = newCap;
		}
	}
	else if (_capacity > StaticCapacity) // we are on the heap now
	{
		if (_size >= _capacity) // we need to increase the amount of memory
		{
			_capacity = capfunc(_size, StaticCapacity, _capacity);
			auto newArr = new T[_capacity];
			std::copy(heapArr, heapArr + prevSize, newArr);
			if (heapArr)
			{
				delete[] (heapArr);
			}
			heapArr = newArr;
		}
		else if (_size <= StaticCapacity) // we need to go back to the stack
		{
			_capacity = StaticCapacity;
			std::copy(heapArr, heapArr + _size, stackArr);
			if (heapArr)
			{
				delete[] (heapArr);
				heapArr = nullptr;
			}
		}
	}
}

/**
 * @brief The function calculates the current capacity corresponding to the new size using the previous and static
 * capacity.
 * @param s the new vector's size.
 * @param stat the static capacity - constant.
 * @param nowCap the old capacity suited for the old size.
 * @return new current capacity.
 */
static size_t capfunc(size_t s, int stat, size_t nowCap)
{
	if (s <= (size_t) stat)
	{
		return stat;
	}
	if (s <= nowCap)
	{
		return nowCap;
	}
	return (3 * (s)) / 2;
}
