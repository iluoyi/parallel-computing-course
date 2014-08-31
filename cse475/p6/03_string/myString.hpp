#include <iostream>
#include <string.h>
#include <cstring>
using namespace std;

/**
* created by Yi and Jundong
*/

class myString
{
	char* m_str;
	int length;
	
public:
	/*
	* constructors
	*/
	//myString();
	__attribute__((transaction_safe))
		myString(const char* str=NULL);
	
	__attribute__((transaction_safe))
		myString(const myString &other);
		
	__attribute__((transaction_safe))
		~myString();
	
	// operator override
	__attribute__((transaction_safe))
		bool operator>= (const myString &other);
	__attribute__((transaction_safe))
		bool operator> (const myString &other);
	__attribute__((transaction_safe))
		bool operator== (const myString &other);
	__attribute__((transaction_safe))
		const char& operator[] (int index) const;
	
	// modifiers
	__attribute__((transaction_safe))
		myString& operator= (const char* other);

	// query methods
	int getLength() const {return length;}
	__attribute__((transaction_safe))
		const char* toString() const;
};

// myString::myString() {
	// length = 0;
	// m_str = (char *)malloc(sizeof(char)*(length + 1));
	// memset(m_str, 0, sizeof(char)*(length + 1));
// }

// one argument constructor
__attribute__((transaction_safe))
myString::myString(const char* str)
{
	if (str == NULL) {
		length = 0;
		m_str = (char *)malloc(sizeof(char)*(length + 1));
		memset(m_str, 0, sizeof(char)*(length + 1));
	} else {
	//TODO
	int i = 0;
	char c = str[i];
	while (c != '\0') {
		i ++;
		c = str[i];
	}
	length = i;
	
		m_str = (char*)malloc(sizeof(char)*(length + 1));
		memset(m_str,0,sizeof(char)*(length + 1));
		for (int j = 0; j < length; j ++) {
			m_str[j] = str[j];
		}
	}
}

// one argument constructor
__attribute__((transaction_safe))
myString::myString(const myString& other)
{
	length = other.getLength();
	m_str = (char *)malloc(sizeof(char)*(length + 1));
	memset(m_str,0,sizeof(char)*(length + 1));
	//TODO
	for (int j = 0; j < length; j ++) {
		m_str[j] = other.m_str[j];
	}
}

// destructor
__attribute__((transaction_safe))
myString::~myString()
{
	free(m_str);
}

// modifier =
__attribute__((transaction_safe))
myString& myString::operator= (const char* str)
{
	free(m_str);
	
	//TODO
	int i = 0;
	char c = str[i];
	while (c != '\0') {
		i ++;
		c = str[i];
	}
	length = i;
	
	m_str = (char*)malloc(sizeof(char)*(length + 1));
	memset(m_str,0,sizeof(char)*(length + 1));
	
	//TODO
	for (int j = 0; j < length; j ++) {
		m_str[j] = str[j];
	}
	
	return *this;
}

// the toString function
__attribute__((transaction_safe))
const char* myString::toString() const {
	return m_str;
}

__attribute__((transaction_safe))
bool myString::operator>= (const myString& other) {
	int ThisLength, OtherLength;
	char * o_data = other.m_str;
	
	//TODO
	int i = 0;
	char c = m_str[i];
	while (c != '\0') {
		i ++;
		c = m_str[i];
	}
	ThisLength = i;
	
	i = 0;
	c = o_data[i];
	while (c != '\0') {
		i ++;
		c = o_data[i];
	}
	OtherLength = i;
	
	bool bigger = false;
	int smaller = ThisLength > OtherLength? OtherLength:ThisLength;
	if(ThisLength == OtherLength) 
		
	for(int i = 0; i < smaller; i++){
		if(*(m_str + i) > *(o_data + i)){
			bigger = true;
			break;
		}
		else if(*(m_str + i) < *(o_data + i))
			break;
		else{
			if((i == smaller - 1) && (ThisLength == OtherLength))
				bigger = true;
		}
	}
	return bigger;
}

__attribute__((transaction_safe))
bool myString::operator> (const myString& other) {
	int ThisLength, OtherLength;
	char * o_data = other.m_str;
	
	//TODO
	int i = 0;
	char c = m_str[i];
	while (c != '\0') {
		i ++;
		c = m_str[i];
	}
	ThisLength = i;
	
	i = 0;
	c = o_data[i];
	while (c != '\0') {
		i ++;
		c = o_data[i];
	}
	OtherLength = i;
		
	bool bigger = false;
	int smaller = ThisLength > OtherLength? OtherLength:ThisLength;
	for(int i = 0; i < smaller; i++){
		if(*(m_str + i) > *(o_data + i)){
			bigger = true;
			break;
		}
		else if(*(m_str + i) < *(o_data + i))
			break;
		else continue;
	}
	return bigger;
}

__attribute__((transaction_safe))
bool myString::operator== (const myString& other) {
	int ThisLength, OtherLength;
	char * o_data = other.m_str;
	
	//TODO
	int i = 0;
	char c = m_str[i];
	while (c != '\0') {
		i ++;
		c = m_str[i];
	}
	ThisLength = i;
	
	i = 0;
	c = o_data[i];
	while (c != '\0') {
		i ++;
		c = o_data[i];
	}
	OtherLength = i;
	
	bool equal = false;
	if(ThisLength == OtherLength){
		for(int i = 0; i < ThisLength; i++){
			if(*(m_str + i) != *(o_data + i)){
				break;
			}
			else{
				if(i == ThisLength - 1)
					equal = true;
				continue;
			}	
		}
	}
	return equal;
}

// used for the hashing function
__attribute__((transaction_safe))
const char& myString::operator[] (int index) const {
	if(index < 0 || index >= length)
		return m_str[length];
	else
		return m_str[index];
}

// customized hash function
namespace std
{
    template<>
    struct hash<myString>
    {
    public:
        std::size_t operator()(myString const& str) const 
        {
			size_t hash = 5381;
            for(int i = 0; i < str.getLength(); i ++){
				hash = ((hash * 31) + hash) + str[i];
			}
            return hash;
        }
    };
}