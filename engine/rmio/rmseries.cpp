
#include "rmseries.h"
#include <cstring>


RMSeries::RMSeries()
{
    vals_raw = NULL;
    length = 0;
    width = 0;
    iterPos = -1;
	name[0] = 0;
}

RMSeries::RMSeries(const RMSeries& s)
{
    vals_raw = NULL;
    length = 0;
    width = 0;
    iterPos = -1;
	name[0] = 0;

    operator=(s);
}

RMSeries& RMSeries::operator=(const RMSeries& s)
{
	if (this == &s)
		return *this;

	// initCopy first because when we're running in debug mode pointers
	// are assigned 0xcccccccc rather than null (or it appears that way)
    initCopy(s.length, s.width, s.vals_raw);

    this->iterPos = s.iterPos;
    this->length = s.length;
    this->width = s.width;
	strncpy(this->name, s.name, NAME_LENGTH);

    return *this;
}


double**& RMSeries::get_pointer()
{
	return vals_raw;
}


void RMSeries::init(int length_)
{
    init(length_, 1);
}


void RMSeries::init(int length_, double** x)
{
    init(length_, 1, x);
}


void RMSeries::init(int length_, double* x)
{
    initCopy(length_, x);
    length = length_;
    width = 1;
}


void RMSeries::init(int length_, int width_)
{
    initCopy(length_, width_, NULL);
    length = length_;
    width = width_;
}


void RMSeries::init(int length_, int width_, double** xy)
{
    initCopy(length_, width_, xy);
    length = length_;
    width = width_;
}

/*
inline
double& RMSeries::vals(int idx)
{
	return vals_raw[idx][0];
}


inline
double& RMSeries::vals(int idx1, int idx2)
{
	// no error checking because we assume you know what you're doing
	return vals_raw[idx1][idx2];
}


inline
double RMSeries::vals(int idx) const
{
	return vals_raw[idx][0];
}


inline
double RMSeries::vals(int idx1, int idx2) const
{
	return vals_raw[idx1][idx2];
}
*/

/*
included in class def
inline
double& RMSeries::operator()(int idx)
{
	return vals_raw[idx][0];
}


inline
double RMSeries::operator()(int idx) const
{
	return vals_raw[idx][0];
}


inline
double& RMSeries::operator()(int idx1, int idx2)
{
	return vals_raw[idx1][idx2];
}


inline
double RMSeries::operator()(int idx1, int idx2) const
{
	return vals_raw[idx1][idx2];
}
*/


void RMSeries::initCopy(int length_, double* input)
{
	// If the array is already initialized, then we check if it
	// is the same size as the requested input array.  If it is
	// we save time by not re-creating the array.
	if (vals_raw != NULL)
	{
		if (length == length_ && width == 1)
			return;
		else
			clear();
	}

    vals_raw = new double*[length_];
    for (int i = 0; i < length_; i++)
    {
        vals_raw[i] = new double[1];
        vals_raw[i][0] = input[i];
    }
}


void RMSeries::initCopy(int length_, int width_, double** input)
{
	// If the array is already initialized, then we check if it
	// is the same size as the requested input array.  If it is
	// we save time by not re-creating the array.
	if (vals_raw != NULL)
	{
		if (length == length_ && width == width_)
			return;
		else
			clear();
	}

    vals_raw = new double*[length_];
    for (int i = 0; i < length_; i++)
    {
        vals_raw[i] = new double[width_];
        for (int j = 0; j < width_; j++)
        {
            if (input == NULL)
                vals_raw[i][j] = 0.0;
            else
                vals_raw[i][j] = input[i][j];
        }
    }
}


double* RMSeries::nextp()
{
    iterPos++;
    if (iterPos >= length)
    {
        iterPos = -1;
        return NULL;
    }
    else
        return vals_raw[iterPos];
}


void RMSeries::resetp()
{
    iterPos = -1;
}


RMSeries::~RMSeries()
{
    clear();
}

void RMSeries::clear()
{
    if (vals_raw != NULL && length > 0)
    {
        for (int i = 0; i < length; i++)
            delete[] vals_raw[i];
        delete[] vals_raw;
    }
    vals_raw = NULL;
    length = width = 0;
    iterPos = -1;
}

