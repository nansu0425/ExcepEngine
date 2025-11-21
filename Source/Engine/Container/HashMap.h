#pragma once
#include "Core/Types.h"
#include "Container/DynamicArray.h"
#include <unordered_map>
#include <initializer_list>
#include <utility>

namespace Excep
{
namespace Container
{

/// @brief std::unordered_map을 래핑한 해시 기반 Key-Value 맵 컨테이너
/// @tparam K Key 타입
/// @tparam V Value 타입
template<typename K, typename V>
class HashMap
{
public:
    using Iterator = typename std::unordered_map<K, V>::iterator;
    using ConstIterator = typename std::unordered_map<K, V>::const_iterator;
    using Pair = std::pair<K, V>;

    /// @brief 기본 생성자
    HashMap()
    {
    }

    /// @brief 초기화 리스트로 생성
    /// @param initList 초기화 리스트
    HashMap(std::initializer_list<Pair> initList)
        : m_data(initList)
    {
    }

    /// @brief Key-Value 쌍 삽입
    /// @param key 키
    /// @param value 값
    /// @return 삽입 성공 시 true, 이미 존재하면 false
    bool8 Insert(const K& key, const V& value)
    {
        auto result = m_data.insert({key, value});
        return result.second;
    }

    /// @brief Key로 요소 제거
    /// @param key 제거할 키
    /// @return 제거 성공 시 true, 키가 없으면 false
    bool8 Remove(const K& key)
    {
        return m_data.erase(key) > 0;
    }

    /// @brief Key로 Value 찾기
    /// @param key 찾을 키
    /// @return Value의 포인터 (없으면 nullptr)
    V* Find(const K& key)
    {
        auto it = m_data.find(key);
        if (it != m_data.end())
        {
            return &(it->second);
        }
        return nullptr;
    }

    /// @brief Key로 Value 찾기 (const 버전)
    /// @param key 찾을 키
    /// @return Value의 const 포인터 (없으면 nullptr)
    const V* Find(const K& key) const
    {
        auto it = m_data.find(key);
        if (it != m_data.end())
        {
            return &(it->second);
        }
        return nullptr;
    }

    /// @brief Key 존재 여부 확인
    /// @param key 확인할 키
    /// @return 존재하면 true, 없으면 false
    bool8 Contains(const K& key) const
    {
        return m_data.find(key) != m_data.end();
    }

    /// @brief Key로 Value 접근 (없으면 생성)
    /// @param key 키
    /// @return Value의 참조
    V& operator[](const K& key)
    {
        return m_data[key];
    }

    /// @brief Key로 Value 접근 (경계 검사, 없으면 예외)
    /// @param key 키
    /// @return Value의 참조
    V& GetAt(const K& key)
    {
        return m_data.at(key);
    }

    /// @brief Key로 Value 접근 (경계 검사, 없으면 예외, const 버전)
    /// @param key 키
    /// @return Value의 const 참조
    const V& GetAt(const K& key) const
    {
        return m_data.at(key);
    }

    /// @brief 맵의 모든 요소 제거
    void Clear()
    {
        m_data.clear();
    }

    /// @brief 맵의 요소 개수 반환
    /// @return 요소 개수
    uint64 GetSize() const
    {
        return static_cast<uint64>(m_data.size());
    }

    /// @brief 맵이 비어있는지 확인
    /// @return 비어있으면 true, 아니면 false
    bool8 IsEmpty() const
    {
        return m_data.empty();
    }

    /// @brief 해시맵의 버킷 용량 예약
    /// @param capacity 예약할 요소 개수
    void Reserve(uint64 capacity)
    {
        m_data.reserve(capacity);
    }

    /// @brief 모든 Key를 배열로 반환
    /// @return Key들의 DynamicArray
    DynamicArray<K> GetKeys() const
    {
        DynamicArray<K> keys;
        keys.Reserve(GetSize());
        for (const auto& pair : m_data)
        {
            keys.Add(pair.first);
        }
        return keys;
    }

    /// @brief 모든 Value를 배열로 반환
    /// @return Value들의 DynamicArray
    DynamicArray<V> GetValues() const
    {
        DynamicArray<V> values;
        values.Reserve(GetSize());
        for (const auto& pair : m_data)
        {
            values.Add(pair.second);
        }
        return values;
    }

    /// @brief 맵의 시작 반복자 반환
    /// @return 시작 반복자
    Iterator Begin()
    {
        return m_data.begin();
    }

    /// @brief 맵의 시작 반복자 반환 (const 버전)
    /// @return const 시작 반복자
    ConstIterator Begin() const
    {
        return m_data.begin();
    }

    /// @brief 맵의 끝 반복자 반환
    /// @return 끝 반복자
    Iterator End()
    {
        return m_data.end();
    }

    /// @brief 맵의 끝 반복자 반환 (const 버전)
    /// @return const 끝 반복자
    ConstIterator End() const
    {
        return m_data.end();
    }

    // Range-based for loop 지원
    Iterator begin() { return m_data.begin(); }
    ConstIterator begin() const { return m_data.begin(); }
    Iterator end() { return m_data.end(); }
    ConstIterator end() const { return m_data.end(); }

private:
    std::unordered_map<K, V> m_data;
};

} // namespace Container
} // namespace Excep
