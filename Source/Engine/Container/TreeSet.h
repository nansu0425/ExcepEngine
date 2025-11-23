#pragma once
#include "Core/Types.h"
#include "Container/DynamicArray.h"
#include <set>
#include <initializer_list>

namespace Excep
{

/// @brief std::set을 래핑한 정렬된 unique 요소 집합 컨테이너
/// @tparam T 저장할 요소의 타입
template<typename T>
class TreeSet
{
public:
    using Iterator = typename std::set<T>::iterator;
    using ConstIterator = typename std::set<T>::const_iterator;

    /// @brief 기본 생성자
    TreeSet()
    {
    }

    /// @brief 초기화 리스트로 생성
    /// @param initList 초기화 리스트
    TreeSet(std::initializer_list<T> initList)
        : m_data(initList)
    {
    }

    /// @brief 요소 삽입
    /// @param value 삽입할 요소
    /// @return 삽입 성공 시 true, 이미 존재하면 false
    bool8 Insert(const T& value)
    {
        auto result = m_data.insert(value);
        return result.second;
    }

    /// @brief 요소 제거
    /// @param value 제거할 요소
    /// @return 제거 성공 시 true, 요소가 없으면 false
    bool8 Remove(const T& value)
    {
        return m_data.erase(value) > 0;
    }

    /// @brief 요소 존재 여부 확인
    /// @param value 확인할 요소
    /// @return 존재하면 true, 없으면 false
    bool8 Contains(const T& value) const
    {
        return m_data.find(value) != m_data.end();
    }

    /// @brief 집합의 모든 요소 제거
    void Clear()
    {
        m_data.clear();
    }

    /// @brief 집합의 요소 개수 반환
    /// @return 요소 개수
    uint64 GetSize() const
    {
        return static_cast<uint64>(m_data.size());
    }

    /// @brief 집합이 비어있는지 확인
    /// @return 비어있으면 true, 아니면 false
    bool8 IsEmpty() const
    {
        return m_data.empty();
    }

    /// @brief 모든 요소를 배열로 반환
    /// @return 요소들의 DynamicArray (정렬된 순서)
    DynamicArray<T> ToArray() const
    {
        DynamicArray<T> array;
        array.Reserve(GetSize());
        for (const auto& value : m_data)
        {
            array.Add(value);
        }
        return array;
    }

    /// @brief 집합의 시작 반복자 반환
    /// @return 시작 반복자
    Iterator Begin()
    {
        return m_data.begin();
    }

    /// @brief 집합의 시작 반복자 반환 (const 버전)
    /// @return const 시작 반복자
    ConstIterator Begin() const
    {
        return m_data.begin();
    }

    /// @brief 집합의 끝 반복자 반환
    /// @return 끝 반복자
    Iterator End()
    {
        return m_data.end();
    }

    /// @brief 집합의 끝 반복자 반환 (const 버전)
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
    std::set<T> m_data;
};

} // namespace Excep
