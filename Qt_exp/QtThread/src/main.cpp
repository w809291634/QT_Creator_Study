#include "Widget.h"
#include <QApplication>

#include <iostream>
#include <functional>

using namespace std;
using namespace std::placeholders;

typedef std::function<void(int,int)> Fun;

class B{
   public:
   void call(int a,Fun f)
   {
     f(a,2);
   }
 };

class Test{
public:
     void callback(int a,int b)
     {
         cout<<a<<"+"<<b<<"="<<a+b<<endl;
     }
     void bind()
     {
        Fun fun=std::bind(&Test::callback,this,_1,_2);
        B b;
        b.call(1,fun);
     }
 };

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();

    Test test;
    test.bind();

    return a.exec();
}
