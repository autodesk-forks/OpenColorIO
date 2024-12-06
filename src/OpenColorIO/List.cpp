#include "List.h"

namespace OCIO_NAMESPACE
{
    List::List()
        : m_impl(new List::Impl)
    {
    }
    
    List::~List()
    {
        delete m_impl;
        m_impl = nullptr;
    }
    
    bool List::add(const char * str)
    {
        if ( !getImpl() || 
             !str || 
             !*str ) 
        {
            return false;
        }
    
        getImpl()->m_list.push_back(str);
        return true;
    }
    
    bool List::remove(const char * str)
    {
        if ( !getImpl() || 
             !str || 
             !*str)
        {
            return false;
        }
        
        std::vector<std::string>& list = getImpl()->m_list;
        auto it = std::find( list.begin(), list.end(), str );

        if( it == list.end() ) return false;

        list.erase( it );
        return true;
    }
    
    void List::clear()
    {
        if (getImpl())
        {
            getImpl()->m_list.clear();
        }
    }
    
    const char * List::get(int index) const
    {
        if (getImpl() && index >= 0 && index < (int) getImpl()->m_list.size())
        {
            return getImpl()->m_list[index].c_str();
        }
        
        return nullptr;
    }
    
    bool List::set( int index, const char * str )
    {
        if ( !getImpl() || 
             !str || 
             !*str )
        {
            return false;
        }
    
        if( index || 
            index >= getImpl()->m_list.size()) 
        {
           return false;
        }
    
        getImpl()->m_list[index] = str;
        return true;
    }
    
    int List::size() const
    {
        if ( getImpl() )
        {
            return static_cast<int>( getImpl()->m_list.size() );
        }
    
        return 0;
    }
}