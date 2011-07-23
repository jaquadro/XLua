#ifndef TAGGED_TYPE_H_
#define TAGGED_TYPE_H_

class TaggedType {
public:

	short type;
	union {
		short	s;
		int		i;
		float	f;
		double	d;
		char*	cstr;
	};

	enum {
		FIELD_NULL, FIELD_SHORT, FIELD_INT, FIELD_FLOAT, FIELD_DOUBLE, FIELD_STRING
	};

	TaggedType () : type(FIELD_NULL), i(0) { }

	TaggedType (short v) : type(FIELD_SHORT), s(v) { }
	TaggedType (int v) : type(FIELD_INT), i(v) { }
	TaggedType (float v) : type(FIELD_FLOAT), f(v) { }
	TaggedType (double v) : type(FIELD_DOUBLE), d(v) { }

	TaggedType (const std::string& str)
		: type(FIELD_STRING) {
		cstr = new char[str.size() + 1];
		strcpy(cstr, str.c_str());
	}

	~TaggedType () {
		CleanupString();
	}

	void CleanupString () {
		if (type == FIELD_STRING) {
			delete[] cstr;
			cstr = 0;
		}
	}

	void SetInt (int v) {
		CleanupString();
		type = FIELD_INT;
		i = v;
	}

	void SetFloat (float v) {
		CleanupString();
		type = FIELD_FLOAT;
		f = v;
	}

	void SetString (const std::string& str) {
		CleanupString();
		type = FIELD_STRING;

		cstr = new char[str.size() + 1];
		strcpy(cstr, str.c_str());
	}
};

#endif
