#include "Core/Pch.h"
#include "World/CTransform.h"

namespace Excep
{

CTransform::CTransform()
    : m_position(0.0f, 0.0f, 0.0f)
    , m_rotation(0.0f, 0.0f, 0.0f)
    , m_scale(1.0f, 1.0f, 1.0f)
{
}

} // namespace Excep
