#ifndef XMLCLASASINCLUDE
#define XMLCLASASINCLUDE

#include <base/StringClass.h>
#include <base/TagClass.h>

//XML Class
class XML{
private:
	TagList tag_list;
	void setTag(Tag &tag, String );
	void setProperty(Tag &tag, String source);
	String setPropertyName(Tag &tag, String &source);
	String setPropertyValue(Tag &tag, String &source);
	int Create(String &source, Tag &parent, String name = "root", int num = 0);
	TagList Create(String &source);
public:
	XML(String source);
	TagList getElementsByTagName(String tag_name);
};

#endif