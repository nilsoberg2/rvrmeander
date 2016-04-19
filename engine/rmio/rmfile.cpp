
#include <cstdio>
#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
#include "rmio.h"
#include "rmseries.h"

/////////////////////////////////////////////////////////////////////////
// CONFIGURATION FILE READING
/*
int loadConfig_Text(FILE* fp);


int LoadConfig(char* file, int type)
{

    if (type != FILE_TEXT)
        return ERR_NOT_IMPL;//loadXYSeries_DBF(file);

    FILE* fp;
    int result = 0;

    fp = fopen(file, "r");

    result = loadConfig_Text(fp);

    if (fp != NULL)
        fclose(fp);

    return result;
}


int loadConfig_Text(FILE* fp)
{
    if (fp == NULL)
        return ERR_FILE_OPEN;

    char buffer[2000];
    char* tok = NULL;
    char* keyword = NULL;
    char delim[] = " \t\n";
    int count = 0;

    while (!feof(fp))
    {
        fgets(buffer, 1999, fp);
        
        // Get the keyword
        tok = strtok(buffer, delim);
        if (tok == NULL)
            continue;
        else
            keyword = tok;

        // Get the = that delimits the keyword/value (optional)
        tok = strtok(buffer, delim);
        if (tok == NULL)
        {
            //config.insert(std::make_pair(keyword, ""));
            count++;
            continue;
        }
        else if (strcmp(tok, "=") != 0)
        {
            //config.insert(std::make_pair(keyword, tok));
            count++;
            continue;
        }
        // else nothing because this should be an =

        // Get the value
        tok = strtok(buffer, delim);
        if (tok != NULL)
        {
            //config.insert(std::make_pair(keyword, tok));
            count++;
        }
    }

    return count;
}
*/
// END CONFIGURATION FILE READING
/////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////
// SERIES FILE READING

int loadSeries_Text(FILE* fp, RMSeries& series);
int loadSeries_Text_readX(FILE* fp, RMSeries& series);
int loadSeries_Text_readXY(FILE* fp, RMSeries& series);
int loadSeries_Text_readXYZ(FILE* fp, RMSeries& series);


LIBRMIO_DLL_IMP_EXP int LoadSeries(const char* file, int type, RMSeries& series)
{
    series.clear();

    if (type == FILE_DBF)
        return ERR_NOT_IMPL;//loadXYSeries_DBF(file);
    else if (type == FILE_NETCDF)
        return ERR_NOT_IMPL;//loadXYSeries_NetCDF(file);

    FILE* fp = NULL;
    int result = 0;

    if (type == FILE_BINARY)
    {
        //fp = fopen(file, "rb");
        result = ERR_NOT_IMPL;
    }
    else if (type == FILE_TEXT)
    {
        fp = fopen(file, "r");
        result = loadSeries_Text(fp, series);
    }
    else
        result = ERR_BAD_ARG;

    if (fp != NULL)
        fclose(fp);

    return result;
}


int loadSeries_Text(FILE* fp, RMSeries& series)
{
    if (fp == NULL)
        return ERR_BAD_ARG;

    /*
    FORMAT:
RMTEXT #dim #rows [#cols [#Z]] x1 y1... x2 y2...
x3 y3...
x4 y4... x5 y5... x6 y6...
...
    */

    char buffer[80];
    memset(buffer, 0, 80* sizeof(char));
    int width = 0, length = 0, nRead = 0;

    nRead = fscanf(fp, "%s %d %d", buffer, &length, &width);
    if (nRead != 3)
        return ERR_FILE_READ;
    if (strcmp(buffer, "RMTEXT") != 0)
        return ERR_FILE_FORMAT;

    int readCount = -1;
    if (width == DIM_X)
    {
        series.init(length, 1);
        readCount = loadSeries_Text_readX(fp, series);
    }
    else if (width == DIM_XY)
    {
        series.init(length, 2);
        readCount = loadSeries_Text_readXY(fp, series);
    }
    else if (width == DIM_XYZ)
    {
        series.init(length, 3);
        readCount = loadSeries_Text_readXYZ(fp, series);
    }

    if (readCount < 0)
        return readCount; // an error
    else if (readCount != length*width)
        return ERR_FILE_ELEM_COUNT;
    else
        return readCount;
}


