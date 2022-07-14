/*!
 ****************************************************************************
 * File xdAcisArchive.h
 *
 * Copyright(C), 2020, XDU.
 *
 * Author jhuang
 *
 * Date: 2020/04/26
 *
 * **************************************************************************
 * 
 */
#pragma once

#include "logical.h"
#include "binfile.hxx"

class xdArchive;

class xdAcisArchive : public BinaryFile
{
public:
	xdAcisArchive(xdArchive& anArchive);

	~xdAcisArchive();
public:
	virtual size_t read(void *buffer, size_t length, logical swap);

	virtual void write(const void *buffer, size_t length, logical swap);

	FilePosition set_mark() {
		return myPos;
	}
	FilePosition goto_mark(FilePosition file_pos) {
		return myPos = file_pos;
	}
	virtual logical  read_header(int &first, int &second, int &third, int &fourth);
	// 	virtual size_t  read_string (char *buffer, size_t maxlen=0) ;
	//	virtual char *  read_string (int &length) ;

protected:

private:
	xdArchive& myArchive;
	FilePosition myPos;
};

