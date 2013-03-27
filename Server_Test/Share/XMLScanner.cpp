#include "XMLScanner.h"
#include <stdarg.h>

CXMLScanner::CXMLScanner(const char *filename)
	: doc_(NULL), current_(NULL), cursor_(NULL)
{
	filename && this->open(filename);
}

CXMLScanner::~CXMLScanner()
{
	for (size_t i = 0; i < this->array_.size(); ++i)
		xmlFree((void *)this->array_[i]);
	this->array_.clear();
	if (this->doc_)
		xmlFree(this->doc_);
}

bool CXMLScanner::open(const char *filename)
{
	xmlDocPtr doc = xmlParseFile(filename);
	if (doc)
	{
		this->~CXMLScanner();
		this->doc_ = doc;
		this->current_ = xmlDocGetRootElement(this->doc_);
		this->cursor_ = NULL;
		return true;
	}
	return false;
}

bool CXMLScanner::seek(const char *path, size_t length)
{
	if (NULL != path || NULL != this->doc_)
	{
		xmlNodePtr current, cursor;

		current = this->current_;
		cursor = this->cursor_;

		if (length && *path == '/')
		{
			current = xmlDocGetRootElement(this->doc_);
			++path;
			--length;
		}

		while (length && *path)
		{
			size_t node_length;

			for (node_length = 0; node_length < length; ++node_length)
				if ('/' == path[node_length] || '\0' == path[node_length])
					break;

			if (node_length)
			{
				while (current && xmlStrncmp(current->name, (const xmlChar *)path, node_length))
					current = current->next;
				if (NULL == current)
					break;
				current = current->xmlChildrenNode;
			}
			node_length < length && path[node_length] && ++node_length;
			path += node_length;
			length -= node_length;
		}

		if (!*path || !length)
		{
			this->current_ = current;
			this->cursor_ = NULL;
			return true;
		}
	}

	return false;
}

bool CXMLScanner::seek()
{
	if (NULL != this->doc_ && NULL != this->cursor_)
	{
		this->current_ = this->cursor_->xmlChildrenNode;
		this->cursor_ = NULL;
		return true;
	}

	return false;
}

int CXMLScanner::scan(int reposition, const char *format, char **pcontent, ...)
{
	if (NULL == this->doc_)
		return -1;

	int rc;
	const char *p;
	p = strrchr(format, '/');
	if (p)
	{
		if (false == this->seek(format, p - format))
			return -1;
		format = p + 1;
	}

	rc = -1;
	if (*format)
	{
		this->cursor_ = (reposition || NULL == this->cursor_)? this->current_: this->cursor_->next;

		while (this->cursor_ && xmlStrcmp(this->cursor_->name, (const xmlChar *)format))
			this->cursor_ = this->cursor_->next;

		if (this->cursor_)
		{
			rc = 0;
			format += strlen(format) + 1;

			if (pcontent)
			{
				const xmlChar *pstr = xmlNodeListGetString(this->doc_, this->cursor_->xmlChildrenNode, 1);
				*pcontent = (char *)pstr;
				if (pstr)
				{
					this->array_.push_back(pstr);
				}
			}

			va_list ap;
			va_start(ap, pcontent);

			while (*format)
			{
				char **pv = va_arg(ap, char **);
				xmlChar *pstr = xmlGetProp(this->cursor_, (const xmlChar *)format);
				pv && (*pv = (char *)pstr);
				if (pstr)
				{
					++rc;
					this->array_.push_back(pstr);
				}
				format += strlen(format) + 1;
			}
			va_end(ap);
		}
	}

	return rc;
}

XPathFile::~XPathFile()
{
	std::vector<xmlChar *>::iterator iter1;
	for (iter1 = this->chars_.begin(); iter1 != this->chars_.end(); ++iter1)
		xmlFree(*iter1);

	std::vector<xmlXPathObjectPtr>::iterator iter2;
	for (iter2 = this->objects_.begin(); iter2 != this->objects_.end(); ++iter2)
		xmlXPathFreeObject(*iter2);

	if (this->context_)
		xmlXPathFreeContext(this->context_);
	if (this->doc_)
		xmlFree(this->doc_);
}

bool XPathFile::open(const char *filename)
{
	bool rc = false;

	if ((this->doc_ = xmlParseFile(filename)) == NULL)
		fprintf(stderr, "Error: unable to parse file \"%s\"\n", filename);
	else if ((this->context_ = xmlXPathNewContext(this->doc_)) == NULL)
		fprintf(stderr, "Error: unable to create new XPath context\n");
	else
		rc = true;

	return rc;
}

xmlNodeSetPtr XPathFile::find(const char *path)
{
	xmlXPathObjectPtr ptr = xmlXPathEvalExpression(BAD_CAST path, this->context_);
	if (ptr)
	{
		this->objects_.push_back(ptr);
		return ptr->nodesetval;
	}

	return NULL;
}

const char *XPathFile::property(xmlNodeSetPtr set, size_t index, const char *name)
{
	if (set && index < (size_t)xmlXPathNodeSetGetLength(set))
	{
		xmlChar *rc = xmlGetProp(set->nodeTab[index], BAD_CAST name);
		if (rc)
		{
			this->chars_.push_back(rc);
			return (const char *)rc;
		}
	}
	return NULL;
}

const char *XPathFile::content(xmlNodeSetPtr set, size_t index)
{
	if (set && index < (size_t)xmlXPathNodeSetGetLength(set))
	{
		xmlChar *rc = xmlNodeGetContent(set->nodeTab[index]);
		if (rc)
		{
			this->chars_.push_back(rc);
			return (const char *)rc;
		}
	}
	return NULL;
}

int XPathFile::scanf(xmlNodePtr node, const char *format, ...)
{
	int rc = 0;
	va_list ap;

	va_start(ap, format);
	while (*format)
	{
		char **pv = va_arg(ap, char **);
		xmlChar *pstr = xmlGetProp(node, BAD_CAST format);
		if (!pstr && !xmlStrcmp(node->name, BAD_CAST format))
			pstr = xmlNodeGetContent(node);
		*pv = (char *)pstr;
		if (pstr)
		{
			++rc;
			this->chars_.push_back(pstr);
		}
		format += strlen(format) + 1;
	}
	va_end(ap);

	return rc;
}