int loadSeries_Text_readX(FILE* fp, RMSeries& series)
{
    double x = 0.0;
    double* temp;
    int count = 0, nRead = 0;
    nRead = fscanf(fp, "%lf", &x);
    while (nRead == 1)
    {
        count++;

        temp = series.nextp();
        if (temp == NULL)
            break;
        temp[0] = x;

        nRead = fscanf(fp, "%lf", &x);
    }

    return count;
}


int loadSeries_Text_readXY(FILE* fp, RMSeries& series)
{
    double x = 0.0, y = 0.0;
    double* temp;
    int count = 0, nRead = 0;
    nRead = fscanf(fp, "%lf %lf", &x, &y);
    while (nRead == 2)
    {
        count += 2;
        
        temp = series.nextp();
        if (temp == NULL)
            break;
        temp[0] = x;
        temp[1] = y;
        
        nRead = fscanf(fp, "%lf %lf", &x, &y);
    }

    return count;
}


int loadSeries_Text_readXYZ(FILE* fp, RMSeries& series)
{
    double x = 0.0, y = 0.0, z = 0.0;
    double* temp;
    int count = 0, nRead = 0;
    nRead = fscanf(fp, "%lf %lf %lf", &x, &y, &z);
    while (nRead == 3)
    {
        count += 3;
        
        temp = series.nextp();
        if (temp == NULL)
            break;
        temp[0] = x;
        temp[1] = y;
        temp[2] = z;
        
        nRead = fscanf(fp, "%lf %lf %lf", &x, &y, &z);
    }

    return count;
}

// END SERIES FILE READING
/////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////
// SERIES FILE WRITING

int saveSeries_Text(FILE* fp, RMSeries& series);
int saveSeries_Tecplot(FILE* fp, RMSeries& series);


LIBRMIO_DLL_IMP_EXP int SaveSeries(const char* file, int type, RMSeries& series)
{
    if (type == FILE_DBF)
        return ERR_NOT_IMPL;//loadXYSeries_DBF(file);
    else if (type == FILE_NETCDF)
        return ERR_NOT_IMPL;//loadXYSeries_NetCDF(file);

    FILE* fp = NULL;
    int result = 0;

    if (type == FILE_BINARY)
    {
        result = ERR_NOT_IMPL;
    }
    else if (type == FILE_TEXT)
    {
        fp = fopen(file, "w");
        result = saveSeries_Text(fp, series);
    }
	else if (type == FILE_TECPLOT && series.width >= DIM_X && series.width <= DIM_XYZ)
	{
		fp = fopen(file, "w");
		result = saveSeries_Tecplot(fp, series);
	}
    else
        result = ERR_BAD_ARG;

    if (fp != NULL)
        fclose(fp);

    return result;
}


int saveSeries_Tecplot(FILE* fp, RMSeries& series)
{
    series.resetp();

    if (fp == NULL)
        return ERR_BAD_ARG;

    /*
    FORMAT:
TITLE = ""
VARIABLES = X,Y

ZONE I=4, ZONETYPE=Ordered, DATAPACKING=POINT
1 2
2 4
3 9
4 16
    */

	fprintf(fp, "TITLE = \"\"\n");
	if (series.width == DIM_X)
		fprintf(fp, "VARIABLES = X\n");
	else if (series.width == DIM_XY)
		fprintf(fp, "VARIABLES = X, Y\n");
	else if (series.width == DIM_XYZ)
		fprintf(fp, "VARIABLES = X, Y, Z\n");
	fprintf(fp, "ZONE I=%d, ZONETYPE=Ordered, DATAPACKING=POINT\n", series.length);

    int count = 0;

    double* p = series.nextp();
    while (p != NULL)
    {
        count += series.width;
        for (int i = 0; i < series.width; i++)
        {
            if (i > 0)
                fprintf(fp, "\t");
            fprintf(fp, "%f", p[i]);
        }
        fprintf(fp, "\n");
        p = series.nextp();
    }

    return count;
}


int saveSeries_Text(FILE* fp, RMSeries& series)
{
    series.resetp();

    if (fp == NULL)
        return ERR_FILE_OPEN;

    fprintf(fp, "RMTEXT %d %d\n", series.length, series.width);

    int count = 0;

    double* p = series.nextp();
    while (p != NULL)
    {
        count += series.width;
        for (int i = 0; i < series.width; i++)
        {
            if (i > 0)
                fprintf(fp, "\t");
            fprintf(fp, "%f", p[i]);
        }
        fprintf(fp, "\n");
        p = series.nextp();
    }

    return count;
}

// END SERIES FILE WRITING
/////////////////////////////////////////////////////////////////////////
