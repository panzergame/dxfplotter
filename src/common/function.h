#pragma once

namespace common
{

template <typename Func>
struct MemberFunctionTraits;

template <class _Object, typename _Return, typename ...Args>
struct MemberFunctionTraits<_Return (_Object::*)(Args ...)>
{
	using Object = _Object;
	using Return = _Return;
};

template <class _Object, typename _Return, typename ...Args>
struct MemberFunctionTraits<_Return (_Object::*)(Args ...) const>
{
	using Object = _Object;
	using Return = _Return;
};

}
