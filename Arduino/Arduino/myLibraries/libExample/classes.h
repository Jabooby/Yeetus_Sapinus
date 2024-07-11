
#ifndef classes_h_
#define classes_h_

class MyClass
{
    public:
    MyClass();
    ~MyClass();
    void myPublicFunction();
    
    protected:
    void myProtectedFunction();
    
    private:
    void myPrivateFunction();
    int myInt = 0;
    bool myBool = false;
};

#endif classes_h_