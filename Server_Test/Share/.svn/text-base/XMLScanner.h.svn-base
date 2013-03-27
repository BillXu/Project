#ifndef _XML_SCANNER__H_
#define _XML_SCANNER__H_

#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <vector>
#include <string.h>

class CXMLScanner
{
	public:
		CXMLScanner(const char * = 0);
		~CXMLScanner();
		int scan(int, const char *, char **, ...);
		bool open(const char *);
		bool seek();
		bool seek(const char *path)
		{
			return this->seek(path, path? strlen(path): 0);
		}
	protected:
		bool seek(const char *, size_t);
	private:
		xmlDocPtr doc_;
		xmlNodePtr current_;
		xmlNodePtr cursor_;
		std::vector<const xmlChar *> array_;
};


class XPathFile
{
	public:
		XPathFile() {}
		~XPathFile();
		bool open(const char *filename);
		xmlNodeSetPtr find(const char *path);
		const char *property(xmlNodeSetPtr set, size_t index, const char *name);
		const char *content(xmlNodeSetPtr set, size_t index);
		int scanf(xmlNodePtr node, const char *format, ...);
	private:
		xmlDocPtr doc_;
		xmlXPathContextPtr context_;
		std::vector<xmlXPathObjectPtr> objects_;
		std::vector<xmlChar *> chars_;
};

#endif

