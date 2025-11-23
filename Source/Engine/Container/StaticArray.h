#pragma once
#include "Core/Types.h"
#include <array>
#include <initializer_list>

namespace Excep
{

/// @brief std::array를 래핑한 고정 크기 배열 컨테이너
/// @tparam T 저장할 요소의 타입
/// @tparam N 배열의 크기
template<typename T, uint64 N>
class StaticArray
{
public:
    using Iterator = typename std::array<T, N>::iterator;
    using ConstIterator = typename std::array<T, N>::const_iterator;

    /// @brief 기본 생성자
    StaticArray()
    {
    }

    /// @brief 초기화 리스트로 생성
    /// @param initList 초기화 리스트 (크기가 N보다 작으면 나머지는 기본값)
    StaticArray(std::initializer_list<T> initList)
    {
        uint64 i = 0;
        for (const auto& value : initList)
        {
            if (i >= N) break;
            m_data[i++] = value;
        }
    }

    /// @brief 배열의 모든 요소를 같은 값으로 채움
    /// @param value 채울 값
    void Fill(const T& value)
    {
        m_data.fill(value);
    }

    /// @brief 배열의 크기 반환
    /// @return 배열의 크기 (컴파일 타임 상수)
    constexpr uint64 GetSize() const
    {
        return N;
    }

    /// @brief 배열이 비어있는지 확인
    /// @return N이 0이면 true, 아니면 false
    constexpr bool8 IsEmpty() const
    {
        return N == 0;
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
    T& GetAt(uint64 index)
    {
        return m_data.at(index);
    }

    /// @brief 인덱스로 요소 접근 (경계 검사 수행, const 버전)
    /// @param index 요소의 인덱스
    /// @return 요소의 const 참조
    const T& GetAt(uint64 index) const
    {
        return m_data.at(index);
    }

    /// @brief 배열의 첫 번째 요소 참조 반환
    /// @return 첫 번째 요소의 참조
    T& GetFront()
    {
        return m_data.front();
    }

    /// @brief 배열의 첫 번째 요소 참조 반환 (const 버전)
    /// @return 첫 번째 요소의 const 참조
    const T& GetFront() const
    {
        return m_data.front();
    }

    /// @brief 배열의 마지막 요소 참조 반환
    /// @return 마지막 요소의 참조
    T& GetBack()
    {
        return m_data.back();
    }

    /// @brief 배열의 마지막 요소 참조 반환 (const 버전)
    /// @return 마지막 요소의 const 참조
    const T& GetBack() const
    {
        return m_data.back();
    }

    /// @brief 배열의 원시 데이터 포인터 반환
    /// @return 배열의 첫 번째 요소를 가리키는 포인터
    T* GetData()
    {
        return m_data.data();
    }

    /// @brief 배열의 원시 데이터 포인터 반환 (const 버전)
    /// @return 배열의 첫 번째 요소를 가리키는 const 포인터
    const T* GetData() const
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
    std::array<T, N> m_data;
};

} // namespace Excep
