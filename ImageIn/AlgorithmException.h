#ifndef ALGORITHMEXCEPTION_H
#define ALGORITHMEXCEPTION_H

#include <iostream>
#include <sstream>
#include <exception>

namespace imagein
{
    //! This is the exception thrown by ImageFile and its subclasses when there is an error reading from or writing in a file.
    class AlgorithmException : public std::exception
    {
    public:
        AlgorithmException(int line, std::string file)
        {
            std::ostringstream oss;
            oss << "Exception raised line " << line << " in file " << file << " : "
                << _msg;
            this->_msg = oss.str();
        }

        virtual ~AlgorithmException() throw()
        {

        }

        virtual const char * what() const throw()
        {
            return this->_msg.c_str();
        }

    private:
        std::string _msg;
    };
	
	class NotEnoughImageException : AlgorithmException {
	};

	class ImageTypeException : AlgorithmException {
	};

	class EmptyAlgorithmCollectionException : AlgorithmException {
	};
}

#endif // ALGORITHMEXCEPTION_H