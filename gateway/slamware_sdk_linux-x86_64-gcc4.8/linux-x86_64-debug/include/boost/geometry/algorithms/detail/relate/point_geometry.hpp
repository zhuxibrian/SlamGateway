// Boost.Geometry (aka GGL, Generic Geometry Library)

// Copyright (c) 2007-2012 Barend Gehrels, Amsterdam, the Netherlands.

// This file was modified by Oracle on 2013, 2014.
// Modifications copyright (c) 2013, 2014, Oracle and/or its affiliates.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// Contributed and/or modified by Adam Wulkiewicz, on behalf of Oracle

#ifndef BOOST_GEOMETRY_ALGORITHMS_DETAIL_RELATE_POINT_GEOMETRY_HPP
#define BOOST_GEOMETRY_ALGORITHMS_DETAIL_RELATE_POINT_GEOMETRY_HPP

#include <boost/geometry/algorithms/detail/within/point_in_geometry.hpp>
//#include <boost/geometry/algorithms/within.hpp>
//#include <boost/geometry/algorithms/covered_by.hpp>

#include <boost/geometry/algorithms/detail/relate/topology_check.hpp>

namespace boost { namespace geometry
{

#ifndef DOXYGEN_NO_DETAIL
namespace detail { namespace relate {

// non-point geometry
template <typename Point, typename Geometry, bool Transpose = false>
struct point_geometry
{
    // TODO: interrupt only if the topology check is complex

    static const bool interruption_enabled = true;

    template <typename Result>
    static inline void apply(Point const& point, Geometry const& geometry, Result & result)
    {
        int pig = detail::within::point_in_geometry(point, geometry);

        if ( pig > 0 ) // within
        {
            set<interior, interior, '0', Transpose>(result);
        }
        else if ( pig == 0 )
        {
            set<interior, boundary, '0', Transpose>(result);
        }
        else // pig < 0 - not within
        {
            set<interior, exterior, '0', Transpose>(result);
        }

        set<exterior, exterior, result_dimension<Point>::value, Transpose>(result);

        if ( result.interrupt )
            return;

        // the point is on the boundary
        if ( pig == 0 )
        {
            // NOTE: even for MLs, if there is at least one boundary point,
            // somewhere there must be another one

            // check if there are other boundaries outside
            typedef detail::relate::topology_check<Geometry> tc_t;
            //tc_t tc(geometry, point);
            //if ( tc.has_interior )
                set<exterior, interior, tc_t::interior, Transpose>(result);
            //if ( tc.has_boundary )
                set<exterior, boundary, tc_t::boundary, Transpose>(result);
        }
        else
        {
            // check if there is a boundary in Geometry
            typedef detail::relate::topology_check<Geometry> tc_t;
            tc_t tc(geometry);
            if ( tc.has_interior )
                set<exterior, interior, tc_t::interior, Transpose>(result);
            if ( tc.has_boundary )
                set<exterior, boundary, tc_t::boundary, Transpose>(result);
        }
    }
};

// transposed result of point_geometry
template <typename Geometry, typename Point>
struct geometry_point
{
    // TODO: interrupt only if the topology check is complex

    static const bool interruption_enabled = true;

    template <typename Result>
    static inline void apply(Geometry const& geometry, Point const& point, Result & result)
    {
        point_geometry<Point, Geometry, true>::apply(point, geometry, result);
    }
};

// TODO: rewrite the folowing:

//// NOTE: Those tests should be consistent with within(Point, Box) and covered_by(Point, Box)
//// There is no EPS used in those functions, values are compared using < or <=
//// so comparing MIN and MAX in the same way should be fine
//
//template <typename Box, std::size_t I = 0, std::size_t D = geometry::dimension<Box>::value>
//struct box_has_interior
//{
//    static inline bool apply(Box const& box)
//    {
//        return geometry::get<min_corner, I>(box) < geometry::get<max_corner, I>(box)
//            && box_has_interior<Box, I + 1, D>::apply(box);
//    }
//};
//
//template <typename Box, std::size_t D>
//struct box_has_interior<Box, D, D>
//{
//    static inline bool apply(Box const&) { return true; }
//};
//
//// NOTE: especially important here (see the NOTE above).
//
//template <typename Box, std::size_t I = 0, std::size_t D = geometry::dimension<Box>::value>
//struct box_has_equal_min_max
//{
//    static inline bool apply(Box const& box)
//    {
//        return geometry::get<min_corner, I>(box) == geometry::get<max_corner, I>(box)
//            && box_has_equal_min_max<Box, I + 1, D>::apply(box);
//    }
//};
//
//template <typename Box, std::size_t D>
//struct box_has_equal_min_max<Box, D, D>
//{
//    static inline bool apply(Box const&) { return true; }
//};
//
//template <typename Point, typename Box>
//struct point_box
//{
//    static inline result apply(Point const& point, Box const& box)
//    {
//        result res;
//
//        if ( geometry::within(point, box) ) // this also means that the box has interior
//        {
//            return result("0FFFFFTTT");
//        }
//        else if ( geometry::covered_by(point, box) ) // point is on the boundary
//        {
//            //if ( box_has_interior<Box>::apply(box) )
//            //{
//            //    return result("F0FFFFTTT");
//            //}
//            //else if ( box_has_equal_min_max<Box>::apply(box) ) // no boundary outside point
//            //{
//            //    return result("F0FFFFFFT");
//            //}
//            //else // no interior outside point
//            //{
//            //    return result("F0FFFFFTT");
//            //}
//            return result("F0FFFF**T");
//        }
//        else 
//        {
//            /*if ( box_has_interior<Box>::apply(box) )
//            {
//                return result("FF0FFFTTT");
//            }
//            else
//            {
//                return result("FF0FFFFTT");
//            }*/
//            return result("FF0FFF*TT");
//        }
//
//        return res;
//    }
//};
//
//template <typename Box, typename Point>
//struct box_point
//{
//    static inline result apply(Box const& box, Point const& point)
//    {
//        if ( geometry::within(point, box) )
//            return result("0FTFFTFFT");
//        else if ( geometry::covered_by(point, box) )
//            return result("FF*0F*FFT");
//        else 
//            return result("FF*FFT0FT");
//    }
//};

}} // namespace detail::relate
#endif // DOXYGEN_NO_DETAIL

}} // namespace boost::geometry

#endif // BOOST_GEOMETRY_ALGORITHMS_DETAIL_RELATE_POINT_GEOMETRY_HPP
