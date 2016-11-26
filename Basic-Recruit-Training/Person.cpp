#include <iostream> //Imports cout
#include <sstream>
#include <string>
#include "Person.h"

// Person::Person();  //creating what the functions actually DO here
Person::Person()    {
name_ = "";
age_ = new int;

}


 Person::Person(const std::string& name, int a)   {
 name_ = name;
// a = new int; //do I have to create this block of memory, or is it unessesary? ask Bill
 age_ = new int(a);

 }

 Person::~Person()  {
    delete age_;
 }

 int Person::getAge() const {
    return *age_;
 }
const std::string& Person::getName() const  {
return name_;

}

void Person::printInfo() const   {
 std::cout << "Name: " << getName() << std::endl
              << "Age: " << getAge() << std::endl;

}
//void Person::printInfo() const  {
//stf::count << Bob.getName() << std::endl << Bob.getAge() << std::endl;
//}


void Person::setName(const std::string& newName)  {
    name_ = newName;
}
void Person::setAge(int newAge)   {
    delete(age_);   //freeing this memory space in the heap
    age_ = new int(newAge);

}
//*static int initial_age = 0;

 void Person::birthday(Person& x)   {
    x.setAge (x.getAge() + 1);
   // *static int initial_age(Person& x) + 1;
 }


int Person::combinedAge(Person** x, int size)  {
 int c = 0; //
 for (int place = 0; place < size; place++)  {
    c = c + (x[place])->getAge();   //have to use brackets otherwise dot operator takes precesdence over double pointer
    std::cout << "combinedAge: " << c << "age:" << (x[place])->getAge() << "\n";

 }

return c;
}
//static int Person::combinedAge(Person** x, int size){
//return 0;
//} // Print the combined Age of everyone pointed to by x


