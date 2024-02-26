/*
 *Name: functions.cpp
 *Author: Justin Mingledorff
 *Created on November 16, 2011 @ 12:21PM
 *Purpose: To house all of the functions used in redcraft that are not part of the redcraft class
 */


template <class T>
inline string toString(const T &t) {
	stringstream ss;
	ss << t;
	return ss.str();
};

void createMask(const char *str, vector<bool> *mask, int mStop, int mStart){
	/**
	 * this function will create a vector of bools to represent a mask of all
	 * residues from residue=1 to residue=mStopResidue. the character array
	 * passed in represents the string value of this mask, and can is delimited
	 * by ','. the character '-' represents a range. numbers represent the
	 * ACTUAL residue number (the vector is indexed from 1). any other character
	 * is ignored. the function expects a pointer to a vector of booleans.
	 */

	/**
	 * Tokenizing the string
	 * =====================
	 * first we're going to break up our string based on the ',' delimiter. any
	 * invalid character is ignored here.
	 */

	//will hold our string's tokens
	vector<string> tokens;

	//will hold the current, unappended token
	string currentToken = "";
	//

	//for each character in str, ignoring the null terminator...
	for(int charIndex = 0; str[charIndex] != '\0'; charIndex++){
		//if we find a delimiter...
		if(str[charIndex] == ','){
			//and if our current token contains something...
			if(currentToken.size() > 0){
				//add that token to our list of tokens
				tokens.push_back(currentToken);

				//and then reset our current token
				currentToken = "";
			}
		}

		//if it wasn't a delimiter, but if it was a number or '-' character...
		else if((str[charIndex] >= '0' && str[charIndex] <= '9') || str[charIndex] == '-'){
			//then append it onto the end of our current token
			currentToken.append(1, str[charIndex]);
		}

		//if the current character has not been dealt with yet, it is not a valid character
		else{
			cerr << "Warning: unrecognized character in specified mask range" << endl;
			cerr << "         " << str << endl;
			cerr << "         ";
			for(int errIndex = 0; errIndex < charIndex; errIndex++)
				cerr << " ";
			cerr << "^" << endl;
			cerr << "         ignoring this character..." << endl;
		}
	}

	//finally, we need to push our last token into our list. first we'll check
	//to make sure the size of this token is non-zero.
	if(currentToken.size() > 0)
		tokens.push_back(currentToken);

	/**
	 * Creating default values
	 * =======================
	 * now we'll clear whatever was in the current mask before it was passed into
	 * this function, and then push a false value onto it for each index between
	 * 0 and mStopResidue. This is to ensure that the mask functions correctly
	 * for continuations.
	 */

	//clear out the old contents of our mask
	mask->clear();

	//for each index between 0 and mStopResidue...
	//FIXME: mStopResidue is available for THIS CLASS
	//      This function shouldn't belong to REDCRAFT -- should be standalone, and therefore not have access to this variable
	int max = mStop > mStart ? mStop : mStart; // in case of reverse run
	for(int defaultMask = 0; defaultMask <= max; defaultMask++)
		//push back a false value
		mask->push_back(false);

	/**
	 * Parsing the tokens
	 * ==================
	 * finally we go through each token and interpret it's values.
	 */

	//for each token we've got...
	for(size_t tokenIndex = 0; tokenIndex < tokens.size(); tokenIndex++){
		//go ahead and store it in an easy-access string variable
		string token = tokens[tokenIndex];

		//if there is no range indicator (i.e. lone value)...
		if(token.find('-') == string::npos){
			//convert the string to an integer
			int maskIndex = atoi(token.c_str());

			//if that integer falls within the range of our mask vector...
			if(maskIndex > 0 && maskIndex <= mStop)
				//set that index to true
				mask->at(maskIndex) = true;
		}

		//if we DID find a range indicator ('-')
		else{
			//find the index in the string that the range indicator is located at
			size_t rangeIndex = token.find('-');

			//so, if the range indicator is located at the first or last character
			//of this token, then there can't be any numbers before or after it.

			//if the range indicator falls in an acceptable place...
			if(rangeIndex > 0 && rangeIndex < token.size() - 1){
				//our starting index will be all characters before the '-', and
				//our ending index will be all characters after. these are
				//converted to integers.
				int maskIndexStart = atoi(token.substr(0, rangeIndex).c_str());
				int maskIndexEnd = atoi(token.substr(rangeIndex + 1, token.size()).c_str());

				//if our start index is larger than our end index...
				if(maskIndexStart > maskIndexEnd){
					//swap them (XOR style, bitches!)
					maskIndexStart = maskIndexStart ^ maskIndexEnd;
					maskIndexEnd = maskIndexStart ^ maskIndexEnd;
					maskIndexStart = maskIndexStart ^ maskIndexEnd;
				}

				//if the start index is smaller than 0 (our starting index), then
				//we'll clamp it to 1 (again, we index from 1 to mStopResidue).
				//the reason we're not clamping it to mStartResidue is because
				//if the start was at 5, and this range was from 2-3, then we
				//would end up mixing up our start and end (2 would turn to 5 and
				//the resulting range would be 5-3 and so the coming for loop
				//wouldn't run).

				//if our start index is out of range...
				if(maskIndexStart <= 0)
					//clamp it to the first residue
					maskIndexStart = 1;

				//if our stop index is out of range...
				if(maskIndexEnd > mStop)
					//clamp it to mStopResidue
					maskIndexEnd = mStop;

				//for each index between start and stop...
				for(int maskIndex = maskIndexStart; maskIndex <= maskIndexEnd; maskIndex++)
					//set the mask's value at that index to true
					mask->at(maskIndex) = true;
			}
		}
	}
}

