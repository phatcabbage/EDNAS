#ifndef PROTOCOLS__HTTP__FILE_HXX___
#define PROTOCOLS__HTTP__FILE_HXX___

/**
 * @file Files/File.h++
 *
 * Describes the Files namespace.
 *
 * @author Sean Salmon
 */

#include <iostream>
#include <string>
#include "Core/Numerical/DataTypes.h++"

/**
 * @namespace Files
 *
 * Files are the objects of pursuit in SEDNAS; any architecture
 * is just a means to transfer a file.
 */
namespace Protocols
{
    namespace HTTP
    {
	using std::string;
	/**
	 * @brief Represents a file.
	 *
	 * This is the most basic file class. It only
	 * has a name and a size.
	 */
	class File
	{
	private:
	    /** A name or description for the file. */
	    string
	    _name;

	    /** The file's size in bytes. */
	    DataType
	    _size;

	public:
	    /** 
	     * Creates a file with the given characteristics.
	     * @param a_name A name or description of the file.
	     * @param a_size The size (in bytes) of the file.
	     */
	    File(const string&   a_name,
		 const DataType& a_size)
		:
		_name(a_name),
		_size(a_size)
	    {}

	    /** @return A text description of the file. */
	    string
	    description() const;
  
	    /** @return The name of the file. */
	    inline 
	    string
	    Name() const
	    { return this->_name; }
    
	    /** Sets the file's name.
	     *  @post The file's name will be set to the new name.
	     */
	    inline
	    void
	    SetName(const string a_name)
	    { this->_name = a_name; }

	    /** @return The size of the file. */
	    inline
	    DataType
	    Size() const
	    { return this->_size; }

	    /** Sets the size of the file.
	     *  @post The file's size will be set to the new size.
	     */
	    inline
	    void
	    SetSize(const DataType a_size)
	    { this->_size = a_size; }
	};

	inline
	std::ostream&
	operator<<(std::ostream& o, const File& f)
	{ return o << "[File: \"" << f.Name() << "\", " << f.Size() << " bytes]";}
    } // HTTP
} // Protocols
#endif // PROTOCOLS__HTTP__FILE_HXX____
