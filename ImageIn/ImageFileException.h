#ifndef IMAGEFILEEXCEPTION_H
#define IMAGEFILEEXCEPTION_H

#include <iostream>
#include <sstream>
#include <exception>

namespace imagein
{
    //! This is the exception thrown by ImageFile and its subclasses when there is an error reading from or writing in a file.
    class ImageFileException : public std::exception
    {
    public:
        ImageFileException( const std::string& msg, int line, std::string file) : _msg(msg), _line(line), _file(file)
        {
        }

        virtual ~ImageFileException() throw()
        {

        }

        virtual const char * what() const throw()
        {
            std::ostringstream oss;
            oss << "Exception raised line " << _line << " in file " << _file << " : " << _msg;
            return oss.str().c_str();
        }
        
        std::string getMsg() const {
            return _msg;
        }

    private:
        std::string _msg;
        int _line;
        std::string _file;
    };
}

#endif // FILENOTFOUNDEXCEPTION_H
