#pragma once
#include "Core/Types.h"
#include <vector>
#include <initializer_list>

namespace Excep
{
namespace Container
{

/// @brief std::vector를 래핑한 동적 배열 컨테이너
/// @tparam T 저장할 요소의 타입
template<typename T>
class DynamicArray
{
public:
    using Iterator = typename std::vector<T>::iterator;
    using ConstIterator = typename std::vector<T>::const_iterator;

    /// @brief 기본 생성자
    DynamicArray()
    {
    }

    /// @brief 크기를 지정하여 생성
    /// @param size 초기 크기
    explicit DynamicArray(uint64 size)
        : m_data(size)
    {
    }

    /// @brief 크기와 초기값을 지정하여 생성
    /// @param size 초기 크기
    /// @param value 초기값
    DynamicArray(uint64 size, const T& value)
        : m_data(size, value)
    {
    }

    /// @brief 초기화 리스트로 생성
    /// @param initList 초기화 리스트
    DynamicArray(std::initializer_list<T> initList)
        : m_data(initList)
    {
    }

    /// @brief 배열 끝에 요소 추가
    /// @param value 추가할 요소
    void Add(const T& value)
    {
        m_data.push_back(value);
    }

    /// @brief 배열 끝에 요소 추가 (Add와 동일)
    /// @param value 추가할 요소
    void Push(const T& value)
    {
        m_data.push_back(value);
    }

    /// @brief 배열 끝에 요소 추가 (move semantics)
    /// @param value 추가할 요소
    void Add(T&& value)
    {
        m_data.push_back(std::move(value));
    }

    /// @brief 배열 끝에 요소 추가 (move semantics, Push와 동일)
    /// @param value 추가할 요소
    void Push(T&& value)
    {
        m_data.push_back(std::move(value));
    }

    /// @brief 특정 값을 가진 첫 번째 요소 제거
    /// @param value 제거할 값
    /// @return 제거 성공 시 true, 값을 찾지 못한 경우 false
    bool8 Remove(const T& value)
    {
        auto it = std::find(m_data.begin(), m_data.end(), value);
        if (it != m_data.end())
        {
            m_data.erase(it);
            return true;
        }
        return false;
    }

    /// @brief 특정 인덱스의 요소 제거
    /// @param index 제거할 요소의 인덱스
    void RemoveAt(uint64 index)
    {
        m_data.erase(m_data.begin() + index);
    }

    /// @brief 마지막 요소 제거
    void Pop()
    {
        m_data.pop_back();
    }

    /// @brief 배열의 모든 요소 제거
    void Clear()
    {
        m_data.clear();
    }

    /// @brief 배열의 요소 개수 반환
    /// @return 요소 개수
    uint64 Size() const
    {
        return static_cast<uint64>(m_data.size());
    }

    /// @brief 배열이 비어있는지 확인
    /// @return 비어있으면 true, 아니면 false
    bool8 IsEmpty() const
    {
        return m_data.empty();
    }

    /// @brief 배열의 용량 예약
    /// @param capacity 예약할 용량
    void Reserve(uint64 capacity)
    {
        m_data.reserve(capacity);
    }

    /// @brief 배열의 크기 변경
    /// @param newSize 새로운 크기
    void Resize(uint64 newSize)
    {
        m_data.resize(newSize);
    }

    /// @brief 배열의 크기 변경 (기본값 지정)
    /// @param newSize 새로운 크기
    /// @param value 새로 추가되는 요소의 기본값
    void Resize(uint64 newSize, const T& value)
    {
        m_data.resize(newSize, value);
    }

    /// @brief 인덱스로 요소 접근 (경계 검사 없음)
    /// @param index 요소의 인덱스
    /// @return 요소의 참조
    T& operator[](uint64 index)
    {
        return m_data[index];
    }

    /// @brief 인덱스로 요소 접근 (경계 검사 없음, const 버전)
    /// @param index 요소의 인덱스
    /// @return 요소의 const 참조
    const T& operator[](uint64 index) const
    {
        return m_data[index];
    }

    /// @brief 인덱스로 요소 접근 (경계 검사 수행)
    /// @param index 요소의 인덱스
    /// @return 요소의 참조
    T& At(uint64 index)
    {
        return m_data.at(index);
    }

    /// @brief 인덱스로 요소 접근 (경계 검사 수행, const 버전)
    /// @param index 요소의 인덱스
    /// @return 요소의 const 참조
    const T& At(uint64 index) const
    {
        return m_data.at(index);
    }

    /// @brief 배열의 원시 데이터 포인터 반환
    /// @return 배열의 첫 번째 요소를 가리키는 포인터
    T* Data()
    {
        return m_data.data();
    }

    /// @brief 배열의 원시 데이터 포인터 반환 (const 버전)
    /// @return 배열의 첫 번째 요소를 가리키는 const 포인터
    const T* Data() const
    {
        return m_data.data();
    }

    /// @brief 배열의 시작 반복자 반환
    /// @return 시작 반복자
    Iterator Begin()
    {
        return m_data.begin();
    }

    /// @brief 배열의 시작 반복자 반환 (const 버전)
    /// @return const 시작 반복자
    ConstIterator Begin() const
    {
        return m_data.begin();
    }

    /// @brief 배열의 끝 반복자 반환
    /// @return 끝 반복자
    Iterator End()
    {
        return m_data.end();
    }

    /// @brief 배열의 끝 반복자 반환 (const 버전)
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
    std::vector<T> m_data;
};

} // namespace Container
} // namespace Excep
