#ifndef AMF_H
#define	AMF_H

#include "amfdefs.h"
#include <string.h>
#include <vector>
#include <map>
#include <stdlib.h>



using namespace std;
class amf3array;
class amf3objectmap;


template <class T>
class amf3reflist
{
public:
	amf3reflist(void);
	~amf3reflist(void);

	void AddObj(T obj);
	void AddObj(char * key, T obj);
	T & GetObj(char * key);
	T & GetObj(int offset);
	int Exists(char * key);
	vector<char *> propnames;
	vector<T> properties;
};

//#undef bool
class amf3object
{
public:
// 	amf3object(void)
// 	{
// 		amf3encapscreate++;
// 		this->type = Undefined;
// 		number = 0;
// 	}

	~amf3object(void);

	Amf3TypeCode type;

	static bool IsEqual (amf3object & obj, amf3object & obj2);

	inline static bool IsNEqual (amf3object & obj, amf3object & obj2)
	{
		return !(amf3object::IsEqual(obj, obj2));
	}
	union {
		bool booltest;
		int integer;
		double number;
		char * text;
		char * date;
		amf3array * _array;
		amf3objectmap * _object;
	} _value;

	amf3object();
	amf3object(const amf3object &val);

	amf3object(const bool &val);
	amf3object(const int8_t &val);
	amf3object(const int16_t &val);
	amf3object(const int32_t &val);
	//amf3object(const int &val);
	amf3object(const int64_t &val);
	amf3object(const uint8_t &val);
	amf3object(const uint16_t &val);
	amf3object(const uint32_t &val);
	amf3object(const uint64_t &val);
	amf3object(const double &val);
	void InternalCopy(const amf3object &val);
	void InternalCopy(const amf3array &val);

	amf3object & operator=(const amf3object &val);
	amf3object & operator=(const amf3array &val);
	amf3object & operator=(const bool &val);
	amf3object & operator=(const int8_t &val);
	amf3object & operator=(const int16_t &val);
	amf3object & operator=(const int32_t &val);
	//amf3object & operator=(const int &val);
	amf3object & operator=(const int64_t &val);
	amf3object & operator=(const uint8_t &val);
	amf3object & operator=(const uint16_t &val);
	amf3object & operator=(const uint32_t &val);
	amf3object & operator=(const uint64_t &val);
	amf3object & operator=(const double &val);

	//Variant & operator=(const Timestamp &val);

	amf3object & operator=(const char *pVal);
	amf3object & operator=(const string &val);

	operator Amf3TypeCode();
	operator bool();
	operator int8_t();
	operator int16_t();
	operator int32_t();
	//operator int();
	operator int64_t();
	operator uint8_t();
	operator uint16_t();
	operator uint32_t();
	operator uint64_t();
	operator double();
	//operator Timestamp();
	operator string();
	operator char*();
	operator amf3array*();
	void Reset(bool isUndefined = false);

	amf3object & operator[](const string &key);
	amf3object & operator[](const char *key);
	amf3object & operator[](const double &key);
	amf3object & operator[](const uint32_t &key);
	amf3object & operator[](const int &key);
	amf3object & operator[](amf3object &key);
	amf3object & GetValue(string key, bool caseSensitive);

	bool operator==(amf3object variant);
	bool operator!=(amf3object variant);
	bool operator==(Amf3TypeCode type);
	bool operator!=(Amf3TypeCode type);

};


class amf3classdef
{
public:
	amf3classdef(void);
	amf3classdef(char * name, vector<char *> & properties, bool dynamic, bool externalizable);
	amf3classdef(const amf3classdef & classdef);
	~amf3classdef(void);

	char * name;
	bool dynamic;
	bool externalizable;
	vector<char *> properties;
	inline bool IsEqual (amf3classdef & obj) const
	{
		if (name != obj.name || dynamic != obj.dynamic || externalizable != obj.externalizable || (properties.size() != obj.properties.size()))
			return false;

		if (properties.size() == obj.properties.size())
		{
			for (uint32_t i = 0; i < properties.size(); ++i)
			{
				if (properties.at(i) != obj.properties.at(i))
					return false;
			}
		}
		else
		{
			return false;
		}
		return true;
	}
};



class amf3parser
{
public:
	amf3parser(char * stream);
	~amf3parser(void);

	amf3object  ReadNextObject(void);

	int ReadInteger(void);
	double ReadNumber(void);
	char * ReadString(void);
	char * ReadDate(void);
	amf3array * ReadArray(void);
	amf3objectmap * ReadAMF3Object(void);

