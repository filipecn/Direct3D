#include "TokenStream.h"

TokenStream::TokenStream() {
	ResetStream();
}
void TokenStream::ResetStream() {
	startIndex_ = endIndex_ = 0;
}
void TokenStream::SetTokenStream(char *data) {
	ResetStream(); 
	data_ = data;
}

bool isValidIdentifier(char c) {
	// Ascii from ! to ~. 
	if ((int)c > 32 && (int)c < 127)
		return true;
	return false;
}

bool isValidIdentifier(char c, char* delimiters, int totalDelimiters) {
	if (delimiters == 0 || totalDelimiters == 0)
		return isValidIdentifier(c);
	for (int i = 0; i < totalDelimiters; i++) {
		if (c == delimiters[i])
			return false;
	}
	return true;
}

bool TokenStream::GetNextToken(std::string* buffer, char* delimiters, int totalDelimiters)
{
	startIndex_ = endIndex_;
	bool inString = false; 
	int length = (int)data_.length();
	if (startIndex_ >= length - 1) 
		return false;

	while (startIndex_ < length && isValidIdentifier(data_[startIndex_], delimiters, totalDelimiters) == false)
	{
		startIndex_++;
	} 
	
	endIndex_ = startIndex_ + 1;
	if (data_[startIndex_] == '"' ) 
		inString = !inString;
	if (startIndex_ < length) {
		while (endIndex_ < length && (isValidIdentifier(data_[endIndex_], delimiters, totalDelimiters) || 
			inString == true)) {
			if (data_[endIndex_] == '"')
				inString = !inString;
			endIndex_++;
		}
			if (buffer != NULL) {
				int size = (endIndex_ - startIndex_); 
				int index = startIndex_;
				buffer->reserve(size + 1); 
				buffer->clear();

				for (int i = 0; i < size; i++) {
					buffer->push_back(data_[index++]);
				}
		}
		return true;
	}
	return false;
}