#pragma once
#include "Core/Types.h"
#include <string>
#include <algorithm>

namespace Excep
{

/// @brief std::string을 래핑한 UTF-8 문자열 컨테이너
class String8
{
public:
    using Iterator = std::string::iterator;
    using ConstIterator = std::string::const_iterator;

    /// @brief 기본 생성자 (빈 문자열)
    String8()
    {
    }

    /// @brief C 스타일 문자열로 생성
    /// @param str C 스타일 문자열
    String8(const char8* str)
        : m_data(str)
    {
    }

    /// @brief std::string으로 생성
    /// @param str std::string
    String8(const std::string& str)
        : m_data(str)
    {
    }

    /// @brief 문자열 길이 반환
    /// @return 문자열 길이 (문자 개수)
    uint64 GetLength() const
    {
        return static_cast<uint64>(m_data.length());
    }

    /// @brief 문자열 크기 반환 (GetLength와 동일)
    /// @return 문자열 크기
    uint64 GetSize() const
    {
        return GetLength();
    }

    /// @brief 문자열이 비어있는지 확인
    /// @return 비어있으면 true, 아니면 false
    bool8 IsEmpty() const
    {
        return m_data.empty();
    }

    /// @brief 문자열의 모든 문자 제거
    void Clear()
    {
        m_data.clear();
    }

    /// @brief 문자열 용량 예약
    /// @param capacity 예약할 용량
    void Reserve(uint64 capacity)
    {
        m_data.reserve(capacity);
    }

    /// @brief 인덱스로 문자 접근 (경계 검사 없음)
    /// @param index 문자의 인덱스
    /// @return 문자의 참조
    char8& operator[](uint64 index)
    {
        return m_data[index];
    }

    /// @brief 인덱스로 문자 접근 (경계 검사 없음, const 버전)
    /// @param index 문자의 인덱스
    /// @return 문자의 const 참조
    const char8& operator[](uint64 index) const
    {
        return m_data[index];
    }

    /// @brief 인덱스로 문자 접근 (경계 검사 수행)
    /// @param index 문자의 인덱스
    /// @return 문자의 참조
    char8& GetAt(uint64 index)
    {
        return m_data.at(index);
    }

    /// @brief 인덱스로 문자 접근 (경계 검사 수행, const 버전)
    /// @param index 문자의 인덱스
    /// @return 문자의 const 참조
    const char8& GetAt(uint64 index) const
    {
        return m_data.at(index);
    }

    /// @brief C 스타일 문자열 포인터 반환
    /// @return null-terminated C 문자열 포인터
    const char8* GetCString() const
    {
        return m_data.c_str();
    }

    /// @brief 문자열 끝에 추가
    /// @param str 추가할 문자열
    void Append(const String8& str)
    {
        m_data.append(str.m_data);
    }

    /// @brief 문자열 끝에 추가 (operator+=)
    /// @param str 추가할 문자열
    /// @return 자기 자신의 참조
    String8& operator+=(const String8& str)
    {
        m_data += str.m_data;
        return *this;
    }

    /// @brief 문자열 연결 (operator+)
    /// @param str 연결할 문자열
    /// @return 연결된 새로운 문자열
    String8 operator+(const String8& str) const
    {
        return String8(m_data + str.m_data);
    }

    /// @brief 특정 위치에 문자열 삽입
    /// @param index 삽입할 위치
    /// @param str 삽입할 문자열
    void Insert(uint64 index, const String8& str)
    {
        m_data.insert(index, str.m_data);
    }

    /// @brief 특정 범위의 문자 제거
    /// @param start 시작 인덱스
    /// @param length 제거할 길이
    void Remove(uint64 start, uint64 length)
    {
        m_data.erase(start, length);
    }

    /// @brief 문자열 내에서 특정 문자열 찾기
    /// @param str 찾을 문자열
    /// @return 찾은 위치 인덱스 (없으면 UINT64_MAX)
    uint64 Find(const String8& str) const
    {
        size_t pos = m_data.find(str.m_data);
        return (pos != std::string::npos) ? static_cast<uint64>(pos) : UINT64_MAX;
    }

    /// @brief 문자열 내에서 주어진 문자들 중 하나가 마지막으로 등장하는 위치 찾기
    /// @param str 찾을 문자들
    /// @return 찾은 위치 인덱스 (없으면 UINT64_MAX)
    uint64 FindLastOf(const String8& str) const
    {
        size_t pos = m_data.find_last_of(str.m_data);
        return (pos != std::string::npos) ? static_cast<uint64>(pos) : UINT64_MAX;
    }

    /// @brief 문자열 포함 여부 확인
    /// @param str 확인할 문자열
    /// @return 포함하면 true, 아니면 false
    bool8 Contains(const String8& str) const
    {
        return m_data.find(str.m_data) != std::string::npos;
    }

    /// @brief 특정 문자열로 시작하는지 확인
    /// @param str 확인할 문자열
    /// @return 시작하면 true, 아니면 false
    bool8 StartsWith(const String8& str) const
    {
        return m_data.rfind(str.m_data, 0) == 0;
    }

    /// @brief 특정 문자열로 끝나는지 확인
    /// @param str 확인할 문자열
    /// @return 끝나면 true, 아니면 false
    bool8 EndsWith(const String8& str) const
    {
        if (str.GetLength() > GetLength()) return false;
        return m_data.compare(GetLength() - str.GetLength(), str.GetLength(), str.m_data) == 0;
    }

    /// @brief 부분 문자열 반환
    /// @param start 시작 인덱스
    /// @param length 길이
    /// @return 부분 문자열
    String8 Substring(uint64 start, uint64 length) const
    {
        return String8(m_data.substr(start, length));
    }

    /// @brief 대문자로 변환
    /// @return 대문자로 변환된 문자열
    String8 ToUpper() const
    {
        std::string result = m_data;
        std::transform(result.begin(), result.end(), result.begin(), ::toupper);
        return String8(result);
    }

    /// @brief 소문자로 변환
    /// @return 소문자로 변환된 문자열
    String8 ToLower() const
    {
        std::string result = m_data;
        std::transform(result.begin(), result.end(), result.begin(), ::tolower);
        return String8(result);
    }

    /// @brief 문자열 비교 (동등)
    /// @param other 비교할 문자열
    /// @return 같으면 true, 아니면 false
    bool8 operator==(const String8& other) const
    {
        return m_data == other.m_data;
    }

    /// @brief 문자열 비교 (부등)
    /// @param other 비교할 문자열
    /// @return 다르면 true, 아니면 false
    bool8 operator!=(const String8& other) const
    {
        return m_data != other.m_data;
    }

    /// @brief 문자열 사전순 비교
    /// @param other 비교할 문자열
    /// @return 음수: this < other, 0: 같음, 양수: this > other
    int32 Compare(const String8& other) const
    {
        return static_cast<int32>(m_data.compare(other.m_data));
    }

    /// @brief 문자열의 시작 반복자 반환
    /// @return 시작 반복자
    Iterator Begin()
    {
        return m_data.begin();
    }

    /// @brief 문자열의 시작 반복자 반환 (const 버전)
    /// @return const 시작 반복자
    ConstIterator Begin() const
    {
        return m_data.begin();
    }

    /// @brief 문자열의 끝 반복자 반환
    /// @return 끝 반복자
    Iterator End()
    {
        return m_data.end();
    }

    /// @brief 문자열의 끝 반복자 반환 (const 버전)
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
    std::string m_data;
};

} // namespace Excep
