#include "Core/Pch.h"
#include "World/WObject.h"

namespace Excep
{
namespace World
{

WObject::WObject()
{
}

void WObject::Update()
{
    for (uint64 i = 0; i < m_components.GetSize(); ++i)
    {
        m_components[i]->Update();
    }
}

} // namespace World
} // namespace Excep
