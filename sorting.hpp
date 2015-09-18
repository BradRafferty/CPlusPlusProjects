// Implementations of different sorting algorithms.
#ifndef __SORTING_HPP__
#define __SORTING_HPP__
#include <vector>

// =============================================================================
// Helper functions
// =============================================================================

// Swaps two values
template <typename T>
void swap(T& a, T& b)
{
    T temp = a;
    a = b;
    b = temp;
}

// Checks if a vector is sorted
template <typename T>
bool is_sorted(const std::vector<T>& vector)
{
    auto t = vector[0];
    for (auto e : vector)
    {
        if (e < t) return false;
        t = e;
    }
    return true;
}

//Print a vector
template <typename T>
void print_vector(const std::vector<T>& vector)
{
    for (T e : vector)
       std::cout << e << " ";
    std::cout << std::endl;
}

// =============================================================================
// Sorting functions
// =============================================================================

// Insertion sort implementation
template <typename T>
std::vector<T> insertion_sort_helper(std::vector<T> vector)
{
	int vector_size = vector.size();
	
    for(int lead = 1; lead < vector_size; lead++)	// Loops through the entire vector, checking each value against the values that come before it.
	{
		T hold = vector[lead];						// Holds the value at the current position in the vector.
		int tail = lead - 1;						// Original tail position.
		while ((tail > -1) and (vector[tail] > hold)) // Moves backwards from the original tail position (as long as that position is greater then 0).
													// The purpose is to check if a previous item in the list is greater then the currently held value.
		{ 
			vector[tail + 1] = vector[tail];		// If the current tail value is found to be greater then the current lead value,
													// then all values between that tail item and the lead value are moved forward by one position.
			tail--;								
		}
		vector[tail + 1] = hold;					// Then the value at the head position is moved into the the position of the tail value that was found to be greater,
													// or if no tail positions were found to be greater, then [tail + 1] would still equal the same, original lead position.
		/*
		Stated more clearly it works like this:
			- Current list: 2, 3, 15, 7, 32, 5
		Once the function reaches position 3, it holds the value, 7, in the hold variable.
			- Current list: 2, 3, 15, 7, 32, 5		hold = 7
		It then compares it with the first tail position 2, whose value is 15.
		Since the value at position 2 is greater then the value at position 3, position 2 is moved up one into the position 3.
			- Current list: 2, 3, 15, 15, 32, 5		hold = 7
		After the necessary tail values are moved forward by one, the hold value can be inserted in the necessary position.
			- Current list: 2, 3, 7, 15, 32, 5
		*/
	}
	
	return vector;
}

template <typename T>
void insertion_sort(std::vector<T>& vector)
{
    vector = insertion_sort_helper<T>(vector);	// Instead of implementing it here, I used a seperate function so I could
}												// return vector values for later parts.




// Merge sort implementation
template <typename T>
std::vector<T> merge(std::vector<T> left, std::vector<T> right)
{
	std::vector<T> vector;
	int i = 0;									// Starts at the beginning of each of the two halfs.
	int j = 0;									// Starts at the beginning of each of the two halfs.
	while (i < left.size() and j < right.size())// Until the end of one of the two halfs is hit, it keeps looping.
	{
		if (left[i] < right[j])					// If the first value in the left half is less,
		{
			vector.push_back(left[i]);			// then push that value into the new vector,
			i++;								// and move to the next value in the left half.
		}
		else 									// Or if the first value in the right half is less,
		{
			vector.push_back(right[j]);			// then push that value into the new vector,
			j++;								// and move to the next value in the right half.
		}
	}
	while (i < left.size())						// If there are no more values in the right half,
	{											// then all the remaining left values are pushed into the new vector
		vector.push_back(left[i]);
		i++;
	}
	while (j < right.size())					// The same applies if there are no more values in the left half.
	{
		vector.push_back(right[j]);
		j++;
	}
	return vector;								// Returns a sorted vector.
}

template <typename T>
std::vector<T> merge_sort_helper(std::vector<T> vector, int basecase)
{
	int vector_size = vector.size();
    
    if(vector_size <= basecase)						// Used to return the vector once the basecase is hit.
    {
		if(basecase == 1)							// Used to switch between only merge_sort and the hybrid_sort alorithms.
			return vector;
		else
			return insertion_sort_helper<T>(vector);
	}
	
	int midpoint;
	
	if (vector_size % 2 == 0)						// Used to find the midpoint, in order to split the vectors into a left and right half.
		midpoint = (vector_size) / 2;
	else
		midpoint = (vector_size - 1) / 2;
	
	std::vector<T> left_vector;
	for (int count = 0; count < midpoint; count++)	// Loops from 0 to the midpoint - 1 and pushes those values into a new "left" vector.
		left_vector.push_back(vector[count]);
	
	std::vector<T> right_vector;
	for (int count = midpoint; count < vector_size; count++)		// Loops from the midpoint to the last value and pushes those values into a new "right" vector.
		right_vector.push_back(vector[count]);
	
	std::vector<T> left = merge_sort_helper(left_vector, basecase);	// Recursively calls the function on each half of the vector until the basecase is hit.
	std::vector<T> right = merge_sort_helper(right_vector, basecase);
	
	return merge<T>(left, right);					// Once left and right are returned, this function merges them into an ordered vector.
	
}

template <typename T>
void merge_sort(std::vector<T>& vector)
{
    vector = merge_sort_helper<T>(vector, 1);		// Since the basecase is set to 1, only the merge sort algorithm is used.
}




// Hybrid sort implementation
template <typename T>
void hybrid_sort(std::vector<T>& vector)
{
    vector = merge_sort_helper<T>(vector, 10);		// Since the basecase is set to a random constant k = 10, insertion sort is used
}													// for vectors less then size 10, and merge sort for vectors greater then.

#endif // __SORTING_HPP__
