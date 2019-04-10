#pragma once

#include <iostream>
#include <string>

enum class Error
{
    NoError,
    CorruptedArchive
};

class Serializer
{
    static constexpr char Separator = ' ';
    std::ostream & out_;

public:
    explicit Serializer(std::ostream& out)
        : out_(out)
    {
    }

    template <class T>
    Error save(T& object)
    {
        return object.serialize(*this);
    }

    template <class... Args>
    Error operator()(Args&&... args)
    {
        return process(std::forward<Args>(args)...);
    }
    
private:
    Error process(bool value)
    {
    	out_ << (value ? "true" : "false") << Separator;
    	return Error::NoError;
    }

    Error process(uint64_t value)
    {
    	out_ << value << Separator;
    	return Error::NoError;
    }

    template<class T>
    Error process(T&& value)
    {
    	return Error::CorruptedArchive;
    }

    template <class T, class... Args>
    Error process(T&& value , Args&&... args)
    {
    	if(process(std::forward<T>(value)) == Error::CorruptedArchive)
    		return Error::CorruptedArchive;
    	return process(std::forward<Args>(args)...);
    }
};


class Deserializer
{
    std::istream & in_;

public:
    explicit Deserializer(std::istream& in)
        : in_(in)
    {
    }

    template <class T>
    Error load(T& object)
    {
        return object.serialize(*this);
    }

    template <class... Args>
    Error operator()(Args&&... args)
    {
        return loading(std::forward<Args>(args)...);
    }
    
private:
   Error loading(bool & value)
    {
    	std::string text;
    	in_ >> text;

    	if (text == "true")
        	value = true;
    	else if (text == "false")
        	value = false;
    	else
        	return Error::CorruptedArchive;
		
		return Error::NoError;
    }

    Error loading(uint64_t & value)
    {
    	std::string text;
    	in_ >> text;
    	value = 0;

    	for(auto tmp : text)
    	{
    		if(isdigit(tmp))
    			value = value*10 + /*(uint64_t)*/(tmp - '0');
    		else
    			return Error::CorruptedArchive;
    	}

    	return Error::NoError;
    }

    template<class T>
    Error loading(T&& value)
    {
    	return Error::CorruptedArchive;
    }

    template <class T, class... Args>
    Error loading(T&& value , Args&&... args)
    {
    	if(loading(std::forward<T>(value)) == Error::CorruptedArchive)
    		return Error::CorruptedArchive;
    	return loading(std::forward<Args>(args)...);
    }

};


