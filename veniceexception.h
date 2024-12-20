#ifndef VENICEEXCEPTION_H
#define VENICEEXCEPTION_H

#include <vector>

//using namespace std;

class VeniceException : public std::exception
{

    public:
        VeniceException(const char* message);

        const char* what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW
        {
            return this->message;
        }

    private:
        const char* message;
};

#endif // VENICEEXCEPTION_H
