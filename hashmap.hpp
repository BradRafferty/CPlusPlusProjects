// An implementation of a hash map using dynamic resizing and separate chaining.
// No data structures from the standard library were used.
#ifndef __HASHMAP_HPP__
#define __HASHMAP_HPP__
#define INITIAL_CAPACITY 10
#include <iostream>
#include <functional>

// A simple node struct
template <class K,class V>
struct KVNode
{
    K key;
    V value;
    KVNode<K,V>* next;
};

template <class K, class V>
class HashMap {
    public:
        // Default constructor: creates an empty hash map
        HashMap();
        // Constructor setting the default value for our map
        HashMap(V value);
        // Destructor: deallocates all memory associated with the hash map
        ~HashMap();
        // size() returns the number of elements currently stored in the hash map.
        unsigned int size() const;
        // set() inserts the given key value pair into the
        // hash map.  If the key is already in the map, then
        // the value for that key is overwritten.
        void set(const K& key, const V& value);
        // get() returns the value associated with the key.
        // If the key is not in the hash map, the default
        // value is returned.
        V get(const K& key) const;
        
        void resize_array();
        int hash_value(K key);
    private:
        KVNode<K,V> **bucket_array;
        std::hash<K> hash;
        // Default value to return if a key is not in the hash map
        V default_value;
        int bucket_array_size = 10;
};

template <class K, class V>
HashMap<K,V>::HashMap() : default_value()
{
	//Creates and populates an array of size 10 with NULL pointers.
	bucket_array = new KVNode<K,V>*[bucket_array_size];
    for (int count = 0; count < bucket_array_size; count++)
		bucket_array[count] = NULL;
}

template <class K, class V>
HashMap<K,V>::HashMap(V value)
{
	//Creates and populates an array of size "Value" with NULL pointers.
    bucket_array = new KVNode<K,V>*[value];
    for (int count = 0; count < value; count++)
		bucket_array[count] = NULL;
	bucket_array_size = value;
}

template <class K, class V>
HashMap<K,V>::~HashMap()
{
	//Deletes allocated array to prevent memory leaks.
    delete[] bucket_array;
}

template <class K, class V>
unsigned int HashMap<K,V>::size() const
{ 
	//If the value in the array is not NULL add one to the size count.
    int size = 0;
    for (int count = 0; count < bucket_array_size; count++)
    {
		if (bucket_array[count] != NULL)
			size++;
	}
	return size;	
}

template <class K, class V>
void HashMap<K,V>::resize_array()
{
	unsigned int filled_positions = this->size();
	if(((float)filled_positions / (float)bucket_array_size) > .75)
	{
		int old_bucket_array_size = bucket_array_size;	//Store old size.
		bucket_array_size *= 2;							//Double the old size.
		std::cout << "Doubling array size to: " << bucket_array_size << std::endl;
		
		KVNode<K,V> ** old_bucket_array = bucket_array;
		bucket_array = new KVNode<K,V> *[bucket_array_size];	//Initialize new array with double the size.
		
		for (int count = 0; count < bucket_array_size; count++)	//Populate new array with NULL pointers.
		{
			bucket_array[count] = NULL;
		}
		
		for (int count = 0; count < old_bucket_array_size; count++)
		{
			if(old_bucket_array[count] != NULL)	//If there was a value in the old array (and not just a NULL pointer).
			{
				KVNode<K,V> * old;
				KVNode<K,V> * node = old_bucket_array[count];	//Store pointer to the first link at each array position.
				while(node != NULL)								//While there still exist nodes in the linked list.
				{
					set(node->key, node->value);				//Move the old key/value into the new LL
					old = node;
					node = node->next;							//Move to the next node
					delete old;									//Delete the old nodes when done.
				}
			}
		}
		delete[] old_bucket_array;	//Delete the original array.
	}
		/****** Previous attempt at resizing ******/
		//KVNode<K,V> * new_bucket_array = new KVNode<K,V>[(bucket_array_size * 2)];
		
		//for(int index = 0; index < (bucket_array_size * 2); index++)
			//new_bucket_array[index].next = NULL;
			
		//for(int index = 0; index < bucket_array_size; index++)
		//{
			//KVNode<K,V> *head = &bucket_array[index];
			//new_bucket_array[index].next = head->next;
			//std::cout << head->next << std::endl;
		//}	
		
		//delete[] bucket_array;
		//bucket_array = new_bucket_array;
		//bucket_array_size *= 2;
		//std::cout << "Array Resized to: " << bucket_array_size << std::endl;
	//}
}

template <class K, class V>
int HashMap<K,V>::hash_value(K key)
{
    int sum = 0;
    for(int count = 0; count < key.size(); count++)
        sum += key[count];
    int hash_number = (sum % bucket_array_size);
    return hash_number;
}

template <class K, class V>
void HashMap<K,V>::set(const K& key, const V& value)
{
    int hash_number = this->hash_value(key);
    
    KVNode<K,V> *node = new KVNode<K,V>;
    node->key = key;
    node->value = value;
    node->next = NULL;
    
    if (bucket_array[hash_number] == NULL)
        bucket_array[hash_number] = node;
    else
    {
        KVNode<K,V> *head = bucket_array[hash_number];
        while(head->next != NULL)
            head = head->next;
        if (head->key == key)
            head->value = value;
        else
            head->next = node;
    }
    
    this->resize_array();
}

	/****** I had to completely rewrite the set and get functions to work with array resizing,
	 however they work fine for a static array ******/
	//resize_array();
    //int hash_number = this->hash_value(key);	
    
    //KVNode<K,V> *node = new KVNode<K,V>;
    //node->key = key;
    //node->value = value;
    //node->next = NULL;
    
    //KVNode<K,V> *head = &bucket_array[hash_number];
    	
	//while (head->next != NULL)
	//{
		//head = head->next;
		//if(head->key == key)
		//{
			//head->value = value;
			//break;
		//}
	//}
	//if(head->next == NULL)
	//{
		//head->next = node;
	//}
	

template <class K, class V>
V HashMap<K,V>::get(const K& key) const
{
	int hash_number = const_cast<HashMap<K,V>*>(this)->hash_value(key);	//Get the hash value for the key.
	if (bucket_array[hash_number] == NULL)			//If there is no linked list for the hash value return a default.
		return default_value;
	else
	{
		KVNode<K,V> *head = bucket_array[hash_number];	//A pointer to the first linked list node.
		while((head != NULL) && (head->key) != key)	//While the linked list isn't empty and the key doesn't match,
			head = head->next;						//traverse the linked list.
		if(head == NULL)							//If the last pointer (null) is reached, return the default value.
			return default_value;
		else 										// Else return the value for the given key.
			return head->value;
	}
	/****** I had to completely rewrite the set and get functions to work with array resizing,
	 however they work fine for a static array ******/
    //int hash_number = const_cast<HashMap<K,V>*>(this)->hash_value(key);
    
    //KVNode<K,V> *head = &bucket_array[hash_number];

	//while (head->next != NULL)
	//{
		//head = head->next;
		//if (head->key == key)
		//{
			////std::cout << "Found Key in Get" << std::endl;
			//return head->value;
		//}
		
	//}
	//if (head->next == NULL)
	//{
		//if (head->key == key)
		//{
			//std::cout << "Found Key in Get" << std::endl;
			//return head->value;
		//}
		//std::cout << "No Value returned from Get" << std::endl;
		//return default_value;
	//}
}

#endif // __HASHMAP_HPP__
