/*
 * Dawn Engine
 * Written by David Avedissian (c) 2012-2015 (avedissian.david@gmail.com)
 */
#include "Common.h"
#include "Rect.h"

NAMESPACE_BEGIN

Rect::Rect() : begin(), end()
{
}

Rect::Rect(int x1, int y1, int x2, int y2) : begin(x1, y1), end(x2, y2)
{
}

Rect::Rect(const Vec2i& _begin, const Vec2i& _end) : begin(_begin), end(_end)
{
}

Rect& Rect::Merge(const Rect& other)
{
    begin.x = math::Min(begin.x, other.begin.x);
    begin.y = math::Min(begin.y, other.begin.y);
    end.x = math::Max(end.x, other.end.x);
    end.y = math::Max(end.y, other.end.y);
    return *this;
}

NAMESPACE_END