	amf3reflist<char *> strlist;
	amf3reflist<char *> internalstrlist;
	amf3reflist<amf3object> objectlist;
	amf3reflist<amf3object> encapslist;
	amf3reflist<amf3classdef> deflist;

	char * stream;
	int position;
};



class amf3writer
{
public:
	amf3writer(char * stream);
	~amf3writer(void);

	bool CheckObjectTable(amf3object & obj)
	{
		// TODO: Fix object tables
// 		map<int, amf3object>::const_iterator iter;
// 		iter = objectTable.begin();
// 		for (uint32_t i = 0; i < objectTable.size(); ++iter, ++i)
// 		{
// 			if (amf3object::IsEqual((amf3object&)(iter->second), obj))
// 			{
// 				TypelessWrite(i << 1);
// 				return true;
// 			}
// 		}
		return false;
	}
	void Write(Amf3TypeCode type);
	void Write(amf3object & obj);
	void Write(short integer);
	void Write(char integer);
	void Write(unsigned short integer);
	void Write(unsigned char integer);
	void Write(unsigned int integer);
	void Write(int integer);
	void Write(double number);
	void Write(char * str);
	void TypelessWrite(uint32_t integer);
	void TypelessWrite(int integer);
	void TypelessWrite(double number);
	void TypelessWrite(char * str);
	void WriteDictionary(amf3reflist<amf3object> * reflist);
	void Write(amf3array * _array, amf3object & obj);
	void Write(amf3objectmap * _object, amf3object & obj);
	void TypelessWrite(amf3array * _array, amf3object & obj);
	void TypelessWrite(amf3objectmap * _object, amf3object & obj);

	amf3reflist<char *> strlist;
	amf3reflist<amf3object> objectlist;
	amf3reflist<amf3object> encapslist;
	amf3reflist<amf3classdef> deflist;
	map<int, amf3object> objectTable;
	map<int, char *> stringTable;
	map<int, amf3classdef> classdefTable;

	char * stream;
	int position;
};


class amf3objectmap
{
public:
	amf3objectmap(amf3classdef & classdef)
	{
		selfdel = true;
		//amf3objectcreate++;
		this->classdef = amf3classdef(classdef);
		//this->classdef = new amf3classdef(*classdef);
		anoncd = false;
	}

	amf3objectmap(amf3objectmap & objectmap)
	{
		anoncd = false;
		classdef = objectmap.classdef;
		for (uint32_t i = 0; i < objectmap.properties.properties.size(); ++i)
		{
			properties.AddObj(objectmap.properties.propnames.at(i), objectmap.properties.GetObj(i));
		}
		//properties = objectmap.properties;
		selfdel = false;
	}
	amf3objectmap()
	{
//		amf3objectcreate++;
		anoncd = true;
		selfdel = true;
		//this->classdef = new amf3classdef(0, new vector<char *>, true, false);
	}
	~amf3objectmap(void);

	amf3object & Get(char * key);
	void Add(char * key, amf3object & obj);
	int Exists(char * key);

	inline bool IsEqual(amf3objectmap * obj)
	{
		for (uint32_t i = 0; i < properties.properties.size(); ++i)
		{
			if (amf3object::IsNEqual(properties.properties.at(i), obj->properties.properties.at(i)))
				return false;
		}
		return true;
	}

	bool anoncd;
	amf3classdef classdef;
	amf3reflist<amf3object> properties;
	int flags;
	bool selfdel;
};

class amf3array
{
public:
	amf3array(void);
	~amf3array(void);

	void Add(amf3object & obj);
	void Add(char * key, amf3object & obj);
	amf3object & Get(int id);
	amf3object & Get(char * key);
	inline bool IsEqual(amf3array * obj)
	{
		if (this->type == 1)
		{
			if (this->dense.size() != obj->dense.size())
				return false;
			for (uint32_t i = 0; i < this->dense.size(); ++i)
			{
				if (amf3object::IsNEqual(this->dense.at(i), obj->dense.at(i)))
					return false;
			}
		}
		else if (this->type == 2)
		{
			if (this->associative.properties.size() != obj->associative.properties.size())
				return false;
			for (uint32_t i = 0; i < this->associative.properties.size(); ++i)
			{
				if (amf3object::IsNEqual(this->associative.properties.at(i), obj->associative.properties.at(i)))
					return false;
			}
		}
		return true;
	}


	amf3reflist<amf3object> associative;
	vector<amf3object> dense;

	char type;
};

#endif	/* AMF_H */

