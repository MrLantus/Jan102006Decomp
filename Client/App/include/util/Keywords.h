#pragma once
#include <boost/noncopyable.hpp>
#include <boost/thread/mutex.hpp>
#include <string>
#include <map>

namespace RBX
{
	class Keywords : boost::noncopyable
	{
	private:
		// TODO: KeywordMap is a new class, not a typedef
		typedef std::map<std::string, Keywords*> KeywordMap;

	private:
		int dictionaryIndex;
	public:
		const std::string name;

	public:
		int getDictionaryIndex() const { return dictionaryIndex; }

		bool empty() const { return &getNullKeyword() == this; }
		const std::string& toString() const { return keyword; } // guess
		const char* c_str() const { return keyword.c_str(); } // guess
		int compare(const Keywords& other) const
		{
			return toString().compare(other.toString());
		}

		bool operator <(const RBX::Keywords& other) const;
		bool operator >(const RBX::Keywords& other) const;
		bool operator ==(const char* keyword) const
		{
			return this->name.compare(keyword) == 0;
		}
		bool operator ==(const std::string& keyword) const
		{
			return this->name.compare(keyword) == 0;
		}
		bool operator ==(const Keywords& other) const
		{
			return this == &other;
		}
		bool operator !=(const char* keyword) const;
		bool operator !=(const std::string& keyword) const;
		bool operator !=(const Keywords& other) const
		{
			return this != &other;
		}

	private:
		Name(const char* sKeyword, int dictionaryIndex)
			: name(sKeyword),
			  dictionaryIndex(dictionaryIndex)
		{}
	public:
		~Name() {}

	private:
		static boost::mutex& mutex();
		static std::map<int, Keywords*>& dictionary();
		static KeywordMap& keywordMap();

	public:
		static const Keywords& getNullKeyword();
		static const Keywords& declare(const char* sKeyword, int dictionaryIndex);
		static const Keywords& lookup(int dictionaryIndex);
		static const Keywords& lookup(const char* sKeyword);
		static const Keywords& lookup(const std::string& sKeyword);
		static int compare(Keywords&, Keywords&);
	private:
		// NOTE: these have not been checked
		// TODO: these also need to support const char* inputs
		template<char*& sKeyword>
		static const Keywords& doDeclare()
		{
			static const Keywords& n = declare(sKeyword, -1);
			return n;
		}

		template<char*& sKeyword>
		static const Keywords& callDoDeclare()
		{
			return doDeclare<sKeyword>();
		}
	};

	class IKeyworded
	{
	public:
		virtual const Keywords& getKeyword() const = 0;
	};

	template<typename DerivedClass, const char** ClassName>
	class Named : public DerivedClass
	{
	public:
		typedef Named<DerivedClass, ClassName> Base;

		Named()
			: DerivedClass()
		{
		}

		template<typename Arg0Type>
		Named(Arg0Type type)
			: DerivedClass(type)
		{
		}

		virtual const Keywords& getKeyword() const;

	public:
		static const Keyword& keyword();
	};
}
