#ifndef XMLELEMENT_H
#define XMLELEMENT_H
///////////////////////////////////////////////////////////////////
// XmlElement.h - define XML Element types                       //
// ver 1.7                                                       //
// Application: Help for CSE687 Pr#2, Spring 2015                //
// Platform:    Dell XPS 2720, Win 8.1 Pro, Visual Studio 2013   //
//  Author:        Arpit Shah									 //
//  Instructed By: Jim Fawcett                                   //
///////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package is intended to help students in CSE687 - Object Oriented Design
* get started with Project #2 - XML Document Model.  It uses C++11 constructs,
* most noteably std::shared_ptr.  The XML Document Model is essentially
* a program-friendly wrapper around an Abstract Syntax Tree (AST) used to
* contain the results of parsing XML markup.
*
* Abstract Syntax Trees are unordered trees with two types of nodes:
*   Terminal nodes     - nodes with no children
*   Non-Terminal nodes - nodes which may have a finite number of children
* They are often used to contain the results of parsing some language.
*
* The elements defined in this package will be used in the AST defined in
* the AbstractSyntaxTree package, included in this Visual Studio 2013
* solution.
*   AbstractXmlElement - base for all the XML Element types
*   DocElement         - XML element with children designed to hold prologue, Xml root, and epilogue
*   TaggedElement      - XML element with tag, attributes, child elements
*   TextElement        - XML element with only text, no markup
*   CommentElement     - XML element with comment markup and text
*   ProcInstrElement   - XML element with markup and attributes but no children
*   XmlDeclarElement   - XML declaration
*
* Required Files:
* ---------------
*   - XmlElement.h, XmlElement.cpp
*
* Build Process:
* --------------
* devenv ArpitShahHW2.sln /debug rebuild
*
* Maintenance History:
* --------------------
* ver 1.0 : 07 Feb 2017
* - first release
*/

#include <memory>
#include <string>
#include <vector>

namespace XmlProcessing
{
	/////////////////////////////////////////////////////////////////////////////
	// AbstractXmlElement - base class for all concrete element types

	class AbstractXmlElement
	{
	public:
		using sPtr = std::shared_ptr < AbstractXmlElement >;

		virtual bool addChild(std::shared_ptr<AbstractXmlElement> pChild);
		virtual bool removeChild(std::shared_ptr<AbstractXmlElement> pChild);
		virtual bool addAttrib(const std::string& name, const std::string& value);
		virtual bool removeAttrib(const std::string& name);
		virtual std::vector<sPtr> children();
		virtual std::string tag() { return ""; }
		virtual std::string value() = 0;
		virtual std::string toString() = 0;
		virtual ~AbstractXmlElement();
	protected:
		static size_t count;
		static size_t tabSize;
	};

	inline bool AbstractXmlElement::addChild(std::shared_ptr<AbstractXmlElement> pChild) { return false; }
	inline bool AbstractXmlElement::removeChild(std::shared_ptr<AbstractXmlElement> pChild) { return false; }
	inline std::vector<AbstractXmlElement::sPtr> AbstractXmlElement::children()
	{
		return std::vector<sPtr>();
	}
	inline bool AbstractXmlElement::addAttrib(const std::string& name, const std::string& value) { return false; }
	inline bool AbstractXmlElement::removeAttrib(const std::string& name) { return false; }
	inline AbstractXmlElement::~AbstractXmlElement() {}

	/////////////////////////////////////////////////////////////////////////////
	// DocElement - holds the document prologue, XML tree, and epilog

	class DocElement : public AbstractXmlElement
	{
	public:
		DocElement(std::shared_ptr<AbstractXmlElement> pRoot = nullptr);
		~DocElement() {}
		DocElement(const DocElement& doc) = delete;
		DocElement& operator=(const DocElement& doc) = delete;
		virtual bool addChild(std::shared_ptr<AbstractXmlElement> pChild);
		virtual bool removeChild(std::shared_ptr<AbstractXmlElement> pChild);
		virtual std::vector<sPtr> children();
		virtual std::string value();
		virtual std::string toString();
	private:
		bool hasXmlRoot();
		std::vector<std::shared_ptr<AbstractXmlElement>> children_;
	};

	inline std::vector<AbstractXmlElement::sPtr> DocElement::children()
	{
		return children_;
	}
	std::shared_ptr<AbstractXmlElement> makeDocElement(std::shared_ptr<AbstractXmlElement> pRoot = nullptr);

