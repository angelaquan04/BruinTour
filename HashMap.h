//
//  HashMap.h
//  32project4
//
//  Created by Angela Quan on 3/8/24.
//


#include <vector>
#include <list>
#include <string>
#include <functional>
#include <iostream>
#ifndef HashMap_h
#define HashMap_h

template <typename T>
class HashMap
{
public:
    HashMap(double max_load = 0.75);
    ~HashMap();
    int size() const;
    void insert(const std::string& key, const T& value);
    T& operator[](const std::string& key);
    const T* find(const std::string& key) const ;
    T* find(const std::string& key);

private:
    struct Node
      {
          Node(std::string _key , T _value)
          : key(_key), value(_value)
          {}
          std::string key;
          T value;
      };
    
    double m_maxLoad;
    int m_buckets;
    int m_size;
    std::vector<std::list<Node>*> m_hash;
    
    void resize();
    int hash(const std::string& key) const;
};
    

//IMPLEMENTATION

template <typename T>
HashMap<T> :: HashMap(double max_load) : m_maxLoad(max_load), m_buckets(10), m_size(0) // constructor
{
       for (int i = 0; i < m_buckets; i++)
           m_hash.push_back(new std::list<Node>());
}

template <typename T>
HashMap<T> :: ~HashMap() // destructor; deletes all of the items in the hashmap
{
    auto p = m_hash.begin();
    while (p != m_hash.end())
    {
        delete *p;
        p = m_hash.erase(p);
    }
}

template <typename T>
int HashMap<T>::size() const
        {return m_size; }

    template <typename T>
    void HashMap<T>::insert(const std::string& key, const T& value)
{
        T* found = find(key);
        if (found != nullptr)
            *found = value;
        else
        {
            m_size++;
            //std::cerr << ((double)m_size/m_buckets) << std::endl;
            if (((double)m_size / m_buckets) > m_maxLoad)
                {resize();}
            m_hash[hash(key)]->push_back(Node(key,value));
        }
}


    template <typename T>
    T& HashMap<T>::operator[](const std::string& key)
    {
        if (find(key) == nullptr)
            insert(key, T());
        return *find(key);
    }


    template <typename T>
    const T* HashMap<T>::find(const std::string& key) const
{
    for (auto p = m_hash[hash(key)]->begin(); p != m_hash[hash(key)]->end(); p++) {
        if (p->key == key)
            return &p->value;
    }
    return nullptr;
}

template <typename T>
void HashMap<T> :: resize()
{
    m_buckets *= 2;
    std::vector<std::list<Node>*> newHash;
    
    for (int i = 0; i < m_buckets; i++)
        newHash.push_back(new std::list<Node>());
    
    for (int i = 0; i < m_hash.size(); i++)
    {
        for (auto it = m_hash[i]->begin(); it!= m_hash[i]->end(); it++)
            newHash[hash(it->key)]->push_back(*it);
    }
    //delete m_hash memory maybe delete old vector
    auto p = m_hash.begin();
    while (p != m_hash.end())
    {
        delete *p;
        p = m_hash.erase(p);
    }
    m_hash = newHash;
}

template <typename T>
int HashMap<T> :: hash(const std::string& key) const
{
    return std::hash<std::string>()(key) % m_buckets;
}

    template <typename T>
T* HashMap<T>::find(const std::string& key) {
    const auto& hm = *this;
    return const_cast<T*>(hm.find(key));
}
#endif /* HashMap_h */

