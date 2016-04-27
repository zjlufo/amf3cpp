amf3cpp
=======

C++ implementation of the Action Message Format (AMF3)

# License #

This project is licensed under the MIT license. See [LICENSE](https://github.com/Daizee/amf3cpp/blob/master/LICENSE)

# Documentation #

Include files within your project to use. Makes use of C++11 functionality. Compiles on both Windows and Linux.

To create an AMF3Object:

```C++
amf3object obj;
obj["command"] = "test";//set 'command' key to string 'test'

amf3object & data = obj["data"];//set 'data' key to new amf3object
data["something"] = 1;//set 'something' key to integer value of 1
data["thing"] = 1.5f;//set 'thing' key to double (AMF3Number type) value of 1.5
```

To create an AMF3Array: (supports both dense and associative arrays)

Dense:
```C++
amf3array arr;

amf3object o;
o["key"] = "this will go into the array";

arr.Add(o);

printf("%s", arr[0]["key"].c_str());
```

Associative:
```C++
amf3object o;
o["key"] = "this will be referenced by key in the array";

arr.Add("second", o);

printf("%s", arr["second"]["key"].c_str());
```

Can also access elements through C++11 ranged based for loop:
```C++
for (auto object : arr)
{
  printf("%s", object["key"].c_str());
}
```

To serialize an object (and potentially send it over a socket):

```C++
amf3object object;
object["key"] = "some data";

char buffer[100];
auto length = 0;

amf3writer writer(buffer);

writer.Write(object);

length = writer.position;
```

To deserialize an object:

```C++
//buffer from the serialize example

amf3parser cparser(buffer);

amf3object temp = cparser.ReadNextObject();

printf("%s\n", temp["key"].c_str());//prints out "some data"

```

# Notes #

When nesting objects together, care should be taken with how you insert objects. Reusing objects and inserting them multiple times will not create new objects but instead create references.
```C++
amf3array arr;

amf3object o;
o["key"] = "this will go into the array";
arr.Add(o);

o["key"] = "this will go into the array1";
arr.Add(o);

o["key"] = "this will go into the array2";
arr.Add(o);

for (auto object : arr)
{
  printf("%s", object["key"].c_str());//this will print "this will go into the array2" x3 times
}
```

The solution is to do `o = amf3object()` when you wish to reuse a variable. This will not clear the original data.

```C++
amf3array arr;

amf3object o;
o["key"] = "this will go into the array";
arr.Add(o);

o = amf3object();
o["key"] = "this will go into the array1";
arr.Add(o);

o = amf3object();
o["key"] = "this will go into the array2";
arr.Add(o);

for (auto object : arr)
{
  printf("%s\n", object["key"].c_str());//this will print all 3 unique lines correctly
}
```

The `amf3object` class is really an encapsulating class that maintains the meta-data of the object. A true amf3object that contains associative keys and data can be checked through the member variable `type` (`enum Amf3TypeCode`). Likewise can be used to check if there is any other sort of value inside the container.

```C++
if (object.type == Amf3TypeCode::Object){}
```

Storing an array as a root object (serializing an array as the base container) is as easy as just setting an object to the array variable:

```C++
amf3object container;
amf3array arr;

amf3object o;
o["key"] = "value";
arr.Add(o);

container = arr;

char buffer[100];
auto length = 0;

amf3writer writer(buffer);
```

If receiving an object like such, accessing the underlying array is just as easy:

```C++
amf3parser cparser(buffer);

amf3object obj = cparser.ReadNextObject();

amf3array & arr = obj;
for (auto object : arr)
{
  printf("%s\n", object["key"].c_str());
}
```

This library does not have Amf3 object types: Xml, ByteArray, Date, or LegacyXmlDocument implemented yet.
