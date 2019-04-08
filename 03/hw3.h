#ifndef HW3_H
#define HW3_H
//922337203 6 8 5 4 7 7 5 8 0 7
//123456789 10111213141516171819
//100000000 0 0 0 0 0 0 0 0 0 0
//1000000000000000000 = max 
//100000000000000000 = basenum
#include <iostream>
#include <cmath>
const int qnum = 10;

void alcmem(long long * value , int size , int quote)
        {
    		long long* tmp = new long long[quote*qnum];

            std::copy(value, value + size, tmp);
            delete [] value;
            value = tmp;
            tmp = nullptr;
            for(size_t i = size; i != quote*qnum; i++)
                value[i] = 0;
        }

class BigInt{
public:
	size_t size;//заполненный размер массива
	short sign;
	int quote;
	static const long long base = 100000000000000000;

	long long* value;
public:
	BigInt(long long number = 0): size(1), sign(1), quote(10), value(new long long[quote*qnum])
	{
    	if (number < 0) 
		{
        	sign = -1;
        	number = abs(number);
    	}
        size_t i = 0;
		if (number != 0)
		{
			while (number)
			{
				value[i] = (number % base);
				number /= base;
				i++;
			}
			size = i;
		}
		else
			value[0] = 0;
		for (i = size; i != quote*qnum; i++){
			value[i] = 0;
			//std::cout << value[i] << ' ';
		}
		//std::cout<<std::endl;
	}


	BigInt(const BigInt & copy): size(copy.size), quote(copy.quote), sign(copy.sign), value(new long long[copy.quote*qnum])
	{
		std::copy(copy.value, copy.value + copy.quote*qnum, value);
	}
    
	BigInt & operator = (const BigInt & copy)
	{
		if (*this == copy)
			return *this;
		
		long long* ptr_copy = new long long[copy.quote*qnum];
		delete [] value;
		value = ptr_copy;

		quote = copy.quote;
		size = copy.size;
		sign = copy.sign;
		

		std::copy(copy.value, copy.value + quote*qnum, value);
		return *this;
	}

	BigInt(BigInt && move): size(move.size), quote(move.quote), sign(move.sign), value(move.value)
	{
		move.value = nullptr;
	}

	BigInt & operator = (BigInt && move)
	{
		if (*this == move)
			return *this;

		delete [] value;

		value = move.value;
		size = move.size;
		quote = move.quote;
		sign = move.sign;

		move.value = nullptr;
		/*move.size = 0;
		move.range = 0;
		move.sign = 0;*/

		return *this;
	}

	~BigInt(){delete [] value;}

	BigInt & operator += (const BigInt & add)
	{
        if (add.size > quote*qnum)
        {
    		alcmem(value, size, add.quote);
        }


		size_t it = 0;
		long long sum = 0;
		int maxsize = 0;
		if (size > add.size)
			maxsize = size;
		else 
			maxsize = add.size;
        
		while (it != maxsize) {
			sum += value[it];
            sum += add.value[it];
			value[it] = sum % base;
			++it;
			sum /= base;
		}

		return *this;
	}

	BigInt operator + (const BigInt & add) const
	{
		BigInt tmp = *this;
        
		if (sign == 1 && add.sign == 1)
			tmp += add;
		if (sign == 1 && add.sign == -1)
			tmp -= add;
		if (sign == -1 && add.sign == 1)
			tmp -= add;
		if (sign == -1 && add.sign == -1)
			tmp += add;

		return tmp;
	}

	BigInt & operator -= (const BigInt & sub)
	{
		if (sub.size > quote*qnum)
        {
    		alcmem(value, size, sub.quote);
        }

		size_t it = 0;
        long long dif = 0;
        int maxsize = 0;
        if (size > sub.size)
			maxsize = size;
		else 
			maxsize = sub.size;


    	while (it != maxsize)
    	{
            dif += value[it];
            dif -= sub.value[it];
	        
	        if (dif < 0) 
	        {
				if (size > 1){
	            	value[it] = dif + base;
	            	std::cout<<dif<<std::endl;
	            }
				else
					value[it] = abs(dif);
				
	            dif = -1;//вычитаем один из старшего разряда
	        } 
	        else 
	        {
	            value[it] = dif % base;
	            dif /= base;//переполнение идет дальше
	        }
	        ++it;
    	}

    	if (dif < 0)
    	 	sign *= -1;
        if (size == 1 && value[0] == 0)
            sign = 1;

    	 return *this;
	}

	BigInt operator - (const BigInt & sub) const
	{
		BigInt tmp = *this;

		if (sign == 1 && sub.sign == 1)
			tmp -= sub;
		if (sign == 1 && sub.sign == -1)
			tmp += sub;
		if (sign == -1 && sub.sign == 1)
			tmp += sub;
		if (sign == -1 && sub.sign == -1)
			tmp -= sub;

		return tmp;
	}

	int compare(const BigInt &comp) const //0 this == comp || -1 this < comp || 1 this > comp
	{
		if (sign == 1 && comp.sign == -1) 
			return 1;
		if (sign == -1 && comp.sign == 1) 
			return -1;

		int check = 1;
		if (sign == -1 && comp.sign == -1) 
			check = -1;

		if (size < comp.size) 
			return -1 * check;
		if (size > comp.size) 
			return check;

		for (size_t i = size; i > 0; --i) 
		{
			if (value[i-1] < comp.value[i-1]) 
				return -1 * check;
			if (value[i-1] > comp.value[i-1]) 
				return check;
		}

		return 0; // ==
	}

	bool operator < (const BigInt & comp) const
	{
		return compare(comp) == -1;
	}

	bool operator <= (const BigInt & comp) const
	{
		int result = compare(comp);

		return result == 0 || result == -1;
	}

	bool operator > (const BigInt & comp) const
	{
		return compare(comp) == 1;
	}

	bool operator >= (const BigInt & comp) const
	{
		int result = compare(comp);

		return result == 0 || result == 1;
	}

	bool operator == (const BigInt & comp) const
	{
		return compare(comp) == 0;
	}

	bool operator != (BigInt const & comp) const
	{
		return !(*this == comp);
	}

	friend std::ostream & operator << (std::ostream & c, const BigInt & Num)
	{
		if (Num.sign == -1)
			c << '-';
		for (size_t i = Num.size - 1; i != -1; i--)
			c << Num.value[i];
		return c;
	}


	BigInt operator -() const
	{
		BigInt x = *this;
		if (x.size != 1 && x.value[0] != 0)
			x.sign *= -1;

		return x;	
	}



};

#endif