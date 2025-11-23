#include "Core/Pch.h"
#include "World/WObject.h"
#include "World/CTransform.h"

namespace Excep
{

WObject::WObject()
{
    // 모든 WObject는 기본적으로 Transform을 가집니다
    m_transform = AddComponent<CTransform>();
}

void WObject::Update()
{
    for (uint64 i = 0; i < m_components.GetSize(); ++i)
    {
        m_components[i]->Update();
    }
}

} // namespace Excep
