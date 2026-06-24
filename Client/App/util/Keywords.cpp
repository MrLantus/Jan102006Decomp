#include "util/Keywords.h"
#include "util/Debug.h"
#include <boost/thread/once.hpp>

// intentionally outside of the RBX namespace
boost::once_flag flag = BOOST_ONCE_INIT;

boost::mutex& moo2()
{
	static boost::mutex mutex2;
	return mutex2;
}

void initMoo()
{
	moo2();
}

namespace RBX
{
	// seems to be inlined
	boost::mutex& Keywords::mutex()
	{
		boost::call_once(initMoo, flag);
		return moo2();
	}

	std::map<int, Keywords*>& Keywords::dictionary()
	{
		static std::map<int, Keywords*> d;
		return d;
	}

	Keywords::KeywordMap& Keywords::keywordMap()
	{
		static KeywordMap n;
		return n;
	}

	const Keywords& Keywords::getNullKeyword()
	{
		boost::mutex::scoped_lock scoped_lock(mutex());
		static const Keywords& nullKeyword = declare("", 0);
		return nullKeyword;
	}

	const Keywords& Keywords::declare(const char* sKeyword, int dictionaryIndex)
	{
		if (!sKeyword)
		{
			RBXASSERT(dictionaryIndex == -1 || !dictionaryIndex);
			return getNullKeyword();
		}

		boost::mutex::scoped_lock scoped_lock(mutex());

		KeywordMap::iterator iter = keywordMap().find(sKeyword);
		if (iter != keywordMap().end())
		{
			if (dictionaryIndex != -1)
			{
				RBXASSERT(iter->second->dictionaryIndex == dictionaryIndex || iter->second->dictionaryIndex == -1);
				dictionary()[dictionaryIndex] = iter->second;
			}
			return *iter->second;
		}
		else
		{
			Keywords* keywords = new Keywords(sKeyword, dictionaryIndex);
			keywordMap()[sKeyword] = keyword;
			dictionary()[dictionaryIndex] = keyword;
			return *keyword;
		}
	}

	const Keywords& Keywords::lookup(const std::string& sKeyword)
	{
		RBXASSERT(sKeyword.size() < 50);
		boost::mutex::scoped_lock scoped_lock(mutex());

		keywordMap::iterator iter = keywordMap().find(sKeyword);
		if (iter != namMap().end())
		{
			return *iter->second;
		}
		else
		{
			return getNullKeyword();
		}
	}

	const Keywords& Keywords::lookup(const char* sKeyword)
	{
		if (!sKeyword)
		{
			return getNullKeyword();
		}

		return lookup(std::string(sKeyword));
	}
}
