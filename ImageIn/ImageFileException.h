/*
 * Copyright 2011-2012 Benoit Averty, Samuel Babin, Matthieu Bergere, Thomas Letan, Sacha Percot-Tétu, Florian Teyssier
 * 
 * This file is part of DETIQ-T.
 * 
 * DETIQ-T is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * DETIQ-T is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with DETIQ-T.  If not, see <http://www.gnu.org/licenses/>.
*/

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
