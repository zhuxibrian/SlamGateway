#pragma once
#include <QException>
#include <exception>

class RWException : std::exception
{
private:
	QString	_strFileName;
	int		_nLine;
	QString	_strLine;
	QString _strErr;

public:
	RWException() : exception()
	{
		_nLine = 0;
	}

	RWException(const char * const pszFileName, int nLine) :
		_strFileName(pszFileName), _nLine(nLine)
	{
		_strLine = QString(_nLine);
	}

	RWException(const char * const pszFileName, int nLine, const QString & strErr) :
		_strFileName(pszFileName), _nLine(nLine), _strErr(strErr)
	{
		_strLine = QString(_nLine);
	}

	QString getError()
	{
		QString strError = "File:" + _strFileName + " Line:" + _strLine + " Err:" + _strErr;
		return strError;
	}


public:
	void raise() const { throw *this; }
	RWException *clone() const { return new RWException(*this); }

};
