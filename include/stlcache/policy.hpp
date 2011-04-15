#ifndef STLCACHE_POLICY_HPP_INCLUDED
#define STLCACHE_POLICY_HPP_INCLUDED

#include <set>

using namespace std;

#include <stlcache/exceptions.hpp>
#include <stlcache/victim.hpp>

namespace stlcache {
    template <class Key> class policy {
    public:
        virtual void insert(const Key& _k) throw(stlcache_invalid_key) =0;
        virtual void remove(const Key& _k) throw() =0;
        virtual void touch(const Key& _k) throw() =0;
        virtual void clear() throw() =0;
        virtual void swap(policy<Key>& _p) throw(stlcache_invalid_policy)=0;

        virtual const _victim<Key> victim() throw()  =0;
    };

    template <class Key> class policy_none : public policy<Key> {
        set<Key> _entries;
    public:
        policy_none<Key>& operator= ( const policy_none<Key>& x) throw() {
            this->_entries=x._entries;
            return *this;
        }
        policy_none(const policy_none<Key>& x) throw() {
            *this=x;
        }
        policy_none(const size_t& size ) throw() { }

        virtual void insert(const Key& _k) throw(stlcache_invalid_key) {
            _entries.insert(_k);
        }
        virtual void remove(const Key& _k) throw() {
            _entries.erase(_k);
        }
        virtual void touch(const Key& _k) throw() { /* Not used here */  }
        virtual void clear() throw() {
            _entries.clear();
        }
        virtual void swap(policy<Key>& _p) throw(stlcache_invalid_policy) {
            try {
                policy_none<Key>& _pn=dynamic_cast<policy_none<Key>& >(_p);
                _entries.swap(_pn._entries);
            } catch (const std::bad_cast& ) {
                throw stlcache_invalid_policy("Attempted to swap incompatible policies");
            }
        }

        virtual const _victim<Key> victim() throw() {
            if (_entries.rbegin()==_entries.rend()) {
                return _victim<Key>();
            }
            return _victim<Key>(*(_entries.rbegin()));
        }
    };
}

#endif /* STLCACHE_POLICY_HPP_INCLUDED */