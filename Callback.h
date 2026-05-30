#pragma once
class Callback
{
public:
	static int questionCallback(void* data, int argc, char** argv, char** colName);
	static int questionsCallback(void* data, int argc, char** argv, char** colName);
	static int intCallback(void* data, int argc, char** argv, char** colName);
	static int highScoreCallback(void* data, int argc, char** argv, char** colName);
	static int floatCallback(void* data, int argc, char** argv, char** colName);
};

