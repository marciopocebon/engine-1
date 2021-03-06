/**
 * @file
 */
#pragma once

#include "core/String.h"
#include "core/NonCopyable.h"
#include "core/collection/StringMap.h"
#include <memory>

namespace backend {

template<class TYPE, class CTX>
class IFactory {
public:
	virtual ~IFactory() {}
	virtual std::shared_ptr<TYPE> create (const CTX* ctx) const = 0;
};

template<class TYPE, class CTX>
class IFactoryRegistry: public core::NonCopyable {
protected:
	typedef core::StringMap<const IFactory<TYPE, CTX>*> FactoryMap;
	typedef typename FactoryMap::iterator FactoryMapConstIter;
	typedef typename FactoryMap::iterator FactoryMapIter;
	FactoryMap _factories;
public:
	bool registerFactory (const core::String& type, const IFactory<TYPE, CTX>& factory)
	{
		FactoryMapConstIter i = _factories.find(type);
		if (i != _factories.end()) {
			return false;
		}

		_factories.put(type, &factory);
		return true;
	}

	bool unregisterFactory (const core::String& type)
	{
		FactoryMapIter i = _factories.find(type);
		if (i == _factories.end()) {
			return false;
		}

		_factories.erase(i);
		return true;
	}

	std::shared_ptr<TYPE> create (const core::String& type, const CTX* ctx = nullptr) const
	{
		FactoryMapConstIter i = _factories.find(type);
		if (i == _factories.end()) {
			return std::shared_ptr<TYPE>();
		}

		const IFactory<TYPE, CTX>* factory = i->second;

		return factory->create(ctx);
	}
};

}
