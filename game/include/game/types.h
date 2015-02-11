#ifndef __TYPES_INCLUDED__
#define __TYPES_INCLUDED__

#include <utility>

namespace game
{
    struct point
    {
        point() : x(0), y(0) {}
        point(int x, int y) : x(x), y(y) {}

        void swap(point & p)
        {
            std::swap(x, p.x);
            std::swap(y, p.y);
        }

        int x, y;
    };

    inline bool operator == (const point &l, const point&r)
    {
        return l.x == r.x && l.y == r.y;
    }
    inline bool operator != (const point &l, const point&r)
    {
        return !(l == r);
    }

    inline void swap(point &l, point &r)
    {
        l.swap(r);
    }
    
    struct size
    {
        size() : cx(0), cy(0) {}
        size(int cx, int cy) : cx(cx), cy(cy) {}

        void swap(size & sz)
        {
            std::swap(cx, sz.cx);
            std::swap(cy, sz.cy);
        }

        bool empty() const
        {
            return cx == 0 && cy == 0;
        }

        int cx, cy;
    };

    inline bool operator == (const size &l, const size&r)
    {
        return l.cx == r.cx && l.cy == r.cy;
    }
    inline bool operator != (const size &l, const size&r)
    {
        return !(l == r);
    }
    
    inline void swap(size &l, size &r)
    {
        l.swap(r);
    }
}

#endif // __PROPERTY_SET_INCLUDED__