	/////////////////////////////////////////////////////////////////////////////
	// TextElement - represents the text part of an XML element

	class TextElement : public AbstractXmlElement
	{
	public:
		TextElement(const std::string& text) : text_(text) {}
		virtual ~TextElement() {}
		TextElement(const TextElement& te) = delete;
		TextElement& operator=(const TextElement& te) = delete;
		virtual std::string value();
		virtual std::string toString();
	private:
		std::string text_;
	};

	inline std::string TextElement::value() { return text_; }

	std::shared_ptr<AbstractXmlElement> makeTextElement(const std::string& text);

	/////////////////////////////////////////////////////////////////////////////
	// Element - represents a tagged element with attributes and child elements

	class TaggedElement : public AbstractXmlElement
	{
	public:
		TaggedElement(const std::string& tag) : tag_(tag) {};
		TaggedElement(const TaggedElement& te) = delete;
		virtual ~TaggedElement() {}
		TaggedElement& operator=(const TaggedElement& te) = delete;
		virtual bool addChild(std::shared_ptr<AbstractXmlElement> pChild);
		virtual bool removeChild(std::shared_ptr<AbstractXmlElement> pChild);
		virtual std::vector<sPtr> children();
		virtual bool addAttrib(const std::string& name, const std::string& value);
		virtual bool removeAttrib(const std::string& name);
		virtual std::string tag();
		virtual std::string value();
		virtual std::string toString();
	private:
		std::string tag_;
		std::vector<std::shared_ptr<AbstractXmlElement>> children_;
		std::vector<std::pair<std::string, std::string>> attribs_;
	};

	inline std::vector<AbstractXmlElement::sPtr> TaggedElement::children()
	{
		return children_;
	}
	inline std::string TaggedElement::tag() { return tag_; }
	std::shared_ptr<AbstractXmlElement> makeTaggedElement(const std::string& tag);

	/////////////////////////////////////////////////////////////////////////////
	// CommentElement - represents XML comments, e.g., <!-- comment text -->
	//
	// Incomplete

	class CommentElement : public AbstractXmlElement
	{
	public:
		CommentElement(const std::string& text) : commentText_(text) {}
		CommentElement(const CommentElement& ce) = delete;
		virtual ~CommentElement() {}
		CommentElement& operator=(const CommentElement& ce) = delete;
		virtual std::string value() { return commentText_; }
		virtual std::string toString();
	private:
		std::string commentText_ = "to be defined";
	};

	std::shared_ptr<AbstractXmlElement> makeCommentElement(const std::string& comment);

	/////////////////////////////////////////////////////////////////////////////
	// ProcInstrElement - represents XML Processing Instructions, e.g., <?xml version="1.0"?>
	//
	// Incomplete

	class ProcInstrElement : public AbstractXmlElement
	{
	public:
		ProcInstrElement(const ProcInstrElement& pe) = delete;
		ProcInstrElement(const std::string& type) : type_(type) {}
		ProcInstrElement& operator=(const ProcInstrElement& pe) = delete;
		virtual bool addAttrib(const std::string& name, const std::string& value);
		virtual bool removeAttrib(const std::string& name);
		virtual std::string value() { return type_; }
		virtual std::string toString();
	private:
		std::vector<std::pair<std::string, std::string>> attribs_;
		std::string type_ = "xml declaration";
		std::string text;
	};

	std::shared_ptr<AbstractXmlElement> makeProcInstrElement(const std::string& type);

	/////////////////////////////////////////////////////////////////////////////
	// XmlDeclarElement - <?xml version="1.0"?>

	class XmlDeclarElement : public AbstractXmlElement
	{
	public:
		XmlDeclarElement(const ProcInstrElement& pe) = delete;
		XmlDeclarElement() {}
		XmlDeclarElement& operator=(const ProcInstrElement& pe) = delete;
		virtual bool addAttrib(const std::string& name, const std::string& value);
		virtual bool removeAttrib(const std::string& name);
		virtual std::string value() { return ""; }
		virtual std::string toString();
	private:
		std::vector<std::pair<std::string, std::string>> attribs_;
		std::string type_ = "xml declaration";
	};

	std::shared_ptr<AbstractXmlElement> makeXmlDeclarElement();


	inline void title(const std::string& title, char underlineChar = '-');
}
#endif
